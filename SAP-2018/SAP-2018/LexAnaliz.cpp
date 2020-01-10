#include "pch.h"
#include "Headers.h"
#include "Div.h"
#include "Graphs.h"
#include "LexAnaliz.h"

#define DIV '|'
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define STAR '*'
#define DIRSLASH '/'
#define EQUAL '='

using namespace std;

namespace Lex
{
	LEX lexAnaliz(Log::LOG log, In::IN in)
	{
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		clearSpace(in.text, in.size);

		unsigned char** word = new unsigned char*[MAX_WORD];
		for (int i = 0; i < MAX_WORD; i++)
			word[i] = new unsigned char[SIZE_WORD] {NULL};

		do {
			word = divideText(in.text, in.size);
		} while (word == NULL);

		int indxLex = 0;		// ������ �������
		int indexID = 0;		// ������ �����.
		int clit = 1;			// ������� ���������
		int line = 1;			// ����� ������
		int position = 0;			// ����� ������� � �������� �����

		unsigned char emptystr[] = "";	// ������ ������
		unsigned char* prefix = new unsigned char[10]{ "" };	// ������� �������
		unsigned char* bufprefix = new unsigned char[10]{ "" };	// ����� ��� ��������
		unsigned char* oldprefix = new unsigned char[10]{ "" };	// ���������� �������
		unsigned char* L = new unsigned char[2]{ "L" };
		unsigned char* bufL = new unsigned char[TI_STR_MAXSIZE];
		unsigned char* nameLiteral = new unsigned char[10]{ "" };
		char* charclit = new char[10]{ "" };		// ��� ��������� ������ ��������

		bool findFunc = false;
		bool findParm = false;
		bool findLib = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indxLex++)
		{
			if (word[i][0] == (unsigned char)'<')
			{
				i++;
				while (word[i][0] != (unsigned char)'>')
					i++;
				continue;
			}

			bool findSameID = false;

			FST::FST fstDecl(word[i], FST_CREATE);
			if (FST::execute(fstDecl))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_CREATE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstTypeInteger(word[i], FST_NUM);
			if (FST::execute(fstTypeInteger))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_NUMB, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::INT;
				continue;
			}
			FST::FST fstTypeIs(word[i], FST_LOGIC);
			if (FST::execute(fstTypeIs))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LOGIC, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::LOGIC;
				continue;
			}
			FST::FST fstTypeString(word[i], FST_WORD);
			if (FST::execute(fstTypeString))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_WORD, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);

				entryIT.iddatatype = IT::STR;
				_mbscpy(entryIT.value.vstr.str, emptystr);
				continue;
			}
			FST::FST fstFunction(word[i], FST_BLOCK);
			if (FST::execute(fstFunction))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BLOCK, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::F;
				findFunc = true;
				continue;
			}
			FST::FST fstLibrary(word[i], FST_LIB);
			if (FST::execute(fstLibrary))
			{
				findLib = true;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LIB, indexID++, line);
				entryIT.iddatatype = IT::INT;
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::LIB;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstExit(word[i], FST_BACK);
			if (FST::execute(fstExit))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BACK, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstOut(word[i], FST_OUT);
			if (FST::execute(fstOut))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OUT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstMain(word[i], FST_MAIN);
			if (FST::execute(fstMain))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_MAIN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				_mbscpy(oldprefix, prefix);
				_mbscpy(prefix, word[i]);
				_mbscpy(entryIT.visibility, emptystr);
				continue;
			}
			FST::FST fstLiteralIs(word[i], FST_LOGICLIT);					//
			if (FST::execute(fstLiteralIs))
			{
				bool value = false;
				if (word[i][0] == 't')
					value = true;
				// ����� ������ ��
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vlogic == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::LOGIC)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// ���� ��� ������ ����� ��
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::LOGIC;
				entryIT.value.vlogic = value;
				entryIT.idxfirstLE = indxLex;						// ������ ������������ ����� ��������
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// ����������� �������� �������� � ������(charclit)
				_mbscpy(bufL, L);									// �������� � ����� "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// ��������� ��� ��� ��������
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstIdentif(word[i], FST_ID);
			if (FST::execute(fstIdentif))
			{
				// ����� ������ ��
				if (findFunc)	// ���� �������
				{
					int idx = IT::IsId(idtable, word[i]);	// ���� ��� ��������
					if (idx != TI_NULLIDX)		// ���� ����� ������������� ��� ����
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
				}
				else
				{
					int idx = IT::IsId(idtable, word[i]);	// ���� ��� ��������, � ����� � ���������
					if (idx != TI_NULLIDX)		// ���� ����� ������������� ��� ����
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						findFunc = false;
						continue;
					}
					_mbscpy(bufprefix, prefix);
					word[i] = _mbscat(bufprefix, word[i]);
					idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX)		// ���� ����� ������������� ��� ����
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						continue;
					}
				}
				LT::Entry entryLT = writeEntry(entryLT, LEX_ID, indexID++, line);
				LT::Add(lextable, entryLT);
				if (findParm) {		// ���� ��������
					entryIT.idtype = IT::P;
					_mbscpy(entryIT.visibility, prefix);
				}
				else if (!findFunc) {	// ���� ����������
					entryIT.idtype = IT::V;
					_mbscpy(entryIT.visibility, prefix);
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_NUMB_DEFAULT;
					if (entryIT.iddatatype == IT::LOGIC)
						entryIT.value.vlogic = TI_LOGIC_DEFAULT;
					if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_WORD_DEFAULT, sizeof(char));
					}
				}
				else {		// ���� �������
					_mbscpy(oldprefix, prefix);
					_mbscpy(prefix, word[i]);
					_mbscpy(entryIT.visibility, emptystr);
				}

				entryIT.idxfirstLE = indxLex;
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				findFunc = false;
				continue;
			}
			FST::FST fstLiteralInt(word[i], FST_INTLIT);
			if (FST::execute(fstLiteralInt))
			{
				int value = atoi((char*)word[i]);	// �������� ��������
													// ����� ������ ��
				for (int k = 0; k < idtable.size; k++)
				{
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT)
					{
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}
				if (findSameID)	// ���� ��� ������ ����� ��
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxfirstLE = indxLex;
				// ������������ ����� ��������
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// ����������� �������� �������� � ������(charclit)
				_mbscpy(bufL, L);	// �������� � ����� "L"
				word[i] = _mbscat(bufL, (unsigned char*)charclit);	// ��������� ��� ��� ��������
				_mbscpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstLiteralString(word[i], FST_STRLIT);
			if (FST::execute(fstLiteralString))
			{
				// ������ ��������
				int length = _mbslen(word[i]);
				for (int k = 0; k < length; k++)	// ���������� �������, ������� �������
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;
				// ����� ������ ��
				for (int k = 0; k < idtable.size; k++)
				{
					if (!(_mbscmp(idtable.table[k].value.vstr.str, word[i])))
					{
						findSameID = true;
						LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID)	// ���� ��� ������ ����� ��
					continue;
				LT::Entry entryLT = writeEntry(entryLT, LEX_LITERAL, indexID++, line);
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.value.vstr.str, word[i]);	// ������ �������� ���������� ��������
				entryIT.value.vstr.len = length - 2;		// ������ ����� ���������� ��������
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxfirstLE = indxLex;
				// ������������ ����� ��������
				_itoa_s(clit++, charclit, sizeof(char) * 10, 10);	// ����������� �������� �������� � ������(charclit)
				_mbscpy(bufL, L);	// �������� � ����� "L"
				nameLiteral = _mbscat(bufL, (unsigned char*)charclit);	// ��������� ��� ��� �������� (L + charclit)
				_mbscpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstOperator(word[i], FST_OPERATOR);
			if (FST::execute(fstOperator))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_OPERATOR, indexID++, line);
				switch (word[i][0])
				{
				case PLUS: case MINUS:
					entryLT.priority = 2;
					break;
				case DIRSLASH: case STAR:
					entryLT.priority = 3;
					break;
				}
				LT::Add(lextable, entryLT);
				_mbscpy(entryIT.id, word[i]);
				entryIT.idxfirstLE = indxLex;
				entryIT.idtype = IT::OP;
				IT::Add(idtable, entryIT);
				continue;
			}
			FST::FST fstTilda(word[i], FST_SEMICOLON);
			if (FST::execute(fstTilda))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstComma(word[i], FST_COMMA);
			if (FST::execute(fstComma))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftBrace(word[i], FST_LEFTBRACE);
			if (FST::execute(fstLeftBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstRightBrace(word[i], FST_BRACELET);
			if (FST::execute(fstRightBrace))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstLeftThesis(word[i], FST_LEFTTHESIS);
			if (FST::execute(fstLeftThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				if (idtable.table[indexID - 1].idtype == IT::F)
					findParm = true;
				continue;
			}
			FST::FST fstRightThesis(word[i], FST_RIGHTTHESIS);
			if (FST::execute(fstRightThesis))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				if (findParm && word[i + 1][0] != LEX_LEFTBRACE && word[i + 2][0] != LEX_LEFTBRACE)		// ���� ����� ������� ��� {
					_mbscpy(prefix, oldprefix);		// ���������� ���������� ���. ���������
				findParm = false;
				LT::Add(lextable, entryLT);
				continue;
			}
			FST::FST fstEqual(word[i], FST_EQUAL);
			if (FST::execute(fstEqual))
			{
				LT::Entry entryLT = writeEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				continue;
			}
			position += _mbslen(word[i]);
			if (word[i][0] == DIV) {
				line++;
				position = 0;
				indxLex--;
				continue;
			}
			throw ERROR_THROW_IN(162, line, position);
		}
		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}
}