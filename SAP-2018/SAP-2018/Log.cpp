#include "pch.h"
#include "Headers.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG lg;
		std::ofstream *file = new std::ofstream(logfile);
		if (file->fail())
			throw ERROR_THROW(112);
		lg.stream = file;
		wcscpy_s(lg.logfile, logfile);
		return lg;
	}

	void WriteLine(LOG log, char* c, ...)
	{
		char **ptr = &c;
		int len = 0;
		for (; *ptr != ""; ptr++)
			len += strlen(*ptr);
		ptr = &c;
		char *str = new char[len + 1];
		str[0] = 0;
		for (; *ptr != ""; ptr++)
			strcat(str, *ptr);
		*log.stream << str << std::endl;
		delete str;
	}

	void WriteLine(LOG log, wchar_t* c, ...)
	{
		wchar_t **ptr = &c;
		int len = 0;
		for (; *ptr != L""; ptr++)
			len += wcslen(*ptr);
		ptr = &c;
		char *str = new char[len + 1];
		wchar_t *wstr = new wchar_t[len + 1];
		wstr[0] = 0;
		for (; *ptr != L""; ptr++)
			wcscat(wstr, *ptr);
		wcstombs(str, wstr, len + 1);
		*log.stream << str << std::endl;
		delete str;
		delete wstr;
	}

	void WriteLog(LOG log)
	{
		time_t rawtime;
		struct tm *timeinfo;
		char str[60];			// ������, � ������� ����� ��������� ������� �����                      
		time(&rawtime);			// ������� ���� � ��������
		timeinfo = localtime(&rawtime);		 // ������� ��������� �����, �������������� � ���������
		*log.stream << std::endl;
		strftime(str, 60, "---- �������� ------ ����:  %d.%m.%Y %X -----------", timeinfo);		// ����������� ������ �������
		*log.stream << str << std::endl;
	}

	void WriteParm(LOG log, Parm::PARM parm)
	{
		int lenLog = wcslen(parm.log), lenOut = wcslen(parm.out), lenIn = wcslen(parm.in);
		char *bufLog = new char[lenLog + 1];
		char *bufOut = new char[lenOut + 1];
		char *bufIn = new char[lenIn + 1];
		wcstombs(bufLog, parm.log, lenLog + 1);
		wcstombs(bufOut, parm.out, lenOut + 1);
		wcstombs(bufIn, parm.in, lenIn + 1);
		*log.stream << "---- ��������� ------" << std::endl;
		*log.stream << "-log: " << bufLog << std::endl;
		*log.stream << "-out: " << bufOut << std::endl;
		*log.stream << "-in: " << bufIn << std::endl;
	}

	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "---- �������� ������ ------" << std::endl;
		*log.stream << "���������� ��������: " << in.size << std::endl;
		*log.stream << "���������������	   : " << in.ignor << std::endl;
		*log.stream << "���������� �����   : " << in.lines << std::endl;
	}

	void WriteError(LOG log, Error::ERROR error)
	{
		if (!log.stream)
			std::cout << "������ " << error.id << ": " << error.message << std::endl;
		else if (error.inext.col >= 0 && error.inext.line >= 0)
			*log.stream << "������ " << error.id << ": " << error.message << ", ������ " << error.inext.line << ", ������� " << error.inext.col << std::endl;
		else
			*log.stream << "������ " << error.id << ": " << error.message << std::endl;
	}

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
}