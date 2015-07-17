#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <stdio.h>

#include <string>
#include <unordered_map>

#include "lexer.h"
#include "parser.h"


class Calculator {
public:
	Calculator(Node *tree): label_count_(0), syntax_tree_(tree) {}
	void print_symbol_table() {
		using namespace std;
		for (unordered_map<string,int>::iterator it = symbol_table_.begin(); it != symbol_table_.end(); it++) {
        	printf("%s\t%d\n", it->first.c_str(), it->second);
		}
	}
	void build_symbol_table(); 
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
	int get_symbol(std::string sym) {
		if (symbol_table_.find(sym) == symbol_table_.end()) 
			return -1;
		return symbol_table_[sym];
	}

	std::unordered_map<std::string, int> symbol_table_;
	int label_count_;
	Node * syntax_tree_;
};

#endif
