#include <base/tool/stringbuf.h>


stringbuf* stringbuffer() {
    stringbuf* sb = (stringbuf*)malloc(sizeof(stringbuf));
    if (!sb) return NULL;
    sb->capacity = 64; 
    sb->buffer = (char*)malloc(sb->capacity);
    if (!sb->buffer) {
        free(sb);
        return NULL;
    }

    sb->buffer[0] = '\0';
    sb->length = 0;
    return sb;
}

void appendStringBuf(stringbuf* sb, char* str) {
    size_t len = strlen(str);
    size_t new_len = sb->length + len;
    if (new_len + 1 > sb->capacity) {
        size_t new_capacity = sb->capacity * 2;
        while (new_capacity <= new_len + 1) {
            new_capacity *= 2;
        }

        char* new_buffer = (char*)realloc(sb->buffer, new_capacity);
        if (!new_buffer) return;

        sb->buffer = new_buffer;
        sb->capacity = new_capacity;
    }
    memcpy(sb->buffer + sb->length, str, len + 1);
    sb->length = new_len;
}
void appendStringBufChar(stringbuf* sb, char c) {
    if (!sb) return;
    if (sb->length + 2 > sb->capacity) {
        size_t new_capacity = sb->capacity * 2;
        char* new_buffer = (char*)realloc(sb->buffer, new_capacity);
        if (!new_buffer) return;

        sb->buffer = new_buffer;
        sb->capacity = new_capacity;
    }
    sb->buffer[sb->length] = c;
    sb->buffer[sb->length + 1] = '\0';
    sb->length++;
}
void freeStringBuf(stringbuf* sb) {
    if (sb) {
        free(sb->buffer);
        free(sb);
    }
}