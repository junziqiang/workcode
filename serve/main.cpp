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
    //���ؽ��ܵ��ֽ���
    iDataNum = recv(serverSocket, recvbuff, 200, 0); //���շ���˷�������Ϣ
    recvbuff[iDataNum] = '\0';
    int res = stringToNum(recvbuff);
    cout<<"���ܵ������֣�"<<res<<endl;
}
/*
������һֱ����accept����״̬��
ֱ���пͻ������ӣ�
���ͻ�������quit�󣬶Ͽ���ͻ��˵�����
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
	//����socket�������ص��ļ�������
	int serverSocket;
	//���������׽���sockaddr_in�ṹ��������ֱ��ʾ�ͻ��˺ͷ�����
	struct sockaddr_in server_addr;
	struct sockaddr_in clientAddr;
	int addr_len = sizeof(clientAddr);
	int client;
	char buffer[200]; //�洢 ���ͺͽ��յ���Ϣ
	int iDataNum;
	//�����ȳ�ʼ��
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) !=2){
	    printf("require version fail!");
	    return -1;
	}
	//socket������ʧ�ܷ���-1
	//int socket(int domain, int type, int protocol);
	//��һ��������ʾʹ�õĵ�ַ���ͣ�һ�㶼��ipv4��AF_INET
	//�ڶ���������ʾ�׽������ͣ�tcp���������ӵ��ȶ����ݴ���SOCK_STREAM
	//��������������Ϊ0
	//����socket
	if((serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		perror("socket");
		return 1;
	}
	//��ʼ�� server_addr
	memset(&server_addr,0, sizeof(server_addr));
	//��ʼ���������˵��׽��֣�����htons��htonl���˿ں͵�ַת�������ֽ���
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//ip�����Ǳ���������ip��Ҳ�����ú�INADDR_ANY���棬����0.0.0.0���������е�ַ
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//����bind��accept֮��ĺ����������׽��ֲ���������Ҫǿ��ת����(struct sockaddr *)
	//bind�����������������˵��׽��ֵ��ļ���������
	if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect");
		return 1;
	}
	//���÷������ϵ�socketΪ����״̬
	if(listen(serverSocket, 5) < 0)
	{
		perror("listen");
		return 1;
	}
	//ѭ�� ������Ϣ��������Ϣ
	while(1)
	{
		printf("�����˿�: %d\n", SERVER_PORT);
		client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
		if(client < 0)
		{
			perror("accept");
			continue;
		}
		printf("wait msg...\n");
		//inet_ntoa ip��ַת���������������ֽ���IPת��Ϊ���ʮ����IP
		//���ʽ��char *inet_ntoa (struct in_addr);
		printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr)); //�������ʵĿͻ��˵�IP��ַ�����
		printf("Port is %d\n", htons(clientAddr.sin_port));
		while(1)
		{
		    recvMsg(client,buffer);
			if(strcmp(buffer, "quit") == 0) break;
			sendMsg(client,buffer);
			if(strcmp(buffer, "quit") == 0) break; //����quitֹͣ����˳���
		}
	}
	close(serverSocket);
	return 0;

}
