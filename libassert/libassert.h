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

/*********************************************************************************
 *                                      API
 *********************************************************************************/

/**
 * @brief Check if a pointer is not NULL.
 *
 * This function is for checking pointer integrity. If the pointer is NULL,
 * the assert will fail and the program will terminate.
 *
 * @param[in] ptr The pointer to be checked.
 * @return This function does not return a value.
 */
void libassert_ptr (void * ptr);

#endif //_LIBASSERT_H

