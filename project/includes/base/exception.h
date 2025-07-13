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
 * @brief ����Exception
 * @param path ·��
 * @param msg ������Ϣ
 * @param line �к�
 * @param column �к�
 * @return Exception* h
 */
LYNIX_API Exception* createException(char*, char*, int, int);

/**
 * @brief Add Exception`s child
 * @param parent
 * @param child
 * @return int
 */
LYNIX_API int appendExceptionChild(Exception*, Exception*);

/**
 * @brief render Exception
 * @param target
 * @return int
 */
LYNIX_API void renderException(Exception*);

/**
 * @brief free Exception
 * @param pre
 */
LYNIX_API void freeException(Exception*);

#endif // !EXCEPTION_H
