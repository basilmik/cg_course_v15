#pragma once
#include "../libwinbgi/src/graphics.h"
#include "stdio.h"
#include "math.h"

#define X 0
#define Y 1
#define Z 2
#define W 3

class Point
{
public:

	double x, y, z, w; // координаты точки

private:
	double m[4][4]; // матрица изменений координат точки

	// перемножение координат и матрицы изменений
	double* multiply()
	{
		double x_ = x;
		double y_ = y;
		double z_ = z;
		double w_ = w;
		double cpy[4] = { x_, y_, z_, w_ };
		double res[4] = { 0,0,0,0 };

		res[X] = cpy[0] * m[0][0] + cpy[1] * m[1][0] + cpy[2] * m[2][0] + cpy[3] * m[3][0];
		res[Y] = cpy[0] * m[0][1] + cpy[1] * m[1][1] + cpy[2] * m[2][1] + cpy[3] * m[3][1];
		res[Z] = cpy[0] * m[0][2] + cpy[1] * m[1][2] + cpy[2] * m[2][2] + cpy[3] * m[3][2];
		res[W] = cpy[0] * m[0][3] + cpy[1] * m[1][3] + cpy[2] * m[2][3] + cpy[3] * m[3][3];

		x = res[X];
		y = res[Y];
		z = res[Z];
		w = res[W];
		return res;
	}


	void setRotXMtx(int deg)
	{
		double rad = deg * 3.1415 / 180;

		m[0][0] = 1; m[0][1] = 0;			m[0][2] = 0;			m[0][3] = 0;
		m[1][0] = 0; m[1][1] = cos(rad);	m[1][2] = -sin(rad);	m[1][3] = 0;
		m[2][0] = 0; m[2][1] = sin(rad);	m[2][2] = cos(rad);		m[2][3] = 0;
		m[3][0] = 0; m[3][1] = 0;			m[3][2] = 0;			m[3][3] = 1;
	}

	void setRotYMtx(int deg) {

		double rad = deg * 3.1415 / 180;

		m[0][0] = cos(rad); m[0][1] = 0;	m[0][2] = -sin(rad);	m[0][3] = 0;
		m[1][0] = 0;		m[1][1] = 1;	m[1][2] = 0;			m[1][3] = 0;
		m[2][0] = sin(rad); m[2][1] = 0;	m[2][2] = cos(rad);		m[2][3] = 0;
		m[3][0] = 0;		m[3][1] = 0;	m[3][2] = 0;			m[3][3] = 1;
	}

	void setRotZMtx(int deg) {
		double rad = deg * 3.1415 / 180;

		m[0][0] = cos(rad);		m[0][1] = -sin(rad);	m[0][2] = 0;	m[0][3] = 0;
		m[1][0] = sin(rad);		m[1][1] = cos(rad);		m[1][2] = 0;	m[1][3] = 0;
		m[2][0] = 0;			m[2][1] = 0;			m[2][2] = 1;	m[2][3] = 0;
		m[3][0] = 0;			m[3][1] = 0;			m[3][2] = 0;	m[3][3] = 1;
	}

	void setMoveMtx(double dx, double dy, double dz)
	{
		m[0][0] = 1;	m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
		m[1][0] = 0;	m[1][1] = 1;		m[1][2] = 0;		m[1][3] = 0;
		m[2][0] = 0;	m[2][1] = 0;		m[2][2] =1;			m[2][3] = 0;
		m[3][0] = dx;	m[3][1] = dy;		m[3][2] = dz;		m[3][3] = 1;
	}

	void setScaleMtx(double sx, double sy, double sz) {
		m[0][0] = sx;	m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
		m[1][0] = 0;	m[1][1] = sy;		m[1][2] = 0;		m[1][3] = 0;
		m[2][0] = 0;	m[2][1] = 0;		m[2][2] = sz;		m[2][3] = 0;
		m[3][0] = 0;	m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
	}

	void setProjectMtx(int c)
	{

		// https://studfile.net/preview/903563/page:5/
		// учитывается, что ось игрек вниз, а не вверх -- изменен знак у некоторых элементов матрицы

		if (c == 1) // изометрия
		{
			m[0][0] = 0.71;		m[0][1] = 0.41;		m[0][2] = 0.58;		m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 0.81;		m[1][2] = -0.58;	m[1][3] = 0;
			m[2][0] = -0.71;	m[2][1] = 0.41;		m[2][2] = 0.58;		m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
		}

		if (c == 2) // кабинетная
		{
			m[0][0] = 1;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 1;		m[1][2] = 0;		m[1][3] = 0;
			m[2][0] = -0.71;	m[2][1] = 0.71;		m[2][2] = 1;		m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
		}

		if (c == 3) // свободная
		{
			m[0][0] = 1;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 1;		m[1][2] = 0;		m[1][3] = 0;
			m[2][0] = -0.35;	m[2][1] = 0.35;		m[2][2] = 1;		m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
		}

		if (c == 4) // ортографическая на хоу с сохранением глубины (1 по з)
		{
			m[0][0] = 1;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 1;		m[1][2] = 0;		m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1;		m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
		}
	}

public:

	// deg - угол поворта
	void rotX(int deg)
	{
		setRotXMtx(deg);
		multiply();
	}

	// deg - угол поворта
	void rotY(int deg)
	{
		setRotYMtx(deg);
		multiply();
	}

	// deg - угол поворта
	void rotZ(int deg)
	{
		setRotZMtx(deg);
		multiply();
	}


	void move(double _x, double _y, double _z)
	{
		setMoveMtx(_x, _y, _z);
		multiply();
	}

	void scale(double sx, double sy, double sz)
	{
		setScaleMtx(sx, sy, sz);
		multiply();
	}

	void project(int projectionMode)
	{
		setProjectMtx(projectionMode);
		multiply();
	}

	void print()
	{
		printf("%f %f %f %f\n", x, y, z, w);
	}

	Point()
	{
	}

	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{
		w = 1;
	}

	void operator=(Point* _point)
	{
		x = _point->x;
		y = _point->y;
		z = _point->z;
		w = _point->w;
	}

};
