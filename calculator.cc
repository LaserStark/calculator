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
            if (get_symbol((dynamic_cast<IdNode *>(node)->value())) > 0) {
                printf("semantic error: %s referenced before it assigned\n", \
                        dynamic_cast<IdNode *>(node)->value().c_str());
                exit(-1);
            }
            break;
        case NODE_ASSIGN: {
                AssignNode * assign = dynamic_cast<AssignNode *>(node); 
                IdNode * id = dynamic_cast<IdNode *> (assign->id_node());
                insert_symbol(id->value());
                gen_symbol_table(assign->expr_node());
                insert_symbol(id->value());
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
    gen_symbol_table(syntax_tree_);
}


