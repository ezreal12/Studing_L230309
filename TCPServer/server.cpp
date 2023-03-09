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
        cout << "모드 선택 1= 서버 , 2= 클라이언트" << endl;
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
    cout << "문자 입력" << endl;
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
    cout << "대기중" << endl;
    WSADATA     wsaData;
    SOCKET      hServSock, hClntSock;
    SOCKADDR_IN servAddr, clntAddr;

    short   port = 5001;
    int     szClntAddr;
    //char    message[] = "무궁화 꽃이 피었습니다.";
    char    buffer[1024] = { 0, };

    // 윈속 초기화, 소켓사용하기전 초기화 해야함
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    // TCP 소켓생성
    hServSock = socket(PF_INET, SOCK_STREAM, 0);

    if (hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    // 소켓의 주소정보
    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 현재 PC IP주소 사용
    servAddr.sin_port = htons(port);        // 포트번호

    // 주소 지정
    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("bind() error!");
    }


    while (1)
    {

        // 접속 받을수 있게 
        // 접속대기 5
        if (listen(hServSock, 5) == SOCKET_ERROR) {
            ErrorHandling("listen() error!");
        }


        // 연결 수락
        szClntAddr = sizeof(clntAddr);
        hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);   // 클라이언트와 통신 할


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
            // 중요
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


