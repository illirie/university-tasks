#include <iostream>
#include <math.h>
#include <string>

using namespace std;
int c_in[2];
int c_out[2];

double count(double x) {
    double result = 0;
    int fact = 1;
    for (int i = 0; i < 1001; i++) {
        if (i != 0) {
            fact = fact * i;
        }
        result = (sin(x) + cos(i)) / (x + fact);
    }
    return result;
}

void frontend() {

}
void backend() {

}
void help() {
    cout << "start program without this key and listen the instructions." << endl;
}

int main()
{
    string s;
    cout << "Input 'start' or '--help' for start: ";
    cin >> s;
    if (s == "--help") {
        help();
    }
    else if (s == "start") {

    }
    else {
        cout << "wrong input.";
    }
}
