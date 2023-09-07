#include <fstream>
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
	friend ofstream& operator << (ofstream& out, rectangle& c) {
		for (int i = 0; i < 4; ++i) {
			out << c.x[i] << ' '  << c.y[i] << ' ';
		}
		return out;
	}
};

class landplot : public rectangle {  // создаем потомка
private:
	double sq; // дополнительное поле - площадь
public:
	landplot() : rectangle() { // конструктор по умолчанию
		sq = square();
	}
	landplot(rectangle c) {
		this->copy(c);
		sq = c.square();
	}
	const bool operator >(landplot c) {
		return sq > c.sq;
	}
	const bool operator ==(landplot c) {
		return sq == c.sq;
	}
	const bool operator <(landplot c) {
		return sq < c.sq;
	}
	~landplot() {};
};

int main() {
	ifstream fin("input.txt"); // открываем файлы ввода и вывода
	ofstream fout("output.txt");
	if (fin.is_open()) {
		rectangle a, b; // создаем объекты классов
		landplot c, d;
		int k; // считываем режим работы
		fin >> k;
		fin >> a >> b; // считываем прямоугольники
		switch (k) { 
		case 0: // изменить центр первого, вывести новые координаты, вывести сумму объектов
			a.changecenter(0, 0);
			fout << a << endl;
			fout << a + b;
			break;
		case 1: // вывести на экран периметры и сравнение участков
			c = a;
			d = b;
			fout << c.perimetr() << ' ' << d.perimetr() << endl;
			if (c > d) {
				fout << "a > b";
			}
			if (c < d) {
				fout << "a < b";
			}
			if (c == d) {
				fout << "a = b";
			}
			break;
		case 2: // изменить сторону первого прямоугольника и вывести новые координаты и сложение двух
			a.changelength(1, 5);
			fout << a << endl;
			fout << a + b;
			break;
		case 3: // вывести на экран площади и сравнение участков
			c = a;
			d = b;
			fout << c.square() << ' ' << d.square() << endl;
			if (c > d) {
				fout << "a > b";
			}
			if (c < d) {
				fout << "a < b";
			}
			if (c == d) {
				fout << "a = b";
			}
			break;
		}
		fin.close(); // закрываем файлы
	}
	fout.close();
	return 0;
}