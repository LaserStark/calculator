#include <stdio.h>

#include "node.h"
#include "parser.h"

int main() {
	Node *node = parse();
	assert(NODE_STMTS == node->type()); 
	return 0;
}
