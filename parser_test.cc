#include <stdio.h>

#include "parser.h"

void test_parser() {
    char * code = "x = 1 + 2;";
    Parser parser(code, false);
    Node *syntax_tree = parser.parse();
    assert(syntax_tree != NULL);
}

int main() {
    test_parser();
	return 0;
}
