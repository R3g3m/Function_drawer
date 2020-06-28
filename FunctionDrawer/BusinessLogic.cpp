#include "stdafx.h"
//#include <windows.h>
#include <math.h>
#include "BusinessLogic.h"
//int NUM = 70;

	// ������� �������� ������ ��� �������
	// (��������� ������, ����� ��������� ��������� ����� ������)

//double(*fcnPtr)(double a);
// TODO: �������� � �������� ��������� ������ getData ������ �� �������, ������� ��� ���� � ������� ������������ ������

//double(*ptFunc)(double ) = &sinusFun;

double ** getData(int n, double (*ptFunction)(double ))
{
	double **f;
	f = new double*[2];
	f[0] = new double[n];
	f[1] = new double[n];
	for (int i = 0; i<n; i++)
	{
		double x = (double)i * 0.1;
		f[0][i] = x;
		f[1][i] = ptFunction(x);
	}
	return f;
}
// ������� ��������� �������
void DrawGraph(HDC &hdc, RECT rectClient,
	double **x, // ������ ������
	int n // ���������� �����
	) 
{
	PAINTSTRUCT pstr;
	double OffsetY, OffsetX;
	double MAX_X, MAX_Y;
	double ScaleX, ScaleY;
	double min, max;
	int height, width;
	int X, Y; // ���������� � ���� (� px)
	HPEN hpen;
	height = rectClient.bottom - rectClient.top;
	width = rectClient.right - rectClient.left;

	// ������� ���������� �������� X
	min = x[0][0];
	max = x[0][0];
	for (int i = 0; i<n; i++)
	{
		if (x[0][i] < min) min = x[0][i];
		if (x[0][i] > max) max = x[0][i];
	}
	double temp = max - min;
	MAX_X = max - min;
	OffsetX = min*width / MAX_X; // �������� X
	ScaleX = (double)width / MAX_X; // ���������� ����������� X

	// ������� ���������� �������� Y
	min = x[1][0];
	max = x[1][0];
	for (int i = 0; i<n; i++)
	{
		if (x[1][i] < min) min = x[1][i];
		if (x[1][i] > max) max = x[1][i];
	}
	MAX_Y = max - min;
	OffsetY = max*height / (MAX_Y); // �������� Y
	ScaleY = (double)height / MAX_Y; // ���������� ����������� Y
	
	// ������ ��������� ������������ ���
	//HDC hdc = BeginPaint(hWndC, &pstr);
	hpen = CreatePen(PS_SOLID, 0, 0); // ������ ���� 1px
	SelectObject(hdc, hpen);
	MoveToEx(hdc, 0, OffsetY, 0); // ����������� � ����� (0;OffsetY)
	LineTo(hdc, width, OffsetY); // ��������� �������������� ���
	MoveToEx(hdc, OffsetX, 0, 0); // ����������� � ����� (OffsetX;0)
	LineTo(hdc, OffsetX, height); // ��������� ������������ ��� 
	DeleteObject(hpen); // �������� ������� ����

	// ������ ������ �������
	int color = 0xFF; // ������� ���� 
	hpen = CreatePen(PS_SOLID, 2, color); 
	SelectObject(hdc, hpen);
	// ������� ��������� ����� �������
	X = (int)(OffsetX + x[0][0] * ScaleX); 
	Y = (int)(OffsetY - x[1][0] * ScaleY);
	MoveToEx(hdc, X, Y, 0); 
	//�������� ���� ��������� �������
	for (int i = 0; i<n; i++)
	{
		X = OffsetX + x[0][i] * ScaleX;
		Y = OffsetY - x[1][i] * ScaleY;
		LineTo(hdc, X, Y);
	}
	DeleteObject(hpen); 


	//EndPaint(hWndC, &pstr);
}

double sinusFun(double x) {
	return sin(x);
}

double cosinusFun(double x) {
	return cos(x);
}

double parabolaFun(double x) {
	return x*x;
}

double giperbolaFun(double x) {
	return x*x*x;
}

double tangensFun(double x) {
	return tan(x);
}

double expFun(double x) {
	return exp(x);
}

