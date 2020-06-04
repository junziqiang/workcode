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
#include<bits/stdc++.h>
#define SERVER_PORT 6666

using namespace std;
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
    //返回介绍的字节数
    iDataNum = recv(serverSocket, recvbuff, 200, 0); //接收服务端发来的消息
    recvbuff[iDataNum] = '\0';
    int res = stringToNum(recvbuff);
    cout<<"接受到的数字："<<res<<endl;
}
/*
监听后，一直处于accept阻塞状态，
直到有客户端连接，
当客户端如数quit后，断开与客户端的连接
*/
/*
int randInt(){
    srand((int)time(0));
    int res = rand()%10;
    return res;
}
int aReturn(int x){
    return x
}
int TP(int x){


}
int TP(int x,int a){
}
*/
int main()
{
	//调用socket函数返回的文件描述符
	int serverSocket;
	//声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
	struct sockaddr_in server_addr;
	struct sockaddr_in clientAddr;
	int addr_len = sizeof(clientAddr);
	int client;
	char buffer[200]; //存储 发送和接收的信息
	int iDataNum;
	//必须先初始化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) !=2){
	    printf("require version fail!");
	    return -1;
	}
	//socket函数，失败返回-1
	//int socket(int domain, int type, int protocol);
	//第一个参数表示使用的地址类型，一般都是ipv4，AF_INET
	//第二个参数表示套接字类型：tcp：面向连接的稳定数据传输SOCK_STREAM
	//第三个参数设置为0
	//建立socket
	if((serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		perror("socket");
		return 1;
	}
	//初始化 server_addr
	memset(&server_addr,0, sizeof(server_addr));
	//初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
	//bind三个参数：服务器端的套接字的文件描述符，
	if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect");
		return 1;
	}
	//设置服务器上的socket为监听状态
	if(listen(serverSocket, 5) < 0)
	{
		perror("listen");
		return 1;
	}
	//循环 接收消息、发送消息
	while(1)
	{
		printf("监听端口: %d\n", SERVER_PORT);
		client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
		if(client < 0)
		{
			perror("accept");
			continue;
		}
		printf("wait msg...\n");
		//inet_ntoa ip地址转换函数，将网络字节序IP转换为点分十进制IP
		//表达式：char *inet_ntoa (struct in_addr);
		printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr)); //把来访问的客户端的IP地址打出来
		printf("Port is %d\n", htons(clientAddr.sin_port));
		while(1)
		{
		    recvMsg(client,buffer);
			if(strcmp(buffer, "quit") == 0) break;
			sendMsg(client,buffer);
			if(strcmp(buffer, "quit") == 0) break; //输入quit停止服务端程序
		}
	}
	close(serverSocket);
	return 0;

}
