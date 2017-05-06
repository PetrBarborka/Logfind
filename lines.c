
#include "options.h"
#include "lines.h"

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <wordexp.h>

#define NDEBUG
#include "dbg.h"

#define MAX_OCCURENCES 100

int insert_into_string(char * original, char * input, const int where, char ** output){

  int needle = 0;
  int input_len = strlen(input);
  int total_len = input_len + strlen(original);

  for(; needle < where; needle++){
      (*output)[needle] = original[needle];
  }
  for(int i = 0; i < input_len; i++, needle++){
      (*output)[needle] = input[i];
  }
  for(; needle < total_len; needle++){
      (*output)[needle] = original[needle - input_len];
  }
  (*output)[total_len] = '\n';

  return 0;
}

int highlight_text(char * text, char ** out, int from, int to){
  char * highlight_start = "\e[4m";
  char * highlight_end = "\e[0m";
  char * temp = calloc(sizeof(char), strlen(text) + 5);
  check_mem(temp);
  insert_into_string(text, highlight_start, from, &temp);
  insert_into_string(temp, highlight_end,
                     to + 4, out);
  free(temp);
  temp = NULL;
  return 0;

error:
  if (temp) free(temp);
  temp = NULL;
  return -1;
}

int check_file(char * filename, logfind_options * options){

  wordexp_t exp_result;
  wordexp(filename, &exp_result, 0);
  filename = exp_result.we_wordv[0];
  FILE * fp = fopen(filename, "r");
  check_mem(fp);

  char * line = NULL;
  size_t len = 0;

  char header_print = 0;

  for(int i = 0; getline(&line, &len, fp) != -1; i++){
    if( strlen(line) && line[strlen(line) - 1] == '\n' ){
      line[strlen(line) -1] = '\0';
    }
    if ( check_line( line, options ) ){
      if ( !header_print ){
        print_file_header(filename);
        header_print = 1;
      }
      print_line(i, line, options);
    }


  }
  free(line); line=NULL;
  fclose(fp); fp=NULL;
  wordfree(&exp_result);
  return 0;

error:
  if ( line ) {
    free(line);
    line = NULL;
  }
  if ( fp ){
    fclose(fp);
    fp = NULL;
  }
  wordfree(&exp_result);
  return -1;
}


int find_all_occurences(const char * line, const logfind_options * options, 
                        int ** occurences, int ** num_occurences){
    int temp_occurences[MAX_OCCURENCES * 2];
    *num_occurences = calloc(sizeof(int), 1);
    check_mem(*num_occurences);
    **num_occurences = 0;

    for (int i = 0; i < options->num_keywords; i++){
      char * occurence = NULL;
      if ( (occurence = strstr(line, options->keywords[i])) ) {
        do {
          temp_occurences[**num_occurences * 2] = occurence - line;
          temp_occurences[**num_occurences * 2 + 1] =
                  temp_occurences[**num_occurences * 2]
                  + strlen(options->keywords[i]);
          **num_occurences += 1;
          check(**num_occurences < MAX_OCCURENCES, "Too many word occurences on a line.");
          debug("occurence: %c", *occurence);
        } while ( (occurence = strstr(occurence + strlen(options->keywords[i]),
                   options->keywords[i])) );
      }

  }

  *occurences = calloc(sizeof(int), (**num_occurences * 2));
  check_mem(*occurences);
  for (int i = 0; i < **num_occurences; i++){
    (*occurences)[i*2] = temp_occurences[i*2];
    (*occurences)[i*2 + 1] = temp_occurences[i*2 + 1];
  }

  return 0;
error:

  free(*num_occurences); *num_occurences = NULL;
  free(*occurences); *occurences = NULL;
  return -1;
}

char check_line(const char * line, const logfind_options *options){
  for (int i = 0; i < options->num_keywords; i++){
    if (strstr(line, options->keywords[i])){
      debug("line %s contains keyword %s", line, options->keywords[i]);
      if( options->logic ) {
        debug("logic is OR, line contains at least one of the keywords, quitting");
        return 1;
      }
    } else {
      debug("line %s doesn't contain keyword %s", line, options->keywords[i]);
      if( !options->logic ) {
        debug("logic is AND, line doesn't contain one of the keywords, quitting");
        return 0;
      }
    }
  }
  if (options->logic){
    debug("logic is OR, found none");
    return 0;
  } else {
    debug("logic is AND, found all");
    return 1;
  }
}

void print_file_header(const char * filename)
{
  printf("FILE: %s\n", filename);
}

int print_line(const int lineno, char * line, const logfind_options *options)
{
  int * occurences = NULL;
  int * num_occurences = NULL;

  int len_line = strlen(line);
  char * highlight_line = malloc(sizeof(char) * (len_line + 1));
  check_mem(highlight_line);

  int rc = find_all_occurences(line, options, &occurences, &num_occurences);
  check(rc == 0, "Finding all occurences");

  printf("\tline %d: %s\n", lineno, line);

  for (int i = 0; i < len_line; i++){
    highlight_line[i] = 32; //space
  }
  highlight_line[len_line] = '\0';
  for (int i = 0; i < *num_occurences; i++){
    int start = occurences[2*i];
    int end = occurences[2*i+1];
    for (int j = start; j < end; j++) {
      highlight_line[j] = 94;
    }
    debug("%d:%d, ", occurences[2*i], occurences[2*i+1]);
  }
  printf("\t\t%s\n", highlight_line);

  free(occurences); occurences = NULL;
  free(num_occurences); num_occurences = NULL;
  free(highlight_line); highlight_line = NULL;
  return 0;

error:
  if( occurences ) free(occurences);
  if( num_occurences ) free(num_occurences);
  if( highlight_line ) free(highlight_line);
  return -1;
}
