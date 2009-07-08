#include <boost/test/minimal.hpp>
#include <clp/CommandLineParser.hpp>

int test_main( int argc , char ** argv )             // note the name!
{
  //-if ../links/h1 -of ../dat/h1_148.8km.dat -h0 148.1

    //simulate command-line arguments
    argc = 5;
    argv[0] = (char*)"./test";
    argv[1] = (char*)"-if ../links/h1";
    argv[2] = (char*)"-of ../data/h1_148.8km.dat";
    argv[3] = (char*)"-h0 148.1";
    argv[4] = (char*)"-a";

    //create parser and add arguments/switches
    CommandLineParser cmd(argc,argv);
    cmd.AddArg("if", "input file", 1, true);
    cmd.AddArg("of", "output file", 1, true);
    cmd.AddArg("h0", "starting height", 1, true);
    cmd.AddSwitch("a", "a switch");
    cmd.Parse();
    
    //tests
    BOOST_CHECK( cmd.GetArgValue<string>("if") == "../links/h1");
    BOOST_CHECK( cmd.GetArgValue<string>("of") == "../data/h1_148.8km.dat");
    BOOST_CHECK( cmd.GetArgValue<float>("h0") == 148.1f);
    BOOST_CHECK( cmd.SwitchSet("a") == true);

    return 0;
}

