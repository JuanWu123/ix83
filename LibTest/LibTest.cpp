// LibTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <string>

using namespace std;


int main()
{
	string cmd;
	string r, n, e;
	typedef int(*popenCommand)();
	//typedef void (*psendCommand)(string cmd, string& response,string& notify,string& error);
	//typedef void(*pcloseCommand)();

	//ypedef int(*pUser32Test)(int index);

	popenCommand openPort;
	//psendCommand sendCommand;
	//pcloseCommand closeCommand;
	//pUser32Test user32Test;
	HINSTANCE hDLL;
	hDLL = LoadLibrary(_T("IX83LV.dll"));

	//sendCommand = (psendCommand)GetProcAddress(hDLL, "sendCommand");
	//closeCommand= (pcloseCommand)GetProcAddress(hDLL, "closeCommand");
	openPort =(popenCommand)GetProcAddress(hDLL, "openPort");

	int num = openPort();

	cout << "address" << num<<endl;

	//cout << "input commands:" << endl;
	//cmd = "OB 1";
	//sendCommand(cmd, r, n, e);

	////getline(cin, cmd);
	//while ((cmd[0] != 'q') && (cmd[0] != 'Q'))
	//{
	//	sendCommand(cmd, r, n, e);
	//	getline(cin, cmd);
	//	cout << r << endl << n << endl << e << endl;
	//}

	//closeCommand();

	//FreeLibrary(hDLL);
	cin.get();
    return 0;
}

