////////////////////////////////////////////////////////////////////////////////
///\file CommandLineParser.h
///
///
///Class for command-line parsing. 
///
///Author: Ryan Seal
///Modified: 07/04/09
////////////////////////////////////////////////////////////////////////////////
#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/static_assert.hpp>

#include <clp/Arg.hpp>
#include <clp/Switch.hpp>
#include <clp/Option.hpp>

using namespace std;

//!\brief CommandLineParser uses both Switch and Arg classes to 
//! provide a complete interface for parsing the command line.
struct CommandLineParser{

    typedef vector<string> StringArray;
    typedef vector<Switch> SwitchArray;
    typedef vector<Arg>    ArgArray;

    string         programName_;
    vector<string> tokens_;
    vector<Switch> switchList_;
    vector<Arg>    argList_;
    vector<string> argHelpList_;
    vector<int>    argNumInputs_;
    bool parse_;

    //!Finds target and returns index if found
    template<class T2>
    const int Find(T2& vec, const string& target){
	int idx;
	for(idx=0; idx<vec.size(); ++idx)
	    if(target == vec[idx].Name()) break;
	if(idx==vec.size()) idx=string::npos;
	return idx;
    }

    //!Formatted printing for Help display
    template<class T>
    void Print(T& vec){
	for(int i=0; i<vec.size(); ++i)
 	    cout << "      " 
 		 << std::left 
 		 << std::setw(10) 
 		 << "-" + vec[i].Name() 
 		 << std::left 
 		 << vec[i].Help() 
 		 << endl;
    }

public:

    CommandLineParser(int argc, char** argv);
    void Parse();
    void PrintHelp();
    template<typename T> 
    const T GetArgValue(string const& name, const int& itemNum=0);
    const bool ArgSet(string const& name);
    const bool SwitchSet(string const& name);

//!Direct method for defining a command line argument. 
    void AddArg(string const& name, string const& helpDesc, 
		int const& numInputs=1, bool const& required=false){
	argList_.push_back(Arg(name,helpDesc,numInputs,required));
    }

//!Member allowing a standalone Arg structure to be passed to the CommandLineParser structure
    void AddArg(const Arg& arg){
	argList_.push_back(arg);
    }

//!Direct method for defining a command line switch. An alternate method allows a standalone Switch struct to 
    void AddSwitch(string const& name, string const& helpDesc, bool const& required=false){
	switchList_.push_back(Switch(name,helpDesc,required));
    }

//!Member allowing a standalone Switch structure to be passed to the CommandLineParser by reference.
    void AddSwitch(const Switch& swtch){
	switchList_.push_back(swtch);
    }

//!Returns the first argument which is the name of the executed program.
    string const& ProgramName() { return programName_; }
    
    typedef CommandLineParser Clp;
    enum {ARGS=1, SWITCH=2};
};

//!Constructor accepting c-style argc and argv parameters.
CommandLineParser::CommandLineParser(int argc, char** argv): parse_(false){
    string str;
    string tStr;
    int index;
    programName_ = argv[0];
    
    for(int i=1; i<argc; ++i){
	str = argv[i];
	//look for '-' delimiter and create tokens
	if(str.find("-") != string::npos){
	    if(i!=1) tokens_.push_back(tStr);
	    tStr = str;
	}
	else
	    tStr += " " + str;
    }     
    if(argc > 1) tokens_.push_back(tStr);
}

//!GetArgValue makes use of a template, allowing the user to define the return type. This allows for conversion 
//! from string to any desired type. If the conversion is not possible, an exception will be thrown. 
template<typename T>
const T CommandLineParser::GetArgValue(string const& name, const int& itemNum){

    //run-time assertion
    assert(parse_==true);

    T value;

    int idx = this->Find<ArgArray>(argList_,name);
    try{
	if(idx == string::npos) throw CLP::NoArg(name);
	value = boost::lexical_cast<T>(argList_[idx].Value(itemNum));
    }
    catch(CLP::Exception& e){
	e.PrintError();
	PrintHelp();
	exit(1);
    }

    return value;
	
}

//!This member performs the work of parsing the command line list and storing information relating
//! to defined switches and arguments
void CommandLineParser::Parse(){

    typedef boost::tokenizer<boost::char_separator<char> > boostToken;
    boost::char_separator<char> delimiter("- ");
    vector<string>::const_iterator inputIter = tokens_.begin();
    int idx;

    while(inputIter != tokens_.end()){

	vector<string> tknVec;
	boostToken tkn(*inputIter,delimiter);
	boostToken::iterator iter=tkn.begin();

	for(; iter!=tkn.end(); ++iter)
	    tknVec.push_back(*iter);
	
 	idx = Find<SwitchArray>(switchList_,tknVec[0]);
 	if(idx != string::npos)
	    switchList_[idx].Set(true);
	
	idx = Find<ArgArray>(argList_,tknVec[0]);
	if(idx != string::npos){
	    tknVec.erase(tknVec.begin());
	    argList_[idx].Add(tknVec);
	}
	++inputIter;
    }

    parse_ = true;
}

void CommandLineParser::PrintHelp(){
    cout.fill(' ');
    cout << "   Available Arguments:" << endl;
    if(argList_.empty())
	cout << "      No Arguments available" << endl;
    else
	Print<ArgArray>(argList_);
    cout << endl;
    cout << "   Available Switches:" << endl;
    if(switchList_.empty())
	cout << "      No Switchess available" << endl;
    else
	Print<SwitchArray>(switchList_);
    cout << endl;
}

//!Returns true if the switch was found in the parsed input.
const bool CommandLineParser::SwitchSet(string const& name){
    //run-time assertion
    assert(parse_==true);
    int idx = Find<SwitchArray>(switchList_,name);
    return idx == string::npos ? false : switchList_[idx].Set();
}
#endif
