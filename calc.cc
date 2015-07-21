/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-20
 */
#include <stdio.h>

#include "utils.h"
#include "parser.h"
#include "calculator.h"

int main(int argc, char *argv[]) {
    const char *usage = "parse and compile calculate expression into TVM code\n"
                        "Usage: calc input_file [out_file]\n";
    if (argc < 2) {
        printf("%s", usage);
        exit(-1);
    }

    Calculator calc(argv[1]);
    if (2 == argc) {
        calc.compile();    
    } else {
        calc.compile(argv[2]);
    }
    //Parser parser(code, false);
    return 0;
}
