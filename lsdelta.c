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
#include <stdlib.h>
#include <errno.h>
#ifdef LSDELTA_DEBUG
#include <stdio.h>
#endif

/* Checks the string for valid formatting and returns
 * the number of digits after the decimal point.
 *
 * Returns `true` on success; sets `errno` and returns
 * `false` on failure.
 */
bool _get_dec_dig(const char *str, size_t *cnt) {
	bool found_dec = false;
	*cnt = 0;
	size_t dig_cnt = 0;
	for ( size_t i=0; str[i]; i++ ) {
		if (strchr(i ? ".0123456789" : "+-.0123456789", str[i])==NULL) {
			// this character is not in the set of valid characters
			errno = EINVAL;
			return false;
		}
		if (str[i]=='.') {  // found *a* decimal point
			if (found_dec) {  // more than one decimal point
				errno = EINVAL;
				return false;
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
		return false;
	}
#ifdef LSDELTA_DEBUG
	printf("Digits: <<%s>> => %ld\n", str, *cnt);
#endif
	return true;  // success
}

/* Removes the decimal point from the string, pads it the
 * specified amount, and converts the result to a number.
 *
 * **IMPORTANT:** Assumes the string has been checked for valid
 * formatting by `_get_dec_dig` and that `num` is a pointer to
 * an *uninitialized* `mpz_t`.
 *
 * Returns `true` on success; sets `errno` and returns
 * `false` on failure. The caller does not need to `mpz_clear`
 * in the latter case.
*/
bool _convert(const char *str, size_t pad, mpz_t *num) {
	const size_t len = strlen(str);
	// initialize the output string
	const size_t new_len = len+pad;
	char *out = (char*) malloc(new_len+1);  // +1 NUL!
	if (out==NULL) return false;  // malloc fail (sets errno)
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
	assert(o<=new_len);  // ok because of new_len+1 above
	out[o] = '\0';
#ifdef LSDELTA_DEBUG
	printf("=> <<%s>> ", out); fflush(stdout);
#endif
	// convert to number
	const int init_rv = mpz_init_set_str(*num, out, 10);
	free(out);  // free immediately to avoid dealing with that later
	if (init_rv) {
		mpz_clear(*num);
		errno = EINVAL;
		return false;
	}
#ifdef LSDELTA_DEBUG
	printf("=> ");
	mpz_out_str(stdout, 10, *num);
	printf("\n");
#endif
	return true;
}

bool lsdelta(const char *a, const char *b, mpz_t *delta) {
	if (a==NULL || b==NULL) {
		errno = EINVAL;
		return false;
	}
#ifdef LSDELTA_DEBUG
	printf("\n##### lsdelta(\"%s\", \"%s\")\n", a, b);
#endif
	// check strings and get number of digits after decimal point
	size_t a_cnt;
	if (!_get_dec_dig(a, &a_cnt)) return false;
	size_t b_cnt;
	if (!_get_dec_dig(b, &b_cnt)) return false;
	// convert the strings to numbers
	mpz_t a_num;
	if (!_convert(a, b_cnt>a_cnt ? b_cnt-a_cnt : 0, &a_num))
		return false;
	mpz_t b_num;
	if (!_convert(b, a_cnt>b_cnt ? a_cnt-b_cnt : 0, &b_num)) {
		mpz_clear(a_num);
		return false;
	}
	// do the delta
	mpz_sub(*delta, a_num, b_num);
	//*delta = a_num - b_num;
	mpz_clear(a_num);
	mpz_clear(b_num);
#ifdef LSDELTA_DEBUG
	printf("Result: ");
	mpz_out_str(stdout, 10, *delta);
	printf("\n");
#endif
	return true;
}
