/*===========================================
        GRRLIB (GX Version)
============================================*/
#include <grrlib.h>
#include <stdlib.h>
#include <math.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <sys/time.h>
#include "font_png.h"
#include "ystar_jpg.h"
#include "boxes_jpg.h"
#include "cross_png.h"
#include "boxesInvert_jpg.h"
#include "shot_jpg.h"
#include "hellocpp.h"

const double MY_PI = 3.14159265358979323846; // Or higher precision

long long timeInMilliseconds() {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int main(int argc, char **argv) {
    // Initialise the Graphics & Video subsystem
    GRRLIB_Init();
    // Initialise the Wiimotes
    WPAD_Init();
    //  font
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_texImg *tex_boxes = GRRLIB_LoadTexture(boxes_jpg);
    GRRLIB_texImg *tex_boxes_invert = GRRLIB_LoadTexture(boxesInvert_jpg);
    GRRLIB_texImg *tex_ystar = GRRLIB_LoadTexture(ystar_jpg);
    GRRLIB_texImg *tex_shot = GRRLIB_LoadTexture(shot_jpg);
    GRRLIB_texImg *tex_cross = GRRLIB_LoadTexture(cross_png);



    GRRLIB_SetBackgroundColour(0x47, 0x25, 0xA8, 0x22);

    int answer = addNumbers(2,2);//call function from c++ file
    // Declare a character array large enough to hold the number + the null terminator
    //char strMath[20] = "2+2=";
    //char strAnswer[10];
    // Use sprintf to format the integer into the array
    //sprintf(strAnswer, "%d", answer);
    //strcat(strMath, strAnswer);

    float playerX = 0.0f;
    float playerY = 0.0f;
    float playerZ = -10.0f;

    float speed = 0.0f;
    float playerAcc = .00910f;
    float drag = .001f;
    float sensitivity = .002f;
    float lookRightAmount = 0.0f;
    float lookUpAmount = 0.0f;

    long long lastTime = timeInMilliseconds();
    long long nextSecond = lastTime + 1000;
    int fpsCount = 0;
    int fps = 0;
    int howManyBeamsTest = 0;

    loadYstars();

    srand(time(NULL));

    bool autoPilot = false;
    void toggleAutoPilot(){
        autoPilot = !autoPilot;
    }

    while(1) {
        long long timeLongLong = timeInMilliseconds();
        char strTime[20];
        sprintf(strTime, "%llu", timeLongLong);
        int timePassedSinceLastFrame = timeLongLong - lastTime;
        lastTime = timeLongLong;
        fpsCount++;
        char strFPS[20] = "FPS: ";
        if(timeLongLong >= nextSecond){
            fps = fpsCount;
            fpsCount = 0;
            nextSecond = timeLongLong + 1000;
        }
        char strFPS_Num[15];
        sprintf(strFPS_Num, "%d", fps);
        strcat(strFPS,strFPS_Num);

        if(abs(playerX) > 125 || abs(playerY) > 125 || abs(playerZ) > 125){
            //move stars beams and special
            moveStarsBeamsSpecialEtc(-playerX,-playerY,-playerZ);
            playerX = 0;
            playerY = 0;
            playerZ = 0;
        }
        loopAroundOutOfBoundsWorldObjects();
        bool moveIt = false;
        WPAD_ScanPads();  // Scan the Wiimotes
        GRRLIB_2dMode();
        // If [HOME] was pressed on the first Wiimote, break out of the loop
        u32 buttonsDown = WPAD_ButtonsDown(0);
        if (buttonsDown & WPAD_BUTTON_HOME){
            break;
        }
        u32 buttonsHeld = WPAD_ButtonsHeld(0);
        if(buttonsHeld & WPAD_BUTTON_RIGHT){//hold wiimote sideways
            //playerY += speed * timePassedSinceLastFrame;
            lookUpAmount += (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_LEFT){
            //playerY -= speed * timePassedSinceLastFrame;
            lookUpAmount -= (sensitivity * timePassedSinceLastFrame);

        }
        if(buttonsHeld & WPAD_BUTTON_UP){
            //playerX -= speed * timePassedSinceLastFrame;
            lookRightAmount -= (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_DOWN){
            //playerX += speed * timePassedSinceLastFrame;
            lookRightAmount += (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_1){
            moveIt = true;
        }
        if(buttonsDown & WPAD_BUTTON_PLUS){
            toggleAutoPilot();
        }
        if(moveIt || autoPilot){
            speed += (playerAcc * timePassedSinceLastFrame);
        }
        //if(buttonsHeld & WPAD_BUTTON_2 || buttonsDown & WPAD_BUTTON_2){
            beamBlocks(rand() % 2000 - 1000,rand() % 2000 - 1000,rand() % 2000 - 1000,timeLongLong);
            //special(timePassedSinceLastFrame);
        //}
        speed -= (speed * drag * timePassedSinceLastFrame);
        if(speed < 0){
            speed = 0;
        }
        //while(lookRightAmount > 2 * MY_PI){
        //    lookRightAmount -= (2 * MY_PI);
        //}
        //while(lookRightAmount < 0.0f){
        //    lookRightAmount += (2 * MY_PI);
        //}
        double quarterRotation = MY_PI / 2.0d - .01d;
        if(lookUpAmount > quarterRotation){
            lookUpAmount = quarterRotation;
        }
        if(lookUpAmount < -quarterRotation){
            lookUpAmount = -quarterRotation;
        }

        double cosa = cos(0);//yaw
        double sina = sin(0);

        double cosb = cos(-lookRightAmount);//pitch
        double sinb = sin(-lookRightAmount);

        double cosc = cos(-lookUpAmount);//roll
        double sinc = sin(-lookUpAmount);

        double Axx = cosa*cosb;
        double Axy = cosa*sinb*sinc - sina*cosc;
        double Axz = cosa*sinb*cosc + sina*sinc;

        double Ayx = sina*cosb;
        double Ayy = sina*sinb*sinc + cosa*cosc;
        double Ayz = sina*sinb*cosc - cosa*sinc;

        double Azx = -sinb;
        double Azy = cosb*sinc;
        double Azz = cosb*cosc;

        double px = 0;
        double py = 0;
        double pz = 1;

        float xLookTarget = Axx*px + Axy*py + Axz*pz;
        float yLookTarget = Ayx*px + Ayy*py + Ayz*pz;
        float zLookTarget = Azx*px + Azy*py + Azz*pz;

        if(buttonsHeld & WPAD_BUTTON_2){
            shoot(timeLongLong,playerX,playerY,playerZ,xLookTarget,yLookTarget,zLookTarget);
        }

        //float xLookTarget = -sin(lookRightAmount);
        //float zLookTarget = cos(lookRightAmount);
        //float yLookTarget = tan(lookUpAmount);//
        playerX = playerX + (speed * xLookTarget);
        playerY = playerY + (speed * yLookTarget);
        playerZ = playerZ + (speed * zLookTarget);
        xLookTarget += playerX;
        yLookTarget += playerY;
        zLookTarget += playerZ;

        GRRLIB_Camera3dSettings(playerX,playerY,playerZ, 0,1,0, xLookTarget,yLookTarget,zLookTarget);

        // ---------------------------------------------------------------------
        // Place your drawing code here
        // ---------------------------------------------------------------------

        //char strTest[10];
        //sprintf(strTest, "%d", howManyBeamsTest);

        GRRLIB_Printf(20, 20, tex_font, 0xFFFFFFFF, 1, "HELLO WORLD!");
        GRRLIB_Printf(20, 36, tex_font, 0xFFFFFFFF, 1, strTime);
        GRRLIB_Printf(20, 52, tex_font, 0xFFFFFFFF, 1, strFPS);
        //GRRLIB_Printf(20, 36, tex_font, 0xFFFFFFFF, 1, strMath);
        //GRRLIB_Printf(20, 68, tex_font, 0xFFFFFFFF, 1, strTest);

        //char* source_pointer = get_static_string();
        //char destination_array[101]; // SIZE must be large enough to hold the string, including the null terminator.
        //snprintf(destination_array, sizeof(destination_array), "%s", source_pointer);
        //GRRLIB_Printf(20, 100, tex_font, 0xFFFFFFFF, 1, destination_array);



        GRRLIB_3dMode(0.1, 2000, 120, 1, 0);
        GRRLIB_SetBlend(GRRLIB_BLEND_ALPHA);
        int howManyShots = loadShots(timeLongLong,timePassedSinceLastFrame);
        int howManyBeams = loadBeams(timePassedSinceLastFrame);
        int howManyBeamsSpecial = loadBeamsSpecial(timePassedSinceLastFrame);

        howManyBeamsTest = howManyBeams + howManyBeamsSpecial;

        GRRLIB_ObjectView(0,0,0,0,0,0,1.0f,1.0f,1.0f);
        GRRLIB_SetTexture(tex_ystar, FALSE);
        GX_Begin(GX_QUADS, GX_VTXFMT0, 50 * 24);//50 ystars

        drawY_Stars();

        GX_End();


        GRRLIB_ObjectView(0,0,0,0,0,0,1.0f,1.0f,1.0f);
        GRRLIB_SetTexture(tex_boxes_invert, FALSE);
        GX_Begin(GX_QUADS, GX_VTXFMT0, howManyBeams * 24);

        drawBeams();

        GX_End();

        GRRLIB_ObjectView(0,0,0,0,0,0,1.0f,1.0f,1.0f);
        GRRLIB_SetTexture(tex_boxes_invert, FALSE);
        GX_Begin(GX_QUADS, GX_VTXFMT0, howManyBeamsSpecial * 24);

        drawBeamsSpecial();

        GX_End();


        GRRLIB_ObjectView(0,0,0,0,0,0,1.0f,1.0f,1.0f);
        GRRLIB_SetTexture(tex_shot, FALSE);
        GX_Begin(GX_QUADS, GX_VTXFMT0, howManyShots * 24);

        drawShots();

        GX_End();


        GRRLIB_DrawImg(-10, -10, tex_cross, 0, 1, 1, 0xFFFFFFFF);  // Draw a picture


        GRRLIB_Render();  // Render the frame buffer to the TV

    }
    GRRLIB_FreeTexture(tex_boxes);
    GRRLIB_FreeTexture(tex_boxes_invert);
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_FreeTexture(tex_ystar);
    GRRLIB_FreeTexture(tex_shot);
    GRRLIB_Exit(); // clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}
