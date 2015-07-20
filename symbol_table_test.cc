#include <stdio.h>

#include "calculator.h"

int main() {
    const char * code = "x = 1 + 2;\n"
                        "y = x + 3;\n"
                        "z = x + y;";
    Parser parser(code, false);
    Node *tree = parser.parse();
    Calculator calc(tree);	
	calc.build_symbol_table(); 
    assert(calc.symbol_table().size() == 3);
    calc.print_symbol_table();
	return 0;
}
