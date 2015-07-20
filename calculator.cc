#include <string.h>

#include "utils.h"
#include "calculator.h"


void Calculator::gen_symbol_table(Node *node) {
    switch (node->type()) {
        case NODE_NUMBER:
            break;
        case NODE_OP:
            gen_symbol_table((dynamic_cast<OpNode *>(node))->left());
            gen_symbol_table((dynamic_cast<OpNode *>(node))->right());
            break;
        case NODE_ID:
            if (get_symbol((dynamic_cast<IdNode *>(node)->value())) < 0) {
                ERROR << "semantic error:" << dynamic_cast<IdNode *>(node)->value()
                      << " referenced before it assigned\n"; 
            }
            break;
        case NODE_ASSIGN: {
                AssignNode * assign = dynamic_cast<AssignNode *>(node); 
                IdNode * id = assign->id_node();
                insert_symbol(id->value());
                gen_symbol_table(assign->expr_node());
            }
            break;
        case NODE_STMTS: {
                StmtsNode *stmts = dynamic_cast<StmtsNode *>(node); 
                for (size_t i = 0; i < stmts->nodes().size(); i++) 
                    gen_symbol_table(stmts->nodes()[i]);
            }
            break;
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
            break;
        case NODE_STMTS: {
                StmtsNode *stmts = dynamic_cast<StmtsNode *>(node); 
                for (size_t i = 0; i < stmts->nodes().size(); i++) 
                    evaluate(stmts->nodes()[i]);
                return 0;
            }
    }
}
