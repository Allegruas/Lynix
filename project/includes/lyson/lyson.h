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
 * @brief 创建一个新的 JSON 对象节点
 * @return Lyson* 新创建的对象节点指针
 */
LYNIX_API Lyson* createLysonObject();

/**
 * @brief 创建一个新的 JSON 数组节点
 * @return Lyson* 新创建的数组节点指针
 */
LYNIX_API Lyson* createLysonArray();


/**
 * @brief 向 JSON 节点添加整数值成员
 * @param parent 父节点（对象或数组）
 * @param key 成员键名（数组可传 NULL）
 * @param value 整数值
 * @return Lyson* 新创建的值节点指针
 */
LYNIX_API Lyson* appendLysonInt(Lyson*, char*, int);

/**
 * @brief 向 JSON 数组添加整数值成员
 * @param parent 数组节点
 * @param value 整数值
 * @return Lyson* 新创建的值节点指针
 */
#define appendLysonArrayInt(parent, value) appendLysonInt(parent, NULL, value)

 /**
  * @brief 向 JSON 节点添加字符串值成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @param value 字符串值
  * @return Lyson* 新创建的值节点指针
  */
LYNIX_API Lyson* appendLysonStr(Lyson*, char*, char*);

/**
 * @brief 向 JSON 数组添加字符串值成员
 * @param parent 数组节点
 * @param value 字符串值
 * @return Lyson* 新创建的值节点指针
 */
#define appendLysonArrayStr(parent, value) appendLysonStr(parent, NULL, value)

 /**
  * @brief 向 JSON 节点添加浮点值成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @param value 浮点数值
  * @return Lyson* 新创建的值节点指针
  */
LYNIX_API Lyson* appendLysonDouble(Lyson*, char*, double);

/**
 * @brief 向 JSON 数组添加浮点值成员
 * @param parent 数组节点
 * @param value 浮点数值
 * @return Lyson* 新创建的值节点指针
 */
#define appendLysonArrayDouble(parent, value) appendLysonDouble(parent, NULL, value)

 /**
  * @brief 向 JSON 节点添加 null 值成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @return Lyson* 新创建的 null 节点指针
  */
LYNIX_API Lyson* appendLysonNull(Lyson*, char*);

/**
 * @brief 向 JSON 数组添加 null 值成员
 * @param parent 数组节点
 * @return Lyson* 新创建的 null 节点指针
 */
#define appendLysonArrayNull(parent) appendLysonNull(parent, NULL)

 /**
  * @brief 向 JSON 节点添加 true 值成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @return Lyson* 新创建的布尔节点指针
  */
LYNIX_API Lyson* appendLysonTrue(Lyson*, char*);

/**
 * @brief 向 JSON 数组添加 true 值成员
 * @param parent 数组节点
 * @return Lyson* 新创建的布尔节点指针
 */
#define appendLysonArrayTrue(parent) appendLysonTrue(parent, NULL)

 /**
  * @brief 向 JSON 节点添加 false 值成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @return Lyson* 新创建的布尔节点指针
  */
LYNIX_API Lyson* appendLysonFalse(Lyson*, char*);

/**
 * @brief 向 JSON 数组添加 false 值成员
 * @param parent 数组节点
 * @return Lyson* 新创建的布尔节点指针
 */
#define appendLysonArrayFalse(parent) appendLysonFalse(parent, NULL)


 /**
  * @brief 向 JSON 节点添加子对象成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @param child 要添加的子对象
  * @return Lyson* 添加的子对象指针
  */
LYNIX_API Lyson* appendLysonObject(Lyson*, char*,Lyson*);

/**
 * @brief 向 JSON 数组添加子对象成员
 * @param parent 数组节点
 * @param child 要添加的子对象
 * @return Lyson* 添加的子对象指针
 */
#define appendLysonArrayObject(parent, child) appendLysonObject(parent, NULL,child)

 /**
  * @brief 向 JSON 节点添加子数组成员
  * @param parent 父节点（对象或数组）
  * @param key 成员键名（数组可传 NULL）
  * @param child 要添加的子数组
  * @return Lyson* 添加的子数组指针
  */
LYNIX_API Lyson* appendLysonArray(Lyson*, char*, Lyson*);

/**
 * @brief 向 JSON 数组添加子数组成员
 * @param parent 数组节点
 * @param child 要添加的子数组
 * @return Lyson* 添加的子数组指针
 */
#define appendLysonArrayArray(parent, child) appendLysonArray(parent, NULL,child)

/**
  * @brief 释放 Lyson 结构
  * @param node 要释放的节点
*/
LYNIX_API void freeLyson(Lyson*);

/**
 * @brief 将Lson结构序列化为JSON字符串
 * @param this 根节点
 * @param mode 输出模式 (0=紧凑, 1=格式化)
 * @return char* JSON字符串
 */
LYNIX_API char* lysonToString(Lyson*,int);



#endif // !LYSON_H
