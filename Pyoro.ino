/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim "TheSola10" Vergnes  <thesola10@bobile.fr>       Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include "main.h"
#include "assets.h"

void setup() 
{
  arduboy.boot();
  arduboy.blank();         
  arduboy.flashlight();    // magic key protection
  arduboy.systemButtons(); // mute toggle
  arduboy.audio.begin();
  doBrandLogos();
  arduboy.waitNoButtons();
  
  // Magic number check. 
  if (EEPROM.read(ROM_MAGIC) != MAGIC_BYTE) 
  {
    initHiScore();
  } else 
  {
    hiscore = EEPROM.get(ROM_HISCORE, hiscore);
    showMenu();
  }
}

// doLoop() is a pointer to the current game logic loop.
void loop() 
{ doLoop(); }

void doBrandLogos()
{
  arduboy.clear();
  arduboy.drawBitmap(12,24,mylogo,106,18, WHITE);
  arduboy.display();
  delay(2000);
}
