/**
 * Pyoro clone for the Arduboy.
 * This program is licensed under the terms of the Creative Commons Zero (CC0) license.
 *
 * Authors:
 * - Karim Vergnes  <me@thesola.io>             Original code
 *
 * Kindly update this comment block when editing this source file, thanks!
 */

#include "logic.hxx"
#include "graphics.hxx"
#include "assets.hxx"

static void MainMenu::init(void)
{

}

static void MainMenu::loop(void)
{

}

void MainMenu::drawBackdrop(void)
{
    arduboy.clear();
    // we draw the floor
    for (int i = 0; i < 32; i++)
        arduboy.drawBitmap(i*4, 60, brick, 4,  4, WHITE); 
    // then the Pyoro logo
    arduboy.drawBitmap( 4,  4, pyoroLogo, 56, 25, WHITE);
    // then Pyoro at x=20
    arduboy.drawBitmap(20, 52, bird,       8,  8, WHITE);
    // then we hide brick 16 (x=60)
    arduboy.fillRect  (60, 60,             4,  4, BLACK);
    // we place an angel with a brick at that coord
    arduboy.drawBitmap(58, 28, angel,      8,  8, WHITE);
    arduboy.drawBitmap(60, 36, brick,      4,  4, WHITE);
}


static void Game::init(void)
{

}

static void Game::loop(void)
{
    
}

void Game::gameOver(void)
{
    doLoop = &DialogBox::loop;
    DialogBox("GAME OVER", this->score.text,
              this->score.value > hiscore ? "NEW RECORD!" : "").draw();
    if (this->score.value > hiscore) {
        EEPROM.put(ROM_HISCORE, this->score.value);
        hiscore = this->score.value;
    }
    delete this;
}


char Game::checkBrick(int index)
{
    if      (this->bricks >> index & 1) return 1;
    else if (this->brickAnim >> index & 1) return 2;
}

void Game::flipBrick(int index, bool anim)
{
    if (anim) this->brickAnim ^= 1 << index;
    else      this->bricks ^= 1 << index;
}

void Game::updateBeans(void)
{
    Bird::tongue_pos tpos = player->getTonguePos();
    for (int i = 0; i < 16; i++) {
        if (beans[i] && beans[i]->type != Bean::type::B_NONE)
            beans[i]->update(this, tpos.tipx, tpos.tipy);
    }
    beanCreator->update(this);
}

void Game::updateAngels(void)
{
    for (int i = 0; i < 8; i++) {
        if (angels[i] && angels[i]->step) {
            angels[i]->display();
            if (!(this->metronome % 10))
                angels[i]->step ++;
            if (angels[i]->step == 5)
                this->flipBrick(angels[i]->replaces, true);
            else if (angels[i]->step == 7)
                delete angels[i];
        }
    }
    angelCreator->update(this);
}

void Game::Score::update(int add)
{
    this->value += add;
    this->level = this->value % 500;

    itoa(this->value, this->text, 10);
}

void Game::Score::draw(void)
{
    arduboy.print(this->text);
}
