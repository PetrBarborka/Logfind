#ifndef __args__h__
#define __args__h__

// ---- args.h ----
// handles command line arguments 

// print usage message in case user doesn't provide
// enough arguments or requests --help
void print_help();

// parses command line arguments into a structure.
// the structure needs to be freed by the user when
// you are done with it using options_purge(&options);
logfind_options parse_args( const int argc, char * argv []);

#endif // __args__h__
