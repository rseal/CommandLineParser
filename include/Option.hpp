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
    string default_;
public:
    Option(const string& name, const string& helpDesc, const bool& required, const string& dfault):
	name_(name), helpDesc_(helpDesc), required_(required), set_(false), default_(dfault) {}

    void Set(bool const& state) { set_ = state;     }
    const bool& Set()           { return set_;      }
    const string& Name()        { return name_;     }
    const string& Help()        { return helpDesc_; }

    const bool operator ==( const string& name) { return name_ == name; }
};
#endif
