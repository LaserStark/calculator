#include <stdio.h>

#include "calculator.h"

int main() {
	Node *node = parse();
	Calculator calc(node);
	calc.build_symbol_table();
	return 0;
}
