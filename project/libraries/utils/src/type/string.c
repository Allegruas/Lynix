#include <base/type/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#endif

wchar_t* strtow(char* str) {
    if (str == NULL || *str == '\0') {
        return NULL;
    }
#ifdef _WIN32
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if (size == 0) {
        fprintf(stderr, "MultiByteToWideChar failed: %lu\n", GetLastError());
        return NULL;
    }

    wchar_t* wstr = (wchar_t*)malloc(size * sizeof(wchar_t));
    if (wstr == NULL) {
        perror("malloc failed");
        return NULL;
    }

    if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size) == 0) {
        free(wstr);
        fprintf(stderr, "MultiByteToWideChar conversion failed: %lu\n", GetLastError());
        return NULL;
    }
    return wstr;
#else
    size_t len = mbstowcs(NULL, str, 0);
    if (len == (size_t)-1) {
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "mbstowcs failed: %s", strerror(errno));
        fprintf(stderr, "%s\n", error_msg);
        return NULL;
    }

    wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (wstr == NULL) {
        perror("malloc failed");
        return NULL;
    }

    if (mbstowcs(wstr, str, len + 1) == (size_t)-1) {
        free(wstr);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), "mbstowcs conversion failed: %s", strerror(errno));
        fprintf(stderr, "%s\n", error_msg);
        return NULL;
    }

    return wstr;
#endif
}
