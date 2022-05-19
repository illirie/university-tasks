#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
//#include <unistd.h>

using namespace std;
enum LState {s, k, n, f};

void incorrectOutput() {
    cout << "error. something wrong with file or command." << endl;
}
void fileCopy() { // функция для копирования файла

}
void fileTransfer() { // функция перемещения файла

}
void fileInfo(string s) { // функция, считывающая информацию о файле
    char* filename = new char[s.length()];
    for (int i = 0; i < s.length(); i++) {
        filename[i] = s[i];
    }
    struct stat buff;
    if (stat(filename, &buff)) {
        incorrectOutput();
    }
    else {
        cout << buff.st_mode << ' ' << buff.st_size << ' ' << buff.st_mtime;
    }
}
void fileChange() { // функция изменения прав доступа к файлу.
    
}
void help() { // справка для работы с программой
    cout << "cp filename for copy" << endl;
    cout << "tr filename new directory for transfer" << endl;
    cout << "in filename for information about file" << endl;
    cout << "ch filename for changing rules" << endl;
}
bool continueWork() {
    cout << "do you want to finish work? (y/n) ";
    char a;
    while (true) {
        cin >> a;
        switch (a) {
        case 'y': return 1;
        case 'Y': return 1;
        case 'n': return 0;
        case 'N': return 0;
        default: {
            cin.ignore(30000, '\n');
            cout << "incorrect input. please, try again. ";
        }
      }
    }
}

int main()
{
    while (true) {
        string s;
        cout << "Input command: ";
        getline(cin, s, '\n');
        istringstream i(s);
        vector<string> lex((istream_iterator<string>(i)), istream_iterator<string>());
        if (lex.size() != 0) {
            if (lex[0] == "cp") {
                fileCopy();
            }
            else if (lex[0] == "tr") {
                fileTransfer();
            }
            else if (lex[0] == "in") {
                fileInfo(lex[1]);
            }
            else if (lex[0] == "ch") {
                fileChange();
            }
            else if (lex[0] == "--help") {
                help();
            }
            else { incorrectOutput(); }
        }
        else {
            incorrectOutput();
        }
        if (continueWork()) {
            return 0;
        }
        lex.clear();
        cin.ignore(30000, '\n');
    }
}
