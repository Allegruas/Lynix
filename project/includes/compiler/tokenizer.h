#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <base/type/bool.h>
#include <base/type/string.h>
#include <base/type/wstring.h>
#include <base/exception.h>

#include "location.h"


typedef struct {
	enum {
		TOKENTYPE_EOF = 0,
		TOKENTYPE_STR = 1,
		TOKENTYPE_INT = 2,
		TOKENTYPE_HEX = 3,
		TOKENTYPE_CHAR = 4,
		TOKENTYPE_FLOAT = 5,
		TOKENTYPE_DOUBLE = 6,
		TOKENTYPE_SYMBOL = 7,
		TOKENTYPE_IDENTIFIER = 8
	}type;
	char* value;
	Position pos;
}Token;

typedef struct {
	Token* data;
	int count;
	int capacity;
} Tokens;
typedef struct {
	char** errors;
	int count;
	int capacity;
} Errors;

typedef struct {
	wstring source;
	char* path;
	int index;
	int eof;
	Tokens* tokens;
	Errors* error;
	Position pos;
}Scanner;

 
LYNIX_API Scanner* scanner(wstring, char*);

LYNIX_API Token getToken(Scanner*);

LYNIX_API int scanTokens(Scanner*);

LYNIX_API void renderTokens(Scanner*);

LYNIX_API void freeScanner(Scanner*);
#endif // !TOKENIZER_H
