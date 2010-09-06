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
#include <boost/test/minimal.hpp>
#include <clp/CommandLineParser.hpp>


//int main(int argc, char **argv){
int test_main( int argc , char ** argv )             // note the name!
{

    //simulate command-line arguments
    argc = 5;
    argv[0] = (char*)"./test";
    argv[1] = (char*)"-x argx";
    argv[2] = (char*)"-w argw";
    argv[3] = (char*)"-r argr";
    argv[4] = (char*)"-a";


    //create parser and add arguments/switches
    CommandLineParser cmd(argc,argv);
    Arg argX("x", "x argument does this", 1, true);
    Arg argW("w", "write to file name", 1, true);
    Arg argR("r", "read file name", 1, true);
    Switch switchA("a", "a switch", true);

    cmd.AddArg(argX);
    cmd.AddArg(argW);
    cmd.AddArg(argR);
    cmd.AddSwitch(switchA);
    cmd.Parse();
    
    //tests
    BOOST_CHECK( cmd.GetArgValue<string>("x") == "argx");
    BOOST_CHECK( cmd.GetArgValue<string>("w") == "argw");
    BOOST_CHECK( cmd.GetArgValue<string>("r") == "argr");
    BOOST_CHECK( cmd.SwitchSet("a") == true);

    return 0;
}
