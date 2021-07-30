/*
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include "main.hxx"

/**
 * \file        graphics.hxx
 * \brief       Miscellaneous graphics functions
 *
 * This file contains definitions for shared graphical components, such as the
 * dialog box.
 */

/**
 * This class represents a dialog box ready to be displayed on screen. It is
 * used for the "game over" screen, as well as the "create new save" screen.
 *
 * \brief       Class for a dialog box
 */
class DialogBox
{
public:
    /**
     * Create a new DialogBox object with two lines of text for a message, and
     * one line for a call to action message.
     *
     * \brief       Constructor for a dialog box
     * \param lineA     First line for message text
     * \param lineB     Second line for message text
     * \param prompt    Line of text for call to action
     * \param center    If true, the dialog box will be centered (origin at 12)
     *                  Otherwise, it will be aligned to the right (at 28)
     * \see     DialogBox::draw()   Display the dialog box to screen
     */
    DialogBox(char lineA[16], char lineB[16], char prompt[10], bool center)
    {
        strncpy(this->lineA, lineA, 16);
        strncpy(this->lineB, lineB, 16);
        strncpy(this->prompt, prompt, 10);
        this->center = center;
    }

    void draw(void);        //!< Display the dialog box to screen

    static void loop(void); //!< Placeholder that waits for an A press
private:
    char lineA[16];
    char lineB[16];
    char prompt[10];
    bool center;            // If yes, dialog box origin is at 12 instead of 28
};

// vim: ft=cpp.doxygen
