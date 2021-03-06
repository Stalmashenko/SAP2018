#include "pch.h"
#include "Headers.h"
#include <locale>
#include <cwchar>
#include "Div.h"
#include "LexAnaliz.h"
#include "PolishNotation.h"
#include "Semantics.h"
#include "Translation.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		In::IN in = In::getin(parm.in);
		Lex::LEX lex = Lex::lexAnaliz(log, in);
		Semantics(lex.lextable, in, lex.idtable, log);
		*(log.stream) << "\tПромежуточный код\n";
		LT::showTable(lex.lextable, log);
		IT::showTable(lex.idtable);
		*(log.stream) << "\n\tТрассировка\n";
		MFST_TRACE_START(log)
		unsigned int start_time = clock();
		MFST::Mfst mfst(lex, GRB::getGreibach());
		bool res = mfst.start(log);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		*(log.stream) << "Время трассировки: "<<search_time<<" мс." << endl;
		mfst.savededucation();
		*(log.stream) << "\tПравила, по которым строится дерево разбора \n";
		mfst.printrules(log);
		bool rc = startPolishNotation(lex);
		if (res)
			cout << "\nСинтаксический анализ выполнен успешно." << endl;
		else
			cout << "\nСинтаксический анализ не завершен." << endl;
		if (rc)
			cout << "Польская нотация создана." << endl;
		else
			cout << "Польская нотация не создана." << endl;

		Log::WriteLog(log);
		Log::WriteIn(log, in);
		Log::WriteParm(log, parm);		
		log = Log::getlog(parm.out);
		CG::StartGeneration(lex.lextable, lex.idtable, log);
		Log::Close(log);

	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};
	system("notepad in.txt.log");
	system("pause");
	return 0;
};
