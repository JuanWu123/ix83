// IX83.cpp 
//

#include "stdafx.h"
#include "IX83Opt.h"
#include <iostream>

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define MY_PORT 8888
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;
extern TCHAR szNotification[256];
extern TCHAR szResponse[256];
extern TCHAR szError[256];


int main()
{
	 WORD sockVersion = MAKEWORD(2, 2);
	 WSADATA wsaData;
	 if (WSAStartup(sockVersion, &wsaData) != 0)return 0;
	 SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	 if (slisten == INVALID_SOCKET)
	 {
		 printf("socket error !");
		 return 0;
	 }
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	//string revData;

	IX83Opt ix;
	ix.initalize();
	cout << "-------------------------------------------------" << endl;
	ix.openPort();
	cout << "input commands:" << endl;
	string s;
	string ch;
	ix.sendInitCmd();
	//getline(cin,ch);

	while (1)
	{	
		printf("Waiting Connecting...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		cout<<"A Client Connected"<<endl<<"Client Address:"<<inet_ntoa(remoteAddr.sin_addr)<<endl;

		break;
	}

	//string cmd="";
	//string precmd="";

	while ((revData[0] != 'q')&& (revData[0] != 'Q'))
	{
		//precmd = cmd;

		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			//revData[ret] = 0x00;
			cout << revData << endl;
		}

		//IX83Info::notification = "";
		//IX83Info::error = "";

		//string rep = IX83Info::response;
		//
		if ((revData[0] == 'q') || (revData[0] == 'Q')) break;
		int count = IX83Info::commandcount;
		bool rc=ix.sendCommand(revData);
		//Sleep(100);
		while(count== IX83Info::commandcount){}
		

		//if (cmd.compare(precmd) == 0) {
		//	Sleep(200);
		//	break;
		//}else {
		//	while (rep.compare(IX83Info::response) == 0 && i < INT32_MAX / 2 - 2)
		//	{
		//		i++;
		//		if ((revData[0] == 'q') || (revData[0] == 'Q'))
		//		{
		//			break;
		//		}
		//	}
		//}

		//Sleep(3000);

		string sendData = "Reponse:"+IX83Info::response + "\t Notification:" + IX83Info::notification + "\t Error:" + IX83Info::error + "\r\n";
		send(sClient, sendData.c_str(), sendData.length() + 1, 0);
	
	}
	closesocket(sClient);
	ix.closePort();

    return 0;
}

