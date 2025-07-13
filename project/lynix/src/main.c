#include <stdio.h>
#include <lyson/lyson.h>
#include <lyson/parser.h>
#include <compiler/tokenizer.h>
#include <base/exception.h>
#include <base/tool/file.h>
#include <base/tool/stringbuf.h>
#include <locale.h>

int main() {
	Scanner* scan = scanner(strtow(readFileAll("main.ly")), "main.ly");
	scanTokens(scan);
	renderTokens(scan);
	return 0;
}