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
    cmd.AddArg("x", "x argument does this", 1, true);
    cmd.AddArg("w", "write to file name", 1, true);
    cmd.AddArg("r", "read file name", 1, true);
    cmd.AddSwitch("a", "a switch");
    cmd.Parse();
    
    //tests
    BOOST_CHECK( cmd.GetArgValue<string>("x") == "argx");
    BOOST_CHECK( cmd.GetArgValue<string>("w") == "argw");
    BOOST_CHECK( cmd.GetArgValue<string>("r") == "argr");
    BOOST_CHECK( cmd.SwitchSet("a") == true);

    return 0;
}
