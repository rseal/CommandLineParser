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
#ifndef ARG_H
#define ARG_H

#include <Option.hpp>
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

    int NumItems() { return numItems_;}

    string Value(const unsigned int& index) {
       bool error = list_.empty() || index > list_.size();
       string value = (required_ && error) ? "ERROR" : error ? default_ : list_.at(index);
       return value; 
    }

    void Add(std::vector<string> list) { list_ = list; Set(true);}
};

#endif
