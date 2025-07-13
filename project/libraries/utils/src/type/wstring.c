#include <base/type/wstring.h>
#include <base/type/string.h>

char wctoc(wchar_t wc) {
	 char buffer[MB_LEN_MAX] = { 0 };
	 mbstate_t state = { 0 };

	 size_t result = wcrtomb(buffer, wc, &state);
	 if (result == (size_t)-1 || result == 0) {
		 return '\0';
	 }
	 return buffer[0];
}

char* wctos(wchar_t wc) {
    char buffer[MB_LEN_MAX] = { 0 };
    mbstate_t state = { 0 };

    size_t result = wcrtomb(buffer, wc, &state);
    if (result == (size_t)-1 || result == 0) {
        return NULL;
    }
    char* str = malloc(result + 1);
    if (!str) return NULL;

    memcpy(str, buffer, result);
    str[result] = '\0';
    return str;
}

char* wcstos(const wchar_t* wstr) {
    if (!wstr) return NULL;
    mbstate_t state = { 0 };
    size_t len = wcsrtombs(NULL, &wstr, 0, &state);
    if (len == (size_t)-1) return NULL;
    char* mbstr = malloc(len + 1);
    if (!mbstr) return NULL;
    wcsrtombs(mbstr, &wstr, len + 1, &state);
    return mbstr;
}