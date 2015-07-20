#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <stdio.h>

#include <string>
#include <unordered_map>

#include "lexer.h"
#include "parser.h"


class Calculator {
public:
	Calculator(Node *tree = NULL): label_count_(0), syntax_tree_(tree) {}
    Calculator(const char *input, bool is_file = true): label_count_(0), 
        parser_(input, is_file) {
        syntax_tree_ = parser_.parse();
        build_symbol_table();
    }
	void print_symbol_table() const{
        std::unordered_map<std::string,int>::const_iterator it = symbol_table_.begin(); 
		for (; it != symbol_table_.end(); it++) {
        	printf("%s\t%d\n", it->first.c_str(), it->second);
		}
	}
    void print_env() const{
        printf("var\tvalue\n");
        std::unordered_map<std::string,int>::const_iterator it = env_.begin(); 
		for (; it != env_.end(); it++) {
        	printf("%s\t%d\n", it->first.c_str(), it->second);
		}
    }
	void build_symbol_table(); 
    void repl(); // read evaluate print loop interpreter
	const std::unordered_map<std::string, int> &symbol_table() const{ return symbol_table_; }
    void compile(char *out_file = NULL) const;
protected:
	//symbol table functions
	void gen_symbol_table(Node *node);
	void insert_symbol(std::string sym) {
		if (symbol_table_.find(sym) == symbol_table_.end()) {
			symbol_table_.insert(make_pair(sym, label_count_));
			label_count_++;
		}
	}
	//0 faied
	int get_symbol(std::string sym) const {
		if (symbol_table_.find(sym) == symbol_table_.end()) 
			return -1;
		return symbol_table_.at(sym);
	}
    //evaluate
    int evaluate(Node * node);
    int get_env(std::string sym, bool *find) const { //get value from current env
        if (env_.find(sym) != env_.end()) {
            *find = true; 
		    return env_.at(sym);
        } else {
            *find = false;
            return 0;
        }
    }
    void set_env(std::string sym, int value) {
        env_[sym] = value;
    }
protected:
	std::unordered_map<std::string, int> symbol_table_;
	std::unordered_map<std::string, int> env_;// repl envoriment symbol value 
	int label_count_;
	Node * syntax_tree_;
    Parser parser_;
};

#endif
