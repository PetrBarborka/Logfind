#ifndef __lines__h__
#define __lines__h__

// unused fcn - insert a string into a string at where. You need to allocate
// and free output with the right size (original + input)
int insert_into_string(char * original, char * input, const int where, char ** output);

// unused fcn to highlight an area of a string form and to a certain index
// with \e[4m and \e[0m. The user needs to preallocate out to a length
// strlen(text) + 9 and then free it. Doesn't handle overlapping of highlight
// sections well.
int highlight_text(char * text, char ** out, int from, int to);

// go through a file and print its name and a matching line
// uppon matches
int check_file(char * filename, logfind_options * options);

// find all occurences of keywords in options in a string. Assumes
// occurences exist ( use check_line to make sure ). Allocates
// occurences and num_occurences, the user needs to free them
// when done
int find_all_occurences(const char * line, const logfind_options * options,
                        int ** occurences, int ** num_occurences);

// check whether line has the right keywords with given logic
// returns 1 == yes, 0 == no
char check_line(const char * line, const logfind_options *options);

// print the name of the file
void print_file_header(const char * filename);

// print a line, highlight keyword occurences, assumes keywords
// match (use check_line() before)
int print_line(const int lineno, char * line, const logfind_options *options);

#endif
