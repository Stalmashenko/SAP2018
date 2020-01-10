#include "pch.h"
#include "Headers.h"
#include <cstring>
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),		// код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "[SYS] Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "[PARM] Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "[PARM] Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "[O/R] Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "[O/R] Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "[O/R] Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120, "[LEX] Превышен максимальный размер таблицы лексем"),
		ERROR_ENTRY(121, "[LEX] Таблица лексем переполнена"),
		ERROR_ENTRY_NODEF(122), ERROR_ENTRY_NODEF(123), ERROR_ENTRY_NODEF(124),
		ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128),
		ERROR_ENTRY_NODEF(129),  ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY(160, "[LEX] Превышен максимальный размер таблицы идентификаторов"),
		ERROR_ENTRY(161, "[LEX] Таблица идентификаторов переполнена"),
		ERROR_ENTRY(162, "[LEX] Ошибка лексического анализа"), ERROR_ENTRY(163, ""), ERROR_ENTRY(164, ""),
		ERROR_ENTRY(165, ""), ERROR_ENTRY(166, ""), ERROR_ENTRY(167, ""), ERROR_ENTRY(168, ""), ERROR_ENTRY(169, ""),
		ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "[SYN] Неверная структура программы"),
		ERROR_ENTRY(601, "[SYN] Неверная структура оператора"),
		ERROR_ENTRY(602, "[SYN] Неверная структура выражений"),
		ERROR_ENTRY(603, "[SYN] Неверная структура параметров функции"),
		ERROR_ENTRY(604, "[SYN] Неверная структура параметров вызываемой функции"),
		ERROR_ENTRY(605, "[SEM] Переменная неизвестного типа"),
		ERROR_ENTRY(606, "[SEM] Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(607, "[SEM] Ошибка формата: присвоение переменной другого типа данных"), 
		ERROR_ENTRY(608, "[SEM] Недопустимое количество лексем main"),
		ERROR_ENTRY(609, "[SEM] Тип возвращаемого параметра функции не совпадает с параметром при объявлении"),
		ERROR_ENTRY(610, "[SEM] Ошибка в функции стандартной библиотеки"),
		ERROR_ENTRY(611, "[SEM] Использование необъявленной переменной"),
		ERROR_ENTRY(612, "[SEM] Несовпадение типов параметров вызываемой функции"),
		ERROR_ENTRY(613, "[SEM] Деление на ноль недопустимо"),
		ERROR_ENTRY(614, "[SEM] Операции с переменными недопустимого типа"),
		ERROR_ENTRY(615, "[SEM] Объявление функции с одинаковым именем "),
		ERROR_ENTRY(616, "[SEM] Повторное объявление стандартной библиотеки "),//
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800),
		ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		er.id = id;
		strcpy_s(er.message, errors[id].message);
		return er;
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		ERROR er;
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		er.id = id;
		er.inext.line = line;
		er.inext.col = col;
		strcpy_s(er.message, errors[id].message);
		return er;
	}
};