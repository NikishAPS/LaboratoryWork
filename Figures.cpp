#include "pch.h"
#include <stdio.h>
#include <iostream>
#define pi 3.14159265359



struct Point
{
public:
	float x, y;
	Point() {}

	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	float Length(Point p1, Point p2)
	{
		return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	}
};





class Figure
{
protected:
	int pointsCount;
	Point *points;

	void SetPoints(Point* points, int pointsCount)
	{
		this->pointsCount = pointsCount;
		this->points = points;
	}
public:

	Figure()
	{
		points = nullptr;
	}

	Figure(Point *points, int pointsCount)
	{
		this->points = points;
		this->pointsCount = pointsCount;
	}

	~Figure()
	{
		delete[] points;
	}

	virtual float GetSquare()
	{
		//Формула Гаусса
		float group1 = 0.0, group2 = 0.0;
		
		for (int i = 0; i < pointsCount - 1; i++)
		{
			group1 += points[i].x * points[i + 1].y;
			group2 += points[i + 1].x * points[i].y;
		}


		group1 += points[pointsCount - 1].x * points[0].y;
		group2 += points[0].x * points[pointsCount - 1].y;


		float s = abs(group1 - group2) * 0.5f;
		return s;
	}

	virtual float GetPerimeter()
	{
		float p = 0;
		for (int i = 0; i < pointsCount - 1; i++)
		{
			p += sqrt(pow(points[i].x - points[i + 1].x, 2) + pow(points[i].y - points[i + 1].y, 2));
		}
		p += sqrt(pow(points[pointsCount - 1].x - points[0].x, 2) + pow(points[pointsCount - 1].y - points[0].y, 2));

		return p;
	}

	virtual void GetInfo()
	{
		printf("Фигура с координатами вершин:\n");
		for (int i = 0; i < pointsCount; i++)
		{
			printf("%f, %f\n", points[i].x, points[i].y);
		}
		//printf("\n");
	}
};

class Rectangle : public Figure
{
public:
	Rectangle() {}

	Rectangle(float a, float b)
	{
		SetPoints(new Point[4]{ Point(0, 0), Point(a, 0), Point(a, b), Point(0, b) }, 4);
	}

	void GetInfo()
	{
		printf("Прямоугольник со сторонами a = %f и b = %f\n", points[2].x, points[2].y);
	}
};

class Square : public Rectangle
{
private:

public:
	Square() {}

	Square(float a)
	{
		SetPoints(new Point[4]{Point(0, 0), Point(a, 0), Point(a, a), Point(0, a) }, 4);
	}

	void GetInfo()
	{
		printf("Квадрат со стороной a = %f\n", points[1].x);
	}
};

class Oval : public Figure
{
protected:
	float r, R;

public:
	Oval() {}

	Oval(float r, float R)
	{
		this->r = r;
		this->R = R;
	}

	float GetSquare()
	{
		float s = pi * r * R;
		return s;
	}

	float GetPerimeter()
	{
		float p = 2.0 * pi * sqrt((r * r + R * r) / 2.0);
		return p;
	}

	void GetInfo()
	{
		printf("Овала с радиусами r = %f и R = %f\n", r, R);
	}
};

class Circle : public Oval
{
public:
	Circle(float r)
	{
		this->r = r;
		this->R = r;
	}

	Circle(Point o, Point a)
	{
		this->r = Point().Length(o, a);
	}

	void GetInfo()
	{
		printf("Окружность с радиусом r = %f\n", r);
	}
};




int main()
{
	setlocale(0, "");

	Figure *figures[5];

	figures[0] = new Figure(new Point[3]{Point(2, 4), Point(3, -8), Point(1, 2)}, 3);
	figures[1] = new Rectangle(2.0, 3.0);
	figures[2] = new Square(5.0);
	figures[3] = new Oval(7.0, 4.0);
	figures[4] = new Circle(5.0);


	for (int i = 0; i < 5; i++)
	{
		figures[i]->GetInfo();
		printf("Площадь: %f\nПериметр: %f\n\n\n", figures[i]->GetSquare(), figures[i]->GetPerimeter());
	}



	return 0;
}

