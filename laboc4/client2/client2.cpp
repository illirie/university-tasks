#include <WinSock2.h>
#include <winsock.h>
#include <stdio.h>
#include <iostream>
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")

using namespace std;
SOCKET Connection;

int enterCheck() {
    while (true) {
        int a;
        cin >> a;
        if (cin.fail() || a < 0) {
            if (a == -1) {
                return a;
            }
            cin.clear();
            cin.ignore(36700, '\n');
            cout << "wrong input. please, try again.";
        }
        else { return a; }
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
    cout << "Input time of living (-1 for infinity): ";
    int n = enterCheck();
    if (n != -1) {
        Sleep(n*1000);
        const char* message = "e";
        send(Connection, message, 2, 0);
    }
    else {
        const char* message = "i";
        send(Connection, message, 2, 0);
        cout << "sent";
        char msg[2];
        recv(Connection, msg, sizeof(msg), 0);
    }
    closesocket(Connection);
    WSACleanup();
    return 0;
}