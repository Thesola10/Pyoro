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
#include "logic.h"
#include "game.h"
#include "assets.h"
#include "math.h"

void Bird::update(Game *game)
{
    Bird::tongue_pos pos = this->getTonguePos();
    int tongueLenPx = this->tongueLen/100;
    int posPx = this->posx/100;

    if      (arduboy.pressed(A_BUTTON)     && tongueLenPx < 52
                                           && ((tongueLenPx + pos.basex) < 128 || this->direction)
                                           && (tongueLenPx < pos.basex || !this->direction)
                                           && !(game->aHeld && this->tongueRet))                
        { add_bound(&this->tongueLen, this->speed*2, 12800); 
          game->aPressed = true; }
                                         
    else if (arduboy.pressed(LEFT_BUTTON)  && this->posx > 0   
                                           && !this->tongueRet && !this->tongueLen
                                           && game->checkBrick((posPx + 4)/4))
        { sub_bound(&this->posx, this->speed);     
          this->direction = true; }
                                         
    else if (arduboy.pressed(RIGHT_BUTTON) && this->posx < 120'00 
                                           && !this->tongueRet && !this->tongueLen
                                           && game->checkBrick((posPx + 8)/4))        
        { add_bound(&this->posx, this->speed, 12000); 
          this->direction = false; }
}

void Bean::Creator::update(Game *game)
{

}

void Angel::Creator::schedule(Game *game)
{
    char index = 0;
    if (! ~(game->bricks)) return;  // Check if there is a broken brick.
                                    // We bitwise-NOT the mask, and if the
                                    // result is 0 then all bricks are intact

    while (true) {
        char i = random(0, 31);
        if (!game->checkBrick(i)) {
            index = i + 1;
            break;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (!sched[i]) {
            sched[i] = index;
            break;
        } else if (sched[i] == index) {     // Prevent duplicates
            break;
        }
    }
}

void Angel::Creator::update(Game *game)
{
    if (this->sched[0]) {   // None of this needs to occur if the schedule is empty
        if (this->timer == 0) {
            // We find an empty slot to add an angel
            for (int i = 0; i < 8; i++)
                if (!game->angels[i]) {
                    game->angels[i] = this->create(this->sched[0]);
                    break;
                }
            // We shift all schedule elements to the left
            for (int i = 1; i < 8; i++) 
                this->sched[i - 1] = this->sched[i];
            this->sched[7] = 0;
            // We reset the timer
            timer = 36;
        } else if (game->metronome % 10 == 0) {
            timer--;
        }
    }
}


Bean *Bean::Creator::create(bool allowsuper)
{
    Bean *b = new Bean();
    char t = random(1, 100);

    if (t < 15)                     b->type = B_HEAL;
    else if (allowsuper && t < 20)  b->type = B_SUPER_A;
    else                            b->type = B_NORMAL;
    b->x = random(0, 120);
    b->y = 0;
    b->speed = random(this->avgSpeed - 3, this->avgSpeed + 3);
    return b;
}

Angel *Angel::Creator::create(char replaces)
{
    Angel *a = new Angel();

    a->replaces = replaces;
    return a;
}


Bird::tongue_pos Bird::getTonguePos(void)
{
    Bird::tongue_pos pos;

    pos.basex = this->direction? this->posx/100 : (this->posx/100) + 8;
    pos.basey = 53;

    pos.tipx = this->direction ? pos.basex - (this->tongueLen/100)
                               : pos.basex + (this->tongueLen/100);
    pos.tipy = 53 - (this->tongueLen/100);

    return pos;
}

void Bird::draw(void)
{
    ardbitmap.drawBitmap(this->posx / 100, 52,           // X and Y coords
                         this->tongueLen? bird+8 : bird, // bird+8 is a second sprite
                         8, 8,                           // Size of the sprite
                         WHITE, ALIGN_NONE,              // Color params
                         this->direction? MIRROR_NONE : MIRROR_HORIZONTAL
                        );
}

void Bird::drawTongue(void)
{
    Bird::tongue_pos pos = getTonguePos();

    arduboy.drawLine(pos.basex, pos.basey, pos.tipx, pos.tipy, WHITE);
}

void Bean::display(void)
{
    if (this->type % 2 == 0)
        arduboy.drawBitmap(x, y/100, bean,   4, 5, WHITE);
    else
        arduboy.drawBitmap(x, y/100, bean+4, 4, 5, WHITE);
}

void Bean::update(Game *game, int tongueTipX, int tongueTipY)
{
    // The player grabbed the bean
    if (game->aPressed  && rough_eq(this->y / 100, tongueTipY, 3)
                        && rough_eq(this->x,       tongueTipX, 2)) {
        // Award score according to height
        if      (this->y > 4000) game->score.update(50);
        else if (this->y > 2400) game->score.update(100);
        else if (this->y > 1000) game->score.update(500);
        else                      game->score.update(1000);

        // Special effect (if applicable)
        switch (this->type) {
        case B_HEAL:
            game->angelCreator->schedule(game);
            break;
        case B_SUPER_A:
        case B_SUPER_B:
            for (int w = 0; w < 8; w++) // Repair 8 bricks
                game->angelCreator->schedule(game);
            for (Bean *bean : game->beans) {
                delete bean;
                game->score.update(50); // Award 50 pts per bean on screen
            }
            break;
        default: break;
        }

        delete this;
    } else if (this->y >= 6000) {      // The bean hit the ground
        if (game->checkBrick((this->x / 4) + 1) == 1) {
            game->flipBrick((this->x / 4) + 1, false);  // Remove brick
            game->flipBrick((this->x / 4) + 1, true);   // Enable animation
        }
        
        delete this;
    } else if (this->y >= 5000         // The bean hit the player
            && rough_eq(this->x, game->player->posx/100 + 2, 8)) {
        game->gameOver();
    } else {                            // Business as usual
        this->y += this->speed;

        // Animation flip-flop
        if      (this->type == B_SUPER_A)   this->type = B_SUPER_B;
        else if (this->type == B_SUPER_B)   this->type = B_SUPER_A;
    }
}

void Angel::display(void)
{
    // We draw the angel
    arduboy.drawBitmap((this->replaces * 4) - 2, Angel::animation[this->step],
                       angel,
                       8, 8, WHITE
                      );
    if (this->step < 5) // and the brick it's holding in the first steps
        arduboy.drawBitmap(this->replaces * 4, Angel::animation[this->step] + 8,
                           brick,
                           4, 4, WHITE
                          );
}

void Angel::update(Game *game)
{
    if (this->step == 7) {
        delete this;            // Ran out of steps, unload
        return;
    } else if (game->metronome % 10) {
        this->step ++;          // Perform angel animation
    }

    if (this->step == 5)        // Replace brick
        game->flipBrick(this->replaces, false);
}
