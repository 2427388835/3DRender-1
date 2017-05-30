#ifndef __MATERIALDEFINE_H
#define __MATERIALDEFINE_H
#include "../3DMath.h"
#include "Define/ColorDefine.h"

struct Material
{
	int id;
	int state;
	int attr;
	char name[64];
	double ka;			//�����ⷴ��ϵ��
	double kd;			//������ⷴ��ϵ��
	double ks;			//����ⷴ��ϵ��
	double shininess;	//�߹�ָ��
	UINT *texBuffer;	//����ͼ
	double width;
	double height;
	Color color;
	Color ra, rd, rs;

	Material(UINT *tex, double width, double height)
	{
		this->texBuffer = tex;
		this->width = width;
		this->height = height;
	}
};
#endif