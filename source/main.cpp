#include <nds.h>
#include <gl2d.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"
#include "bg.h"
//using namespace SNF;
// classes to keep track of variables
class Pipe
{
public:
    int pipeX;
    int pipeY;
};
class Bird
{
public:
    int xPos;
    int yPos;
};

// clrscr function taken from libnds practical wiki https://github.com/NotImplementedLife/libnds-practical-wiki
inline void clrscr() { iprintf("\e[1;1H\e[2J"); }

int main()
{
    consoleDemoInit();

    // Initialize Maxmod sound engine
    mmInitDefaultMem((mm_addr)soundbank_bin);
    // load sound effects
    mmLoadEffect(SFX_DING);
    mmLoadEffect(SFX_FLAP);
    mmLoadEffect(SFX_SMACK);
    mmLoadEffect(SFX_SMACKWHOOSHQUICK);
    mmLoadEffect(SFX_SMACKWHOOSHSLOW);
    mmLoadEffect(SFX_WHOOSH);


    

    //background
    
    videoSetMode(MODE_5_3D);
    //bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    
    //int x = bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    //decompress(bgBitmap, BG_GFX,  LZ77Vram);
    //videoBgDisable(x);
    //videoSetMode(MODE_5_3D);

    glScreen2D();
    
    float yVel = 0;
    int score = 0;

    srand(time(0));
    Bird bird = Bird{77, 96};
    Pipe pipe1 = Pipe{276, 50 + (rand() % (143 - 49))};
    Pipe pipe2 = Pipe{276 + 69 + 5, 50 + (rand() % (143 - 49))};
    Pipe pipe3 = Pipe{276 + 69 * 2 + 10, 50 + (rand() % (143 - 49))};
    Pipe pipe4 = Pipe{276 + 69 * 3 + 15, 50 + (rand() % (143 - 49))};
    //Pipe pipeArray[] = {pipe1, pipe2, pipe3, pipe4};

    bool over = false;
    bool collided = false;
    //smackSound and floorSound booleans prevents death sound from infinitely playing
    bool smackSound = false;
    bool floorSound = true;
    //bool deathHigh = false;
    while (1)
    {

        scanKeys();

        glBegin2D();

        // DRAW PIPES & BIRD

        // draw Pipes
        glBoxFilled(pipe1.pipeX, pipe1.pipeY - 25, pipe1.pipeX + 20, -10, RGB15(85, 255, 0));
        glBoxFilled(pipe1.pipeX, pipe1.pipeY + 25, pipe1.pipeX + 20, 192, RGB15(85, 255, 0));

        glBoxFilled(pipe2.pipeX, pipe2.pipeY - 25, pipe2.pipeX + 20, -10, RGB15(85, 255, 0));
        glBoxFilled(pipe2.pipeX, pipe2.pipeY + 25, pipe2.pipeX + 20, 192, RGB15(85, 255, 0));

        glBoxFilled(pipe3.pipeX, pipe3.pipeY - 25, pipe3.pipeX + 20, -10, RGB15(85, 255, 0));
        glBoxFilled(pipe3.pipeX, pipe3.pipeY + 25, pipe3.pipeX + 20, 192, RGB15(85, 255, 0));

        glBoxFilled(pipe4.pipeX, pipe4.pipeY - 25, pipe4.pipeX + 20, -10, RGB15(85, 255, 0));
        glBoxFilled(pipe4.pipeX, pipe4.pipeY + 25, pipe4.pipeX + 20, 192, RGB15(85, 255, 0));

        // draw bird
        glBoxFilled(bird.xPos, bird.yPos, bird.xPos + 10, bird.yPos + 10, RGB15(102, 255, 255));
        

        // COLLISIONS

        // looping attempt
        // looping through pipe array doesnt work :( need 2 figure out why
        /*
        for (Pipe x : pipeArray){
            if (
             //  compare bird yPos with top half of pipe's yPos
                 ((bird.yPos<=x.pipeY-25)
             //  compare bird yPos with bottom half of pipe's yPos
             ||  (bird.yPos+10>(x.pipeY+25)))
             //  check if bird is within pipe's xPos
             &&  ((bird.xPos+10 >= x.pipeX)  &&  (bird.xPos <= x.pipeX+20))
             ){
             collided = true;
            }
        }
        */

        // working collisions
        if ((!over) && ((
                            //  compare bird yPos with top half of pipe's yPos
                            ((bird.yPos <= pipe1.pipeY - 25)
                            //  compare bird yPos with bottom half of pipe's yPos
                            || (bird.yPos + 10 > (pipe1.pipeY + 25)))
                            //  check if bird is within pipe's xPos
                            && ((bird.xPos + 10 >= pipe1.pipeX) && (bird.xPos <= pipe1.pipeX + 20))) || 
                        (
                            ((bird.yPos <= pipe2.pipeY - 25) || (bird.yPos + 10 > (pipe2.pipeY + 25))) && ((bird.xPos + 10 >= pipe2.pipeX) && (bird.xPos <= pipe2.pipeX + 20))) ||  // same thing for pipe2
                        (
                            ((bird.yPos <= pipe3.pipeY - 25) || (bird.yPos + 10 > (pipe3.pipeY + 25))) && ((bird.xPos + 10 >= pipe3.pipeX) && (bird.xPos <= pipe3.pipeX + 20))) ||  // same thing for pipe3
                        (
                            ((bird.yPos <= pipe4.pipeY - 25) || (bird.yPos + 10 > (pipe4.pipeY + 25))) && ((bird.xPos + 10 >= pipe4.pipeX) && (bird.xPos <= pipe4.pipeX + 20))))    // same thing for pipe4

        )
        {
            /*
            if      (((bird.xPos + 10 >= pipe1.pipeX) && (bird.xPos <= pipe1.pipeX + 20)) && (bird.yPos <= pipe1.pipeY - 25)){
                deathHigh = true;
            }
            else if (((bird.xPos + 10 >= pipe3.pipeX) && (bird.xPos <= pipe3.pipeX + 20)) && (bird.yPos <= pipe2.pipeY - 25)){
                deathHigh = true;
            }
            else if (((bird.xPos + 10 >= pipe3.pipeX) && (bird.xPos <= pipe3.pipeX + 20)) && (bird.yPos <= pipe3.pipeY - 25)){
                deathHigh = true;
            }
            else if (((bird.xPos + 10 >= pipe4.pipeX) && (bird.xPos <= pipe4.pipeX + 20)) && (bird.yPos <= pipe4.pipeY - 25)){
                deathHigh = true;
            }
            else {
                deathHigh = false;
            }*/
            collided = true;
            over = true;
            smackSound = true;
        }

        /*
        if (smackSound && deathHigh)
        {
            mmEffect(SFX_SMACKWHOOSHSLOW);
            smackSound = false;
        }
        else if (smackSound && !deathHigh)
        {
            mmEffect(SFX_SMACKWHOOSHQUICK);
            smackSound = false;
        }*/
        if (smackSound)
        {
            mmEffect(SFX_SMACK);
            smackSound = false;
        }

        

        // SCORE
        if (
            (pipe1.pipeX == bird.xPos) || (pipe2.pipeX == bird.xPos) || (pipe3.pipeX == bird.xPos) || (pipe4.pipeX == bird.xPos))
        {
            score++;
            // play sound!
            mmEffect(SFX_DING);
        }

        glEnd2D();
        if (!over)
        {
            // move pipes towards bird if not yet collided
            if (!collided)
            {
                pipe1.pipeX -= 1;
                pipe2.pipeX -= 1;
                pipe3.pipeX -= 1;
                pipe4.pipeX -= 1;
            }
        }

        // reset pipes once they move off screen
        if (pipe1.pipeX <= -20)
        {
            pipe1.pipeX = 276;
            pipe1.pipeY = 50 + (rand() % (143 - 49));
        }
        if (pipe2.pipeX <= -20)
        {
            pipe2.pipeX = 276;
            pipe2.pipeY = 50 + (rand() % (143 - 49));
        }
        if (pipe3.pipeX <= -20)
        {
            pipe3.pipeX = 276;
            pipe3.pipeY = 50 + (rand() % (143 - 49));
        }
        if (pipe4.pipeX <= -20)
        {
            pipe4.pipeX = 276;
            pipe4.pipeY = 50 + (rand() % (143 - 49));
        }

        // MOVEMENT
        if (!over)
        {
            if (keysDown())
            {
                //move bird upwards
                yVel = -2.2;
                //play flap sound
                mmEffect(SFX_FLAP);
            }
        }

        if (bird.yPos < 182){
            bird.yPos += yVel;
        }
        yVel += .125;


        // dont let bird go out of the screen
        // prevents from leaving top
        if (bird.yPos < -10)
        {
            bird.yPos = -10;
        }
        // prevents from leaving bottom
        if (bird.yPos >= 182)
        {
            if (floorSound && !collided){
                mmEffect(SFX_SMACK);
                floorSound = false;
            }
            bird.yPos = 182;
            over = true;
        }

        // BOTTOM SCREEN

        // printing and testing
        clrscr();
        
        iprintf("yPos=%d\n", bird.yPos);/*
        iprintf("pipe1X=%d\n", pipe1.pipeX);
        iprintf("pipe1Y=%d\n", pipe1.pipeY);
        iprintf("collided?=%s\n", collided ? "true" : "false");
        */
        iprintf("\n\n\n\n\n\t\tScore=%d\n", score);

        if (over)
        {
            clrscr();
            iprintf("\n\nyPos=%d\n", bird.yPos);
            iprintf("\n\n\n\n\n\n\n\n\n\t\t\t\tGame Over!\n");
            iprintf("  Press START button to restart");
            over = true;
        }
        
        //Restart game
        int currentKeysDown = keysDown();
        if ((KEY_START & currentKeysDown) && over)
        {
            bird.yPos = 96;
            yVel = 0;
            pipe1.pipeX = 276;
            pipe1.pipeY = 50 + (rand() % (143 - 49));
            pipe2.pipeX = 276 + 69 + 5;
            pipe2.pipeY = 50 + (rand() % (143 - 49));
            pipe3.pipeX = 276 + 69 * 2 + 10;
            pipe3.pipeY = 50 + (rand() % (143 - 49));
            pipe4.pipeX = 276 + 69 * 3 + 15;
            pipe4.pipeY = 50 + (rand() % (143 - 49));
            score = 0;
            over = false;
            collided = false;
            floorSound = true;
        }

        glFlush(0);
        swiWaitForVBlank();
    }
    return 0;
}