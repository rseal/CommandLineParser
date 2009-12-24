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
#include <stdexcept>
#include <algorithm>

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

   typedef vector<string> StringVec;
   typedef vector<Switch> SwitchVec;
   typedef vector<Arg>    ArgVec;

   string       programName_;
   StringVec    tokens_;
   SwitchVec    switchVec_;
   ArgVec       argList_;
   StringVec    argHelpList_;
   vector<int>  argNumInputs_;
   bool         parse_;

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

   //!Member allowing a standalone Arg structure to be passed to the CommandLineParser structure
   void AddArg(const Arg& arg){
      argList_.push_back(arg);
   }

   //!Member allowing a standalone Switch structure to be passed to the CommandLineParser by reference.
   void AddSwitch(const Switch& swtch){
      switchVec_.push_back(swtch);
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

   if(!parse_)
      throw std::runtime_error("CLP Exception : User must call Parse() prior to GetArgValue()");

   ArgVec::iterator arg = find(argList_.begin(),argList_.end(),name);

   if(arg == argList_.end()){
      PrintHelp();
      throw std::invalid_argument("CLP Exception : Arg " + name + " not found");
   }

   return boost::lexical_cast<T>(arg->Value(itemNum));
}

//!This member performs the work of parsing the command line list and storing information relating
//! to defined switches and arguments
void CommandLineParser::Parse(){

   typedef boost::tokenizer<boost::char_separator<char> > boostToken;
   boost::char_separator<char> delimiter("- ");
   StringVec::const_iterator inputIter = tokens_.begin();

   while(inputIter != tokens_.end()){

      StringVec tknVec;
      boostToken tkn(*inputIter,delimiter);
      boostToken::iterator iter=tkn.begin();

      for(; iter!=tkn.end(); ++iter)
         tknVec.push_back(*iter);

      SwitchVec::iterator sw = std::find(switchVec_.begin(), switchVec_.end(), tknVec[0]);
      if(sw != switchVec_.end()) sw->Set(true);

      ArgVec::iterator arg = std::find(argList_.begin(), argList_.end(), tknVec[0]);
      if(arg != argList_.end()){
         tknVec.erase(tknVec.begin());
         arg->Add(tknVec);
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
      Print<ArgVec>(argList_);
   cout << endl;
   cout << "   Available Switches:" << endl;
   if(switchVec_.empty())
      cout << "      No Switchess available" << endl;
   else
      Print<SwitchVec>(switchVec_);
   cout << endl;
}

//!Returns true if the switch was found in the parsed input.
const bool CommandLineParser::SwitchSet(string const& name){

   if(!parse_)
      throw std::runtime_error("CLP Exception : User must call Parse() prior to GetArgValue()");

   SwitchVec::iterator sw = find(switchVec_.begin(),switchVec_.end(), name);
   return sw == switchVec_.end() ? false : sw->Set();
}
#endif
