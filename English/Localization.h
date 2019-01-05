#pragma once

#include "Manager.h"

Constant std::wstring WindowCaptionString(L"Standards management");

Constant std::wstring ErrorDrawingString(L"Failure when trying to draw.");
Constant std::wstring ErrorReadingFileString(L"Failure when trying to read a file.");
Constant std::wstring ErrorDeletingFontString(L"Failure when trying to delete a font.");
Constant std::wstring ErrorDeletingBrushString(L"Failure when trying to remove a brush.");
Constant std::wstring ErrorCopyingContextString(L"Failure when trying to copy a drawing context from memory.");
Constant std::wstring ErrorDeletingBitMapString(L"Failure when trying to delete a canvas.");
Constant std::wstring ErrorDeletingMemoryContextString(L"Failure when trying to delete a drawing context in memory.");
Constant std::wstring ErrorCreatingMemoryContextString(L"Failure when trying to get a drawing context in memory.");
Constant std::wstring ErrorDeletingContextString(L"Failure when attempting to delete drawing context.");
Constant std::wstring ErrorCreatingContextString(L"Failure when trying to get drawing context.");

Constant std::wstring LondonTimeString(L"London time:");
Constant std::wstring LocalTimeString(L"Local time время:");
Constant std::wstring FrameRateString(L"Frame rate ");
Constant std::wstring CurrentFrameString(L", current frame ");
Constant std::wstring PointString(L".");

Constant std::wstring CatalogString(L"Catalog");
Constant std::wstring CatalogBaseString = CatalogString + std::wstring(L"\\");

Constant std::wstring DayOfWeekListMondayString(L"Monday");
Constant std::wstring DayOfWeekListTuesdayString(L"Tuesday");
Constant std::wstring DayOfWeekListWednesdayString(L"Wednesday");
Constant std::wstring DayOfWeekListThursdayString(L"Thursday");
Constant std::wstring DayOfWeekListFridayString(L"Friday");
Constant std::wstring DayOfWeekListSaturdayString(L"Saturday");
Constant std::wstring DayOfWeekListSundayString(L"Sunday");

Constant std::wstring MonthListJanuaryString(L"January");
Constant std::wstring MonthListFebruaryString(L"February");
Constant std::wstring MonthListMarchString(L"March");
Constant std::wstring MonthListAprilString(L"May");
Constant std::wstring MonthListMayString(L"May");
Constant std::wstring MonthListJuneString(L"June");
Constant std::wstring MonthListJulyString(L"July");
Constant std::wstring MonthListAugustString(L"August");
Constant std::wstring MonthListSeptemberString(L"September");
Constant std::wstring MonthListOctoberString(L"October");
Constant std::wstring MonthListNovemberString(L"November");
Constant std::wstring MonthListDecemberString(L"December");

Constant std::wstring NominativeDayOfWeekDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Day of Week");
Constant std::wstring NominativeMonthsDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Months");
Constant std::wstring NominativeMonthDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Month");

Constant std::wstring GenitiveYearDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Year");
Constant std::wstring GenitiveMonthsDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Months");
Constant std::wstring GenitiveMonthDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Month");
Constant std::wstring GenitiveDayDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Day");

Constant BYTE CHARACTERS_SET = ANSI_CHARSET;

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