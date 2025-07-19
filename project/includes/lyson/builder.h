#ifndef LYSON_TREE_BUILDER  
#define LYSON_TREE_BUILDER  

#include "../lynix.h"  
#include <lyson/parser.h>


typedef struct TreeBuilder {  
    Lyson* root;  
    Lyson* current;  
    char* key;  
    Lyson** stack;  
    int size;  
    int capacity;  
} TreeBuilder;  

LYNIX_API TreeBuilder* createTreeBuilder(void);  
LYNIX_API Lyson* getBuiltLyson(TreeBuilder*);  
LYNIX_API void BuilderEventHandler(LysonEvent, const wchar_t*, size_t, void*);  
LYNIX_API void FreeTreeBuilder(TreeBuilder*);  

#endif