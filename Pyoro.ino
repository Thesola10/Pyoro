/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include "main.h"
#include "assets.h"

ArduboyTones sound(arduboy.audio.enabled);
void (*doLoop)(void) = MainMenu::loop;

void setup() 
{
  arduboy.boot();
  arduboy.blank();         
  arduboy.flashlight();    // magic key protection
  arduboy.systemButtons(); // mute toggle
                           //TODO: Drop once settings menu is complete
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
    MainMenu::init();
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

void initHiScore()
{
    //TODO: do something
}
