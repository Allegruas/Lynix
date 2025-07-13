#ifndef FILE_H
#define FILE_H

#include "../../lynix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <libgen.h>
#endif


/*
*	@brief ��ȡ��ȫ·��
*	@param path ����ȫ·��?
*/
LYNIX_API char* getAbsolutePath(const char*);

/*
*	@brief ��ȡָ���е�����
*	@param path ·��
*	@param ��
*/
LYNIX_API char* getFileLine(const char*, int);

/*
*	@brief ��ȡָ������
*	@param path ·��
*	@param ��
*/
LYNIX_API char* readFileAll(const char* path);

#endif // !FILE_H
