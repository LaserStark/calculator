#include <stdio.h>

#include "parser.h"

void test_parser() {
    const char * code = "x = 1 + 2;\n"
                        "y = x + 3;\n"
                        "z = x + y;";
    Parser parser(code, false);
    StmtsNode *syntax_tree = dynamic_cast<StmtsNode *>(parser.parse());
    assert(syntax_tree != NULL);
    assert(syntax_tree->nodes().size() == 3);
    assert(syntax_tree->nodes()[0]->type() == NODE_ASSIGN);
    assert(syntax_tree->nodes()[1]->type() == NODE_ASSIGN);
    assert(syntax_tree->nodes()[2]->type() == NODE_ASSIGN);
}

int main() {
    test_parser();
	return 0;
}
