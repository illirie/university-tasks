#include <fstream>
#include <iomanip>
#include "math.h"
using namespace std;

double summakubov(double a[], int n, double b) {
    double s = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] >= b && a[i] > 0) {
            s += pow(a[i], 3);
        }
    }
    return s;
}

int main()
{
    double k, b = 0;
    int n = 1, i = 0;
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    if (fin.is_open()) {
        fin >> b;
        double* a = new double[n];
        while (fin >> k) { // считываем, пока в файле есть данные.
            if (i < n) { // если их количество больше, чем можно вписать в массив, 
                a[i] = k;
                i++;
            }
            else {
                double* c = new double[n]; // то мы копируем массив.
                for (int j = 0; j < n; j++) {
                    c[j] = a[j];
                };
                delete[] a; // удаляем начальный массив.
                a = new double[n + 1]; // пересоздаем его снова.
                for (int j = 0; j < n; j++) { // и копируем в него все данные.
                    a[j] = c[j];
                }
                a[n] = k; // добавляем последний элемент.
                n++;
                i++;
                delete[] c; // удаляем копию.
            }
        }
        fout << setprecision(15) << summakubov(a, n, b);
        delete[] a;
    }
    fin.close();
    fout.close();
}