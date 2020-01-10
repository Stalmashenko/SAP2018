#pragma once
#include "Headers.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"
#include "In.h"
#include "Error.h"

bool Semantics(LT::LexTable &Lextable, In::IN &InStruct, IT::IdTable &idtable, Log::LOG &log);

struct semFunction
{
	int parmsNumber;//количество параметров
	int* parmTypesArray;
	unsigned char* name;
	semFunction()
	{
		parmsNumber = -1;
	}
};

semFunction* CreateFunc(int number, int* arr, unsigned char* name);

struct ListId
{
	unsigned char* symbol;
	ListId *Next, *Head;
};

struct ListFunc
{
	semFunction* symbol;
	ListFunc *Next, *Head;
};
