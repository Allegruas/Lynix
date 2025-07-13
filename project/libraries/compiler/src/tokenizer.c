#include <compiler/tokenizer.h>
#include <base/tool/stringbuf.h>
#include <base/tool/file.h>
#include <lyson/lyson.h>
#include <assert.h>
#include <locale.h>


Scanner* scanner(wstring source, char* path) {
	setlocale(LC_ALL, "en_US.UTF-8");
	Scanner* this = (Scanner*)malloc(sizeof(Scanner));
	notNull(this);
	*this = (Scanner){
		.index = 0,
		.eof = 0,
		.source = wcsdup(source),
		.path = strdup(path),
		.pos = (Position){
			.line = 1,
			.column = 0
		},
	};
	this->tokens = (Tokens*)malloc(sizeof(Tokens));
	notNull(this->tokens);
	this->tokens->data = (Token*)malloc(sizeof(Token));
	this->tokens->count = 0;
	this->tokens->capacity = 0;

	this->error = (Errors*)malloc(sizeof(Errors));
	notNull(this->error);
	this->error->errors = (char**)malloc(sizeof(char));
	this->error->count = 0;
	this->error->capacity = 0;
	return this;
}

static wchar current(Scanner* this) {
	notNull(this);
	return this->index < wcslen(this->source) ? this->source[this->index] : L'\0';
}

static wchar peek(Scanner* this) {
	notNull(this);
	return (this->index + 1) < wcslen(this->source) ? this->source[this->index + 1] : L'\0';
}

static Token createToken(Scanner* this, int type, const char* data) {
	notNull(this);
	return (Token) {
		.type = type,
			.value = strdup(data),
			.pos = this->pos
	};
}

static void appendScannerfailure(Scanner* this, const char* msg) {
	notNull(this);
	if (this->error->count >= this->error->capacity) {
		int capacity = (this->error->capacity) < 8 ? 8 : (this->error->capacity) * 2;
		char** errors = (char**)realloc(this->error->errors, sizeof(char) * (capacity));
		notNull(errors)
			this->error->errors = errors;
		notNull(this->error->errors);
		this->error->capacity = capacity;
	}
	this->error->errors[this->error->count] = strdup(msg);
	this->error->count++;
}


static void advance(Scanner* this) {
	notNull(this);
	this->index++;
	this->pos.column++;
}

static int appendToken(Scanner* this, Token createToken) {
	notNull(this);

	if (this->tokens->count >= this->tokens->capacity) {
		int capacity = (this->tokens->capacity == 0) ? 8 : this->tokens->capacity * 2;
		Token* data = (Token*)realloc(this->tokens->data, capacity * sizeof(Token));
		notNull(data);
		this->tokens->data = data;
		this->tokens->capacity = capacity;
	}
	this->tokens->data[this->tokens->count] = createToken;
	this->tokens->count++;
	return 1;
}

static Token scanString(Scanner* this) {
	notNull(this);
	stringbuf* buf = stringbuffer();
	int start_line = this->pos.line;
	advance(this);
	if (current(this) == L'"') {
		advance(this);
		appendStringBufChar(buf, '\0');
		return createToken(this, TOKENTYPE_STR, buf->buffer);
	}
	while (current(this) != L'"' && current(this) != L'\0') {
		if (this->pos.line != start_line) {
			appendScannerfailure(this, "String literal must be on a single line");
			break;
		}
		if (current(this) == L'\\') {
			advance(this);
			if (this->pos.line != start_line) {
				appendScannerfailure(this, "Escape sequence cannot span multiple lines");
				break;
			}
			switch (current(this)) {
			case L'n': appendStringBufChar(buf, '\n'); break;
			case L't': appendStringBufChar(buf, '\t'); break;
			case L'b': appendStringBufChar(buf, '\b'); break;
			case L'"': appendStringBufChar(buf, '"'); break;
			case L'\\': appendStringBufChar(buf, '\\'); break;
			default:
				appendScannerfailure(this, "Unknown escape sequence in character literal");
				appendStringBuf(buf, wctos(current(this)));
				break;
			}
			advance(this);
		}
		else {
			appendStringBuf(buf, wctos(current(this)));
			advance(this);
		}
	}
	if (current(this) == L'"') {
		advance(this);
	}
	else {
		if (this->pos.line == start_line) {
			appendScannerfailure(this, "Unclosed string literal");
		}
		else {
			appendScannerfailure(this, "String literal spans multiple lines");
		}
	}
	appendStringBufChar(buf, '\0');

	Token result = createToken(this, TOKENTYPE_STR, buf->buffer);
	freeStringBuf(buf);
	return result;
}

static Token scanChar(Scanner* this) {
	notNull(this);
	stringbuf* buf = stringbuffer();
	advance(this);
	if (current(this) == L'\\') {
		advance(this);
		switch (current(this)) {
		case L'n': appendStringBufChar(buf, '\n'); break;
		case L't': appendStringBufChar(buf, '\t'); break;
		case L'b': appendStringBufChar(buf, '\b'); break;
		case L'"': appendStringBufChar(buf, '"'); break;
		case L'\\': appendStringBufChar(buf, '\\'); break;
		default:
			appendScannerfailure(this, "Unknown escape sequence in character literal");
			appendStringBuf(buf, wctos(current(this)));
			break;
		}
		advance(this);
	}
	else {
		appendStringBuf(buf, wctos(current(this)));
		advance(this);
	}
	if (current(this) != L'\'')
	{
		appendScannerfailure(this, "Unclosed character literal");
	}
	else {
		advance(this);
	}
	Token result = createToken(this, TOKENTYPE_CHAR, buf->buffer);
	freeStringBuf(buf);
	return result;
}

static Token scanNumber(Scanner* this) {
	notNull(this);
	stringbuf* buf = stringbuffer();
	int type = TOKENTYPE_INT;
	if (current(this) == L'0' && (peek(this) == L'x' || peek(this) == L'X'))
	{
		type = TOKENTYPE_HEX;
		appendStringBufChar(buf, '0');
		advance(this);
		appendStringBuf(buf, wctos(current(this)));
		advance(this);
		while (iswxdigit(current(this))) {
			appendStringBuf(buf, wctos(current(this)));
			advance(this);
		}
	}
	while (iswdigit(current(this))) {
		appendStringBuf(buf, wctos(current(this)));
		advance(this);
	}
	if (current(this) == L'.')
	{
		type = TOKENTYPE_FLOAT;
		appendStringBufChar(buf, '.');
		advance(this);
		while (iswdigit(current(this))) {
			appendStringBuf(buf, wctos(current(this)));
			advance(this);
		}
	}
	if (current(this) == L'e' || current(this) == L'E') {
		type = TOKENTYPE_FLOAT;
		appendStringBuf(buf, wctos(current(this)));
		advance(this);
		if (current(this) == L'+' || current(this) == L'-') {
			appendStringBuf(buf, wctos(current(this)));
			advance(this);
		}
		while (iswdigit(current(this))) {
			appendStringBuf(buf, wctos(current(this)));
			advance(this);
		}
	}
	if (current(this) == L'f' || current(this) == L'F')
	{
		advance(this);
		type = TOKENTYPE_FLOAT;
	}
	if (current(this) == L'd' || current(this) == L'D')
	{
		advance(this);
		type = TOKENTYPE_DOUBLE;
	}
	Token result = createToken(this, type, buf->buffer);
	freeStringBuf(buf);
	return result;
}

static Token scanIdent(Scanner* this) {
	notNull(this);
	stringbuf* buf = stringbuffer();
	while (iswalnum(current(this)) || current(this) == L'_') {
		appendStringBuf(buf, wctos(current(this)));
		advance(this);
	}
	Token result = createToken(this, TOKENTYPE_IDENTIFIER, buf->buffer);
	freeStringBuf(buf);
	return result;
}


Token getToken(Scanner* this) {
	notNull(this);
	if (current(this) == L'\n')
	{
		advance(this);
		Token result = createToken(this, TOKENTYPE_SYMBOL, "\\n");
		this->pos.line++;
		this->pos.column = 0;
		return result;
	}
	while (iswspace(current(this))) {
		advance(this);
	}
	if (current(this) == L'/')
	{
		wchar next = peek(this);
		if (next == L'/')
		{
			advance(this);
			advance(this);
			while (current(this) != L'\n' && current(this) != L'\0') {
				advance(this);
				return getToken(this);
			}
		}
		else if (next == L'*')
		{
			advance(this);
			advance(this);
			while (1) {
				if (current(this) == L'\0')
				{
					appendScannerfailure(this, "Unclosed block comment");
					return getToken(this);
				}
				if (current(this) == L'*' && peek(this) == L'/')
				{
					advance(this);
					advance(this);
					return getToken(this);
				}
				if (current(this) == L'\n')
				{
					this->pos.line++;
					this->pos.column = 0;
				}
				advance(this);
			}
			return getToken(this);
		}
	}
	if (this->index >= wcslen(this->source) || current(this) == '\0')
	{
		this->eof = 1;
		return createToken(this, TOKENTYPE_EOF, "\0");
	}
	wchar c = current(this);
	if (c == L'"')
	{
		return scanString(this);
	}
	if (c == L'\'')
	{
		return scanChar(this);
	}
	if (iswdigit(c))
	{
		return scanNumber(this);
	}
	if (iswalpha(c) || c == L'_')
	{
		return scanIdent(this);
	}
	stringbuf* buf = stringbuffer();
	appendStringBufChar(buf, wctoc(c));
	advance(this);
	Token result = createToken(this, TOKENTYPE_SYMBOL, buf->buffer);
	freeStringBuf(buf);
	return result;
}

int scanTokens(Scanner* this) {
	notNull(this);
	while (!this->eof) {
		appendToken(this, getToken(this));
	}
	return this->error->count > 0;
}

void renderTokens(Scanner* this) {
	Lyson* lyson = createLysonObject();
	appendLysonStr(lyson, "path", getAbsolutePath(this->path));
	appendLysonInt(lyson, "count", this->tokens->count);
	Lyson* array = createLysonArray();
	for (size_t i = 0; i < this->tokens->count; i++)
	{
		Token current = this->tokens->data[i];
		Lyson* child = createLysonObject();
		switch (current.type) {
		case TOKENTYPE_EOF:
			appendLysonStr(child, "type", "EOF");
			break;
		case TOKENTYPE_STR:
			appendLysonStr(child, "type", "String");
			break;
		case TOKENTYPE_INT:
			appendLysonStr(child, "type", "Int");
			break;
		case TOKENTYPE_HEX:
			appendLysonStr(child, "type", "Hex");
			break;
		case TOKENTYPE_CHAR:
			appendLysonStr(child, "type", "Char");
			break;
		case TOKENTYPE_FLOAT:
			appendLysonStr(child, "type", "Float");
			break;
		case TOKENTYPE_DOUBLE:
			appendLysonStr(child, "type", "Double");
			break;
		case TOKENTYPE_SYMBOL:
			appendLysonStr(child, "type", "Symbol");
			break;
		case TOKENTYPE_IDENTIFIER:
			appendLysonStr(child, "type", "Identifier");
			break;
		default:
			appendLysonStr(child, "type", "Unknown");
			break;
		}
		appendLysonStr(child, "value", current.value);
		Lyson* pos = createLysonObject();
		appendLysonInt(pos, "line", current.pos.line);
		appendLysonInt(pos, "column", current.pos.column);
		appendLysonObject(child, "position", pos);
		appendLysonArrayObject(array, child);
	}
	appendLysonArray(lyson, "tokens", array);
	printf("%s", lysonToString(lyson, 1));
}

void freeScanner(Scanner* this) {
	notNull(this);
	if (this->source) free(this->source);
	if (this->path) free(this->path);
	if (this->tokens) {
		for (size_t i = 0; i < this->tokens->count; i++) {
			free(this->tokens->data[i].value);
		}
		free(this->tokens->data);
		free(this->tokens);
	}
	if (this->error) {
		for (size_t i = 0; i < this->error->count; i++) {
			free(this->error->errors[i]);
		}
		free(this->error->errors);
		free(this->error);
	}
	free(this);
}