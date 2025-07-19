#ifndef LYSON_PARSER_H
#define LYSON_PARSER_H

#include "../lynix.h"
#include "lyson.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>


typedef enum LysonState {
    LS_START,
    LS_IN_STRING,
    LS_ESCAPE,
    LS_IN_NUMBER,
    LS_IN_TRUE,
    LS_IN_FALSE,
    LS_IN_NULL,
    LS_AFTER_KEY,
    LS_AFTER_VALUE,
    LS_IN_ARRAY,
    LS_IN_OBJECT
} LysonState;

typedef enum LysonEvent {
    EVENT_OBJECT_START,
    EVENT_OBJECT_END,
    EVENT_ARRAY_START,
    EVENT_ARRAY_END,
    EVENT_KEY,
    EVENT_STRING,
    EVENT_NUMBER,
    EVENT_BOOLEAN,
    EVENT_NULL
} LysonEvent;

typedef enum LysonResult {
    PARSE_SUCCESS = 0,
    INVALID_STARTING_CHARACTER = -1,
    INVALID_CHARACTER = -2,
    INVALID_COMMA = -3,
    INVALID_VALUE = -4,
    SHOULD_BE_COLON = -5,
    UNCLOSED_STRUCTURE = -6
} LysonResult;


/**
 * @brief 事件回调函数类型，用于处理解析过程中产生的事件。
 *
 * @param event 事件类型，参见 LysonEvent 枚举。
 * @param value 事件相关的宽字符数据指针（如字符串、数字等），可能为 NULL。
 * @param length 数据长度（字符数），若 value 为 NULL 则为 0。
 * @param userdata 用户自定义数据指针，创建解析流时传入。
 */
typedef void (*LysonEventHandler)(LysonEvent, const wchar_t*, size_t, void*);

typedef struct LysonStream {
    LysonState state;
    LysonState prev;
    int depth;
    int key; 
    int in_value;
    wchar_t* buf;
    size_t size;
    size_t pos;
    LysonEventHandler handler;
    void* userdata;
} LysonStream;


/**
 * @brief 创建一个新的 LysonStream 解析流对象。
 *
 * @param handler 事件回调函数指针，用于处理解析过程中产生的事件。
 * @param userdata 用户自定义数据指针，将在事件回调时传递。
 * @return LysonStream* 新分配的 LysonStream 指针，需调用 FreeLysonStream 释放。
 */
LYNIX_API LysonStream* createLysonStream(LysonEventHandler, void*);

/**
 * @brief 向 LysonStream 解析流输入数据进行解析。
 *
 * @param stream 解析流对象指针。
 * @param input 输入的宽字符数据指针。
 * @param length 输入数据的长度（字符数）。
 * @return int 解析结果，返回 LysonResult 枚举值。
 */
LYNIX_API int LysonStreamGet(LysonStream*, const wchar_t*, size_t);

/**
 * @brief 通知解析流输入已结束，进行收尾处理。
 *
 * @param stream 解析流对象指针。
 * @return int 解析结果，返回 LysonResult 枚举值。
 */
LYNIX_API int LysonStreamFinalize(LysonStream*);

/**
 * @brief 释放由 createLysonStream 创建的解析流对象及其资源。
 *
 * @param stream 解析流对象指针。
 */
LYNIX_API void FreeLysonStream(LysonStream*);

#endif // !LYSON_PARSER_H