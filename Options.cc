#include "Options.hh"

void parseargs(int argc, char** argv,
	       OptionSpec& options, Flags& flags,
	       std::vector<std::string>& args)
{
  char optchar = 0;
  const int numopts = options.size();
  option longopts[numopts];

  std::string shortoptsstr = options.GetShortOpts();

  char shortopts[shortoptsstr.size()+1];
  for(int i = 0; i < shortoptsstr.size(); ++i) {
    shortopts[i] = shortoptsstr[i];
  }
  for(int i = 0; i < numopts; ++i) longopts[i].flag=0;
  int index=0;
  for(OptionSpec::optionspeciter i = options.options.begin();
      i != options.options.end();
      i++) {
    longopts[index].name = i->second.longopt.c_str();
    longopts[index].val = i->second.shortopt;
    longopts[index].has_arg = i->second.argspec;
    index++;
  }
  while( (optchar = getopt_long(argc,argv,shortopts,longopts,0))
	 != -1 && optchar != '?') {
    Flag currentflag = options.options[optchar];
    int tmp = options.options[optchar].argspec;
    if(tmp != no_argument) {
      Flag tmpflag = currentflag;
      tmpflag.arg = optarg == 0 ? "" : optarg;
      flags.flags.insert(std::make_pair(optchar,tmpflag));
    }
    else
      flags.flags.insert(std::make_pair(optchar,currentflag));
  }
  if(optchar == '?')
    throw ParseException("Error"); //I'm not sure what it means at the moment
  int j = 0;
  args.resize(argc-optind);
  for(int i = optind; i < argc; (++i, ++j)) {
    args[j] = argv[i];
  }
  return;
}
