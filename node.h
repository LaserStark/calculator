/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-10
 */
#ifndef _NODE_H_
#define _NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

#include "lexer.h"

/* gen_code: gen TVM(https://github.com/robin1001/tvm) assembly code.
 * it's a RISC like assembly language implements most basic assembly 
 * operations. for simplity, here only use 3 registers of TVM.
 * r0, r1, r7
 * 1. r0 keep all calc result in r0
 * 2. r1 as ALU src register add r0, r1, r0
 * 3. r7 as base address register, just like bp in X86
 */

typedef enum {
	NODE_NUMBER,
	NODE_OP,
	NODE_ID,
	NODE_ASSIGN,
	NODE_STMTS
} NodeType;

typedef std::unordered_map<std::string, int> Table; 

class Node {
public:
    Node(NodeType type): type_(type) {}
    virtual NodeType type() const{ return type_; }
    virtual int eval(Table *env) const { return 0; }
    virtual void gen_symbol(Table *symbol_table) const {}
    virtual void gen_code(FILE *fp, const Table &symbol_table) const {}
protected:
    NodeType type_;
};

class NumberNode: public Node {
public:
    NumberNode(int value): Node(NODE_NUMBER), value_(value) {}
	int value() const { return value_; }
    int eval(Table *env) const { return value_; }
    void gen_code(FILE *fp, const Table &symbol_table) const {
        fprintf(fp, "mov r0, %d\n", value_); 
    }
protected:
    int value_;
};

class OpNode: public Node {
public:
    OpNode(TokenType op, Node *left, Node *right): Node(NODE_OP), 
		op_type_(op), left_(left), right_(right) {
		assert(TOKEN_ADD == op_type_ || TOKEN_MINUS == op_type_ || \
            TOKEN_MULTI == op_type_ || TOKEN_DEVI == op_type_);
    }
    TokenType op_type() const { return op_type_; }
	Node *left() const { return left_; }
	Node *right() const { return right_; }
    int eval(Table *env) const; 

    void gen_symbol(Table *symbol_table) const {
        left_->gen_symbol(symbol_table);
        right_->gen_symbol(symbol_table);
    }
    void gen_code(FILE *fp, const Table &symbol_table) const;
protected:
	TokenType op_type_;
    Node *left_, *right_;
};


class IdNode: public Node {
public:
    IdNode(std::string value): Node(NODE_ID), value_(value) {}
	std::string value() const { return value_; }
    int eval(Table *env) const {
        if (env->find(value_) == env->end()) {
            printf("undifined variable: %s\n", value_.c_str());
            return 0;
        }
        return env->at(value_);
    }
    void gen_symbol(Table *symbol_table) const {
        if (symbol_table->find(value_) == symbol_table->end()) {
            ERROR << "semantic error: " << value_ << " referenced before it assigned";
        }
    }
    void gen_code(FILE *fp, const Table &symbol_table) const {
        assert(symbol_table.find(value_) != symbol_table.end());     
        int memory_addr = symbol_table.at(value_);
        fprintf(fp, "mov r7, %d\n", memory_addr);
        fprintf(fp, "ld r0, [r7]\n");
    }
protected:
    std::string value_; 
};

class AssignNode: public Node {
public:
    AssignNode(Node *id, Node *e): Node(NODE_ASSIGN), id_node_(id), expr_node_(e)  {}
    Node *id_node() const { return id_node_; }
    Node *expr_node() const { return expr_node_; }
    int eval(Table *env) const {
        int x = expr_node_->eval(env);
        (*env)[dynamic_cast<IdNode *>(id_node_)->value()] = x;
        return x;
    }
    void gen_symbol(Table *symbol_table) const {
        expr_node_->gen_symbol(symbol_table);
        std::string value = dynamic_cast<IdNode *>(id_node_)->value();
        if (symbol_table->find(value) == symbol_table->end()) { // not in symbol table
            symbol_table->insert(std::make_pair(value, symbol_table->size()));
        }
    }
    void gen_code(FILE *fp, const Table &symbol_table) const {
        expr_node_->gen_code(fp, symbol_table);
        std::string symbol = dynamic_cast<IdNode *>(id_node_)->value();
        assert(symbol_table.find(symbol) != symbol_table.end());     
        int memory_addr = symbol_table.at(symbol);
        fprintf(fp, "mov r7, %d\n", memory_addr);
        fprintf(fp, "st r0, [r7]\n");
    }
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
    const std::vector<Node *> & nodes() const {
		return node_vec_;
	}
    int eval(Table *env) const {
        for (size_t i = 0; i < node_vec_.size(); i++) {
            node_vec_[i]->eval(env);
        }
        return 0;
    }
    void gen_symbol(Table *symbol_table) const {
        for (size_t i = 0; i < node_vec_.size(); i++) {
            node_vec_[i]->gen_symbol(symbol_table);
        }
    }
    void gen_code(FILE *fp, const Table &symbol_table) const {
        for (size_t i = 0; i < node_vec_.size(); i++) {
            node_vec_[i]->gen_code(fp, symbol_table);
        }
    }
protected:
	std::vector<Node *> node_vec_;
};

#endif
