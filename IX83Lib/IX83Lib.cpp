// IX83Lib.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <Windows.h>
#include <direct.h>
#include "IX83Lib.h"
#include "IX83Opt.h"
#include <string>
#include <tchar.h>
//#define _DEBUG_

//#define GetCurrentDir _getcwd
//#define	GT_MDK_PORT_MANAGER	"msl_pm.dll"

using namespace std;

typedef int(*GetMetrixcs)(int);
//typedef void(*pcloseCommand)();

IX83Opt ix;

bool openPort()
{
	bool value;
	value=ix.initalize();
	if (!value)return false;
	int port=ix.openPort();
	if (port < 1)return false;
	ix.sendInitCmd();
}

IX83LIB void sendCommand(std::string cmd,string& response, string& notification,string& error)
{
	if (ix.address == NULL) {
		openPort();
		sendCommand(cmd, response,notification,error);
	}
	ix.sendCommand(cmd);
	response = ::response;
	notification = ::notification;
	error = ::error;
}

IX83LIB void closeCommand() {
	
	ix.closePort();
}

IX83LIB int add(int a,int b) {
	return a + b;
}

IX83LIB void sendCommandWithoutStr(std::string cmd)
{
	if (ix.address == NULL) {
		openPort();
		sendCommandWithoutStr(cmd);
	}
	ix.sendCommand(cmd);

}

IX83LIB int user32Test(int index)
{
	HMODULE hDLL= LoadLibrary(_T("user32.dll"));
	GetMetrixcs getMetrixcs= (GetMetrixcs)GetProcAddress(hDLL, "GetSystemMetrics");
	int num= getMetrixcs(index);
	return num;

}

