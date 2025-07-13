#ifndef STRINGBUF_H
#define STRINGBUF_H

#include <stdlib.h>
#include <string.h>
#include "../../lynix.h"

typedef struct stringbuf {
    char* buffer;    // �ַ���������
    size_t length;   // ��ǰ�ַ������ȣ�������ֹ����
    size_t capacity; // ������������
} stringbuf;

/**
 * @brief �����µ��ַ���������
 * @return StringBuf* �´����Ļ�����ָ��
 */
LYNIX_API stringbuf* stringbuffer();

/**
 * @brief ׷���ַ�����������
 * @param sb �ַ���������
 * @param str Ҫ׷�ӵ��ַ���
 */
LYNIX_API void appendStringBuf(stringbuf*,char*);

/**
 * @brief ׷���ַ���������
 * @param sb �ַ���������
 * @param str Ҫ׷�ӵ��ַ�
 */
LYNIX_API void appendStringBufChar(stringbuf*, char);

/**
 * @brief �ͷ��ַ���������
 * @param sb Ҫ�ͷŵĻ�����
 */
LYNIX_API void freeStringBuf(stringbuf*);

#endif // STRINGBUF_H
