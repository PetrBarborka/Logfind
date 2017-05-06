#ifndef __files__h__
#define __files__h__

// ----- files.h ------
// which files to read and a function to do so

// unpack the files in configfile into files using glob
// returns mallocd array files and int num_files - user
// needs to free files (each and the whole array) themselves,
// also free num_files
int get_files_to_read(const char * configfile, char *** files,  
                      int * num_files);

// check file filename using options, print a file header
// and an underlined matched lines if matches found
int check_file(char * filename, logfind_options * options);

#endif
