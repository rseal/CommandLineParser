#ifndef OPTION_H
#define OPTION_H

#include <string>
using std::string;

//!\brief Provides an interface for defining switches on the command line. 
class Option{
protected:
    string name_;
    string helpDesc_;
    bool required_;
    bool set_;
public:
    Option(const string& name, const string& helpDesc, const bool& required):
	name_(name), helpDesc_(helpDesc), required_(required), set_(false) {}

    void Set(bool const& state) { set_ = state;     }
    const bool& Set()           { return set_;      }
    const string& Name()        { return name_;     }
    const string& Help()        { return helpDesc_; }
};
#endif
