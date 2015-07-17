CXX = g++
CXXFLAGS = -g  -std=c++11 -Wall#-I(include) -L(lib dir) -l(libname)
OBJ = parser.o lexer.o 

main: $(OBJ) main.o 
	$(CXX) $(CXXFLAGS) $(OBJ) main.cc -o $@ 

#TODO type check
test: lexer_test parser_test symbol_table_test 

lexer_test: lexer_test.cc lexer.o
	$(CXX) -o $@ $< lexer.o $(CXXFLAGS) 
parser_test: parser_test.cc $(OBJ)
	$(CXX) -o $@ $< $(OBJ) $(CXXFLAGS) 
symbol_table_test: symbol_table_test.cc $(OBJ)
	$(CXX) -o $@ $< $(OBJ) $(CXXFLAGS) 


lexer.o: lexer.h
parser.o: lexer.h node.h symbol_table.h

.PHONY: clean
clean:
	-rm -f *.exe
	-rm -f *.o 
