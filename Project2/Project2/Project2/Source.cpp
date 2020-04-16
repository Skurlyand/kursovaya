#include <iostream>
#include <cmath>
#include <glut.h>
using namespace std;
float acc;
float v;
float xinit;
float tinit;
int choice;
double A(double x, double y1, double y2, double y3, double y4) {
	return y3;
}
double B(double x, double y1, double y2, double y3, double y4) {
	return y4;
}
double C(double x, double y1, double y2, double y3, double y4) {
	return acc*y4;
}
double D(double x, double y1, double y2, double y3, double y4) {
	return acc*y3;
}

void init(void)
{
	glClearColor(0, 0, 0, 0); //Выбор фонового цвета (RGBA)
	glMatrixMode(GL_PROJECTION); //Установить проекцию и говорит о том, что команды относятся к проекту
	glLoadIdentity(); //считывает текущую матрицу
}
void display(void)
{
	glColor3f(0, 1, 0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glLineWidth(1);
	float a = 0.7;
	//координатная сетка
	glBegin(GL_LINES);
	glVertex3f(a, a, 0.0);
	glVertex3f(-a, -a, 0.0);
	glVertex3f(-a, a, 0.0);
	glVertex3f(a, -a, 0.0);

	glVertex3f(a, a, 0.0);
	glVertex3f(-a, a, 0.0);

	glVertex3f(-a, a, 0.0);
	glVertex3f(-a, -a, 0.0);

	glVertex3f(-a, -a, 0.0);
	glVertex3f(a, -a, 0);

	glVertex3f(a, -a, 0.0);
	glVertex3f(a, a, 0.0);

	float p0, q0;
	p0 = 2 * a*atan(-50) / 3.14;
	q0 = 2 * a*atan(-50) / 3.14;
	for (float j = -5; j <= 5; j += 0.25)
	{
		for (float i = -50; i < 50; i += 0.01)
		{
			glVertex3f(p0, q0, 0.0);
			p0 = 2 * a*atan(i + j) / 3.14;
			q0 = 2 * a*atan(i - j) / 3.14;
			if ((p0 > a) || (q0 > a) || (q0 < -a) || (p0 < -a)) break;
			glVertex3f(p0, q0, 0.0);
		}
	}
	for (float i = -5; i <= 5; i += 0.25)
	{
		for (float j = -50; j < 50; j += 0.01)
		{
			glVertex3f(p0, q0, 0.0);
			p0 = 2 * a*atan(i + j) / 3.14;
			q0 = 2 * a*atan(i - j) / 3.14;
			if ((p0 > a) || (q0 > a) || (q0 < -a) || (p0 < -a)) break;
			glVertex3f(p0, q0, 0.0);
		}
	}
	glEnd();

	if (choice == 1)
	{
		//постоянная скорость
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3f(0.6, 0.3, 0.8);
		float p, q;
		p = 2 * a*atan(-(1 + v) * 50) / 3.14;
		q = 2 * a*atan(-(1 - v) * 50) / 3.14;
		for (float i = -50; i < 50; i += 0.01)
		{
			glVertex3f(p, q, 0.0);
			p = 2 * a*atan((1 + v)*i) / 3.14;
			q = 2 * a*atan((1 - v)*i) / 3.14;
			if ((p > a) || (q > a) || (q < -a) || (p < -a)) break;
			glVertex3f(p, q, 0.0);
		}
		glEnd();
		glFlush();
	}

	//ускорение
	if (choice == 2)
	{
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3f(0.6, 0.3, 0.8);

		double init = 0; double b = 100; double h = 0.0001;
		double n = (b - init) / h;
		//размер системы
		int c = 4;
		// Выделение памяти для массива
		double *X = new double[n];//столбец параметра
		//иттерационные функции
		double **F1 = new double*[c];
		for (int i = 0; i < c; i++)
		{
			F1[i] = new double[n];
		}
		double **F2 = new double*[c];
		for (int i = 0; i < c; i++)
		{
			F2[i] = new double[n];
		}
		double **F3 = new double*[c];
		for (int i = 0; i < c; i++)
		{
			F3[i] = new double[n];
		}
		double **F4 = new double*[c];
		for (int i = 0; i < c; i++)
		{
			F4[i] = new double[n];
		}
		//вывод решения
		double *Y1 = new double[n];
		double *Y2 = new double[n];
		double *Y3 = new double[n];
		double *Y4 = new double[n];


		//подсчет
		int i = 0;
		X[0] = init; Y1[0] = tinit; Y2[0] = xinit; Y3[0] = pow((1 - v * v), (-0.5)); Y4[0] = v * v / pow((1 - v * v), (-0.5));
		for (int j = 1; j <= n; j++) {
			X[j] = init + j * h;

			F1[i][j] = h * A(X[j - 1], Y1[j - 1], Y2[j - 1], Y3[j - 1], Y4[j - 1]);
			F1[i + 1][j] = h * B(X[j - 1], Y1[j - 1], Y2[j - 1], Y3[j - 1], Y4[j - 1]);
			F1[i + 2][j] = h * C(X[j - 1], Y1[j - 1], Y2[j - 1], Y3[j - 1], Y4[j - 1]);
			F1[i + 3][j] = h * D(X[j - 1], Y1[j - 1], Y2[j - 1], Y3[j - 1], Y4[j - 1]);


			F2[i][j] = h * A(X[j - 1] + h / 2.0, Y1[j - 1] + F1[i][j] / 2.0, Y2[j - 1] + F1[i + 1][j] / 2.0, Y3[j - 1] + F1[i + 2][j] / 2.0, Y4[j - 1] + F1[i + 3][j] / 2.0);
			F2[i + 1][j] = h * B(X[j - 1] + h / 2.0, Y1[j - 1] + F1[i][j] / 2.0, Y2[j - 1] + F1[i + 1][j] / 2.0, Y3[j - 1] + F1[i + 2][j] / 2.0, Y4[j - 1] + F1[i + 3][j] / 2.0);
			F2[i + 2][j] = h * C(X[j - 1] + h / 2.0, Y1[j - 1] + F1[i][j] / 2.0, Y2[j - 1] + F1[i + 1][j] / 2.0, Y3[j - 1] + F1[i + 2][j] / 2.0, Y4[j - 1] + F1[i + 3][j] / 2.0);
			F2[i + 3][j] = h * D(X[j - 1] + h / 2.0, Y1[j - 1] + F1[i][j] / 2.0, Y2[j - 1] + F1[i + 1][j] / 2.0, Y3[j - 1] + F1[i + 2][j] / 2.0, Y4[j - 1] + F1[i + 3][j] / 2.0);

			F3[i][j] = h * A(X[j - 1] + h / 2.0, Y1[j - 1] + F2[i][j] / 2.0, Y2[j - 1] + F2[i + 1][j] / 2.0, Y3[j - 1] + F2[i + 2][j] / 2.0, Y4[j - 1] + F2[i + 3][j] / 2.0);
			F3[i + 1][j] = h * B(X[j - 1] + h / 2.0, Y1[j - 1] + F2[i][j] / 2.0, Y2[j - 1] + F2[i + 1][j] / 2.0, Y3[j - 1] + F2[i + 2][j] / 2.0, Y4[j - 1] + F2[i + 3][j] / 2.0);
			F3[i + 2][j] = h * C(X[j - 1] + h / 2.0, Y1[j - 1] + F2[i][j] / 2.0, Y2[j - 1] + F2[i + 1][j] / 2.0, Y3[j - 1] + F2[i + 2][j] / 2.0, Y4[j - 1] + F2[i + 3][j] / 2.0);
			F3[i + 3][j] = h * D(X[j - 1] + h / 2.0, Y1[j - 1] + F2[i][j] / 2.0, Y2[j - 1] + F2[i + 1][j] / 2.0, Y3[j - 1] + F2[i + 2][j] / 2.0, Y4[j - 1] + F2[i + 3][j] / 2.0);

			F4[i][j] = h * A(X[j - 1] + h, Y1[j - 1] + F3[i][j], Y2[j - 1] + F3[i + 1][j], Y3[j - 1] + F3[i + 2][j], Y4[j - 1] + F3[i + 3][j]);
			F4[i + 1][j] = h * B(X[j - 1] + h, Y1[j - 1] + F3[i][j], Y2[j - 1] + F3[i + 1][j], Y3[j - 1] + F3[i + 2][j], Y4[j - 1] + F3[i + 3][j]);
			F4[i + 2][j] = h * C(X[j - 1] + h, Y1[j - 1] + F3[i][j], Y2[j - 1] + F3[i + 1][j], Y3[j - 1] + F3[i + 2][j], Y4[j - 1] + F3[i + 3][j]);
			F4[i + 3][j] = h * D(X[j - 1] + h, Y1[j - 1] + F3[i][j], Y2[j - 1] + F3[i + 1][j], Y3[j - 1] + F3[i + 2][j], Y4[j - 1] + F3[i + 3][j]);

			Y1[j] = Y1[j - 1] + (F1[i][j] + 2 * F2[i][j] + 2 * F3[i][j] + F4[i][j]) / 6;
			Y2[j] = Y2[j - 1] + (F1[i + 1][j] + 2 * F2[i + 1][j] + 2 * F3[i + 1][j] + F4[i + 1][j]) / 6;
			Y3[j] = Y3[j - 1] + (F1[i + 2][j] + 2 * F2[i + 2][j] + 2 * F3[i + 2][j] + F4[i + 2][j]) / 6;
			Y4[j] = Y4[j - 1] + (F1[i + 3][j] + 2 * F2[i + 3][j] + 2 * F3[i + 3][j] + F4[i + 3][j]) / 6;
		}
		float p, q;
		p = 2 * a*atan(Y1[0] + Y2[0]) / 3.14;
		q = 2 * a*atan(Y1[0] - Y2[0]) / 3.14;
		for (int i = 0; i <= n; i++)
		{
			glVertex3f(p, q, 0.0);
			p = 2 * a*atan(Y1[i] + Y2[i]) / 3.14;
			q = 2 * a*atan(Y1[i] - Y2[i]) / 3.14;
			if ((p > a) || (q > a) || (q < -a) || (p < -a)) break;
			glVertex3f(p, q, 0.0);
		}
		glEnd();
	}

	glFlush();//выполнение буферизации
}
int main(int argc, char **argv)
{

	setlocale(LC_ALL, "Russian");
	cout << "Выберите кривую" << endl;
	cout << "1 Постоянная скорость" << endl;
	cout << "2 Постоянное ускорение" << endl;
	cin >> choice;
	cout << "Введите начальную скорость (в единицах с)" << endl;
	cin >> v;
	if ((v > 1) || (v < -1))
	{
		cout << "Слишком быстро" << endl;
		system("Pause");
	}
	if ((choice == 2) && ((v!=1) || (v!=-1)))
	{
		cout << "Введите начальную временную координату" << endl;
		cin >> tinit;
		cout << "Введите начальную пространственную координату"<<endl;
		cin >> xinit;
		cout << "Введите ускорение" << endl;
		cin >> acc;
	}
	glutInit(&argc, argv); //установка начальных характеристик окна
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // выбор статики и установка цветового поля
	glutInitWindowSize(500, 500); //размер окна
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Progga"); //открытие окна с заголовком
	init();
	glutDisplayFunc(display);//зарегистрировать дисплейную функцию обратного вызова
	glutMainLoop();//войти в главный цикл
	return 0;
}
