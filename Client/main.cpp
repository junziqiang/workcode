#include <sys/stat.h>
#include <fcntl.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
#include <errno.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include<ws2tcpip.h>
#include <stdio.h>
#include <unistd.h>
#define SERVER_PORT 6666
# include<stdio.h>
#include<stdlib.h>
#include<bits/stdc++.h>
#include<string>
using namespace std;
/*
    �����ַ����鷵������
*/
int stringToNum(char buff[]){
    string s = buff;
    stringstream ss;
    ss<<s;
    int res;
    ss>>res;
    return res;
}
/*
    ����Ҫת�������ֺͶ�Ӧ���ַ�����
*/
void numTOString(int num,char c[]){
    string res;
    stringstream ss;
    ss<<num;
    ss>>res;
    int i=0;
    strcpy(c,res.c_str());
}
/*
    ������Ϣ
*/
void sendMsg(int serverSocket,char sendbuff[]){
    cout<<"������Ҫ���͵�����:"<<endl;
    int num;
    cin>>num;
    numTOString(num,sendbuff);
    send(serverSocket, sendbuff, strlen(sendbuff), 0);
}
void recvMsg(int serverSocket,char recvbuff[]){
    int iDataNum;
    recvbuff[0]='\0';
    iDataNum = recv(serverSocket, recvbuff, 200, 0); //���շ���˷�������Ϣ
    recvbuff[iDataNum] = '\0';
    int res = stringToNum(recvbuff);
    cout<<"���ܵ������֣�"<<res<<endl;
}
int main()
{

	//�ͻ���ֻ��Ҫһ���׽����ļ������������ںͷ�����ͨ��
	int serverSocket;
	//������������socket
	struct sockaddr_in serverAddr;
	char sendbuf[200]; //�洢 ���͵���Ϣ
	char recvbuf[200]; //�洢 ���յ�����Ϣ
	int iDataNum;
	//��������ʼ��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) !=2){
	    printf("require version fail!");
	    return -1;
	}
	if((serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		perror("socket");
		return 1;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	//ָ���������˵�ip�����ز��ԣ�127.0.0.1
	//inet_addr()�����������ʮ����IPת���������ֽ���IP
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("connect...\n");
	while(1)
	{
	    sendMsg(serverSocket,sendbuf);
		if(strcmp(sendbuf, "quit") == 0) break;
		recvMsg(serverSocket,recvbuf);
	}

	close(serverSocket);

	return 0;
}
