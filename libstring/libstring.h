#ifndef _LIBSTRING_H
#define _LIBSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char * before;
	char * matched;
	char * after;
	long offset;
}match_pattern_t;

/** 
 * @param * text pointer to string
 *
 * @return the length of the text
 */
long libstring_length (char * text);

/** 
 * @param * text_a string to concatenate into
 * @param * text_b string to be concatenated
 *
 * @return the lenght of the added element
 */
long libstring_concat (char * text_a, char * text_b);

/** 
 * @param * subset the obtained string subset
 *
 * @return the lenght of subset string
 */
long libstring_subset (char * text, long offset, long length, char * subset);

/** 
 *
 * @return number of replaced char elements
 */
long libstring_replace (char * text, char * replace, long offset);

/** 
 *
 * @return the position of text where searched string begins
 */
long libstring_search (char * text, char * searched);

/** 
 *
 * @return struct with the strings from before, matched and after
 * 		   and the position of text where matched string begins
 */
//match_pattern_t libstring_match_pattern (char * string, char * match);

//match_pattern_t libstring_free_matched (match_pattern_t matched_t);


#endif //_LIBSTRING_H
