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
 * @brief �¼��ص��������ͣ����ڴ�����������в������¼���
 *
 * @param event �¼����ͣ��μ� LysonEvent ö�١�
 * @param value �¼���صĿ��ַ�����ָ�루���ַ��������ֵȣ�������Ϊ NULL��
 * @param length ���ݳ��ȣ��ַ��������� value Ϊ NULL ��Ϊ 0��
 * @param userdata �û��Զ�������ָ�룬����������ʱ���롣
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
 * @brief ����һ���µ� LysonStream ����������
 *
 * @param handler �¼��ص�����ָ�룬���ڴ�����������в������¼���
 * @param userdata �û��Զ�������ָ�룬�����¼��ص�ʱ���ݡ�
 * @return LysonStream* �·���� LysonStream ָ�룬����� FreeLysonStream �ͷš�
 */
LYNIX_API LysonStream* createLysonStream(LysonEventHandler, void*);

/**
 * @brief �� LysonStream �������������ݽ��н�����
 *
 * @param stream ����������ָ�롣
 * @param input ����Ŀ��ַ�����ָ�롣
 * @param length �������ݵĳ��ȣ��ַ�������
 * @return int ������������� LysonResult ö��ֵ��
 */
LYNIX_API int LysonStreamGet(LysonStream*, const wchar_t*, size_t);

/**
 * @brief ֪ͨ�����������ѽ�����������β����
 *
 * @param stream ����������ָ�롣
 * @return int ������������� LysonResult ö��ֵ��
 */
LYNIX_API int LysonStreamFinalize(LysonStream*);

/**
 * @brief �ͷ��� createLysonStream �����Ľ�������������Դ��
 *
 * @param stream ����������ָ�롣
 */
LYNIX_API void FreeLysonStream(LysonStream*);

#endif // !LYSON_PARSER_H