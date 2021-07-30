/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original assets
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __ASSETS_H
#define __ASSETS_H

#include <avr/pgmspace.h>

/**
 * \file        assets.hxx
 * \brief       Bitmap assets used in the game
 *
 * This file contains static definitions for the graphical objects in game. 
 */

PROGMEM const unsigned char mylogo[] =
{
  0x00, 0x00, 0x80, 0xc0, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe0, 0x00, 0xa0, 0x00, 0x00, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7e, 0x02, 0x00, 0x7e, 0x08, 0x70, 0x00, 0x30, 0x48, 0x10, 0x00, 0x24, 0x4a, 0x34, 0x00, 0x30, 0x48, 0x30, 0x00, 0x7e, 0x00, 0x20, 0x48, 0x30, 0x04, 0x7e, 0x00, 0x7e, 0x42, 0x7e, 0x00, 0x01, 0x00, 0x20, 0x21, 0x12, 0x0c, 0x0c, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x20, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x21, 0x19, 0x07, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};  //!< [106x18]   Splash screen logo

PROGMEM const unsigned char pyoroLogo[] =
{
  0x00, 0xfe, 0x02, 0x02, 0x72, 0x4a, 0x42, 0xe2, 0x22, 0x02, 0x86, 0x0c, 0xf8, 0xf0, 0x40, 0xc0, 0x80, 0x80, 0xc0, 0x40, 0xc0, 0xc0, 0x80, 0x80, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0xc0, 0x40, 0xc0, 0x80, 0xc0, 0x40, 0x40, 0xc0, 0xc0, 0x80, 0xc0, 0xc0, 0x40, 0x40, 0x40, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x71, 0x71, 0x71, 0x78, 0x7c, 0xff, 0xf0, 0xc0, 0xc0, 0x87, 0x87, 0x00, 0x00, 0x00, 0xff, 0x87, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x87, 0xff, 0x00, 0x00, 0x00, 0xf1, 0xf8, 0xfc, 0xfc, 0xfe, 0x87, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x87, 0xfe, 0xf8, 0x00, 0x00, 0x7f, 0xf0, 0xe0, 0xf0, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1d, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1c, 0x1f, 0x0f, 0x06, 0x0c, 0x1c, 0x18, 0x18, 0x18, 0x1c, 0x1c, 0x1e, 0x0f, 0x0f, 0x1c, 0x18, 0x1c, 0x1f, 0x1f, 0x0f, 0x00, 0x01, 0x03, 0x06, 0x0c, 0x1c, 0x18, 0x18, 0x18, 0x1c, 0x1c, 0x0e, 0x0f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
};  //!< [56x25]    Pyoro logotype on start screen

PROGMEM const unsigned char bird[] =
{
  0x18, 0x34, 0x52, 0xc5, 0x41, 0xc2, 0x3c, 0x00, /* next */0x12, 0x36, 0x5a, 0xc5, 0x41, 0xc2, 0x3c, 0x00,
};  //!< 2×[8x8]    Sprites for the player character (0: normal, 1: beak open)

PROGMEM const unsigned char brick[] =
{
  0xe, 0xa, 0xe, 0x00, /* next */0xa, 0x00, 0xa, 0x00,
};  //!< 2×[4x4]    Sprites for a brick on the ground (0: normal, 1: breaking)

PROGMEM const unsigned char bean[] =
{
  0xc, 0x1f, 0x1e, 0xc, /* next */0xc, 0x13, 0x12, 0xc,
};  //!< 2×[4x5]    Sprites for a falling bean (0: normal, 1: white)

PROGMEM const unsigned char angel[] =
{
  0x1e, 0x18, 0x2a, 0x45, 0xea, 0x18, 0xe, 0x1c, 
};  //!< [8x8]      Sprite for the angel character

#endif //__ASSETS_H

// vim: ft=cpp.doxygen
