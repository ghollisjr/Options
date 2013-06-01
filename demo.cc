/*
  Demonstration program for the Options getopt_long wrapper functions
  and classes.

  To use the wrapper utilities, you have the following:

  OptionSpec: a class for holding your desired options.  Each option
  specification consists of 1. The character for the short option,
  2. A string for the long option, and 3. An integer denoting whether
  the option has no argument, a required argument, or an optional
  argument.

  Flags: a class for storing the flags which are given to the program
  via the short and long options you've defined with the OptionSpec.
  This class has the member function void GetArgs(char c,
  vector<string>& v) which stores arguments to an option c in v.

  parseargs: After defining your options with an OptionSpec and
  creating an empty Flags object and a vector<string> object to store
  leftover arguments, run parseargs to actually parse the arguments
  with getopt_long.
 */

#include "Options.hh" //It's just one header
#include <iostream>
#include <vector> //not technically necessary, already in Options.hh
#include <string> //also not necessary

int main(int argc, char** argv)
{
  OptionSpec options; //Here's our option definition object

  //To add an option to our option definition, we use AddOption which
  //takes the short option character, the long option string, and then
  //the integer for whether it takes an argument:

  //Add an input file name flag which requires an argument:
  options.AddOption('i',"inputfile",required_argument,"select input file");

  //Add a help flag which needs no argument:
  options.AddOption('h',"help",no_argument,"show this help message");

  //Add a miscellaneous flag which has an optional argument:
  options.AddOption('m',"misc",optional_argument,"miscellaneous option");
  //note that per the GNU extension, the argument must be given like
  //-mMYARGUMENT; if it is given like -m MYARGUMENT it will be ignored
  //and sent to the leftover arguments.

  //Create our flag holder:
  Flags flags;

  //Create a vector to store leftover arguments:
  std::vector<std::string> leftovers;

  //Now we can parse the arguments:
  parseargs(argc,argv,options,flags,leftovers);

  //Did the user ask us for help?
  if(flags.Check('h')) {
    std::cerr << "Usage: demo [options]" << std::endl << std::endl;
    //using convenient usage generator:
    std::cerr << options.GetOptionDesc() << std::endl;
    return 1;
  }

  //Perhaps we'd like to get all the input file names given:
  std::vector<std::string> infilenames;
  flags.GetArgs('i',infilenames);
  //infilenames will now store all the input file name arguments in
  //the order they were given.

  //We can also get the miscellaneous option arguments:
  std::vector<std::string> miscargs;
  flags.GetArgs('m',miscargs);
  //miscargs now stores the arguments passed to miscargs; if there was
  //no argument passed to -m/--misc, then that particular entry in the
  //vector will be an empty string, "".

  //Let's print out our set flags:
  std::cout << "Input file names:" << std::endl;
  for(int i =0; i < infilenames.size(); ++i) {
    std::cout << infilenames[i] << std::endl;
  }
  std::cout << "Misc. flags:" << std::endl;
  for(int i = 0; i < miscargs.size(); ++i) {
    std::cout << miscargs[i] << std::endl;
  }

  //We can also look at the extra arguments given via the commandline
  //which were not parsed by getopt_long:
  std::cout << "Extra arguments:" << std::endl;
  for(int i = 0; i < leftovers.size(); ++i) {
    std::cout << leftovers[i] << std::endl;
  }
  return 0;
}
