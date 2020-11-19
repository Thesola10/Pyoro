/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim "TheSola10" Vergnes  <thesola10@bobile.fr>       Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __LOGIC_H
#define __LOGIC_H
#include "game.h"

class MainMenu
{
public:
    static void loop(void);
private:
    void drawBackdrop(void);
};

class Game 
{
public:
    class Score
    {
    public:
        void update(int add);
        void draw(void);
        
        unsigned int value;
        unsigned int level;     // Difficulty level, increases every 500 points
        char text[8];           // We store the itoa for the score to save cycles
    };

    Score score;

    static void loop(void);
    void gameOver(void);
    void flipBrick(int index, bool anim);
    char checkBrick(int index);
private:
    Bird *player;
    Bean::Creator   *beanCreator;
    Angel::Creator  *angelCreator;

    Bean  *beans[16];           // Beans currently on screen
    Angel *angels[8];           // Angels currently on screen

    unsigned long bricks    = 0xFFFFFFFF, 
                  brickAnim = 0x00000000;   // masks for states of all 32 bricks
    unsigned char metronome;    // +1 per frame

    bool aPressed, aHeld;

    void updateBeans(void);
    void updateAngels(void);

};

#endif //__LOGIC_H
