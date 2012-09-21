#ifndef OPTIONS_HH
#define OPTIONS_HH

#include <vector>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <getopt.h>

typedef int ARGSPEC;
//possible values are: required_argument, optional_argument, no_argument

class OptionSpec; //apparently I need this

struct Flag {
  char shortopt;
  std::string longopt;
  std::string arg;
  ARGSPEC argspec;
};

class Flags {
  std::multimap<char,Flag> flags;

public:
  //shorthand for flags iterator
  typedef std::multimap<char,Flag>::iterator flagiter;
  flagiter begin() {return flags.begin();}
  flagiter end() {return flags.end();}
  int size()
  {
    return flags.size();
  }
  void GetArgs(char c, std::vector<std::string>& v)
  {
    v.resize(0);
    typedef std::pair<flagiter,flagiter> flagpair;
    flagpair p = flags.equal_range(c);
    for(flagiter i = p.first;
	i != p.second;
	++i) {
      v.push_back(i->second.arg);
    }
    return;
  }

  //check to see if flag c is set:
  bool Check(char c)
  {
    return flags.find(c) != flags.end();
  }

  friend void parseargs(int argc, char** argv,
			OptionSpec& options, Flags& flags,
			std::vector<std::string>& args);
};

class OptionSpec {
  std::map<char,Flag> options;
public:
  //shorthand for iterator to options
  typedef std::map<char,Flag>::iterator optionspeciter;
  
  //add a flag with shortopt c and longopt s
  void AddOption(char c,const std::string& s,ARGSPEC argspec)
  {
    Flag tmp = {c,s,"",argspec};
    options[c]=tmp;
  }
  
  std::string GetShortOpts()
  {
    std::stringstream ss;
    for(optionspeciter i = options.begin(); i != options.end(); ++i) {
      ss << i->second.shortopt;
      switch(i->second.argspec) {
      case optional_argument:
	ss << "::";
	break;
      case required_argument:
	ss << ":";
	break;
      }
    }
    return ss.str();
  }

  int size() {return options.size();}

  friend void parseargs(int argc, char** argv,
			OptionSpec& options, Flags& flags,
			std::vector<std::string>& args);
};

class ParseException {
  std::string e;
public:
  ParseException() {}
  ParseException(const std::string& s) : e(s) {}
  std::string GetErrorString() {return e;}
};

void parseargs(int argc, char** argv,
	       OptionSpec& options, Flags& flags,
	       std::vector<std::string>& args);
#endif
