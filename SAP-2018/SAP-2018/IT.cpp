#include "pch.h"
#include <iomanip>
#include "Headers.h"

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE) throw ERROR_THROW(160);
		IdTable Table;
		Table.maxsize = size;
		Table.size = 0;
		Table.table = new Entry[size];
		return Table;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size > idtable.maxsize) throw ERROR_THROW(161);
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, unsigned char id[ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (_mbscmp(idtable.table[i].id, id) == 0) return i;
		}

		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}

	void showTable(IdTable& idtable)
	{
		int i, numberOP = 0;
		std::cout << std::setfill('�') << std::setw(87) << '-' << std::endl;
		std::cout << "   �" << " | " << "�������������" << " | " << "��� ������" << " | " << "��� ��������������" << " | " << "������ � ��" << " | " << "��������    " << std::endl;
		std::cout << std::setw(87) << '-' << std::endl;
		for (i = 0; i < idtable.size; i++)
		{
			std::cout << std::setfill('0') << std::setw(4) << std::right << i << " | ";
			std::cout << std::setfill(' ') << std::setw(13) << std::left << idtable.table[i].id << " | ";
			switch (idtable.table[i].iddatatype)
			{
			case LOGIC: std::cout << std::setw(10) << std::left;
				if ((idtable.table[i].idtype == OP) || (idtable.table[i].idtype == LIB)) std::cout << "-" << " | ";
				else std::cout << "logic" << " | "; break;
			case INT: std::cout << std::setw(10) << std::left;
				if ((idtable.table[i].idtype == OP) || (idtable.table[i].idtype == LIB)) std::cout << "-" << " | ";
				else std::cout << "numb" << " | "; break;
			case STR: std::cout << std::setw(10) << std::left;
				if (idtable.table[i].idtype == OP || idtable.table[i].idtype == LIB) std::cout << "-" << " | ";
				else std::cout << "word" << " | "; break;
			default: std::cout << std::setw(10) << std::left << "unknown" << " | "; break;
			}
			switch (idtable.table[i].idtype)
			{
			case V: std::cout << std::setw(18) << std::left << "����������" << " | "; break;
			case F: std::cout << std::setw(18) << std::left << "�������" << " | "; break;
			case P: std::cout << std::setw(18) << std::left << "��������" << " | "; break;
			case L: std::cout << std::setw(18) << std::left << "�������" << " | "; break;
			case OP: std::cout << std::setw(18) << std::left << "��������" << " | "; break;
			case LIB: std::cout << std::setw(18) << std::left << "����������" << " | "; break;
				numberOP++;
				break;
			default: std::cout << std::setw(18) << std::left << "unknown" << " | "; break;
			}
			std::cout << std::setw(11) << std::left << idtable.table[i].idxfirstLE << " | ";
			if (idtable.table[i].iddatatype == INT && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << std::setw(18) << std::left << idtable.table[i].value.vint;
			else if (idtable.table[i].iddatatype == STR && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				std::cout << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			else
			{
				if (idtable.table[i].iddatatype == LOGIC && (idtable.table[i].idtype == V || idtable.table[i].idtype == L))
				{
					std::cout << (idtable.table[i].value.vlogic ? "true" : "false");
				}
				else
					std::cout << "-";
			}
			std::cout << std::endl;
		}
		std::cout << std::setfill('-') << std::setw(87) << '-' << std::endl;
		std::cout << "���������� ���������������: " << i - numberOP << std::endl;
		std::cout << std::setw(87) << '-' << std::endl;
	}
}