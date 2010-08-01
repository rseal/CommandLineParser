// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Command Line Parser (CLP) Software.
//
// CLP is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// CLP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CLP.  If not, see <http://www.gnu.org/licenses/>.
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

    // if the option is required but not set, return false.
    // This is intended to be checked after parsing.
    const bool Valid() { return required_ && !set_ ? false : true; }
};
#endif
