
/** ***** test_lsdelta.c *****
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

#define _XOPEN_SOURCE 600  // for time.h
#include "lsdelta.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#define UNUSED(x) (void)(x)

#include "test_lsdelta_tests.inc.c"

int main(int argc, char** argv) {
	UNUSED(argc); UNUSED(argv);
	size_t fails = 0;
	for ( size_t i=0; i<NUM_TESTS; i++ ) {
        printf("a=\"%s\" b=\"%s\" o=%s ", TEST[i].a, TEST[i].b, TEST[i].o==NULL ? "NULL" : TEST[i].o);
		mpz_t delta;
		mpz_init(delta);
		bool rv = lsdelta(TEST[i].a, TEST[i].b, &delta);
		if ( TEST[i].o==NULL ) {  // expect failure
			if (rv) {  // but didn't fail
				printf("FAIL exp=fail got=");
				mpz_out_str(stdout, 10, delta);
				printf("\n");
				fails++;
			}
			else printf("PASS\n");  // failure as expected
		}
		else {  // expect success
			if (rv) {  // success as expected, compare
				mpz_t exp;
				if (mpz_init_set_str(exp, TEST[i].o, 10)) {
					printf("ERROR: failed to parse \"%s\"\n", TEST[i].o);
					mpz_clear(exp);
					mpz_clear(delta);
					return 2;
				} // else:
				if (mpz_cmp(exp, delta)) {
					printf("FAIL exp=");
					mpz_out_str(stdout, 10, exp);
					printf(" got=");
					mpz_out_str(stdout, 10, delta);
					printf("\n");
					fails++;
				}
				else printf("PASS\n");
				mpz_clear(exp);
			}
			else {  // unexpected failure
				printf("FAIL exp=%s got=fail\n", TEST[i].o);
				fails++;
			}
		}
		mpz_clear(delta);
	}
	if ( fails>0 ) printf("%ld FAILURES\n", fails);
	else printf("ALL %d PASS\n", NUM_TESTS);

	// time test
	const size_t LOOPS = 1000000;
	mpz_t d2;
	mpz_init(d2);
	struct timespec start;
	if (clock_gettime(CLOCK_MONOTONIC, &start)) {
		printf("clock_gettime: %s\n", strerror(errno));
		return 2;
	}
	printf("start: %10jd.%09ld\n", (intmax_t) start.tv_sec, start.tv_nsec);
	for ( size_t i=0; i<LOOPS ; i++ ) {
		lsdelta("123.45","123.4449", &d2);
	}
	struct timespec end;
	if (clock_gettime(CLOCK_MONOTONIC, &end)) {
		printf("clock_gettime: %s\n", strerror(errno));
		return 2;
	}
	mpz_clear(d2);
	printf("  end: %10jd.%09ld\n", (intmax_t) end.tv_sec, end.tv_nsec);
	unsigned long long nsec = end.tv_nsec + 1000000000LL*(end.tv_sec - start.tv_sec) - start.tv_nsec;
	printf(" = %lldns = %12.1f loops/s\n", nsec, ((double)LOOPS)*1e9/(float)nsec);
	return fails ? 1 : 0;
}
