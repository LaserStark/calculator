#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <string>
#include <sstream>

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

TokenType Lexer::get_token() {
    char ch;
    if (!is_read_) ch = read_char();
    //comment
    if ('#' == ch) {
        do {
            ch = read_char();
        } while ( ch != '\n');
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
        token_ = ss.str();
        is_read_ = true;
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
        is_read_ = true;
		return TOKEN_NUMBER;
	}
	//g_token = ch;
	//other
	switch(ch) {
		case '=': is_read_ = false; return TOKEN_ASSIGN;
		case '+': is_read_ = false;	return TOKEN_ADD;
		case '-': is_read_ = false;	return TOKEN_MINUS;
		case '*': is_read_ = false;	return TOKEN_MULTI;
		case '/': is_read_ = false;	return TOKEN_DEVI;
		case '(': is_read_ = false;	return TOKEN_LEFT_PAREN;
		case ')': is_read_ = false;	return TOKEN_RIGHT_PAREN;
		case ';': is_read_ = false;	return TOKEN_SEMICOLON;
	}
	printf("unknown char %c\n", ch);
	exit(-1);
	return TOKEN_UNKNOWN;
}

