#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <assert.h>
#include "../lynix.h"


typedef struct Exception
{
	char* path;
	char* vol; 
	char* message;
	int line;
	int column;

	struct Exception* child;
}Exception;

#define notNull(ptr) if (!(ptr)) { renderException(createException(__FILE__,"NullpointerError: Inner Nullpointer",__LINE__ - 1,-1)); exit(-1);  } assert(ptr != NULL);


/**
 * @brief 创建Exception
 * @param path 路径
 * @param msg 错误信息
 * @param line 行号
 * @param column 列号
 * @return Exception*
 */
LYNIX_API Exception* createException(char*, char*, int, int);

/**
 * @brief 添加子项目
 * @param parent 父
 * @param child 子
 * @return int
 */
LYNIX_API int appendExceptionChild(Exception*, Exception*);

/**
 * @brief 显示Exception
 * @param target 目标Exception
 * @return int
 */
LYNIX_API void renderException(Exception*);

/**
 * @brief 释放Exception
 * @param pre
 */
LYNIX_API void freeException(Exception*);

#endif // !EXCEPTION_H
