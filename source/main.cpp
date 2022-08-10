#include <nds.h>
#include <gl2d.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <maxmod9.h>
#include "sprites.h"

#include "soundbank.h"
#include "soundbank_bin.h"
#include "pipeTop.h"
#include "bird.h"
#include "birdUp.h"
#include "birdDown.h"
#include "bgLong.h"
#include "floor.h"
#include "ready.h"
#include "bottom.h"
#include "scoreSprite.h"
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
inline void clrscr() { iprintf("\e[1;1H\e[2J"); }
static const int DMA_CHANNEL = 3;
void initVideo()
{
    /*
     *  Map VRAM to display a background on the main and sub screens.
     *
     *  The vramSetPrimaryBanks function takes four arguments, one for each of
     *  the major VRAM banks. We can use it as shorthand for assigning values
     *  to each of the VRAM bank's control registers.
     *
     *  We map banks A and B to main screen  background memory. This gives us
     *  256KB, which is a healthy amount for 16-bit graphics.
     *
     *  We map bank C to sub screen background memory.
     *
     *  We map bank D to LCD. This setting is generally used for when we aren't
     *  using a particular bank.
     *
     *  We map bank E to main screen sprite memory (aka object memory).
     */
    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000,
                        VRAM_B_MAIN_BG_0x06020000,
                        VRAM_C_SUB_BG_0x06200000,
                        VRAM_D_LCD);

    vramSetBankE(VRAM_E_MAIN_SPRITE);

    /*  Set the video mode on the main screen. */

    /*  Set the video mode on the sub screen. */
    videoSetModeSub(MODE_5_2D |          // Set the graphics mode to Mode 5
                    DISPLAY_BG3_ACTIVE); // Enable BG3 for display
}

void initBackgrounds()
{
    /*  Set up affine background 3 on main as a 16-bit color background. */
    REG_BG3CNT = BG_BMP16_256x256 |
                 BG_BMP_BASE(0) | // The starting place in memory
                 BG_PRIORITY(3);  // A low priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;

    /*  Place main screen background 3 at the origin (upper left of the
     *  screen).
     */
    REG_BG3X = 0;
    REG_BG3Y = 0;

    /*  Set up affine background 2 on main as a 16-bit color background. */
    REG_BG2CNT = BG_BMP16_128x128 |
                 BG_BMP_BASE(8) | // The starting place in memory
                 BG_PRIORITY(2);  // A higher priority

    /*  Set the affine transformation matrix for the main screen background 3
     *  to be the identity matrix.
     */
    REG_BG2PA = 1 << 8;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 1 << 8;

    /*  Place main screen background 2 in an interesting place. */
    REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
    REG_BG2Y = -32 << 8;

    /*  Set up affine background 3 on the sub screen as a 16-bit color
     *  background.
     */
    REG_BG3CNT_SUB = BG_BMP16_256x256 |
                     BG_BMP_BASE(0) | // The starting place in memory
                     BG_PRIORITY(3);  // A low priority

    /*  Set the affine transformation matrix for the sub screen background 3 to
     *  be the identity matrix.
     */
    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PB_SUB = 0;
    REG_BG3PC_SUB = 0;
    REG_BG3PD_SUB = 1 << 8;

    /*
     *  Place main screen background 3 at the origin (upper left of the screen)
     */
    REG_BG3X_SUB = 0;
    REG_BG3Y_SUB = 0;
}
void displayBottom()
{
    dmaCopyHalfWords(DMA_CHANNEL, bottomBitmap, (uint16 *)BG_BMP_RAM_SUB(0), bottomBitmapLen);
}

int main()
{
    SpriteInfo spriteInfo[SPRITE_COUNT];
    OAMTable *oam = new OAMTable();

    consoleDemoInit();
    initVideo();
    initBackgrounds();
    initOAM(oam);
    oamInit(&oamSub, SpriteMapping_1D_128, false);
    // displayBottom();
    videoSetModeSub(MODE_5_2D);
    // vramSetBankC(VRAM_C_SUB_BG_0x06200000);

    int bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    

    dmaCopy(bottomBitmap, bgGetGfxPtr(bg3), 256 * 192);
    dmaCopy(bottomBitmap, BG_PALETTE_SUB, 256 * 2);

    
    

    // Initialize Maxmod sound engine
    mmInitDefaultMem((mm_addr)soundbank_bin);
    // load sound effects
    mmLoadEffect(SFX_DING);
    mmLoadEffect(SFX_FLAP);
    mmLoadEffect(SFX_SMACK);
    mmLoadEffect(SFX_SMACKWHOOSHQUICK);
    mmLoadEffect(SFX_SMACKWHOOSHSLOW);
    mmLoadEffect(SFX_WHOOSH);

    videoSetMode(MODE_5_3D);
    // videoSetModeSub(MODE_2_2D);
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankB(VRAM_B_TEXTURE);
    vramSetBankD(VRAM_D_TEXTURE);
    vramSetBankE(VRAM_E_TEX_PALETTE); 

    // Top Screen Sprite setup
    glScreen2D();
    glImage bg_images[1];
    glLoadTileSet(bg_images,                                                                          // pointer to glImage array
                  512,                                                                                // sprite width
                  192,                                                                                // sprite height
                  512,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)bgLongPal,                                                                   // Load our 256 color tiles palette
                  (u8 *)bgLongBitmap                                                                  // image data generated by GRIT
    );

    glImage floor_images[1];
    glLoadTileSet(floor_images,                                                                       // pointer to glImage array
                  256,                                                                                // sprite width
                  192,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_128,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)floorPal,                                                                    // Load our 256 color tiles palette
                  (u8 *)floorBitmap                                                                   // image data generated by GRIT
    );
    glImage bird_images[1];
    glLoadTileSet(bird_images,                                                                        // pointer to glImage array
                  256,                                                                                // sprite width
                  192,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_128,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)birdPal,                                                                     // Load our 256 color tiles palette
                  (u8 *)birdBitmap                                                                    // image data generated by GRIT
    );
    glImage birdUp_images[1];
    glLoadTileSet(birdUp_images,                                                                      // pointer to glImage array
                  256,                                                                                // sprite width
                  192,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_128,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)birdUpPal,                                                                   // Load our 256 color tiles palette
                  (u8 *)birdUpBitmap                                                                  // image data generated by GRIT
    );
    glImage birdDown_images[1];
    glLoadTileSet(birdDown_images,                                                                    // pointer to glImage array
                  256,                                                                                // sprite width
                  192,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_128,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)birdDownPal,                                                                 // Load our 256 color tiles palette
                  (u8 *)birdDownBitmap                                                                // image data generated by GRIT
    );

    glImage pipeTop_images[1];
    glLoadTileSet(pipeTop_images,                                                                     // pointer to glImage array
                  26,                                                                                 // sprite width
                  161,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)pipeTopPal,                                                                  // Load our 256 color tiles palette
                  (u8 *)pipeTopBitmap                                                                 // image data generated by GRIT
    );
    glImage ready_images[1];
    glLoadTileSet(ready_images,                                                                       // pointer to glImage array
                  256,                                                                                // sprite width
                  128,                                                                                // sprite height
                  256,                                                                                // bitmap width
                  192,                                                                                // bitmap height
                  GL_RGB256,                                                                          // texture type for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_256,                                                                   // sizeX for glTexImage2D() in videoGL.h
                  TEXTURE_SIZE_128,                                                                   // sizeY for glTexImage2D() in videoGL.h
                  GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T | TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
                  256,                                                                                // Length of the palette to use (256 colors)
                  (u16 *)readyPal,                                                                    // Load our 256 color tiles palette
                  (u8 *)readyBitmap                                                                   // image data generated by GRIT
    );

    float yVel = 0;
    int scoreCounter = 0;
    int highScore = 0;
    int floorXPos = 0;
    int floor2XPos = 252;
    int overCounter = 0;
    int floatCounter = 0;

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
    while (1)
    {

        scanKeys();

        glBegin2D();

        // SPRITES!!!
        // background image
        glSprite(0, 0, GL_FLIP_NONE, bg_images);

        // DRAW PIPES & BIRD
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

        if ((collided || over) && overCounter < 6)
        {
            glBoxFilled(0, 0, 256, 192, RGB15(255, 255, 255));
            overCounter++;
        }
        // COLLISIONS
        if ((!over) && ((
                            //  compare bird yPos with top half of pipe's yPos
                            ((bird.yPos <= pipe1.pipeY - 25)
                             //  compare bird yPos with bottom half of pipe's yPos
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
            if (scoreCounter > highScore)
            {
                highScore = scoreCounter;
            }
        }

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
            // play sound
            mmEffect(SFX_DING);
        }

        if (!over)
        {
            // move pipes towards bird if not yet collided
            if (!collided && started)
            {
                pipe1.pipeX -= 1;
                pipe2.pipeX -= 1;
                pipe3.pipeX -= 1;
                pipe4.pipeX -= 1;
                floorXPos -= 1;
                floor2XPos -= 1;
            }
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
            if (keysDown() & KEY_START)
            {
                started = true;
            }
            if (keysDown() && started)
            {
                // move bird upwards
                yVel = -2.5;
                // play flap sound
                mmEffect(SFX_FLAP);
            }
        }

        if (!over && started)
        {
            bird.yPos += yVel;
            yVel += .125;
        }
        if (over && bird.yPos != 170)
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

        /*
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
        int currentKeysDown = keysDown();
        if ((KEY_START & currentKeysDown) && over)
        {
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
            over = false;
            collided = false;
            floorSound = true;
            started = false;
        }

        glFlush(0);
        swiWaitForVBlank();
        updateOAM(oam);
    }
    return 0;
}