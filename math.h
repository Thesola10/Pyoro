/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim "TheSola10" Vergnes  <thesola10@bobile.fr>       Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __MATH_H
#define __MATH_H

void sub_bound(unsigned int *value, const unsigned int delta, const unsigned int min = 0);

void add_bound(unsigned int *value, const unsigned int delta, const unsigned int max);

bool rough_eq(const unsigned int a, const unsigned int b, const unsigned char precision);

#endif //__MATH_H
