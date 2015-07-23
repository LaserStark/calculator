/* Calculator Interpreter & Compiler
 * Author: robin1001
 * Date: 2015-07-20
 */
#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <stdio.h>

#include <string>
#include <unordered_map>

#include "utils.h"
#include "lexer.h"
#include "parser.h"


class Calculator {
public:
    Calculator(Node *tree = NULL): syntax_tree_(tree) {}
    Calculator(const char *input, bool is_file = true): parser_(input, is_file) {
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
    std::unordered_map<std::string, int> symbol_table_;
    std::unordered_map<std::string, int> env_;// repl envoriment symbol value 
    Node * syntax_tree_;
    Parser parser_;
};

#endif
