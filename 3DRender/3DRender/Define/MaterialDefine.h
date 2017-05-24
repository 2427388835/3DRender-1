#ifndef __MATERIALDEFINE_H
#define __MATERIALDEFINE_H
#include "../3DMath.h"

struct Material
{
	int id;
	int state;
	int attr;
	char *name;
	double ka;			//�����ⷴ��ϵ��
	double kd;			//������ⷴ��ϵ��
	double ks;			//����ⷴ��ϵ��
	double shininess;	//�߹�ָ��
	UINT *texBuffer;	//����ͼ
	UINT width;
	UINT height;
};
#endif