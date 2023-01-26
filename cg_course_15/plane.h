#pragma once
#include "point.h"

class Plane {

	Point** vertices; // вершины
	
public:
	int vNumber; // количество вершин
	int color; // цвет плоскости
	double A, B, C, D; // коэффициенты плоскости

	double getMiddleX()
	{
		double x = 0;
		for (int i = 0; i < vNumber; i++)
		{
			x += vertices[i]->x;
		}
		x /= (double)vNumber;
		return x;
	}

	double getMiddleY()
	{
		double y = 0;
		for (int i = 0; i < vNumber; i++)
		{
			y += vertices[i]->y;
		}
		y /= (double)vNumber;
		return y;
	}

	Plane()
	{
		vertices = nullptr;
		vNumber = 0;
		color = 1;
	}

	Plane(int vNum)
	{
		vertices = new Point*[vNum];
		for (int i = 0; i < vNum; i++)
		{
			vertices[i] = new Point;
		}
		vNumber = vNum;
		color = 7;
	}


	Plane(Point* v0, Point* v1, Point* v2, int clr)
	{
		vertices = new Point*[3];
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;
		vNumber = 3;
		color = clr;
		updateCoefs();
	}


	Plane(Point* v0, Point* v1, Point* v2, Point* v3, int clr)
	{
		vertices = new Point * [4];
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;
		vertices[3] = v3;
		vNumber = 4;
		color = clr;
		updateCoefs();
	}


	Point* operator[] (int i)
	{
		return vertices[i];
	}


	void operator =(Plane* _plane)
	{
		vertices = new Point * [_plane->vNumber];
		vNumber = _plane->vNumber;
		color = _plane->color;

		for (int i = 0; i < vNumber; i++)
		{
			vertices[i] = new Point;
			*(vertices[i]) = ((*_plane)[i]); // копирование значений каждой координаты
		}

		updateCoefs();
	}

	void project(int projectionMode)
	{
		for (int i = 0; i < vNumber; i++)
			vertices[i]->project(projectionMode);		
		updateCoefs();
	}

	void updateCoefs()
	{
		int ai = 0;
		int i = 1;	
		int bi = 2;

		double M[3] = { vertices[i]->x, vertices[i]->y, vertices[i]->z };
		double a[3] = { vertices[ai]->x - vertices[i]->x, vertices[ai]->y - vertices[i]->y, vertices[ai]->z - vertices[i]->z };
		double b[3] = { vertices[bi]->x - vertices[i]->x, vertices[bi]->y - vertices[i]->y, vertices[bi]->z - vertices[i]->z };

		A = a[Y] * b[Z] - a[Z] * b[Y];
		B = a[X] * b[Z] - a[Z] * b[X];
		B = -B;
		C = a[X] * b[Y] - a[Y] * b[X];

		D = M[X] * A + M[Y] * B + M[Z] * C;
		D = -D;
	}

};
