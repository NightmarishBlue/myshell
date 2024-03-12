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

#include "../headers/config.h"

#include "../main.h"

#include <stdio.h>
#include <string.h>

// the shell options
alias* aliases;
int aliaslen = 0;

int longpath = 1;

void printprompt()
{
    char* pwd = (longpath) ? cwd : strrchr(cwd, '/'); // the working directory to print
    printf("%s 8> ", pwd);
}

// wrapper for strdup that prints an error message
char* dupwerr(char* string)
{
    char* new = strdup(string);
    if (new == NULL)
    {
        fprintf(stderr, "msh: could not allocate heap memory for '%s': ", string);
        perror("");
    }
    return new;
}

// another string processing helper
int cntwrds(char* string)
{
    int len = 0;
    while (*string != '\0')
    {
        string += strspn(string, ARG_WHITESPACE);
        if (*string == '\0')
            continue;
        string += strcspn(string, ARG_WHITESPACE);
        len++;
    }
    return len;
}

int idalias(char* name)
{
    for (int i = 0; i < aliaslen; i++)
    {
        alias* curr = aliases + i;
        if (curr == NULL) continue;
        if (strcmp(curr->name, name) == 0) return i;
    }
    return -1;
}

const alias* getalias(int index)
{
    if (index < 0 || index >= aliaslen) return NULL;
    return aliases + index;
}

int pshalias(char* input, char* output)
{
    // allocate heap for both of these strings
    char* name = dupwerr(input);
    if (name == NULL)
        return 0;

    char* expans = dupwerr(output); // the expansion of the alias - will be broken by strtok
    if (expans == NULL)
    {
        free(name);
        return 0;
    }

    // figure out how big the expansion should be as a set of args
    int arglen = cntwrds(expans);
    char** args = malloc(arglen * sizeof(char*));
    if (args == NULL)
    {
        perror("msh: could not allocate space for alias expansion");
        free(name); free(expans);
        return 0;
    }

    // we can also populate the args now (the code might fail later but it is more readable this way)
    int index = 0;
    char* tok = strtok(expans, ARG_WHITESPACE);
    while (tok != NULL)
    {
        args[index++] = tok;
        tok = strtok(NULL, ARG_WHITESPACE);
    }

    // now a new alias must be memory allocated too
    if (aliaslen < 1) // if no aliases have been made, we must allocate the new array
    {
        aliases = malloc(sizeof(alias));
        if (aliases == NULL)
        {
            perror("msh: could not allocate space for alias");
            free(name); free(expans); free(args);
            return 0;
        }

        // assign this new alias's input and output
        alias* new = aliases;
        new->name = name;
        new->arglen = arglen;
        new->expargs = args;
        
        aliaslen++;
        return 1;
    }

    // otherwise, try to resize the array of aliases
    alias* new = reallocarray(aliases, aliaslen + 1, sizeof(alias));
    if (new == NULL) // make sure to handle the case of an error happening
    {
        perror("msh: could not allocate space for alias");
        free(name); free(expans);
        return 0;
    }

    aliases = new; // move aliases to point to the right block
    new = aliases + aliaslen++; // new is our new alias - also increment the length

    new->name = name;
    new->arglen = arglen;
    new->expargs = args;
    return 1;
}

void freealiases()
{
    for (int i = 0; i < aliaslen; i++)
    {
        alias* curr = aliases + i;
        free(curr->name);
        free(curr->expargs);
    }
    free(aliases);
    aliases = NULL;
    aliaslen = 0;
}