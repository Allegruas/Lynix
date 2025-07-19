#include <base/type/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
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

    int result = MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size);
    if (result == 0) {
        free(wstr);
        fprintf(stderr, "MultiByteToWideChar conversion failed: %lu\n", GetLastError());
        return NULL;
    }

    return wstr;

#else
    char* saved_locale = setlocale(LC_CTYPE, NULL);
    if (saved_locale) {
        saved_locale = strdup(saved_locale);
    }
    if (!setlocale(LC_CTYPE, "en_US.UTF-8")) {
        if (!setlocale(LC_CTYPE, "C.UTF-8") &&
            !setlocale(LC_CTYPE, "UTF-8")) {
            fprintf(stderr, "Failed to set UTF-8 locale\n");
            if (saved_locale) free(saved_locale);
            return NULL;
        }
    }

    size_t len = mbstowcs(NULL, str, 0);
    if (len == (size_t)-1) {
        fprintf(stderr, "mbstowcs failed: %s\n", strerror(errno));
        if (saved_locale) {
            setlocale(LC_CTYPE, saved_locale);
            free(saved_locale);
        }
        return NULL;
    }

    wchar_t* wstr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
    if (wstr == NULL) {
        perror("malloc failed");
        if (saved_locale) {
            setlocale(LC_CTYPE, saved_locale);
            free(saved_locale);
        }
        return NULL;
    }

    size_t converted = mbstowcs(wstr, str, len + 1);
    if (converted == (size_t)-1) {
        free(wstr);
        fprintf(stderr, "mbstowcs conversion failed: %s\n", strerror(errno));
        if (saved_locale) {
            setlocale(LC_CTYPE, saved_locale);
            free(saved_locale);
        }
        return NULL;
    }
    if (saved_locale) {
        setlocale(LC_CTYPE, saved_locale);
        free(saved_locale);
    }

    return wstr;
#endif
}