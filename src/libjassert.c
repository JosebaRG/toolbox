/**
 * @file libjassert.c
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

#include "libjassert.h"

void libjassert_assert ()
{
#ifdef LIBJASSERT_ASSERT
	assert (0);
#endif //LIBJASSERT_ASSERT

#ifdef LIBJASSERT_LOOP
	do {} while (1);
#endif //LIBJASSERT_LOOP
}
