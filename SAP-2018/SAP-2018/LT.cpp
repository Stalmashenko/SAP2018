#include "pch.h"
#include "Headers.h"

namespace LT
{
	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE) throw ERROR_THROW(120);
		LexTable Table;
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size > lextable.maxsize) throw ERROR_THROW(121);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}

	Entry writeEntry(Entry &entry, unsigned char lexema, int indx, int line)
	{
		entry.lexema = lexema;
		entry.idxTI = indx;
		entry.sn = line;
		return entry;
	}

	void showTable(LexTable lextable, Log::LOG &log)		// ����� ������� ������
	{
		int number = 1;
		*log.stream << "01 ";
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].sn != number && lextable.table[i].sn != -1)
			{
				while (lextable.table[i].sn - number > 1)	// ���� ������ �� ������ �����
					number++;
				if (number < 9)
					*log.stream << std::endl << '0' << lextable.table[i].sn << std::ends;
				else
					*log.stream << std::endl << lextable.table[i].sn << std::ends;
				number++;
			}
			*log.stream << lextable.table[i].lexema;
			if (lextable.table[i].lexema == LEX_ID || lextable.table[i].lexema == LEX_OPERATOR || lextable.table[i].lexema == LEX_LITERAL)
				*log.stream << "[" << lextable.table[i].idxTI << "]";
		}
	}
}