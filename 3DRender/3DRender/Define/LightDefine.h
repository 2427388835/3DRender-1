#ifndef __LIGHTDEFINE_H
#define __LIGHTDEFINE_H
#include "../Define/GeometryDefine.h"

const int LIGHT_STATE_ON = 0;
const int LIGHT_STATE_OFF = 1;

//���Դ
const int LIGHT_TYPE_POINT = 0;
const int LIGHT_TYPE_DIRECTION = 1;
const int LIGHT_TYPE_SPOT = 2;
const int LIGHT_TYPE_AMBIENT = 3;

//������
class Light
{
public:
	int id;
	int state;
	int attr;
	Point3D position;
	Color color;

	//�����ǿ
	virtual Color CalculateColor(const Vertex3D &vertex, const Material &material, const Point3D &objectPos)
	{
		double r = material.ka;
		double g = material.ka;
		double b = material.ka;
		//�ж��Ƿ����
		r = r > 1.0 ? 1.0 : r;
		g = g > 1.0 ? 1.0 : g;
		b = b > 1.0 ? 1.0 : b;
		return Color(color.r *r, color.g *g, color.b *b);
	}
};

//���Դ
class PointLight :public Light
{
public:
	//˥��ϵ��
	double kc;
	double kl;
	double kq;

	Color CalculateColor(const Vertex3D &vertex, const Material &material, const Point3D &objectPos)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		//����ɢ���
		Vector3D l = position- vertex.vertex;		
		double d = CalculateVector3DLength(l);
		VectorNormalize(l);
		double dp = VectorDot(vertex.normal, l);
		//��˥��ϵ��
		double atten = kc + kl * d + kq *d * d;

		if (dp > 0)
		{
			r += material.kd * dp / atten;
			g += material.kd * dp / atten;
			b += material.kd * dp / atten;
		}

		//���㾵��߹�
		l = vertex.vertex - position;
		double x = l.x - 2 * VectorDot(l, vertex.normal) * vertex.normal.x;
		double y = l.y - 2 * VectorDot(l, vertex.normal) * vertex.normal.y;
		double z = l.z - 2 * VectorDot(l, vertex.normal) * vertex.normal.z;
		Vector3D rVector = { x,y,z,1 };
		Vector3D v = objectPos - vertex.vertex;
		VectorNormalize(rVector);
		VectorNormalize(v);
		double is =  pow(VectorDot(rVector, v), material.shininess);
		if (is > 0)
		{
			r += material.ks * is / atten;
			g += material.ks * is / atten;
			b += material.ks * is / atten;
		}

		//�ж��Ƿ����
		r = r > 1.0 ? 1.0 : r;
		g = g > 1.0 ? 1.0 : g;
		b = b > 1.0 ? 1.0 : b;
		return Color(color.r *r, color.g *g, color.b *b);
	}
};

//�����Դ
class DirectionLight :public Light
{
public:
	Vector3D direction;

	Color CalculateColor(const Vertex3D &vertex, const Material &material, const Point3D &objectPos)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		//����ɢ���
		Vector3D l;
		l.x = -direction.x;
		l.y = -direction.y;
		l.z = -direction.z;
		VectorNormalize(l);
		double dp = VectorDot(vertex.normal, direction);
		//��˥��ϵ��
		double atten = 1;

		if (dp > 0)
		{
			r += material.kd * dp / atten;
			g += material.kd * dp / atten;
			b += material.kd * dp / atten;
		}

		//���㾵��߹�
		l = direction;
		double x = l.x - 2 * VectorDot(l, vertex.normal) * vertex.normal.x;
		double y = l.y - 2 * VectorDot(l, vertex.normal) * vertex.normal.y;
		double z = l.z - 2 * VectorDot(l, vertex.normal) * vertex.normal.z;
		Vector3D rVector = { x, y, z, 1 };
		Vector3D v = objectPos - vertex.vertex;
		VectorNormalize(rVector);
		VectorNormalize(v);
		double is = pow(VectorDot(rVector, v), material.shininess);
		if (is > 0)
		{
			r += material.ks * is / atten;
			g += material.ks * is / atten;
			b += material.ks * is / atten;
		}

		//�ж��Ƿ����
		r = r > 1.0 ? 1.0 : r;
		g = g > 1.0 ? 1.0 : g;
		b = b > 1.0 ? 1.0 : b;
		return Color(color.r *r, color.g *g, color.b *b);
	}
};

//�۹��
class SpotLight :public Light
{
public:

	//˥��ϵ��
	double kc;
	double kl;
	double kq;

	Vector3D direction;
	double inAngle;		//��׶�Ƕ�
	double outAngle;	//��׶�Ƕ�
	double pf;			//ָ������

	void SetInAngle(double inAngle)
	{
		this->inAngle = inAngle / 180.0 * PI;
	}

	void SetOutAngle(double outAngle)
	{
		this->outAngle = outAngle / 180.0 * PI;
	}

	Color CalculateColor(const Vertex3D &vertex, const Material &material, const Point3D &objectPos)
	{
		Vector3D l = vertex.vertex - position;
		VectorNormalize(l);
		VectorNormalize(direction);
		double dp = VectorDot(l, direction);

		double r = 0.0;
		double g = 0.0;
		double b = 0.0;
		if (dp <= 0.0)
		{
			return Color(r, g, b);
		}
		double angle = acos(dp);



		//��Ӱ
		if (angle < inAngle)
		{
			//����ɢ���
			Vector3D l = position - vertex.vertex;
			double d = CalculateVector3DLength(l);
			VectorNormalize(l);
			double dp = VectorDot(vertex.normal, l);
			//��˥��ϵ��
			double atten = kc + kl * d + kq *d * d;

			if (dp > 0)
			{
				r += material.kd * dp / atten;
				g += material.kd * dp / atten;
				b += material.kd * dp / atten;
			}

			//���㾵��߹�
			l = vertex.vertex - position;
			double x = l.x - 2 * VectorDot(l, vertex.normal) * vertex.normal.x;
			double y = l.y - 2 * VectorDot(l, vertex.normal) * vertex.normal.y;
			double z = l.z - 2 * VectorDot(l, vertex.normal) * vertex.normal.z;
			Vector3D rVector = { x, y, z, 1 };
			Vector3D v = objectPos - vertex.vertex;
			VectorNormalize(rVector);
			VectorNormalize(v);
			double is = pow(VectorDot(rVector, v), material.shininess);
			if (is > 0)
			{
				r += material.ks * is / atten;
				g += material.ks * is / atten;
				b += material.ks * is / atten;
			}
		}
		//��Ӱ
		else if (angle >= inAngle && angle < outAngle)
		{
			//����ɢ���
			Vector3D l = position - vertex.vertex;
			double d = CalculateVector3DLength(l);
			VectorNormalize(l);
			double dp = VectorDot(vertex.normal, l);
			//��˥��ϵ��
			double atten = (kc + kl * d + kq *d * d);
			double pfValue = pow((cos(angle) - cos(2 * outAngle)) / (cos(2 * inAngle) - cos(2 * outAngle)), pf);
			atten = pfValue * atten;
			if (dp > 0)
			{
				r += material.kd * dp / atten;
				g += material.kd * dp / atten;
				b += material.kd * dp / atten;
			}

			//���㾵��߹�
			l = vertex.vertex - position;
			double x = l.x - 2 * VectorDot(l, vertex.normal) * vertex.normal.x;
			double y = l.y - 2 * VectorDot(l, vertex.normal) * vertex.normal.y;
			double z = l.z - 2 * VectorDot(l, vertex.normal) * vertex.normal.z;
			Vector3D rVector = { x, y, z, 1 };
			Vector3D v = objectPos - vertex.vertex;
			VectorNormalize(rVector);
			VectorNormalize(v);
			double is = pow(VectorDot(rVector, v), material.shininess);
			if (is > 0)
			{
				r += material.ks * is / atten;
				g += material.ks * is / atten;
				b += material.ks * is / atten;
			}
		}

		//�ж��Ƿ����
		r = r > 1.0 ? 1.0 : r;
		g = g > 1.0 ? 1.0 : g;
		b = b > 1.0 ? 1.0 : b;
		return Color(r, g, b);
	}
};
#endif