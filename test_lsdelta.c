
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
        printf("a=\"%s\" b=\"%s\" rv=%d o=%lld", TEST[i].a, TEST[i].b, TEST[i].rv, TEST[i].o);
		long long o = 0;
		char rv = lsdelta(TEST[i].a, TEST[i].b, &o);
		if ( rv != TEST[i].rv )
			{ printf(" FAIL rv=%d\n", rv); fails++; }
		else if ( o != TEST[i].o )
			{ printf(" FAIL o=%lld\n", o); fails++; }
		else printf(" PASS\n");
	}
	if ( fails>0 ) printf("%ld FAILURES\n", fails);
	else printf("ALL %d PASS\n", NUM_TESTS);
	return fails ? 1 : 0;
}
