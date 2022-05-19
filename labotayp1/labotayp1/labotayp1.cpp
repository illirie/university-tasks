#include <fstream>
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
using namespace std;

enum AState { S, A, B, C, E, F };

struct lex {
    bool valid;
    char* str;
};

int check(char text[], int firstPos, int lastPos, int i) {
    if ((text[firstPos + i] >= 'А' && text[firstPos + i] <= 'я') || text[firstPos + i] == 'ё' || text[firstPos + i] == 'Ё') {
    }
    else {
        return 2;
    }
    if (text[firstPos + i] == text[lastPos - i] || text[firstPos + i] + 'a' - 'A' == text[lastPos - i]) {
        return 1;
    }
    else {
        return 2;
    }
};

vector<lex*> findingpolindroms(char text[], int size) {
    vector<lex*> palindroms;
    int position = 0; // текущая позиция в строке
    AState state = AState::S; // текущее состояние
    lex* lexema = new lex;
    enum AState states[5][4] = {
    states[S][0] = S,
    states[S][1] = A,
    states[S][2] = E,
    states[S][3] = E,
    states[A][0] = S,
    states[A][1] = B,
    states[A][2] = E,
    states[A][3] = F,
    states[B][0] = S,
    states[B][1] = C,
    states[B][2] = E,
    states[B][3] = F,
    states[C][0] = S,
    states[C][1] = E,
    states[C][2] = E,
    states[C][3] = F,
    states[E][0] = S,
    states[E][1] = E,
    states[E][2] = E,
    states[E][3] = E
    };
    while (position < size)
    {
        int b = 0;
        lexema->valid = false;
        int firstPos, l = 0, lastPos;
        if (state == AState::S && text[position] != ' ' && text[position] != '\t' && text[position] != '\n' && text[position] != '\0' && text[position] != '\r') // инициализируем лексему
        {
            firstPos = position;
            lexema->valid = true;
            while (text[position] != ' ' && text[position] != '\t' && text[position] != '\n' && text[position] != '\0' && text[position] != '\r') { // считываем слово до разделяющего символа
                ++position;
            }
            lastPos = position - 1, l = position - firstPos; // запоминаем позицию последнего символа в слове и его длину
        }
        if (lexema->valid) {
            for (int i = 0; i <= (l - 1) / 2; ++i) { // проверяем на палиндром
                b = check(text, firstPos, lastPos, i);
                state = states[state][b]; // переходим по матрице состояний
            }
        }
        state = states[state][3];
        if (state == AState::F)
        {
            lexema->str = new char[l + 1];
            for (int i = 0; i < l; ++i) {
                lexema->str[i] = text[firstPos + i]; // считываем лексему
            }
            lexema->str[l] = '\0';
            palindroms.push_back(lexema);
            lexema = new lex;
        }
        state = AState::S;
        ++position;
    }
    return palindroms; // возвращаем список палиндромов
};

int main()
{
    ifstream fin("input.txt", ios::binary);
    ofstream fout("output.txt");
    if (fin.is_open()) {
        int n = 0;
        fin.seekg(0, ios::end);
        n = fin.tellg(); // находим длину файла
        fin.seekg(0, ios::beg);
        char* text = new char[n + 1];
        fin.getline(text, n + 1, '\0'); // считываем файл
        vector<lex*> palindroms = findingpolindroms(text, n + 1);
        for (int i = 0; i < palindroms.size(); ++i) {
            fout << palindroms[i]->str << ' '; // выводим на экран и чистим память.
            delete palindroms[i]->str;
            delete palindroms[i];
        }
        delete[] text; // удаляем массив символов
        fin.close();
        fout.close();
    };
    return 0;
};