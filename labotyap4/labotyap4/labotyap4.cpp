#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

enum AState {S, A, B, C, D, D1, R, Q, E, F};
enum lexState {};

struct lex {
	bool valid;
	lexState type;
	char* word;
};
int check(char c) { // проверка символа
	if (isalpha(c)) return 0; // если буква
	if (isdigit(c)) return 1; // если цифра
	if (c == '>' || c == '<') return 2; // символ сравнения
	if (c == '=') return 3; // равно
	if (c == '+' || c == '-') return 4; // арифметические операции
	if (c == ' ' || c == '\0' || c == '\n' || c == '\r' || c == '\t') return 5; // разделяющие символы
	else return 6; // не принадлежащие алфавиту символы
}
vector<lex>* analis(char s[], int n) {
	vector<lex>* result = new vector<lex>;
	lex lexema;
	lexema.valid = false;
	AState state = AState::S;
	AState laststate = AState::S; // переменная, хранящая предыдущее состояние автомата
	int pos = 0, firstPos = 0;
	AState states[9][7]{ // матрица переходов
		A, C, D, R, Q, S, E,
		A, B, D, R, Q, F, E,
		B, B, D, R, Q, F, E,
		E, C, D, R, Q, F, E,
		A, C, D1, D1, E, F, E,
		A, C, D, R, Q, F, E,
		A, C, D, R, Q, F, E,
		A, C, D, R, Q, F, E,
		E, E, E, E, E, E, E
	};
	while (pos <= n) { // пока не дойдем до конца файла
		int b = 0;
		if (state == AState::S && s[pos] != ' ' && s[pos] != '\r' && s[pos] != '\n' && s[pos] != '\0' && s[pos] != '\t') { // инициализируем лексему
			firstPos = pos;
			lexema.valid = true;
		}
		if (lexema.valid) {
			b = check(s[pos]); // обрабатываем символ
			if (state != AState::S) {
				laststate = state; // сохраняем предыдущее состояние автомата
			}
			state = states[state][b]; // переходим по матрице
		}
		if (state == AState::R || state == AState::Q) { // если нашли символы =, +, -, <, >, то обрабатываем особым образом, сохраняя лексему перед ними
			if (laststate == AState::A || laststate == AState::B || laststate == AState::C) {
				int l = pos - firstPos;
				//createlexem(lexema, laststate, s, l, firstPos);
				result->push_back(lexema);
			}
			if (laststate != AState::F) {
				//createlexem(lexema, state, s, 1, pos);
				result->push_back(lexema);
				firstPos = pos + 1; // указываем начало для новой лексемы
			}
			laststate = AState::F;
		}
		if (state == AState::D) {
			if (laststate == AState::A || laststate == AState::B || laststate == AState::C) {
				int l = pos - firstPos;
				//createlexem(lexema, laststate, s, l, firstPos);
				result->push_back(lexema);
			}
			b = check(s[pos + 1]); // проверяем следующий символ
			AState newstate = states[state][b];
			if (newstate == AState::D1) { // если это >=, <=, то сохраняем всю эту лексему
				//createlexem(lexema, newstate, s, 2, pos);
				result->push_back(lexema);
				firstPos = pos + 2;
			}
			else {
				//createlexem(lexema, state, s, 1, pos);
				result->push_back(lexema);
				firstPos = pos + 1;
			}
			laststate = AState::F;
		}
		if (state == AState::F) { // если достигли разделяющего символа, то считываем лексему
			state = AState::S;
			lexema.valid = false;
			int l = pos - firstPos;
			//createlexem(lexema, laststate, s, l, firstPos);
			result->push_back(lexema);
			laststate = AState::F;
		}
		if (state == AState::E) { // если нашли неверную лексему, то пропускаем все символы до следующего разделяющего
			while (s[pos] != ' ' && s[pos] != '\r' && s[pos] != '\n' && s[pos] != '\t' && s[pos] != '\0' && s[pos] != '+' && s[pos] != '-' && s[pos] != '=' && s[pos] != '>' && s[pos] != '<') {
				++pos;
			}
			int l = pos - firstPos;
			//createlexem(lexema, AState::E, s, l, firstPos);
			result->push_back(lexema);
			state = AState::S;
			laststate = AState::E;
			--pos; // возвращаемся на последний символ неверной лексемы, чтобы случайно не потерять следующую лексему
		}
		++pos;
	}
	return result; // возвращаем указатель на вектор
}

int main() {
	ifstream fin("input.txt", ios::binary);
	ofstream fout("output.txt");
	if (fin.is_open()) {
		int n = 0;
		fin.seekg(0, ios::end);
		n = fin.tellg(); // находим длину файла
		fin.seekg(0, ios::beg);
		char* text = new char[n + 1];
		fin.getline(text, n + 1, '\0'); // считываем файл
		vector<lex>* result = analis(text, n);
		for (int i = 0; i < result->size(); ++i) {
			fout << (*result)[i].word << "[" << (*result)[i].type << "] "; // выводим лексемы
		}
		for (int i = 0; i < result->size(); ++i) {
			delete[](*result)[i].word; // удаляем лексемы
		}
		delete result; // удаляем вектор
		delete[] text; // удаляем строку
		fin.close(); // закрываем файлы
		fout.close();
	}
	return 0;
}
