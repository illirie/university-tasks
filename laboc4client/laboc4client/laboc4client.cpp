#include <WinSock2.h>
#include <winsock.h>
#include <stdio.h>
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WORD ver = MAKEWORD(2, 2);
    WSADATA wsaData;
    int retVal = 0;

    WSAStartup(ver, (LPWSADATA)&wsaData);

    LPHOSTENT hostEnt;

    hostEnt = gethostbyname("localhost");

    if (!hostEnt)
    {
        printf("Unable to collect gethostbyname\n");
        WSACleanup();
        return 1;
    }

    //Создаем сокет
    SOCKET clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSock == SOCKET_ERROR)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverInfo;

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr = *((LPIN_ADDR)*hostEnt->h_addr_list);
    serverInfo.sin_port = htons(1111);

    retVal = connect(clientSock, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to connect\n");
        WSACleanup();
        return 1;
    }

    printf("Connection made sucessfully\n");

    const char* pBuf = "Request";

    printf("Sending request from client\n");
    retVal = send(clientSock, pBuf, strlen(pBuf), 0);

    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to send\n");
        WSACleanup();
        return 1;
    }

    char szResponse[9];
    retVal = recv(clientSock, szResponse, 9, 0);

    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        WSACleanup();
        return 1;
    }

    printf("Got the response from server\n%s\n", szResponse);

    closesocket(clientSock);
    WSACleanup();

    return 0;
}
