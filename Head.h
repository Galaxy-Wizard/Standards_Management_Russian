#pragma once

#define Constant const
#define DataSize sizeof
#define RECTANGLE RECT
#define Integer int
#define Automatic auto

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

#include "Manager.cpp"