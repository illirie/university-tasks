#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <string>
#include "process.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

//Усовершенствованная версия чата. Разработать чат для обмена сообщениями. Пусть на сервере есть чат, к которому могут
//одновременно присоединяться только 2 процесса - клиента.Остальные ждут своей очереди. Чат общий для всех, у каждого клиента
//сообщения пишутся своим цветом. Если клиент только подключился – ему отсылается вся текущая история и задается цвет фона консоли
//процесса - клиента на один из заранее предусмотренных.

vector <SOCKET> waiting_list;
vector <SOCKET> active_list;
vector<char*> chat_history;
vector<int> chat_size;
vector<int> indexes;
HANDLE semaphore;
int counter = 0;

void ClientHandler(void*) {
    int index = counter;
    counter++;
    DWORD dwWaitResult = 1;	
    int main_color = 1, second_color = 1, back_color = 0;
    while (dwWaitResult != WAIT_OBJECT_0)	
    {
        Sleep(1000);							
        dwWaitResult = WaitForSingleObject(semaphore, 1);
    }
    active_list.push_back(waiting_list[index]);
    if (index % 2) {
        main_color = 1;
        second_color = 4;
        back_color = 0;
    }
    else {
        main_color = 4;
        second_color = 1;
        back_color = 1;
    }
    send(active_list[index], (char*)&main_color, sizeof(int), NULL);
    send(active_list[index], (char*)&second_color, sizeof(int), NULL);
    send(active_list[index], (char*)&index, sizeof(int), NULL);
    cout << "color send" << endl;
    Sleep(10);
    if (chat_history.size()) {
        for (int i = 0; i < chat_history.size(); i++) {
            send(active_list[index], (char*)&indexes[i], sizeof(int), NULL);
            send(active_list[index], (char*)&chat_size[i], sizeof(int), NULL);
            send(active_list[index], chat_history[i], chat_size[i], NULL);
        }
        cout << "history send" << endl;
    }
    int msg_size;
    while (true) {
        if (recv(active_list[index], (char*)&msg_size, sizeof(int), NULL) > 0) {
            char* msg = new char[msg_size + 1];
            msg[msg_size] = '\0';
            recv(active_list[index], msg, msg_size, NULL);
            indexes.push_back(index);
            chat_history.push_back(msg);
            chat_size.push_back(msg_size);
            for (int i = 0; i < active_list.size(); i++) {
                if (i == index) {
                    continue;
                }
                send(active_list[i], (char*)&index, sizeof(int), NULL);
                send(active_list[i], (char*)&msg_size, sizeof(int), NULL);
                send(active_list[i], msg, msg_size, NULL);
            }
        }
        else {
            active_list[index] = INVALID_SOCKET;
            closesocket(waiting_list[index]);
            cout << "client #" << index + 1 << " ended his work" << endl;
            ReleaseSemaphore(semaphore, 1, NULL);
            _endthread();
        }
    }
};


int main()
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
    semaphore = CreateSemaphore(NULL, 2, 2, NULL);
    while(true) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);// инициализация сокета 
        if (newConnection == 0) {
            cout << "Error!";
        }
        else {
            cout << "Connection #" << counter + 1 << " made " << endl;
            char mes[2];
            mes[0] = (char)('1' + counter);
            mes[1] = '\0';
            waiting_list.push_back(newConnection);
            send(waiting_list[counter], mes, sizeof(mes), 0);
            _beginthread(ClientHandler, 1024, NULL);
        }
    }
    system("pause");
    closesocket(sListen);
    for (int i = 0; i < chat_history.size(); i++) {
        delete[] chat_history[i];
    }
}
