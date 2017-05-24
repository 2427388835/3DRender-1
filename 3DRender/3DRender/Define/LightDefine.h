#ifndef __LIGHTDEFINE_H
#define __LIGHTDEFINE_H
#include "../Define/GeometryDefine.h"

class Light
{
public:
	int id;
	int state;
	int attr;
	Point3D position;
	Color color;

	//�����ǿ
	virtual Color CalculateColor(Vertex3D &vertex, Material &material) = 0;
};

//���Դ
class PointLight :public Light
{
	//˥��ϵ��
	double kc;
	double kl;
	double kq;

	Color CalculateColor(Vertex3D &vertex, Material &material)
	{

	}
};

//�����Դ
class DirectionLight :public Light
{
	Vector3D direction;

	Color CalculateColor(Vertex3D &vertex, Material &material)
	{

	}
};

//�۹��
class SpotLight :public Light
{
	//˥��ϵ��
	double kc;
	double kl;
	double kq;

	Vector3D direction;
	double inAngle;		//��׶�Ƕ�
	double outAngle;	//��׶�Ƕ�

	Color CalculateColor(Vertex3D &vertex, Material &material)
	{

	}
};
#endif