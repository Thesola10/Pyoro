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

class DialogBox
{
public:
    DialogBox(char lineA[16], char lineB[16], char prompt[10], bool center = false)
    {
        strncpy(this->lineA, lineA, 16);
        strncpy(this->lineB, lineB, 16);
        strncpy(this->prompt, prompt, 10);
        this->center = center;
    }
    void draw(void);

    static void loop(void); // Placeholder that waits for an A press
private:
    char lineA[16];
    char lineB[16];
    char prompt[10];
    bool center;            // If yes, dialog box origin is at 12 instead of 28
};
