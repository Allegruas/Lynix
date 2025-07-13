#ifndef LYNIX_TREE_H
#define LYNIX_TREE_H

#include <base/type/string.h>
#include "../../lynix.h"

typedef struct Tree
{
	int depth;
	char* value;
	struct Tree* child;
	struct Tree* next;
}Tree;

LYNIX_API Tree* createTree(char*);
LYNIX_API void appendChild(Tree*,Tree*);
LYNIX_API void appendNext(Tree*,Tree*);
LYNIX_API Tree* insertChild(Tree*, char*, int);
LYNIX_API Tree* insertNext(Tree*, char*, int);
LYNIX_API char* TreeToString(Tree*);
LYNIX_API void renderTree(Tree*);
#endif