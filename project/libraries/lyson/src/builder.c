#include <lyson/builder.h>
#include <wchar.h>

static char* wcstos(const wchar_t* wstr, size_t len) {
    if (!wstr || len == 0) return NULL;
    mbstate_t state = { 0 };
#ifdef _WIN32  
    size_t length;
    wcsrtombs_s(&length, NULL, 0, &wstr, 0, &state);
#else  
    size_t length = wcsrtombs(NULL, &wstr, 0, &state);
#endif  
    if (length == (size_t)-1) return NULL;
    char* cstr = malloc(length + 1);
    if (!cstr) return NULL;
    wcsrtombs(cstr, &wstr, length + 1, &state);
    return cstr;
}

TreeBuilder* createTreeBuilder(void) {
    TreeBuilder* this = malloc(sizeof(TreeBuilder));
    if (!this) return NULL;
    this->root = NULL;
    this->current = NULL;
    this->key = NULL;
    this->stack = NULL;
    this->size = 0;
    this->capacity = 0;
    return this;
}

Lyson* getBuiltLyson(TreeBuilder* this) {
    if (!this) return NULL;
    return this->root;
}

void FreeTreeBuilder(TreeBuilder* this) {
    if (this) {
        if (this->key) free(this->key);
        if (this->stack) free(this->stack);
        free(this);
    }
}

static void push(TreeBuilder* this, Lyson* node) {
    if (!this || !node) return;

    if (this->size >= this->capacity) {
        int capacity = this->capacity == 0 ? 8 : this->capacity * 2;
        Lyson** stack = realloc(this->stack, capacity * sizeof(Lyson*));
        if (!stack) return;

        this->stack = stack;
        this->capacity = capacity;
    }

    this->stack[this->size++] = node;
}

static Lyson* pop(TreeBuilder* this) {
    if (!this || this->size == 0) return NULL;
    return this->stack[--this->size];
}

static void setKey(TreeBuilder* this, const wchar_t* key, size_t len) {
    if (!this) return;
    if (this->key) {
        free(this->key);
        this->key = NULL;
    }
    if (!key || len == 0) return;
    this->key = wcstos(key, len);
}

static void appendCurrent(TreeBuilder* this, Lyson* node) {
    if (!this || !node) return;
    if (!this->current) {
        this->root = node;
        this->current = node;
        return;
    }
    if (this->current->type == LYSON_OBJECT) {
        node->key = this->key;
        this->key = NULL; 
        if (!this->current->child) {
            this->current->child = node;
        }
        else {
            Lyson* last = this->current->child;
            while (last->next) {
                last = last->next;
            }
            last->next = node;
        }
    }
    else if (this->current->type == LYSON_ARRAY) {
        if (!this->current->child) {
            this->current->child = node;
        }
        else {
            Lyson* last = this->current->child;
            while (last->next) {
                last = last->next;
            }
            last->next = node;
        }
    }
}

void BuilderEventHandler(LysonEvent event, const wchar_t* data, size_t len, void* userdata) {
    TreeBuilder* this = (TreeBuilder*)userdata;
    if (!this) return;
    switch (event) {
    case EVENT_OBJECT_START: {
        Lyson* obj = createLysonObject();
        if (!obj) return;
        if (!this->root) {
            this->root = obj;
        }
        else {
            appendCurrent(this, obj);
        }
        push(this, this->current);
        this->current = obj;
        break;
    }

    case EVENT_ARRAY_START: {
        Lyson* array = createLysonArray();
        if (!array) return;
        if (!this->root) {
            this->root = array;
        }
        else {
            appendCurrent(this, array);
        }
        push(this, this->current);
        this->current = array;
        break;
    }

    case EVENT_KEY: {
        setKey(this, data, len);
        break;
    }

    case EVENT_STRING: {
        char* str = wcstos(data, len);
        if (str) {
            appendLysonStr(this->current,this->key, str);
            free(str);
        }
        break;
    }

    case EVENT_NUMBER: {
        wchar_t* endptr;
        long long integer = wcstoll(data, &endptr, 10);
        if (endptr == data + len) {
            appendLysonInt(this->current, this->key, (int)integer);
        }
        else {
            double dub = wcstod(data, &endptr);
            if (endptr == data + len) {
                appendLysonDouble(this->current, this->key, dub);
            }
        }
        break;
    }

    case EVENT_BOOLEAN: {
        int value = (len == 4 && wcsncmp(data, L"true", 4) == 0);
        if (value) {
            appendLysonTrue(this->current, this->key);
        }
        else {
            appendLysonFalse(this->current, this->key);
        }
        break;
    }

    case EVENT_NULL: {
        appendLysonNull(this->current, this->key);
        break;
    }

    case EVENT_OBJECT_END:
    case EVENT_ARRAY_END: {
        this->current = pop(this);
        break;
    }
    }
}