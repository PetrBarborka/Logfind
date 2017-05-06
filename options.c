#include "options.h"
#include "dbg.h"
#include <stdlib.h>

void options_init(logfind_options * options){
  options->help = 0;
  options->logic = 0;
  options->keywords = NULL;
  options->num_keywords = 0;
}

void options_purge(logfind_options * options){
  options->help = 0;
  options->logic = 0;
  if ( options->keywords ) { 
    for (int i = 0; i < options->num_keywords; i++) {
      if ( options->keywords[i] ){
        free( options ->keywords[i] );
      }
    }
    free(options->keywords); 
  }
  options->keywords = NULL;
  options->num_keywords = 0;
}

int options_set_keywords(logfind_options * options,
                          int num_keywords, char ** keywords){
  // allocate pointers to elements
  options->keywords = calloc(sizeof(char *), num_keywords);
  check_mem(options->keywords);
  for(int i = 0; i < num_keywords; i++){
    options->keywords[i] = calloc(sizeof(char), strlen(keywords[i]) + 1);
    strcpy(options->keywords[i], keywords[i]);
    check( options->keywords[i] != NULL, 
           "Failed to copy keyword %d: %s",
           i, keywords[i] );
  }
  options->num_keywords = num_keywords;
  return 0;

error:
  options_purge(options);
  return -1;
}
