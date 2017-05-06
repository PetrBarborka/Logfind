# Project Logfind

This is my shot at excercise 26 of the Learn C the hard way book by Zed Shaw.

tldr: searches files for keywords.

It is a small program emulating some of the functionality of grep. Fill the file
./logfind with names of files you want scanned ( allowing for glob syntax and 
effectively also discarding lines starting with # ), specify keywords you
are interested in and get names of the files containing them and list of rows
with matches highlighted. 

usage: see ./logfind --help or the print_help() fcn in the args.c file.
