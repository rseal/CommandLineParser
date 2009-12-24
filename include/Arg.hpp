#ifndef ARG_H
#define ARG_H

#include <clp/Option.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>

class Arg: public Option{
    int numItems_;
    std::vector<string> list_;

public:
    Arg(const string& name, const string& helpDesc, const int& numItems, 
	const bool& required, const string& dfault=""):
        Option(name,helpDesc,required,dfault), numItems_(numItems), list_(){}

    const int NumItems() { return numItems_;}

    const string Value(const int& index) {
       bool error = list_.empty() || index > list_.size();
       string value = (required_ && error) ? "ERROR" : error ? default_ : list_.at(index);
       return value; 
    }

    void Add(std::vector<string> list) { list_ = list; Set(true);}
};

#endif
