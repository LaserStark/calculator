#ifndef _PARSER_H_
#define _PARSER_H_

#include "utils.h"
#include "lexer.h"
#include "node.h"

class Parser {
public:
    Parser(char *input, bool is_file): lexer_(input, is_file) {}
    Node *parse();
private: //recursive down funtions of parser
    Node *expr(); 
    Node *term(); 
    Node *factor();
    Node *assign();
    Node *statements();
    void match(TokenType expect_type);
private:
    Lexer lexer_;
    TokenType next_token_;
};

#endif
