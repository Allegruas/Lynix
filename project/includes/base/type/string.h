#ifndef LYNIX_STRING_H
#define LYNIX_STRING_H

#include <string.h>     
#include "../../lynix.h"

#if defined(_WIN32) && defined(_MSC_VER)
	#define strdup _strdup 
#endif

/**
 * �����ֽ��ַ���ת��Ϊ���ַ���
 * @param str ����Ķ��ֽ��ַ��� (UTF-8)
 * @return �·���Ŀ��ַ��� (������� free)��ʧ�ܷ��� NULL
 */
LYNIX_API wchar_t* strtow(char*);

#endif
