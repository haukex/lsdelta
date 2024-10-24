#ifndef LSDELTA_H
#define LSDELTA_H

/** ***** lsdelta.h *****
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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** This function takes two decimal numbers stored as strings,
 * pads them both to the same length after the decimal point,
 * and then removes the decimal point and subtracts them,
 * giving you the difference in their least significant digits.
 *
 * For example, the delta between "1035.349" and "1035.35" is -1,
 * and the delta between "1035.110" and "1035.1" is 10.
 *
 * Please be aware that `long long` typically ranges from
 * -9,223,372,036,854,775,808 to +9,223,372,036,854,775,807
 * and this function is therefore limited to that range.
 *
 * On success, returns zero (0) and stores the result in the
 * argument `delta`; on failure, returns a negative value
 * (the value of `delta` is undefined in this case).
 * Currently should not return positive values, but callers
 * wishing to play it safe can treat them as errors as well.
 */
char lsdelta(const char *a, const char *b, long long *delta);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // #ifndef LSDELTA_H
