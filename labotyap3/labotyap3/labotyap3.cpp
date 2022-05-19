#include <fstream>
#include <ctype.h>
#include <vector>
using namespace std;

enum AState { S, A1, A2, A3, A4, A5, B2, B3, B4, B5, C1, C2, C3, C4, C5, D, D1, R, Q, P, E, F }; // состояния автомата для лексического анализатора
enum Elextype { lif, lth, lel, len, lid, lvl, lco, lao, leq, lsc, lno, llo, lwl }; // типы лексем
enum LState { s, a, a1, b, c, d, g, j, i, k, e }; // состояния автомата для синтаксического анализатора

struct lex { // структура лексемы
	char type[3]; // тип
	Elextype ltype;
	bool valid; // инициирована или нет
	char* word; // сама лексема
};
int check(char c) { // проверка символа
	if (isalpha(c)) return 0; // если буква
	if (isdigit(c)) return 1; // если цифра
	if (c == '<') return 2; // символ сравнения
	if (c == '>') return 3;
	if (c == '=') return 4; // равно
	if (c == '+' || c == '-' || c == '*' || c == '/') return 5; // арифметические операции
	if (c == ';') return 6;
	if (c == ' ' || c == '\0' || c == '\n' || c == '\r' || c == '\t') return 8; // разделяющие символы
	else return 7; // не принадлежащие алфавиту символы
}
int check2(lex lexema) {
	switch (lexema.ltype) {
	case Elextype::lif: return 0; // проверяем тип лексемы и возвращаем число
	case Elextype::lth: return 1;
	case Elextype::lel: return 2;
	case Elextype::len: return 3;
	case Elextype::lid: return 4;
	case Elextype::lvl: return 5;
	case Elextype::lco: return 6;
	case Elextype::lao: return 7;
	case Elextype::leq: return 8;
	case Elextype::lsc: return 9;
	case Elextype::lno: return 10;
	case Elextype::llo: return 11;
		return 12;
	}
}
int keywords(lex lexema) { // проверка на то, что лексема является ключевым словом
	if (lexema.word[0] == 'i' && lexema.word[1] == 'f' && lexema.word[2] == '\0') return 1;
	if (lexema.word[0] == 't' && lexema.word[1] == 'h' && lexema.word[2] == 'e' && lexema.word[3] == 'n' && lexema.word[4] == '\0') return 2;
	if (lexema.word[0] == 'e' && lexema.word[1] == 'l' && lexema.word[2] == 's' && lexema.word[3] == 'e' && lexema.word[4] == '\0') return 3;
	if (lexema.word[0] == 'e' && lexema.word[1] == 'n' && lexema.word[2] == 'd' && lexema.word[3] == '\0') return 4;
	if (lexema.word[0] == 'o' && lexema.word[1] == 'r' && lexema.word[2] == '\0') return 5;
	if (lexema.word[0] == 'a' && lexema.word[1] == 'n' && lexema.word[2] == 'd' && lexema.word[3] == '\0') return 5;
	if (lexema.word[0] == 'n' && lexema.word[1] == 'o' && lexema.word[2] == 't' && lexema.word[3] == '\0') return 6;
	else return 0;
}
void typeoflexem(lex& lexema, AState laststate) { // определяющая по состоянию автомата тип лексемы функция
	switch (laststate) {
	case AState::A1: // если лексема буквенная, то сверяем с списком ключевых слов
	case AState::A2:
	case AState::A3:
	case AState::A4:
	case AState::A5:
		switch (keywords(lexema)) {
		case 0:
			lexema.ltype = Elextype::lid;
			lexema.type[0] = 'i';
			lexema.type[1] = 'd';
			lexema.type[2] = '\0';
			break;
		case 1:
			lexema.ltype = Elextype::lif;
			lexema.type[0] = 'i';
			lexema.type[1] = 'f';
			lexema.type[2] = '\0';
			break;
		case 2:
			lexema.ltype = Elextype::lth;
			lexema.type[0] = 't';
			lexema.type[1] = 'h';
			lexema.type[2] = '\0';
			break;
		case 3:
			lexema.ltype = Elextype::lel;
			lexema.type[0] = 'e';
			lexema.type[1] = 'l';
			lexema.type[2] = '\0';
			break;
		case 4:
			lexema.ltype = Elextype::len;
			lexema.type[0] = 'e';
			lexema.type[1] = 'n';
			lexema.type[2] = '\0';
			break;
		case 5:
			lexema.ltype = Elextype::llo;
			lexema.type[0] = 'l';
			lexema.type[1] = 'o';
			lexema.type[2] = '\0';
			break;
		case 6:
			lexema.ltype = Elextype::lno;
			lexema.type[0] = 'n';
			lexema.type[1] = 'o';
			lexema.type[2] = '\0';
			break;
		}
		break;
	case AState::B2: // присваеваем типы остальным видам лексем
	case AState::B3:
	case AState::B4:
	case AState::B5:
		lexema.ltype = Elextype::lid;
		lexema.type[0] = 'i';
		lexema.type[1] = 'd';
		lexema.type[2] = '\0';
		break;
	case AState::C1:
	case AState::C2:
	case AState::C3:
	case AState::C4:
	case AState::C5:
		lexema.ltype = Elextype::lvl;
		lexema.type[0] = 'v';
		lexema.type[1] = 'l';
		lexema.type[2] = '\0';
		break;
	case AState::D:
	case AState::D1:
		lexema.ltype = Elextype::lco;
		lexema.type[0] = 'c';
		lexema.type[1] = 'o';
		lexema.type[2] = '\0';
		break;
	case AState::R:
		lexema.ltype = Elextype::leq;
		lexema.type[0] = 'e';
		lexema.type[1] = 'q';
		lexema.type[2] = '\0';
		break;
	case AState::Q:
		lexema.ltype = Elextype::lao;
		lexema.type[0] = 'a';
		lexema.type[1] = 'o';
		lexema.type[2] = '\0';
		break;
	case AState::P:
		lexema.ltype = Elextype::lsc;
		lexema.type[0] = 's';
		lexema.type[1] = 'c';
		lexema.type[2] = '\0';
		break;
	case AState::E:
		lexema.ltype = Elextype::lwl;
		lexema.type[0] = 'w';
		lexema.type[1] = 'l';
		lexema.type[2] = '\0';
		break;
	}
}
void createlexem(lex& lexema, AState state, char s[], int l, int firstPos) { // создаем саму лексему
	lexema.word = new char[l + 1]; // выделяем память
	for (int i = 0; i < l; ++i) {
		lexema.word[i] = s[firstPos + i]; // считываем лексему
	}
	lexema.word[l] = '\0';
	typeoflexem(lexema, state); // определяем тип лексемы
}
vector<lex>* analis(char s[], int n) {
	vector<lex>* result = new vector<lex>;
	lex lexema;
	lexema.valid = false;
	AState state = AState::S;
	AState laststate = AState::S; // переменная, хранящая предыдущее состояние автомата
	int pos = 0, firstPos = 0;
	AState states[21][9]{ // матрица переходов
		A1, C1, D, D1, R, Q, P, E, S,
		A2, B2, F, F, F, F, F, E, F,
		A3, B3, F, F, F, F, F, E, F,
		A4, B4, F, F, F, F, F, E, F,
		A5, B5, F, F, F, F, F, E, F,
		E, E, F, F, F, F, F, E, F,
		B3, B3, F, F, F, F, F, E, F,
		B4, B4, F, F, F, F, F, E, F,
		B5, B5, F, F, F, F, F, E, F,
		E, E, F, F, F, F, F, E, F,
		E, C2, F, F, F, F, F, E, F,
		E, C3, F, F, F, F, F, E, F,
		E, C4, F, F, F, F, F, E, F,
		E, C5, F, F, F, F, F, E, F,
		E, E, F, F, F, F, F, E, F,
		F, F, F, D1, F, F, F, F, F,
		F, F, F, F, F, F, F, F, F,
		F, F, F, F, F, F, F, F, F,
		F, F, F, F, F, F, F, F, F,
		F, F, F, F, F, F, F, F, F,
		E, E, F, F, F, F, F, E, F
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
		if (state == AState::F) { // если достигли разделяющего символа, то считываем лексему
			state = AState::S;
			lexema.valid = false;
			int l = pos - firstPos;
			createlexem(lexema, laststate, s, l, firstPos);
			result->push_back(lexema);
			laststate = AState::F;
			--pos;
		}
		++pos;
	}
	return result; // возвращаем указатель на вектор
}
void checkconstr(vector<lex>* result, ofstream& fout) { // проверяем верность конструкции
	LState state = LState::s; // текущее состояние
	LState laststate = LState::s; // прошлое состояние
	LState states[11][13]{ // матрица переходов
		a, e, e, e, e, e, e, e, e, e, e, e, e,
		e, e, e, e, b, b, e, e, e, e, a1, e, e,
		e, e, e, e, b, b, e, e, e, e, e, e, e,
		e, g, e, e, e, e, c, e, c, e, e, a, e,
		e, e, e, e, d, d, e, e, e, e, e, e, e,
		e, g, e, e, e, e, e, e, e, e, a, a, e,
		e, e, e, e, j, e, e, e, e, e, e, e, e,
		e, e, e, e, e, e, e, e, i, e, e, e, e,
		e, e, e, e, k, k, e, e, e, e, e, e, e,
		e, e, g, s, e, e, e, i, e, g, e, e, e,
		e, e, e, e, e, e, e, e, e, e, e, e, e,
	};
	int i = 0; // пока не дойдем до конца вектора лексем
	while (i < result->size()) {
		int a = check2((*result)[i]); // получаем тип лексемы
		laststate = state; // сохраняем состояние
		state = states[state][a]; // переходим по матрице
		if (state == LState::e) { // если нашли ошибку, то выходим
			break;
		}
		++i;
	}
	if (state == LState::s) fout << "OK"; // если достигли конца файла и вернулись в стартовое состояние, то конструкция верна
	else if (i != result->size()) { // если нет, то смотрим предыдущее состояние и выводим верные варианты лексем
		fout << i << ' ';
		switch (laststate) {
		case LState::s: fout << "if"; break;
		case LState::a: case LState::a1: fout << "id vl"; break;
		case LState::b: fout << "co eq lo th"; break;
		case LState::c: fout << "id vl"; break;
		case LState::d: fout << "lo no th"; break;
		case LState::g: fout << "id"; break;
		case LState::j: fout << "eq"; break;
		case LState::i: fout << "id vl"; break;
		case LState::k: fout << "ao el en sc"; break;
		}
	}
	else { // или если мы дошли до конца вектора, то смотрим на какой лексеме остановились
		fout << i << ' ';
		switch (state) {
		case LState::s: fout << "if"; break;
		case LState::a: case LState::a1: fout << "id vl"; break;
		case LState::b: fout << "co eq lo th"; break;
		case LState::c: fout << "id vl"; break;
		case LState::d: fout << "lo no th"; break;
		case LState::g: fout << "id"; break;
		case LState::j: fout << "eq"; break;
		case LState::i: fout << "id vl"; break;
		case LState::k: fout << "ao el en sc"; break;
		}
	}
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
		fout << endl;
		checkconstr(result, fout);
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