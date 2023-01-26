#pragma once
#include "plane.h"
#include "zbuffer.h"

class Figure
{
	void findMiddle()
	{
		middle->x = 0;
		middle->y = 0;
		middle->z = 0;
		middle->w = 1;

		for (int i = 0; i < vNumber; i++)
		{
			middle->x += vertices[i]->x;
			middle->y += vertices[i]->y;
			middle->z += vertices[i]->z;
		}
		middle->x /= (double)vNumber;
		middle->y /= (double)vNumber;
		middle->z /= (double)vNumber;
	}

public:

	Point** vertices; // вершины
	int vNumber; // количество вершин

	Plane** obj; // массив плоскостей объекта
	Plane** objView; // массив плоскостей объекта в проекции

	Plane** obj_shadow; // массив плоскостей теней объекта
	Plane** obj_shadowView; // массив плоскостей тени объекта в проекции
	
	int sNumber; // количество плоскостей (сторон)

	Point* middle; // центр объекта

	// перемещение в 0 0 0 
	void toZero()
	{
		findMiddle();
		move(-middle->x, -middle->y, -middle->z);
	}

	// перемещение обратно, где фигура располагалась
	void toPlace()
	{
		move(middle->x, middle->y, middle->z);
	}
	
	void move(double x, double y, double z)
	{
		for (int i = 0; i < vNumber; i++)
			vertices[i]->move(x, y, z);
	}
	
	void rotByX(int sign)
	{
		toZero();
		for (int i = 0; i < vNumber; i++)
			vertices[i]->rotX(sign*20);	
		toPlace();
	}

	void scale(double sx, double sy, double sz)
	{
		toZero();
		for (int i = 0; i < vNumber; i++)
			vertices[i]->scale(sx, sy, sz);
		toPlace();
	}
	
	void rotByY(int sign)
	{
		toZero();
		for (int i = 0; i < vNumber; i++)
			vertices[i]->rotY(sign * 20);
		toPlace();
	}

	void rotByY(int sign, int deg)
	{
		toZero();
		for (int i = 0; i < vNumber; i++)
			vertices[i]->rotY(sign * deg);
		toPlace();
	}

	void rotByZ(int sign)
	{
		toZero();
		for (int i = 0; i < vNumber; i++)
			vertices[i]->rotZ(sign * 20);
		toPlace();
	}

	void copyObjToView()
	{
		for (int i = 0; i < sNumber; i++)
		{	
			*(objView[i]) = obj[i]; // копирование каждой точки и каждой координаты
			*(obj_shadowView[i]) = obj_shadow[i]; // копирование каждой точки и каждой координаты
		}
	}

	void projectObjView(int projectionMode)
	{
		for (int i = 0; i < sNumber; i++)
		{
			objView[i]->project(projectionMode);
			obj_shadowView[i]->project(projectionMode);
		}
	}

	void project(int projectionMode) 
	{			
		copyObjToView(); // копирование данных
		projectObjView(projectionMode);		
	}



	void drawFigureToBuffer(ZBuffer* buffer)
	{	
		for (int i = 0; i < sNumber; i++)
		{
			buffer->putSideToBuffer(objView[i]);
			buffer->putSideToBuffer(obj_shadowView[i]);
		}
	}

	void updateFrame(int* minX, int* minY, int* maxX, int* maxY)
	{
		for (int j = 0; j < sNumber; j++)
		{
			for (int i = 0; i < (*objView[j]).vNumber; i++)
			{
				if ((*objView[j])[i]->x < *minX)
					*minX = (*objView[j])[i]->x;

				if ((*objView[j])[i]->y < *minY)
					*minY = (*objView[j])[i]->y;

				if ((*objView[j])[i]->x > *maxX)
					*maxX = (*objView[j])[i]->x;

				if ((*objView[j])[i]->y > *maxY)
					*maxY = (*objView[j])[i]->y;


				if ((*obj_shadowView[j])[i]->x < *minX)
					*minX = (*obj_shadowView[j])[i]->x;

				if ((*obj_shadowView[j])[i]->y < *minY)
					*minY = (*obj_shadowView[j])[i]->y;

				if ((*obj_shadowView[j])[i]->x > *maxX)
					*maxX = (*obj_shadowView[j])[i]->x;

				if ((*obj_shadowView[j])[i]->y > *maxY)
					*maxY = (*obj_shadowView[j])[i]->y;
			}
		}
	}

	void print()
	{
		for (int i = 0; i < vNumber; i++)
			vertices[i]->print();
	}

	void createShadow()
	{
		double lightX = 200;
		double lightY = 0;
		double lightZ = 0;

		double y = 300;
		double t, x, z;
		for (int i = 0; i < sNumber; i++)
		{
			for (int j = 0; j < obj[i]->vNumber; j++)
			{
				t = (y - (*obj[i])[j]->y) / (lightY - (*obj[i])[j]->y);
				x = (lightX - (*obj[i])[j]->x) * t + (*obj[i])[j]->x;
				z = (lightZ - (*obj[i])[j]->z) * t + (*obj[i])[j]->z;

				(*obj_shadow[i])[j]->x = x;
				(*obj_shadow[i])[j]->y = y;
				(*obj_shadow[i])[j]->z = z;
			}
		}
	}



	void controlMenu() // меню управления фигурой
	{
		bool defautFlag = false;
		char c = getch();
		switch (c)
		{
		case 'w':
			move(0, -20, 0);
			break;

		case 's':
			move(0, 20, 0);
			break;

		case 'a':
			move(-20, 0, 0);
			break;

		case 'd':
			move(20, 0, 0);
			break;

		case 'q':
			move(0, 0, 20);
			break;

		case 'e':
			move(0, 0, -20);
			break;

		case 'r':
			rotByX(1);
			break;

		case 't':
			rotByX(-1);
			break;

		case 'y':
			rotByY(1);
			break;

		case 'u':
			rotByY(-1);
			break;

		case 'i':
			rotByZ(1);
			break;

		case 'o':
			rotByZ(-1);
			break;

		case 'z':
			scale(0.5, 0.5, 0.5);
			break;

		case 'x':
			scale(2, 2, 2);
			break;

		default:
			defautFlag = true;
			printf("unknown 2nd command\n");
		}

		if (defautFlag == false)
			printf("%c\n", c); // введенная команда (кроме ошибочной)
	}
};


class Piramid:public Figure {
public:
	Piramid()
	{
		vNumber = 4;
		sNumber = 4;

		middle = new Point;
		vertices = new Point * [vNumber];
		vertices[0] = new Point(10, 40, 10);
		vertices[1] = new Point(20, 40, 30);
		vertices[2] = new Point(40, 40, 10);
		vertices[3] = new Point(30, 10, 20);

		obj = new Plane*[sNumber];
		obj_shadow = new Plane*[sNumber];

		obj[0] = new Plane(vertices[0], vertices[2], vertices[1], 1);
		obj[1] = new Plane(vertices[0], vertices[1], vertices[3], 2);
		obj[2] = new Plane(vertices[1], vertices[2], vertices[3], 3);
		obj[3] = new Plane(vertices[2], vertices[0], vertices[3], 4);

		objView = new Plane * [sNumber];
		obj_shadowView = new Plane * [sNumber];

		for (int i = 0; i < sNumber; i++)
		{
			objView[i] = new Plane;
			obj_shadow[i] = new Plane(3);
			obj_shadowView[i] = new Plane(3);
		}
	}
};

class Parallelepiped :public Figure {
public:
	Parallelepiped()
	{
		vNumber = 8;
		sNumber = 6;

		middle = new Point;
		vertices = new Point * [vNumber];

		vertices[0] = new Point(0, 40, 40);
		vertices[1] = new Point(40, 40, 40);
		vertices[2] = new Point(40, 40, 0);
		vertices[3] = new Point(0, 40, 0);
		vertices[4] = new Point(0, 0, 40);
		vertices[5] = new Point(40, 0, 40);
		vertices[6] = new Point(40, 0, 0);
		vertices[7] = new Point(0, 0, 0);

		obj = new Plane * [sNumber];
		obj_shadow = new Plane * [sNumber];

		obj[0] = new Plane(vertices[0], vertices[1], vertices[5], vertices[4], 1);
		obj[1] = new Plane(vertices[1], vertices[2], vertices[6], vertices[5], 2);
		obj[2] = new Plane(vertices[2], vertices[3], vertices[7], vertices[6], 3);
		obj[3] = new Plane(vertices[3], vertices[0], vertices[4], vertices[7], 4);
		obj[4] = new Plane(vertices[4], vertices[5], vertices[6], vertices[7], 5);
		obj[5] = new Plane(vertices[3], vertices[2], vertices[1], vertices[0], 6);

		objView = new Plane * [sNumber];
		obj_shadowView = new Plane * [sNumber];

		for (int i = 0; i < sNumber; i++)
		{
			objView[i] = new Plane;
			obj_shadow[i] = new Plane(4);
			obj_shadowView[i] = new Plane(4);
		}
	}
};
