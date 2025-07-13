#include <lyson/lyson.h>
#include <string.h>
#include <base/tool/stringbuf.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>




#if defined(_WIN32) && defined(_MSC_VER)
    #define strdup _strdup 
#endif

#if defined(_WIN32) && defined(_MSC_VER)
    #define strcpy(dest,src) strcpy_s(dest,sizeof(dest),src)
#endif


static Lyson* newLyson() {
	Lyson* this = (Lyson*)malloc(sizeof(Lyson));
	if (!this) return NULL;
	memset(this, 0, sizeof(Lyson));
	this->next = NULL;
	this->child = NULL;
	return this;
}

Lyson* createLysonObject() {
	Lyson* this = newLyson();
	if (!this) return NULL;
	this->type = LYSON_OBJECT;
	return this;
}

Lyson* createLysonArray() {
	Lyson* node = newLyson();
	if (!node) return NULL;

	node->type = LYSON_ARRAY;
	return node;
}


static void appendChild(Lyson* parent, Lyson* child) {
	if (!parent || !child) return;

	if (!parent->child) {
		parent->child = child;
	}
	else {
		Lyson* last = parent->child;
		while (last->next) {
			last = last->next;
		}
		last->next = child;
	}
}

Lyson* appendLysonInt(Lyson* parent, char* key, int value) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_INT;
    node->_int = value;

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonStr(Lyson* parent, char* key, char* value) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_STRING;
    if (value) {
        node->_str = strdup(value);
        if (!node->_str) {
            free(node);
            return NULL;
        }
    }
    else {
        node->_str = strdup("");
        if (!node->_str) {
            free(node);
            return NULL;
        }
    }

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node->_str);
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonDouble(Lyson* parent, char* key, double value) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_DOUBLE;
    node->_double = value;

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonNull(Lyson* parent, char* key) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_NULL;

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonTrue(Lyson* parent, char* key) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_BOOL;
    node->_int = 1;

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonFalse(Lyson* parent, char* key) {
    if (!parent || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    Lyson* node = newLyson();
    if (!node) return NULL;

    node->type = LYSON_BOOL;
    node->_int = 0;

    if (parent->type == LYSON_OBJECT && key) {
        node->key = strdup(key);
        if (!node->key) {
            free(node);
            return NULL;
        }
    }

    appendChild(parent, node);
    return node;
}

Lyson* appendLysonObject(Lyson* parent, char* key, Lyson* child) {
    if (!parent || !child || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    if (parent->type == LYSON_OBJECT && key) {
        if (child->key) free(child->key);
        child->key = strdup(key);
        if (!child->key) {
            return NULL;
        }
    }
    else if (parent->type == LYSON_ARRAY) {
        if (child->key) {
            free(child->key);
            child->key = NULL;
        }
    }

    appendChild(parent, child);
    return child;
}

Lyson* appendLysonArray(Lyson* parent, char* key, Lyson* child) {
    if (!parent || !child || (parent->type != LYSON_OBJECT && parent->type != LYSON_ARRAY)) {
        return NULL;
    }

    if (parent->type == LYSON_OBJECT && key) {
        if (child->key) free(child->key);
        child->key = strdup(key);
        if (!child->key) {
            return NULL;
        }
    }
    else if (parent->type == LYSON_ARRAY) {
        if (child->key) {
            free(child->key);
            child->key = NULL;
        }
    }

    appendChild(parent, child);
    return child;
}

void freeLyson(Lyson* node) {
    if (!node) return;

    if (node->child) {
        freeLyson(node->child);
    }

    if (node->next) {
        freeLyson(node->next);
    }

    if (node->key) {
        free(node->key);
    }

    if (node->type == LYSON_STRING && node->_str) {
        free(node->_str);
    }

    free(node);
}

typedef struct jsonbuf {
    char* buffer; 
    size_t length;   
    size_t capacity; 
} jsonbuf;

/**
 * @brief 创建新的字符串缓冲区
 * @return stringbuf* 新创建的缓冲区指针
 */
static jsonbuf* newbuf() {
    jsonbuf* sb = (jsonbuf*)malloc(sizeof(jsonbuf));
    if (!sb) return NULL;
    sb->capacity = 128;
    sb->buffer = (char*)malloc(sb->capacity);
    if (!sb->buffer) {
        free(sb);
        return NULL;
    }

    sb->buffer[0] = '\0';
    sb->length = 0;
    return sb;
}

/**
 * @brief 追加字符串到缓冲区
 * @param buf 字符串缓冲区
 * @param str 要追加的字符串
 */
static void appendStr(jsonbuf* buf, const char* str) {
    if (!buf || !str) return;
    size_t len = strlen(str);
    size_t new_len = buf->length + len;
    if (new_len + 1 > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (new_capacity <= new_len + 1) {
            new_capacity *= 2;
        }

        char* new_buffer = (char*)realloc(buf->buffer, new_capacity);
        if (!new_buffer) return;

        buf->buffer = new_buffer;
        buf->capacity = new_capacity;
    }
    memcpy(buf->buffer + buf->length, str, len + 1);
    buf->length = new_len;
}

/**
 * @brief 追加单个字符到缓冲区
 * @param buf 字符串缓冲区
 * @param c 要追加的字符
 */
static void appendChar(jsonbuf* buf, char c) {
    if (!buf) return;
    if (buf->length + 2 > buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        char* new_buffer = (char*)realloc(buf->buffer, new_capacity);
        if (!new_buffer) return;

        buf->buffer = new_buffer;
        buf->capacity = new_capacity;
    }
    buf->buffer[buf->length] = c;
    buf->buffer[buf->length + 1] = '\0';
    buf->length++;
}

/**
 * @brief 追加缩进到缓冲区
 * @param buf 字符串缓冲区
 * @param level 缩进级别
 */
static void appendIndent(jsonbuf* buf, int level) {
    for (int i = 0; i < level; i++) {
        appendStr(buf, "    ");
    }
}

/**
 * @brief 释放字符串缓冲区并返回字符串
 * @param buf 要释放的缓冲区
 * @return char* 字符串内容（需要调用者释放）
 */
static char* cstr(jsonbuf* buf) {
    if (!buf) return NULL;

    char* result = strdup(buf->buffer);
    free(buf->buffer);
    free(buf);
    return result;
}


/**
 * @brief 将字符串转义为JSON格式
 * @param str 原始字符串
 * @return char* 转义后的字符串（需要调用者释放）
 */
static char* jsonString(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    size_t capacity = len * 6 + 1;
    char* buf = (char*)malloc(capacity);
    if (!buf) return NULL;
    char* ptr = buf;
    while (*str) {
        switch (*str) {
        case '\"': {
            assert(capacity > 8);
            strcpy(ptr, "\\\""); 
            ptr += 2; 
            break;
        }
        case '\\': {
            assert(capacity > 8);
            strcpy(ptr, "\\\\"); 
            ptr += 2; break;
        }
        case '\b': {
            assert(capacity > 8);
            strcpy(ptr, "\\b"); 
            ptr += 2; 
            break;
        }
        case '\f': {
            assert(capacity > 8);
            strcpy(ptr, "\\f"); 
            ptr += 2; 
            break;
        }
        case '\n': {
            assert(capacity > 8);
            strcpy(ptr, "\\n"); 
            ptr += 2; 
            break;
        }
        case '\r': {
            assert(capacity > 8);
            strcpy(ptr, "\\r");
            ptr += 2; 
            break;
        }
        case '\t': {
            assert(capacity > 8);
            strcpy(ptr, "\\t"); 
            ptr += 2; 
            break;
        }
        default:
            if (iscntrl((unsigned char)*str)) {
                #if defined(_WIN32) && defined(_MSC_VER)
                    assert(capacity > 8);
                    sprintf_s(ptr, sizeof(ptr), "\\u%04X", (unsigned char)*str);
                #else
                    sprintf(p,"\\u%04X", (unsigned char)*str);
                #endif
                ptr += 6;
            }
            else {
                *ptr++ = *str;
            }
            break;
        }
        str++;
    }
    *ptr = '\0';
    return buf;
}


static void recursive(Lyson* node, jsonbuf* buffer, int level, int mode) {
    if (!node) return;

    switch (node->type) {
    case LYSON_NULL:
        appendStr(buffer, "null");
        break;

    case LYSON_BOOL:
        appendStr(buffer, node->_int ? "true" : "false");
        break;

    case LYSON_INT: {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", node->_int);
        appendStr(buffer, buf);
        break;
    }

    case LYSON_DOUBLE: {
        char buf[64];
        double abs_val = node->_double < 0 ? -node->_double : node->_double;
        if (abs_val > 1e15 || (abs_val != 0 && abs_val < 1e-15)) {
            snprintf(buf, sizeof(buf), "%.16e", node->_double);
        }
        else {
            snprintf(buf, sizeof(buf), "%.16g", node->_double);
        }
        appendStr(buffer, buf);
        break;
    }

    case LYSON_STRING: {
        char* str = jsonString(node->_str);
        if (str) {
            appendChar(buffer, '"');
            appendStr(buffer, str);
            appendChar(buffer, '"');
            free(str);
        }
        else {
            appendStr(buffer, "\"\"");
        }
        break;
    }
    case LYSON_ARRAY:
        appendChar(buffer, '[');
        if (mode && node->child) appendChar(buffer, '\n');
        for (Lyson* child = node->child; child; child = child->next) {
            if (mode) appendIndent(buffer, level + 1);
            recursive(child, buffer, level + 1, mode);
            if (child->next) appendChar(buffer, ',');
            if (mode) appendChar(buffer, '\n');
        }
        if (mode && node->child) appendIndent(buffer, level);
        appendChar(buffer, ']');
        break;

    case LYSON_OBJECT:
        appendChar(buffer, '{');
        if (mode && node->child) appendChar(buffer, '\n');
        for (Lyson* child = node->child; child; child = child->next) {
            if (mode) appendIndent(buffer, level + 1);
            char* key = jsonString(child->key);
            if (key) {
                appendChar(buffer, '"');
                appendStr(buffer, key);
                appendChar(buffer, '"');
                free(key);
            }
            else {
                appendStr(buffer, "\"\"");
            }
            appendStr(buffer, mode ? ": " : ":");
            recursive(child, buffer, level + 1, mode);

            if (child->next) appendChar(buffer, ',');
            if (mode) appendChar(buffer, '\n');
        }

        if (mode && node->child) appendIndent(buffer, level);
        appendChar(buffer, '}');
        break;
    }
}

char* lysonToString(Lyson* this, int mode) {
    if (!this) return strdup("null");
    jsonbuf* buf = newbuf();
    if (!buf) return NULL;
    if (mode > 0) {
        recursive(this, buf, 0, 1);
    }
    else {
        recursive(this, buf, 0, 0);
    }

    return cstr(buf);
}


