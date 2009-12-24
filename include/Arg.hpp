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
	const bool& required): Option(name,helpDesc,required), 
			       numItems_(numItems), list_(){}

    const int NumItems() { return numItems_;}

    const string Value(const int& index) {
	if(list_.empty() || (required_ && (index > list_.size())))
           throw std::invalid_argument("CLP Exception : Argument " + Name() + " not found ");
	return list_.at(index);
    }

    void Add(std::vector<string> list) { list_ = list; Set(true);}
};

#endif
