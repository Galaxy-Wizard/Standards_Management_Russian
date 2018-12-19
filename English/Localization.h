#pragma once

const std::wstring WindowCaptionString(L"Standards management");

const std::wstring ErrorDrawingString(L"Failure when trying to draw.");
const std::wstring ErrorReadingFileString(L"Failure when trying to read a file.");
const std::wstring ErrorDeletingFontString(L"Failure when trying to delete a font.");
const std::wstring ErrorDeletingBrushString(L"Failure when trying to remove a brush.");
const std::wstring ErrorCopyingContextString(L"Failure when trying to copy a drawing context from memory.");
const std::wstring ErrorDeletingBitMapString(L"Failure when trying to delete a canvas.");
const std::wstring ErrorDeletingMemoryContextString(L"Failure when trying to delete a drawing context in memory.");
const std::wstring ErrorCreatingMemoryContextString(L"Failure when trying to get a drawing context in memory.");
const std::wstring ErrorDeletingContextString(L"Failure when attempting to delete drawing context.");
const std::wstring ErrorCreatingContextString(L"Failure when trying to get drawing context.");

const std::wstring LondonTimeString(L"London time:");
const std::wstring LocalTimeString(L"Local time время:");
const std::wstring FrameRateString(L"Frame rate ");
const std::wstring CurrentFrameString(L", current frame ");
const std::wstring PointString(L".");

const std::wstring CatalogString(L"Catalog");
const std::wstring CatalogBaseString = CatalogString + std::wstring(L"\\");

const std::wstring DayOfWeekListMondayString(L"Monday");
const std::wstring DayOfWeekListTuesdayString(L"Tuesday");
const std::wstring DayOfWeekListWednesdayString(L"Wednesday");
const std::wstring DayOfWeekListThursdayString(L"Thursday");
const std::wstring DayOfWeekListFridayString(L"Friday");
const std::wstring DayOfWeekListSaturdayString(L"Saturday");
const std::wstring DayOfWeekListSundayString(L"Sunday");

const std::wstring MonthListJanuaryString(L"January");
const std::wstring MonthListFebruaryString(L"February");
const std::wstring MonthListMarchString(L"March");
const std::wstring MonthListAprilString(L"May");
const std::wstring MonthListMayString(L"May");
const std::wstring MonthListJuneString(L"June");
const std::wstring MonthListJulyString(L"July");
const std::wstring MonthListAugustString(L"August");
const std::wstring MonthListSeptemberString(L"September");
const std::wstring MonthListOctoberString(L"October");
const std::wstring MonthListNovemberString(L"November");
const std::wstring MonthListDecemberString(L"December");

const std::wstring NominativeDayOfWeekDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Day of Week");
const std::wstring NominativeMonthsDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Months");
const std::wstring NominativeMonthDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Month");

const std::wstring GenitiveYearDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Year");
const std::wstring GenitiveMonthsDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Months");
const std::wstring GenitiveMonthDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Month");
const std::wstring GenitiveDayDirectoryString = CatalogBaseString + std::wstring(L"\\Standards\\Time\\Day");
