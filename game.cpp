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
#include "logic.h"
#include "game.h"

void Bird::update(Game *game)
{
    Bird::tongue_pos pos = this->getTonguePos();
    tongueLenPx = this->tongueLen/100;
    posPx = this->posx/100;

    if      (arduboy.pressed(A_BUTTON)     && tongueLenPx < 52
                                           && ((tongueLenPx + pos.basex) < 128 || this->direction)
                                           && (tongueLenPx < pos.basex || !this->direction)
                                           && !(game->aHeld && this->tongueRet))                
        { add(&this->posx, this->speed*2, 12800); 
          game->aPressed = true; }
                                         
    else if (arduboy.pressed(LEFT_BUTTON)  && this->posx > 0   
                                           && !this->tongueRet && !this->tongueLen
                                           && game->checkBrick((posPx + 4)/4))
        { sub(&this->posx, this->speed);     
          this->direction = true; }
                                         
    else if (arduboy.pressed(RIGHT_BUTTON) && this->posx < 12000 
                                           && !this->tongueRet && !this->tongueLen
                                           && game->checkBrick((posPx + 8)/4))        
        { add(&this->posx, this->speed, 12000); 
          this->direction = false; }
}

void Bean::Creator::update(Game *game)
{

}

void Angel::Creator::schedule(Game *game)
{
    char index = 0;
    if (! ~(game->bricks)) return;

    while (true) {
        char i = random(0, 31);
        if (!game->checkBrick(i)) {
            index = i + 1;
            break;
        }
    }
    for (slot : this->schedule) {
        if (!slot) {
            slot = index;
            break;
        } else if (slot == index) {     // Prevent duplicates
            break;
        }
    }
}

void Angel::Creator::update(Game *game)
{
if (this->schedule[0])      // None of this needs to occur if the schedule is empty
    if (this->timer == 0) {
        // We find an empty slot to add an angel
        for (slot : game->angels)
            if (!slot) {
                slot = this->create(this->schedule[0]);
                break;
            }
        // We shift all schedule elements to the left
        for (int i = 1; i < 8; i++) 
            this->sched[i - 1] = this->sched[i];
        this->sched[7] = 0;
        // We reset the timer
        timer = 36
    } else if (game->metronome % 10 == 0) {
        timer--;
    }
}


Bean Bean::Creator::create(bool allowsuper)
{
    Bean b = new Bean;
    char t = random(1, 100);

    if (t < 15)                     b.type = B_HEAL;
    else if (allowsuper && t < 20)  b.type = B_SUPER_A;
    else                            b.type = B_NORMAL;
    b.x = random(0, 120);
    b.y = 0;
    b.speed = random(this->avgSpeed - 3, this->avgSpeed + 3);
}

Angel Angel::Creator::create(char replaces)
{
    Angel a = new Angel;

    a.replaces = replaces;
    return a;
}


Bird::tongue_pos getTonguePos(void)
{
    Bird::tongue_pos pos;

    pos.basex = this.direction? this.posx/100 : (this.posx/100) + 8;
    pos.basey = 53;

    pos.tipx = this.direction? basex - (this.tongueLen/100)
                             : basex + (this.tongueLen/100);
    pos.tipy = 53 - (this.tongueLen/100);

    return pos;
}

void Bird::draw(void)
{
    ardbitmap.drawBitmap(this.posx / 100, 52,           // X and Y coords
                         this.tongueExt? bird+8 : bird, // bird+8 is a second sprite
                         8, 8,                          // Size of the sprite
                         WHITE, ALIGN_NONE,             // Color params
                         this.direction? MIRROR_NONE : MIRROR_HORIZONTAL
                        );
}

void Bird::drawTongue(void)
{
    Bird::tongue_pos pos = getTonguePos();

    arduboy.drawLine(pos.basex, pos.basey, pos.tipx, pos.tipy, WHITE);
}

void Bean::display(void)
{
    if (this.type % 2 == 0)
        arduboy.drawBitmap(x, y/100, bean,   4, 5, WHITE);
    else
        arduboy.drawBitmap(x, y/100, bean+4, 4, 5, WHITE);
}

void Angel::display(void)
{
    // We draw the angel
    arduboy.drawBitmap((this.replaces * 4) - 2, Angel::animation[this.step],
                       angel,
                       8, 8, WHITE
                      );
    if (this.step < 5) // and the brick it's holding in the first steps
        arduboy.drawBitmap(this.replaces * 4, Angel::animation[this.step] + 8,
                           brick,
                           4, 4, WHITE
                          );
}
