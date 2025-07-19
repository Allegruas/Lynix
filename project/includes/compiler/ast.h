#ifndef AST_H
#define AST_H
#include "location.h"

typedef struct AST
{
	enum
	{
		AST_PROGRAM = 0
	}type;
	Location range;
}AST;

typedef struct {
	AST base;
	char* path;
	AST* value;
}ProgramAST;



#endif // !AST_H
