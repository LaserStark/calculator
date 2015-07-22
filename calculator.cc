/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-20
 */
#include <string.h>

#include "utils.h"
#include "calculator.h"

void Calculator::gen_symbol_table(Node *node) {
    switch (node->type()) {
        case NODE_NUMBER:
            return;
        case NODE_OP:
            gen_symbol_table((dynamic_cast<OpNode *>(node))->left());
            gen_symbol_table((dynamic_cast<OpNode *>(node))->right());
            return;
        case NODE_ID:
            if (get_symbol((dynamic_cast<IdNode *>(node)->value())) < 0) {
                ERROR << "semantic error:" << dynamic_cast<IdNode *>(node)->value()
                      << " referenced before it assigned\n"; 
            }
            return;
        case NODE_ASSIGN: {
                AssignNode * assign = dynamic_cast<AssignNode *>(node); 
                IdNode * id = assign->id_node();
                insert_symbol(id->value());
                gen_symbol_table(assign->expr_node());
            }
            return;
        case NODE_STMTS: {
                StmtsNode *stmts = dynamic_cast<StmtsNode *>(node); 
                for (size_t i = 0; i < stmts->nodes().size(); i++) 
                    gen_symbol_table(stmts->nodes()[i]);
            }
            return;
        default:
            return;
    }
}

void Calculator::build_symbol_table() {
    assert(syntax_tree_ != NULL);
    gen_symbol_table(syntax_tree_);
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

