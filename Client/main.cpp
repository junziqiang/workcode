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
    输入字符数组返回数字
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
    输入要转化的数字和对应的字符数组
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
    发送消息
*/
void sendMsg(int serverSocket,char sendbuff[]){
    cout<<"请输入要发送的数字:"<<endl;
    int num;
    cin>>num;
    numTOString(num,sendbuff);
    send(serverSocket, sendbuff, strlen(sendbuff), 0);
}
void recvMsg(int serverSocket,char recvbuff[]){
    int iDataNum;
    recvbuff[0]='\0';
    iDataNum = recv(serverSocket, recvbuff, 200, 0); //接收服务端发来的消息
    recvbuff[iDataNum] = '\0';
    int res = stringToNum(recvbuff);
    cout<<"接受到的数字："<<res<<endl;
}
int main()
{

	//客户端只需要一个套接字文件描述符，用于和服务器通信
	int serverSocket;
	//描述服务器的socket
	struct sockaddr_in serverAddr;
	char sendbuf[200]; //存储 发送的信息
	char recvbuf[200]; //存储 接收到的信息
	int iDataNum;
	//下面代码初始化
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
	//指定服务器端的ip，本地测试：127.0.0.1
	//inet_addr()函数，将点分十进制IP转换成网络字节序IP
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
