#ifndef CCSTRING_H_
#define CCSTRING_H_

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

/**
 *
 * this is a small library of useful string utilities that gets
 * added to as new things come up
 *
 * to use this, include this header and compile with -lccstring
 *
 **/


//
// useful ? macros
//
#define DBLCHAR_EACH(dchar,idx,action) for (idx=0; dchar[idx]; idx++) { action ; }
#define DBLCHAR_FOREACH(dchar, idx) for (idx=0; dchar[idx]; idx++)

#define foreach_dblchar(dchar, idx) for (idx=0; dchar[idx]; idx++)
#define free_dblchar(dchar) {int _i_=0;while(dchar[_i_]) free(dchar[_i_++]);free(dchar);}

#define dblchar_dump(dchar) {int _i_=0;while(dchar[_i_]) printf("%s\n",dchar[_i_++]);}

typedef void (ccstr_file_fn) (int nlines, int cline, char *line, void *data);

char **dblchar_new ();
int dblchar_add (char ***dchar, char *value);
void dblchar_free (char **dchar);
char ***trplchar_new ();
int trplchar_add (char ****tchar, char **list);
void trplchar_free (char ***tchar);
void dblchar_print_v (char **dchar) ;
void dblchar_print_h (char **dchar) ;
void trplchar_print (char ***tchar) ;

static inline int ischar (char *s, char c) {return (s && !s[1] && s[0] == c);}

//-------------------------------------------------------------------------
//
// free a void **variable with a free function of the form
// void freefunction (void *) (can be free itself)
//
//-------------------------------------------------------------------------
void dblvoidfree (void **p, int n, void (*free_fn)(void *));
//-------------------------------------------------------------------------
//
// copy over an entire string into the memory location at 'to'. assumes that
// 'from' is null terminiated. It makes a copy of the 'from' pointer, so
// it can be used without the side effect of losing the original
// 'from' pointer
//
//-------------------------------------------------------------------------
void ccstr_pcpy (char **to, char *from, int finalize);
//-------------------------------------------------------------------------
//
// is a character in the instring? ueseful for checking whether a character
// equals any in a set of characters
//
//-------------------------------------------------------------------------
int ccstr_charin (char *c, char *instring);
//-------------------------------------------------------------------------
//
// fancier version of ccstr_charin, returns the index of the matching character
// inside the passed in map of characters
//
//-------------------------------------------------------------------------
int ccstr_matchmap (char *test, char *map);
//-------------------------------------------------------------------------
//
// ccstr_substr
// if start is negative then start that many characters in from the right
// if length is 0 then get all chars to end of string
// if length is negative then leave that many chars off the end
// if length is more than the available characters just get as many as
//    possible
//
// make sure to free!
//-------------------------------------------------------------------------
char *ccstr_substr (const char *input, int start, int length);
//-------------------------------------------------------------------------
//
// split a string by seperator - make sure to free
//
//-------------------------------------------------------------------------
char **ccstr_split (const char seperator, const char *input);
//-------------------------------------------------------------------------
//
// split a string by seperator into n sections - make sure to free
//
//-------------------------------------------------------------------------
char **ccstr_split_n (const char seperator, const char *input, int n);
//-------------------------------------------------------------------------
//
// join assumes array is null terminated
//
//-------------------------------------------------------------------------
char *ccstr_join (const char seperator, char **stringarray);
//-------------------------------------------------------------------------
//
// concat does exactly what you think it does, make sure to FREE
//
//-------------------------------------------------------------------------
char *ccstr_concat (char *a, char *b);
char *ccstr_concat3 (char *a, char *b, char *c);
char *ccstr_concat4 (char *a, char *b, char *c, char *d);
//-------------------------------------------------------------------------
//
// concatenate together a list of strings
//
//-------------------------------------------------------------------------
char *ccstr_concatenate_list (char **list, int size)
;
//-------------------------------------------------------------------------
//
// concatenate together a list of strings, last one must be NULL
//
//-------------------------------------------------------------------------
char *ccstr_cat (char *a, ...)
;
//-------------------------------------------------------------------------
//
// nest a string within another string, on either side of it
//
//-------------------------------------------------------------------------
char *ccstr_nest (char *input, char *border)
;//-------------------------------------------------------------------------
//
// replace the first occurance in target of findstring with
// replacementstring
//
//-------------------------------------------------------------------------
char *ccstr_replace (char *target, char *findstring, char *replacementstring)
;//-------------------------------------------------------------------------
//
// count the number of times a string appears within another string
//
//-------------------------------------------------------------------------
int ccstr_occurs (char *target, char *find)
;
//-------------------------------------------------------------------------
//
// replace all occurances in target of findstring with
// replacementstring
//
//-------------------------------------------------------------------------
char *ccstr_replace_all (char *target, char *findstring, char *replacementstring)
;
//-------------------------------------------------------------------------
//
// replace any character in the map with the replacement string given
// requires 2 passes of the input to avoid limiting the algorithm by
// constraining size, or by doing repeated reallocs
//
//-------------------------------------------------------------------------
char *ccstr_replace_all_map (char *target, char *map, char *replacementstring)
	;
//-------------------------------------------------------------------------
//
// replace all characters in map with strings in the second map, respective
// of order
//
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// assuming a and b are pointers to different locations within a string,
// and a is less than b, what is the number of characters that they make
// up between them, inclusive of each pointer's character? they may
// point to the same location, in which case the length is 1
//
//-------------------------------------------------------------------------
int ccstr_dist (char *a, char *b)
	;
//-------------------------------------------------------------------------
//
// return a new string which is a copy of the bytes within a string from
// pointer a to pointer b within that string. It is assumed that a is less
// than b, or this wont make sense
//
//-------------------------------------------------------------------------
char *ccstr_bcpy (char *a, char *b)
	;
//-------------------------------------------------------------------------
//
// trim all whitespace from the end of a string and the beginning of
// the string - space, tab, newline, carriage return
//
//-------------------------------------------------------------------------
char *ccstr_trim (char *in)
	;
//-------------------------------------------------------------------------
//
// first trims off whitespace, then trims off quotes if they are there
//
//-------------------------------------------------------------------------
char *ccstr_trimquotes (char *in)
	;
//-------------------------------------------------------------------------
//
// copy byte chunk and trim result
//
//-------------------------------------------------------------------------
char *ccstr_bcpy_trim (char *a, char *b)
	;
//-------------------------------------------------------------------------
//
// copy byte chunk and trim results with quote trim
//
//-------------------------------------------------------------------------
char *ccstr_bcpy_ccstr_trimquotes (char *a, char *b)
	;
//-------------------------------------------------------------------------
//
// split, but concious of matching quotes and brackets. So, if a split
// operator comes inside a paired set it is ignored. This is handy for
// spliting out arguement strings - using this on a bracket or quote
// will have strange and undefined results
//
//-------------------------------------------------------------------------
char **ccstr_tokenize (char *input, const char seperator, int tquotes)
	;
//-------------------------------------------------------------------------
//
// this parses out a standard arguement list of values seperated by commas
// it assumes that arguments with quotes are to be stripped of quotes, like
// a comma delimited file, which would be another excellent use for this
// function
//
//-------------------------------------------------------------------------
char **ccstr_arglist (char *input)
;
//-------------------------------------------------------------------------
//
// just get a ccstr_valuelist seperated by seperator with just regular whitespace
// trimming
//
//-------------------------------------------------------------------------
char **ccstr_valuelist (char *input, const char seperator)
;
//-------------------------------------------------------------------------
//
// given a string in key=value format, parse them out and return them
// WARNING: Potential memory leak!!!!!
// 		call with NON-ALLOCATED key and value pointers
//
//-------------------------------------------------------------------------
void ccstr_keyvalue (char *input, char **key, char **value)
;
//-------------------------------------------------------------------------
//
// Converts a hex character to its integer value
//
//-------------------------------------------------------------------------
char ccstr_from_hex(char ch)
;
//-------------------------------------------------------------------------
//
// Converts an integer value to its hex character
//
//-------------------------------------------------------------------------
char ccstr_to_hex(char code)
;
//-------------------------------------------------------------------------
//
// Returns a url-encoded version of str (needs FREE)
//
//-------------------------------------------------------------------------
char *ccstr_url_encode (char *str)
;
//-------------------------------------------------------------------------
//
// Returns a url-decoded version of str (needs FREE)
//
//-------------------------------------------------------------------------
char *ccstr_url_decode (char *str)
;
//-------------------------------------------------------------------------
//
// escape internal ones
//
//-------------------------------------------------------------------------
char *ccstr_escapequotes (char *str)
;
//-------------------------------------------------------------------------
//
// change a whole string to lowercase
//
//-------------------------------------------------------------------------
char *ccstr_lc (char *str)
;
//-------------------------------------------------------------------------
//
// append an entire string to a passed in buffer
//
//-------------------------------------------------------------------------
void ccstr_append (char **buffer, char *format, ... )
	;
//-------------------------------------------------------------------------
//
// the selftest
//
//-------------------------------------------------------------------------
int ccstring_selftest ()
;
//-------------------------------------------------------------------------
//
// read in a file into a char **
// needs free
//
//-------------------------------------------------------------------------
char **ccstr_read_file (char *filename, int *numlines)
	;
//-------------------------------------------------------------------------
//
// read in a file and pass each line to a function
//
//-------------------------------------------------------------------------
char **ccstr_read_file_function (char *filename, int *numlines, ccstr_file_fn *f, void *data)
	;


#endif

