#include "pch.h"
#include "Headers.h"
#include "Semantics.h"


void Add(semFunction* x, ListFunc *&List)
{
	ListFunc *temp = new ListFunc;
	temp->symbol = x;
	temp->Next = List->Head;
	List->Head = temp;
}

void ClearList(ListFunc *List)
{
	while (List->Head != NULL)
	{
		ListFunc *temp = List->Head->Next;
		delete List->Head;
		List->Head = temp;
	}
}

void Add(unsigned char* x, ListId *&List)
{
	ListId *temp = new ListId;
	temp->symbol = x;
	temp->Next = List->Head;
	List->Head = temp;
}

void ClearList(ListId *List)
{
	while (List->Head != NULL)
	{
		ListId *temp = List->Head->Next;
		delete List->Head;
		List->Head = temp;
	}
}

bool checkId(ListId *List, unsigned char* f)
{
	ListId *temp = List->Head;
	bool find = false;
	int co = 0;
	while (temp != NULL)
	{
		int k = 0;
		co = 0;
		for (k = 0; *(temp->symbol + k) != '\0' && *(f + k) != '\0'; k++)
		{
			if (*(temp->symbol + k) == *(f + k))
			{
				co++;
			}
		}
		if (co == k)
		{
			find = true;
			break;
		}
		temp = temp->Next;
	}
	return find;
}

bool Compare(semFunction* f1, semFunction* f2)
{
	if (f1->name != f2->name)
		return false;
	if (f1->parmsNumber != f2->parmsNumber)
		return false;
	for (int i = 0; i < f1->parmsNumber; i++)
	{
		if (f1->parmTypesArray[i] != f2->parmTypesArray[i])
			return false;
	}
	return true;
}

bool checkFunc(ListFunc *MyList, semFunction* f) //объ€вление функции с одинаковым именем 
{
	ListFunc *temp = MyList->Head;
	bool find = false;
	while (temp != NULL)
	{
		if (Compare(temp->symbol, f))
		{
			if (find)
			{
				throw ERROR_THROW(615);
			}
			find = true;
		}
		temp = temp->Next;
	}
	return find;
}

semFunction* CreateFunc(int number, int* arr, unsigned char* name)
{
	semFunction* f = new semFunction();
	f->parmsNumber = number;
	f->parmTypesArray = new int[number];
	for (int i = 0; i < number; i++)
		f->parmTypesArray[i] = arr[i];
	f->name = name;
	return f;
}

bool checkLib(unsigned char* t)
{
	bool z = false;
	unsigned char wordlen[] = "wordlen";
	unsigned char subword[] = "subword";
	unsigned char getnod[] = "getnod";
	unsigned char getnok[] = "getnok";
	unsigned char* libFunctions[] = { wordlen, subword, getnod, getnok, NULL };

	for (int i = 0; libFunctions[i] != NULL; i++)
	{
		int count = 0;
		int k;
		for (k = 0; *((libFunctions[i]) + k) != '\0' && *(t + k) != '\0'; k++)
		{
			if (*((libFunctions[i]) + k) == *(t + k))
				count++;
		}

		if (count == k)
		{
			z = true;
			break;
		}
	}
	return z;
}

bool  Semantics(LT::LexTable &Lextable, In::IN &in, IT::IdTable &idtable, Log::LOG &log)
{
	bool stlib = false;
	bool mainFlag = false;
	bool errorParam = false;
	bool errorid = false;
	bool formatError = false;
	bool exitParamError = false;
	int params[10];
	ListFunc *Functions = new ListFunc;
	ListId *Ids = new ListId;
	Ids->Head = NULL;
	Functions->Head = NULL;
	unsigned char* name;
	int counter = 0;
	int type = 0;


	for (int i = 0; i < idtable.size; i++)								//проверка на переменную неопознанного типа??????????????
	{
		if (idtable.table[i].iddatatype != IT::V && idtable.table[i].iddatatype != IT::F
			&& idtable.table[i].iddatatype != IT::P && idtable.table[i].iddatatype != IT::L
			&& idtable.table[i].iddatatype != IT::OP && idtable.table[i].iddatatype != IT::LIB)
			errorid = true;
		if (stlib && idtable.table[i].idtype == IT::LIB)
			throw ERROR_THROW(616);
		if (idtable.table[i].idtype == IT::LIB)
			stlib = true;
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка на количество main'ов
	{
		if (Lextable.table[i].lexema == LEX_LIB)
			stlib = true;
		if (Lextable.table[i].lexema == LEX_MAIN)
			counter++;
	}
	if (counter != 1)
		mainFlag = true;
	counter = 0;

	for (int i = 0; i < Lextable.size; i++)					//проверка типов данных при присваивании
	{
		if (formatError)
			break;
		if (Lextable.table[i].lexema == LEX_EQUAL)
		{
			type = idtable.table[Lextable.table[i - 1].idxTI].iddatatype;
			i++;
			while (Lextable.table[i].lexema != LEX_SEMICOLON)
			{
				if (Lextable.table[i].lexema == LEX_ID || Lextable.table[i].lexema == LEX_LITERAL)
				{
					if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
					{
						if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
						{
							formatError = true;
							break;
						}
						while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
							i++;
						continue;
					}
					if (idtable.table[Lextable.table[i].idxTI].iddatatype != type)
					{
						formatError = true;
						break;
					}
				}
				i++;
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)
	{
		if (Lextable.table[i].lexema == LEX_BLOCK)						//ищет все прототипы функций
		{
			int index = Lextable.table[i + 1].idxTI;
			name = idtable.table[index].id;
			int k = i + 2;
			counter = 0;
			while (Lextable.table[k].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[k].lexema != LEX_COMMA && Lextable.table[k].lexema != LEX_LEFTTHESIS)
				{
					index = Lextable.table[k + 1].idxTI;
					params[counter] = idtable.table[index].iddatatype;
					counter++;
					k++;
				}
				k++;
			}
			semFunction* f = CreateFunc(counter, params, name);					//создаем структуру с описанием прототипа функции
			Add(f, Functions);
		}
		//проверка делени€ на 0 + проверка на операции с Ќ≈ целочисл. переменными 
		if (Lextable.table[i].lexema == LEX_OPERATOR)
		{
			if (!strcmp((char*)idtable.table[Lextable.table[i].idxTI].id, "/"))
			{
				if (Lextable.table[i + 1].lexema != LEX_LEFTTHESIS)
				{
					if (idtable.table[Lextable.table[i + 1].idxTI].value.vint == 0)
					{
						throw ERROR_THROW(613);
					}
				}
			}

			for (int j = i; Lextable.table[j].lexema != LEX_SEMICOLON; j++)
			{
				if (Lextable.table[j].idxTI < idtable.size)
				{
					if (idtable.table[Lextable.table[j].idxTI].iddatatype != IT::INT && idtable.table[Lextable.table[j].idxTI].idtype != IT::P && idtable.table[Lextable.table[j].idxTI].idtype != IT::OP)
						throw ERROR_THROW(614);
				}
			}
		}
	}

	int level;
	int typeInit;
	int typeExit;
	for (int i = 0; i < Lextable.size; i++)					//проверка возвращаемого значени€ функции
	{
		if (Lextable.table[i].lexema == LEX_BLOCK && !(stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))
		{																		//проверка функций Ќ≈ стандартной библиотеки
			typeInit = idtable.table[Lextable.table[i + 1].idxTI].iddatatype;	//тип при инициализации
			int k = i;
			level = 0;
			while (Lextable.table[k].lexema != LEX_BACK /*&& level == 0*/)				//тут строки закоменчены, потому что не
			{																				//получилось сделать проверку дл€ 
																							/*if (Lextable.table[k].lexema == LEX_LEFTBRACE)								//вложенных функций
																							level++;
																							if (Lextable.table[k].lexema == LEX_RIGHTTHESIS)
																							level--;*/
				k++;
			}

			typeExit = idtable.table[Lextable.table[k + 1].idxTI].iddatatype;	//тип возвращаемого значени€

			if (typeInit != typeExit)
			{
				exitParamError = true;
				break;
			}
		}
		else
		{
			if (Lextable.table[i].lexema == LEX_BLOCK && (stlib && checkLib(idtable.table[Lextable.table[i + 1].idxTI].id)))	//если полключена библиотека и 
			{																	//провер€ем стандартную функцию
				unsigned char wordlen[] = "wordlen";
				unsigned char subword[] = "subword";
				unsigned char getnod[] = "getnod";
				unsigned char getnok[] = "getnok";
				unsigned char* libFunctions[] = { wordlen, subword, getnod, getnok, NULL };
				bool find = false;
				int id = -1;
				for (int i3 = 0; libFunctions[i3] != NULL; i3++)
				{
					int count = 0;
					int k;
					for (k = 0; *((libFunctions[i3]) + k) != '\0' && *(idtable.table[Lextable.table[i + 1].idxTI].id + k) != '\0'; k++)
					{
						if (*((libFunctions[i3]) + k) == *(idtable.table[Lextable.table[i + 1].idxTI].id + k))
							count++;
					}
					if (count == k)
					{
						find = true;
						id = i3;
						break;
					}
				}
				if (!find)
				{
					throw ERROR_THROW(610);
				}
				else
				{
					switch (id)
					{
					case 0:								//wordlen
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							exitParamError = true;
						break;

					case 1:								//subword
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::LOGIC)
							exitParamError = true;
						break;
					case 2:								//getnod
					case 3:								//getnok
						if (idtable.table[Lextable.table[i + 1].idxTI].iddatatype != IT::INT)
							exitParamError = true;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < Lextable.size; i++)					//проверка использовани€ необъ€вленной переменной
	{
		if (Lextable.table[i].lexema == LEX_CREATE)									//заполн€ем лист объ€вленными идентификаторами
		{
			if (Lextable.table[i + 2].lexema != LEX_BLOCK)
			{
				Add(idtable.table[Lextable.table[i + 2].idxTI].id, Ids);
			}
		}

		if (Lextable.table[i].lexema == LEX_BLOCK)
		{
			i++;			//сдвиг на название функции
			i++;			//сдвиг на (
			while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
			{
				if (Lextable.table[i].lexema == LEX_ID)
					Add(idtable.table[Lextable.table[i].idxTI].id, Ids);
				i++;
			}
		}

		int count;
		if (Lextable.table[i].idxTI < idtable.size)
		{
			if (idtable.table[Lextable.table[i].idxTI].idtype == IT::F)
			{
				if (Lextable.table[i - 1].lexema != LEX_BLOCK)		//если это не объ€вление функции
				{
					name = idtable.table[Lextable.table[i].idxTI].id;
					i++;
					count = 0;
					//std::cout << "\n\n" << name << "\n";
					while (Lextable.table[i].lexema != LEX_RIGHTTHESIS)
					{
						if (Lextable.table[i].idxTI < idtable.size)		//если идентификатор
						{


							params[count] = idtable.table[Lextable.table[i].idxTI].iddatatype;
							count++;
							//std::cout << params[count] << "   _____   ";	//тут всЄ ок

						}
						i++;
					}
					semFunction* fcheck = CreateFunc(count, params, name);
					if (!checkFunc(Functions, fcheck))
					{
						throw ERROR_THROW(612);
					}
				}
			}
		}
	}

	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype != IT::OP && idtable.table[i].idtype != IT::LIB && idtable.table[i].idtype != IT::F && idtable.table[i].idtype != IT::L)
		{
			if (!(checkId(Ids, idtable.table[i].id)))
			{
				throw ERROR_THROW(611);
			}
		}
	}


	ClearList(Ids);
	ClearList(Functions);

	if (mainFlag)								//проверка на кол-во main'ов, +
		throw ERROR_THROW(608);
	if (errorParam)								//проверка параметров, кол-ва и типов, - (!)
		throw ERROR_THROW(606);
	if (exitParamError)							//проверка типа выходного параметра,  (дописать дл€ станд. функций)
		throw ERROR_THROW(609);
	if (errorid)								//наличие переменных неопределенного типа в программе, +	(такое получаетс€ если какие-то
		throw ERROR_THROW(605);															// ошибки в объ€влении функции)(уже забыла когда)
	if (formatError)							//проверка на совпадение типов данных при присваивании переменной
		throw ERROR_THROW(607);
	return true;
}