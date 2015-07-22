/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-20
 */
#include <stdio.h>

#include "utils.h"
#include "calculator.h"

int main() {
    const char * code = "x = 1 + 2;\n"
                        "y = x + 3;\n"
                        "z = x + y;";
    Calculator calc(code, false);	
	//calc.build_symbol_table(); 
    LOG << calc.symbol_table().size();
    assert(calc.symbol_table().size() == 3);
    calc.print_symbol_table();
	return 0;
}
