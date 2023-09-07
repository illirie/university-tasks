#include <iostream>
#include <fstream>
#include <cstdlib>
#include "time.h"
#include "math.h"
using namespace std;

class rectangle { // создаем класс
private:
	double x[4], y[4]; // храним координаты вершин
public:
	rectangle() { // конструктор по умолчанию
		for (int i = 0; i < 4; ++i) {
			x[i] = 0;
			y[i] = 0;
		}
	};
	rectangle(double c[8]) { // конструктор инициализации
		for (int i = 0; i < 4; ++i) {
			x[i] = c[i];
			y[i] = c[i + 1];
		}
	}
	rectangle copy(rectangle c) { // конструктор копирования
		for (int i = 0; i < 4; ++i) {
			x[i] = c.x[i];
			y[i] = c.y[i];
		}
		return *this;
	};
	rectangle createrect() {
		x[0] = rand() % 100 + 0;
		y[0] = rand() % 100 + 0;
		x[2] = rand() % 100 + 0;
		y[2] = rand() % 100 + 0;
		x[1] = x[2];
		y[1] = y[0];
		x[3] = x[0];
		y[3] = y[2];
		return *this;
	}
	~rectangle() {}; // деструктор
	void changecenter(double x1, double y1) { // меняем центр прямоугольника
		double x0 = (x[0] + x[2]) / 2; // находим начальный центр
		double y0 = (y[0] + y[2]) / 2;
		for (int i = 0; i < 4; ++i) { // находим сдвиг (разницу координат старого и нового центром) и переносим все вершины
			x[i] = x[i] + (x1 - x0);
			y[i] = y[i] + (y1 - y0);
		}
	}
	double changelength(int a, double b) { // меняем длину стороны
		switch (a) {
		case 1:
			x[1] = x[0] + b;
			x[2] = x[3] + b;
			break;
		case 2:
			y[0] = y[3] + b;
			y[1] = y[2] + b;
			break;
		}
		return b;
	}
	double length(int i) { // вычисляем длину стороны
		return sqrt(pow((x[i] - x[i - 1]), 2) + pow((y[i] - y[i - 1]), 2));
	}
	double perimetr() { // вычисляем периметр
		return 2 * (length(1) + length(2));
	}
	double square() { // вычисляем площадь
		return length(1) * length(2);
	}
	const double operator +(rectangle c) { // перегрузки операторов:
		double b = square() + c.square();
		return b;
	}
	const rectangle& operator =(rectangle c) {
		for (int i = 0; i < 4; ++i) {
			x[i] = c.x[i];
			y[i] = c.y[i];
		}
		return *this;
	};
	const bool operator >(rectangle c) {
		return square() > c.square();
	}
	const bool operator ==(rectangle c) {
		return square() == c.square();
	}
	const bool operator !=(rectangle c) {
		return square() != c.square();
	}
	const bool operator <(rectangle c) {
		return square() < c.square();
	}
	const bool operator >=(rectangle c) {
		return square() >= c.square();
	}
	const bool operator <=(rectangle c) {
		return square() <= c.square();
	}
	friend istream& operator >> (istream& in, rectangle& c) {
		for (int i = 0; i < 4; ++i) {
			in >> c.x[i] >> c.y[i];
		}
		return in;
	}
	friend ostream& operator << (ostream& out, rectangle& c) {
		for (int i = 0; i < 4; ++i) {
			out << c.x[i] << ' ' << c.y[i] << ' ';
		}
		return out;
	}
};
struct list { // список
    rectangle value; // значение
    list* prev, * next;  // указатели на следующий и предыдущие элементы
};
list* init(rectangle a)  // создаем первый элемент списка
{
    list* lst = new list;
    lst->value = a;
    lst->next = NULL; // указатель на следующий узел
    lst->prev = NULL; // указатель на предыдущий узел
    return lst;
}
list* pushback(list* last, rectangle k) // добавляем элемент в конец списка.
{
    list* p = new list;
    p->value = k;
    p->prev = last;
    p->next = NULL;
    last->next = p;
    return p;
}
void erase(list*& head) { // удаление списка
    while (head) {
        list* s = head;
        head = head->next;
        delete s;
    }
}
list* swap(list* lst1, list* lst2, list* head)
{
    list* prev1, * prev2, * next1, * next2;
    prev1 = lst1->prev;  // узел предшествующий lst1
    prev2 = lst2->prev;  // узел предшествующий lst2
    next1 = lst1->next; // узел следующий за lst1
    next2 = lst2->next; // узел следующий за lst2
    if (lst2 == next1)  // обмениваются соседние узлы
    {
        lst2->next = lst1;
        lst2->prev = prev1;
        lst1->next = next2;
        lst1->prev = lst2;
        if (next2 != NULL)
            next2->prev = lst1;
        if (lst1 != head)
            prev1->next = lst2;
    }
    else if (lst1 == next2)  // обмениваются соседние узлы
    {
        lst1->next = lst2;
        lst1->prev = prev2;
        lst2->next = next1;
        lst2->prev = lst1;
        if (next1 != NULL)
            next1->prev = lst2;
        if (lst2 != head)
            prev2->next = lst1;
    }
    else  // обмениваются отстоящие узлы
    {
        if (lst1 != head)  // указатель prev можно установить только для элемента,
            prev1->next = lst2; // не являющегося корневым
        lst2->next = next1;
        if (lst2 != head)
            prev2->next = lst1;
        lst1->next = next2;
        lst2->prev = prev1;
        if (next2 != NULL) // указатель next можно установить только для элемента,
            next2->prev = lst1; // не являющегося последним
        lst1->prev = prev2;
        if (next1 != NULL)
            next1->prev = lst2;
    }
    return lst2;
}
bool checksort(list* head) {
	while (head) {
		rectangle c = head->value;
		if (c > head->value) {
			return false;
		}
		head = head->next;
	}
	return true;
}

void vuborsort(list*& head) { // сортировка выбором
	list* p = head;
    while (p->next) {  // проходим по всему списку от головы
		list* min = p;
        list* last = p->next;
        while (last) { // сравниваем элементы и выбираем наименьший
            if (min->value > last->value) {
				min = last;
            }
            last = last->next;
        }
		p = swap(p, min, head); // меняем ячейки местами
		if (!p->prev) { // проверяем, не съехала ни случайно голова списка
			head = p;
		}
        p = p->next;
    }
}

int main() {
	srand(static_cast<unsigned int>(time(0))); // задаем "зерно" для генерации случайной последовательности.
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	if (fin.is_open()) {
		int a;
		rectangle b;
		fin >> a;
		int c = a;
		list* head = NULL;
		list* last = NULL;
		if (a) {
			head = init(b.createrect());
		    last = head;
			--a;
		}
		while (a--) {
			last = pushback(last, b.createrect());
		}
		vuborsort(head);
		bool check = checksort(head);
		if (check) {
			cout << "correct sort";
			fout << "correct sort" << endl;
		}
		else {
			cout << "incorrect sort";
			fout << "incorrect sort" << endl;
		}
		fout << c;
		erase(head);
		fin.close();
	}
	fout.close();
	return 0;
}