#include "stdafx.h"
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <stack>
#include "Parser.h"
using namespace std;

//Множество разделителей
set<char> DelimSet;
//Приоритеты операций
map <string, int> prior;

//Указатель на функцию(для операций)
typedef double(*func_type)(stack<double> &);

//Массив операций
typedef map<string, func_type> Ops;
Ops ops;
//Функция считывает выражение в строку "expr" и ищем переменные или массивы
void ReadExpressionFromStream(ifstream &inp, string &expr, Variables &var, Massives &mas) {
	std::getline(inp, expr);
	string temp;
	int pos;

	while (!inp.eof()) {
		getline(inp, temp);
		//Если встретили '=', то это переменная, заносим ее имя и значение в массив
		pos = temp.find('=');
		if (pos>0) {
			string name = temp.substr(0, pos);
			double value = atof(temp.substr(pos + 1).c_str());
			var[name] = value;
		}
		//Если нашли '{', это массив, заносим имя массива и значения в соответствующие массивы
		pos = temp.find('{');
		if (pos>0) {
			string name = temp.substr(0, pos);
			//Ищем значения массива и запоминаем их
			int pos1 = pos, pos2;
			do {
				pos2 = temp.find(',');
				double value = atof(temp.substr(pos1 + 1, pos2).c_str());
				mas[name].push_back(value);
				if (pos2 == -1)
					break;
				temp[pos2] = ' ';
				pos1 = pos2;
			} while (pos2 > 0);
		}
	}
	return;
}


//////////////////////////////////////////////////////

//Инициализирует множество разделителей
void CreateSetOfDelimiters() {
	for (int i = 0; i < delimiters.size(); i++)
		DelimSet.insert(delimiters[i]);
	return;
}

//Проверка, является ли символ разделителем
bool IsDelimiter(char sym) {
	return DelimSet.count(sym) > 0;
}

//Разбиваем выражение на токены
void CreateTokensFromExpression(string &expr, tokens &texpr) {
	CreateSetOfDelimiters();
	string ex = expr + " ";
	string name;

	//Получаем имя токена
	int i = 0;
	while (i < ex.size() - 1) {
		name = "";
		//Если текущий символ разделитель
		if (IsDelimiter(ex[i])) {
			if (ex[i] == ' ') { //Пробел просто перепрыгиваем
				i++;
				continue;
			}
			name = ex[i]; //Любой другой добавляем в имя токена
			i++;
		}

		else {
			while (!IsDelimiter(ex[i])) {
				/*Если не разделитель непример, переменная или имя массива,
				Считываем его польностью */
				name += ex[i];
				i++;
			}
		}
		//Заносим получившийся токен в список токенов
		texpr.push_back(token(name, var));
	}

	//Раздаем получившимся токенам типы
	for (int j = 0; j < texpr.size(); j++) {
		if (texpr[j].name[0] == '[') {
			texpr[j].type = op_sbr;
			continue;
		}
		if (texpr[j].name[0] == ']') {
			texpr[j].type = cl_sbr;
			continue;
		}
		if (texpr[j].name[0] == '(') {
			texpr[j].type = op_br;
			continue;
		}
		if (texpr[j].name[0] == ')') {
			texpr[j].type = cl_br;
			continue;
		}
		if (isdigit(texpr[j].name[0])) {
			texpr[j].type = num;
			continue;
		}

		//mas
		if (isalpha(texpr[j].name[0])) {
			if (j < texpr.size() - 1 && texpr[j + 1].name[0] == '[')
				texpr[j].type = mas;
			//continue;
		}

		if (isalpha(texpr[j].name[0])) {
			if (j < texpr.size() - 1 && texpr[j + 1].name[0] == '(')
				texpr[j].type = func;
			continue;
		}

		texpr[j].type = op;
	}

	//Проверяем минус и !, что это префиксные операции
	for (int j = 0; j < texpr.size(); j++) {
		if (texpr[j].name == "-" && (j == 0 || texpr[j - 1].type == op_br))
			texpr[j].name = "opposite";
		if (texpr[j].name == "!" && (j == texpr.size() - 1 || texpr[j + 1].type == cl_br || texpr[j + 1].type == op))
			texpr[j].name = "factorial";
	}

	return;
}

//////////////////////////Преобразование в постфиксную запись//////////////////////////////////

//Функция выставляет приоритеты операций
void CreatePrior() {
	prior["+"] = 10;
	prior["-"] = 10;
	prior["*"] = 20;
	prior["/"] = 20;
	prior["^"] = 30;
	prior["opposite"] = 10;
	prior["factorial"] = 30;
	prior["%"] = 20;
	prior["&"] = 5;
	prior["|"] = 5;
	prior["!"] = 40;
}

//Переводим выражение в постфиксную запись
void CreatePostfixFromTokens(tokens &texpr, tokens &pexpr) {
	//Задаем приоритеты операций
	CreatePrior();
	stack <token> TStack;

	//Ловим токены и работаем по алгоритму
	for (int i = 0; i < texpr.size(); i++) {
		switch (texpr[i].type) {
		case var:
		case num:
			pexpr.push_back(texpr[i]);
			break;

		case op_br:
			TStack.push(texpr[i]);
			break;

		case cl_br:
			while (TStack.top().type != op_br) {
				pexpr.push_back(TStack.top());
				TStack.pop();
			}
			TStack.pop();
			break;

		case op_sbr:
			TStack.push(texpr[i]);
			break;

		case cl_sbr:
			while (TStack.top().type != op_sbr) {
				pexpr.push_back(TStack.top());
				TStack.pop();
			}
			TStack.pop();
			break;

		case op:
			if (TStack.size()) {
				while (TStack.size() && ((TStack.top().type == op && prior[texpr[i].name] <= prior[TStack.top().name]) ||
					TStack.top().type == func ||
					TStack.top().type == mas)) {
					pexpr.push_back(TStack.top());
					TStack.pop();
				}
			}
			TStack.push(texpr[i]);
			break;

		case mas:
			while (TStack.size() && TStack.top().type == mas) {
				pexpr.push_back(TStack.top());
				TStack.pop();
			}
			TStack.push(texpr[i]);
			break;

		case func:
			while (TStack.size() && TStack.top().type == func) {
				pexpr.push_back(TStack.top());
				TStack.pop();
			}
			TStack.push(texpr[i]);
			break;
		}
	}

	while (TStack.size()) {
		pexpr.push_back(TStack.top());
		TStack.pop();
	}

	return;
}

//Инициализация массива операций
void CreateOps() {
	ops["+"] = op_plus;
	ops["-"] = op_minus;
	ops["*"] = op_mul;
	ops["/"] = op_div;
	ops["^"] = op_deg;
	ops["opposite"] = op_opposite;
	ops["factorial"] = op_factorial;
	ops["%"] = op_odiv;
	ops["&"] = op_and;
	ops["|"] = op_or;
	ops["!"] = op_not;
	ops["sin"] = op_sin;
	ops["cos"] = op_cos;
	ops["tan"] = op_tan;
	ops["acos"] = op_acos;
	ops["asin"] = op_asin;
	ops["atan"] = op_atan;

	return;
}

//Считаем результат выражения
double ResultExpr(tokens &pexpr, Variables &expvars, Massives &varmas) {
	CreateOps();
	stack <double> s;

	for (int i = 0; i<pexpr.size(); i++) {
		switch (pexpr[i].type) {
		case num: {
			s.push(atoi(pexpr[i].name.c_str()));
		}
				  break;

		case var: {
			Variables::iterator Vit;
			for (Vit = expvars.begin(); Vit != expvars.end(); Vit++) {
				if (Vit->first == pexpr[i].name) {
					s.push(Vit->second);
					break;
				}
			}
		}
				  break;

		case func:
		case op: {
			Ops::iterator Oit;
			for (Oit = ops.begin(); Oit != ops.end(); Oit++) {
				if (Oit->first == pexpr[i].name) {
					s.push(Oit->second(s));
					break;
				}
			}
		}
				 break;

		case mas: {
			int index = s.top();
			s.pop();

			Massives::iterator it;
			for (it = varmas.begin(); it != varmas.end(); it++) {
				if (it->first == pexpr[i].name)
					s.push(it->second[index]);
			}
		}
		}
	}

	return s.top();
}

//Математические операции

double fact(double n) {
	if (n == 0)
		return 1;
	return n*fact(n - 1);
}
double op_plus(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a + b;
}
double op_minus(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b - a;
}
double op_mul(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a*b;
}
double op_div(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b / a;
}
double op_deg(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	//b^a!!
	return pow(b, a);
}
double op_opposite(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return -a;
}
double op_factorial(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return fact(a);
}
double op_odiv(stack <double> &s) {
	long long a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b%a;
}
double op_and(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a&&b;
}
double op_or(stack <double> &s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a || b;
}
double op_not(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return !a;
}
double op_sin(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return sin(a);
}
double op_cos(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return cos(a);
}
double op_tan(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return tan(a);
}
double op_asin(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return asin(a);
}
double op_acos(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return acos(a);
}
double op_atan(stack <double> &s) {
	double a;
	a = s.top();
	s.pop();
	return atan(a);
}