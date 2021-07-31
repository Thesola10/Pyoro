/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
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

/**
 * \file        main.h
 * \brief       Header for game entry point
 *
 * This file serves as the main header for the game's entry point.
 */

#ifndef DOXYGEN

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define ARDBITMAP_SBUF arduboy.sBuffer
#include <ArdBitmap.h>

ArdBitmap<WIDTH, HEIGHT> ardbitmap;

#endif //DOXYGEN

/**
 * This macro specifies a two-byte area in EEPROM to which the high score
 * should be written.
 *
 * \brief           EEPROM offset for high score save
 * \see     hiscore         Working copy of this value in memory.
 */
#define ROM_HISCORE EEPROM_STORAGE_SPACE_START
/**
 * This macro specifies a single byte area in EEPROM to check that the save
 * data belongs to this game and is up to date.
 *
 * \brief           EEPROM offset for magic number
 * \see     MAGIC_BYTE      The current value for the magic number
 */
#define ROM_MAGIC   EEPROM_STORAGE_SPACE_START+2
/**
 * This macro specifies a single byte area in EEPROM to which the last known
 * sound mute state should be written.
 *
 * \brief           EEPROM offset for sound mute setting
 */
#define ROM_SOUND   EEPROM_STORAGE_SPACE_START+3

/**
 * This macro specifies the current value of the save file magic number.
 * This value should be increased if there is a significant change to game
 * difficulty, that would render existing high scores outdated.
 *
 * \brief           Value of the EEPROM magic number
 * \see     ROM_MAGIC       Location in EEPROM to store and check for value
 */
#define MAGIC_BYTE  0x02


/**
 * This variable stores the high score to be displayed on the main menu,
 * either loaded from EEPROM, or from the latest high-score game. This variable
 * serves to reduce reads and writes to EEPROM.
 *
 * \brief           In-memory copy of high score
 * \see     ROM_HISCORE     Area in EEPROM where high score is loaded/saved
 */
unsigned int hiscore;

/**
 * This variable stores the current instance of @ref Game, for use in the
 * @ref Game::loop() loop state. This is static because @ref doLoop itself
 * does not take arguments.
 *
 * \brief           State instance for an ongoing game
 * \see     doLoop          Run-time state function which uses this variable
 */
static Game *game;

/**
 * This variable indicates the running state of the game. In order to switch to
 * another state, such as @ref Game::loop(), this variable's value should be
 * updated.
 *
 * \brief           Running state of the game
 */
void (*doLoop)() = &MainMenu::loop;

#endif //__MAIN_H

// vim: ft=cpp.doxygen
