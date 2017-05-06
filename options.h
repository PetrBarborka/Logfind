#ifndef __options__h__
#define __options__h__

// ---- options.h -----------
// cmd args handling

typedef struct l_options {
  // display help? 1 or 0
  char help;
  // 0 == AND logic, 1 == OR logic
  char logic;
  char ** keywords;
  int num_keywords;
} logfind_options;

// get the initial structure, user has to free
// it when done
void options_init(logfind_options * options);

// free the memory allocated inside the structure
void options_purge(logfind_options * options);

// load and allocate the keywords into the structure
int options_set_keywords(logfind_options * options,
                          int num_keywords, char ** keywords);

#endif // __options__h__
