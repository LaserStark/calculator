#include <stdio.h>
#include <assert.h>

#include "lexer.h"

void test_lexer() {
    Lexer lexer("#this is comment;\n aa * (111 + 255);", false);
    assert(lexer.get_token() == TOKEN_ID);
    assert(lexer.get_token() == TOKEN_MULTI);
    assert(lexer.get_token() == TOKEN_LEFT_PAREN);
    assert(lexer.get_token() == TOKEN_NUMBER);
    assert(lexer.get_token() == TOKEN_ADD);
    assert(lexer.get_token() == TOKEN_NUMBER);
    assert(lexer.get_token() == TOKEN_RIGHT_PAREN);
    assert(lexer.get_token() == TOKEN_SEMICOLON);
    printf("lexer test success\n");
}


int main(int argc, char *argv[]) {
	test_lexer();
}
