#include "node.h"


void OpNode::gen_code(FILE *fp, const Table &symbol_table) const {
    /* 1. left expr's result in r0 at first
     * 2. push r0(avoid override when gen right) 
     * 3. gen right expr code, result in r0
     * 4. pop r1(just pop the value to r1 which pushed in step 2(r0);
     * 5. finish (left in r1, right in r0)
     */
    left_->gen_code(fp, symbol_table); 
    fprintf(fp, "push r0\n");
    right_->gen_code(fp, symbol_table);
    fprintf(fp, "pop r1\n");
    switch (op_type_) {
        case TOKEN_ADD:
            fprintf(fp, "add r0, r1, r0\n");
            return;
        case TOKEN_MINUS:
            fprintf(fp, "sub r0, r1, r0\n");
            return;
        case TOKEN_MULTI:
            fprintf(fp, "mul r0, r1, r0\n");
            return;
        case TOKEN_DEVI:
            fprintf(fp, "div r0, r1, r0\n");
            return;
        default:
            return;
    }
}
