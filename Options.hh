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
  std::string helpdescription; //to be used in making a printable help
			       //message
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
  void AddOption(char c,
		 const std::string& s,
		 ARGSPEC argspec,
		 const std::string& helpdesc = "")
  {
    Flag tmp = {c,s,"",argspec,helpdesc};
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

  //Return option description string, useful for making a help message
  std::string GetOptionDesc(int spacing = 2)
  {
    using namespace std;
    int maxsize=0;
    for(optionspeciter i = options.begin();
	i != options.end();
	++i) {
      maxsize = (maxsize < i->second.longopt.size()) ?
	i->second.longopt.size() :
	maxsize;
    }
    stringstream ss;
    ss << "Options:";
    for(optionspeciter i = options.begin();
	i != options.end();
	++i) {
      ss << endl;
      ss << '-' << i->second.shortopt << ",--" << i->second.longopt << ':';
      int numspaces=maxsize - i->second.longopt.size()+spacing;
      for(int j = 0; j < numspaces; ++j) ss << ' ';
      ss << i->second.helpdescription;
    }
    return ss.str();
  }

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
