#include <base/type/wstring.h>
#include <base/type/string.h>
#include <limits.h>
#define InitMB(s) memset(&(s), 0, sizeof(mbstate_t))

char wctoc(wchar_t wc) {
    char buffer[MB_LEN_MAX] = { 0 };

#ifdef _WIN32
    size_t result;
    if (wcrtomb_s(&result, buffer, MB_LEN_MAX, wc, NULL) != 0) {
        return '\0';
    }
#else
    mbstate_t state;
    INIT_MBSTATE(state);
    size_t result = wcrtomb(buffer, wc, &state);
    if (result == (size_t)-1 || result == 0) {
        return '\0';
    }
#endif
    return buffer[0];
}


char* wctos(wchar_t wc) {
    char buffer[MB_LEN_MAX] = { 0 };
    size_t result = 0;

#ifdef _WIN32
    if (wcrtomb_s(&result, buffer, MB_LEN_MAX, wc, NULL) != 0) {
        return NULL;
    }
#else
    mbstate_t state;
    INIT_MBSTATE(state);
    result = wcrtomb(buffer, wc, &state);
    if (result == (size_t)-1 || result == 0) {
        return NULL;
    }
#endif
    char* str = malloc(result + 1);
    if (!str) return NULL;

    memcpy(str, buffer, result);
    str[result] = '\0';
    return str;
}

char* wcstos(wchar_t* wstr) {
    if (!wstr) return NULL;
#ifdef _WIN32
    const wchar_t* src = wstr;
    size_t len = 0;
    if (wcsrtombs_s(&len, NULL, 0, &src, 0, NULL) != 0) {
        return NULL;
    }
    char* mbstr = malloc(len);
    if (!mbstr) return NULL;
    src = wstr;
    if (wcsrtombs_s(&len, mbstr, len, &src, _TRUNCATE, NULL) != 0) {
        free(mbstr);
        return NULL;
    }
    return mbstr;

#else
    mbstate_t state;
    INIT_MBSTATE(state);
    const wchar_t* src = wstr;
    size_t byteCount = wcsrtombs(NULL, &src, 0, &state);
    if (byteCount == (size_t)-1) {
        return NULL;
    }
    INIT_MBSTATE(state);
    src = wstr;
    char* mbstr = malloc(byteCount + 1);
    if (!mbstr) return NULL;
    size_t converted = wcsrtombs(mbstr, &src, byteCount + 1, &state);
    if (converted == (size_t)-1) {
        free(mbstr);
        return NULL;
    }
    return mbstr;
#endif
}