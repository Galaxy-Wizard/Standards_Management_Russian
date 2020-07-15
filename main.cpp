#include <windows.h>

#include <windowsx.h>
#include <fileapi.h>

#include <atltime.h>

#include "Manager.h"

#ifdef Russian
#include "Localization.h"
#else // Russian
#ifdef English
#include "English\Localization.h"
#else // Russian
#error Localization does not exist.
#endif // English

#endif // Russian


Constant bool MemoryLeaksDebuging = true;
Constant bool MemoryLeaksDebugingDone = true;

LRESULT CALLBACK MessagesHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT Render(HWND, Integer, Integer);
LRESULT LeftMouseButtonPressed(HWND p1, UINT p2, WPARAM p3, LPARAM p4);
LRESULT WindowScroll(HWND p1, UINT p2, WPARAM p3, LPARAM p4);

std::wstring FormatTime(WORD Year, WORD Month, WORD Day, WORD DayOfWeek, WORD Hour, WORD Minute, WORD Second);
std::wstring FormatDay(WORD Day);
std::wstring FormatDayOfWeek(WORD DayOfWeek);
std::wstring FormatMonth(WORD Month);
std::wstring FormatYear(WORD Year);

SYSTEMTIME CalendarTime;

Integer x = 22;
Integer y = 16;

Integer WindowScrollY = 0;
Integer SavedCoordinateY = 0;

bool Rendering = false;
DWORD RenderFrequency = 0;
DWORD FramesPerSecond = 1;

std::list<std::wstring> CurrentStandardsManager;
std::wstring CurrentDirectory(CatalogString);
std::wstring CurrentTable;

std::wstring BaseDirectory;

bool LeftMouseState = false;

HCURSOR Cursor = NULL;

double ApplicationDrawingRefreshTime = 0;

Integer WINAPI WinMain(HINSTANCE Instance, HINSTANCE, LPSTR, Integer)
{
	Constant Integer BaseDirectoryBufferLength = 50000;
	WCHAR BaseDirectoryBuffer[BaseDirectoryBufferLength];

	ZeroMemory(&BaseDirectoryBuffer, BaseDirectoryBufferLength * DataSize(WCHAR));

	GetCurrentDirectoryW(BaseDirectoryBufferLength, BaseDirectoryBuffer);
	BaseDirectory = std::wstring(BaseDirectoryBuffer);

	WNDCLASSEXW WindowClass = { DataSize(WNDCLASSEXW), CS_CLASSDC, MessagesHandler, 0, 0, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"WindowClass", NULL };

	RegisterClassExW(&WindowClass);

	HWND WindowHandle = CreateWindowW(L"WindowClass", WindowCaptionString.c_str(),
		WS_OVERLAPPEDWINDOW | WS_POPUP,
		0, 0, 700, 900, GetDesktopWindow(), NULL, WindowClass.hInstance, NULL);

	Automatic CURSOR_ARROW = MAKEINTRESOURCEW(32512);

	Cursor = LoadCursorW(NULL, CURSOR_ARROW);

	SetCursor(Cursor);

	ZeroMemory(&CalendarTime, DataSize(SYSTEMTIME));

	GetLocalTime(&CalendarTime);

	ShowWindow(WindowHandle, SW_SHOW);

	UpdateWindow(WindowHandle);

	MSG Message;

	DWORD MaximumFramesPerSecond = 1000;

	DWORD StartTicks = GetTickCount();

	DWORD TicksPast = 0;

	PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE);

	for (; Message.message != WM_QUIT && Message.message != WM_CLOSE;)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageW(&Message);
		}
		else
		{
			if (!Rendering)
			{
				if (FramesPerSecond <= MaximumFramesPerSecond)
				{
					Render(WindowHandle, x, y);
				}

				DWORD EndTicks = GetTickCount();

				TicksPast = EndTicks - StartTicks;

				if (TicksPast > 1000)
				{
					RenderFrequency = FramesPerSecond;

					FramesPerSecond = 1;
					StartTicks = GetTickCount();
				}
			}
		}
	}

	DestroyCursor(Cursor);

	CloseWindow(WindowHandle);

	return 1;
}


LRESULT CALLBACK MessagesHandler(HWND p1, UINT p2, WPARAM p3, LPARAM p4)
{
	LRESULT Result = 1;

	if (p2 == WM_ERASEBKGND)
	{
		x = 22;
		y = 16;

		Result = 0;
	}
	else
	{
		if (p2 == WM_PAINT)
		{
			Result = DefWindowProcW(p1, p2, p3, p4);
			if (Result == 0)
			{
				if (!Rendering)
				{
					Result = Render(p1, x, y);
				}
			}
		}
		else
		{
			if (p2 == WM_LBUTTONUP)
			{
				Result = LeftMouseButtonPressed(p1, p2, p3, p4);

				LeftMouseState = false;
			}
			else
			{
				if (p2 == WM_LBUTTONDOWN)
				{
					LeftMouseState = true;

					Result = 0;
				}
				else
				{
					if (p2 == WM_MOUSEMOVE)
					{
						SetCursor(Cursor);

						Result = WindowScroll(p1, p2, p3, p4);
					}
					else
					{
						Result = DefWindowProcW(p1, p2, p3, p4);
					}
				}
			}
		}
	}

	return Result;
}


LRESULT Render(HWND WindowHandle, Integer x, Integer y)
{
	LRESULT Result = 0;

	Rendering = true;

	DWORD ApplicationDrawingRefreshTimeStart = GetTickCount();

	SetCurrentDirectoryW(BaseDirectory.c_str());

	HDC DeviceContextHandleWindow = GetDC(WindowHandle);

	if (DeviceContextHandleWindow != NULL)
	{
		HDC DeviceContextHandle = CreateCompatibleDC(DeviceContextHandleWindow);

		if (DeviceContextHandle != NULL)
		{
			RECTANGLE ClientRectangle;
			ZeroMemory(&ClientRectangle, DataSize(RECTANGLE));

			GetClientRect(WindowHandle, &ClientRectangle);

			ClientRectangle.right -= ClientRectangle.left;
			ClientRectangle.bottom -= ClientRectangle.top;

			DWORD GRAY_COLOR = COLORREF(RGB(128, 128, 128));
			DWORD BLACK_COLOR = COLORREF(RGB(0, 0, 0));

			HBITMAP BitMapHandle = CreateCompatibleBitmap(DeviceContextHandleWindow, ClientRectangle.right, ClientRectangle.bottom);

			if (BitMapHandle != NULL)
			{
				SelectObject(DeviceContextHandle, BitMapHandle);

				HBRUSH Brush = HBRUSH(GetStockObject(GRAY_BRUSH));

				FillRect(DeviceContextHandle, &ClientRectangle, Brush);

				SetBkMode(DeviceContextHandle, TRANSPARENT);

				HGDIOBJ OldBrush = SelectObject(DeviceContextHandle, Brush);

				SetTextColor(DeviceContextHandle, BLACK_COLOR);

				LOGFONTW LogFontDefault;

				ZeroMemory(&LogFontDefault, DataSize(LogFontDefault));

				LogFontDefault.lfHeight = 16;
				LogFontDefault.lfCharSet = CHARACTERS_SET;
				//wcscpy_s(LogFontDefault.lfFaceName, L"Times New Roman");


				HFONT FontDefault = CreateFontIndirectW(&LogFontDefault);

				HFONT FontDefaultOld = SelectFont(DeviceContextHandle, FontDefault);

				if (Rectangle(DeviceContextHandle, 0 + 1, ClientRectangle.top + 1 - WindowScrollY, 200 - 1, ClientRectangle.bottom - 1 - WindowScrollY) != TRUE)
				{
					std::wstring Error(ErrorDrawingString);

					Result = 1;
				}
				if (Rectangle(DeviceContextHandle, 200 + 1, ClientRectangle.top + 1, 240 - 1, ClientRectangle.bottom - 1) != TRUE)
				{
					std::wstring Error(ErrorDrawingString);

					Result = 1;
				}

				if (MemoryLeaksDebugingDone)
				{
					Manager StandardsManager;

					std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(CurrentDirectory);

					if (MemoryLeaksDebugingDone)
					{
						CurrentStandardsManager = CurrentDirectoryStandardsManager;
						if (MemoryLeaksDebugingDone)
						{
							if (CurrentTable.length() == 0)
							{
								for (Automatic Iterator = CurrentDirectoryStandardsManager.begin(); Iterator != CurrentDirectoryStandardsManager.end(); Iterator++)
								{
									if (y + 1 - 30 / 2 - WindowScrollY >= ClientRectangle.top && y - 1 + 30 / 2 - WindowScrollY <= ClientRectangle.bottom)
									{
										if (Rectangle(DeviceContextHandle, 0 + 1, y + 1 - 30 / 2 - WindowScrollY, 200 - 1, y - 1 + 30 / 2 - WindowScrollY) != TRUE)
										{
											std::wstring Error(ErrorDrawingString);

											Result = 1;

											break;
										}
									}

									if (y - WindowScrollY >= ClientRectangle.top && y - WindowScrollY <= ClientRectangle.bottom)
									{
										Constant std::wstring Text = *Iterator;
										Integer TextLength = Integer(Text.length());

										if (TextOutW(DeviceContextHandle, x + 10, y - WindowScrollY, Text.c_str(), TextLength) != TRUE)
										{
											std::wstring Error(ErrorDrawingString);

											Result = 1;
											break;
										}
									}

									x += 0;
									y += 30;
								}
							}
							else
							{
								HANDLE CurrentTableFile = CreateFileW(CurrentTable.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

								if (CurrentTableFile != NULL)
								{
									/*/
									DWORD BytesRead = 0;
									Constant DWORD FileBufferSize = 50000;

									WCHAR FileBuffer[FileBufferSize];
									ZeroMemory(&FileBuffer, FileBufferSize * DataSize(WCHAR));

									WCHAR FileSignatute = WCHAR();
									ReadFile(CurrentTableFile, &FileSignatute, DataSize(WCHAR), &BytesRead, NULL);

									if (ReadFile(CurrentTableFile, &FileBuffer, FileBufferSize * DataSize(WCHAR), &BytesRead, NULL) == TRUE)
									{
										std::wstring Table;

										std::list<std::wstring> TableLines;

										for (DWORD Item = 0; Item < BytesRead / DataSize(WCHAR); Item++)
										{
											WCHAR CurrentSymbol = FileBuffer[Item];

											if (CurrentSymbol != L'\n' && CurrentSymbol != L'\r')
											{
												Table += CurrentSymbol;
											}
											else
											{
												if (Table.length() != 0)
												{
													TableLines.push_back(Table);
												}

												Table = std::wstring();
											}
										}

										for (Automatic Iterator = TableLines.begin(); Iterator != TableLines.end(); Iterator++)
										{
											if (y + 1 - 30 / 2 - WindowScrollY >= ClientRectangle.top && y - 1 + 30 / 2 - WindowScrollY <= ClientRectangle.bottom)
											{
												if (Rectangle(DeviceContextHandle, 0 + 1, y + 1 - 30 / 2 - WindowScrollY, 200 - 1, y - 1 + 30 / 2 - WindowScrollY) != TRUE)
												{
													std::wstring Error(ErrorDrawingString);

													Result = 1;

													break;
												}

											}

											if (y - WindowScrollY >= ClientRectangle.top && y - WindowScrollY <= ClientRectangle.bottom)
											{
												Constant std::wstring Text = *Iterator;
												Integer TextLength = Integer(Text.length());
												if (TextOutW(DeviceContextHandle, x + 10, y - WindowScrollY, Text.c_str(), TextLength) != TRUE)
												{
													std::wstring Error(ErrorDrawingString);

													Result = 1;

													break;
												}
											}

											x += 0;
											y += 30;
										}
									}
									else
									{
										std::wstring Error(ErrorReadingFileString);

										Result = 1;
									}
									/*/
									CloseHandle(CurrentTableFile);

									ShellExecuteW(WindowHandle, std::wstring(L"Open").c_str(), CurrentTable.c_str(), NULL, CurrentDirectory.c_str(), SW_SHOW);

									CurrentTable = std::wstring();
								}
							}
						}
					}
				}


				SYSTEMTIME SystemTime;
				ZeroMemory(&SystemTime, DataSize(SYSTEMTIME));

				GetSystemTime(&SystemTime);

				if (MemoryLeaksDebugingDone)
				{
					WORD Year = SystemTime.wYear;
					WORD Month = SystemTime.wMonth;
					WORD Day = SystemTime.wDay;
					WORD DayOfWeek = SystemTime.wDayOfWeek;
					WORD Hour = SystemTime.wHour;
					WORD Minute = SystemTime.wMinute;
					WORD Second = SystemTime.wSecond;

					std::wstring ClockCity(LondonTimeString);
					std::wstring CurrentTime = FormatTime(Year, Month, Day, DayOfWeek, Hour, Minute, Second);

					Integer ClockCityLength = Integer(ClockCity.length());

					if (TextOutW(DeviceContextHandle, 250, 16, ClockCity.c_str(), ClockCityLength) != TRUE)
					{
						std::wstring Error(ErrorDrawingString);

						Result = 1;
					}

					Integer CurrentTimeLength = Integer(CurrentTime.length());

					if (TextOutW(DeviceContextHandle, 250, 16 + 30, CurrentTime.c_str(), CurrentTimeLength) != TRUE)
					{
						std::wstring Error(ErrorDrawingString);

						Result = 1;
					}
				}

				if (MemoryLeaksDebugingDone)
				{
					SYSTEMTIME LocalTime;
					ZeroMemory(&LocalTime, DataSize(SYSTEMTIME));

					GetLocalTime(&LocalTime);

					WORD Year = LocalTime.wYear;
					WORD Month = LocalTime.wMonth;
					WORD Day = LocalTime.wDay;
					WORD DayOfWeek = LocalTime.wDayOfWeek;
					WORD Hour = LocalTime.wHour;
					WORD Minute = LocalTime.wMinute;
					WORD Second = LocalTime.wSecond;

					std::wstring ClockCity(LocalTimeString);
					std::wstring CurrentTime = FormatTime(Year, Month, Day, DayOfWeek, Hour, Minute, Second);

					Integer ClockCityLength = Integer(ClockCity.length());

					if (TextOutW(DeviceContextHandle, 250, 16 + 30 + 30, ClockCity.c_str(), ClockCityLength) != TRUE)
					{
						std::wstring Error(ErrorDrawingString);

						Result = 1;
					}

					Integer CurrentTimeLength = Integer(CurrentTime.length());

					if (TextOutW(DeviceContextHandle, 250, 16 + 30 + 30 + 30, CurrentTime.c_str(), CurrentTimeLength) != TRUE)
					{
						std::wstring Error(ErrorDrawingString);

						Result = 1;
					}

					if (MemoryLeaksDebugingDone)
					{
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50, 250 + 210, 16 + 30 + 30 + 30 + 50 + 100) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						if (Rectangle(DeviceContextHandle, 250 + 40, 16 + 30 + 30 + 30 + 50, 250 + 210 - 40, 16 + 30 + 30 + 30 + 50 + 100) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						std::wstring CurrentDayOfWeek = FormatDayOfWeek(CalendarTime.wDayOfWeek);

						Integer CurrentDayOfWeekLength = Integer(CurrentDayOfWeek.length());

						if (TextOutW(DeviceContextHandle, 250 + 40 + 10, 16 + 30 + 30 + 30 + 50 + 16 / 2, CurrentDayOfWeek.c_str(), CurrentDayOfWeekLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}


						LOGFONTW LogFont;

						ZeroMemory(&LogFont, DataSize(LogFont));

						LogFont.lfHeight = 64;
						LogFont.lfCharSet = CHARACTERS_SET;
						//wcscpy_s(LogFont.lfFaceName, L"Times New Roman");


						HFONT Font = CreateFontIndirectW(&LogFont);

						HFONT FontOld = SelectFont(DeviceContextHandle, Font);

						std::wstring CurrentDay = FormatDay(CalendarTime.wDay);

						Integer CurrentDayLength = Integer(CurrentDay.length());

						if (TextOutW(DeviceContextHandle, 250 + 40 + 10, 16 + 30 + 30 + 30 + 50 + 16 / 2 + 30, CurrentDay.c_str(), CurrentDayLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						SelectFont(DeviceContextHandle, FontOld);

						WORD BackwardArrow = 0x02C2;
						std::wstring CurrentBackward;
						CurrentBackward += WCHAR(BackwardArrow);

						Integer CurrentBackwardLength = Integer(CurrentBackward.length());

						if (TextOutW(DeviceContextHandle, 250 + 10, 16 + 30 + 30 + 30 + 50 + 16 / 2 + 30, CurrentBackward.c_str(), CurrentBackwardLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						WORD ForwardArrow = 0x02C3;
						std::wstring CurrentForward;
						CurrentForward += WCHAR(ForwardArrow);

						Integer CurrentForwardLength = Integer(CurrentForward.length());

						if (TextOutW(DeviceContextHandle, 250 + 210 - 40 + 10, 16 + 30 + 30 + 30 + 50 + 16 / 2 + 30, CurrentForward.c_str(), CurrentForwardLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 0, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 1) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 1, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 2) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 2, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 3) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 3, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 4) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 4, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 5) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 5, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}


						std::list<std::list<std::wstring>> CalendarMonthStrings;

						ATL::CTime TimeCalculator(CalendarTime);

						ATL::CTimeSpan TimeSpanCalculator1(1, 0, 0, 0);
						ATL::CTimeSpan TimeSpanCalculator2(TimeCalculator.GetDay(), 0, 0, 0);

						TimeCalculator += TimeSpanCalculator1;
						TimeCalculator -= TimeSpanCalculator2;

						ATL::CTime TimeCalculatorIterator(TimeCalculator);
						ATL::CTimeSpan TimeSpanCalculatorDay(1, 0, 0, 0);

						Automatic CurrentCalculatorDayOfWeek = TimeCalculatorIterator.GetDayOfWeek();

						std::list<std::wstring> CurrentLineOfCalendarMonthStrings;


						Automatic CurrentDayOfWeekIterator = 2;

						if (CurrentCalculatorDayOfWeek == 1)
						{
							CurrentCalculatorDayOfWeek = 8;
						}

						for (; CurrentDayOfWeekIterator < CurrentCalculatorDayOfWeek; CurrentDayOfWeekIterator++)
						{
							CurrentLineOfCalendarMonthStrings.push_back(std::wstring(L" "));
						}

						for (; CurrentDayOfWeekIterator <= 8; CurrentDayOfWeekIterator++)
						{
							std::wstring CurrentDayString = FormatDay(TimeCalculatorIterator.GetDay());
							CurrentLineOfCalendarMonthStrings.push_back(CurrentDayString);
							TimeCalculatorIterator += TimeSpanCalculatorDay;
						}

						CalendarMonthStrings.push_back(CurrentLineOfCalendarMonthStrings);

						CurrentLineOfCalendarMonthStrings.clear();

						for (; TimeCalculatorIterator.GetMonth() == TimeCalculator.GetMonth(); )
						{
							for (CurrentDayOfWeekIterator = 2; CurrentDayOfWeekIterator <= 8 && TimeCalculatorIterator.GetMonth() == TimeCalculator.GetMonth(); CurrentDayOfWeekIterator++)
							{
								std::wstring CurrentDayString = FormatDay(TimeCalculatorIterator.GetDay());
								CurrentLineOfCalendarMonthStrings.push_back(CurrentDayString);
								TimeCalculatorIterator += TimeSpanCalculatorDay;
							}

							CalendarMonthStrings.push_back(CurrentLineOfCalendarMonthStrings);

							CurrentLineOfCalendarMonthStrings.clear();
						}

						Automatic Y = 0;
						for (Automatic IteratorY = CalendarMonthStrings.begin(); IteratorY != CalendarMonthStrings.end(); IteratorY++)
						{
							Automatic X = 0;
							for (Automatic IteratorX = IteratorY->begin(); IteratorX != IteratorY->end(); IteratorX++)
							{
								std::wstring CurrentMonthCalendarDay = *IteratorX;
								Integer CurrentMonthCalendarDayLength = Integer(CurrentMonthCalendarDay.length());


								if (TextOutW(DeviceContextHandle, 250 + 40 * X, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * Y, CurrentMonthCalendarDay.c_str(), CurrentMonthCalendarDayLength) != TRUE)
								{
									std::wstring Error(ErrorDrawingString);

									Result = 1;

									break;
								}

								X++;
							}

							Y++;
						}


						std::wstring CurrentMonth = FormatMonth(CalendarTime.wMonth);
						Integer CurrentMonthLength = Integer(CurrentMonth.length());

						FontOld = SelectFont(DeviceContextHandle, Font);

						if (TextOutW(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20, CurrentMonth.c_str(), CurrentMonthLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						std::wstring CurrentYear = FormatYear(CalendarTime.wYear);
						Integer CurrentYearLength = Integer(CurrentYear.length());

						if (TextOutW(DeviceContextHandle, 250, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 64 + 20, CurrentYear.c_str(), CurrentYearLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						SelectFont(DeviceContextHandle, FontOld);

						if (Rectangle(DeviceContextHandle, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20, 250 + 40 * 7 + 40, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
						if (Rectangle(DeviceContextHandle, 250 + 40 * 7, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30 + 5, 250 + 40 * 7 + 40, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30 + 5 + 30) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						WORD UpArrow = 0x02C4;
						std::wstring CurrentUp;
						CurrentUp += WCHAR(UpArrow);
						Integer CurrentUpLength = Integer(CurrentUp.length());
						if (TextOutW(DeviceContextHandle, 250 + 40 * 7 + 10, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 10, CurrentUp.c_str(), CurrentUpLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						WORD DownArrow = 0x02C5;
						std::wstring CurrentDown;
						CurrentDown += WCHAR(DownArrow);
						Integer CurrentDownLength = Integer(CurrentDown.length());
						if (TextOutW(DeviceContextHandle, 250 + 40 * 7 + 10, 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30 + 10, CurrentDown.c_str(), CurrentDownLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}

						if (DeleteObject(Font) != TRUE)
						{
							std::wstring Error(ErrorDeletingFontString);

							Result = 1;
						}

						FontDefaultOld = SelectFont(DeviceContextHandle, FontDefault);

						if (DeleteObject(FontDefault) != TRUE)
						{
							std::wstring Error(ErrorDeletingFontString);

							Result = 1;
						}
					}

					if (MemoryLeaksDebugingDone)
					{
						Constant Integer NumberBufferLength = 50000;
						WCHAR NumberBuffer[NumberBufferLength];

						ZeroMemory(NumberBuffer, NumberBufferLength * DataSize(WCHAR));

						_itow_s(RenderFrequency, NumberBuffer, NumberBufferLength, 10);

						std::wstring NumberRenderFrequency(NumberBuffer);

						for (Automatic iterator = NumberRenderFrequency.length(); iterator < 4; iterator++)
						{
							NumberRenderFrequency = std::wstring(L"0") + NumberRenderFrequency;
						}

						ZeroMemory(NumberBuffer, NumberBufferLength * DataSize(WCHAR));

						_itow_s(FramesPerSecond, NumberBuffer, NumberBufferLength, 10);

						std::wstring NumberRenderCurrentFrameNumber(NumberBuffer);

						for (Automatic iterator = NumberRenderCurrentFrameNumber.length(); iterator < 4; iterator++)
						{
							NumberRenderCurrentFrameNumber = std::wstring(L"0") + NumberRenderCurrentFrameNumber;
						}

						std::wstring CurrentNumber;
						CurrentNumber = std::wstring(FrameRateString) + NumberRenderFrequency + std::wstring(CurrentFrameString) + NumberRenderCurrentFrameNumber + std::wstring(PointString);

						Integer CurrentNumberLength = Integer(CurrentNumber.length());

						RECTANGLE ClientRectangle;
						ZeroMemory(&ClientRectangle, DataSize(RECTANGLE));

						GetClientRect(WindowHandle, &ClientRectangle);

						SetBkMode(DeviceContextHandle, TRANSPARENT);

						if (TextOutW(DeviceContextHandle, ClientRectangle.right - CurrentNumberLength * 10, ClientRectangle.bottom - 30, CurrentNumber.c_str(), CurrentNumberLength) != TRUE)
						{
							std::wstring Error(ErrorDrawingString);

							Result = 1;
						}
					}
				}

				SelectObject(DeviceContextHandle, OldBrush);

				if (BitBlt(DeviceContextHandleWindow, ClientRectangle.left, ClientRectangle.top, ClientRectangle.right, ClientRectangle.bottom, DeviceContextHandle, 0, 0, SRCCOPY) != TRUE)
				{
					std::wstring Error(ErrorCopyingContextString);

					Result = 1;
				}

				if (DeleteObject(Brush) != TRUE)
				{
					std::wstring Error(ErrorDeletingBrushString);

					Result = 1;
				}

				if (DeleteObject(BitMapHandle) != TRUE)
				{
					std::wstring Error(ErrorDeletingBitMapString);

					Result = 1;
				}

				FramesPerSecond++;
			}

			if (DeleteDC(DeviceContextHandle) != TRUE)
			{
				std::wstring Error(ErrorDeletingMemoryContextString);

				Result = 1;
			}
		}
		else
		{
			std::wstring Error(ErrorCreatingMemoryContextString);
			Result = 1;
		}

		if (DeleteDC(DeviceContextHandleWindow) != TRUE)
		{
			std::wstring Error(ErrorDeletingContextString);

			Result = 1;
		}
	}
	else
	{
		std::wstring Error(ErrorCreatingMemoryContextString);
		Result = 1;
	}

	DWORD ApplicationDrawingRefreshTimeEnd = GetTickCount();

	ApplicationDrawingRefreshTime += ApplicationDrawingRefreshTimeEnd - ApplicationDrawingRefreshTimeStart;

	Rendering = false;


	return Result;
}


LRESULT LeftMouseButtonPressed(HWND p1, UINT p2, WPARAM p3, LPARAM p4)
{
	Integer CoordinateX = GET_X_LPARAM(p4);
	Integer CoordinateY = GET_Y_LPARAM(p4);

	LRESULT Result = 0;

	if (CoordinateX >= 0 && CoordinateX < 200)
	{

		bool StringFound = false;
		bool TableSelected = false;

		std::wstring SelectedString;

		Integer IteratorY = 16;

		for (Automatic Iterator = CurrentStandardsManager.begin(); Iterator != CurrentStandardsManager.end(); Iterator++)
		{
			if ((CoordinateY >= IteratorY + 1 - 30 / 2 - WindowScrollY) && (CoordinateY <= IteratorY - 1 + 30 / 2 - WindowScrollY))
			{
				SelectedString = *Iterator;

				StringFound = true;

				break;
			}

			IteratorY += 30;
		}

		if (StringFound)
		{
			if (SelectedString == L".")
			{
				CurrentDirectory = CatalogString;
			}
			else
				if (SelectedString == L"..")
				{
					std::wstring CurrentDirectoryWorking = CurrentDirectory;
					for (; CurrentDirectoryWorking.length() != 0;)
					{
						if (CurrentDirectoryWorking.back() == L'\\')
						{
							CurrentDirectoryWorking.pop_back();
							break;
						}
						CurrentDirectoryWorking.pop_back();
					}

					CurrentDirectory = CurrentDirectoryWorking;

					CurrentTable = std::wstring();
				}
				else
				{
					std::wstring CurrenStringWorking = SelectedString;

					for (; CurrenStringWorking.length() != 0;)
					{
						if (CurrenStringWorking.back() == L'.')
						{
							CurrenStringWorking.pop_back();
							TableSelected = true;

							break;
						}
						CurrenStringWorking.pop_back();
					}

					if (TableSelected)
					{
						CurrentTable = SelectedString;
					}
					else
					{
						CurrentDirectory += L"\\";
						CurrentDirectory += SelectedString;

						CurrentTable = std::wstring();
					}
				}

			if (CurrentDirectory.length() == 0)
			{
				CurrentDirectory = CatalogString;

				CurrentTable = std::wstring();
			}
		}

		if (!Rendering)
		{
			Result = Render(p1, x, y);
		}
	}
	else
	{
		if (CoordinateX >= 200 && CoordinateX < 240)
		{

		}
		else
		{
			if (CoordinateX > 250 && CoordinateX < 250 + 40 && CoordinateY > 16 + 30 + 30 + 30 + 50 && CoordinateY < 16 + 30 + 30 + 30 + 50 + 100)
			{
				//	std::wstring CurrentBackward;

				CTime CalendarTimeCurrent(CalendarTime);

				CalendarTimeCurrent -= CTimeSpan(1, 0, 0, 0);

				CalendarTimeCurrent.GetAsSystemTime(CalendarTime);

				if (!Rendering)
				{
					Result = Render(p1, x, y);
				}
			}
			else
			{
				if (CoordinateX > 250 + 210 - 40 && CoordinateX < 250 + 210 && CoordinateY > 16 + 30 + 30 + 30 + 50 && CoordinateY < 16 + 30 + 30 + 30 + 50 + 100)
				{
					//	std::wstring CurrentForward;

					CTime CalendarTimeCurrent(CalendarTime);

					CalendarTimeCurrent += CTimeSpan(1, 0, 0, 0);

					CalendarTimeCurrent.GetAsSystemTime(CalendarTime);

					if (!Rendering)
					{
						Result = Render(p1, x, y);
					}
				}
				else
				{
					if (
						CoordinateX > 250 + 40 * 7 &&
						CoordinateX < 250 + 40 * 7 + 40 &&
						CoordinateY > 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 &&
						CoordinateY < 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30
						)
					{
						//	std::wstring CurrentUp;
						CTime CalendarTimeCurrent(CalendarTime);

						Automatic SavedMonth = CalendarTimeCurrent.GetMonth();
						Automatic SavedDay = CalendarTimeCurrent.GetDay();

						for (; SavedMonth == CalendarTimeCurrent.GetMonth();)
						{
							CalendarTimeCurrent += CTimeSpan(1, 0, 0, 0);
						}

						SavedMonth = CalendarTimeCurrent.GetMonth();
						for (; SavedDay > CalendarTimeCurrent.GetDay() && SavedMonth == CalendarTimeCurrent.GetMonth();)
						{
							CalendarTimeCurrent += CTimeSpan(1, 0, 0, 0);
						}
						if (SavedMonth != CalendarTimeCurrent.GetMonth())
						{
							CalendarTimeCurrent -= CTimeSpan(1, 0, 0, 0);
						}

						CalendarTimeCurrent.GetAsSystemTime(CalendarTime);

						if (!Rendering)
						{
							Result = Render(p1, x, y);
						}
					}
					else
					{
						if (
							CoordinateX > 250 + 40 * 7 &&
							CoordinateX < 250 + 40 * 7 + 40 &&
							CoordinateY > 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30 + 5 &&
							CoordinateY < 16 + 30 + 30 + 30 + 50 + 100 + 10 + 40 * 6 + 20 + 30 + 5 + 30
							)
						{
							//	std::wstring CurrentDown;
							CTime CalendarTimeCurrent(CalendarTime);

							Automatic SavedMonth = CalendarTimeCurrent.GetMonth();
							Automatic SavedDay = CalendarTimeCurrent.GetDay();

							for (; SavedMonth == CalendarTimeCurrent.GetMonth();)
							{
								CalendarTimeCurrent -= CTimeSpan(1, 0, 0, 0);
							}

							SavedMonth = CalendarTimeCurrent.GetMonth();
							for (; SavedDay < CalendarTimeCurrent.GetDay() && SavedMonth == CalendarTimeCurrent.GetMonth();)
							{
								CalendarTimeCurrent -= CTimeSpan(1, 0, 0, 0);
							}
							if (SavedMonth != CalendarTimeCurrent.GetMonth())
							{
								CalendarTimeCurrent += CTimeSpan(1, 0, 0, 0);
							}

							CalendarTimeCurrent.GetAsSystemTime(CalendarTime);

							if (!Rendering)
							{
								Result = Render(p1, x, y);
							}
						}
						else
						{
							CurrentTable = std::wstring();
							WindowScrollY = 0;

							if (!Rendering)
							{
								Result = Render(p1, x, y);
							}
						}
					}
				}
			}
		}
	}

	return Result;
}

LRESULT WindowScroll(HWND p1, UINT p2, WPARAM p3, LPARAM p4)
{
	Integer CoordinateX = GET_X_LPARAM(p4);
	Integer CoordinateY = GET_Y_LPARAM(p4);

	LRESULT Result = 0;

	if (CoordinateX >= 200 && CoordinateX < 240)
	{
		if (LeftMouseState)
		{
			WindowScrollY += CoordinateY - SavedCoordinateY;

			Result = Render(p1, x, y);
		}
	}

	SavedCoordinateY = CoordinateY;

	return Result;
}


std::wstring FormatTimeLunarStyle(WORD Year, WORD Month, WORD Day, WORD DayOfWeek, WORD Hour, WORD Second)
{
	std::wstring Result;

	Constant Integer CurrentDirectoryBufferLength = 50000;
	WCHAR CurrentDirectoryBuffer[CurrentDirectoryBufferLength];

	ZeroMemory(CurrentDirectoryBuffer, CurrentDirectoryBufferLength * DataSize(WCHAR));

	GetCurrentDirectoryW(CurrentDirectoryBufferLength, CurrentDirectoryBuffer);

	std::wstring CurrentDirectory(CurrentDirectoryBuffer);

	SetCurrentDirectoryW(BaseDirectory.c_str());

	Manager StandardsManager;

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::wstring DayDirectory(GenitiveDayDirectoryString);

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(DayDirectory);

		std::wstring StringDay;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringDay = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringDay.pop_back();
				if (StringDay.back() == L'.')
				{
					StringDay.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Constant Integer DayNumberBufferLength = 50000;
		WCHAR DayNumberBuffer[DayNumberBufferLength];

		ZeroMemory(DayNumberBuffer, DayNumberBufferLength * DataSize(WCHAR));

		_itow_s(Day, DayNumberBuffer, DayNumberBufferLength, 10);

		std::wstring DayNumber(DayNumberBuffer);

		Result += std::wstring(L" ") + DayNumber + std::wstring(L" ") + StringDay;
	}

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::wstring MonthDirectory(GenitiveMonthDirectoryString);

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(MonthDirectory);

		std::wstring StringMonth;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringMonth = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringMonth.pop_back();
				if (StringMonth.back() == L'.')
				{
					StringMonth.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Constant Integer MonthNumberBufferLength = 50000;
		WCHAR MonthNumberBuffer[MonthNumberBufferLength];

		ZeroMemory(MonthNumberBuffer, MonthNumberBufferLength * DataSize(WCHAR));

		_itow_s(Month, MonthNumberBuffer, MonthNumberBufferLength, 10);

		std::wstring MonthNumber(MonthNumberBuffer);

		Result += std::wstring(L" ") + MonthNumber + std::wstring(L" ") + StringMonth;
	}

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::wstring YearDirectory(GenitiveYearDirectoryString);

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(YearDirectory);

		std::wstring StringYear;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringYear = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringYear.pop_back();
				if (StringYear.back() == L'.')
				{
					StringYear.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Constant Integer YearNumberBufferLength = 50000;
		WCHAR YearNumberBuffer[YearNumberBufferLength];

		ZeroMemory(YearNumberBuffer, YearNumberBufferLength * DataSize(WCHAR));

		_itow_s(Year, YearNumberBuffer, YearNumberBufferLength, 10);

		std::wstring YearNumber(YearNumberBuffer);

		Result += std::wstring(L" ") + YearNumber + std::wstring(L" ") + StringYear + std::wstring(L".");
	}

	SetCurrentDirectoryW(CurrentDirectory.c_str());

	return Result;
}


std::wstring FormatTime(WORD Year, WORD Month, WORD Day, WORD DayOfWeek, WORD Hour, WORD Minute, WORD Second)
{

	{
		Constant DWORD CurrentDatabaseLength = 10000;
		wchar_t CurrentDatabase[CurrentDatabaseLength];
		if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
		{
			std::wstring CurrentDirectory(CurrentDatabase);
		}
	}

	std::wstring Result;

	Constant Integer CurrentDirectoryBufferLength = 50000;
	WCHAR CurrentDirectoryBuffer[CurrentDirectoryBufferLength];

	ZeroMemory(CurrentDirectoryBuffer, CurrentDirectoryBufferLength * DataSize(WCHAR));

	GetCurrentDirectoryW(CurrentDirectoryBufferLength, CurrentDirectoryBuffer);

	std::wstring CurrentDirectory(CurrentDirectoryBuffer);

	SetCurrentDirectoryW(BaseDirectory.c_str());

	Manager StandardsManager;

	{
		std::list<std::wstring> DayOfWeekList;

		DayOfWeekList.push_back(DayOfWeekListMondayString);
		DayOfWeekList.push_back(DayOfWeekListTuesdayString);
		DayOfWeekList.push_back(DayOfWeekListWednesdayString);
		DayOfWeekList.push_back(DayOfWeekListThursdayString);
		DayOfWeekList.push_back(DayOfWeekListFridayString);
		DayOfWeekList.push_back(DayOfWeekListSaturdayString);
		DayOfWeekList.push_back(DayOfWeekListSundayString);

		Automatic Iterator = DayOfWeekList.begin();

		switch (DayOfWeek)
		{
		case 0:DayOfWeek = 5; break;
		case 1:DayOfWeek = 6; break;
		case 2:DayOfWeek = 0; break;
		case 3:DayOfWeek = 1; break;
		case 4:DayOfWeek = 2; break;
		case 5:DayOfWeek = 3; break;
		case 6:DayOfWeek = 4; break;
		default: return std::wstring();
		}

		for (Automatic Counter = 0; Counter <= DayOfWeek; Counter++)
		{
			Iterator++;
		}
		if (Iterator == DayOfWeekList.end())
		{
			Iterator = DayOfWeekList.begin();
		}

		std::wstring DayOfWeekDirectory(NominativeDayOfWeekDirectoryString);

		DayOfWeekDirectory += std::wstring(L"\\") + *Iterator;

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(DayOfWeekDirectory);

		std::wstring StringDayOfWeek;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringDayOfWeek = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringDayOfWeek.pop_back();
				if (StringDayOfWeek.back() == L'.')
				{
					StringDayOfWeek.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Result += std::wstring(L" ") + StringDayOfWeek;
	}

	{
		Constant Integer HourNumberBufferLength = 50000;
		WCHAR HourNumberBuffer[HourNumberBufferLength];

		ZeroMemory(HourNumberBuffer, HourNumberBufferLength * DataSize(WCHAR));

		_itow_s(Hour, HourNumberBuffer, HourNumberBufferLength, 10);

		std::wstring HourNumber(HourNumberBuffer);

		if (HourNumber.length() == 1)
		{
			HourNumber = std::wstring(L"0") + HourNumber;
		}

		Result += std::wstring(L" ") + HourNumber;
	}

	{
		Constant Integer MinuteNumberBufferLength = 50000;
		WCHAR MinuteNumberBuffer[MinuteNumberBufferLength];

		ZeroMemory(MinuteNumberBuffer, MinuteNumberBufferLength * DataSize(WCHAR));

		_itow_s(Minute, MinuteNumberBuffer, MinuteNumberBufferLength, 10);

		std::wstring MinuteNumber(MinuteNumberBuffer);

		if (MinuteNumber.length() == 1)
		{
			MinuteNumber = std::wstring(L"0") + MinuteNumber;
		}

		Result += std::wstring(L":") + MinuteNumber;
	}

	{
		Constant Integer SecondNumberBufferLength = 50000;
		WCHAR SecondNumberBuffer[SecondNumberBufferLength];

		ZeroMemory(SecondNumberBuffer, SecondNumberBufferLength * DataSize(WCHAR));

		_itow_s(Second, SecondNumberBuffer, SecondNumberBufferLength, 10);

		std::wstring SecondNumber(SecondNumberBuffer);

		if (SecondNumber.length() == 1)
		{
			SecondNumber = std::wstring(L"0") + SecondNumber;
		}

		Result += std::wstring(L":") + SecondNumber;
	}

	{
		Constant Integer DayNumberBufferLength = 50000;
		WCHAR DayNumberBuffer[DayNumberBufferLength];

		ZeroMemory(DayNumberBuffer, DayNumberBufferLength * DataSize(WCHAR));

		_itow_s(Day, DayNumberBuffer, DayNumberBufferLength, 10);

		std::wstring DayNumber(DayNumberBuffer);

		if (DayNumber.length() == 1)
		{
			DayNumber = std::wstring(L"0") + DayNumber;
		}

		Result += std::wstring(L" ") + DayNumber;
	}

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::list<std::wstring> MonthList;

		MonthList.push_back(MonthListJanuaryString);
		MonthList.push_back(MonthListFebruaryString);
		MonthList.push_back(MonthListMarchString);
		MonthList.push_back(MonthListAprilString);
		MonthList.push_back(MonthListMayString);
		MonthList.push_back(MonthListJuneString);
		MonthList.push_back(MonthListJulyString);
		MonthList.push_back(MonthListAugustString);
		MonthList.push_back(MonthListSeptemberString);
		MonthList.push_back(MonthListOctoberString);
		MonthList.push_back(MonthListNovemberString);
		MonthList.push_back(MonthListDecemberString);

		Automatic Iterator = MonthList.begin();

		for (Automatic Counter = 0; Counter < Month - 1; Counter++)
		{
			Iterator++;
		}

		std::wstring MonthDirectory(GenitiveMonthsDirectoryString);

		MonthDirectory += std::wstring(L"\\") + *Iterator;

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(MonthDirectory);

		std::wstring StringMonth;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringMonth = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringMonth.pop_back();
				if (StringMonth.back() == L'.')
				{
					StringMonth.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Result += std::wstring(L" ") + StringMonth;
	}

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::wstring YearDirectory(GenitiveYearDirectoryString);

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(YearDirectory);

		std::wstring StringYear;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringYear = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringYear.pop_back();
				if (StringYear.back() == L'.')
				{
					StringYear.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Constant Integer YearNumberBufferLength = 50000;
		WCHAR YearNumberBuffer[YearNumberBufferLength];

		ZeroMemory(YearNumberBuffer, YearNumberBufferLength * DataSize(WCHAR));

		_itow_s(Year, YearNumberBuffer, YearNumberBufferLength, 10);

		std::wstring YearNumber(YearNumberBuffer);

		Result += std::wstring(L" ") + YearNumber + std::wstring(L" ") + StringYear + std::wstring(L".");
	}

	SetCurrentDirectoryW(CurrentDirectory.c_str());

	return Result;
}


std::wstring FormatDay(WORD Day)
{

	{
		Constant DWORD CurrentDatabaseLength = 10000;
		wchar_t CurrentDatabase[CurrentDatabaseLength];
		if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
		{
			std::wstring CurrentDirectory(CurrentDatabase);
		}
	}

	std::wstring Result;

	{
		Constant Integer DayNumberBufferLength = 50000;
		WCHAR DayNumberBuffer[DayNumberBufferLength];

		ZeroMemory(DayNumberBuffer, DayNumberBufferLength * DataSize(WCHAR));

		_itow_s(Day, DayNumberBuffer, DayNumberBufferLength, 10);

		std::wstring DayNumber(DayNumberBuffer);

		if (DayNumber.length() == 1)
		{
			DayNumber = std::wstring(L"0") + DayNumber;
		}

		Result = DayNumber;
	}

	return Result;
}


std::wstring FormatDayOfWeek(WORD DayOfWeek)
{

	{
		Constant DWORD CurrentDatabaseLength = 10000;
		wchar_t CurrentDatabase[CurrentDatabaseLength];
		if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
		{
			std::wstring CurrentDirectory(CurrentDatabase);
		}
	}

	std::wstring Result;

	Constant Integer CurrentDirectoryBufferLength = 50000;
	WCHAR CurrentDirectoryBuffer[CurrentDirectoryBufferLength];

	ZeroMemory(CurrentDirectoryBuffer, CurrentDirectoryBufferLength * DataSize(WCHAR));

	GetCurrentDirectoryW(CurrentDirectoryBufferLength, CurrentDirectoryBuffer);

	std::wstring CurrentDirectory(CurrentDirectoryBuffer);

	SetCurrentDirectoryW(BaseDirectory.c_str());

	Manager StandardsManager;

	{
		std::list<std::wstring> DayOfWeekList;

		DayOfWeekList.push_back(DayOfWeekListMondayString);
		DayOfWeekList.push_back(DayOfWeekListTuesdayString);
		DayOfWeekList.push_back(DayOfWeekListWednesdayString);
		DayOfWeekList.push_back(DayOfWeekListThursdayString);
		DayOfWeekList.push_back(DayOfWeekListFridayString);
		DayOfWeekList.push_back(DayOfWeekListSaturdayString);
		DayOfWeekList.push_back(DayOfWeekListSundayString);

		Automatic Iterator = DayOfWeekList.begin();

		switch (DayOfWeek)
		{
		case 0:DayOfWeek = 5; break;
		case 1:DayOfWeek = 6; break;
		case 2:DayOfWeek = 0; break;
		case 3:DayOfWeek = 1; break;
		case 4:DayOfWeek = 2; break;
		case 5:DayOfWeek = 3; break;
		case 6:DayOfWeek = 4; break;
		default: return std::wstring();
		}

		for (Automatic Counter = 0; Counter <= DayOfWeek; Counter++)
		{
			Iterator++;
		}
		if (Iterator == DayOfWeekList.end())
		{
			Iterator = DayOfWeekList.begin();
		}

		std::wstring DayOfWeekDirectory(NominativeDayOfWeekDirectoryString);

		DayOfWeekDirectory += std::wstring(L"\\") + *Iterator;

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(DayOfWeekDirectory);

		std::wstring StringDayOfWeek;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringDayOfWeek = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringDayOfWeek.pop_back();
				if (StringDayOfWeek.back() == L'.')
				{
					StringDayOfWeek.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Result = StringDayOfWeek;
	}

	return Result;
}


std::wstring FormatMonth(WORD Month)
{

	{
		Constant DWORD CurrentDatabaseLength = 10000;
		wchar_t CurrentDatabase[CurrentDatabaseLength];
		if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
		{
			std::wstring CurrentDirectory(CurrentDatabase);
		}
	}

	std::wstring Result;

	Constant Integer CurrentDirectoryBufferLength = 50000;
	WCHAR CurrentDirectoryBuffer[CurrentDirectoryBufferLength];

	ZeroMemory(CurrentDirectoryBuffer, CurrentDirectoryBufferLength * DataSize(WCHAR));

	GetCurrentDirectoryW(CurrentDirectoryBufferLength, CurrentDirectoryBuffer);

	std::wstring CurrentDirectory(CurrentDirectoryBuffer);

	SetCurrentDirectoryW(BaseDirectory.c_str());

	Manager StandardsManager;

	{
		SetCurrentDirectoryW(BaseDirectory.c_str());

		std::list<std::wstring> MonthList;

		MonthList.push_back(MonthListJanuaryString);
		MonthList.push_back(MonthListFebruaryString);
		MonthList.push_back(MonthListMarchString);
		MonthList.push_back(MonthListAprilString);
		MonthList.push_back(MonthListMayString);
		MonthList.push_back(MonthListJuneString);
		MonthList.push_back(MonthListJulyString);
		MonthList.push_back(MonthListAugustString);
		MonthList.push_back(MonthListSeptemberString);
		MonthList.push_back(MonthListOctoberString);
		MonthList.push_back(MonthListNovemberString);
		MonthList.push_back(MonthListDecemberString);

		Automatic Iterator = MonthList.begin();

		for (Automatic Counter = 0; Counter < Month - 1; Counter++)
		{
			Iterator++;
		}

		std::wstring MonthDirectory(NominativeMonthsDirectoryString);

		MonthDirectory += std::wstring(L"\\") + *Iterator;

		std::list<std::wstring> CurrentDirectoryStandardsManager = StandardsManager.Select(MonthDirectory);

		std::wstring StringMonth;

		if (CurrentDirectoryStandardsManager.size() != 0)
		{
			CurrentDirectoryStandardsManager.pop_front();
			CurrentDirectoryStandardsManager.pop_front();

			StringMonth = CurrentDirectoryStandardsManager.front();

			for (;;)
			{
				StringMonth.pop_back();
				if (StringMonth.back() == L'.')
				{
					StringMonth.pop_back();
					break;
				}
			}
		}

		SetCurrentDirectoryW(CurrentDirectory.c_str());

		Result = StringMonth;
	}

	return Result;
}


std::wstring FormatYear(WORD Year)
{

	{
		Constant DWORD CurrentDatabaseLength = 10000;
		wchar_t CurrentDatabase[CurrentDatabaseLength];
		if (GetCurrentDirectoryW(CurrentDatabaseLength, CurrentDatabase) != 0)
		{
			std::wstring CurrentDirectory(CurrentDatabase);
		}
	}

	Constant Integer YearNumberBufferLength = 50000;
	WCHAR YearNumberBuffer[YearNumberBufferLength];

	ZeroMemory(YearNumberBuffer, YearNumberBufferLength * DataSize(WCHAR));

	_itow_s(Year, YearNumberBuffer, YearNumberBufferLength, 10);

	std::wstring YearNumber(YearNumberBuffer);

	return YearNumber;
}

