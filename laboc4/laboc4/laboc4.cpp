#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

SOCKET connected[3];

/*реализация через сокеты:
С процесса-сервера запускается n процессов клиентов. Для каждого из созданных клиентов указывается время жизни (в
секундах). Клиент запускается, существует заданное время и завершает работу. Также следует предусмотреть значение для
бесконечного времени. Требуется не менее трех одновременно запускаемых процессов-клиентов.*/

void ClientHandler(int n) {
    char mes[2];
        recv(connected[n], mes, sizeof(mes), 0);
        if (mes[0] == 'i') {
            cout << "client #" << n+1 << " started eternal process. do you want to end its live? (Y/y)";
            char c;
            while (true) {
                cin >> c;
                if (c == 'y' || c == 'Y') {
                    char msg[2] = "y";
                    send(connected[n], msg, 2, 0);
                    break;
                }
                else {
                    cout << "Wrong input, please, try again: ";
                }
            }
        }
        else if (mes[0] == 'e') {
            cout << "client #" << n+1 << " ended its work." << endl;
        }
}

int main(void)
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) { 
        cout << "Error!";
        return 1;
    }
    SOCKADDR_IN addr;// собираем информацию о адресе сокета
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");// адрес
    addr.sin_port = htons(1111);// порт
    addr.sin_family = AF_INET;// семейство протоколов

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);// сокет для прослушивания порта
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));// привязка адреса сокету
    listen(sListen, SOMAXCONN);// прослушивание, сколько запросов ожидается

    int sizeofaddr = sizeof(addr);
    SOCKET newConnection;
    for (int i = 0; i < 3; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);// инициализация сокета 
        if (newConnection == 0) {
            cout << "Error!";
        }
        else {
            cout << "Connection #" << i + 1 << " made " << endl;
            char mes[2];
            mes[0] = (char)('1' + i);
            mes[1] = '\0';
            connected[i] = newConnection;
            send(connected[i], mes, sizeof(mes), 0);
        }
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
    }
    system("pause");
    closesocket(sListen);
    for (int i = 0; i < 3; i++) {
        closesocket(connected[i]);
    }
    WSACleanup();
	return 0;
}
