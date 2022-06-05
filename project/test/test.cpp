#include <windows.h>
#include <GL/gl.h>
#include <GL/GL.H>
#include <GL/glut.h>
#include <GL/GLU.H>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <iterator>
#include <algorithm> 
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#define PI 3.1415926535898

using namespace std;

float const xmax = 20, xmin = 0, ymax = 10, ymin = 0;
int const width = 900, height = 700;
double  x, y;
float d = 0.15, d2 = 0.0001;

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xmin, xmax, ymin, ymax);
}

class Point {
public:double x, y;
	  Point() { x = 0; y = 0; }
	  Point(double x_, double y_) : x(x_), y(y_) {}
	  void setPoint(double x_, double y_) {
		  x = x_;
		  y = y_;
	  }
	  Point operator+ (Point const arg)
	  {
		  return Point(x + arg.x, y + arg.y);
	  }
	  Point operator- (Point const arg) {
		  return Point(x - arg.x, y - arg.y);
	  }
	  friend bool operator == (Point A, Point B)
	  {
		  return (A.x == B.x && A.y == B.y);
	  }
	  friend bool operator != (Point A, Point B)
	  {
		  return (A.x != B.x || A.y != B.y);
	  }
};

Point Convert(Point arg) {
	float scale_x = (xmax - xmin) / width;
	float scale_y = (ymax - ymin) / height;
	return Point(arg.x * scale_x + xmin, ymax - arg.y * scale_y);
}

vector<Point> v;
vector<Point> v2;
vector<Point> dot;//для проверки
vector<Point> temp;
vector<Point> cross;
vector<Point> sortCross;
bool isClosed = false;
int color = 2, q = 1;
bool color_poly = false;
bool per = false;

double psevdo_skal_p(Point A, Point B)
{
	return (A.x * B.y - A.y * B.x);
}

double scal_p(Point A, Point B)
{
	return (A.x * B.x + A.y * B.y);
}

//длина вектора от точки с координатой вектора
float Lenght(Point& argA) {
	return sqrt(pow(argA.x, 2) + pow(argA.y, 2));
}

void drawPoint(Point& argA)
{
	glPointSize(3);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 0);
	glVertex2f(argA.x, argA.y);
	glEnd();
	glFinish();
}

void drawLine(Point& argA, Point& argB) {
	glLineWidth(2);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(argA.x, argA.y);
	glVertex2f(argB.x, argB.y);
	glEnd();
	glFinish();
}

float distance(Point& argA, Point& argB) {
	return pow(pow((argB.x - argA.x), 2) + pow((argB.y - argA.y), 2), 0.5);
}

void face(void) {
	//координаты 6-угольника; можно изменить на другой выпуклый полигон + не забыть изменить рисовку (*)
	v.push_back(Point(4, 2));
	v.push_back(Point(2, 5));
	v.push_back(Point(4, 8));
	v.push_back(Point(10, 8));
	v.push_back(Point(12, 5));
	v.push_back(Point(10, 2));
	v.push_back(Point(4, 2));

	glLineWidth(2);
	// window
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	glVertex2f(1, 0.5);
	glVertex2f(13, 0.5);
	glVertex2f(13, 9.5);
	glVertex2f(1, 9.5);
	glEnd();
	//выпуклый полигон (6-угольник)
	// (*)
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(4, 2);
	glVertex2f(2, 5);
	glVertex2f(4, 8);
	glVertex2f(10, 8);
	glVertex2f(12, 5);
	glVertex2f(10, 2);
	glEnd();
	//color
	//red
	glColor3ub(176, 11, 11);
	glBegin(GL_POLYGON);
	glVertex2f(14, 7);
	glVertex2f(16, 7);
	glVertex2f(16, 9);
	glVertex2f(14, 9);
	glEnd();
	//purple
	glColor3ub(104, 11, 176);
	glBegin(GL_POLYGON);
	glVertex2f(14, 4);
	glVertex2f(16, 4);
	glVertex2f(16, 6);
	glVertex2f(14, 6);
	glEnd();
	//yellow
	glColor3ub(217, 185, 28);
	glBegin(GL_POLYGON);
	glVertex2f(14, 1);
	glVertex2f(16, 1);
	glVertex2f(16, 3);
	glVertex2f(14, 3);
	glEnd();
	//blue
	glColor3ub(30, 150, 200);
	glBegin(GL_POLYGON);
	glVertex2f(17, 7);
	glVertex2f(19, 7);
	glVertex2f(19, 9);
	glVertex2f(17, 9);
	glEnd();
	//green
	glColor3ub(88, 217, 28);
	glBegin(GL_POLYGON);
	glVertex2f(17, 4);
	glVertex2f(19, 4);
	glVertex2f(19, 6);
	glVertex2f(17, 6);
	glEnd();
	//pink
	glColor3ub(176, 11, 102);
	glBegin(GL_POLYGON);
	glVertex2f(17, 1);
	glVertex2f(19, 1);
	glVertex2f(19, 3);
	glVertex2f(17, 3);
	glEnd();

	glFinish();
}

void quads(void)
{
	glBegin(GL_QUADS);
	glColor3ub(128, 111, 176);
	glVertex2f(0, 10);
	glVertex2f(0, 0);

	glColor3f(152, 144, 173);
	glVertex2f(20, 0);
	glVertex2f(20, 10);

	glEnd();
	glFinish();
}

void vpuk(vector<Point> v) //он неотрицателен при обходе против часовой стрелки и неположителен при обходе по часовой стрелки.
{
	int q1 = 0, q2 = 0;
	for (int i = 0; i <= v.size(); i++)
	{
		if (i == v.size() - 1)
		{
			v[i].x = v[0].x;
			v[i].y = v[0].y;
			break;
		}
		else
		{
			v[i].x = v[i + 1].x - v[i].x;
			v[i].y = v[i + 1].y - v[i].y;
		}
	}
	for (int i = 0; i < v.size() - 1; i += 1)
	{
		if (psevdo_skal_p(v[i], v[i + 1]) > 0)
		{
			if (q2 == 0)
			{
				q1 += 1;
			}
			else
			{
				q1 = 1;
				break;
			}
		}
		else
		{
			if (q1 == 0)
			{
				q2 += 1;
			}
			else
			{
				q2 = 1;
				break;
			}
		}
	}
	if ((q1 == 0) or (q2 == 0))
	{
		cout << "1) Полигон выпуклый" << endl;
		if (q1 > 0)
		{
			cout << "2) Построен против ч.с." << endl;
			reverse(v.begin(), v.end());
		}
		else
		{
			cout << "2) Построен по ч.с." << endl;
		}
	}
	else
	{
		cout << "1) Полигон не выпуклый." << endl;
		exit(0);
	}

}

bool samoper(vector<Point> v)
{
	for (int i = 0; i < v.size() - 3; i++)
	{
		Point a((v[i + 1].x - v[i].x), (v[i + 1].y - v[i].y));
		for (int j = i + 2; j <= v.size() - 2; j++)
		{
			Point b((v[j].x - v[i].x), (v[j].y - v[i].y));
			Point c((v[j + 1].x - v[i].x), (v[j + 1].y - v[i].y));
			Point d((v[j + 1].x - v[j].x), (v[j + 1].y - v[j].y));
			Point e((v[i].x - v[j].x), (v[i].y - v[j].y));
			Point f((v[i + 1].x - v[j].x), (v[i + 1].y - v[j].y));

			if ((psevdo_skal_p(a, b) * psevdo_skal_p(a, c) < 0) && (psevdo_skal_p(d, e) * psevdo_skal_p(d, f) < 0))
			{
				cout << "Самопересечение1" << endl;
				//return true;
				exit(0);
			}
		}
	}
	return false;
}

int where_dot()//метод лучей
{
	int count = 0;
	for (int i = 1; i < v.size(); i++)
	{
		Point ba = Point(v[i].x - v[i - 1].x, v[i].y - v[i - 1].y);
		Point bc = Point(dot[dot.size() - 1].x - v[i - 1].x, dot[dot.size() - 1].y - v[i - 1].y);
		Point bd = Point(xmin - v[i - 1].x, dot[dot.size() - 1].y - v[i - 1].y);
		Point cd = Point(xmin - dot[dot.size() - 1].x, 0);
		Point ca = Point(v[i].x - dot[dot.size() - 1].x, v[i].y - dot[dot.size() - 1].y);
		Point cb = Point(v[i - 1].x - dot[dot.size() - 1].x, v[i - 1].y - dot[dot.size() - 1].y);

		if (((psevdo_skal_p(ba, bc) * psevdo_skal_p(ba, bd)) <= 0) && (psevdo_skal_p(cd, ca) * psevdo_skal_p(cd, cb) <= 0))
		{

			if (!(((dot[dot.size() - 1].y == v[i].y) && (v[i].y >= v[i - 1].y)) || ((dot[dot.size() - 1].y == v[i - 1].y) && (v[i - 1].y >= v[i].y))))
				count++;
		}
	}
	if (count % 2 == 1) {
		//cout << "\nInside";
		return 1;
	}
	else {
		//cout << "\nOutside";
		return 0;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	quads();
	face();
	glEnd();
	glFinish();
}

void Podarok()
{
	int minX = 0, maxIndexCos = 0;
	double maxCos = -1.0, c;
	for (int i = 0; i < cross.size(); i++)//левая нижняя точка
	{
		if (cross[i].x != cross[minX].x)
		{
			if (cross[i].x < cross[minX].x) {
				minX = i;
			}
		}
		else
		{
			if (cross[i].y < cross[minX].y)
				minX = i;
		}
	}
	Point cur = cross[minX];
	Point kon = Point(cross[minX].x, ymax);
	do
	{
		for (int i = 0; i < cross.size(); i++)
		{
			Point A = Point((kon - cur).x, (kon - cur).y);
			Point B = Point((cross[i] - cur).x, (cross[i] - cur).y);
			c = scal_p(A, B) / (Lenght(A) * Lenght(B));//cos
			if (c > maxCos)
			{
				maxCos = c;
				maxIndexCos = i;
			}
		}
		sortCross.push_back(cur);
		sortCross.push_back(cross[maxIndexCos]);
		kon = cross[maxIndexCos] + (cross[maxIndexCos] - cur);
		cur = cross[maxIndexCos];
		maxCos = -1;
	} while (!(cur == cross[minX]));
}

void poly_color(void) {
	if (!isClosed) {
		if (v2.size() > 1) {
			//замыкание фигуры
			if (distance(v2[0], v2[v2.size() - 1]) < d && v2.size() > 2) {
				isClosed = true;
				v2.pop_back();
				v2.push_back(v2[0]);
				drawLine(v2[v2.size() - 1], v2[v2.size() - 2]);
				if (!samoper(v2))
				{
					cout << endl << "Цветной полигон: " << endl;
					vpuk(v2);
				}
			}
			else {
				//не рисовать точку в 2-ух предыдущих вершинах
				if (distance(v2[v2.size() - 1], v2[v2.size() - 2]) < d) {
					v2.pop_back();
					cout << "Vector draw!" << endl;
					return;
				}
				if (v2.size() >= 3) {
					if (distance(v2[v2.size() - 1], v2[v2.size() - 3]) < d) {
						v2.pop_back();
						cout << "Vector draw!!" << endl;
						return;
					}
				}
				//если фигуру не замыкаем и не ставим точку в 2ух предыдущих то рисуем
				drawLine(v2[v2.size() - 1], v2[v2.size() - 2]);
			}
		}
		//рисуем точку
		drawPoint(v2[v2.size() - 1]);
		glFinish();
	}
	glutSwapBuffers();
	if (color_poly)//после поиска точек рисуется полигон пересечений
	{
		if (!cross.empty()) {
			Podarok();
			glLineWidth(2);
			switch (color)
			{
			case 1:
				glColor3ub(176, 11, 11);
				break;
			case 2:
				glColor3ub(104, 11, 176);
				break;
			case 3:
				glColor3ub(217, 185, 28);
				break;
			case 4:
				glColor3ub(30, 150, 200);
				break;
			case 5:
				glColor3ub(88, 217, 28);
				break;
			case 6:
				glColor3ub(176, 11, 102);
				break;
			}
			glBegin(GL_POLYGON);
			for (int i = 0; i < sortCross.size() - 1; i++)
			{
				glVertex2f(sortCross[i].x, sortCross[i].y);
			}
			glEnd();
			glFinish();
		}
		sortCross.clear();
	}
	glFinish();
}

Point crossPoint(Point begin, Point end, Point begin2, Point end2) //точка пересечения
{
	Point tochk;
	double tochkX, tochkY = 0;
	double a = begin.y - end.y;
	double b = end.x - begin.x;
	double c = begin.x * end.y - end.x * begin.y;
	double a2 = begin2.y - end2.y;
	double b2 = end2.x - begin2.x;
	double c2 = begin2.x * end2.y - end2.x * begin2.y;
	double del = a * b2 - a2 * b;
	tochkX = (c2 * b - c * b2) / del;
	tochkY = (a2 * c - a * c2) / del;
	tochk.setPoint(tochkX, tochkY);
	return tochk;
}

void poisk(void)
{
	int pro = 0;
	color_poly = true;
	dot.clear();
	temp = v;
	v.clear();
	v = v2;
	for (int i = 0; i < temp.size(); i++)
	{
		dot.push_back(temp[i]);
		if (where_dot())
		{
			cross.push_back(temp[i]);
		}
	}
	poly_color();
	v.clear();
	v = temp;
	dot.clear();

	if (per) {
		cout << "Полигоны пересекаются" << endl;
	}
	else
	{
		dot.clear();
		for (int i = 0; i < v2.size(); i++)
		{
			dot.push_back(v2[i]); //сколько вершин цветного полигона в прямоугольнике
			if (where_dot())
			{
				pro++;
			}
		}
		if (pro == v2.size())
		{
			cout << "Цветной полигон внутри" << endl;
			return;
		}
		else
		{
			pro = 0;
			dot.clear();
			temp = v;
			v.clear();
			v = v2;
			for (int i = 0; i < temp.size(); i++)
			{
				dot.push_back(temp[i]);
				if (where_dot())
				{
					pro++;
				}
			}
			v.clear();
			v = temp;
			dot.clear();

		}
		if (pro == temp.size())
		{
			cout << "Прямоугольник содержится в цветном полигоне" << endl;
		}
		else {
			cout << "Цветной полигон снаружи" << endl;
		}
	}
}

void tochki(void)
{
	bool check = false;
	Point ab, cd, ac, ad, ca, cb, p1, p2, p3, p4;
	for (int j = 0; j < v.size() - 1; j++)
	{
		ab.setPoint(v2[v2.size() - 1].x - v2[v2.size() - 2].x, v2[v2.size() - 1].y - v2[v2.size() - 2].y);
		cd.setPoint(v[j + 1].x - v[j].x, v[j + 1].y - v[j].y);
		ac.setPoint(v[j].x - v2[v2.size() - 2].x, v[j].y - v2[v2.size() - 2].y);
		ad.setPoint(v[j + 1].x - v2[v2.size() - 2].x, v[j + 1].y - v2[v2.size() - 2].y);
		ca.setPoint(v2[v2.size() - 2].x - v[j].x, v2[v2.size() - 2].y - v[j].y);
		cb.setPoint(v2[v2.size() - 1].x - v[j].x, v2[v2.size() - 1].y - v[j].y);

		p1.setPoint(v2[v2.size() - 2].x, v2[v2.size() - 2].y);
		p2.setPoint(v2[v2.size() - 1].x, v2[v2.size() - 1].y);
		p3.setPoint(v[j].x, v[j].y);
		p4.setPoint(v[j + 1].x, v[j + 1].y);

		if ((psevdo_skal_p(ab, ac) * psevdo_skal_p(ab, ad) < 0) && (psevdo_skal_p(cd, ca) * psevdo_skal_p(cd, cb) < 0))
		{
			per = true;
			check = true;
			Point p = crossPoint(p1, p2, p3, p4);
			cross.push_back(p);
			dot.clear();
			dot.push_back(v2[v2.size() - 1]);
			if (where_dot()) //проверка последней v2 с прямоугольником 
				cross.push_back(v2[v2.size() - 1]);
		}
	}

	if (!check) //если не пересекаются
	{
		dot.clear();
		dot.push_back(v2[v2.size() - 1]);
		if (where_dot())
			cross.push_back(v2[v2.size() - 1]);
	}
	check = false;
}

void mouse(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && (!isClosed))
	{
		Point coord = Convert(Point(x, y));
		if (coord.x >= 14 && coord.x <= 16) { // 1 столбик
			if (coord.y >= 7 && coord.y <= 9) {//red
				color = 1;
			}
			else if (coord.y >= 4 && coord.y <= 6) {//purple
				color = 2;
			}
			else color = 3;//yellow

		}
		if (coord.x >= 17 && coord.x <= 19) { // 2 столбик
			if (coord.y >= 7 && coord.y <= 9) {//blue
				color = 4;
			}
			else if (coord.y >= 4 && coord.y <= 6) {//green
				color = 5;
			}
			else color = 6; // pink

		}
		else if ((coord.x >= 1.2 && coord.x <= 12.8) && (coord.y >= 0.75 && coord.y <= 9.25)) {
			v2.push_back(coord);
			poly_color();//рисуем полигон
			if (v2.size() > 1) {
				tochki();//ищем точки пересечения ребер полигона с прямоугольником
			}
		}
	}
	if ((isClosed) && (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		poisk();
		poly_color();//рисуем цветной полигон
		v2.clear();
		dot.clear();
		cross.clear();
		color_poly = false;
		per = false;
		isClosed = false;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Paint on minimalki");
	glutDisplayFunc(display);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_FLAT);
	glutReshapeFunc(reshape);
	if (!isClosed)
		glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
}