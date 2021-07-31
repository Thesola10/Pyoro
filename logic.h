/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#ifndef __LOGIC_H
#define __LOGIC_H
#include "game.h"

/**
 * \file        logic.h
 * \brief       Functions and objects that represent overarching game logic
 *
 * This file contains the functions and objects that define the normal running
 * states of the game.
 */

/**
 * This static class hosts functions related to the "main menu" running state.
 *
 * \brief       Functions for main menu state
 */
class MainMenu
{
public:
    static void loop(void);     //!< Default loop for main menu context
    static void init(void);     //!< Initialize the main menu context
private:
    void drawBackdrop(void);
};

/**
 * This class hosts state and functions for the currently running game.
 *
 * \brief       Current game state object
 */
class Game 
{
public:
    /**
     * This class is responsible for managing score and score-based difficulty,
     * as well as displaying the in-game score to the screen.
     *
     * \brief       Live score and difficulty object
     */
    class Score
    {
    public:
        void update(int add);   //!< Increase the score by the given value
        void draw(void);        //!< Display the score on screen
        
        unsigned int value;     //!< The current numerical value for the score
        unsigned int level;     //!< Difficulty level, increases every 500pts
        char text[8];           //!< Cache for the text (itoa) value to show
    };

    Score score;                    //!< The score object for the current game

    Bird *player;                   //!< The player object for the current game
    Bean::Creator   *beanCreator;   //!< The bean creator object
    Angel::Creator  *angelCreator;  //!< The angel creator object

    Bean  *beans[16];           //!< All beans currently on screen
    Angel *angels[8];           //!< All angels scheduled to appear

    /**
     * This is a 32-bit mask where each bit represents a brick. 1 means the
     * brick exists, 0 means it has been destroyed.
     *
     * \brief       Brick state mask
     * \see     Game::checkBrick()      Recommended method to read brick state
     * \see     Game::flipBrick()       Recommended method to update bricks
     */
    unsigned long bricks    = 0xFFFFFFFF;
    /**
     * This is a 32-bit mask where each bit represents a breaking brick.
     * 1 means the brick's breaking frame should be played, 0 means it should
     * not.
     *
     * \brief           Brick mid frame mask
     * \see     Game::flipBrick()       Recommended method to update animation
     */
    unsigned long brickAnim = 0x00000000;

    unsigned char metronome;    //!< General-purpose frame counter for interval

    bool aPressed;              //!< Whether A is pressed on current frame
    bool aHeld;                 //!< Whether A was pressed on previous frame

    /**
     * This function tells each existing @ref Bean to update with the player
     * tongue coordinates (to check if it was eaten), then tells the
     * @ref Bean::Creator to update (to create a new bean).
     *
     * \brief       Update bean collision and creation
     * \see     Bean::update()          Individual bean updater
     * \see     Bean::Creator::update() Updater to create new beans
     */
    void updateBeans(void);
    /**
     * This function renders scheduled @ref Angel s and restores their target
     * bricks, then tells the @ref Angel::Creator to flush its schedule into
     * new @ref Angel objects.
     *
     * \brief       Update angel animation and schedule
     * \see     Angel::Creator::update()    Updater to create new angels
     */
    void updateAngels(void);
    /**
     * This function places the game in a "Game Over" state. This destroys the
     * current @ref Game object, then displays a @ref DialogBox with the
     * "Game Over" message and saves the high score to EEPROM (if applicable).
     *
     * \brief       End the game with "Game Over" logic
     * \see     DialogBox               Class used to create the message
     */
    void gameOver(void);

    /**
     * This function inverts the state or animation state of a brick at index.
     *
     * \brief       Invert the state or animation for a brick
     * \param index     Position of the brick to update starting from 0
     * \param anim      If true, the animation state is updated. Otherwise, the
     *                  physical state is updated.
     */
    void flipBrick(int index, bool anim);
    /**
     * This function checks and returns the current state of a brick at index.
     *
     * \brief       Return current state of a brick
     * \param index     Position of the brick to check starting from 0
     * \return      0   if the brick is broken
     *              1   if the brick exists
     *              2   if the brick is in its breaking animation
     */
    char checkBrick(int index);

    static void init(void);     //!< Initialize a new game context
    static void loop(void);     //!< Default loop for game context
};

#endif //__LOGIC_H

// vim: ft=cpp.doxygen
