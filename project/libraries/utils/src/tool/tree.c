#include <base/tool/tree.h>
#include <assert.h>
#include <stdio.h>
#include <base/tool/stringbuf.h> 
#include <stdlib.h>
#include <string.h>

Tree* createTree(char* name) {
	Tree* this = (Tree*)malloc(sizeof(Tree));
	assert(this != NULL);
	this->depth = 0;
	this->value = strdup(name);
	this->child = NULL;
	this->next = NULL;
	return this;
}
void appendChild(Tree* parent, Tree* child) {
	child->depth = parent->depth + 1;
	if (parent->child == NULL);
	{
		parent->child = child;
	}
}
void appendNext(Tree* this, Tree* next) {
	next->depth = this->depth;
	this->next = next;
}



Tree* insertChild(Tree* parent, char* name, int index) {
    if (!parent || index < 0) return NULL;
    Tree* tree = createTree(name);
    if (!tree) return NULL;
    tree->depth = parent->depth + 1;

    if (index == 0 || parent->child == NULL) {
        tree->next = parent->child;
        parent->child = tree;
    }
    else {
        Tree* prev = parent->child;
        int i = 0;
        while (prev->next && i < index - 1) {
            prev = prev->next;
            i++;
        }
        tree->next = prev->next;
        prev->next = tree;
    }

    return tree;
}

Tree* insertNext(Tree* node, char* name, int index) {
    if (!node || index < 0) return NULL;
    Tree* tree = createTree(name);
    if (!tree) return NULL;
    tree->depth = node->depth;
    if (index == 0) {
        tree->next = node->next;
        node->next = tree;
    }
    else {
        Tree* prev = node->next;
        int i = 0;
        while (prev && prev->next && i < index - 1) {
            prev = prev->next;
            i++;
        }
        if (prev) {
            tree->next = prev->next;
            prev->next = tree;
        }
        else {
            node->next = tree;
        }
    }

    return tree;
}

static void STRecursive(Tree* node, stringbuf* sb, const char* prefix, int isLast) {
    appendStringBuf(sb, prefix);
    appendStringBuf(sb, isLast ? "©¸©¤©¤ " : "©À©¤©¤ ");
    appendStringBuf(sb, node->value);
    appendStringBufChar(sb, '\n');
    size_t prefix_len = strlen(prefix);
    char* new_prefix = (char*)malloc(prefix_len + 5);
    if (!new_prefix) return;
    strcpy(new_prefix, prefix);
    strcat(new_prefix, isLast ? "    " : "©¦   ");
    Tree* child = node->child;
    while (child) {
        Tree* next = child->next;
        STRecursive(child, sb, new_prefix, next == NULL);
        child = next;
    }

    free(new_prefix);
}

char* TreeToString(Tree* root) {
    if (!root) return NULL;
    stringbuf* sb = stringbuffer();
    if (!sb) return NULL;
    appendStringBuf(sb, root->value);
    appendStringBufChar(sb, '\n');
    char* empty_prefix = strdup("");
    if (!empty_prefix) {
        freeStringBuf(sb);
        return NULL;
    }
    Tree* child = root->child;
    while (child) {
        Tree* next = child->next;
        STRecursive(child, sb, empty_prefix, next == NULL);
        child = next;
    }
    free(empty_prefix);
    char* result = strdup(sb->buffer);
    freeStringBuf(sb);
    return result;
}


void recursive(Tree* node, char* prefix, int isLast) {
    printf("%s", prefix);
    printf("%s", isLast ? "©¸©¤©¤" : "©À©¤©¤");
    printf("%s\n", node->value);
    size_t len = wcslen(prefix);
    char* i = (char*)malloc((len + 5) * sizeof(char));
    if (i != 0)
    {
        wcscpy(i, prefix);
        wcscat(i, isLast ? L"    " : L"©¦   ");
    }
    Tree* child = node->child;
    while (child) {
        Tree* next = child->next;
        recursive(child, i, next == NULL);
        child = next;
    }
    free(i);
}

void renderTree(Tree* root) {
    if (!root) return;
    printf("%s\n", root->value);
    Tree* child = root->child;
    while (child) {
        Tree* next = child->next;
        recursive(child, L"", next == NULL);
        child = next;
    }
}