#ifndef __GEOMETRYDEFINE_H
#define __GEOMETRYDEFINE_H
#include "../3DMath.h"
#include "../Define/MaterialDefine.h"
#include "../Define/ColorDefine.h"

struct Vertex3D
{
	Vector3D vertex;	//����
	Vector3D normal;	//����
	Color color;		//������ɫ
	Vector2D uv;		//��������
};

struct Triangle
{
	int id;
	int state;
	int attr;
	Vertex3D* vertexList;	//�����б�
	int index[3];			//����
	Material *material;		//����
};

struct GameObject
{
	int id;
	int state;
	int attr;
	char *name;		
	int state;		//״̬

	int vertexCount;
	Vertex3D *localVertexList;
	Vertex3D *transVertexList;
	Triangle *triangleList;
};
#endif