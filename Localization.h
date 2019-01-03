#pragma once

const std::wstring WindowCaptionString(L"Управление стандартами");

const std::wstring ErrorDrawingString(L"Неудача при попытке рисования.");
const std::wstring ErrorReadingFileString(L"Неудача при попытке чтения файла.");
const std::wstring ErrorDeletingFontString(L"Неудача при попытке удаления шрифта.");
const std::wstring ErrorDeletingBrushString(L"Неудача при попытке удаления кисти.");
const std::wstring ErrorCopyingContextString(L"Неудача при попытке копировании контекста рисования из памяти.");
const std::wstring ErrorDeletingBitMapString(L"Неудача при попытке удаления холста.");
const std::wstring ErrorDeletingMemoryContextString(L"Неудача при попытке удаления контекста рисования в памяти.");
const std::wstring ErrorCreatingMemoryContextString(L"Неудача при попытке получения контекста рисования в памяти.");
const std::wstring ErrorDeletingContextString(L"Неудача при попытке удаления контекста рисования.");
const std::wstring ErrorCreatingContextString(L"Неудача при попытке получения контекста рисования.");

const std::wstring LondonTimeString(L"Лондонское время:");
const std::wstring LocalTimeString(L"Местное время:");
const std::wstring FrameRateString(L"Частота кадров ");
const std::wstring CurrentFrameString(L", текущий кадр ");
const std::wstring PointString(L".");

const std::wstring CatalogString(L"Каталог");
const std::wstring CatalogBaseString = CatalogString + std::wstring(L"\\");

const std::wstring DayOfWeekListMondayString(L"Понедельник");
const std::wstring DayOfWeekListTuesdayString(L"Вторник");
const std::wstring DayOfWeekListWednesdayString(L"Среда");
const std::wstring DayOfWeekListThursdayString(L"Четверг");
const std::wstring DayOfWeekListFridayString(L"Пятница");
const std::wstring DayOfWeekListSaturdayString(L"Суббота");
const std::wstring DayOfWeekListSundayString(L"Воскресение");

const std::wstring MonthListJanuaryString(L"Январь");
const std::wstring MonthListFebruaryString(L"Февраль");
const std::wstring MonthListMarchString(L"Март");
const std::wstring MonthListAprilString(L"Апрель");
const std::wstring MonthListMayString(L"Май");
const std::wstring MonthListJuneString(L"Июнь");
const std::wstring MonthListJulyString(L"Июль");
const std::wstring MonthListAugustString(L"Август");
const std::wstring MonthListSeptemberString(L"Сентябрь");
const std::wstring MonthListOctoberString(L"Октябрь");
const std::wstring MonthListNovemberString(L"Ноябрь");
const std::wstring MonthListDecemberString(L"Декабрь");

const std::wstring NominativeDayOfWeekDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\День недели");
const std::wstring NominativeMonthsDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\Месяцы");
const std::wstring NominativeMonthDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Именительный\\Месяц");

const std::wstring GenitiveYearDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Год");
const std::wstring GenitiveMonthsDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Месяцы");
const std::wstring GenitiveMonthDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\Месяц");
const std::wstring GenitiveDayDirectoryString = CatalogBaseString + std::wstring(L"Стандарты\\Время\\Родительный\\День");

const BYTE CHARACTERS_SET = RUSSIAN_CHARSET;

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