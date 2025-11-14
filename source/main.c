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
#include "sky_jpg.h"
#include "ystar_jpg.h"
#include "boxes_jpg.h"
#include "cross_png.h"
#include "boxesInvert_jpg.h"
#include "shot_jpg.h"
#include "hellocpp.h"

const double MY_PI = 3.14159265358979323846;

// Define a 3D Vector
typedef struct {
    float x, y, z;
} Vector3;

// Define a Quaternion
// [w, x, y, z] -> w is the scalar (real) part, (x, y, z) is the vector (imaginary) part
typedef struct {
    float w;
    Vector3 v;
} Quaternion;

// Dot product of two vectors
float dot(Vector3 v1, Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Cross product of two vectors
Vector3 cross(Vector3 v1, Vector3 v2) {
    Vector3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

// Vector scaling (e.g., v * scalar)
Vector3 scale(Vector3 v, float s) {
    Vector3 result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

// Vector addition
Vector3 add(Vector3 v1, Vector3 v2) {
    Vector3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// --- Quaternion Functions ---

/**
 * @brief Creates a quaternion from a normalized axis and an angle (in radians).
 */
Quaternion quat_from_axis_angle(Vector3 axis, float angle_rad) {
    float half_angle = angle_rad / 2.0f;
    float sin_half = sinf(half_angle);

    Quaternion q;
    q.w = cosf(half_angle);
    q.v.x = axis.x * sin_half;
    q.v.y = axis.y * sin_half;
    q.v.z = axis.z * sin_half;

    return q;
}

/**
 * @brief Returns the conjugate of a quaternion (q*).
 */
Quaternion quat_conjugate(Quaternion q) {
    Quaternion conj;
    conj.w = q.w;
    conj.v.x = -q.v.x;
    conj.v.y = -q.v.y;
    conj.v.z = -q.v.z;
    return conj;
}

/**
 * @brief Multiplies two quaternions (q_new = q1 * q2).
 */
Quaternion quat_multiply(Quaternion q1, Quaternion q2) {
    Quaternion q_new;

    // w = w1*w2 - dot(v1, v2)
    q_new.w = q1.w * q2.w - dot(q1.v, q2.v);

    // v = w1*v2 + w2*v1 + cross(v1, v2)
    Vector3 term1 = scale(q2.v, q1.w); // w1 * v2
    Vector3 term2 = scale(q1.v, q2.w); // w2 * v1
    Vector3 term3 = cross(q1.v, q2.v); // v1 x v2

    Vector3 sum = add(term1, term2);
    q_new.v = add(sum, term3);

    return q_new;
}

/**
 * @brief Rotates a 3D vector by the quaternion (v' = q * v_pure * q*).
 */
Vector3 quat_rotate_vector(Quaternion q_rot, Vector3 vec) {
    // 1. Convert 3D vector to a pure quaternion: q_pure = [0, vec.x, vec.y, vec.z]
    Quaternion q_pure = {0.0f, vec};

    // 2. Compute the conjugate
    Quaternion q_conj = quat_conjugate(q_rot);

    // 3. Compute q * v_pure
    Quaternion temp = quat_multiply(q_rot, q_pure);

    // 4. Compute (q * v_pure) * q_conjugate
    Quaternion q_rotated = quat_multiply(temp, q_conj);

    // 5. The result is the imaginary part (x, y, z)
    return q_rotated.v;
}

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
    GRRLIB_texImg *tex_sky = GRRLIB_LoadTexture(sky_jpg);




    //GRRLIB_SetBackgroundColour(0x47, 0x25, 0xA8, 0x22);
    GRRLIB_SetBackgroundColour(0x47, 0x0, 0x0, 0x22);

    //int answer = addNumbers(2,2);//call function from c++ file
    // Declare a character array large enough to hold the number + the null terminator
    //char strMath[20] = "2+2=";
    //char strAnswer[10];
    // Use sprintf to format the integer into the array
    //sprintf(strAnswer, "%d", answer);
    //strcat(strMath, strAnswer);

    float playerX = 0.0f;
    float playerY = 0.0f;
    float playerZ = 0.0f;


    float speed = 0.0f;
    float playerAcc = .00420f;
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
    PAD_Init();

    Vector3 x_axis = {1.0f, 0.0f, 0.0f};//axis of rotation
    Vector3 y_axis = {0.0f, 1.0f, 0.0f};//axis of rotation
    Vector3 z_axis = {0.0f, 0.0f, 1.0f};//axis of rotation
    float angle_rad = lookUpAmount;//how much to rotate
    Quaternion quaternionX = quat_from_axis_angle(x_axis, 0);
    Quaternion quaternionY = quat_from_axis_angle(y_axis, 0);
    Quaternion quaternionZ = quat_from_axis_angle(z_axis, 0);
    Quaternion quaternion = quat_from_axis_angle(x_axis, 0);//just initialize with whatever works


    while(1) {
        float rotatePlayerX = 0;
        float rotatePlayerY = 0;
        float rotatePlayerZ = 0;
        WPAD_ScanPads();  // Scan the Wiimotes
        PAD_ScanPads();
        int buttonsDownGameCube = PAD_ButtonsDown(0);
        int buttonsHeldGameCube = PAD_ButtonsHeld(0);
        // not great for the arrow buttons being smooth on gc controller
        int stickX = PAD_StickX(0);
        int stickY = PAD_StickY(0);
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

        GRRLIB_2dMode();
        // If [HOME] was pressed on the first Wiimote, break out of the loop
        u32 buttonsDown = WPAD_ButtonsDown(0);
        if (buttonsDown & WPAD_BUTTON_HOME || buttonsDownGameCube & PAD_BUTTON_MENU){
            break;
        }
        u32 buttonsHeld = WPAD_ButtonsHeld(0);
        if(buttonsHeld & WPAD_BUTTON_RIGHT || buttonsHeldGameCube & PAD_BUTTON_UP){//hold wiimote sideways
            rotatePlayerX = (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_LEFT || buttonsHeldGameCube & PAD_BUTTON_DOWN){
            rotatePlayerX = (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_UP || buttonsHeldGameCube & PAD_BUTTON_LEFT){
            rotatePlayerY = (sensitivity * timePassedSinceLastFrame);
        }
        if(buttonsHeld & WPAD_BUTTON_DOWN || buttonsHeldGameCube & PAD_BUTTON_RIGHT){
            rotatePlayerY = (sensitivity * timePassedSinceLastFrame);
        }
        if( stickX > 18 || stickX < -18){
            rotatePlayerY = (stickX * sensitivity * timePassedSinceLastFrame * .00777);
        }
        if( stickY > 18 || stickY < -18){
            rotatePlayerX = (stickY * sensitivity * timePassedSinceLastFrame * .00777);
        }
        if(buttonsHeld & WPAD_BUTTON_1 || buttonsHeldGameCube & PAD_BUTTON_A){//PAD_TRIGGER_R
            moveIt = true;
        }
        if(buttonsDown & WPAD_BUTTON_PLUS || buttonsDownGameCube & PAD_TRIGGER_Z){
            toggleAutoPilot();
        }
        if(moveIt || autoPilot){
            speed += (playerAcc * timePassedSinceLastFrame);
        }
        //if(buttonsHeld & WPAD_BUTTON_2){
            beamBlocks(rand() % 2000 - 1000,rand() % 2000 - 1000,rand() % 2000 - 1000,timeLongLong);
            //special(timePassedSinceLastFrame);
        //}
        speed -= (speed * drag * timePassedSinceLastFrame);
        if(speed < 0){
            speed = 0;
        }


        // Define the directions (vectors) we want to rotate
        Vector3 forward_vector = {0.0f, 1.0f, 0.0f}; //
        Vector3 up_vector = {0.0f, 0.0f, 1.0f}; //

        quaternionX = quat_from_axis_angle(x_axis, -rotatePlayerX);
        quaternionY = quat_from_axis_angle(y_axis, rotatePlayerY);
        quaternionZ = quat_from_axis_angle(z_axis, 0);
        Quaternion q_delta_frame = quat_multiply(quaternionX,quat_multiply(quaternionY,quaternionZ));

        quaternion = quat_multiply(quaternion, q_delta_frame);

        Vector3 rotated_forward_vector = quat_rotate_vector(quaternion, forward_vector);
        Vector3 rotated_up_vector = quat_rotate_vector(quaternion, up_vector);





        if(buttonsHeld & WPAD_BUTTON_2 || buttonsHeldGameCube & PAD_BUTTON_X || buttonsHeldGameCube & PAD_BUTTON_Y){
            shoot(timeLongLong,playerX,playerY,playerZ,rotated_forward_vector.x,rotated_forward_vector.y,rotated_forward_vector.z);
        }

        playerX = playerX + (speed * rotated_forward_vector.x);
        playerY = playerY + (speed * rotated_forward_vector.y);
        playerZ = playerZ + (speed * rotated_forward_vector.z);





        float eyeX = playerX - (rotated_forward_vector.x * .01f);
        float eyeY = playerY - (rotated_forward_vector.y * .01f);
        float eyeZ = playerZ - (rotated_forward_vector.z * .01f);


        GRRLIB_Camera3dSettings(eyeX, eyeY, eyeZ, rotated_up_vector.x, rotated_up_vector.y, rotated_up_vector.z, playerX + rotated_forward_vector.x,playerY + rotated_forward_vector.y,playerZ + rotated_forward_vector.z);



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

        moveYStars();

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


        GRRLIB_ObjectView(0,0,0,0,0,0,1.0f,1.0f,1.0f);
        GRRLIB_SetTexture(tex_sky, FALSE);
        GX_Begin(GX_QUADS, GX_VTXFMT0, 24);

        drawSky(playerX,playerY,playerZ);

        GX_End();


        GRRLIB_DrawImg(-10, -10, tex_cross, 0, 1, 1, 0xFFFFFFFF);  // Draw a picture


        GRRLIB_Render();  // Render the frame buffer to the TV

    }
    GRRLIB_FreeTexture(tex_boxes);
    GRRLIB_FreeTexture(tex_boxes_invert);
    GRRLIB_FreeTexture(tex_font);
    GRRLIB_FreeTexture(tex_ystar);
    GRRLIB_FreeTexture(tex_shot);
    GRRLIB_FreeTexture(tex_sky);
    GRRLIB_Exit(); // clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
}
