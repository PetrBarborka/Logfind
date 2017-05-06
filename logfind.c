#define NDEBUG
#include "dbg.h"

#include "options.h"
#include "args.h"
#include "files.h"
#include "lines.h"

#include <wordexp.h>
#include <stdlib.h>
#include <glob.h>
#include <sys/stat.h>

/* ----------- Logfind program -------------
 * Given keywords to search for, the program
 * scans through lines of files defined in 
 * ~/.logfind (allows glob() syntax. If it
 * finds lines that contain all or any (-o flag)
 * of the keywords, it prints the name of the file
 * and the contents of the line, highlighting the
 * matches.
 */

// load cmd arguments, initialize options,
// print help message or get to work
int main(int argc, char * argv[]){
  logfind_options options = parse_args(argc, argv);

  debug("options.help: %d", options.help);
  debug("options.logic: %d", options.logic);
  debug("options.num_keywords: %d", options.num_keywords);

  for(int i = 0; i < options.num_keywords; i++){
    debug("options.keywords[%d]: %s", options.num_keywords, options.keywords[i]);
  }

  // use arguments - help or work
  if( options.help || options.num_keywords == 0 ){
    print_help();
  } else {
    if ( options.logic ){
      debug("Going to use or logic.");
    }
    for(int i = 0; i < options.num_keywords; i++){
      debug("word %d: %s", i, options.keywords[i]);
    }

    char * config_file = "~/.logfind";
    char ** files;
    int * num_files = malloc(sizeof(int));
    get_files_to_read(config_file, &files, num_files);
    for( int i = 0; i < *num_files; i++){
      debug("checking file %s", files[i]);
      check_file(files[i], &options);

      free(files[i]);
    }
    free(files);
    free(num_files);
  }


  options_purge(&options);
}
