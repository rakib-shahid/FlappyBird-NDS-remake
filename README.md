# FlappyBird-NDS-remake


An in progress Nintendo DS remake of Flappy Bird

Run FlappyBird-NDS-remake.nds in an emulator or on a DS

or build it by running the "make" command in this directory (devkitPro + libnds needed, delete build folder first)

## Progress Pictures
<img src="/pics/flappybirdRemakeGif.gif" width="216" height="340">

## To-Do

- Sounds are working ~~except hitting ground~~ (Add whoosh sound after smack)

- SPRITES!

  * Background **and pipe** sprites now drawn,

    * ~~Bird sprite is bugged! :(~~

      * ~~Physics break if bird is drawn, likely vram or idk~~

        * Bird sprite fixed

- bottom screen

  * Added score and floor-colored background to bottom screen

    * Oddly, score sprites only works when creating an unused memory pointer for sprites (spriteScoreMem in line 82)
  
  * ~~Add Score card and medals~~

  * Add sprites for score >= 100

  * ~~Add "Press start" sprite when dead~~
