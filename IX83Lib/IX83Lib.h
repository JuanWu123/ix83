#pragma once
#include <string>
#include "IX83Opt.h"
#ifndef IX83LIB_H_
#define IX83LIB_H_

#ifdef IX83LIB
#define IX83LIB extern "C" _declspec(dllimport) 
#else
#define IX83LIB extern "C" _declspec(dllexport) 
#endif

//IX83LIB int openPort(void*);
IX83LIB void sendCommand(std::string,string&,string&,string&);
IX83LIB void closeCommand();
IX83LIB int add(int, int);
IX83LIB void sendCommandWithoutStr(string);
IX83LIB int user32Test(int);

bool openPort();

#endif