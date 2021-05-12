/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __GAME_H
#define __GAME_H

class Game;

class Bird
{
public:
    struct tongue_pos {
        int tipx, tipy;
        int basex, basey;
    };

    Bird::tongue_pos getTonguePos(void);
    void update(Game *game);
    void draw(void);
    void drawTongue(void);
private:
    unsigned int posx;      // In 0.01 pixel units, Pyoro's x-coordinate
    int          speed;     // In 0.01 pixel units per second, Pyoro's x speed
    unsigned int tongueLen; // In 0.01 pixel units, Pyoro's tongue length
    bool         tongueRet; // True if Pyoro's tongue is retracting
    bool         direction; // True if Pyoro is facing left, false otherwise.
};

class Bean
{
public:
    enum type {             // Types of bean in Pyoro
        B_NONE      = 0,    // No bean
        B_NORMAL    = 1,    // Regular (green) bean, normal sprite
        B_HEAL      = 2,    // Replaces one brick if eaten, inverted sprite
        B_SUPER_A   = 3,    // Super-bean spinner A (replace 8 bricks)
        B_SUPER_B   = 4,    // Super-bean spinner B
    };
                            // NB: The super-bean spinner is used to make the bean blink. They are otherwise equivalent.

    class Creator
    {
    public:
        unsigned int freq;       // Minimum frames between 2 beans
        unsigned int avgSpeed;   // Average speed of a new bean
        
        void update(Game *game);
    private:
        unsigned int timer;      // Countdown until next bean appears
        
        Bean create(bool allowsuper);
    };

    unsigned int speed;     // In 0.01 pixel units per frame, vertical speed
    unsigned char x;        // In pixels, X position, does not change on instance
    unsigned int y;         // In 0.01 pixel units, Y position updated with speed
    Bean::type type;        // Current type of the bean

    void display(void);
    void update(Game *game, int tongueTipX, int tongueTipY);
};

class Angel
{
public:
    static const unsigned char animation[8] = { 0, 2, 24, 40, 48, 52, 40, 10 };
                    // In pixels, successive Y-positions of an angel
    
    class Creator
    {
    public:
        void schedule(Game *game);
        void update(Game *game);
    private:
        unsigned int timer = 0;     // Countdown until the next scheduled angel appears
        unsigned char sched[8];     // Scheduled angels

        Angel create(char replaces);
    };

    char replaces;  // Index of the brick being replaced
    char step = 1;  /* Animation step for Pyoro angel, as specified above
                     *
                     * Step 0 is invalid, angel not drawn
                     * At Step 5, the brick is no longer held, and restored in the mask
                     * At Step 7, the angel is unloaded
                     */

    void display(void);
};

#endif //__GAME_H
