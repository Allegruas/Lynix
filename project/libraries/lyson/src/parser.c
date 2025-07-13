#include <lyson/parser.h>
#include <malloc.h>
#include <string.h>
typedef struct {
	char* json;
	int index;
}Parser;

#if defined(_WIN32) && defined(_MSC_VER)
    #define strdup _strdup 
#endif

#if defined(_WIN32) && defined(_MSC_VER)
    #define strcpy(dest,src) strcpy_s(dest,sizeof(dest),src)
#endif


Lyson* parseLyson(const char* json) {
	Parser* this = (Parser*)malloc(sizeof(Parser));
	this = &(Parser) {
			.json = strdup(json),
			.index = 0
	};

}