/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-20
 */
#include <string.h>

#include "utils.h"
#include "calculator.h"

void Calculator::build_symbol_table() {
    assert(syntax_tree_ != NULL);
    syntax_tree_->gen_symbol(&symbol_table_);
}

void Calculator::repl() {
    printf("calculator REPL, version 0.0\n");
    char code_str[1024] = {0};
    while (true) {
        printf(">> ");
        gets(code_str);
        if (strcmp(code_str, "quit") == 0 || strcmp(code_str, "exit") == 0 \
            || strcmp(code_str, "q") == 0 )
            break;
        Node * tree = Parser(code_str, false).parse();
        tree->eval(&env_);
        //show all variable in env
        print_env(); 
    }
}

void Calculator::compile(char *out_file) const{
    assert(syntax_tree_ != NULL);
    LOG << "compile calculate expression into TVM assembly code";
    if (NULL == out_file) {
        syntax_tree_->gen_code(stdout, symbol_table_);
    } else {
        FILE * fp = fopen(out_file, "w");
        syntax_tree_->gen_code(fp, symbol_table_);
        fclose(fp);
    }
    LOG << "compile Done";
}

