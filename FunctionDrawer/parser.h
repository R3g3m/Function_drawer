#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <stack>
using namespace std;

//Массив переменных
typedef map<string, double> Variables;
//Массив "массивов"
typedef map<string, vector<double> > Massives;
// Типы токенов
extern enum tokentype {
	//Переменная, константа, (, ), функция, операция, массив, {, }
	var, num, op_br, cl_br, func, op, mas, op_sbr, cl_sbr
};
// Структура токена
struct token {
	string name;
	tokentype type;

	//Конструкторы
	token(string str, tokentype typ) {
		name = str;
		type = typ;
	}
	token() {}
};


//Массив токенов
typedef vector<token> tokens;
//Множество разделителей
//extern set<char> DelimSet;
//Приоритеты операций
//extern map <string, int> prior;
//Разделители
const string delimiters = " ()+/*-^&|!%[]";

//Инициализирует множество разделителей
void CreateSetOfDelimiters();

double fact(double n);
double op_plus(stack <double> &s);
double op_minus(stack <double> &s);
double op_mul(stack <double> &s);
double op_div(stack <double> &s);
double op_deg(stack <double> &s);
double op_opposite(stack <double> &s);
double op_factorial(stack <double> &s);
double op_odiv(stack <double> &s);
double op_and(stack <double> &s);
double op_or(stack <double> &s);
double op_not(stack <double> &s);
double op_sin(stack <double> &s);
double op_cos(stack <double> &s);
double op_tan(stack <double> &s);
double op_asin(stack <double> &s);
double op_acos(stack <double> &s);
double op_atan(stack <double> &s);


void CreateSetOfDelimiters();
//Проверка, является ли символ разделителем
bool IsDelimiter(char sym);
//Разбиваем выражение на токены
void CreateTokensFromExpression(string &expr, tokens &texpr);

//Функция выставляет приоритеты операций
void CreatePrior();

void CreatePostfixFromTokens(tokens &texpr, tokens &pexpr);

