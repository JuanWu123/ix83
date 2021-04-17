// IX83.cpp :
//

#include "stdafx.h"
#include "IX83Opt.h"
#include <iostream>
using namespace std;
int main()
{
	IX83Opt ix;
	ix.initalize();
	cout << "-------------------------------------------------" << endl;
	//void* address=NULL;
	ix.openPort();
	//ix.sendInitCmd();
	cout << "input commands:" << endl;
	string s;
	string ch;
	ix.sendInitCmd();
	getline(cin,ch);

	while ((ch[0] != 'q')&& (ch[0] != 'Q'))
	{
		ix.sendCommand(ch);
		getline(cin,ch);
	}
	
	ix.closePort();
    return 0;
}

