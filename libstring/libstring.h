/**
 * @file libstring.h
 *
 * @brief Library to work with strings.
 *
 * This library was created to simplify working with strings in C. 
 * The implemented functions aim to provide similar functionality
 * to that obtained in LabVIEW.
 *
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#ifndef _LIBSTRING_H
#define _LIBSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************************************************************
 *                                    STRUCTS
 *********************************************************************************/

/**
 * @brief Struct to represent a matched pattern in a string.
 * This struct contains the before, matched and after substrings, as well as 
 * the offset of the matched pattern in the original string.
 */
typedef struct{
	char * before;  /**< The substring before the matched pattern. */
	char * matched; /**< The matched pattern. */
	char * after;   /**< The substring after the matched pattern. */
	long offset;    /**< The offset of the matched pattern in the original string. */
}match_pattern_t;

/*********************************************************************************
 *                                      API
 *********************************************************************************/

/**
 * @brief Returns the length of the input string.
 *
 * This function iterates over the input string until it finds the null
 * character '\0', which indicates the end of the string, and returns the
 * length of the string.
 *
 * @param[in] text Pointer to the input string.
 *
 * @return The length of the input string.
 */
long libstring_length (char * text);

/**
 * @brief Concatenates two strings.
 *
 * This function concatenates two strings by appending the second string 
 * to the end of the first one.
 *
 * @param[in,out] text_a Pointer to the first string.
 * @param[in] text_b Pointer to the second string.
 * 
 * @return The length of the concatenated string.
 */
long libstring_concat (char * text_a, char * text_b);

/**
 * @brief Extracts a substring from string.
 *
 * Extracts a substring from the given text starting at the 
 * specified offset and with the specified length.
 *
 * @param[in] text The text from which to extract the substring.
 * @param[in] offset The starting position from which to extract the substring.
 * @param[in] length The length of the substring to extract.
 * @param[out] subset The substring to be extracted.
 *
 * @return The number of characters copied into the subset.
 */
long libstring_subset (char * text, long offset, long length, char * subset);

/**
 * @brief Replaces a substring in a string at a given offset with a new substring.
 *
 * This function replaces the substring in the string "text" at the specified "offset"
 * with the new substring "replace". The function returns the number of characters
 * that were replaced. If the "replace" string is empty or null, the function will
 * return 0 without replacing anything.
 *
 * @param[in,out] text The string to perform the replacement on.
 * @param[in] offset The offset in the string where the replacement should occur.
 * @param[in] replace The new substring to replace the old substring with.
 * 
 * @return The number of characters replaced in the string.
 */
long libstring_replace (char * text, long offset, char * replace);

/**
 * @brief Compare two strings and returns whether they are equal or not.
 *
 * This function compares the content of two strings and returns 0 if they are
 * equal and -1 if they are different. The comparison is case sensitive and takes
 * into account the length of the strings.
 *
 * @param[in] a The first string to be compared.
 * @param[in] b The second string to be compared.
 *
 * @return 0 if the strings are equal, -1 otherwise.
 */
int libstring_compare (char * a, char * b);

/**
 * @brief Searches for a substring within a given string starting at a specified offset.
 *
 * @param[in] text The input string to search.
 * @param[in] offset The starting offset within the input string.
 * @param[in] searched The substring to search for.
 *
 * @return If found, returns the position of the substring within the input string
 * starting from the given offset, else -1.
 */
long libstring_search (char * text, long offset, char * searched);

/**
 * @brief Finds the first occurrence of a substring in a string and returns a
 * structure with information about the match.
 *
 * @param[in] string The string to search in.
 * @param[in] match The substring to search for.
 * 
 * @return match_pattern_t A structure containing information about the match,
 * including the portion of the string before and after the match, the match itself,
 * and the offset of the match.
 */
//match_pattern_t libstring_match_pattern (char * string, char * match);

/**
 * @brief Frees the memory used by a match_pattern_t struct.
 *
 * This function frees the memory previously allocated for the before, matched,
 * and aftermembers of a match_pattern_t struct. It then returns the same struct
 * with its members set to NULL.
 *
 * @param[in] matched_t The match_pattern_t struct to be freed.
 *
 * @return The match_pattern_t struct with its members set to NULL.
 */
//match_pattern_t libstring_free_matched (match_pattern_t matched_t);


#endif //_LIBSTRING_H
