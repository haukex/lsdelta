/** ***** lsdelta.c *****
 *
 * Copyright (c) 2024 Hauke Daempfling (haukex@zero-g.net)
 * at the Leibniz Institute of Freshwater Ecology and Inland Fisheries (IGB),
 * Berlin, Germany, https://www.igb-berlin.de/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see https://www.gnu.org/licenses/
 */

#include "lsdelta.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#ifdef LSDELTA_DEBUG
#include <stdio.h>
#endif

/* Checks the string for valid formatting and returns
 * the number of digits after the decimal point.
 *
 * Returns `false` on success; sets `errno` and returns
 * `true` on failure.
 */
bool _get_dec_dig(const char *str, size_t *cnt) {
	bool found_dec = false;
	*cnt = 0;
	size_t dig_cnt = 0;
	for ( size_t i=0; str[i]; i++ ) {
		if (strchr(i ? ".0123456789" : "+-.0123456789", str[i])==NULL) {
			// this character is not in the set of valid characters
			errno = EINVAL;
			return true;
		}
		if (str[i]=='.') {  // found *a* decimal point
			if (found_dec) {  // more than one decimal point
				errno = EINVAL;
				return true;
			}
			found_dec = true;
		}
		else {
			if ( str[i]!='-' && str[i]!='+' )  // count for below check
				dig_cnt++;
			if (found_dec)  // after decimal
				(*cnt)++;
		}
	}
	if (dig_cnt<1) {  // no digits seen at all! (empty string, "+", "-.", etc.)
		errno = EINVAL;
		return true;
	}
#ifdef LSDELTA_DEBUG
	printf("Digits: <<%s>> => %ld\n", str, *cnt);
#endif
	return false;  // success
}

/* Removes the decimal point from the string, pads it the
 * specified amount, and converts the result to a number.
 *
 * IMPORTANT: Assumes the string has been checked for valid
 * formatting by `_get_dec_dig`.
 *
 * Returns `false` on success; sets `errno` and returns
 * `true` on failure.
*/
char _convert(const char *str, size_t pad, long long *num) {
	const size_t len = strlen(str);
	// initialize the output string
	const size_t new_len = len+pad;
	char *out = (char*) malloc(new_len+1);  // +1 NUL!
	if (out==NULL) return true;  // malloc fail (sets errno)
	memset(out, 0, new_len+1);  // +1 NUL!
#ifdef LSDELTA_DEBUG
	printf("Convert: <<%s>> (new_len=%ld) ", str, new_len); fflush(stdout);
#endif
	// copy over without decimal point
	size_t o = 0;
	for( size_t i=0; i<len; i++ ) {
		assert(o<new_len);
		if (str[i]!='.')
			out[o++] = str[i];
	}
	// pad
	for ( size_t i=0; i<pad; i++ ) {
		assert(o<new_len);
		out[o++] = '0';
	}
#ifdef LSDELTA_DEBUG
	printf("=> <<%s>> ", out); fflush(stdout);
#endif
	// convert to number
	char *endptr;
	errno = 0;  // to detect error conditions in the following
	*num = strtoll(out, &endptr, 10);
	// check for error conditions
	if (endptr == str) errno = EINVAL;  // no digits were found
	else if (*endptr != '\0') errno = EINVAL; // invalid characters found
	const bool rv = !!errno;  // store for below
	// done!
	free(out);  // documented to preserve errno on glibc 2.33+, but we don't rely on that
#ifdef LSDELTA_DEBUG
	printf("=> %lld\n", *num);
#endif
	return rv;
}

char lsdelta(const char *a, const char *b, long long *delta) {
	if (a==NULL || b==NULL) {
		errno = EINVAL;
		return -1;
	}
#ifdef LSDELTA_DEBUG
	printf("\n");
#endif
	// check strings and get number of digits after decimal point
	size_t a_cnt;
	if (_get_dec_dig(a, &a_cnt)) return -1;
	size_t b_cnt;
	if (_get_dec_dig(b, &b_cnt)) return -1;
	// convert the strings to numbers
	long long a_num;
	if (_convert(a, b_cnt>a_cnt ? b_cnt-a_cnt : 0, &a_num)) return -1;
	long long b_num;
	if (_convert(b, a_cnt>b_cnt ? a_cnt-b_cnt : 0, &b_num)) return -1;
	// do the delta
#ifndef __has_builtin
	#define __has_builtin(x) 0
#endif
#if __has_builtin(__builtin_ssubll_overflow)
	// this should be the case on GCC and Clang
	// https://gcc.gnu.org/onlinedocs/gcc/Integer-Overflow-Builtins.html
	// https://clang.llvm.org/docs/LanguageExtensions.html#checked-arithmetic-builtins
	if (__builtin_ssubll_overflow(a_num, b_num, delta)) {
		errno = EOVERFLOW;
		return -1;
	}
#else
	#warning "This build of lsdelta does not check for overflows!"
	*delta = a_num - b_num;
#endif
	return 0;
}
