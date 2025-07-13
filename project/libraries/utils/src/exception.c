#include <base/exception.h>
#include <base/tool/file.h>
#include <base/tool/stringbuf.h>
#include <base/type/string.h>

Exception* createException(char* path, char* msg, int line, int col) {
    char* abs_path = getAbsolutePath(path);
    if (!abs_path) return NULL;
    char* vol = getFileLine(abs_path, line);
    Exception* tb = (Exception*)malloc(sizeof(Exception));
    if (!tb) {
        free(abs_path);
        free(vol);
        return NULL;
    }
    tb->path = abs_path;
    tb->vol = vol;
    tb->message = msg ? strdup(msg) : NULL;
    tb->line = line;
    tb->column = col;
    tb->child = NULL;

    return tb;
}



int appendExceptionChild(Exception* parent, Exception* child) {
    if (!parent || !child) return -1;
    if (!parent->child) {
        parent->child = child;
        return 0;
    }
    Exception* current = parent->child;
    while (current->child) {
        current = current->child;
    }
    current->child = child;
    return 0;
}


static void renderException_(Exception* tb, char* prefix, int level) {
    if (!tb) return;
    printf("%s¨q©¤> %s:%d:%d\n", prefix, tb->path, tb->line, tb->column < 0 ? 0 : tb->column);
    printf("%s©¦\n", prefix);
    printf("%s©¦   %s\n", prefix, tb->vol ? tb->vol : "(empty)");
    stringbuf* buf = stringbuffer();
    appendStringBuf(buf, prefix);
    appendStringBuf(buf, "©¦   ");
    if (tb->column != -1)
    {
        for (size_t i = 0; i < tb->column; i++)
        {
            appendStringBufChar(buf, ' ');
        }
        appendStringBufChar(buf, '^');
    }
    appendStringBufChar(buf, '\n');
    printf("%s", buf->buffer);
    freeStringBuf(buf);
    if (tb->child)
    {
        stringbuf* c_buf = stringbuffer();
        appendStringBuf(c_buf, "©¦    ");
        for (size_t i = 0; i < level; i++)
        {
            appendStringBuf(c_buf, "©¦    ");
        }
        renderException_(tb->child, c_buf->buffer, level + 1);
    }
    printf("%s¨t©¤ %s\n", prefix, tb->message);
}

void renderException(Exception* tb) {
    if (!tb) return;
    renderException_(tb, "",0);
}

void freeException(Exception* pre) {
    if (!pre) return;
    free(pre->path);
    free(pre->vol);
    free(pre->message);
    freeException(pre->child);
    free(pre);
}