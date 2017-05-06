#include "dbg.h"
#include "options.h"
#include "args.h"

void print_help(){
  printf("\t\t\t\tLogfind \n");
  printf("usage: \n");
  printf("\tlogfind word1 [word2 [word3] ...] [-o] [-h | --help]\n");
  printf("\t\t-h|--help   display this\n");
  printf("\t\t-o look for ANY of the words (default: ALL)\n\n");
  printf("\tScan logfiles specified in ~/.logfind according\n");
  printf("\tto glob syntax (man 3 glob). Outputs filenames and\n");
  printf("\tspecific lines matching sought after words.\n");
  printf("\tthe matches are \e[4munderlined\e[0m.\n");
}

logfind_options parse_args( const int argc, char * argv []){
  // argument flags

  logfind_options options;
  options_init(&options);

  int i = 0;
  
  // we are going to have min argc -1 
  // and min argc -3 words
  char * words_temp[argc-1];
  int word_count = 0;

  // parse cmd line arguments
  for(i = 1; i<argc; i++){
    // -o flag
    if( strcmp(argv[i], "-o") == 0 ){
      options.logic  = 1;

    } else {
      // -h or --help
      if( (strcmp(argv[i], "-h") == 0) || 
          (strcmp(argv[i], "--help") == 0) ){
        options.help = 1;
      // else add as a word
      } else {
        words_temp[word_count] = argv[i];
        word_count++;
      }
    }
  }

  // make string array of just the words
  // we actually caught
  char * words[word_count];
  for(i = 0; i < word_count; i++){
    words[i] = words_temp[i];
  }
  options_set_keywords(&options, word_count,
                       words);

  return options;
}
