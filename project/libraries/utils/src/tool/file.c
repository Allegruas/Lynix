#include <base/tool/file.h>
#include <base/type/string.h>

char* getAbsolutePath(const char* path) {
    char* resolved_path = NULL;
#ifdef _WIN32
    resolved_path = (char*)malloc(MAX_PATH);
    if (resolved_path) {
        DWORD ret = GetFullPathNameA(path, MAX_PATH, resolved_path, NULL);
        if (ret == 0 || ret > MAX_PATH) {
            free(resolved_path);
            resolved_path = NULL;
        }
    }
#else
    resolved_path = realpath(path, NULL);
#endif
    return resolved_path;
}


char* getFileLine(const char* path, int line) {
    FILE* fp = fopen(path, "r");
    if (!fp) return NULL;

    char* value = NULL;
    char buf[1024];
    int current = 1;

    while (fgets(buf, sizeof(buf), fp)) {
        if (current == line) {
            size_t len = strlen(buf);
            if (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
                buf[len - 1] = '\0';
            }
            if (len > 1 && buf[len - 2] == '\r') {
                buf[len - 2] = '\0';
            }
            value = strdup(buf);
            break;
        }
        current++;
    }

    fclose(fp);
    return value;
}

char* readFileAll(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        return NULL;
    }

    rewind(file);
    char* content = (char*)malloc(size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    size_t bytes_read = fread(content, 1, size, file);
    if (bytes_read != (size_t)size) {
        char* temp = realloc(content, bytes_read + 1);
        if (!temp) {
            free(content);
            fclose(file);
            return NULL;
        }
        content = temp;
    }

    content[bytes_read] = '\0'; 
    fclose(file);
    return content;
}