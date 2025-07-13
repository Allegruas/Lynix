#ifndef LYNIX_STRING_H
#define LYNIX_STRING_H

#include <string.h>     
#include "../../lynix.h"

#if defined(_WIN32) && defined(_MSC_VER)
	#define strdup _strdup 
#endif

/**
 * 将多字节字符串转换为宽字符串
 * @param str 输入的多字节字符串 (UTF-8)
 * @return 新分配的宽字符串 (需调用者 free)，失败返回 NULL
 */
LYNIX_API wchar_t* strtow(char*);

#endif
