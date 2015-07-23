/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-01
 */
#include <stdio.h>
#include <assert.h>

#include "lexer.h"
#include "utils.h"

void test_lexer() {
    const char * code = "#this is comment;\n"
                  "x = aa * (111 + 255);";
    //LOG << code;
    Lexer lexer(code, false);
    assert(lexer.get_token() == TOKEN_ID);
    assert(lexer.get_token() == TOKEN_ASSIGN);
    assert(lexer.get_token() == TOKEN_ID);
    assert(lexer.get_token() == TOKEN_MULTI);
    assert(lexer.get_token() == TOKEN_LEFT_PAREN);
    assert(lexer.get_token() == TOKEN_NUMBER);
    assert(lexer.get_token() == TOKEN_ADD);
    assert(lexer.get_token() == TOKEN_NUMBER);
    assert(lexer.get_token() == TOKEN_RIGHT_PAREN);
    assert(lexer.get_token() == TOKEN_SEMICOLON);
    assert(lexer.get_token() == TOKEN_EOI);
    LOG << "lexer test success";
}


int main(int argc, char *argv[]) {
    test_lexer();
}
