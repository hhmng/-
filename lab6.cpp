#include <windows.h>
#include <GL/gl.h>
#include <gl/GL.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>


using namespace std;

const int wight = 700;
const int height = 700;
const int x_min = -1;
const int x_max = 10;
const int y_min = -1;
const int y_max = 10;
const double chisl = 0.4;

class Point {
public:double x, y;
      Point() { x = 0; y = 0; }
      Point(double x_, double y_) : x(x_), y(y_) {}
      void setPoint(double x_, double y_) {
          x = x_;
          y = y_;
      }
      Point operator- (Point const arg) {
          return Point(x - arg.x, y - arg.y);
      }

};

void drawPoint(Point& argA) {
    glPointSize(6);
    glColor3f(0.9, 0.48, 0.26);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(argA.x, argA.y);
    glEnd();
    glFinish();
}


vector <Point> Points;
vector <Point> obolochka;

Point convert(Point arg) {
    double scale_x = (x_max - x_min) / (double)wight;
    double scale_y = (y_max - y_min) / (double)height;
    return Point(arg.x * scale_x + x_min, -arg.y * scale_y + y_max);
}

double distance(Point A, Point B) {
    return pow(B.x - A.x, 2) + pow(B.y - A.y, 2);
}



void drawGrid() {//fon
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(x_min, x_max, y_min, y_max);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(2.0);
    glColor3f(0.6, 0.0, 1.0);
    glLineStipple(1, 0x0101);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    for (int i = x_min; i < x_max; i++) {
        glVertex2f(i, y_min);
        glVertex2f(i, y_max);
    }
    for (int i = ceil(x_min); i < x_max; i++) {
        glVertex2f(x_min, i);
        glVertex2f(x_max, i);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2f(0, y_min);
    glVertex2f(0, y_max);
    glVertex2f(x_min, 0);
    glVertex2f(x_max, 0);

    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.1, 9.7);
    glVertex2f(0, y_max);
    glVertex2f(0.1, 9.7);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(9.7, -0.1);
    glVertex2f(x_max, 0);
    glVertex2f(9.7, 0.1);
    glEnd();
    glFinish();
}


void DrawLine(Point a, Point b) {
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glEnd();
    glFinish();
}

double psevdo(Point A, Point B, Point C) {
    return (B - A).x * (C - B).y - (B - A).y * (C - B).x;
}

double skal(Point A, Point B, Point C) {
    return (B - A).x * (C - A).x + (B - A).y * (C - A).y;
}

void DrawObolochka(vector <Point>& obol) {//соединяем точки оболочки
    glLineWidth(2.0);
    glColor3f(255, 255, 255);
    glLineStipple(1, 0x0101);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < obol.size(); i++) {
        glVertex2f(obol[i].x, obol[i].y);
    }

    glEnd();
    glFinish();
    for (int i = 0; i < obol.size(); i++) {
        drawPoint(obol[i]);
    }
}

//определяем в оболочке точка или нет. Если нет по будем перерисовывать оболочку
bool dot_in_obolochka(Point A) {
    double angle = 0;
    int n = obolochka.size();
    double a;
    double b;
    double cosinus;
    for (int i = 0; i < n - 1; i++) {
        a = sqrt(distance(A, obolochka[i]));
        b = sqrt(distance(A, obolochka[i + 1]));
        cosinus = skal(A, obolochka[i], obolochka[i + 1]) / (a * b);//косинус угла 
        if (cosinus < -1) {
            cosinus = -1;
        }
        else if (cosinus > 1) {
            cosinus = 1;
        }
        if (psevdo(A, obolochka[i], obolochka[i + 1]) <= 0) {

            angle -= acos(cosinus);
        }
        else {
            angle += acos(cosinus);
        }
    }
    a = sqrt(distance(A, obolochka[n - 1]));
    b = sqrt(distance(A, obolochka[0]));
    cosinus = skal(A, obolochka[n - 1], obolochka[0]) / (a * b);//проверили последнее ребро
    if (cosinus < -1) {
        cosinus = -1;
    }
    else if (cosinus > 1) {
        cosinus = 1;
    }
    if (psevdo(A, obolochka[n - 1], obolochka[0]) <= 0) {
        angle -= acos(cosinus);
    }
    else {
        angle += acos(cosinus);
    }
    cout << "u = " << abs(angle) << endl;
    return (abs(angle) < 0.1);//если тру - точка внешняя(т.е. сумма углов равна 0) если фолс - внутренняя(+- 2пи)
}


void RebuildingObolochka(Point A) {
    // индексы "левой" и "правой" точек не определены
    int rightindex = -1;
    int leftindex = -1;

    bool direction = signbit(psevdo(A, obolochka[0], obolochka[1]));//возвращяет знак(1 если знак отрицательный, 0 когда знак положительный)
    bool IsIndexFinfed = true;
    //определяем направление поворота с помощбю псевдо
    int n = obolochka.size();
    for (int j = 2; j < n; j++) {
        if (direction != signbit(psevdo(A, obolochka[0], obolochka[j]))) {
            IsIndexFinfed = false;
            break;
        }
    }

    // в случае, если предыдущий цикл прошёл все итерации, то нашлась либо левая, либо правая точка
     // direction = 0, тогда поворот против часовой от нулевой точки до всех остальных, V[0] - правая точка, весь полигон расположен по левую сторону от вектора AV[0]
     // direction = 1, тогда поворот  по чавсовой от нулевой точки до всех остальных, точка V[0] - левая точка , весь полигон расположен по правую сторону от вектора AV[0]
    if (IsIndexFinfed) {
        if (direction) {
            rightindex = 0;
        }
        else
        {
            leftindex = 0;
        }
    }

    for (int i = 1; i < obolochka.size(); i++) {
        IsIndexFinfed = true;
        direction = signbit(psevdo(A, obolochka[i], obolochka[0]));//опять проверили обход от итой до 0
        for (int j = 0; j < obolochka.size(); j++) {
            if (i != j) {
                if (direction != signbit(psevdo(A, obolochka[i], obolochka[j]))) {//опять проверили обход от итой до всех остальных совпадает ли с дирекшен?
                    IsIndexFinfed = false;
                    break;
                }
            }
        }

        if (IsIndexFinfed) {
            if (direction) {
                rightindex = i;
            }
            else
            {
                leftindex = i;
            }
        }
        // если индексы левой и правой точек определены, то их дальнейшее нахождение не нужно
        if (rightindex != -1 && leftindex != -1) {
            break;
        }
    }

    // левая и правая точки определены, сделаем переиндексацию чтобы правая точка имела индекс 0

    vector <Point> obolochkacopy = obolochka;
    n = obolochkacopy.size();
    cout << "n = " << n << endl;
    for (int i = 0; i < n; i++) {
        if (i + rightindex < n) {
            obolochka[i] = obolochkacopy[i + rightindex];
        }
        else {
            obolochka[i] = obolochkacopy[i - (n - rightindex)];
        }
    }

    leftindex -= rightindex;
    if (leftindex < 0) {
        leftindex += n;
    }
    rightindex = 0;

    obolochka.erase(obolochka.begin() + leftindex + 1, obolochka.end());//удаляем ненужные точки
    obolochka.push_back(A);


    drawGrid();
    DrawObolochka(obolochka);

}
void display(void)
{
    drawGrid();
}



void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        Point temp(x, y);
        temp = convert(temp);
        if (Points.size() == 0) {
            Points.push_back(temp);
            drawPoint(Points[0]);
        }
        else {
            if (distance(Points[0], temp) > chisl * chisl) {
                if (Points.size() == 1) {
                    Points.push_back(temp);
                    DrawLine(Points[0], Points[1]);
                    drawPoint(Points[0]);
                    drawPoint(Points[1]);
                }
                else if (Points.size() == 2) {
                    Points.push_back(temp);
                    if (psevdo(Points[0], Points[1], Points[2]) > 0) {
                        reverse(Points.begin(), Points.end());

                        //обход по часовой
                    }

                    obolochka = Points;
                    DrawLine(Points[0], Points[1]);
                    DrawLine(Points[1], Points[2]);
                    DrawLine(Points[2], Points[0]);
                    drawPoint(Points[0]);
                    drawPoint(Points[1]);
                    drawPoint(Points[2]);
                }

                // построение выпуклой оболочки

                // если размер оболочки >= 3
                else {

                    // если точка не пренадлежит оболочке
                    if (dot_in_obolochka(temp)) {
                        // перестроение оболочки
                        RebuildingObolochka(temp);
                    }
                    else {
                        cout << "\nпринадлежит оболочке";
                    }
                    Points.push_back(temp);
                    drawPoint(temp);
                }
            }
        }
    }

}
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    glutInitWindowSize(wight, height);
    glutInitWindowPosition(800, 50);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("risuno4ek");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glClearColor(6.0, 0.0, 1.0, 0);
    glutMainLoop();
    system("pause");
    return 0;
}