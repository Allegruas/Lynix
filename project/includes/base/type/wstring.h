#ifndef STRING_H
#define STRING_H

#include <wchar.h>
#include <stdlib.h>
#include "bool.h"
#include "../../lynix.h"

#define wstring wchar_t*
#define wchar wchar_t


#ifdef _WIN32
	#define wcscasecmp _wcsicmp
#endif

#if defined(_WIN32) && defined(_MSC_VER)
	#define wcsdup _wcsdup 
#endif


LYNIX_API char wctoc(wchar);
LYNIX_API char* wcstos(wstring);
LYNIX_API char* wctos(wchar);
#endif // !STRING_H
