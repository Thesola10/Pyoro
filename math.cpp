/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim "TheSola10" Vergnes  <thesola10@bobile.fr>       Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include <limits.h>
#include "math.h"

void sub_bound(unsigned int *value, const unsigned int delta, const unsigned int min)
{
    *value -= (delta + min <= *value)? delta : (*value - min);
}

void add_bound(unsigned int *value, const unsigned int delta, const unsigned int max)
{
    *value += ((delta + *value) <= max)? delta : (max - *value);
}

bool rough_eq(const unsigned int a, const unsigned int b, const unsigned char precision)
{
    unsigned int b_up = b;
    unsigned int b_lo = b;
    sub_bound(&b_lo, precision);
    add_bound(&b_up, precision, UINT_MAX);
}
