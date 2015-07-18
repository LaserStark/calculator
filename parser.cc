#include <string>

#include "parser.h"


void Parser::match(TokenType expect_type) {
	if (next_token_ == expect_type) {
        LOG << next_token_ << "\t" << lexer_.token_map(next_token_) << "\t" << lexer_.token();
		next_token_ = lexer_.get_token();
	}
    else {
        ERROR << "syntax error: expected " << lexer_.token_map(expect_type) << ", but get " << lexer_.token();
    }
}

Node * Parser::parse() {
	next_token_ = lexer_.get_token();	
	return statements(); 
}

Node * Parser::statements() {
	StmtsNode *node = new StmtsNode;	
    while (next_token_ != TOKEN_EOI) {
        Node *n = assign();
		node->add_node(n);
    }
	return dynamic_cast<Node *>(node);
}

Node * Parser::assign() {
	std::string id = lexer_.token();
    match(TOKEN_ID);
    Node *id_node = new IdNode(id); 
	//add to symbol_table
    match(TOKEN_ASSIGN);
    Node *expr_node = expr();
    match(TOKEN_SEMICOLON);
    return new AssignNode(id_node, expr_node);
}

Node * Parser::expr() {
    Node *node = term();
    while (TOKEN_ADD == next_token_ || TOKEN_MINUS == next_token_) {
        TokenType op = next_token_;
		match(op);
        Node *node2 = term();
        node = new OpNode(op, node, node2);
    }
    return node;
}

Node * Parser::term() {
    Node *node = factor();
    while (TOKEN_MULTI == next_token_ || TOKEN_DEVI == next_token_) {
        TokenType op = next_token_;
		match(op);
        Node *node2 = factor();
        node = new OpNode(op, node, node2);
    }
    return node;
}

Node * Parser::factor() {
	Node * node;
	switch(next_token_) {
		case TOKEN_NUMBER:
			{
				int val = atoi(lexer_.token().c_str());
				match(TOKEN_NUMBER);
				node = new NumberNode(val);
			}
			break;
		case TOKEN_ID:
			{
				std::string id = lexer_.token();
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
            ERROR  << "syntax error: unexpected token " << lexer_.token();
			break;
	}
	return node;
}	


