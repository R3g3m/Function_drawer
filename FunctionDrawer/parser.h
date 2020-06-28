#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <stack>
using namespace std;

//������ ����������
typedef map<string, double> Variables;
//������ "��������"
typedef map<string, vector<double> > Massives;
// ���� �������
extern enum tokentype {
	//����������, ���������, (, ), �������, ��������, ������, {, }
	var, num, op_br, cl_br, func, op, mas, op_sbr, cl_sbr
};
// ��������� ������
struct token {
	string name;
	tokentype type;

	//������������
	token(string str, tokentype typ) {
		name = str;
		type = typ;
	}
	token() {}
};


//������ �������
typedef vector<token> tokens;
//��������� ������������
//extern set<char> DelimSet;
//���������� ��������
//extern map <string, int> prior;
//�����������
const string delimiters = " ()+/*-^&|!%[]";

//�������������� ��������� ������������
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
//��������, �������� �� ������ ������������
bool IsDelimiter(char sym);
//��������� ��������� �� ������
void CreateTokensFromExpression(string &expr, tokens &texpr);

//������� ���������� ���������� ��������
void CreatePrior();

void CreatePostfixFromTokens(tokens &texpr, tokens &pexpr);

