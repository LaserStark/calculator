#ifndef _UNITS_H_
#define _UNITS_H_

#include <stdio.h>
#include <stdlib.h>

#include <sstream>

class Message {
public:
    Message(const char *file, const char *func, int line, bool error = false): error_(error) {
        if (error) ss << "Error (";
        else ss << "Log (";
        ss << file << ":" << func << "():" << line << ") ";
    }
    ~Message() {
        printf("%s\n", ss.str().c_str());
        if (error_) exit(-1);
    }
    std::stringstream &stream() { return ss; }
private:
    std::stringstream ss;
    bool error_;
};


#define LOG Message(__FILE__, __func__, __LINE__, false).stream()
#define ERROR Message(__FILE__, __func__, __LINE__, true).stream()

#endif
