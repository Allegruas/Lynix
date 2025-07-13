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
*	@brief 获取完全路径
*	@param path 不完全路径?
*/
LYNIX_API char* getAbsolutePath(const char*);

/*
*	@brief 获取指定行的内容
*	@param path 路径
*	@param 行
*/
LYNIX_API char* getFileLine(const char*, int);

/*
*	@brief 获取指定内容
*	@param path 路径
*	@param 行
*/
LYNIX_API char* readFileAll(const char* path);

#endif // !FILE_H
