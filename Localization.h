#pragma once

#include "Manager.h"

Constant std::wstring WindowCaptionString(L"Управление стандартами");

Constant std::wstring ErrorDrawingString(L"Неудача при попытке рисования.");
Constant std::wstring ErrorReadingFileString(L"Неудача при попытке чтения файла.");
Constant std::wstring ErrorDeletingFontString(L"Неудача при попытке удаления шрифта.");
Constant std::wstring ErrorDeletingBrushString(L"Неудача при попытке удаления кисти.");
Constant std::wstring ErrorCopyingContextString(L"Неудача при попытке копировании контекста рисования из памяти.");
Constant std::wstring ErrorDeletingBitMapString(L"Неудача при попытке удаления холста.");
Constant std::wstring ErrorDeletingMemoryContextString(L"Неудача при попытке удаления контекста рисования в памяти.");
Constant std::wstring ErrorCreatingMemoryContextString(L"Неудача при попытке получения контекста рисования в памяти.");
Constant std::wstring ErrorDeletingContextString(L"Неудача при попытке удаления контекста рисования.");
Constant std::wstring ErrorCreatingContextString(L"Неудача при попытке получения контекста рисования.");

Constant std::wstring LondonTimeString(L"Лондонское время:");
Constant std::wstring LocalTimeString(L"Местное время:");
Constant std::wstring FrameRateString(L"Частота кадров ");
Constant std::wstring CurrentFrameString(L", текущий кадр ");
Constant std::wstring PointString(L".");

Constant std::wstring CatalogString(L"Каталог");
Constant std::wstring CatalogBaseString = CatalogString + std::wstring(L"\\");

Constant std::wstring DayOfWeekListMondayString(L"Понедельник");
Constant std::wstring DayOfWeekListTuesdayString(L"Вторник");
Constant std::wstring DayOfWeekListWednesdayString(L"Среда");
Constant std::wstring DayOfWeekListThursdayString(L"Четверг");
Constant std::wstring DayOfWeekListFridayString(L"Пятница");
Constant std::wstring DayOfWeekListSaturdayString(L"Суббота");
Constant std::wstring DayOfWeekListSundayString(L"Воскресение");

Constant std::wstring MonthListJanuaryString(L"Январь");
Constant std::wstring MonthListFebruaryString(L"Февраль");
Constant std::wstring MonthListMarchString(L"Март");
Constant std::wstring MonthListAprilString(L"Апрель");
Constant std::wstring MonthListMayString(L"Май");
Constant std::wstring MonthListJuneString(L"Июнь");
Constant std::wstring MonthListJulyString(L"Июль");
Constant std::wstring MonthListAugustString(L"Август");
Constant std::wstring MonthListSeptemberString(L"Сентябрь");
Constant std::wstring MonthListOctoberString(L"Октябрь");
Constant std::wstring MonthListNovemberString(L"Ноябрь");
Constant std::wstring MonthListDecemberString(L"Декабрь");

Constant std::wstring NominativeDayOfWeekDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\День недели");
Constant std::wstring NominativeMonthsDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\Месяцы");
Constant std::wstring NominativeMonthDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\Месяц");

Constant std::wstring GenitiveYearDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Год");
Constant std::wstring GenitiveMonthsDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Месяцы");
Constant std::wstring GenitiveMonthDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Месяц");
Constant std::wstring GenitiveDayDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\День");

Constant BYTE CHARACTERS_SET = RUSSIAN_CHARSET;

/*/
Manage font: set 'BYTE CHARACTERS_SET' one of

ANSI_CHARSET
BALTIC_CHARSET
CHINESEBIG5_CHARSET
DEFAULT_CHARSET
EASTEUROPE_CHARSET
GB2312_CHARSET
GREEK_CHARSET
HANGUL_CHARSET
MAC_CHARSET
OEM_CHARSET
RUSSIAN_CHARSET
SHIFTJIS_CHARSET
SYMBOL_CHARSET
TURKISH_CHARSET
VIETNAMESE_CHARSET
/*/