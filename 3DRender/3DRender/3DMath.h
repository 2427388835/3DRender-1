/*
*3D��ѧ��
*/
#ifndef __3DMATH_H
#define __3DMATH_H
#include <cmath>

typedef unsigned char UCHAR;
typedef unsigned int UINT;

const double PI = 3.1415926;

typedef unsigned int UINT32;
//4X4����
struct Matrix4X4
{
	double m[4][4];
};

//3D����
struct Vector3D
{
	double x, y, z, w;

	Vector3D operator-(const Vector3D & v)
	{
		return Vector3D{this->x - v.x, this->y - v.y, this->z - v.z, 0 };
	}

	Vector3D operator-(const Vector3D & v) const
	{
		return Vector3D{ this->x - v.x, this->y - v.y, this->z - v.z, 0 };
	}

	Vector3D operator+(const Vector3D & v)
	{
		return Vector3D{ this->x + v.x, this->y + v.y, this->z + v.z, 0 };
	}

	Vector3D operator+(const Vector3D & v) const
	{
		return Vector3D{ this->x + v.x, this->y + v.y, this->z + v.z, 0 };
	}

	Vector3D operator/(double x)
	{
		return Vector3D{ this->x / x, this->y / x, this->z / x, 0 };
	}
};

//2D����
struct Vector2D
{
	double x, y;

	bool operator<(const Vector2D & v) 
	{
		return (y < v.y);
	}

	bool operator>(const Vector2D & v)
	{
		return (y > v.y);
	}

};

//2D��
typedef Vector2D Point2D;

template<class T>
void Swap(T &p1, T &p2)
{
	T temp = p1;
	p1 = p2;
	p2 = temp;
}

//ѡ������
template<class T>
void SelectionSort(T arr[], int count)
{	
	for (int i = 0; i < count-1; i++)
	{
		int index = i;
		for (int j = index; j < count; j++)
		{
			if (arr[j] > arr[index])
			{
				continue;
			}
			index = j;
		}

		if (index != i)
		{
			Swap(arr[i], arr[index]);
		}
	}
}

//4D��
typedef Vector3D Point3D;

//ֵ�ڷ�Χ��
int MakeValueInRange(int value, int min, int max)
{
	return (value < min) ? min : ((value > max) ? max : value);
}

//�����ֵ t[0, 1]
double Interp(double value1, double value2, double t)
{
	return value1 + (value2 - value1) * t;
}

//������������
double CalculateVector3DLength(const Vector3D &v)
{
	double sq = v.x * v.x + v.y * v.y + v.z * v.z;
	return sqrt(sq);
}

//������
void VectorAdd(Vector3D &destV, const Vector3D &x, const Vector3D &y)
{
	destV.x = x.x + y.x;
	destV.y = x.y + y.y;
	destV.z = x.z + y.z;
	destV.w = 1.0f;
}

//������
void VectorSub(Vector3D &destV, const Vector3D &x, const Vector3D &y)
{
	destV.x = x.x - y.x;
	destV.y = x.y - y.y;
	destV.z = x.z - y.z;
	destV.w = 1.0f;
}

//�������
double VectorDot(const Vector3D &x, const Vector3D &y)
{
	return x.x * y.x + x.y * y.y + x.z * y.z;
}

//�������
void VectorCross(Vector3D &destV, const Vector3D &x, const Vector3D &y)
{
	destV.x = x.y * y.z - x.z * y.y;
	destV.y = x.z * y.x - y.z * x.x;
	destV.z = x.x * y.y - y.x * x.y;
	destV.w = 1.0f;
}

//������ֵ
void VectorInterp(Vector3D &destV, const Vector3D &x, const Vector3D &y, double t)
{
	destV.x = Interp(x.x, y.x, t);
	destV.y = Interp(x.y, y.y, t);
	destV.z = Interp(x.z, y.z, t);
	destV.w = 1.0f;
}

//������һ��
void VectorNormalize(Vector3D &v)
{
	double length = CalculateVector3DLength(v);
	if (length != 0.0f)
	{
		v.x /= length;
		v.y /= length;
		v.z /= length;
	}
}

//�������
void MatrixAdd(Matrix4X4 &destM, const Matrix4X4 &m1, const Matrix4X4 &m2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			destM.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
}

//�����
void MatrixSub(Matrix4X4 &destM, const Matrix4X4 &m1, const Matrix4X4 &m2)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			destM.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
}

//����˷�
void MatrixMul(Matrix4X4 &destM, const Matrix4X4 &m1, const Matrix4X4 &m2)
{
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			destM.m[j][i] = (m1.m[j][0] * m2.m[0][i]) +
				(m1.m[j][1] * m2.m[1][i]) +
				(m1.m[j][2] * m2.m[2][i]) +
				(m1.m[j][3] * m2.m[3][i]);
		}
	}
}

//�����;������
void MatrixApply(Vector3D &destV, const Vector3D v, const Matrix4X4 &m)
{
	double X = v.x, Y = v.y, Z = v.z, W = v.w;
	destV.x = X * m.m[0][0] + Y * m.m[1][0] + Z * m.m[2][0] + W * m.m[3][0];
	destV.y = X * m.m[0][1] + Y * m.m[1][1] + Z * m.m[2][1] + W * m.m[3][1];
	destV.z = X * m.m[0][2] + Y * m.m[1][2] + Z * m.m[2][2] + W * m.m[3][2];
	destV.w = X * m.m[0][3] + Y * m.m[1][3] + Z * m.m[2][3] + W * m.m[3][3];
}

//��������
void MatrixScale(Matrix4X4 &destM, const Matrix4X4 &m, double scale)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			destM.m[i][j] = m.m[i][j] * scale;
		}
	}
}

//��λ����
void MatrixIdentity(Matrix4X4 &destM)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				destM.m[i][j] = 1.0f;
			}
			else
			{
				destM.m[i][j] = 0.0f;
			}			
		}
	}
}

//�����
void MatrixZero(Matrix4X4 &destM)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			destM.m[i][j] = 0.0f;
		}
	}
}

//ƽ�Ʊ任
void MatrixTranslate(Matrix4X4 &m, double x, double y, double z)
{
	MatrixIdentity(m);
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
}

//���ű任
void MatrixScale(Matrix4X4 &m, double x, double y, double z)
{
	MatrixIdentity(m);
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
}

//������ת
void MatrixRotate(Matrix4X4 &m, double x, double y, double z, double theta)
{
	double qsin = (double)sin(theta * 0.5f);
	double qcos = (double)cos(theta * 0.5f);
	Vector3D vec = { x, y, z, 1.0f };
	double w = qcos;
	VectorNormalize(vec);
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	m.m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m.m[1][0] = 2 * x * y - 2 * w * z;
	m.m[2][0] = 2 * x * z + 2 * w * y;
	m.m[0][1] = 2 * x * y + 2 * w * z;
	m.m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m.m[2][1] = 2 * y * z - 2 * w * x;
	m.m[0][2] = 2 * x * z - 2 * w * y;
	m.m[1][2] = 2 * y * z + 2 * w * x;
	m.m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;
}

struct Transform
{
	Matrix4X4 world;
	Matrix4X4 view;
	Matrix4X4 projection;
	Matrix4X4 transform;	//transform = world * view * projection
	double w, h;				//��Ļ��С
};

//���¾���
void TransformUpdate(Transform &ts)
{
	Matrix4X4 m;
	MatrixMul(m, ts.world, ts.view);
	MatrixMul(ts.transform, m, ts.projection);
}

struct Linear2D
{
	double x0, y0, x1, y1;

	Linear2D(double x0, double y0, double x1, double y1) 
	{
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
	}

	//f01��x��y��=��y0-y1��*x+��x1-x0��*y+x0*y1-x1*y0
	double InputXGetY(double x)
	{
		return -((y0 - y1)*x + x0*y1 - x1*y0) / (x1 - x0);
	}

	double InputYGetX(double y)
	{
		return -((x1 - x0)*y + x0*y1 - x1*y0) / (y0 - y1);
	}
};

//���������ε������㣬�����ֵ
double GetInterpValue(double x0, double y0, double value0, 
	double x1, double y1, double value1, 
	double x2, double y2, double value2, 
	double x, double y)
{
	Vector3D p1 = { x1 - x, y1 - y, 0 };
	Vector3D p2 = { x2 - x, y2 - y, 0 };
	Vector3D p0 = { x0 - x, y0 - y };

	Vector3D p12;
	VectorCross(p12, p1, p2);
	double Sp12 = CalculateVector3DLength(p12);

	Vector3D p02;
	VectorCross(p02, p0, p2);
	double Sp02 = CalculateVector3DLength(p02);

	Vector3D p01;
	VectorCross(p01, p0, p1);
	double Sp01 = CalculateVector3DLength(p01);

	double total = Sp12 + Sp02 + Sp01;

	double value = value0 * Sp12 / total + value1 * Sp02 / total + value2 * Sp01 / total;
	return value;
}

#endif