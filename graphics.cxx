/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include "graphics.hxx"

void DialogBox::draw(void)
{
    unsigned char origin = this->center? 12 : 28;

    arduboy.display();
    arduboy.fillRect(origin, 6, 100, 56, BLACK);
    arduboy.drawRect(origin, 6, 100, 56, WHITE);

    arduboy.setCursor(origin + 2, 8);
    arduboy.print(this->lineA);
    
    arduboy.setCursor(origin + 2, 20);
    arduboy.print(this->lineB);

    arduboy.setCursor(origin + 30, 50);
    arduboy.print(this->prompt);
}

/* static */
void DialogBox::loop(void)
{

}
