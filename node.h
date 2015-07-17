#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <string>
#include <sstream>

#include "lexer.h"

typedef enum {
	NODE_NUMBER,
	NODE_OP,
	NODE_ID,
	NODE_ASSIGN,
	NODE_STMTS
} NodeType;



class Node {
public:
    Node(NodeType type): type_(type) {}
    virtual NodeType type() const { return type_; }
protected:
    NodeType type_;
};

class NumberNode: public Node {
public:
    NumberNode(int value): Node(NODE_NUMBER), value_(value) {}
	int value() { return value_; }
protected:
    int value_;
};

class OpNode: public Node {
public:
    OpNode(TokenType op, Node *left, Node *right): Node(NODE_OP), 
		op_(op), left_(left), right_(right) {
		assert(TOKEN_ADD == op_ || TOKEN_MINUS == op_ || TOKEN_MULTI == op_ || TOKEN_DEVI == op_);
    }
	Node *left() { return left_; }
	Node *right() { return right_; }
protected:
	TokenType op_;
    Node *left_, *right_;
};


class IdNode: public Node {
public:
    IdNode(std::string value): Node(NODE_ID), value_(value) {}
	std::string value() { return value_; }
protected:
   std::string value_; 
};

class AssignNode: public Node {
public:
    AssignNode(Node *id, Node *e): Node(NODE_ASSIGN), id_node_(id), expr_node_(e)  {}
    Node *id_node() { return id_node_; }
    Node *expr_node() { return expr_node_; }
protected:
    Node *id_node_;
    Node *expr_node_;
};

class StmtsNode: public Node {
public:
	StmtsNode(): Node(NODE_STMTS) {}
	void add_node(Node *node) {
		node_vec_.push_back(node);
	}
	const std::vector<Node *> & nodes() {
		return node_vec_;
	}
protected:
	std::vector<Node *> node_vec_;
};

#endif
