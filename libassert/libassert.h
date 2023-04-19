/**
 * @file libassert.h
 *
 * @brief Collection of functions to check the integrity of parameters.
 *
 * This library provides a collection of functions that use the assert()
 * macro to check the integrity of their input parameters. By including
 * this library in one project, it can easily incorporate parameter
 * checking into the code without having to write your specific custom 
 * error-checking code.
 *
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#ifndef _LIBASSERT_H
#define _LIBASSERT_H

#include <stdio.h>

/*********************************************************************************
 *                                   DEFINITIONS
 *********************************************************************************/

//#define LIBASSERT_ASSERT
//#define LIBASSERT_LOOP

#define LIBASSERT_MAX_LINE 100
/**
 * sprintf (msg, "%s %s - %20s : %25s : %4d", __DATE__,__TIME__, __FILE__, __func__, __LINE__); \
 * __DATE__ 11 chars
 * __TIME__  8 chars
 * __FILE__ 20 chars
 * __func__ 30 chars
 * __LINE__  4 chars
 * extra    10 chars
 * TOTAL    83 chars
 */

/**
 * @def LIBASSERT_PTR
 * 
 * @brief Macro to check the integrity of a pointer.
 *
 * This macro checks if a pointer is NULL. If it is, an error message is printed,
 * and the assert macro is called.
 * 
 * @param[in] ptr A pointer to be checked.
 */
#define LIBASSERT_PTR(ptr) \
    do \
	{ \
		if (ptr == NULL) \
		{ \
			char msg [100]; \
			sprintf (msg, "%s %s - %s(%d):%s - POINTER ASSERT ERROR", __DATE__,__TIME__, __FILE__, __LINE__, __func__); \
			printf ("\n%s", msg); \
			libassert_assert (); \
		} \
    } while (0)

/*********************************************************************************
 *                                      API
 *********************************************************************************/

/**
 * @brief Asserts or loops depending on which macros are defined.
 *
 * This function checks if certain macros are defined and performs an action accordingly.
 *
 * If the macro LIBASSERT_ASSERT is defined, it calls the assert function, which will
 * cause the program to terminate with an error message.
 *
 * If the macro LIBASSERT_LOOP is defined, it enters an infinite loop using a do-while.
 * 
 * @note SHOULD NOT be called directly. Its purpose is to be called from macros.
 */
void libassert_assert ();

#endif //_LIBASSERT_H

