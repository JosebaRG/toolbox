/**
 * @file libassert.c
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

#include <stddef.h>
#include <assert.h>

#include "libassert.h"

void libassert_ptr (void * ptr)
{
	assert(ptr != NULL);
}
