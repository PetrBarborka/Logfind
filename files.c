#include "options.h"
#include "files.h"

#include <wordexp.h>
#include <stdlib.h>
#include <glob.h>
#include <sys/stat.h>


#define NDEBUG 
#include "dbg.h" 

#define MAX_FILES 1024


int get_files_to_read(const char * configfile, char *** files,  
                      int * num_files){

  char * temp_files[MAX_FILES];
  *num_files = 0;
 
  wordexp_t exp_result;
  wordexp(configfile, &exp_result, 0);
  char * filename = exp_result.we_wordv[0];
  FILE * fp = fopen(filename, "r");
  check_mem(fp);

  char * line = NULL;
  size_t len = 0;

  glob_t glob_buf;

  getline(&line, &len, fp);
  if( line[strlen(line) - 1] == '\n' ){
    line[strlen(line) -1] = '\0';
  }
  glob(line, GLOB_TILDE, NULL, &glob_buf);

  while(getline(&line, &len, fp) != -1){
    if( line[strlen(line) - 1] == '\n' ){
      line[strlen(line) -1] = '\0';
    }
    glob(line, GLOB_TILDE| GLOB_APPEND, NULL, &glob_buf);
  }
  debug("globbed files:\n");
  for(int i = 0; i < glob_buf.gl_pathc; i++){
    struct stat stat_buf;

    char * curr_file = glob_buf.gl_pathv[i];
    int status = stat(curr_file, &stat_buf);
    check(status >= 0, "File status check failed");
    if ( S_ISREG(stat_buf.st_mode) ) {
      check(*num_files < MAX_FILES, "Too many files");
      debug("file\t%s\n", curr_file);
      temp_files[*num_files] = malloc(sizeof(char) *
                                    (strlen(curr_file) + 1));
      check_mem(temp_files[*num_files]);
      strcpy(temp_files[*num_files], curr_file);
      *num_files += 1;
    } else {
      debug("dir\t%s\n", curr_file);
    }
  }

  if (*num_files) {
    (*files) = malloc(sizeof(char *) * (*num_files));
    for( int i = 0; i < *num_files; i++){
      char * tf = temp_files[i];
      (*files)[i] = malloc(sizeof(char) * (strlen(tf) + 1));
      check_mem((*files)[i]);
      strcpy((*files)[i], tf);
    }
  }

  for ( int i = 0; i < *num_files; i++) free(temp_files[i]);
  free(line); line = NULL;
  fclose(fp); fp = NULL;
  wordfree(&exp_result);
  globfree(&glob_buf);

  return 0;

error:
  for ( int i = 0; i < *num_files; i++) free(temp_files[i]);
  if(line) {
     free(line);
     line = NULL;
  }
  if(fp) {
    fclose(fp);
    fp = NULL;
  }
  wordfree(&exp_result);
  globfree(&glob_buf);
  return -1;
}
