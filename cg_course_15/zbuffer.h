#pragma once
#include "../libwinbgi/src/graphics.h"
#include "point.h"
#include "plane.h"

// индексы в массиве buffer
#define DEPTH 0
#define CLR 1

#define SIZE 400 // размер окна
#define MIN_Z -1000000 // минимальная глубина объекта в сцене

class ZBuffer
{
	float buffer[SIZE][SIZE][2] = { 0 };
	bool cheked_flag[SIZE][SIZE] = { false }; // для затравки, определение была ли точка уже пройдена при затравке

	// если точка с правой стороны от каждой стороны плоскости, то принадлежит
	bool belongsToPlane(Plane* side, double x, double y)
	{
		for (int i = 0, j = 0; i < side->vNumber; i++)
		{
			j = i + 1;
			if (i == side->vNumber - 1)
				j = 0;
		
			if ((x - (*side)[j]->x) * ((*side)[i]->y - (*side)[j]->y) - (y - (*side)[j]->y) * ((*side)[i]->x - (*side)[j]->x) > 0)
				return false;
		}

		return true;
	}

	// закраска затравкой в буфере
	void forEachPointIfVisible(Plane* side, int x, int y, int color)
	{
		if (x < 0 || y < 0 || x >= SIZE || y >= SIZE || !belongsToPlane(side, x, y))
			return;

		if (cheked_flag[x][y] == false)
		{
			int z = 0;
			if (side->C != 0)
				z = -(side->A * x + side->B * y + side->D) / side->C;
			else
				for (int i = 1; i < side->vNumber; i++)
					z = max((*side)[i]->z, (*side)[i - 1]->z);

			cheked_flag[x][y] = true;

			if (z > buffer[x][y][DEPTH])
			{
				buffer[x][y][DEPTH] = z;
				buffer[x][y][CLR] = color;
			}

			forEachPointIfVisible(side, x + 1, y, color);
			forEachPointIfVisible(side, x, y + 1, color);
			forEachPointIfVisible(side, x - 1, y, color);
			forEachPointIfVisible(side, x, y - 1, color);
		}
	}


	void clearCheckedBuffer()
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
				cheked_flag[i][j] = false;
	}


public:

	void putSideToBuffer(Plane* side)
	{
		clearCheckedBuffer();
		forEachPointIfVisible(side, side->getMiddleX(), side->getMiddleY(), side->color);
	}

	void clearBuffer()
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				buffer[i][j][DEPTH] = MIN_Z;
				buffer[i][j][CLR] = 0;
			}
		}
	}


	void putBufferToScreen(int minX, int minY, int maxX, int maxY)
	{
		clearviewport();

		if (minX < 0)	
			minX = 0;
		if (minY < 0)
			minY = 0;
		if (maxX > SIZE)
			maxX = SIZE;
		if (maxY > SIZE)
			maxY = SIZE;

		for (int i = minX; i < maxX; i++)
			for (int j = minY; j < maxY; j++)
				putpixel(i, j, buffer[i][j][CLR]);
	}

	// нарисовать оси
	void outlineAxis(int projectionMode)
	{
		Point* zero = new Point(0, 0, 0);
		Point* x100 = new Point(20, 0, 0);
		Point* y100 = new Point(0, 20, 0);
		Point* z100 = new Point(0, 0, 20);

		zero->project(projectionMode);
		x100->project(projectionMode);
		y100->project(projectionMode);
		z100->project(projectionMode);

		zero->move(30, 360, 0);
		x100->move(30, 360, 0);
		y100->move(30, 360, 0);
		z100->move(30, 360, 0);

		setcolor(12);
		line(zero->x, zero->y, x100->x, x100->y);

		setcolor(13);
		line(zero->x, zero->y, y100->x, y100->y);

		setcolor(14);
		line(zero->x, zero->y, z100->x, z100->y);
	}

};