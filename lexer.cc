/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-01
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <string>
#include <sstream>

#include "utils.h"
#include "lexer.h"

std::string Lexer::token_map(TokenType expect_type) {
    switch (expect_type) {
        case TOKEN_ID: return "ID";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_ASSIGN: return "=";
        case TOKEN_ADD: return "+";
        case TOKEN_MINUS: return "-";
        case TOKEN_MULTI: return "*";
        case TOKEN_DEVI: return "/";
        case TOKEN_LEFT_PAREN: return "(";
        case TOKEN_RIGHT_PAREN: return ")";
        case TOKEN_SEMICOLON: return ";";
        default: return "unexpected";
    }
}

char Lexer::read_char() {
    if (is_file_) {// file or stdin 
        assert(fp_ != NULL);
        return getc(fp_);
    } else {
        if (input_cur_ < input_.size()) return input_[input_cur_++];
        else return EOF;
    }
}

void Lexer::back_char(char ch) {
    if (is_file_) {
        assert(fp_ != NULL);
        ungetc(ch, fp_);
    } else {
        input_cur_--; 
    }
}

TokenType Lexer::get_token() {
    char ch = read_char();
    //comment
    if ('#' == ch) {
        do {
            ch = read_char();
        } while ( ch != '\n' && ch != EOF);
    }
    //black tab
    while (' ' == ch || '\t' == ch || '\n' == ch) ch = read_char();
    //eof
    if (ch == EOF) return TOKEN_EOI;
    //id
    if (isalpha(ch)) {
        std::stringstream ss;
        do {
            ss << ch;
            ch =  read_char();
        } while (isalpha(ch));
        back_char(ch); //read one more, back it
        token_ = ss.str();
        return TOKEN_ID;
    }
    //number
    if (isdigit(ch)) {
        std::string num_str;
        do {
            num_str += ch;
            ch = read_char();
        } while (isdigit(ch));//bug 
        token_ = num_str;
        back_char(ch);
        return TOKEN_NUMBER;
    }
    token_ = ch;
    //other
    switch(ch) {
        case '=':   return TOKEN_ASSIGN;
        case '+':   return TOKEN_ADD;
        case '-':   return TOKEN_MINUS;
        case '*':   return TOKEN_MULTI;
        case '/':   return TOKEN_DEVI;
        case '(':   return TOKEN_LEFT_PAREN;
        case ')':   return TOKEN_RIGHT_PAREN;
        case ';':   return TOKEN_SEMICOLON;
    }
    //unknown char 
    ERROR << "lex error: unkonwn char " << ch;
    return TOKEN_UNKNOWN;
}

