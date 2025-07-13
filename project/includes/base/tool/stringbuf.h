#ifndef STRINGBUF_H
#define STRINGBUF_H

#include <stdlib.h>
#include <string.h>
#include "../../lynix.h"

typedef struct stringbuf {
    char* buffer;    // 字符串缓冲区
    size_t length;   // 当前字符串长度（不含终止符）
    size_t capacity; // 缓冲区总容量
} stringbuf;

/**
 * @brief 创建新的字符串缓冲区
 * @return StringBuf* 新创建的缓冲区指针
 */
LYNIX_API stringbuf* stringbuffer();

/**
 * @brief 追加字符串到缓冲区
 * @param sb 字符串缓冲区
 * @param str 要追加的字符串
 */
LYNIX_API void appendStringBuf(stringbuf*,char*);

/**
 * @brief 追加字符到缓冲区
 * @param sb 字符串缓冲区
 * @param str 要追加的字符
 */
LYNIX_API void appendStringBufChar(stringbuf*, char);

/**
 * @brief 释放字符串缓冲区
 * @param sb 要释放的缓冲区
 */
LYNIX_API void freeStringBuf(stringbuf*);

#endif // STRINGBUF_H
