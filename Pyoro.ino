//// DEFINITIONS

#include <stdlib.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "assets.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

#define ROM_HISCORE EEPROM_STORAGE_SPACE_START
#define ROM_MAGIC   EEPROM_STORAGE_SPACE_START+2
#define ROM_SOUND   EEPROM_STORAGE_SPACE_START+3

// Savefile magic byte (increases if there is a difficulty update)
#define MAGIC_BYTE  0x02

#define ARDBITMAP_SBUF arduboy.sBuffer
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;


// Pointer manipulation, cause I can, ooh yeaah
void (*doLoop)() = &menuLoop;

unsigned int pyoroX, tngLen;  // Game state data (in .01 px)
bool tngRet;

unsigned int speed;           // deltaX of pyoro in .01 px/s

unsigned int bricksU, bricksL, brickAnimU, brickAnimL;   // masks for states of all 32 bricks
unsigned int score, hiscore, scoreLvl;
unsigned char metronome;      // +1 per frame

unsigned int beanTimer = 0;
unsigned int beanFreq;  // Frames between 2 beans (minimum)
unsigned int beanSpd;   // average speed of a bean in 1/100 px/s

unsigned char angelTimer = 15;

char scorebuf[6];               // buffer for score itoa

unsigned char angelSchedule[8]; // Brick index to repair + 1, scheduled angels to create

bool facingLeft; // true means pyoro is facing left, false means he is facing right
bool aHeld;      // Whether aPressed was true on previous frame.

struct bean {
  char speed; // deltaY in .01 px/s
  char x;     // X position, does not change
  int  y;     // Y position, updated thru speed, set in .01 px
  char type;  /* Types of beans in Pyoro
               * 
               * Type 0 is invalid, bean not drawn
               * Type 1 is the regular (green) bean, normal sprite
               * Type 2 replaces one brick if eaten, inverted sprite
               * Type 3 is super-bean spinner A (keep)
               * Type 4 is super-bean spinner B (invert)
               * 
               * This makes the bean blink. Types 3 and 4 are otherwise equal, replacing 8 bricks if eaten.
               */
};

struct angel {
  char replaces; // Index of the brick being replaced
  char step;     /* Animation steps for Pyoro angel
                  *
                  * Step 0 is invalid, angel not drawn
                  * Step 1: y = 2
                  * Step 2: y = 24
                  * Step 3: y = 40
                  * Step 4: y = 48
                  * Step 5: y = 52. Brick no longer held, restored in mask
                  * Step 6: y = 40
                  * Step 7: y = 10. Unload on next frame
                  */
};


struct bean  beans[16]; // Beans on screen
struct angel angels[8]; // Angels on screen

void setup() 
{
  arduboy.boot();
  arduboy.blank();         
  arduboy.flashlight();    // magic key protection
  arduboy.systemButtons(); // mute toggle
  arduboy.audio.begin();
  doBrandLogos();
  arduboy.waitNoButtons();
  
  // Magic number check. 
  if (EEPROM.read(ROM_MAGIC) != MAGIC_BYTE) 
  {
    initHiScore();
  } else 
  {
    hiscore = EEPROM.get(ROM_HISCORE, hiscore);
    showMenu();
  }
}

// doLoop() is a pointer to the current game logic loop.
void loop() 
{ doLoop(); }

//// MAIN LOOPS

void menuLoop() 
{
  if (arduboy.pressed(A_BUTTON)) initGameLogic();
  else if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(B_BUTTON)) initHiScore();
}

void dialogLoop()
{
  if (arduboy.pressed(A_BUTTON)) showMenu();
}

void gameLoop()
{
  arduboy.clear();
  arduboy.pollButtons();

  bool          aPressed = false; // true if tongue is legally extending 
  unsigned char tngLenPx = tngLen/100;
  unsigned char pyoroXPx = pyoroX/100;

  unsigned char pyoTongueX = facingLeft?pyoroXPx:(pyoroXPx+8);
  unsigned char tngTipX = facingLeft?pyoTongueX-tngLenPx:pyoTongueX+tngLenPx; 
  unsigned char tngTipY = 53-tngLenPx;
  unsigned int  scoreprev = score;
  
  // UPDATE INPUT
  if      (arduboy.pressed(A_BUTTON)     && tngLenPx < 52 
                                         && ((tngLenPx + pyoTongueX) < 128 || facingLeft)
                                         && (tngLenPx < pyoTongueX || !facingLeft)
                                         && !(aHeld && tngRet))                { add(&tngLen, speed*2, 12800); 
                                                                                  aPressed = true; }
                                         
  else if (arduboy.pressed(LEFT_BUTTON)  && pyoroX > 0   
                                         && !tngRet && !tngLen
                                         && checkBrick((pyoroXPx+4)/4))        { sub(&pyoroX, speed);     
                                                                                 facingLeft = true; }
                                         
  else if (arduboy.pressed(RIGHT_BUTTON) && pyoroX < 12000 
                                         && !tngRet && !tngLen
                                         && checkBrick((pyoroXPx+8)/4))        { add(&pyoroX, speed, 12000); 
                                                                                 facingLeft = false; }

  // DRAW PYORO
  ardbitmap.drawBitmap(pyoroXPx,52,tngLen?bird+8:bird,8,8, WHITE, ALIGN_NONE, facingLeft?MIRROR_NONE:MIRROR_HORIZONTAL);

  if (arduboy.justPressed(A_BUTTON)) 
      sound.tone(NOTE_C4H,30, NOTE_G4,30, NOTE_C5,30);
  
  // CHECK COLLISION & RENDER BEANS
  for (int i = 0; i < 16; i++)
  {
    if (beans[i].type) // if bean is valid (i.e. it exists)
    {
      beans[i].y += beans[i].speed; // make it move

      // render it
      if (beans[i].type%2 == 0) arduboy.drawBitmap(beans[i].x,beans[i].y/100,bean,  4,5,WHITE);
      else                      arduboy.drawBitmap(beans[i].x,beans[i].y/100,bean+4,4,5,WHITE);
      
      // pyoro ate the bean (4px precision)
      if (req(beans[i].y/100,tngTipY,3) && req(beans[i].x,tngTipX,2) && aPressed)
      {
        sound.tone(NOTE_C5H,50, NOTE_G5,50);
        // award points with height
        if      (beans[i].y > 4000) score += 50;
        else if (beans[i].y > 2400) score += 100;
        else if (beans[i].y > 1000) score += 500;
        else                        score += 1000;

        // special effect (if applicable)
        switch (beans[i].type)
        {
        case 2:
          //replace one brick
          schedAngel();
          break;
        case 3:
        case 4:
          //replace 8 bricks
          //kill all beans on frame and award 50pts for each
          for (int w = 0; w < 8; w++)
              schedAngel();
          for (int x = 0; x < 16; x++)
            if (beans[x].type)
            {
              beans[x].type = 0;
              score += 50;
            }
          break;
        }

        // unload the bean
        beans[i].type = 0;
        aPressed = false; // cancel tongue
      }

      // if we're at the ground
      if (beans[i].y >= 6000) 
      {
        beans[i].type = 0; // unload
        if (checkBrick((beans[i].x/4)+1) == 1)
        {
          setBrick((beans[i].x/4)+1,false);
          setBrick((beans[i].x/4)+1,true);
        }
      }
      
      // a bean hit pyoro on the head
      if (beans[i].y >= 5000 && req(beans[i].x, (pyoroXPx+2),8)) gameOver();
      
      // superbean flip flop
      if (beans[i].type == 3)      beans[i].type = 4;
      else if (beans[i].type == 4) beans[i].type = 3;
      
    } else if (!beanTimer) // bean timer ran out, so we create a new bean
    {
      char t = random(1, 100);
      if      (t < 15)           beans[i].type = 2;
      else if (scoreLvl > 1250 
                     && t < 20)  beans[i].type = 3;
      else                       beans[i].type = 1;
      beans[i].x = random(0,120);
      beans[i].y = 0;
      beans[i].speed = random(beanSpd-3, beanSpd+3);
      beanTimer      = random(beanFreq, beanFreq+10); // reset timer with random parameter
    }
  }
  beanTimer--;
  
  // RENDER ANGELS
  for (int j = 0; j < 8; j++)
  {
    if(angels[j].step)
    {
      unsigned char ypos;
      if (angels[j].step < 5)
      {
        
        switch (angels[j].step)
        {
        case 1:
          ypos = 2;
          break;
        case 2:
          ypos = 24;
          break;
        case 3:
          ypos = 40;
          break;
        case 4:
          ypos = 48;
          break;
        }
        arduboy.drawBitmap((angels[j].replaces*4),ypos+8,brick,4,4, WHITE); 
        if (!(metronome%10)) angels[j].step++;
      }
      if(angels[j].step == 5)
      {
        ypos = 52;
        setBrick(angels[j].replaces,false); // flip brick back on
        angels[j].step++;
      }
      else 
      {
        switch (angels[j].step)
        {
        case 6:
          ypos = 40;
          if (!(metronome%5)) angels[j].step++;
          break;
        case 7:
          ypos = 10;
          angels[j].step = 0; // unload
          break;
        }
      }
      arduboy.drawBitmap((angels[j].replaces*4)-2,ypos,angel,8,8, WHITE);
    } else if (angelSchedule[0]) // if an angel is scheduled to appear
    {
      if (!angelTimer)
      {
        angels[j].step = 1;
        angels[j].replaces = angelSchedule[0] - 1;

        for (int l = 0; l < 8; l++)
             angelSchedule[l-1] = angelSchedule[l]; // shift array indexes to the left once

        angelSchedule[7] = 0;
        
        angelTimer = 36; // reset timer
      }
    }
  }
  if (angelSchedule[0]) angelTimer --;

  // RENDER BRICKS
  for (char k = 0; k < 32; k++)
  {
    if (checkBrick(k) == 1)
    {
      arduboy.drawBitmap(k*4,60,brick,4,4,WHITE);
    }
    else if (checkBrick(k) == 2)
    {
      arduboy.drawBitmap(k*4,60,brick+4,4,4,WHITE);
      setBrick(k,true); // flip bit back to zero
    }
  }

  // RENDER TONGUE
  if (tngLen > 0) 
  {
    arduboy.drawLine(pyoTongueX,53,tngTipX,tngTipY, WHITE);
    if (!aPressed) 
    {
      sub(&tngLen,200); // retract
      tngRet = true;
    }
  } else tngRet = false;

  // SET CONTINUITY CHECK AND PRINT SCORE
  aHeld = aPressed;
  metronome ++;
  
  arduboy.setCursor(0,0);

  if (scoreprev-score) 
  {
    itoa(score, scorebuf, 10);

    // Increasing difficulty every 500 points
    while (score >= scoreLvl)
    {
      sub(&beanFreq, 20,60);
      add(&beanSpd,  1, 20);
      add(&speed,    1, 30);
      scoreLvl += 500;
    }
  }

  arduboy.print(scorebuf);
  
  arduboy.display();
  arduboy.nextFrame();
}

//// GAME LOGIC TRANSITIONS

void doBrandLogos()
{
  arduboy.clear();
  arduboy.drawBitmap(12,24,mylogo,112,19, WHITE);
  arduboy.display();
  delay(2000);
}

void showMenu()
{
  arduboy.clear();
  // we draw the floor
  for (int i = 0; i < 32; i++) arduboy.drawBitmap(i*4,60,brick,4,4,WHITE); 
  // then the Pyoro logo
  arduboy.drawBitmap( 4, 4,pyoroLogo,56,25, WHITE);
  // then Pyoro at x=20
  arduboy.drawBitmap(20,52,bird,     8, 8,  WHITE);
  // then we hide brick 16 (x=60)
  arduboy.fillRect(60,60,4,4, BLACK);
  // we place an angel with a brick at that coord
  arduboy.drawBitmap(58,28,angel,    8, 8,  WHITE);
  arduboy.drawBitmap(60,36,brick,    4, 4,  WHITE);

  arduboy.setCursor(75,16);
  arduboy.print("Press A!");

  arduboy.setCursor(75,30);
  arduboy.print("HI SCORE:");

  arduboy.setCursor(78,40);
  arduboy.print(hiscore);
  
  // and we display the result!
  arduboy.display();

  // Set the game logic to "Menu loop"
  doLoop = &menuLoop;
  
  delay(300); //anti-Bounce
}

void gameOver()
{
  // Set the game logic to "Dialog loop"
  doLoop = &dialogLoop;

  // We don't clear the game area, rather, we overlay the messagebox
  arduboy.display() ; // draw after the graphics queue is complete
  arduboy.fillRect(28,6,100,56, BLACK);
  arduboy.drawRect(28,6,100,56, WHITE);

  // Then we print "GAME OVER"
  arduboy.setCursor(30,8);
  arduboy.print("GAME OVER");

  // then the score (no cast needed with arduboy's print())
  arduboy.setCursor(30,20);
  arduboy.print(score);
  if (score > hiscore)
  {
    EEPROM.put(ROM_HISCORE, score);
    hiscore = score;
    arduboy.setCursor(30,30);
    arduboy.print("NEW RECORD!");
  }
  sound.tone(NOTE_C4H,50, NOTE_G3H,100);
  
  arduboy.display();
}

void initGameLogic()
{
  // Set the game logic to "Game loop"
  doLoop = &gameLoop;
  
  // Below, we reset all game variables
  
  // 0 <= pyoroX <= 12000
  pyoroX     = 200;       // Set pyoro's starting X
  tngLen     = 0;         // Set the tongue status to zero
  tngRet     = false;
  
  bricksU    = 0xFFFF;
  bricksL    = 0xFFFF;    // reset bricks
  brickAnimU = 0x0000;
  brickAnimL = 0x0000; 
  
  beanFreq   = 300;       // reset bean speed
  beanSpd    = 8;
  
  score      = 0;
  scoreLvl   = 250;
  speed      = 10;
  
  strcpy(scorebuf, "0");  // reset itoa buffer
  
  for (int i = 0; i < 16; i++)
        beans[i] = (struct bean){0,0,0,0};

  for (int j = 0; j < 8; j++)
  {
    angelSchedule[j] = 0;
    angels[j] = (struct angel){0,0};
  }

  delay(300); // Anti-bounce
}

void initHiScore()
{
  arduboy.clear();
  arduboy.fillRect(12,6,100,56, BLACK);
  arduboy.drawRect(12,6,100,56, WHITE);

  arduboy.setCursor(14,8);
  arduboy.print("A new save file");
  arduboy.setCursor(14,16);
  arduboy.print("was created.");

  arduboy.setCursor(45,50);
  arduboy.print("Press A...");
  EEPROM.write(ROM_MAGIC, MAGIC_BYTE);
  EEPROM.put(ROM_HISCORE, 0x0000);
  arduboy.display();
  delay(300);

  // Set the game logic to "Dialog loop"
  doLoop = &dialogLoop;
}

//// MATH FUNCTIONS

// check if a brick exists
char checkBrick(const unsigned char index)
{
  if (index > 15)
  {
    if      (bricksU    >> (index-15) & 1) return 1;
    else if (brickAnimU >> (index-15) & 1) return 2;
    else return 0;
  } else
  {
    if      (bricksL    >> index & 1) return 1;
    else if (brickAnimL >> index & 1) return 2;
    else return 0;
  }
}

// set a brick value
void setBrick(const unsigned char index, const bool anim)
{
  if (index > 15)
  {
    if (anim) brickAnimU ^= 1 << index-15;
    else bricksU ^= 1 << index-15;
  } else
  {
    if (anim) brickAnimL ^= 1 << index;
    else bricksL ^= 1 << index;
  }
}

// schedule an angel to repair a random broken block
void schedAngel()
{
  char index = 0;
  if (!(~bricksU || ~bricksL)) return; // avoid getting into an infinite loop if all bricks are repaired
  while (true) // While we haven't returned
  {
    int i = random(0,31);
    if (!checkBrick(i))   // check random brick
    {
      index = i+1;          // set the index and break out of the function
      break;
    }
  }
  for (int j = 0; j < 8; j++)
    if (!angelSchedule[j])
    {
      angelSchedule[j] = index;
      break;
    } else if (angelSchedule[j] == index) // prevent duplicates
    {
      break;
    }
  return;
}

// substract with underflow protection
void sub(unsigned int *value, const unsigned int difference)
{
  *value -= (difference <= *value)?difference:*value;
}

// overload for above-zero applications
void sub(unsigned int *value, const unsigned int difference, const unsigned int lower)
{
  *value -= (difference + lower <= *value)?difference:(*value-lower);
}

// add with upper limit
void add(unsigned int *value, const unsigned int add, const unsigned int upper)
{
  *value += ((add+*value) <= upper)?add:(upper-*value);
}

// roughly equal, used by collision check. For example, req(2,6,4) is true.
bool req(const unsigned int val1, const unsigned int val2, const unsigned char margin)
{
  unsigned int val2l = val2;
  unsigned int val2u = val2;
  sub(&val2l,margin);
  add(&val2u,margin,UINT_MAX);
  return (val1 >= (val2-margin)) && (val1 <= (val2+margin));
}
