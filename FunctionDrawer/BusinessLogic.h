#pragma once
#include "stdafx.h"
#include <math.h>


void DrawGraph(HDC &hdc, RECT rectClient,
	double **x, // массив данных
	int n // количество точек
	);

double ** getData(int n, double(*ptFunction)(double));

double sinusFun(double x);

double cosinusFun(double x);
double parabolaFun(double x);
double giperbolaFun(double x);
double tangensFun(double x);
double expFun(double x);