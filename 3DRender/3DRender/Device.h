/*
*��Ⱦװ��
*/
#ifndef DEVICE_H
#define DEVICE_H
#include "3DMath.h"
#include "Camera.h"
#include "Define/GeometryDefine.h"

const int RENDER_STATE_WIREFRAME = 1;	// ��Ⱦ�߿�
const int RENDER_STATE_COLOR = 2;		// ��Ⱦ��ɫ
const int RENDER_STATE_TEXTURE = 4;		// ��Ⱦ����

class Device
{
public:
	//��Ⱦ������
	void Render()
	{
		Clear();
		LocalToWorld();
		//RemoveBackfaceTriangle();
		WorldToCamera();
		Projection();
		ViewTransform();
		RenderObject();
		/*Triangle t({200, 400, 200}, { 400, 400, 200 }, { 300, 100, 200 }, 0x00FF0000, 0x0000FF00, 0x000000FF, true);
		DrawTriangle(t);*/
	}
	void Clear()
	{
		memset(frameBuffer, 99, sizeof(UINT)* width * height);
		memset(depthBuffer, 0, sizeof(double)* width * height);
	}
public:
	Device(UVNCamera *camera, int width, int height, void *frameBuffer, int renderState, UINT foreground, UINT background)
	{
		Init(camera, width, height, frameBuffer, renderState, foreground, background);
	}
	~Device()
	{

	}
	//���òü�����
	void SetClip(int clipMinX, int clipMinY, int clipMaxX, int clipMaxY)
	{
		this->clipMaxX = clipMaxX;
		this->clipMaxY = clipMaxY;
		this->clipMinX = clipMinX;
		this->clipMinY = clipMinY;
	}
	//����
	void DrawPoint(int x, int y, UINT color)
	{
		if (x < clipMinX || x >= clipMaxX || y < clipMinY || y >= clipMaxY)
		{
			return;
		}
		frameBuffer[(UINT)x + (UINT)y * (UINT)width] = color;
	}
	//����
	void DrawLine(int x1, int y1, int x2, int y2, UINT color)
	{
		int dy = y2 - y1;
		int dx = x2 - x1;
		int x = x1;
		int y = y1;
		if (dx == 0)
		{
			if (y <= y2)
			{
				while (y <= y2)
				{
					DrawPoint(x, y++, color);
				}
			}
			else
			{
				while (y > y2)
				{
					DrawPoint(x, y--, color);
				}
			}
			return;
		}
		double k = (double)dy / (double)dx;
		double b = y1 - k*x1;
		if (abs(k) < 1)
		{
			if (x <= x2)
			{
				while (x <= x2)
				{
					DrawPoint(x, y, color);
					y = (int)(k * ++x + b);
				}
			}
			else
			{
				while (x > x2)
				{
					DrawPoint(x, y, color);
					y = (int)(k * --x + b);
				}
			}
		}
		else
		{
			if (y <= y2)
			{
				while (y <= y2)
				{
					DrawPoint(x, y, color);
					x = (int)((++y - b) / k);
				}
			}
			else
			{
				while (y > y2)
				{
					DrawPoint(x, y, color);
					x = (int)((--y - b) / k);
				}
			}
		}
	}
	void DrawLine(const Point2D &p1, const Point2D &p2, UINT color)
	{
		DrawLine(p1.x, p1.y, p2.x, p2.y, color);
	}
	//��������
	void DrawTriangle(const Point2D &p1, const Point2D &p2, const Point2D &p3, UINT color, int mode)
	{
		//if (RENDER_STATE_WIREFRAME == mode)
		//{
			DrawLine(p1, p2, color);
			DrawLine(p2, p3, color);
			DrawLine(p3, p1, color);
		//}
		//else if (RENDER_STATE_COLOR == mode)
		//{
		//	//�Ƿ���ƽ�׻�ƽ��������
		//	if (p1.y == p2.y)
		//	{
		//		if (p3.y > p2.y)
		//		{
		//			FillBottomTriangle(p1, p2, p3, color);
		//		}
		//		else if (p3.y < p2.y)
		//		{
		//			FillTopTriangle(p1, p2, p3, color);
		//		}
		//	}
		//	else if (p1.y == p3.y)
		//	{
		//		if (p2.y > p1.y)
		//		{
		//			FillBottomTriangle(p1, p2, p3, color);
		//		}
		//		else if (p2.y < p1.y)
		//		{
		//			FillTopTriangle(p1, p2, p3, color);
		//		}
		//	}
		//	else if (p2.y == p3.y)
		//	{
		//		if (p3.y > p2.y)
		//		{
		//			FillBottomTriangle(p1, p2, p3, color);
		//		}
		//		else if (p3.y < p2.y)
		//		{
		//			FillTopTriangle(p1, p2, p3, color);
		//		}
		//	}
		//	//����ƽ��Ҳ����ƽ��,��Ҫ���
		//	else
		//	{
		//		//���ϵ������ж���
		//		Point2D pList[] = { p1, p2, p3 };
		//		SelectionSort(pList, 3);
		//		//(y - b) / k
		//		double k = (pList[0].y - pList[2].y) * 1.0f / (pList[0].x - pList[2].x);
		//		double b = pList[2].y - k*pList[2].x;
		//		int CenterPointX = (int)((pList[1].y - b) / k);
		//		Point2D centerPoint = { CenterPointX, pList[1].y };
		//		FillTopTriangle(pList[0], centerPoint, pList[1], color);
		//		FillBottomTriangle(centerPoint, pList[1], pList[2], color);
		//	}
		//}
	}
	void DrawTriangle(const Triangle &triangle, UINT color, int mode)
	{
		Point2D p1 = { (int)triangle.GetVertex(0).vertex.x, (int)triangle.GetVertex(0).vertex.y };
		Point2D p2 = { (int)triangle.GetVertex(1).vertex.x, (int)triangle.GetVertex(1).vertex.y };
		Point2D p3 = { (int)triangle.GetVertex(2).vertex.x, (int)triangle.GetVertex(2).vertex.y };
		DrawTriangle(p1, p2, p3, color, mode);
	}


	//͸�ӽ�������
	void FillBottomTriangleByTexCorrect(const Triangle &t)
	{
		Vertex3D bottom, left, right;
		Vector2D bottomUV, leftUV, rightUV;
		if (t.GetVertex(0).vertex.y == t.GetVertex(1).vertex.y)
		{
			bottom = t.GetVertex(2);
			bottomUV = t.uv[2];
			left = t.GetVertex(0);
			leftUV = t.uv[0];
			right = t.GetVertex(1);
			rightUV = t.uv[1];
		}
		else if (t.GetVertex(1).vertex.y == t.GetVertex(2).vertex.y)
		{
			bottom = t.GetVertex(0);
			left = t.GetVertex(1);
			right = t.GetVertex(2);
			bottomUV = t.uv[0];
			leftUV = t.uv[1];
			rightUV = t.uv[2];
		}
		else
		{
			bottom = t.GetVertex(1);
			left = t.GetVertex(0);
			right = t.GetVertex(2);
			bottomUV = t.uv[1];
			leftUV = t.uv[0];
			rightUV = t.uv[2];
		}
		if (left.vertex.x > right.vertex.x)
		{
			Swap(left, right);
			Swap(leftUV, rightUV);
		}
		double leftDxDivDy = (bottom.vertex.x - left.vertex.x) / (bottom.vertex.y - left.vertex.y);
		double rightDxDivDy = (right.vertex.x - bottom.vertex.x) / (right.vertex.y - bottom.vertex.y);
		double xleft = left.vertex.x;
		double xRight = right.vertex.x;
		int y = left.vertex.y;

		//1/Z
		double leftDxDivDyReciprocalZ = ((1 / bottom.vertex.z) - (1 / left.vertex.z)) / (bottom.vertex.y - left.vertex.y);
		double rightDxDivDyReciprocalZ = ((1 / right.vertex.z) - (1 / bottom.vertex.z)) / (right.vertex.y - bottom.vertex.y);
		double xLeftReciprocalZ = 1 / left.vertex.z;
		double xRightReciprocalZ = 1 / right.vertex.z;

		//U
		double leftDxDivDyColorU = (bottomUV.x / bottom.vertex.z - leftUV.x / left.vertex.z) / (bottom.vertex.y - left.vertex.y);
		double rightDxDivDyColorU = (rightUV.x / right.vertex.z - bottomUV.x / bottom.vertex.z) / (right.vertex.y - bottom.vertex.y);
		double xLeftColorU = leftUV.x / left.vertex.z;
		double xRightColorU = rightUV.x / right.vertex.z;
		//V
		double leftDxDivDyColorV = (bottomUV.y / bottom.vertex.z - leftUV.y / left.vertex.z) / (bottom.vertex.y - left.vertex.y);
		double rightDxDivDyColorV = (rightUV.y / right.vertex.z - bottomUV.y / bottom.vertex.z) / (right.vertex.y - bottom.vertex.y);
		double xLeftColorV = leftUV.y / left.vertex.z;
		double xRightColorV = rightUV.y / right.vertex.z;

		double texWidth = t.material->width;
		double texHeight = t.material->height;
		UINT *texBuffer = t.material->texBuffer;

		while (y < bottom.vertex.y)
		{
			double dxReciprocalZ = (xRightReciprocalZ - xLeftReciprocalZ) / (xRight - xleft);
			double reciprocalZ = xLeftReciprocalZ;

			double dxColorU = (xRightColorU - xLeftColorU) / (xRight - xleft);
			double ColorStarU = xLeftColorU;

			double dxColorV = (xRightColorV - xLeftColorV) / (xRight - xleft);
			double ColorStarV = xLeftColorV;
			for (int i = (int)(xleft); i < (int)(xRight); i++)
			{

				double u = (ColorStarU / reciprocalZ) * (texWidth - 1);
				double v = (ColorStarV / reciprocalZ) * (texHeight - 1);
				if (u < 0 || v < 0 || u >= texWidth || v >= texHeight)
				{
					int haa = 0;
				}
				if (TestZ(i, y, reciprocalZ))
				{
					UINT color = texBuffer[(UINT)u + (UINT)v * (UINT)texWidth];
					DrawPoint(i, y, color);
				}
				
				ColorStarU += dxColorU;
				ColorStarV += dxColorV;
				reciprocalZ += dxReciprocalZ;
			}
			y++;
			xleft += leftDxDivDy;
			xRight += rightDxDivDy;

			xLeftReciprocalZ += leftDxDivDyReciprocalZ;
			xRightReciprocalZ += rightDxDivDyReciprocalZ;

			xLeftColorU += leftDxDivDyColorU;
			xRightColorU += rightDxDivDyColorU;

			xLeftColorV += leftDxDivDyColorV;
			xRightColorV += rightDxDivDyColorV;
		}
	}

	//�����������Ⱦ�б�
	void AddObjectList(GameObject *objecet)
	{
		if (objecet == nullptr)
		{
			return;
		}
		objecetList[objectListCount++] = objecet;
	}

	//��Ⱦ�����б�
	void RenderObject()
	{
		for (int i = 0; i < objectListCount; i++)
		{
			if (objecetList[i]->state == RENDER_STATE_WIREFRAME)
			{
				for (int j = 0; j < objecetList[i]->triangleCount; j++)
				{
					//if (objecetList[i]->triangleList[j].state == TRIANGLE_BACKFACE)
					//	continue;
					objecetList[i]->triangleList[j].vertexList = objecetList[i]->transVertexList;

					DrawTriangle(objecetList[i]->triangleList[j], foreground, RENDER_STATE_WIREFRAME);
				}
			}
			/*else if (objecetList[i]->state == RENDER_STATE_COLOR)
			{
				for (int j = 0; j < objecetList[i]->triangleCount; j++)
				{
					if (objecetList[i]->triangleList[j].state == TRIANGLE_BACKFACE)
						continue;

					DrawTriangle(objecetList[i]->triangleList[j]);
				}
			}
			else if (objecetList[i]->state == RENDER_STATE_TEXTURE)
			{
				for (int j = 0; j < objecetList[i]->triangleCount; j++)
				{
					if (objecetList[i]->triangleList[j].state == TRIANGLE_BACKFACE)
						continue;
					DrawTriangle(objecetList[i]->triangleList[j], 0x000000FF, RENDER_STATE_WIREFRAME);
					DrawTriangleByTexCorrect(objecetList[i]->triangleList[j]);
					
				}
			}*/
			
		}
	}

	//�����Ⱦ�б�
	void ClearObjectList()
	{
		objectListCount = 0;
	}

	//����任ss
	void WorldToCamera()
	{
		for (int i = 0; i < objectListCount; i++)
		{
			for (int j = 0; j < objecetList[i]->vertexCount; j++)
			{
				objecetList[i]->transVertexList[j].vertex = camera->WolrdToCamera(objecetList[i]->transVertexList[j].vertex);
			}
		}
	}

	////�����޳�
	//void RemoveBackfaceTriangle()
	//{
	//	for (int i = 0; i < objectListCount; i++)
	//	{
	//		for (int j = 0; j < objecetList[i]->triangleCount; j++)
	//		{
	//			Vector3D v1 = objecetList[i]->triangleList[j].vertex[1].newPos - objecetList[i]->triangleList[j].vertex[0].newPos;
	//			Vector3D v2 =  objecetList[i]->triangleList[j].vertex[2].newPos - objecetList[i]->triangleList[j].vertex[1].newPos;
	//			//����
	//			Vector3D normal;
	//			VectorCross(normal, v1, v2);

	//			Vector3D direction = objecetList[i]->triangleList[j].vertex[0].newPos - camera->GetPosition();

	//			double dot = VectorDot(direction, normal);

	//			if (dot <= 0.0f)
	//			{
	//				objecetList[i]->triangleList[j].state = TRIANGLE_BACKFACE;
	//			}
	//			else
	//			{
	//				objecetList[i]->triangleList[j].state = TRIANGLE_FOREFACE;
	//			}
	//		}
	//	}
	//}
	//����任
	void LocalToWorld()
	{
		for (int i = 0; i < objectListCount; i++)
		{
			if (objecetList[i]->transVertexList != nullptr)
			{
				free(objecetList[i]->transVertexList);
				objecetList[i]->transVertexList = nullptr;
			}
			objecetList[i]->transVertexList = (Vertex3D*)malloc(sizeof(Vertex3D)* objecetList[i]->vertexCount);
			memcpy(objecetList[i]->transVertexList, objecetList[i]->localVertexList, sizeof(Vertex3D)* objecetList[i]->vertexCount);

			Matrix4X4 translation = { 1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				objecetList[i]->position.x, objecetList[i]->position.y, objecetList[i]->position.z, 1 };

			for (int j = 0; j < objecetList[i]->vertexCount; j++)
			{
				MatrixApply(objecetList[i]->transVertexList[j].vertex,
					objecetList[i]->localVertexList[j].vertex, translation);
			}
		}
	}

	//ͶӰ�任
	void Projection()
	{
		for (int i = 0; i < objectListCount; i++)
		{
			for (int j = 0; j < objecetList[i]->vertexCount; j++)
			{
				objecetList[i]->transVertexList[j].vertex.x = objecetList[i]->transVertexList[j].vertex.x / objecetList[i]->transVertexList[j].vertex.z * camera->GetViewDistance();
				objecetList[i]->transVertexList[j].vertex.y = objecetList[i]->transVertexList[j].vertex.y / objecetList[i]->transVertexList[j].vertex.z * camera->GetViewDistance();
			}
		}
	}

	//��ͼ�任
	void ViewTransform()
	{
		for (int i = 0; i < objectListCount; i++)
		{
			for (int j = 0; j < objecetList[i]->vertexCount; j++)
			{

				double t = (objecetList[i]->transVertexList[j].vertex.x + camera->GetViewWidth() / 2) / camera->GetViewWidth();
				objecetList[i]->transVertexList[j].vertex.x = Interp(0.0f, (double)width, t);
				t = (objecetList[i]->transVertexList[j].vertex.y + camera->GetViewHeight() / 2) / camera->GetViewHeight();
					//windows�����Y��Ҫ��ת
				objecetList[i]->transVertexList[j].vertex.y = (double)height - Interp(0.0f, (double)height, t);
			}
		}
	}

	void RenderTexture(UINT *texTureBuffer, int width, int height)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				UINT color = texTureBuffer[j + width * i];
				DrawPoint(j, i, color);
			}
		}
	}

private:
	void Init(UVNCamera *camera, int width, int height, void *frameBuffer, int renderState, UINT foreground, UINT background)
	{
		this->camera = camera;
		this->width = width;
		this->height = height;
		this->frameBuffer = (UINT*)frameBuffer;
		depthBuffer = (double*)malloc(sizeof(double)*width * height);
		this->renderState = renderState;
		this->foreground = foreground;
		this->background = background;
		this->clipMinX = 0;
		this->clipMinY = 0;
		this->clipMaxX = width;	
		this->clipMaxY = height;
		objectListCount = 0;
	}
	void Destroy()
	{
		if (frameBuffer != nullptr)
		{
			delete frameBuffer;
			frameBuffer = nullptr;
		}

		if (depthBuffer != nullptr)
		{
			free(depthBuffer);
		}
	}

	bool TestZ(double x, double y, double z)
	{
		if (x < clipMinX || x >= clipMaxX || y < clipMinY || y >= clipMaxY)
		{
			return false;
		}
		double curPixelDepth = depthBuffer[(UINT)x + width * (UINT)y];
		if (curPixelDepth < z)
		{
			depthBuffer[(UINT)x + width * (UINT)y] = z;
			return true;
		}
		else
		{
			return false;
		}
	}

	//void DrawTriangleTexture(const Triangle &triangle)
	//{
	//	VertexInfo vi = triangle.vi;
	//	vi.TopToBottomSort();
	//	float xLeft = vi.vertex[0].x;
	//	float xRight = vi.vertex[0].x;

	//	float y = vi.vertex[0].y;

	//	float dyLeft;
	//	float dxDivDyLeft;

	//	float dyRight;
	//	float dxDivDyRight;
	//	bool isChanged = false;

	//	if (vi.vertex[1].x < vi.vertex[2].x)
	//	{
	//		dyLeft = (vi.vertex[0].y - vi.vertex[1].y);
	//		dxDivDyLeft = dyLeft == 0 ? 0 : -(vi.vertex[0].x - vi.vertex[1].x) / dyLeft;

	//		dyRight = (vi.vertex[2].y - vi.vertex[0].y);
	//		dxDivDyRight = dyRight == 0 ? 0 : -(vi.vertex[2].x - vi.vertex[0].x) / dyRight;
	//	}
	//	else
	//	{
	//		dyLeft = (vi.vertex[0].y - vi.vertex[2].y);
	//		dxDivDyLeft = dyLeft == 0 ? 0 : -(vi.vertex[0].x - vi.vertex[2].x) / dyLeft;

	//		dyRight = (vi.vertex[1].y - vi.vertex[0].y);
	//		dxDivDyRight = dyRight == 0 ? 0 : -(vi.vertex[1].x - vi.vertex[0].x) / dyRight;
	//	}

	//	while (y >= vi.vertex[2].y)
	//	{
	//		float xStart = xLeft;
	//		float xEnd = xRight;

	//		for (int i = xStart; i <= xEnd; i++)
	//		{
	//			DrawPoint(i, y, triangle.color);
	//		}

	//		if ((int)y <= (int)vi.vertex[1].y && !isChanged)
	//		{
	//			dxDivDyLeft = -(vi.vertex[2].x - vi.vertex[1].x) / (vi.vertex[2].y - vi.vertex[1].y);
	//			isChanged = true;
	//			xLeft = vi.vertex[1].x;
	//		}

	//		xLeft += dxDivDyLeft;
	//		xRight += dxDivDyRight;

	//		y--;
	//	}
	//}
	//void DrawTriangleTexture(const Vector3D &p0, const Vector3D &p1, const Vector3D &p2)
	//{
	//	VertexInfo vi;
	//	vi.vertex[0] = p0;
	//	vi.vertex[1] = p1;
	//	vi.vertex[2] = p2;
	//	vi.TopToBottomSort();

	//	float dxDivDyLeft = -(vi.vertex[0].x - vi.vertex[1].x) / (vi.vertex[0].y - vi.vertex[1].y);
	//	float dxDivDyRight = -(vi.vertex[2].x - vi.vertex[0].x) / (vi.vertex[2].y - vi.vertex[0].y);
	//	float xLeft = vi.vertex[0].x;
	//	float xRight = vi.vertex[0].x;

	//	float dxDivDyLeftU = (vi.uv[0].x - vi.uv[1].x) / (vi.vertex[0].y - vi.vertex[1].y);
	//	float dxDivDyRightU = (vi.uv[2].x - vi.uv[1].x) / (vi.vertex[2].y - vi.vertex[0].y);
	//	float xLeftU = vi.uv[0].x;
	//	float xRightU = vi.uv[0].x;

	//	float dxDivDyLeftV = (vi.uv[0].y - vi.uv[1].y) / (vi.vertex[0].y - vi.vertex[1].y);
	//	float dxDivDyRightV = (vi.uv[2].y - vi.uv[1].y) / (vi.vertex[2].y - vi.vertex[0].y);
	//	float xLeftV = vi.uv[0].y;
	//	float xRightV = vi.uv[0].y;

	//	float y = vi.vertex[0].y;


	//	bool isChanged = false;
	//	while (y >= vi.vertex[2].y)
	//	{
	//		float xStart = xLeft;
	//		float xEnd = xRight;
	//		float dx = (xRight - xLeft);
	//		float du = (xRightU - xLeftU) / dx;
	//		float dv = (xRightV - xLeftV) / dx;
	//		if (dx == 0)
	//		{
	//			du = dv = 0;
	//		}
	//		if (xEnd < xStart)
	//		{
	//			Swap(xEnd, xStart);
	//		}
	//		for (int i = xStart; i <= xEnd; i++)
	//		{
	//			//UINT color = triangle.textureBuffer[(UINT)(xLeftU * triangle.textureWidth) + (UINT)(triangle.textureWidth * xLeftV * triangle.textureHeight)];
	//			DrawPoint(i, y, foreground);

	//			xLeftU += du;
	//			xLeftV += dv;
	//		}

	//		if ((int)y <= (int)vi.vertex[1].y && !isChanged)
	//		{
	//			dxDivDyLeft = -(vi.vertex[2].x - vi.vertex[1].x) / (vi.vertex[2].y - vi.vertex[1].y);
	//			isChanged = true;
	//			xLeft = vi.vertex[1].x;
	//		}


	//		xLeft += dxDivDyLeft;
	//		xRight += dxDivDyRight;

	//		xLeftU += dxDivDyLeftU;
	//		xRightU += dxDivDyRightU;

	//		xLeftV += dxDivDyLeftV;
	//		xRightV += dxDivDyRightV;

	//		y--;


	//	}
	//}

	
private:
	int width;
	int height;
	UINT *frameBuffer;		//ͼ�񻺴�
	double *depthBuffer;		//��Ȼ���
	int renderState;
	UINT background;		//������ɫ
	UINT foreground;		//�߿���ɫ
	double clipMaxX;
	double clipMaxY;
	double clipMinX;
	double clipMinY;

	UVNCamera *camera;		//���

	int objectListCount;
	GameObject *objecetList[64];			//��Ⱦ�����б�
};


#endif
