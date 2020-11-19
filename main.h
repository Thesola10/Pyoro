/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim "TheSola10" Vergnes  <thesola10@bobile.fr>       Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <stdlib.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "game.h"
#include "logic.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define ARDBITMAP_SBUF arduboy.sBuffer
#include <ArdBitmap.h>

#define ROM_HISCORE EEPROM_STORAGE_SPACE_START
#define ROM_MAGIC   EEPROM_STORAGE_SPACE_START+2
#define ROM_SOUND   EEPROM_STORAGE_SPACE_START+3

// Savefile magic byte (increases if there is a difficulty update)
#define MAGIC_BYTE  0x02

ArdBitmap<WIDTH, HEIGHT> ardbitmap;


unsigned int hiscore;

// Static game instance for the game loop function, since it has to be static itself
static Game *game;

// This pointer indicates what state the game is in
void (*doLoop)() = &MainMenu::loop;

#endif //__MAIN_H
