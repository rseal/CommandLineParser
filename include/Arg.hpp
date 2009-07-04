#ifndef ARG_H
#define ARG_H

#include <clp/Option.hpp>
#include <iostream>
#include <vector>

using std::cerr;
using std::endl;
using std::vector;

namespace CLP{
    
    class Exception{
    public:
	virtual void PrintError() { 
	    cerr << "CLP Exception thrown" << endl;
	}
    };

    class NoArg: public Exception{
	string arg_;
    public:
	explicit NoArg(const string& arg): arg_(arg){}
	virtual void PrintError() { 
	    cerr << "CLP Exeption - argument " << arg_ << " not found" << endl;
	}
    };
}

class Arg: public Option{
    int numItems_;
    vector<string> list_;

public:
    Arg(const string& name, const string& helpDesc, const int& numItems, 
	const bool& required): Option(name,helpDesc,required), 
			       numItems_(numItems), list_(){}

    const int NumItems() { return numItems_;}


    const string Value(const int& index) {
	if(list_.empty() || (required_ && (index > list_.size())))
	    throw CLP::NoArg(name_);
	return list_.at(index);
    }

    void Add(vector<string> list) { list_ = list; Set(true);}

};

#endif
