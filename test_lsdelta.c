
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

#include "lsdelta.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
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
	return fails ? 1 : 0;
}
