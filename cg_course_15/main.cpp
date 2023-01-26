#include "../libwinbgi/src/graphics.h"
#include "figure.h"
#include "zbuffer.h"

int main()
{
	initwindow(SIZE, SIZE); // создать графическое окно (400x400)

	ZBuffer* buf = new ZBuffer; // буфер эрана

	Parallelepiped* lepiped = new Parallelepiped;
	Piramid* piramid = new Piramid;

	// расположить в стартовой позиции
	piramid->move(110, 180, 0);
	piramid->rotByX(1);
	lepiped->move(210, 170, 0);
	lepiped->rotByX(1);
	lepiped->rotByY(1);

	int projectionMode = 1;

	// размер рамки - перерисовываемого окна (только где есть плоскости)
	int minX = SIZE;
	int minY = SIZE;
	int maxX = 0;
	int maxY = 0;

	while (true)
	{
		buf->clearBuffer();

		piramid->createShadow();
		lepiped->createShadow();
		
		piramid->project(projectionMode);
		lepiped->project(projectionMode);
		
		// обновить рамку
		piramid->updateFrame(&minX, &minY, &maxX, &maxY);
		lepiped->updateFrame(&minX, &minY, &maxX, &maxY);
		
		// поместить плоскости фигур в буфер
		piramid->drawFigureToBuffer(buf);
		lepiped->drawFigureToBuffer(buf);

		// вывод буфера и осей
		buf->putBufferToScreen(minX, minY, maxX, maxY);
		buf->outlineAxis(projectionMode);


		// users's 1st command input
		char c = getch();
		switch (c)
		{
		case '1':
			projectionMode = 1; 
			printf("projection mode set: 1\n");
			break;

		case '2':
			projectionMode = 2;
			printf("projection mode set: 2\n");
			break;

		case '3':
			projectionMode = 3;
			printf("projection mode set: 3\n");
			break;

		case '4':
			projectionMode = 4;
			printf("projection mode set: 4\n");
			break;

		case 'P':
			printf("piramid + ");
			piramid->controlMenu();		
			break;

		case 'L':
			printf("parallelepiped + ");
			lepiped->controlMenu();
			break;

		case 'X': // exit
			closegraph();
			return 0;

		default:
			printf("unknown 1st command\n");
		}
	}

	closegraph();
	return 0;
}