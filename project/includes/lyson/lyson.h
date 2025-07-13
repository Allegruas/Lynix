#ifndef LYSON_H
#define LYSON_H

#include "../lynix.h"



/*
* @brief LysonObject
*/
typedef struct Lyson {
	enum
	{
		LYSON_NULL = 0,
		LYSON_BOOL = 1,
		LYSON_INT = 3,
		LYSON_DOUBLE = 4,
		LYSON_STRING = 5,
		LYSON_ARRAY = 6,
		LYSON_OBJECT = 7
	}type;
	char* key;
	
	int _int;
	char* _str;
	double _double;

	struct Lyson* next;
	struct Lyson* child;
}Lyson;

/**
 * @brief ����һ���µ� JSON ����ڵ�
 * @return Lyson* �´����Ķ���ڵ�ָ��
 */
LYNIX_API Lyson* createLysonObject();

/**
 * @brief ����һ���µ� JSON ����ڵ�
 * @return Lyson* �´���������ڵ�ָ��
 */
LYNIX_API Lyson* createLysonArray();


/**
 * @brief �� JSON �ڵ��������ֵ��Ա
 * @param parent ���ڵ㣨��������飩
 * @param key ��Ա����������ɴ� NULL��
 * @param value ����ֵ
 * @return Lyson* �´�����ֵ�ڵ�ָ��
 */
LYNIX_API Lyson* appendLysonInt(Lyson*, char*, int);

/**
 * @brief �� JSON �����������ֵ��Ա
 * @param parent ����ڵ�
 * @param value ����ֵ
 * @return Lyson* �´�����ֵ�ڵ�ָ��
 */
#define appendLysonArrayInt(parent, value) appendLysonInt(parent, NULL, value)

 /**
  * @brief �� JSON �ڵ�����ַ���ֵ��Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @param value �ַ���ֵ
  * @return Lyson* �´�����ֵ�ڵ�ָ��
  */
LYNIX_API Lyson* appendLysonStr(Lyson*, char*, char*);

/**
 * @brief �� JSON ��������ַ���ֵ��Ա
 * @param parent ����ڵ�
 * @param value �ַ���ֵ
 * @return Lyson* �´�����ֵ�ڵ�ָ��
 */
#define appendLysonArrayStr(parent, value) appendLysonStr(parent, NULL, value)

 /**
  * @brief �� JSON �ڵ���Ӹ���ֵ��Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @param value ������ֵ
  * @return Lyson* �´�����ֵ�ڵ�ָ��
  */
LYNIX_API Lyson* appendLysonDouble(Lyson*, char*, double);

/**
 * @brief �� JSON ������Ӹ���ֵ��Ա
 * @param parent ����ڵ�
 * @param value ������ֵ
 * @return Lyson* �´�����ֵ�ڵ�ָ��
 */
#define appendLysonArrayDouble(parent, value) appendLysonDouble(parent, NULL, value)

 /**
  * @brief �� JSON �ڵ���� null ֵ��Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @return Lyson* �´����� null �ڵ�ָ��
  */
LYNIX_API Lyson* appendLysonNull(Lyson*, char*);

/**
 * @brief �� JSON ������� null ֵ��Ա
 * @param parent ����ڵ�
 * @return Lyson* �´����� null �ڵ�ָ��
 */
#define appendLysonArrayNull(parent) appendLysonNull(parent, NULL)

 /**
  * @brief �� JSON �ڵ���� true ֵ��Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @return Lyson* �´����Ĳ����ڵ�ָ��
  */
LYNIX_API Lyson* appendLysonTrue(Lyson*, char*);

/**
 * @brief �� JSON ������� true ֵ��Ա
 * @param parent ����ڵ�
 * @return Lyson* �´����Ĳ����ڵ�ָ��
 */
#define appendLysonArrayTrue(parent) appendLysonTrue(parent, NULL)

 /**
  * @brief �� JSON �ڵ���� false ֵ��Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @return Lyson* �´����Ĳ����ڵ�ָ��
  */
LYNIX_API Lyson* appendLysonFalse(Lyson*, char*);

/**
 * @brief �� JSON ������� false ֵ��Ա
 * @param parent ����ڵ�
 * @return Lyson* �´����Ĳ����ڵ�ָ��
 */
#define appendLysonArrayFalse(parent) appendLysonFalse(parent, NULL)


 /**
  * @brief �� JSON �ڵ�����Ӷ����Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @param child Ҫ��ӵ��Ӷ���
  * @return Lyson* ��ӵ��Ӷ���ָ��
  */
LYNIX_API Lyson* appendLysonObject(Lyson*, char*,Lyson*);

/**
 * @brief �� JSON ��������Ӷ����Ա
 * @param parent ����ڵ�
 * @param child Ҫ��ӵ��Ӷ���
 * @return Lyson* ��ӵ��Ӷ���ָ��
 */
#define appendLysonArrayObject(parent, child) appendLysonObject(parent, NULL,child)

 /**
  * @brief �� JSON �ڵ�����������Ա
  * @param parent ���ڵ㣨��������飩
  * @param key ��Ա����������ɴ� NULL��
  * @param child Ҫ��ӵ�������
  * @return Lyson* ��ӵ�������ָ��
  */
LYNIX_API Lyson* appendLysonArray(Lyson*, char*, Lyson*);

/**
 * @brief �� JSON ��������������Ա
 * @param parent ����ڵ�
 * @param child Ҫ��ӵ�������
 * @return Lyson* ��ӵ�������ָ��
 */
#define appendLysonArrayArray(parent, child) appendLysonArray(parent, NULL,child)

/**
  * @brief �ͷ� Lyson �ṹ
  * @param node Ҫ�ͷŵĽڵ�
*/
LYNIX_API void freeLyson(Lyson*);

/**
 * @brief ��Lson�ṹ���л�ΪJSON�ַ���
 * @param this ���ڵ�
 * @param mode ���ģʽ (0=����, 1=��ʽ��)
 * @return char* JSON�ַ���
 */
LYNIX_API char* lysonToString(Lyson*,int);



#endif // !LYSON_H
