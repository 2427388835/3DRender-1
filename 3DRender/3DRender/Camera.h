/*
*�����
*/
#ifndef __CAMERA_H
#define __CAMERA_H
#include "3DMath.h"

class UVNCamera
{
public:
	Point3D GetPosition() const
 	{
		return position;
	}
	//����任
	Point3D WolrdToCamera(const Point3D &worldPos)
	{
		Point3D tPos;
		MatrixApply(tPos, worldPos, matCamera);
		return tPos;
	}

	float GetViewDistance() const
	{
		return viewDistance;
	}

	float GetFov() const
	{
		return fov;
	}

	float GetViewWidth() const
	{
		return width;
	}

	float GetViewHeight() const
	{
		return height;
	}

	//�������Ŀ��
	void SetTarget(const Point3D &target, Vector3D &v)
	{
		v = v;
		//n����
 		n = target - position;
		//n����
		VectorCross(u, v, n);
		//����v����
		VectorCross(v, n, u);

		VectorNormalize(u);
		VectorNormalize(v);
		VectorNormalize(n);

		CalculateMatCamera();
	}
		

	UVNCamera(Point3D position, Vector3D target, Vector3D v, 
		float nearClip, float farClip, float fov, float viewDistance, float aspectRatio)
		:position(position), u(u), v(v),
		nearClip(nearClip), farClip(farClip), fov(fov), viewDistance(viewDistance), aspectRatio(aspectRatio)
	{
		SetTarget(target, v);
		width = (float)(viewDistance * tan(fov *  PI / 360.0f ) * 2);
		height = width / aspectRatio;
	}
	~UVNCamera()
	{
	}
private:
	//�����������
	void CalculateMatCamera()
	{
		matCamera
			= { u.x, v.x, n.x, 0,
			u.y, v.y, n.y, 0,
			u.z, v.z, n.z, 0,
			-position.x,-position.y,-position.z,1 };
	}

private:
	Point3D position;	//�������
	Matrix4X4 matCamera;//����任����

	Vector3D u;			//u����
	Vector3D v;			//v����
	Vector3D n;			//n����

	float fov;			//�ӹ��
	float nearClip;		//���ü���
	float farClip;		//Զ�ü���
	float viewDistance;	//�Ӿ�

	float aspectRatio;	//��߱�
	float width;
	float height;
};
#endif