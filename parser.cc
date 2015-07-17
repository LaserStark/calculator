#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "lexer.h"
#include "node.h"

void match(TokenType expect_type) {
	if (g_token_type == expect_type) {
		//printf("%s\t%s\n", token_map(expect_type).c_str(), g_token.c_str());
		g_token_type = get_token();
	}
    else {
		printf("syntax error: expected %s, but get %s\n", token_map(expect_type).c_str(), g_token.c_str());
		exit(-1);
    }
}

void syntax_error(std::string error_msg) {
	printf("%s\n", error_msg.c_str());
	exit(-1);
}


Node *expr(); 
Node *term(); 
Node *factor();
Node *assign();
Node *statements();
Node *parse();

Node *parse() {
	return statements(); 
}

Node *statements() {
	g_token_type = get_token();	
	StmtsNode *node = new StmtsNode;	
    while (g_token_type != TOKEN_EOI) {
        Node *n = assign();
		node->add_node(n);
    }
	return static_cast<Node *>(node);
}

Node *assign() {
	std::string id = g_token;
    match(TOKEN_ID);
    Node *id_node = new IdNode(id); 
	//add to symbol_table
    match(TOKEN_ASSIGN);
    Node *expr_node = expr();
    match(TOKEN_SEMICOLON);
    return new AssignNode(id_node, expr_node);
}

Node *expr() {
    Node *node = term();
    while (TOKEN_ADD == g_token_type || TOKEN_MINUS == g_token_type) {
        TokenType op = g_token_type;
		match(op);
        Node *node2 = term();
        node = new OpNode(op, node, node2);
    }
    return node;
}

Node *term() {
    Node *node = factor();
    while (TOKEN_MULTI == g_token_type || TOKEN_DEVI == g_token_type) {
        TokenType op = g_token_type;
		match(op);
        Node *node2 = factor();
        node = new OpNode(op, node, node2);
    }
    return node;
}

Node *factor() {
	Node * node;
	switch(g_token_type) {
		case TOKEN_NUMBER:
			{
				int val = atoi(g_token.c_str());
				match(TOKEN_NUMBER);
				node = new NumberNode(val);
			}
			break;
		case TOKEN_ID:
			{
				std::string id = g_token;
    			match(TOKEN_ID);
    			node = new IdNode(id); 
			}
			break;
		case TOKEN_LEFT_PAREN: 
			match(TOKEN_LEFT_PAREN);
 	   		node = expr();
 	   		match(TOKEN_RIGHT_PAREN);
			break;
		default:
			std::stringstream ss;
			ss << "syntax error: unexpected token " << g_token;
			syntax_error(ss.str());
			break;
	}
	return node;
}	


