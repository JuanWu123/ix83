#pragma once

#include "stdafx.h"
#include <Windows.h>
#include "mdk_if.h"
#include <direct.h>

#define _DEBUG_

#define GetCurrentDir _getcwd
#define	GT_MDK_PORT_MANAGER	"msl_pm.dll"

//static TCHAR szNotification[256];
//static TCHAR szResponse[256];
//static TCHAR szError[256];

static class IX83Info {
public:
	static string response;
	static string notification;
	static string error;
	static int commandcount;
};


class IX83Opt
{

public:
	IX83Opt();
	~IX83Opt();
	MDK_MSL_CMD cmd;
	void* address;

	//static string notification;
	//static string response;
	//static string error;

	HMODULE						m_hMod;
	fn_MSL_PM_GetInterfaceInfo	pfn_GetInterfaceInfo;
	fn_MSL_PM_Initialize		pfn_InifInterface;
	fn_MSL_PM_EnumInterface		pfn_EnumInterface;
	fn_MSL_PM_OpenInterface		pfn_OpenInterface;
	fn_MSL_PM_CloseInterface	pfn_CloseInterface;
	fn_MSL_PM_SendCommand		pfn_SendCommand;
	fn_MSL_PM_RegisterCallback	pfn_RegisterCallback;

	void load();
	bool initalize();
	int openPort();
	bool closePort();
	bool sendCommand(string);
	void sendInitCmd();
	
	static LPCWSTR stringToLPCWSTR(string);

	static int	CALLBACK ErrorCallback(
		ULONG		MsgId,			// Callback ID.
		ULONG		wParam,			// Callback parameter, it depends on callback event.
		ULONG		lParam,			// Callback parameter, it depends on callback event.
		PVOID		pv,				// Callback parameter, it depends on callback event.
		PVOID		pContext,		// This contains information on this call back function.
		PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
		);
	static int	CALLBACK CommandCallback(
		ULONG		MsgId,			// Callback ID.
		ULONG		wParam,			// Callback parameter, it depends on callback event.
		ULONG		lParam,			// Callback parameter, it depends on callback event.
		PVOID		pv,				// Callback parameter, it depends on callback event.
		PVOID		pContext,		// This contains information on this call back function.
		PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
		);
	static int	CALLBACK NotifyCallback(
		ULONG		MsgId,			// Callback ID.
		ULONG		wParam,			// Callback parameter, it depends on callback event.
		ULONG		lParam,			// Callback parameter, it depends on callback event.
		PVOID		pv,				// Callback parameter, it depends on callback event.
		PVOID		pContext,		// This contains information on this call back function.
		PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
		);

private:
	string GetDllPath();
	static void Wchar_tToString(std::string& , wchar_t *);

	wchar_t *multiByteToWideChar(const string&);
	void stringToWstring(std::wstring&, std::string);

	
};


