#ifndef SWITCH_H
#define SWITCH_H

#include <clp/Option.hpp>

//!\brief Provides an interface for defining switches on the command line. 
class Switch: public Option{
public:
    Switch(const string& name, const string& helpDesc, const bool& required, 
    const string& dfault=""):
	Option(name,helpDesc,required,dfault){};
};
#endif
