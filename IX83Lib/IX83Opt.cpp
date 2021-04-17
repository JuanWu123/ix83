#include "stdafx.h"
#include "IX83Opt.h"
#include <iostream>
#include <tchar.h>

IX83Opt::IX83Opt()
{
	pfn_GetInterfaceInfo = NULL;
	pfn_InifInterface = NULL;
	pfn_EnumInterface = NULL;
	pfn_OpenInterface = NULL;
	pfn_CloseInterface = NULL;
	pfn_SendCommand = NULL;
	pfn_RegisterCallback = NULL;
}


IX83Opt::~IX83Opt()
{
	FreeLibrary(m_hMod);
}

void IX83Opt::load()
{
	
}

bool IX83Opt::initalize()
{
	m_hMod = LoadLibrary(_T(GT_MDK_PORT_MANAGER));
	if (m_hMod == NULL) return false;
	
	//-----------------------------------------------------
	// get function address
	pfn_InifInterface = (fn_MSL_PM_Initialize)GetProcAddress(m_hMod, "MSL_PM_Initialize");
	if (pfn_InifInterface == NULL) return false;

	pfn_EnumInterface = (fn_MSL_PM_EnumInterface)GetProcAddress(m_hMod, "MSL_PM_EnumInterface");
	if (pfn_EnumInterface == NULL) return false;

	pfn_GetInterfaceInfo = (fn_MSL_PM_GetInterfaceInfo)GetProcAddress(m_hMod, "MSL_PM_GetInterfaceInfo");
	if (pfn_GetInterfaceInfo == NULL) return false;

	pfn_OpenInterface = (fn_MSL_PM_OpenInterface)GetProcAddress(m_hMod, "MSL_PM_OpenInterface");
	if (pfn_OpenInterface == NULL)return false;

	pfn_CloseInterface = (fn_MSL_PM_CloseInterface)GetProcAddress(m_hMod, "MSL_PM_CloseInterface");
	if (pfn_CloseInterface == NULL) return false;

	pfn_SendCommand = (fn_MSL_PM_SendCommand)GetProcAddress(m_hMod, "MSL_PM_SendCommand");
	if (pfn_SendCommand == NULL)  return false;

	pfn_RegisterCallback = (fn_MSL_PM_RegisterCallback)GetProcAddress(m_hMod, "MSL_PM_RegisterCallback");
	if (pfn_RegisterCallback == NULL) return false;
	return true;

}

int IX83Opt::openPort()
{
	int ret = 0;
	ret=(*pfn_InifInterface)();
	Sleep(1000);
	//cout << "InifInterface status: " + ret << endl;
	if (pfn_EnumInterface == NULL)	return -1;
	int count = (*pfn_EnumInterface)();
	//cout << "Interface Count:" << count << endl;
	//cout << "Interface Address:" << endl;
	if (count == 0)return 0;
	void* ifData = NULL;
	for (int i = 0; i < count; i++) {
		if (pfn_GetInterfaceInfo != NULL) {
			if ((*pfn_GetInterfaceInfo)(i, &ifData)) {
				Sleep(1000);
				//cout << "\t" << hex << (unsigned long)(ifData) << endl;
			}
		}
	}
	address = ifData;

	bool rc = (*pfn_OpenInterface)(ifData);
	if (!rc) {
		//cout << "Could not open port!";
		return -1;
	}
	else {

		if (pfn_RegisterCallback != NULL) {
			(*pfn_RegisterCallback)(
				ifData,
				CommandCallback,
				NotifyCallback,
				ErrorCallback,
				this);	// Context
		}
		return 1;
	}
}

bool IX83Opt::closePort()
{
	bool rc = false;
	if (address != 0) {
		if (pfn_CloseInterface == NULL)	return false;
		sendCommand("L 0,0");
		Sleep(1000);
		bool rc = (*pfn_CloseInterface)(address);
	}
	return rc;
}

void IX83Opt::sendCommand(string command) {
	//void		*mdk_if;
	void*		result;
	bool		rc;
	memset(&cmd, 0x00, sizeof(MDK_MSL_CMD));
	command+= ("\r\n");
	size_t len = command.length();
	wstring szDst ;
	stringToWstring(szDst, command);
#ifdef	_UNICODE
		size_t	conv;
		wcstombs_s(&conv, (char *)cmd.m_Cmd, (size_t)MAX_COMMAND_SIZE, szDst.c_str(), _TRUNCATE);
#else
		memcpy(cmd.m_Cmd, (LPCSTR)s, len + 1);
#endif
		////////////////////////////////////////////////////////////
		// set parameters for command
		cmd.m_CmdSize = len;
		cmd.m_Callback = CommandCallback;
		cmd.m_Context = NULL;		// this pointer passed by pv
		cmd.m_Timeout = 10000;	// (ms)
		cmd.m_Sync = FALSE;

		cmd.m_Command = TRUE;		// TRUE: Command , FALSE: it means QUERY form ('?').

									/////////////////////////////////////////////////////////
									//	Send command
		if (pfn_SendCommand != NULL&&address!=NULL) {
			rc = (*pfn_SendCommand)(address, &cmd);
		}
}

void IX83Opt::sendInitCmd()
{
	string l,o,n;
	l = "L 1,1";
	sendCommand(l);
	Sleep(3000);

	o = "OPE 0";
	sendCommand(o);
	Sleep(3000);

	n = "N 1";
	sendCommand(n);
}


string IX83Opt::GetDllPath()
{
	char cCurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return "Not Find";
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
												   //printf("The current working directory is %s", cCurrentPath);
												   //cout << endl;	
	return string(cCurrentPath) + "\\" + GT_MDK_PORT_MANAGER;
}
void IX83Opt::Wchar_tToString(std::string& szDst, wchar_t * wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用  
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量  
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用  
	szDst = psText;// std::string赋值  
	delete[]psText;// psText的清除  
}

void IX83Opt::stringToWstring(std::wstring& szDst, std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	std::wstring r = wszUtf8;
	delete[] wszUtf8;
}

#pragma	region
int	CALLBACK IX83Opt::ErrorCallback(
	ULONG		MsgId,			// Callback ID.
	ULONG		wParam,			// Callback parameter, it depends on callback event.
	ULONG		lParam,			// Callback parameter, it depends on callback event.
	PVOID		pv,				// Callback parameter, it depends on callback event.
	PVOID		pContext,		// This contains information on this call back function.
	PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
	)
{
	UNREFERENCED_PARAMETER(MsgId);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(pContext);
	UNREFERENCED_PARAMETER(pCaller);
	UNREFERENCED_PARAMETER(pv);

	IX83Opt	*ix = (IX83Opt *)pContext;

	/////////////////////////////////////////////////////////
	//	show notification

	//cout<<"ERROR: --- Interface is going to be closed";
	//::error= "ERROR: --- Interface is going to be closed";
	//closePort();
	return	0;
}
#pragma	endregion

#pragma	region
int	CALLBACK IX83Opt::CommandCallback(
	ULONG		MsgId,			// Callback ID.
	ULONG		wParam,			// Callback parameter, it depends on callback event.
	ULONG		lParam,			// Callback parameter, it depends on callback event.
	PVOID		pv,				// Callback parameter, it depends on callback event.
	PVOID		pContext,		// This contains information on this call back function.
	PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
	)
{
	UNREFERENCED_PARAMETER(MsgId);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(pContext);
	UNREFERENCED_PARAMETER(pCaller);
	UNREFERENCED_PARAMETER(pv);

	IX83Opt	*ix = (IX83Opt *)pContext;

	if (ix == NULL)	return	0;
	/////////////////////////////////////////////////////////
	//	show result.
	TCHAR	szMsg[256];

	if (pContext != NULL) {
		_stprintf_s(szMsg, 256, _T("Response - %S"), ix->cmd.m_Rsp);
		//cout << "Command Response:" << endl << "\t" << ix -> cmd.m_Rsp << endl;
	}
	else {
		//cout << "Command Error." << endl;
	}
	::response =(char*) ix->cmd.m_Rsp;

	return	0;
}
#pragma	endregion

#pragma	region

//-----------------------------------------------------------------------------
//	NOTIFICATION: call back entry from SDK port manager
int	CALLBACK IX83Opt::NotifyCallback(
	ULONG		MsgId,			// Callback ID.
	ULONG		wParam,			// Callback parameter, it depends on callback event.
	ULONG		lParam,			// Callback parameter, it depends on callback event.
	PVOID		pv,				// Callback parameter, it depends on callback event.
	PVOID		pContext,		// This contains information on this call back function.
	PVOID		pCaller			// This is the pointer specified by a user in the requirement function.
	)
{
	UNREFERENCED_PARAMETER(MsgId);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(pContext);
	UNREFERENCED_PARAMETER(pCaller);
	UNREFERENCED_PARAMETER(pv);
	IX83Opt	*ix = (IX83Opt *)pContext;
	TCHAR	szMsg[256];
	_stprintf_s(szMsg, 256, _T("Notifycation - %S"), pv);
	//cout << "Notification:" << endl << "\t" << (char*)pv << endl;
	::notification = (char*)pv;
	return	0;
}
#pragma	endregion