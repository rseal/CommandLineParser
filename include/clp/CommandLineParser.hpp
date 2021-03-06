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
   ArgVec       argVec_;
   StringVec    argHelpList_;
   vector<int>  argNumInputs_;
   bool         parse_;

   //!Formatted printing for Help display
   template<class T> void Print(T& vec){

      for(unsigned int idx=0; idx<vec.size(); ++idx)
         cout 
            << "      " 
            << std::left 
            << std::setw(10) 
            << "-" + vec[idx].Name() 
            << std::left 
            << vec[idx].Help() 
            << endl;
   }

   public:

   CommandLineParser(int argc, char** argv);
   void Parse();
   void PrintHelp();
   void Validate();
   template<typename T> 
      T GetArgValue(string const& name, const int& itemNum=0);
   bool ArgSet(string const& name);
   bool SwitchSet(string const& name);

   //!Member allowing a standalone Arg structure to be passed to the 
   //CommandLineParser structure
   void AddArg(const Arg& arg){
      argVec_.push_back(arg);
   }

   //!Member allowing a standalone Switch structure to be passed to the 
   //CommandLineParser by reference.
   void AddSwitch(const Switch& swtch){
      switchVec_.push_back(swtch);
   }

   //!Returns the first argument which is the name of the executed program.
   string & ProgramName() { return programName_; }

   typedef CommandLineParser Clp;
   enum {ARGS=1, SWITCH=2};
};

//!Constructor accepting c-style argc and argv parameters.
CommandLineParser::CommandLineParser(int argc, char** argv): parse_(false){
   string str;
   string tStr;
   programName_ = argv[0];

   for(int idx=1; idx<argc; ++idx)
   {
      str = argv[idx];
      //look for '-' delimiter and create tokens
      if(str.find("-") != string::npos)
      {
         if(idx!=1) tokens_.push_back(tStr);
         tStr = str;
      }
      else
      {
         tStr += " " + str;
      }
   }     
   if(argc > 1) { tokens_.push_back(tStr); }
}

//!GetArgValue makes use of a template, allowing the user to define the 
//return type. This allows for conversion from string to any desired type. 
//If the conversion is not possible, an exception will be thrown. 
template<typename T>
T CommandLineParser::GetArgValue(string const& name, const int& itemNum){

   if(!parse_)
      throw std::runtime_error(
            "CLP Exception : User must call Parse() prior to GetArgValue()"
            );

   ArgVec::iterator arg = find(argVec_.begin(),argVec_.end(),name);

   if(arg == argVec_.end()){
      PrintHelp();
      throw std::invalid_argument("CLP Exception : Arg " + name + " not found");
   }

   const string value = arg->Value(itemNum);
   if(value == "ERROR"){
      PrintHelp();
      throw std::invalid_argument(
            "CLP Exception : Requesting invalid value from Arg " + name
            );
   }

   return boost::lexical_cast<T>(value);
}

//!This member performs the work of parsing the command line list and 
//storing information relating to defined switches and arguments
void CommandLineParser::Parse(){

   typedef boost::tokenizer<boost::char_separator<char> > boostToken;
   boost::char_separator<char> delimiter("- ");
   StringVec::const_iterator inputIter = tokens_.begin();

   while(inputIter != tokens_.end()){

      // parse next token 
      StringVec tknVec;
      boostToken tkn(*inputIter,delimiter);
      boostToken::iterator iter=tkn.begin();

      // create vector from token for multi-arg options
      for(; iter!=tkn.end(); ++iter)
         tknVec.push_back(*iter);

      // search defined switches for match with command line input
      SwitchVec::iterator sw = 
         std::find(switchVec_.begin(), switchVec_.end(), tknVec[0]);

      if(sw != switchVec_.end()) sw->Set(true);

      // search defined arguments for match with command line input
      ArgVec::iterator arg = 
         std::find(argVec_.begin(), argVec_.end(), tknVec[0]);

      if(arg != argVec_.end()){
         // remove argument name from vector
         tknVec.erase(tknVec.begin());
         // set options by adding list to defined argument
         arg->Add(tknVec);
      }

      ++inputIter;
   }

   parse_ = true;
}

// call this after parsing to ensure that required arguments/switches have
// been set. This was originally intended to call within Parse() but this 
// produces a possible error if the user just wants to print the help. Help
// should be a required, builtin switch to solve this issue in the future.
void CommandLineParser::Validate(){

   SwitchVec::iterator swIter = switchVec_.begin();
   while( swIter != switchVec_.end()) {

      if( !swIter->Valid() ) 
      {
         PrintHelp();
         throw std::runtime_error(
               "CLP Exception: You have not set a required switch " +
               swIter->Name() + "\n See --help for options."
               );
      }
      ++swIter;
   }

   ArgVec::iterator argIter = argVec_.begin();
   while( argIter != argVec_.end())
   {
      if( !argIter->Valid() ) 
      {
         PrintHelp();
         throw std::runtime_error(
               "CLP Exception: You have not set a required argument " + 
               argIter->Name() + "\n See --help for options."
               );
      }
      ++argIter;
   }
}

void CommandLineParser::PrintHelp(){
   cout.fill(' ');
   cout << "   Available Arguments:" << endl;
   if(argVec_.empty())
      cout << "      No Arguments available" << endl;
   else
      Print<ArgVec>(argVec_);
   cout << endl;
   cout << "   Available Switches:" << endl;
   if(switchVec_.empty())
      cout << "      No Switchess available" << endl;
   else
      Print<SwitchVec>(switchVec_);
   cout << endl;
}

bool CommandLineParser::ArgSet(string const& name){
   if(!parse_) 
      throw std::runtime_error(
            "CLP Exception: User must call Parse() prior to ArgSet()"
            );
   ArgVec::iterator arg = find(argVec_.begin(), argVec_.end(), name);
   return arg == argVec_.end() ? false : arg->Set();
}

//!Returns true if the switch was found in the parsed input.
bool CommandLineParser::SwitchSet(string const& name){

   if(!parse_)
      throw std::runtime_error(
            "CLP Exception : User must call Parse() prior to GetArgValue()"
            );

   SwitchVec::iterator sw = find(switchVec_.begin(),switchVec_.end(), name);
   return sw == switchVec_.end() ? false : sw->Set();
}
#endif
