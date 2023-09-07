#include <iostream>

using namespace std;

#define PHI ((1.0 + sqrt(5.0)) * 0.5)
typedef const double& cdref;

double f(double x) {
    return (x - 3) * x;
}

double bisect(double (*p) (double), double x0, double x1, double EPS, int max_steps) {
    if (x0 > x1) {
        swap(x0, x1);
    }
    double xc = 0;
    for (int i = 0; i < max_steps; i++) {
        xc = (x0 + x1) * 0.5;
        if (f(xc - EPS) > f(xc + EPS)) {
            x0 = xc;
        }
        else {
            x1 = xc;
        }
        if (abs(x0 - x1) < EPS) {
            cout << "bisect iter number: " << i << endl;
            break;
        }
    }
    return xc;
}

double golden_ratio(double (*p) (double), double x0, double x1, double EPS, int max_steps) {
    if (x0 > x1) {
        swap(x0, x1);
    }
    double xl = x0, xr = x1, dx;
    for (int i = 0; i < max_steps; i++) {
        if (xr - xl < EPS) {
            cout << "golden_ratio iter number: " << i << endl;
            break;
        }
        dx = x1 - x0;
        xl = x1 - dx / PHI;
        xr = x0 + dx / PHI;
        if (f(xl) >= f(xr)) {
            x0 = xl;
        }
        else {
            x1 = xr;
        }
    }
    return (xl + xr) * 0.5;
}

void get_closest_pair(double x, int& f_n, int& f_n1) {
    if (x < 1) return;
    f_n1 = 1;
    if (x < 2) return;
    int temp = 0;
    while (f_n < x) {
        temp = f_n;
        f_n = f_n1;
        f_n1 += temp;
    }
}

double fibonacci(double (*p) (double), double x0, double x1, double EPS, int max_steps) {
    if (x0 > x1) {
        swap(x0, x1);
    }
    double xl = x0, xr = x1, dx;
    int f_n = 0, f_n1 = 0, temp = 0, iters = 0;
    get_closest_pair((x1 - x0) / EPS, f_n, f_n1);
    while (f_n)
    {
        // if (xr - xl < EPS) {
        //     cout << "fibonacci iter number: " << iters << endl;
        //     break;
        // }
        dx = x1 - x0;
        temp = f_n1 - f_n;
        xl = x0 + dx * ((double)temp / f_n1);
        xr = x0 + dx * ((double)f_n / f_n1);
        f_n1 = f_n;
        f_n = temp;
        if (f(xl) < f(xr)) {
            x1 = xr;
        }
        else {
            x0 = xl;
        }
        iters++;
    }
    cout << "fibonacci iter number: " << iters << endl;
    return (xl + xr) * 0.5;
}


int main()
{
    double EPS = 0.00001;
    double x0 = -1000;
    double x1 = 3000;
    //const double PHI = 1.61803398874989484820;
    int max_steps = 1000;
    cout << "x0 = " << x0 << ", x1 = " << x1 << endl;
    cout << "bisect      : " << bisect(f, x0, x1, EPS, max_steps) << endl;
    cout << "golden_ratio: " << golden_ratio(f, x0, x1, EPS, max_steps) << endl;
    cout << "fibonacci   : " << fibonacci(f, x0, x1,EPS, max_steps) << endl;
    return 0;
}