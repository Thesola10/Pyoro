/*
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

/**
 * \file        game.hxx
 * \brief       In-game logic for bird, bean and angel
 *
 * This file contains definitions for objects that represent components of the
 * game, such as the @ref Bird, @ref Bean and @ref Angel.
 */

class Game;

/**
 * This class represents the player's controls, as well as the graphics for the
 * character (the bird) on screen.
 *
 * \brief       Object for the player character and logic
 */
class Bird
{
public:
    /**
     * This struct represents the location of the player's tongue, as two
     * 2-dimensional points. For the base point, only the x-coordinate is
     * significant, but the y-coordinate is added for compatibility with
     * drawing functions.
     *
     * \brief       Struct for the player's tongue location
     */
    struct tongue_pos {
        int tipx, tipy;     //!< In pixels, the coordinates for the tip
        int basex, basey;   //!< In pixels, the coordinates for the base
    };

    /**
     * This function takes the current state of the player's tongue, and
     * returns it in the @ref Bird::tongue_pos format for rendering.
     *
     * \brief       Retrieve tongue position in direct-to-render format
     * \return      A @ref Bird::tongue_pos instance representing the state of
     *              the player's tongue at the time of calling
     */
    Bird::tongue_pos getTonguePos(void);

    /**
     * This function observes which buttons are pressed, and performs an
     * update cycle on the player's state accordingly. This involves updating
     * the player's position, speed and direction, as well as the length of
     * their tongue.
     *
     * \brief       Perform updates on the player object
     * \param game      A @ref Game object representing the current game state
     */
    void update(Game *game);

    /**
     * This function displays the player character to the screen. This function
     * does not draw the player's tongue.
     *
     * \brief       Render the player to screen
     * \see Bird::drawTongue()      Render the player's tongue
     */
    void draw(void);
    /**
     * This function displays a line representing the player's tongue.
     *
     * \brief       Render the player's tongue to screen
     * \see Bird::getTonguePos()    Retrieve the position of the tongue
     */
    void drawTongue(void);
private:
    unsigned int posx;      // In 0.01 pixel units, Pyoro's x-coordinate
    int          speed;     // In 0.01 pixel units per second, Pyoro's x speed
    unsigned int tongueLen; // In 0.01 pixel units, Pyoro's tongue length
    bool         tongueRet; // True if Pyoro's tongue is retracting
    bool         direction; // True if Pyoro is facing left, false otherwise.
};

/**
 * This class represents a single bean. Beans move downward at a rate defined
 * when created. They grant points when they touch the player's tongue, they
 * destroy a brick from the ground if they hit the ground, and if they touch
 * the player directly, the game is over.
 *
 * \brief       Object for the bean
 */
class Bean
{
public:
    /**
     * This enum specifies the possible types a bean can take once created.
     * Some types have special properties which change the way the bean is
     * drawn or behaves.
     * 
     * In the case of @ref Bean::type::B_SUPER_A and @ref Bean::type::B_SUPER_B
     * the type value alternates between the two to create the flashing
     * animation. They are otherwise functionally equivalent.
     *
     * \brief       Types for a bean
     */
    enum type {             // Types of bean in Pyoro
        B_NONE      = 0,    //!< Invalid, the bean does not exist
        B_NORMAL    = 1,    //!< Regular (green) bean, normal sprite
        B_HEAL      = 2,    //!< Replaces one brick if eaten, inverted sprite
        B_SUPER_A   = 3,    //!< Super-bean (replaces 8 bricks) frame A
        B_SUPER_B   = 4,    //!< Super-bean (replaces 8 bricks) frame B
    };

    /**
     * This class represents the parameters with which to create new beans,
     * such as their average vertical speed, as well as the time elapsed since
     * the last bean was created.
     *
     * \brief       Parameters to create new beans
     */
    class Creator
    {
    public:
        /**
         * This value specifies the minimum amount of frames to wait before
         * creating a new bean. This is a component of the game's difficulty.
         *
         * \brief       Minimum frames between 2 beans
         */
        unsigned int freq;

        /**
         * This value specifies the median vertical speed for a given bean.
         * A random function is then applied to spice things up. This is a
         * component of the game's difficulty.
         *
         * \brief       Average speed of a new bean
         */
        unsigned int avgSpeed;
        
        /**
         * This function updates the internal countdown to create a new bean
         * and adds a new bean to the game state once that countdown reaches
         * zero.
         *
         * \brief       Update state of bean creator
         * \param game      The @ref Game object to add the bean to
         */
        void update(Game *game);
    private:
        unsigned int timer;      // Countdown until next bean appears
        
        Bean *create(bool allowsuper);
    };

    /**
     * This value represents the set vertical speed for the bean, in units of
     * 0.01 pixel per frame. This means that, in order for a bean to move one
     * pixel per frame, this value needs to be set to 100.
     *
     * \brief       Vertical speed of bean in 0.01 pixel/frame
     */
    unsigned int speed;     // In 0.01 pixel units per frame, vertical speed

    unsigned char x;        //!< In pixels, the set X-coordinate of the bean
    unsigned int y;         //!< In 0.01 pixel units, vertical position of bean
    Bean::type type;        //!< The set @ref Bean::type of the current bean

    /**
     * This function renders the current bean object on screen.
     *
     * \brief       Render bean object on screen
     */
    void display(void);

    /**
     * This function performs an update on the bean, such as downward movement
     * and checking collision with the player's tongue and the ground.
     *
     * \brief       Perform update on bean
     * \param game      A @ref Game object representing the current game state
     * \param tongueTipX The X-coordinate of the tip of the player's tongue
     * \param tongueTipY The Y-coordinate of the tip of the player's tongue
     * \see Bird::getTonguePos()
     *                  Obtain the player's tongue coordinates
     */
    void update(Game *game, int tongueTipX, int tongueTipY);
};

/**
 * This class represents a single angel. The angel is a character which carries
 * a brick, in order to replace it and repair the ground.
 *
 * \brief       Object for the angel
 */
class Angel
{
public:
    /**
     * A list of the successive vertical positions of an angel during its
     * animation. The purpose of the angel character is mostly cosmetic, so it
     * is important that we are able to carry out a smooth animation as it
     * descends. These values are pre-computed as it isn't useful to calculate
     * an unchanging trajectory dynamically.
     *
     * \brief       List of positions for descent animation
     */
    static const unsigned char animation[8] = { 0, 2, 24, 40, 48, 52, 40, 10 };
   
    /**
     * This class represents the parameters with which to create new angels,
     * such as which brick they are supposed to replace, as well as the time
     * elapsed since the last angel was shown.
     *
     * \brief       Parameters to create new angels
     */
    class Creator
    {
    public:
        /**
         * This function selects a broken brick at random, and schedules it to
         * be repaired by the next angel
         *
         * \brief       Schedule a new coordinate for an angel to repair
         * \param game      A @ref Game object representing the current state
         */
        void schedule(Game *game);
        /**
         * This function periodically reads the schedule for bricks to replace
         * and adds a new angel to the corresponding location in the game.
         *
         * \brief       Update state of angel creator
         * \param game      The @ref Game object to add the angel to
         */
        void update(Game *game);
    private:
        unsigned int timer = 0;     // Countdown until the next scheduled angel
        unsigned char sched[8];     // Scheduled angels

        Angel *create(char replaces);
    };

    char replaces;  //!< Set index of the brick that the angel replaces

    /**
     * This value represents a step in the angel's animation. Note that certain
     * values are functionally significant:
     *
     * - Step 0 is invalid, the angel does not exist
     * - At Step 5, the angel is no longer holding the brick as it is repaired
     * - At Step 7, the angel has finished its animation and is unloaded
     *
     * \brief       Current animation step
     * \see Angel::animation    Definition for the angel's animation frames
     */
    char step = 1;

    /**
     * This function renders the current angel object on screen.
     *
     * \brief       Render angel object on screen
     */
    void display(void);

    /**
     * This function performs an update on the angel, such as animation step
     * and replacing the targeted brick.
     *
     * \brief       Perform update on angel
     * \param game      A @ref Game object representing the current game state
     */
    void update(Game *game);
};

#endif //__GAME_H

// vim: ft=cpp.doxygen
