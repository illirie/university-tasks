#include <iostream>
using namespace std;

void max_and_min(int a[], int n) {
    int min = a[0];
    int max = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < min) {
            min = a[i];
        }
        if (a[i] > max) {
            max = a[i];
        }
    }
    cout << "min: " << min << " max: " << max;
}

int enterCheck() {
    while (true) {
        int a;
        cin >> a;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(36700, '\n');
            cout << "wrong input. please, try again.";
        }
        else { return a; }
    }
}

int enterCheckLen() {
    while (true) {
        int a;
        cin >> a;
        if (cin.fail() || a <= 0) {
            cin.clear();
            cin.ignore(36700, '\n');
            cout << "wrong input. please, try again.";
        }
        else { return a; }
    }
}

int main()
{
    cout << "input length: ";
    int n = enterCheckLen();
    int* a = new int[n];
    cout << "input values: ";
    for (int i = 0; i < n; i++) {
        a[i] = enterCheck();
    }
    max_and_min(a, n);
    return 0;
}
 