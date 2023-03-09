#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <thread>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;


void ErrorHandling(const char* message);
void ServerFunc();
void CliFunc();


int main(int argc, char* argv[])
{
   
    //std::thread t1(ServerFunc);

    while (true)
    {
        int mode;
        cout << "��� ���� 1= ���� , 2= Ŭ���̾�Ʈ" << endl;
        cin >> mode;

        switch (mode)
        {
        case 1: ServerFunc(); break;
        case 2:  CliFunc(); break;
        default:
            break;
        }
       
    }
}

void CliFunc() {
    char   sendMessage[1024];
    cout << "���� �Է�" << endl;
    cin >> sendMessage;
    WSADATA     wsaData;
    SOCKET      hSocket;
    SOCKADDR_IN servAddr;

    int     port = 5001;

    char message[30];
    //char    sendMessage[] = "give me message.";
  
    int     strLen;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    hSocket = socket(PF_INET, SOCK_STREAM, 0);

    if (hSocket == INVALID_SOCKET)
        ErrorHandling("hSocket() error!");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = inet_addr("127.0.1");


    if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");

    send(hSocket, sendMessage, strlen(sendMessage) + 1, 0);



    strLen = recv(hSocket, message, sizeof(message) - 1, 0);

    if (strLen == -1)
        ErrorHandling("read() error!");

    printf("%s \n", message);

    closesocket(hSocket);
    WSACleanup();
    
}


void ServerFunc() {
    cout << "�����" << endl;
    WSADATA     wsaData;
    SOCKET      hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;

    short   port = 5001;
    int     szClntAddr;
    //char    message[] = "����ȭ ���� �Ǿ����ϴ�.";
    char    buffer[1024] = { 0, };

    // ���� �ʱ�ȭ, ���ϻ���ϱ��� �ʱ�ȭ �ؾ���
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // TCP ���ϻ���
    hServSock = socket(PF_INET, SOCK_STREAM, 0);

    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    // ������ �ּ�����
    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // ���� PC IP�ּ� ���
    servAddr.sin_port = htons(port);        // ��Ʈ��ȣ

    // �ּ� ����
    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("bind() error!");
    }


    while (1)
    {

        // ���� ������ �ְ� 
        // ���Ӵ�� 5
        if (listen(hServSock, 5) == SOCKET_ERROR) {
            ErrorHandling("listen() error!");
        }


        // ���� ����
        szClntAddr = sizeof(clntAddr);
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);   // Ŭ���̾�Ʈ�� ��� ��


        if (hClntSock == INVALID_SOCKET) {
            ErrorHandling("accept() error!");
        }


        // ------------------------------
       // int recvlen = recv(hClntSock, buffer, sizeof(message), 0);
        int recvlen = recv(hClntSock, buffer, 1024, 0);

        if (recvlen > 0)
        {
            printf("%s\n", buffer);
            send(hClntSock, buffer, 1024, 0);
            // �߿�
            break;
        }
    }



    closesocket(hClntSock);
    closesocket(hServSock);
    WSACleanup();
}

void ErrorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


