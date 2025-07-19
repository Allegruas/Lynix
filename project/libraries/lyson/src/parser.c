#include <lyson/parser.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>


LysonStream* createLysonStream(LysonEventHandler handler, void* userdata) {
    LysonStream* this = malloc(sizeof(LysonStream));
    if (!this) return NULL;

    this->state = LS_START;
    this->prev = LS_START;
    this->depth = 0;
    this->key = 0;
    this->in_value = 0;
    this->size = 256;
    this->buf = malloc(this->size * sizeof(wchar_t));
    this->pos = 0;
    this->handler = handler;
    this->userdata = userdata;

    if (!this->buf) {
        free(this);
        return NULL;
    }

    setlocale(LC_ALL, "en_US.UTF-8");
    return this;
}

void FreeLysonStream(LysonStream* this) {
    if (this) {
        if (this->buf) free(this->buf);
        free(this);
    }
}

#define restBuf(this) (this)->pos = 0

void appendBuf(LysonStream* this, wchar_t c) {
    if (this->pos >= this->size - 1) {
        this->size *= 2;
        wchar_t* new_buf = realloc(this->buf, this->size * sizeof(wchar_t));
        if (!new_buf) {
            return;
        }
        this->buf = new_buf;
    }
    this->buf[this->pos++] = c;
    this->buf[this->pos] = L'\0';
}

void trigger(LysonStream* this, LysonEvent event) {
    if (this->handler) {
        this->handler(event, this->buf, this->pos, this->userdata);
    }
    restBuf(this);
}

int LysonStreamGet(LysonStream* this, const wchar_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        wchar_t c = data[i];

        if (iswspace(c)) {
            continue;
        }

        switch (this->state) {
        case LS_START: {
            if (c == L'{') {
                this->depth++;
                this->state = LS_IN_OBJECT;
                trigger(this, EVENT_OBJECT_START);
            }
            else if (c == L'[') {
                this->depth++;
                this->state = LS_IN_ARRAY;
                trigger(this, EVENT_ARRAY_START);
            }
            else if (!iswspace(c)) {
                return INVALID_STARTING_CHARACTER;
            }
            break;
        }

        case LS_IN_OBJECT: {
            if (c == L'"') {
                this->state = LS_IN_STRING;
                if (!this->in_value)
                {
                    this->key = 1;
                }
                restBuf(this);
            }
            else if (c == L'[' || c == L'{')
            {
                this->state = LS_START;
                if (c == L'{')
                {
                    this->in_value = 0;
                }
                i--;
                restBuf(this);
            }
            else if (c == L'}') {
                if (this->depth <= 0) {
                    return UNCLOSED_STRUCTURE;
                }
                this->depth--;
                trigger(this, EVENT_OBJECT_END);
                this->state = LS_AFTER_VALUE;
            }
            else if (!iswspace(c)) {
                return INVALID_CHARACTER;
            }
            break;
        }

        case LS_IN_ARRAY: {
            if (c == L']') {
                if (this->depth <= 0) {
                    return UNCLOSED_STRUCTURE;
                }
                this->depth--;
                trigger(this, EVENT_ARRAY_END);
                this->state = LS_AFTER_VALUE;
            }
            else if (c == L'"') {
                this->state = LS_IN_STRING;
                this->key = 0;
                restBuf(this);
            }
            else if (c == L'{') {
                this->depth++;
                this->state = LS_IN_OBJECT;
                trigger(this, EVENT_OBJECT_START);
            }
            else if (c == L'[') {
                this->depth++;
                this->state = LS_IN_ARRAY;
                trigger(this, EVENT_ARRAY_START);
            }
            else if (iswdigit(c) || c == L'-') {
                this->state = LS_IN_NUMBER;
                restBuf(this);
                appendBuf(this, c);
            }
            else if (c == L't') {
                this->state = LS_IN_TRUE;
                restBuf(this);
                appendBuf(this, c);
            }
            else if (c == L'f') {
                this->state = LS_IN_FALSE;
                restBuf(this);
                appendBuf(this, c);
            }
            else if (c == L'n') {
                this->state = LS_IN_NULL;
                restBuf(this);
                appendBuf(this, c);
            }
            else if (!iswspace(c)) {
                return INVALID_CHARACTER;
            }
            break;
        }

        case LS_IN_STRING: {
            if (c == L'\\') {
                this->prev = LS_IN_STRING;
                this->state = LS_ESCAPE;
            }
            else if (c == L'"') {
                if (this->key) {
                    trigger(this, EVENT_KEY);
                    this->state = LS_AFTER_KEY;
                }
                else {
                    trigger(this, EVENT_STRING);
                    this->state = LS_AFTER_VALUE;
                }
            }
            else {
                appendBuf(this, c);
            }
            break;
        }

        case LS_ESCAPE: {
            switch (c) {
            case L'n': appendBuf(this, L'\n'); break;
            case L't': appendBuf(this, L'\t'); break;
            case L'r': appendBuf(this, L'\r'); break;
            case L'b': appendBuf(this, L'\b'); break;
            case L'f': appendBuf(this, L'\f'); break;
            case L'u':
                break;
            default:
                appendBuf(this, c);
                break;
            }
            this->state = this->prev;
            break;
        }

        case LS_AFTER_KEY: {
            if (c == L':') {
                this->state = LS_IN_OBJECT;
                this->in_value = 1;
                this->key = 0;
            }
            else if (!iswspace(c)) {
                return SHOULD_BE_COLON;
            }
            break;
        }

        case LS_AFTER_VALUE: {
            if (c == L',') {
                if (this->depth > 0) {
                    if (this->prev == LS_IN_ARRAY) {
                        this->state = LS_IN_ARRAY;
                    }
                    else {
                        this->state = LS_IN_OBJECT;
                    }
                }
                else {
                    return INVALID_COMMA;
                }
            }
            else if (c == L'}') {
                if (this->depth <= 0) {
                    return UNCLOSED_STRUCTURE;
                }
                this->depth--;
                trigger(this, EVENT_OBJECT_END);
            }
            else if (c == L']') {
                if (this->depth <= 0) {
                    return UNCLOSED_STRUCTURE;
                }
                this->depth--;
                trigger(this, EVENT_ARRAY_END);
            }
            else if (!iswspace(c)) {
                return INVALID_CHARACTER;
            }
            this->in_value = 0;
            break;
        }

        case LS_IN_NUMBER: {
            if (iswdigit(c) || c == L'.' || c == L'e' || c == L'E' || c == L'+' || c == L'-') {
                appendBuf(this, c);
            }
            else {
                trigger(this, EVENT_NUMBER);
                this->state = LS_AFTER_VALUE;
                i--; 
            }
            break;
        }

        case LS_IN_TRUE: {
            appendBuf(this, c);
            if (this->pos == 4) {
                if (wcscmp(this->buf, L"true") == 0) {
                    trigger(this, EVENT_BOOLEAN);
                    this->state = LS_AFTER_VALUE;
                }
                else {
                    return INVALID_VALUE;
                }
            }
            break;
        }

        case LS_IN_FALSE: {
            appendBuf(this, c);
            if (this->pos == 5) {
                if (wcscmp(this->buf, L"false") == 0) {
                    trigger(this, EVENT_BOOLEAN);
                    this->state = LS_AFTER_VALUE;
                }
                else {
                    return INVALID_VALUE;
                }
            }
            break;
        }

        case LS_IN_NULL: {
            appendBuf(this, c);
            if (this->pos == 4) {
                if (wcscmp(this->buf, L"null") == 0) {
                    trigger(this, EVENT_NULL);
                    this->state = LS_AFTER_VALUE;
                }
                else {
                    return INVALID_VALUE;
                }
            }
            break;
        }
        }
    }

    return PARSE_SUCCESS;
}

int LysonStreamFinalize(LysonStream* this) {
    if (this->depth != 0) {
        return UNCLOSED_STRUCTURE;
    }
    return PARSE_SUCCESS;
}

#undef restBuf