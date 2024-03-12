/*
AUTHOR: Niall Bastible Díaz | 22314443 | niall.bastiblediaz2@mail.dcu.ie
ACKNOWLEDGEMENTS
    I understand that the University regards breaches of academic integrity and plagiarism as grave and serious.

    I have read and understood the DCU Academic Integrity and Plagiarism Policy.
    I accept the penalties that may be imposed should I engage in practice or practices that breach this policy.

    I have identified and included the source of all facts, ideas, opinions and viewpoints of others in the assignment references.
    Direct quotations, paraphrasing, discussion of ideas from books, journal articles, internet sources, module text, or any other
    source whatsoever are acknowledged and the sources cited are identified in the assignment references.

    I declare that this material, which I now submit for assessment, is entirely my own work and has not been taken from the work of
    others save and to the extent that such work has been cited and acknowledged within the text of my work.
*/

// functions that process the commandline arguments
// convention in msh is for the args to be char** and terminated by NULL

#include <stdlib.h>

#define MAX_CMD_LEN 1024 // the maximum length of an input command
#define MAX_ARGS 64 // the maximum number of parsed arguments.

#define ARG_WHITESPACE " \n\t\r" // a rudimentary macro that contains whitespace.
#define ARG_QUOTES "\"'`" // the chars that are considered quotes and will group words together

// take a cmd buffer and split it into tokens, placing each into the argarr and terminating with NULL
// returns the number of args found, which cannot be more than MAX_ARGS
int splitargs(char cmdstr[MAX_CMD_LEN], char* argarr[MAX_ARGS]);

// given a string, evaluate which of the builtins it is
// if it is not a builtin, return -1
int parsebuiltin(const char cmd[]);

// look for >s in the args, and perform the redirection on  target files
// each redirection and its target will be replaced with a blank string
// if user attempts multiple >s, the first is applied and the rest ignored, but still removed as above
// returns the number of arguments that have been removed
int parseioredirects(char* args[MAX_ARGS]);

// remove all blank strings from an arg arr, reterminating as needed
// returns the new length
int cleanargs(char* args[MAX_ARGS]);

// concatenate an array of NULL-terminated strings into a single, space separated string
// unneeded
// void concatstrs(char dest[], char* srcstrs[MAX_ARGS]);

// parse the & at the end of the command
// cmdargs is an array of strings terminated by NULL
// returns true if the final arg is &, and also removes it if it is
int parsebackground(char* cmdargs[MAX_ARGS]);

// expand arguments that begin with a prefix like $ to their environment variables
// note that the assigned pointers lead to memory that may not belong to msh - take care
// if a request variable like TERM isn't set, it will remain as $TERM
void expandvars(char* cmdargs[MAX_ARGS]);

// if the first arg is an alias, expand it
void expandalias(char* cmdargs[MAX_ARGS]);