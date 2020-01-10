#include "stdafx.h"
#include <iostream>

extern "C"
{
	void _stdcall outputWord(char * str)
	{
		setlocale(0, "");
		std::cout << str << std::endl;
	}

	void _stdcall outputLogic(int logic)
	{
		if (logic == 0)
			std::cout << "false" << std::endl;
		else
			std::cout << "true" << std::endl;
	}

	void _stdcall outputNumb(int intnum)
	{
		std::cout << intnum << std::endl;
	}

	int _stdcall factorial(int number)
	{
		int res = 1;
		for (int i = 1; i <= number; i++)
		{
			res *= i;
		}
		return res;
	}

	int _stdcall numbin(int numb, int step)
	{
		int result = 1;
		for (int i = 0; i <=step; i++)
		{
			result *= numb;
		}
		return result;
	}

	bool _stdcall subword(char* str, char* sub)
	{
		char *ptrstrstr = strstr(str, sub);
		if (ptrstrstr == NULL)
			return false;
		return true;
	}

	int _stdcall wordlen(char* str)
	{
		return strlen(str);
	}
}