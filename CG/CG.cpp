// CG.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// Сделать две линии
// Перемешать, увеличивать, вращать по кнопке или клавише

#include <iostream>
#include <cmath>
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

; using namespace std;

#define STEP 10 // шаг перемещения
#define SCALE 1 // коэфицент масштабирования
#define CORNER 3 // угол поворота
#define M_PI 3.1415926 // число пи

class LINE {
public:
	double x0, x1, y0, y1; // координаты
	colors color = WHITE; // цвет линии (по умолчанию - белый)

	void lineBresenham();
	void renderLine();
	void moveLine(char);
	void rotateLine(char);
	void scalingLine(char);
};

void LINE::moveLine(char direction) {
	switch (direction) {
	case 'u':
		this->y0 -= STEP;
		this->y1 -= STEP;
		break;
	case 'd':
		this->y0 += STEP;
		this->y1 += STEP;
		break;
	case 'l':
		this->x0 -= STEP;
		this->x1 -= STEP;
		break;
	case 'r':
		this->x0 += STEP;
		this->x1 += STEP;
		break;
	}
}
void LINE::rotateLine(char direction) {
	double eps = M_PI / 180;  //Это для последующего перевода в радианы

	double Y = (y0 + y1) / 2;  //Ищем точку Y центра отрезка
	double X = (x0 + x1) / 2;  //Ищем точку X центра отрезка

	double y, x;  //Это для удобства чтения
	double tempx0, tempx1, tempy0, tempy1; //Для того, чтобы сблизить функции draw() и clear()

	switch (direction) {
	case 'r':
		x = x0, y = y0;
		tempx0 = -sin(CORNER * eps) * (y - Y) + cos(CORNER * eps) * (x - X) + X;
		tempy0 = cos(CORNER * eps) * (y - Y) + sin(CORNER * eps) * (x - X) + Y;

		/*РАСЧЁТ ПО ВТОРОЙ ТОЧКЕ*/
		x = x1, y = y1;
		tempx1 = -sin(CORNER * eps) * (y - Y) + cos(CORNER * eps) * (x - X) + X;
		tempy1 = cos(CORNER * eps) * (y - Y) + sin(CORNER * eps) * (x - X) + Y;
		break;
	case 'l':
		x = x0, y = y0;
		tempx0 = -sin(-CORNER * eps) * (y - Y) + cos(-CORNER * eps) * (x - X) + X;
		tempy0 = cos(-CORNER * eps) * (y - Y) + sin(-CORNER * eps) * (x - X) + Y;

		/*РАСЧЁТ ПО ВТОРОЙ ТОЧКЕ*/
		x = x1, y = y1;
		tempx1 = -sin(-CORNER * eps) * (y - Y) + cos(-CORNER * eps) * (x - X) + X;
		tempy1 = cos(-CORNER * eps) * (y - Y) + sin(-CORNER * eps) * (x - X) + Y;
		break;
	}
	x0 = tempx0, x1 = tempx1, y0 = tempy0, y1 = tempy1; //обновление координат

}
// масштабирование
void LINE::scalingLine(char direction) {
	switch (direction) {
	case 'u':
		if (x0 > x1) {
			x0 += SCALE;
			x1 -= SCALE;
		}
		else if (x0 < x1) {
			x0 -= SCALE;
			x1 += SCALE;
		}


		if (y0 > y1) {
			y0 += SCALE;
			y1 -= SCALE;
		}
		else if (y0 < y1) {
			y0 -= SCALE;
			y1 += SCALE;
		}
		break;
	case 'd':
		if (abs(x0 - x1) <= 2 * SCALE && abs(y0 - y1) <= 2 * SCALE)
			break;

		if (x0 > x1) {
			x0 -= SCALE;
			x1 += SCALE;
		}
		else if (x0 < x1) {
			x0 += SCALE;
			x1 -= SCALE;
		}


		if (y0 > y1) {
			y0 -= SCALE;
			y1 += SCALE;
		}
		else if (y0 < y1) {
			y0 += SCALE;
			y1 -= SCALE;
		}
		break;
	}
}
// рисуем брезенхэмом
void LINE::lineBresenham() {
	int moveX0, moveY0, moveX1, moveY1;
	moveX0 = x0;
	moveY0 = y0;
	moveX1 = x1;
	moveY1 = y1;
	
	//устрановка цвета
	setcolor(color);
	//алгоритм
	int dx = abs(moveX1 - moveX0);
	int dy = abs(moveY1 - moveY0);
	const int signX = moveX0 < moveX1 ? 1 : -1;
	const int signY = moveY0 < moveY1 ? 1 : -1;
	float error = dx - dy;
	float dError;
	putpixel(moveX1, moveY1, color);

	while (moveX0 != moveX1 || moveY0 != moveY1) {
		putpixel(moveX0, moveY0, color);
		int dError = error * 2;
		if (dError > -dy) {
			error -= dy;
			moveX0 += signX;
		}
		if (dError < dx) {
			error += dx;
			moveY0 += signY;
		}
	}
}


// рисует линию
void LINE::renderLine() {
	setcolor(color);
	line(x0, y0, x1, y1);
}

int main() {
	initwindow(500, 500);

	LINE ln[2];

	ln[0].x0 = 50;
	ln[0].x1 = 70;
	ln[0].y0 = 150;
	ln[0].y1 = 170;
	ln[0].color = WHITE;

	ln[1].x0 = 170;
	ln[1].x1 = 120;
	ln[1].y0 = 150;
	ln[1].y1 = 200;
	ln[1].color = YELLOW;


	// какая линия будет изменять координаты
	int activeIndex = 0;
	// какой вид смещения будет происходить: 0 - перемещение | 1 - поворот | 2 - масштабирование
	int mode = 0;

	// рисуем линии
	cout << "color: " << ln[0].color << endl;
	cout << "color2: " << ln[1].color << endl;

	ln[0].renderLine();
	ln[1].lineBresenham();

	cout << "color: " << ln[0].color << endl;
	cout << "color2: " << ln[1].color << endl;



	while (true)
	{
		if (kbhit())
		{
			int keyUp = getch();

			switch (keyUp) {

				// меняем активную линию
			case VK_TAB:
				activeIndex = activeIndex == 0 ? 1 : 0;
				break;

				// меняем мод
			case 49: // клавиша 1
				mode = 0; 
				break;
			case 50: // клавиша 2
				mode = 1; 
				break;
			case 51: // клавиша 3
				mode = 2; 
				break;

				//перемещение линий осуществляется на стрелочках
			case 72:
				if (mode == 0) { ln[activeIndex].moveLine('u'); } // move up
				if (mode == 2) { ln[activeIndex].scalingLine('u'); } // scaling - увеличиваем
				break;
			case 80:
				if (mode == 0) { ln[activeIndex].moveLine('d'); } // move up
				if (mode == 2) { ln[activeIndex].scalingLine('d'); } // scaling - уменьшаем
				break;
			case 75:
				if (mode == 0) { ln[activeIndex].moveLine('l'); } // move left
				if (mode == 1) { ln[activeIndex].rotateLine('l'); } // rotation left
				break;
			case 77:
				if (mode == 0) { ln[activeIndex].moveLine('r'); } // move right
				if (mode == 1) { ln[activeIndex].rotateLine('r'); } // rotation right
				break;
			}
			// При нажатии на esc выходим
			if (keyUp == 27) { break; }
			// чистим окно
			cleardevice();
			// рисуем линии
			cout << "color: " << ln[0].color << endl;
			cout << "color: " << ln[1].color << endl;

			ln[0].renderLine();
 			ln[1].lineBresenham();

			cout << "color: " << ln[0].color << endl;
			cout << "color: " << ln[1].color << endl;


		}


	}
	closegraph();
	return 0;
}