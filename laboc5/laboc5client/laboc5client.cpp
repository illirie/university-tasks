#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

SOCKET Connection;

void ClientHandler() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int main_color = 1;
    int second_color = 1;
    int main_index = 1, second_index = 0;
    int msg_size;
    recv(Connection, (char*)&main_color, sizeof(int), NULL);
    recv(Connection, (char*)&second_color, sizeof(int), NULL);
    recv(Connection, (char*)&main_index, sizeof(int), NULL);
    //cout << main_color << second_color << endl;
    //SetConsoleTextAttribute(handle, ((back_color << 4) | main_color));
    SetConsoleTextAttribute(handle, main_color);
    while (true) {
        recv(Connection, (char*)&second_index, sizeof(int), NULL);
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        SetConsoleTextAttribute(handle, second_color);
        cout << second_index + 1 << ": " << msg << endl;
        SetConsoleTextAttribute(handle, main_color);
        cout << main_index + 1 << ": ";
        delete[] msg;
    }
}

int main()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Error!";
        exit(1);
    }

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;
    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    connect(Connection, (SOCKADDR*)&addr, sizeof(addr));
    char num[2];
    recv(Connection, num, sizeof(num), 0);
    cout << "connected " << num << endl;
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
    string msg1;
    while (true) {
        getline(cin, msg1);
        int msg_size = msg1.size();
        send(Connection, (char*)&msg_size, sizeof(int), NULL);
        send(Connection, msg1.c_str(), msg_size, NULL);
        Sleep(10);
    }
    system("pause");
    return 0;
}
