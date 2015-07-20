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
        evaluate(tree);
        //show all variable in env
        print_env(); 
    }
}

int Calculator::evaluate(Node * node) {
    switch (node->type()) {
        case NODE_NUMBER:
            return dynamic_cast<NumberNode *>(node)->value();
        case NODE_OP: {
            OpNode *op_node = dynamic_cast<OpNode *>(node); 
            int x = evaluate(op_node->left());
            int y = evaluate(op_node->right());
            switch (op_node->op_type()) {
                case TOKEN_ADD:   return x + y;    
                case TOKEN_MINUS: return x - y;    
                case TOKEN_MULTI: return x * y;    
                case TOKEN_DEVI:  return x / y;    
                default:
                    ERROR << "unknown op type " << op_node->op_type();
            }
            return 0;
        }
        case NODE_ID: {
            IdNode * id_node = dynamic_cast<IdNode *>(node);
            bool find = true;
            int x = get_env(id_node->value(), &find);
            if (find) return x;
            else {
                printf("undifined variable: %s\n", dynamic_cast<IdNode *>(node)->value().c_str());                 
                return x;
            }
        }
        case NODE_ASSIGN: {
                AssignNode * assign = dynamic_cast<AssignNode *>(node); 
                int x = evaluate(assign->expr_node());
                set_env(assign->id_node()->value(), x); 
                return 0;
            }
        case NODE_STMTS: {
                StmtsNode *stmts = dynamic_cast<StmtsNode *>(node); 
                for (size_t i = 0; i < stmts->nodes().size(); i++) 
                    evaluate(stmts->nodes()[i]);
                return 0;
            }
    }
    return 0;
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

