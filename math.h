/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __MATH_H
#define __MATH_H
/**
 * \file        math.h
 * \brief       Math functions useful to the game logic
 *
 * This file contains bounded implementations for common arithmetic operators.
 */

/**
 * This function attempts to subtract a certain value until it reaches a lower bound.
 * If the resulting value ends up below the lower bound, the lower bound is returned.
 * Else the normal subtraction result is returned.
 *
 * \brief       Integer subtraction with lower bound
 * \param value     Pointer to the variable to subtract from
 * \param delta     Amount to subtract from the variable
 * \param min       Lower bound of operation
 * \see     add_bound()     Addition with upper bound
 */
void sub_bound(unsigned int *value, const unsigned int delta, const unsigned int min = 0);

/**
 * This function attempts to add a certain value until it reaches an upper bound.
 * If the resulting value ends up above the upper bound, the upper bound is returned.
 * Else the normal addition result is returned.
 *
 * \brief       Integer addition with upper bound
 * \param value     Pointer to the variable to add to
 * \param delta     Amount to add to the variable
 * \param max       Upper bound of operation
 * \see     sub_bound()     Subtraction with lower bound
 */
void add_bound(unsigned int *value, const unsigned int delta, const unsigned int max);

/**
 * This function compares two integer values, verifies below a set separation.
 * We first calculate the lower and upper bounds of <code>b</code> by adding
 * and subtracting a given value, then we check if <code>a</code> lands within
 * our bounds.
 *
 * \brief       Integer comparison with acceptable range
 * \param a         First number to compare
 * \param b         Second number to compare. Range calculation is applied to 
 *                  this value.
 * \param precision How far away from <code>b</code> the function can be
 *                  verified for <code>a</code>
 * \return      True if <code>a</code> falls between <code>b - precision</code>
 *              and <code>b + precision</code>. False otherwise.
 * \see     add_bound()     Used internally to prevent overflow
 * \see     sub_bound()     Used internally to prevent underflow
 */
bool rough_eq(const unsigned int a, const unsigned int b, const unsigned char precision);

#endif //__MATH_H

// vim: ft=cpp.doxygen
