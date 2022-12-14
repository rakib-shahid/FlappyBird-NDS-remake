#include <nds.h>
#include <gl2d.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <maxmod9.h>
//#include "sprites.h"

// sounds
#include "soundbank.h"
#include "soundbank_bin.h"

// sprites/images
#include "pipeTop.h"
#include "bird.h"
#include "birdUp.h"
#include "birdDown.h"
#include "bgLong.h"
#include "floor.h"
#include "ready.h"
#include "bottom.h"
#include "scoreSprite.h"
#include "startSub.h"
#include "allNums.h"
#include "miniNums.h"
#include "scoreCardFull.h"
#include "medals.h"
#include "shine.h"

//  classes to keep track of variables
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
    double yPos;
};

// clrscr function taken from libnds practical wiki https://github.com/NotImplementedLife/libnds-practical-wiki
// no longer used
// inline void clrscr() { iprintf("\e[1;1H\e[2J"); }

int main()
{
    consoleDemoInit();
    touchPosition touch;

    // Initialize Maxmod sound engine
    mmInitDefaultMem((mm_addr)soundbank_bin);
    // load sound effects
    mmLoadEffect(SFX_DING);
    mmLoadEffect(SFX_FLAP);
    mmLoadEffect(SFX_SMACK);
    mmLoadEffect(SFX_SMACKWHOOSHQUICK);
    mmLoadEffect(SFX_SMACKWHOOSHSLOW);
    mmLoadEffect(SFX_WHOOSH);

    // video and vram setup
    videoSetMode(MODE_5_3D);
    videoSetModeSub(MODE_5_2D);
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankC(VRAM_C_TEXTURE);
    // vramSetBankD(VRAM_D_SUB_SPRITE);
    vramSetBankI(VRAM_I_LCD);
    dmaCopy(scoreSpritePal, &VRAM_I_EXT_SPR_PALETTE[0][0], 512);
    dmaCopy(scoreCardFullPal, &VRAM_I_EXT_SPR_PALETTE[1][0], 512);
    dmaCopy(scoreCardFullPal, &VRAM_I_EXT_SPR_PALETTE[2][0], 512);
    dmaCopy(medalsPal, &VRAM_I_EXT_SPR_PALETTE[3][0], 512);
    dmaCopy(miniNumsPal, &VRAM_I_EXT_SPR_PALETTE[4][0], 512);
    dmaCopy(shinePal, &VRAM_I_EXT_SPR_PALETTE[5][0], 512);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
    vramSetBankE(VRAM_E_TEX_PALETTE);

    // Sub Screen Solid color Background
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    dmaCopy(bottomBitmap, bgGetGfxPtr(bg3), 2);
    dmaCopy(bottomBitmap, BG_PALETTE_SUB, 2);
    // Sub Screen sprite setup
    oamInit(&oamSub, SpriteMapping_1D_128, true);
    u16 *spriteStartSubMem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_16Color);

    //score sprites refuse to show properly unless this variable is declared even though it remains unused
    u16 *spriteScoreMem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_16Color);

    u16 *spriteNumMem = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
    u16 *spriteNum2Mem = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
    u16 *spriteScoreCardMem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
    u16 *spriteScoreCard2Mem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
    u16 *spriteMedalMem = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
    u16 *spriteMiniNumMem = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    u16 *spriteMiniNum2Mem = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    u16 *spriteBestNumMem = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    u16 *spriteBestNum2Mem = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
    u16 *spriteShineMem = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);

    // Top Screen Sprite GL2D setup
    glScreen2D();

    glImage bg_images[1];
    glImage floor_images[1];
    glImage bird_images[1];
    glImage birdUp_images[1];
    glImage birdDown_images[1];
    glImage pipeTop_images[1];
    glImage ready_images[1];

    glLoadTileSet(bg_images,
                  512,
                  192,
                  512,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_256,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)bgLongPal,
                  (u8 *)bgLongBitmap);

    glLoadTileSet(floor_images,
                  256,
                  192,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_128,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)floorPal,
                  (u8 *)floorBitmap);

    glLoadTileSet(bird_images,
                  256,
                  192,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_128,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)birdPal,
                  (u8 *)birdBitmap);

    glLoadTileSet(birdUp_images,
                  256,
                  192,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_128,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)birdUpPal,
                  (u8 *)birdUpBitmap);

    glLoadTileSet(birdDown_images,
                  256,
                  192,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_128,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)birdDownPal,
                  (u8 *)birdDownBitmap);

    glLoadTileSet(pipeTop_images,
                  26,
                  161,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_256,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)pipeTopPal,
                  (u8 *)pipeTopBitmap);

    glLoadTileSet(ready_images,
                  256,
                  128,
                  256,
                  192,
                  GL_RGB256,
                  TEXTURE_SIZE_256,
                  TEXTURE_SIZE_128,
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT,
                  256,
                  (u16 *)readyPal,
                  (u8 *)readyBitmap);

    float yVel = 0;
    int scoreCounter = 0;
    int highScore = 0;
    int floorXPos = 0;
    int floor2XPos = 252;
    int overCounter = 0;
    int startCounter = 0;
    int floatCounter = 0;
    double scoreHeight = 30;
    double scoreNumHeight = 50;
    int scoreCardX = 64;
    double scoreCardY = 193;
    double scoreCardVel = 33;
    int medalCounter = 0;
    int medalTimer = 0;
    double startHeight = 193;
    int shineCounter = -10;

    // setup bird and pipe x and y values
    srand(time(0));
    Bird bird = Bird{77, 96};
    Pipe pipe1 = Pipe{276, 50 + (rand() % (143 - 49))};
    Pipe pipe2 = Pipe{276 + 69 + 5, 50 + (rand() % (143 - 49))};
    Pipe pipe3 = Pipe{276 + 69 * 2 + 10, 50 + (rand() % (143 - 49))};
    Pipe pipe4 = Pipe{276 + 69 * 3 + 15, 50 + (rand() % (143 - 49))};

    bool over = false;
    bool collided = false;
    // smackSound and floorSound booleans prevents death sound from infinitely playing
    bool smackSound = false;
    bool floorSound = true;

    bool started = false;

    // main loop
    while (1)
    {

        scanKeys();

        glBegin2D();

        // sub screen sprites
        if (started)
        {
            // "Score" text
            // dmaCopy(scoreSpritePal, SPRITE_PALETTE_SUB, 512);
            dmaCopy(scoreSpriteTiles, spriteStartSubMem, 64 * 64);
            if (scoreHeight > -65)
            {
                oamSet(&oamSub, 4, 128 - 21, scoreHeight, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, false, false, false, false);
            }

            // Sprites for when score < 10
            if (scoreCounter < 10)
            {
                // calculate which number to use from allNums image, each number is its own 32x32 box
                u8 *scoreNum = (u8 *)allNumsTiles + scoreCounter * 32 * 32;
                // copy to memory and display
                dmaCopy(scoreNum, spriteNumMem, 32 * 32);

                if (scoreNumHeight > -65)
                {
                    oamSet(&oamSub, 1, 128 - 16, scoreNumHeight, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNumMem, -1, false, false, false, false, false);
                }
            }
            // Sprites for 10 < score < 100
            // not yet implemented 100 and greater bc 3 digits, if u get to 100 find something better to do
            else if (scoreCounter < 100)
            {
                // tens digit
                u8 *scoreNum1 = (u8 *)allNumsTiles + (scoreCounter / 10) * 32 * 32;
                // ones digit
                u8 *scoreNum2 = (u8 *)allNumsTiles + (scoreCounter % 10) * 32 * 32;
                dmaCopy(scoreNum1, spriteNumMem, 32 * 32);
                if (scoreNumHeight > -65)
                {
                    oamSet(&oamSub, 1, 128 - 21, scoreNumHeight, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNumMem, -1, false, false, false, false, false);
                }
                dmaCopy(scoreNum2, spriteNum2Mem, 32 * 32);

                // slightly move the "1" sprite closer to tens digit since texture has gap for the number 1
                if (scoreNumHeight > -65)
                {
                    if (scoreCounter % 10 == 1)
                    {
                        oamSet(&oamSub, 2, 128 - 10, scoreNumHeight, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNum2Mem, -1, false, false, false, false, false);
                    }
                    else
                    {
                        oamSet(&oamSub, 2, 128 - 7, scoreNumHeight, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNum2Mem, -1, false, false, false, false, false);
                    }
                }
            }
        }
        if (over)
        {
            scoreNumHeight -= 3.5;
            scoreHeight -= 3.5;
        }
        // flash "PRESS START" on sub screen if game not yet started
        if (!started)
        {

            startCounter++;
            if (startCounter == 60)
            {
                startCounter = 0;
            }
            // dmaCopy(startSubPal, SPRITE_PALETTE_SUB, 512);
            dmaCopy(startSubTiles, spriteStartSubMem, 64 * 64);
            if (startCounter < 40)
            {
                oamSet(&oamSub, 4, 96, 30, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, false, false, false, false);
            }
            else
            {
                oamSet(&oamSub, 4, 96, 30, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, true, false, false, false);
            }
        }

        // main screen sprites
        // background image
        glSprite(0, 0, GL_FLIP_NONE, bg_images);

        // draw Pipes
        glSprite(pipe1.pipeX, pipe1.pipeY - 185, GL_FLIP_NONE, pipeTop_images);
        glSprite(pipe1.pipeX, pipe1.pipeY + 25, GL_FLIP_V, pipeTop_images);

        glSprite(pipe2.pipeX, pipe2.pipeY - 185, GL_FLIP_NONE, pipeTop_images);
        glSprite(pipe2.pipeX, pipe2.pipeY + 25, GL_FLIP_V, pipeTop_images);

        glSprite(pipe3.pipeX, pipe3.pipeY - 185, GL_FLIP_NONE, pipeTop_images);
        glSprite(pipe3.pipeX, pipe3.pipeY + 25, GL_FLIP_V, pipeTop_images);

        glSprite(pipe4.pipeX, pipe4.pipeY - 185, GL_FLIP_NONE, pipeTop_images);
        glSprite(pipe4.pipeX, pipe4.pipeY + 25, GL_FLIP_V, pipeTop_images);

        // Floor sprites
        if (floorXPos == -252)
        {
            floorXPos = 252;
        }
        if (floor2XPos == -252)
        {
            floor2XPos = 252;
        }
        glSprite(floorXPos, 176, GL_FLIP_NONE, floor_images);
        glSprite(floor2XPos, 176, GL_FLIP_NONE, floor_images);

        // draw bird
        if (started)
        {
            if (!over)
            {
                if (yVel < -.7)
                {
                    // glSprite(bird.xPos-129, bird.yPos-90, GL_FLIP_NONE, birdDown_images);
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, birdDown_images);
                }
                else if (yVel < 1)
                {
                    // glSprite(bird.xPos-129, bird.yPos-90, GL_FLIP_NONE, bird_images);
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, bird_images);
                }
                else
                {
                    // glSprite(bird.xPos-129, bird.yPos-90, GL_FLIP_NONE, birdUp_images);
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, birdUp_images);
                }
            }
            else
            {
                glSpriteRotate(bird.xPos, bird.yPos, yVel * 1000, GL_FLIP_NONE, birdUp_images);
            }
        }
        // float the bird before the game is started
        if (!started)
        {
            if (bird.yPos < 110)
            {
                yVel += 0.05;
                bird.yPos += yVel;
                if (yVel < 0)
                {

                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, birdDown_images);
                }
                else if (bird.yPos < 96 && bird.yPos > 92)
                {
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, bird_images);
                }
                else
                {
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, birdUp_images);
                }
            }
            else
            {
                yVel -= .25;
                bird.yPos += yVel;
                if (bird.yPos < 98 && bird.yPos > 92)
                {
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, bird_images);
                }
                else
                {
                    glSpriteRotate(bird.xPos, bird.yPos + 4, yVel * 1000, GL_FLIP_NONE, birdDown_images);
                }
            }
            floatCounter++;

            glSprite(0, 0, GL_FLIP_NONE, ready_images);
            floor2XPos--;
            floorXPos--;
        }

        // flash white for 6 frames when dead
        if ((collided || over) && overCounter < 6)
        {
            glBoxFilled(0, 0, 256, 192, RGB15(255, 255, 255));
            overCounter++;
        }

        // COLLISIONS
        if ((!over) && ((
                            //  check if bird is in top half of pipe
                            ((bird.yPos <= pipe1.pipeY - 25)
                             //  check if bird is in bottom half of pipe
                             || (bird.yPos + 10 > (pipe1.pipeY + 25)))
                            //  check if bird is within pipe's xPos
                            && ((bird.xPos + 5 >= pipe1.pipeX) && (bird.xPos <= pipe1.pipeX + 24))) ||
                        (((bird.yPos <= pipe2.pipeY - 25) || (bird.yPos + 10 > (pipe2.pipeY + 25))) && ((bird.xPos + 5 >= pipe2.pipeX) && (bird.xPos <= pipe2.pipeX + 24))) || // same thing for pipe2
                        (
                            ((bird.yPos <= pipe3.pipeY - 25) || (bird.yPos + 10 > (pipe3.pipeY + 25))) && ((bird.xPos + 5 >= pipe3.pipeX) && (bird.xPos <= pipe3.pipeX + 24))) || // same thing for pipe3
                        (
                            ((bird.yPos <= pipe4.pipeY - 25) || (bird.yPos + 10 > (pipe4.pipeY + 25))) && ((bird.xPos + 5 >= pipe4.pipeX) && (bird.xPos <= pipe4.pipeX + 24)))) // same thing for pipe4

        )
        {
            collided = true;
            over = true;
            smackSound = true;
        }

        // set high score if new high score
        if (scoreCounter > highScore)
        {
            highScore = scoreCounter;
        }

        u8 *bestScoreNum1 = (u8 *)miniNumsTiles + (highScore / 10) * 16 * 16;
        u8 *bestScoreNum2 = (u8 *)miniNumsTiles + (highScore % 10) * 16 * 16;
        // u8 *miniScoreNum = (u8 *)miniNumsTiles + (scoreCounter % 10) * 16 * 16;
        dmaCopy(bestScoreNum2, spriteBestNumMem, 16 * 16);
        if (highScore >= 10)
        {
            dmaCopy(bestScoreNum1, spriteBestNum2Mem, 16 * 16);
        }

        // play smack sound once when dead
        if (smackSound)
        {
            mmEffect(SFX_SMACK);
            smackSound = false;
        }

        // SCORE
        if ((!collided) &&
            ((pipe1.pipeX + 3 == bird.xPos) || (pipe2.pipeX + 3 == bird.xPos) || (pipe3.pipeX + 3 == bird.xPos) || (pipe4.pipeX + 3 == bird.xPos)))
        {
            scoreCounter++;
            // play ding sound
            mmEffect(SFX_DING);
        }

        // move pipes towards bird if not yet collided
        if (!over && !collided && started)
        {
            pipe1.pipeX -= 1;
            pipe2.pipeX -= 1;
            pipe3.pipeX -= 1;
            pipe4.pipeX -= 1;
            floorXPos -= 1;
            floor2XPos -= 1;
        }

        // reset pipes once they move off screen
        if (pipe1.pipeX <= -26)
        {
            pipe1.pipeX = 276;
            pipe1.pipeY = 40 + (rand() % (143 - 49));
        }
        if (pipe2.pipeX <= -26)
        {
            pipe2.pipeX = 276;
            pipe2.pipeY = 40 + (rand() % (143 - 49));
        }
        if (pipe3.pipeX <= -26)
        {
            pipe3.pipeX = 276;
            pipe3.pipeY = 40 + (rand() % (143 - 49));
        }
        if (pipe4.pipeX <= -26)
        {
            pipe4.pipeX = 276;
            pipe4.pipeY = 40 + (rand() % (143 - 49));
        }

        // MOVEMENT
        if (!over)
        {
            if (!started)
            {
                // start the game
                if ((keysDown() & KEY_START) || (keysDown() & KEY_TOUCH))
                {
                    started = true;
                    oamSet(&oamSub, 4, 128 - 32, 30, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, true, false, false, false);
                }
            }
            if (keysDown() && started)
            {
                // move bird upwards
                yVel = -2.5;
                // play flap sound
                mmEffect(SFX_FLAP);
            }
        }

        if ((!over && started) || (over && bird.yPos != 170))
        {
            bird.yPos += yVel;
            yVel += .125;
        }

        // prevents bird from leaving bottom
        if (bird.yPos >= 170)
        {
            if (floorSound && !collided)
            {
                mmEffect(SFX_SMACK);
                floorSound = false;
            }
            bird.yPos = 170;
            over = true;
        }

        if (scoreNumHeight <= -64 && over && bird.yPos == 170)
        {
            // show the score card
            // dmaCopy(scoreCardFullPal,SPRITE_PALETTE_SUB,scoreCardFullPalLen);
            // vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
            dmaCopy((u8 *)scoreCardFullTiles + 0 * 64 * 64, spriteScoreCardMem, 64 * 64);
            dmaCopy((u8 *)scoreCardFullTiles + 1 * 64 * 64, spriteScoreCard2Mem, 64 * 64);
            if (scoreCardY > (192 / 2 - 64))
            {
                if (scoreCardVel > 0)
                {
                    scoreCardY -= scoreCardVel;
                    scoreCardVel *= .8;
                }
            }
            oamSet(&oamSub, 9, scoreCardX, scoreCardY, 0, 2, SpriteSize_64x64, SpriteColorFormat_256Color, spriteScoreCardMem, -1, false, false, false, false, false);
            oamSet(&oamSub, 10, scoreCardX + 64, scoreCardY, 0, 2, SpriteSize_64x64, SpriteColorFormat_256Color, spriteScoreCard2Mem, -1, false, false, false, false, false);
            if (scoreCounter >= 40)
            {
                medalCounter = 4;
            }
            else if (scoreCounter >= 30)
            {
                medalCounter = 3;
            }
            else if (scoreCounter >= 20)
            {
                medalCounter = 2;
            }
            else if (scoreCounter >= 10)
            {
                medalCounter = 1;
            }
            if (medalCounter >= 1)
            {
                if (medalTimer >= 60)
                {
                    dmaCopy((u8 *)medalsTiles + (medalCounter - 1) * 32 * 32, spriteMedalMem, 32 * 32);
                    oamSet(&oamSub, 1, scoreCardX + 15, scoreCardY + 20, 0, 3, SpriteSize_32x32, SpriteColorFormat_256Color, spriteMedalMem, -1, false, false, false, false, false);
                    //animate the shine, looks pretty awful
                    if (medalTimer > 80){
                        shineCounter++;
                        if (shineCounter < 10){
                            dmaCopy((u8 *)shineTiles + 0 * 32 * 32, spriteShineMem, 32 * 32);
                            oamSet(&oamSub, 0, scoreCardX+10, scoreCardY+15, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, false, false, false, false);
                        }
                        else if (shineCounter < 20){
                            dmaCopy((u8 *)shineTiles + 1 * 32 * 32, spriteShineMem, 32 * 32);
                            oamSet(&oamSub, 0, scoreCardX+10, scoreCardY+15, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, false, false, false, false);
                        }
                        else if (shineCounter < 60){
                            oamSet(&oamSub, 0, scoreCardX+10, scoreCardY+15, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, true, false, false, false);
                        }
                        else if (shineCounter < 70){
                            dmaCopy((u8 *)shineTiles + 0 * 32 * 32, spriteShineMem, 32 * 32);
                            oamSet(&oamSub, 0, scoreCardX+20, scoreCardY+27, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, false, false, false, false);
                        }
                        else if (shineCounter < 90){
                            dmaCopy((u8 *)shineTiles + 1 * 32 * 32, spriteShineMem, 32 * 32);
                            oamSet(&oamSub, 0, scoreCardX+20, scoreCardY+27, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, false, false, false, false);
                        }
                        else if (shineCounter > 100){
                            oamSet(&oamSub, 0, scoreCardX+20, scoreCardY+27, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, true, false, false, false);
                        }
                        if (shineCounter == 140) {
                            shineCounter = 0;
                        }

                    }
                    //dmaCopy((u8 *)shineTiles + 1 * 32 * 32, spriteShineMem, 32 * 32);
                }
                if (keysDown() & KEY_TOUCH)
                {
                    medalTimer = 60;
                }
            }

            if (scoreCardY <= (192 / 2 - 64))
            {
                u8 *miniScoreNum1 = (u8 *)miniNumsTiles + (scoreCounter / 10) * 16 * 16;
                u8 *miniScoreNum2 = (u8 *)miniNumsTiles + (scoreCounter % 10) * 16 * 16;
                // u8 *miniScoreNum = (u8 *)miniNumsTiles + (scoreCounter % 10) * 16 * 16;
                dmaCopy(miniScoreNum2, spriteMiniNumMem, 16 * 16);
                oamSet(&oamSub, 3, 162, 48, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNumMem, -1, false, false, false, false, false);
                if (scoreCounter >= 10)
                {
                    dmaCopy(miniScoreNum1, spriteMiniNum2Mem, 16 * 16);
                    oamSet(&oamSub, 2, 154, 48, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNum2Mem, -1, false, false, false, false, false);
                }

                oamSet(&oamSub, 6, 162, 68, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteBestNumMem, -1, false, false, false, false, false);
                if (highScore >= 10)
                {
                    oamSet(&oamSub, 7, 154, 68, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteBestNum2Mem, -1, false, false, false, false, false);
                }
                scoreCardVel = 8;
                dmaCopy(startSubTiles, spriteStartSubMem, 64 * 64);
                oamSet(&oamSub, 4, 128 - 32, startHeight, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, false, false, false, false);
                if (startHeight >= 90)
                {
                    startHeight -= scoreCardVel;
                    scoreCardVel *= 0.5;
                }
            }

            medalTimer++;
        }

        /*
        // printing to console for testing
        if (over && bird.yPos == 170)
        {
            clrscr();
            // iprintf("\n\nyPos=%d\n", bird.yPos);
            iprintf("\n\n\n\n\n\n\n\n\n\t\t\t\tGame Over!\n");
            iprintf("  Press START button to restart");
            iprintf("\n\n\n\t\t    Final Score=%d\n", scoreCounter);
            if (scoreCounter >= 30)
            {
                iprintf("\t\t\tGold medal!\n");
            }
            else if (scoreCounter >= 20)
            {
                iprintf("\t\t\tSilver medal!\n");
            }
            else if (scoreCounter >= 10)
            {
                iprintf("\t\t\tBronze medal!\n");
            }
            iprintf("\n\n\n\t\t    Best Score=%d\n", highScore);
            over = true;
        }*/

        // Restart game
        // if player touches PRESS START sprite
        int currentKeysDown = keysDown();
        bool touchedStart = false;
        if (over)
        {
            if (startHeight <= 90)
            {
                if (KEY_TOUCH & currentKeysDown)
                {
                    touchRead(&touch);
                    if (((touch.px <= 96 + 64) && (touch.px >= 96)) && ((touch.py <= 90 + 64) && (touch.py >= 90)))
                    {
                        touchedStart = true;
                    }
                }
            }
        }
        //or if player presses START key
        //restart
        if (((KEY_START & currentKeysDown) && over) || (touchedStart))
        {
            oamSet(&oamSub, 5, 128 - 21, 30, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteStartSubMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 2, 128 - 21, 30, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNumMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 3, 128 - 21, 30, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteNum2Mem, -1, false, true, false, false, false);
            oamSet(&oamSub, 9, scoreCardX, scoreCardY, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteScoreCardMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 10, scoreCardX + 64, scoreCardY, 0, 0, SpriteSize_64x64, SpriteColorFormat_256Color, spriteScoreCard2Mem, -1, false, true, false, false, false);
            oamSet(&oamSub, 1, scoreCardX + 15, scoreCardY + 20, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, spriteMedalMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 4, 162, 48, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNumMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 6, 154, 48, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNum2Mem, -1, false, true, false, false, false);
            oamSet(&oamSub, 7, 162, 68, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNumMem, -1, false, true, false, false, false);
            oamSet(&oamSub, 8, 154, 68, 0, 4, SpriteSize_16x16, SpriteColorFormat_256Color, spriteMiniNum2Mem, -1, false, true, false, false, false);
            oamSet(&oamSub, 0, scoreCardX+22, scoreCardY+28, 0, 5, SpriteSize_32x32, SpriteColorFormat_256Color, spriteShineMem, -1, false, true, false, false, false);
            bird.yPos = 96;
            yVel = 0;
            pipe1.pipeX = 276;
            pipe1.pipeY = 40 + (rand() % (143 - 49));
            pipe2.pipeX = 276 + 69 + 5;
            pipe2.pipeY = 40 + (rand() % (143 - 49));
            pipe3.pipeX = 276 + 69 * 2 + 10;
            pipe3.pipeY = 40 + (rand() % (143 - 49));
            pipe4.pipeX = 276 + 69 * 3 + 15;
            pipe4.pipeY = 40 + (rand() % (143 - 49));
            floorXPos = 0;
            floor2XPos = 252;
            overCounter = 0;
            scoreCounter = 0;
            scoreHeight = 30;
            scoreNumHeight = 50;
            scoreCardX = 64;
            scoreCardY = 193;
            startHeight = 193;
            scoreCardVel = 33;
            medalCounter = 0;
            medalTimer = 0;
            shineCounter = 0;
            over = false;
            collided = false;
            floorSound = true;
            started = false;
            touchedStart = false;
        }
        // glEnd2D();
        // glEnd();
        glFlush(0);

        swiWaitForVBlank();

        // DC_FlushAll();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
    return 0;
}