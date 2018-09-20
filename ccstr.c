
#include "ccstr.h"
/**
 *
 * this is a small library of useful string utilities that gets
 * added to as new things come up
 *
 * to use this, include this header and compile with -lccstring
 *
 **/

// -------------------------------------------------------------------------
//
// string lists (double char pointers), just manage lists of strings
//
// -------------------------------------------------------------------------
inline char **dblchar_new () {
	return (char **) calloc (0, sizeof (char *));
}
inline int dblchar_add (char ***dchar, char *value) {
	char **cc = *dchar;
	int i = 0;
	if (cc != NULL) foreach_dblchar (cc, i) i = i;
	cc = (char **) realloc (cc, sizeof (char *) * (i+2));
	if (cc == NULL) exit (200);
	cc[i] = strdup (value);
	cc[i+1] = NULL;
	*dchar = cc;
	return (i+1);
}
inline void dblchar_free (char **dchar) {
	int i = 0;
	foreach_dblchar (dchar, i) free (dchar[i]);
	free (dchar);
}
void dblchar_print_v (char **dchar) {
	int i;
	foreach_dblchar (dchar, i) printf ("%s\n", dchar[i]);
}
void dblchar_print_h (char **dchar) {
	int i;
	foreach_dblchar (dchar, i) printf ("%s   ", dchar[i]);
}
// -------------------------------------------------------------------------
//
// lists of lists that are strings (triple char pointers) - manage
//
// -------------------------------------------------------------------------
inline char ***trplchar_new () {
	return (char ***) calloc (0, sizeof (char **));
}
inline int trplchar_add (char ****tcharp, char **list) {
	char ***tchar = *tcharp;
	int i = 0;
	if (tchar != NULL) foreach_dblchar (tchar, i) i = i;
	tchar = (char ***) realloc (tchar, sizeof (char **) * (i+2));
	if (tchar == NULL) exit (200);
	tchar[i] = list;
	tchar[i+1] = NULL;
	*tcharp = tchar;
	return (i+1);
}
inline void trplchar_free (char ***tchar) {
	int i;
	foreach_dblchar (tchar, i) dblchar_free (tchar[i]);
	free (tchar);
}
void trplchar_print (char ***tchar) {
	int i;
	printf ("[\n");
	foreach_dblchar (tchar, i) {
		printf ("   [   ");
		dblchar_print_h (tchar[i]);
		printf ("]\n");
	}
	printf ("]\n");
}

//inline int ischar (char *s, char c) {
//	return (s && !s[1] && s[0] == c);
//}
//-------------------------------------------------------------------------
//
// free a void **variable with a free function of the form
// void freefunction (void *) (can be free itself)
//
//-------------------------------------------------------------------------
void dblvoidfree (void **p, int n, void (*free_fn)(void *))
{
	int i=0;
	for (i=0; i<n; i++) free_fn (p[i]);
	free (p);
}

//-------------------------------------------------------------------------
//
// copy over an entire string into the memory location at 'to'. assumes that
// 'from' is null terminiated. It makes a copy of the 'from' pointer, so
// it can be used without the side effect of losing the original
// 'from' pointer
//
//-------------------------------------------------------------------------
void ccstr_pcpy (char **to, char *from, int finalize)
{
	char *temp = from;
	char *copytome = *to;
	while (*temp)
	{
		*copytome++ = *temp++;
	}
	if (finalize) *copytome = '\0';
	*to = copytome;
}

//-------------------------------------------------------------------------
//
// is a character in the instring? ueseful for checking whether a character
// equals any in a set of characters
//
//-------------------------------------------------------------------------
int ccstr_charin (char *c, char *instring)
{
	while (*instring && *c != *instring) instring++;
	return (*c == *instring);
}

//-------------------------------------------------------------------------
//
// fancier version of ccstr_charin, returns the index of the matching character
// inside the passed in map of characters
//
//-------------------------------------------------------------------------
int ccstr_matchmap (char *test, char *map)
{
	int i=0, result=-1;
	for (i=0; map[i]; i++)
	{
		if (*test == map[i])
		{
			result = i;
			break;
		}
	}
	return result;
}

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
char *ccstr_substr (const char *input, int start, int length)
{
	int i, len = strlen(input);
	int a = (start >= 0) ? start : len+start;
	int b = (length < 0) ? len+length : ((length==0) ? len : a+length );
	if (b > len) b = len;
	char *result = (char *)malloc(sizeof(char)*((b-a)+1)), *p = result;
	if (result==NULL){fprintf(stderr, "out of memory ccstr_substr\n");exit (200);}
	for (i=a; i<b; i++) *p++ = input[i];
	*p = '\0';
	return result;
}
//-------------------------------------------------------------------------
//
// split a string by seperator - make sure to free
//
//-------------------------------------------------------------------------
char **ccstr_split (const char seperator, const char *input)
{
	int sections = 1;
	int i, j, start;
	for (i=0; input[i]; i++) sections += (input[i]==seperator);
	char **results = (char **)calloc( sections+1, sizeof(char *) );
	if (results==NULL){fprintf(stderr, "out of memory ccstr_split\n");exit (200);}
	results[sections] = NULL;
	for (i=0, j=0, start=0; input[i]; i++)
	{
		if (input[i]==seperator)
		{
			results[j] = ccstr_substr(input, start, i-start);
			j++;
			start=i+1;
		}
	}
	results[j] = ccstr_substr(input, start, i-start);
	return results;
}
//-------------------------------------------------------------------------
//
// split a string by seperator - make sure to free
//
//-------------------------------------------------------------------------
char **ccstr_split_n (const char seperator, const char *input, int n)
{
	int sections = n;
	int i, j, start;
	char **results = (char **)calloc( sections+1, sizeof(char *) );
	if (results==NULL){fprintf(stderr, "out of memory ccstr_split_n\n");exit (200);}
	for (i=0; i<sections+1; i++) results[i] = NULL;
	for (i=0, j=0, start=0; input[i]; i++)
	{
		if (input[i]==seperator)
		{
			results[j] = ccstr_substr(input, start, i-start);
			j++;
			start=i+1;
		}
	}
	results[j] = ccstr_substr(input, start, i-start);
	return results;
}
//-------------------------------------------------------------------------
//
// join assumes array is null terminated
//
//-------------------------------------------------------------------------
char *ccstr_join (const char seperator, char **stringarray)
{
	int numchars, i, j, k=0;
	for (i=0, numchars=0; stringarray[i]; i++) numchars+=strlen(stringarray[i]);
	numchars+=i;
	char *result = (char *)malloc(sizeof(char)*(numchars)), *p = result;
	if (result==NULL){fprintf(stderr, "out of memory ccstr_join\n");exit (200);}
	for (i=0, k=0; stringarray[i]; i++)
	{
		if (stringarray[i] != NULL)
		{
			for (j=0; stringarray[i][j]; j++, k++) *p++ = stringarray[i][j];
			*p++ = seperator;
		}
	}
	*--p = '\0';
	return result;
}
//-------------------------------------------------------------------------
//
// concat does exactly what you think it does, make sure to FREE
//
//-------------------------------------------------------------------------
char *ccstr_concat (char *a, char *b)
{
	int len = strlen(a)+strlen(b);
	char *ret = (char *)malloc(sizeof(char)*(len+1)), *p=ret;
	if (ret==NULL){fprintf(stderr, "out of memory ccstr_concat\n");exit (200);}
	ccstr_pcpy (&p, a, 0);
	ccstr_pcpy (&p, b, 1);
	return ret;
}
char *ccstr_concat3 (char *a, char *b, char *c)
{
	int len = strlen(a)+strlen(b)+strlen(c);
	char *ret = (char *)malloc(sizeof(char)*(len+1)), *p=ret;
	if (ret==NULL){fprintf(stderr, "out of memory ccstr_concat\n");exit (200);}
	ccstr_pcpy (&p, a, 0);
	ccstr_pcpy (&p, b, 0);
	ccstr_pcpy (&p, c, 1);
	return ret;
}
char *ccstr_concat4 (char *a, char *b, char *c, char *d)
{
	int len = strlen(a)+strlen(b)+strlen(c)+strlen(d);
	char *ret = (char *)malloc(sizeof(char)*(len+1)), *p=ret;
	if (ret==NULL){fprintf(stderr, "out of memory ccstr_concat\n");exit (200);}
	ccstr_pcpy (&p, a, 0);
	ccstr_pcpy (&p, b, 0);
	ccstr_pcpy (&p, c, 0);
	ccstr_pcpy (&p, d, 1);
	return ret;
}

//-------------------------------------------------------------------------
//
// concatenate together a list of strings
//
//-------------------------------------------------------------------------
char *ccstr_concatenate_list (char **list, int size)
{
	int i, sz = 0;
	for (i=0; i<size; i++) sz += strlen (list[i]);
	char *result = (char *) malloc (sizeof (char) * (sz+1));
	if (result==NULL){fprintf(stderr, "out of memory ccstr_concatenatelist\n");exit (200);}

	char *p = result;
	for (i=0; i<size; i++)
	{
		ccstr_pcpy (&p, list[i], 0);
	}
	*p = '\0';
	return result;
}

//-------------------------------------------------------------------------
//
// concat any number of elements, last one must be NULL
//
//-------------------------------------------------------------------------
char *ccstr_cat (char *a, ...)
{
	char *result;
	char *temp2, *temp3;
	va_list arg_ptr;
	va_start(arg_ptr, a);
	result = strdup (a);
	while ((temp2 = va_arg (arg_ptr, char *)))
	{
		temp3 = ccstr_concat (result, temp2);
		free (result);
		result = temp3;
	}
	va_end(arg_ptr);
	return result;
}

//-------------------------------------------------------------------------
//
// nest a string within another string, on either side of it
//
//-------------------------------------------------------------------------
char *ccstr_nest (char *input, char *border)
{
	int sz = strlen (input) + (2 * (strlen (border)));
	char *result = (char *) malloc (sizeof (char) * (sz + 1));
	if (result==NULL){fprintf(stderr, "out of memory ccstr_nest\n");exit (200);}

	char *p = result;
	ccstr_pcpy (&p, border, 0);
	ccstr_pcpy (&p, input, 0);
	ccstr_pcpy (&p, border, 1);
	return result;
}

//-------------------------------------------------------------------------
//
// replace the first occurance in target of findstring with
// replacementstring
//
//-------------------------------------------------------------------------
char *ccstr_replace (char *target, char *findstring, char *replacementstring)
{
	char *result = NULL, *l = strstr (target, findstring);
	if (l != NULL)
	{
		int findlen = strlen (findstring);
		result = (char *) malloc (sizeof (char) * (strlen (target) - findlen + strlen (replacementstring) + 1));
		if (result==NULL){fprintf(stderr, "out of memory ccstr_replace\n");exit (200);}

		if (result != NULL)
		{
			char *tar = target, *r = result;
			//
			// copy over the first bit of target
			//
			while (tar != l) *r++ = *tar++;
			//
			// copy over replacementstring
			//
			ccstr_pcpy (&r, replacementstring, 0);
			//
			// copy over the rest of target after the find string
			//
			tar += findlen;
			ccstr_pcpy (&r, tar, 1);
		}
	}
	return result;
}

//-------------------------------------------------------------------------
//
// count the number of times a string appears within another string
//
//-------------------------------------------------------------------------
int ccstr_occurs (char *target, char *find)
{
	int n = 0;
	int flen = strlen (find);
	char *l = strstr (target, find);
	while (l != NULL)
	{
		n++;
		l += flen;
		l = strstr (l, find);
	}
	return n;
}

//-------------------------------------------------------------------------
//
// replace all occurances in target of findstring with
// replacementstring
//
//-------------------------------------------------------------------------
char *ccstr_replace_all2 (char *target, char *findstring, char *replacementstring)
{
	char *temp = strdup (target);
	char *result = NULL, *l = strstr (temp, findstring);
	int findlen = strlen (findstring);
	//int sanity = 0;
	while (l != NULL)
	{
		//fprintf (stderr, "replacing %s in temp = %s\n", replacementstring, temp);
		//if (++sanity > 10) exit (12) ;
		result = (char *) realloc (result, sizeof (char) * (strlen (temp) - findlen + strlen (replacementstring) + 1));
		if (result==NULL){fprintf(stderr, "out of memory ccstr_replaceal2l\n");exit (200);}

		if (result != NULL)
		{
			char *tar = temp, *r = result, *rep = replacementstring;
			//
			// copy over the first bit of target
			//
			while (tar != l) *r++ = *tar++;
			//
			// copy over replacementstring
			//
			while (*rep) *r++ = *rep++;
			//
			// copy over the rest of target after the find string
			//
			tar += findlen;
			while (*tar) *r++ = *tar++;
			*r = '\0';
		}
		free (temp);
		temp = strdup (result);
		l = strstr (temp, findstring);
	}
	if (result == NULL) result = strdup (target);
	free (temp);
	return result;
}
char *ccstr_replace_all (char *target, char *findstring, char *replacementstring)
{
	int rlen = strlen (replacementstring);
	int tlen = strlen (target);
	int flen = strlen (findstring);
	char *result = NULL;
	int occurs = ccstr_occurs (target, findstring);
	if (occurs == 0)
	{
		result = strdup (target);
	}
	else
	{
		//fprintf (stderr, "replacing %s in %s\n", replacementstring, target);
		result = (char *) malloc (sizeof (char) * (tlen - (occurs * flen) + (occurs * rlen) + 1));
		if (result==NULL){fprintf(stderr, "out of memory ccstr_replaceall\n");exit (200);}

		char *res = result;
		char *rep = replacementstring;
		char *tar = target;
		char *l = strstr (tar, findstring);
		//int sanity = 0;
		while (l != NULL)
		{
			//fprintf (stderr, "   result:>%s<  tar:>%s<\n", result, tar);
			//if (++sanity > 10) exit (12) ;
			//
			// copy over the first bit of target
			//
			while (tar != l) *res++ = *tar++;
			//
			// copy over replacementstring
			//
			ccstr_pcpy (&res, rep, 0);
			//
			// advance tar to the end of the found string
			//
			tar += flen;
			l = strstr (tar, findstring);
			rep = replacementstring;  // reset before start of loop
		}
		//
		// copy over any remaining chars and terminate the string
		//
		ccstr_pcpy (&res, tar, 1);
	}
	return result;
}


//-------------------------------------------------------------------------
//
// replace any character in the map with the replacement string given
// requires 2 passes of the input to avoid limiting the algorithm by
// constraining size, or by doing repeated reallocs
//
//-------------------------------------------------------------------------
char *ccstr_replace_all_map (char *target, char *map, char *replacementstring)
{
	int rlen = strlen (replacementstring);
	int tlen = strlen (target);
	char *result = NULL, *t = target;
	//
	// count the number of map chars we find
	//
	int i=0, nchars = 0;
	while (*t)
	{
		for (i=0; map[i]; i++)
		{
			if (*t == map[i])
			{
				nchars ++;
				break;
			}
		}
		t++;
	}
	//
	// allocate the right size
	//
	result = (char *) malloc (sizeof (char) * (tlen - nchars + (nchars * rlen) + 1));
	if (result==NULL){fprintf(stderr, "out of memory ccstr_replaceallmap\n");exit (200);}

	char *res = result;
	//
	// start copying over
	//
	//
	// for each character in target
	//
	int matched = 0;
	t = target;
	while (*t)
	{
		matched = 0;
		//
		// for each character in map
		//
		for (i=0; map[i]; i++)
		{
			//
			// if matches a map character, then replace with
			// replacementstring
			//
			if (map[i] == *t)
			{
				matched = 1;
				ccstr_pcpy (&res, replacementstring, 0);
				break;
			}
		}
		if (!matched)
		{
			*res++ = *t;
		}
		t++;
	}
	*res = '\0';
	return result;
}

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
{
	if (b < a) return 0;
	char *t = a;
	int result = 1;
	while (t < b) {t++; result++;}
	return result;
}

//-------------------------------------------------------------------------
//
// return a new string which is a copy of the bytes within a string from
// pointer a to pointer b within that string. It is assumed that a is less
// than b, or this wont make sense
//
//-------------------------------------------------------------------------
char *ccstr_bcpy (char *a, char *b)
{
	int sz = ccstr_dist (a, b) + 1;
	char *result = (char *) malloc (sizeof (char) * sz);
	if (result==NULL){fprintf(stderr, "out of memory ccstr_bcpy\n");exit (200);}

	char *t = a, *p = result;
	while (t <= b) *p++ = *t++;
	*p = '\0';
	return result;
}

//-------------------------------------------------------------------------
//
// trim all whitespace from the end of a string and the beginning of
// the string - space, tab, newline, carriage return
//
//-------------------------------------------------------------------------
char *ccstr_trim (char *in)
{
	char *whitespace = " \t\n\r";
	char *result = (char *) malloc (sizeof (char) * (strlen (in) + 1));
	if (result==NULL){fprintf(stderr, "out of memory ccstr_trim\n");exit (200);}

	char *r = result, *i = in;
	while (*i && ccstr_charin (i, whitespace)) i++;
	while (*i) *r++ = *i++;
	if (r == result) *r = '\0';
	else
	{
		*r = ' ';
		while (ccstr_charin (r, whitespace)) *r-- = '\0' ;
	}
	char *ret = strdup (result);
	free (result);
	return ret;
}

//-------------------------------------------------------------------------
//
// first trims off whitespace, then trims off quotes if they are there
//
//-------------------------------------------------------------------------
char *ccstr_trimquotes (char *in)
{
	char *result = ccstr_trim (in);
	char *start = result;
	char *end = result;
	while (*end) end++; // go to the end
	end--;
	//
	// only trim if there are quotes at both ends that match
	// then shift over 1 and write
	//
	if ((*start == '\'' || *start == '\"') && *start == *end)
	{
		char *temp = start;
		temp++;
		while (temp != end) *start++ = *temp++ ;
		*start = '\0';
		*end = '\0';
	}
	return result;
}

//-------------------------------------------------------------------------
//
// copy byte chunk and trim result
//
//-------------------------------------------------------------------------
char *ccstr_bcpy_trim (char *a, char *b)
{
	char *r = ccstr_bcpy (a, b);
	char *result = ccstr_trim (r);
	free (r);
	return result;
}

//-------------------------------------------------------------------------
//
// copy byte chunk and trim results with quote trim
//
//-------------------------------------------------------------------------
char *ccstr_bcpy_ccstr_trimquotes (char *a, char *b)
{
	char *r = ccstr_bcpy (a, b);
	char *result = ccstr_trimquotes (r);
	free (r);
	return result;
}

//-------------------------------------------------------------------------
//
// split, but concious of matching quotes and brackets. So, if a split
// operator comes inside a paired set it is ignored. This is handy for
// spliting out arguement strings - using this on a bracket or quote
// will have strange and undefined results
//
//-------------------------------------------------------------------------
char **ccstr_tokenize (char *input, const char seperator, int tquotes)
{
	char *openmap = "\"'([{<";
	char *closemap = "\"')]}>";
	int numtokens = 0, internal = 0, match = 0;
	char *lastfound = input, *p = input, *r;
	char **result = NULL;
	char *(*trimfunction) (char *);
	trimfunction = (tquotes) ? ccstr_trimquotes : ccstr_trim;
	while (*p)
	{
		if (internal)
		{
			if (*p == closemap[match]) internal = 0;
		}
		else
		{
			if (*p == seperator)
			{
				result = (char **) realloc (result, sizeof (char *) * ++numtokens);
				if (result==NULL){fprintf(stderr, "out of memory ccstr_tokenize\n");exit (200);}

				if (*lastfound == seperator) lastfound++;
				r = ccstr_bcpy (lastfound, --p);
				//fprintf (stderr, "r = %s\n", r);
				//result[numtokens-1] = strdup (r);
				result[numtokens-1] = trimfunction (r);
				//fprintf (stderr, "result[numtokens-1] = %s\n",result[numtokens-1]);
				free (r);
				p++;
				lastfound = p;
			}
			else if ((match = ccstr_matchmap (p, openmap)) != -1)
			{
				internal = 1;
			}
		}

		p++;
	}

	result = (char **) realloc (result, sizeof (char *) * (numtokens+=2));
	if (result==NULL){fprintf(stderr, "out of memory ccstr_tokenize 2\n");exit (200);}

	if (*lastfound == seperator) lastfound++;
	r = ccstr_bcpy (lastfound, --p);
	result[numtokens-2] = trimfunction (r);
	free (r);
	result[numtokens-1] = NULL;
	return result;
}

//-------------------------------------------------------------------------
//
// this parses out a standard arguement list of values seperated by commas
// it assumes that arguments with quotes are to be stripped of quotes, like
// a comma delimited file, which would be another excellent use for this
// function
//
//-------------------------------------------------------------------------
char **ccstr_arglist (char *input)
{
	return ccstr_tokenize (input, ',', 1);
}
//-------------------------------------------------------------------------
//
// just get a ccstr_valuelist seperated by seperator with just regular whitespace
// trimming
//
//-------------------------------------------------------------------------
char **ccstr_valuelist (char *input, const char seperator)
{
	return ccstr_tokenize (input, seperator, 0);
}
//-------------------------------------------------------------------------
//
// given a string in key=value format, parse them out and return them
// WARNING: Potential memory leak!!!!!
// 		call with NON-ALLOCATED key and value pointers
//
//-------------------------------------------------------------------------
void ccstr_keyvalue (char *input, char **key, char **value)
{
	char **p = ccstr_tokenize (input, '=', 1);
	*key = strdup (p[0]);
	*value = strdup (p[1]);
	free_dblchar (p);
}

//-------------------------------------------------------------------------
//
// Converts a hex character to its integer value
//
//-------------------------------------------------------------------------
char ccstr_from_hex(char ch)
{
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

//-------------------------------------------------------------------------
//
// Converts an integer value to its hex character
//
//-------------------------------------------------------------------------
char ccstr_to_hex(char code)
{
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

//-------------------------------------------------------------------------
//
// is this string made up entirely of integers ?
//
//-------------------------------------------------------------------------
int ccstr_isdigits (char *str) {
	char *p = str;
	int result = 0;
	while (*p) { result = (result || isdigit(*p)); p++;}
	return result;
}
//-------------------------------------------------------------------------
//
// Returns a url-encoded version of str (needs FREE)
//
//-------------------------------------------------------------------------
char *ccstr_url_encode (char *str)
{
	char *pstr = str, *buf = (char *)malloc(strlen(str) * 3 + 1), *pbuf = buf;
	if (buf==NULL){fprintf(stderr, "out of memory ccstr_urlencode\n");exit (200);}

	while (*pstr) {
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ')
			*pbuf++ = '+';
		else
			*pbuf++ = '%', *pbuf++ = ccstr_to_hex(*pstr >> 4), *pbuf++ = ccstr_to_hex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

//-------------------------------------------------------------------------
//
// Returns a url-decoded version of str (needs FREE)
//
//-------------------------------------------------------------------------
char *ccstr_url_decode (char *str)
{
	char *pstr = str, *buf = (char *)malloc(strlen(str) + 1), *pbuf = buf;
	if (buf==NULL) {
		fprintf(stderr, "out of memory ccstr_urldecode\n");
		exit (200);
	}

	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = ccstr_from_hex(pstr[1]) << 4 | ccstr_from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') {
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

//-------------------------------------------------------------------------
//
// escape internal ones
//
//-------------------------------------------------------------------------
char *ccstr_escapequotes (char *str)
{
	char *pstr = str, *buf = (char *) malloc (strlen (str) * 2 + 1), *pbuf = buf;
	if (buf==NULL){fprintf(stderr, "out of memory ccstr_escapequotes\n");exit (200);}

	while (*pstr)
	{
		if (*pstr == '"')
		{
			*pbuf++ = '\\';
			*pbuf++ = '"';
		}
		else
		{
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

//-------------------------------------------------------------------------
//
// change a whole string to lowercase
//
//-------------------------------------------------------------------------
char *ccstr_lc (char *str)
{
	char *result = strdup (str);
	char *p = result;
	while (*str) *p++ = tolower ((int)*str++);
	return result;
}

//-------------------------------------------------------------------------
//
// append an entire string to a passed in buffer
//
//-------------------------------------------------------------------------
void ccstr_append (char **buffer, char *format, ... )
{
	char temp[128000];
	va_list args;
	va_start (args, format);
	vsprintf (temp, format, args);
	va_end (args);
	int l = strlen (temp);
	int m = strlen (*buffer);
	*buffer = realloc (*buffer, sizeof (char) * (m+l+1));
	if (buffer==NULL){fprintf(stderr, "out of memory ccstr_append\n");exit (200);}

	char *t = temp;
	char *b = &((*buffer)[m]);
	while (*t) *b++ = *t++;
	*b = '\0';
}

//-------------------------------------------------------------------------
//
// read in a file into a char **
// needs free
//
//-------------------------------------------------------------------------
char **ccstr_read_file (char *filename, int *numlines)
{
	char **result = NULL;
	int n = 0;
	//
	// open the file
	//
	FILE *fp = fopen (filename, "r");
	if (fp == NULL)
	{
		fprintf (stderr, "ERROR: could not open file %s for read\n", filename);
	}
	else
	{
		//
		// count the number of lines
		//
		int c=0,b;while ((b=fgetc(fp))!=EOF) c+=(b==10)?1:0;fseek(fp,0,SEEK_SET);
		//
		// set up some stuff for reading it all in, and allocate our top-level array
		//
		result = (char **) malloc (sizeof (char *) * (c+2));
		if (result == NULL)
		{
			fprintf (stderr, "ERROR: could not allocate memory for file %s slurp\n", filename);
		}
		else
		{
			char line[64000];
			char *p;
			while (fgets (line, 64000, fp) != NULL)
			{
				p = strchr (line, '\n'); if (p) *p = '\0';
				p = strchr (line, '\r'); if (p) *p = '\0';
				p = line;
				if (*p != '\0') result[n++] = strdup (line);
			}
			result[n] = NULL;
		}
	}
	*numlines = n;
	fclose (fp);
	return result;
}

//-------------------------------------------------------------------------
//
// read in a file and pass each line to a function
//
//-------------------------------------------------------------------------
char **ccstr_read_file_function (char *filename, int *numlines, ccstr_file_fn *f, void *data)
{
	char **result = NULL;
	int n = 0;
	//
	// open the file
	//
	FILE *fp = fopen (filename, "r");
	if (fp == NULL)
	{
		fprintf (stderr, "ERROR: could not open file %s for read\n", filename);
	}
	else
	{
		//
		// count the number of lines
		//
		int c=0,b;while ((b=fgetc(fp))!=EOF) c+=(b==10)?1:0;fseek(fp,0,SEEK_SET);
		//
		// read
		//
		char line[64000];
		char *p, *pass;
		while (fgets (line, 64000, fp) != NULL)
		{
			p = strchr (line, '\n'); if (p) *p = '\0';
			p = strchr (line, '\r'); if (p) *p = '\0';
			p = line;
			if (*p != '\0')
			{
				pass = strdup (line);
				f (c, n++, pass, data);
				free (pass);
			}
		}
		result[n] = NULL;
	}
	*numlines = n;
	fclose (fp);
	return result;
}


