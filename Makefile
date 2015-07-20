CXX = g++
CXXFLAGS = -g  -std=c++11 -Wall#-I(include) -L(lib dir) -l(libname)
PARSE_OBJ = node.o parser.o lexer.o
OBJ = calculator.o node.o parser.o lexer.o 

all: calc repl test

calc: calc.cc $(OBJ) 
	$(CXX) -o $@  $< $(OBJ) $(CXXFLAGS) 
repl: repl.cc $(OBJ)
	$(CXX) -o $@  $< $(OBJ) $(CXXFLAGS) 

#TODO type check
test: lexer_test parser_test symbol_table_test

lexer_test: lexer_test.cc lexer.o
	$(CXX) -o $@ $< lexer.o $(CXXFLAGS) 

parser_test: parser_test.cc $(PARSE_OBJ)
	$(CXX) -o $@ $< $(PARSE_OBJ) $(CXXFLAGS) 

symbol_table_test: symbol_table_test.cc $(OBJ)
	$(CXX) -o $@ $< $(OBJ) $(CXXFLAGS) 


lexer.o: lexer.h utils.h
parser.o: lexer.h node.h utils.h 
node.o: node.h utils.h
calculator.o: calculator.h parser.h utils.h

.PHONY: clean
clean:
	-rm -f *.exe
	-rm -f *.o 
