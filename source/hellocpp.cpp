#include <vector>
#include <grrlib.h>
#include <string>
#include <cctype>    // Required for ::toupper
#include <math.h>

#include "hellocpp.h"
#include "Beam.h"
#include "Y_Star.h"
#include "Shot.h"

const float BEAM_SIZE_SHOTS = 4.20f;


static std::vector<Y_Star> yStars;
void loadYstars(){
    srand(time(NULL));
    for(int i = 0; i < 50;i++){
        yStars.push_back(Y_Star(rand() % 2000 - 1000,rand() % 2000 - 1000,rand() % 2000 - 1000));
    }
}

int addNumbers(int a, int b) {
    return (a + b); // Returns the sum of a and b
}

static std::string myString = "DEFAULT STRING";
static char buffer[100];

void to_uppercase_transform(std::string &inputString) {
    for (char &c : inputString) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
}

char *get_static_string() {
    //myString = nextWord();
    myString = "test 31";
    to_uppercase_transform(myString);
    snprintf(buffer, sizeof(buffer), "%s", myString.c_str());
    return buffer;
}

static std::vector<Beam> beams;
long long reloadingSpeed = 142;
long long reloadedTime = 0;
void beamBlocks(float fromX, float fromY, float fromZ, long long currentTime){
    if(reloadedTime <= currentTime){
        beams.push_back(Beam(fromX,fromY,fromZ,0,1,0));
        beams.push_back(Beam(fromX,fromY,fromZ,1,0,0));
        beams.push_back(Beam(fromX,fromY,fromZ,0,-1,0));
        beams.push_back(Beam(fromX,fromY,fromZ,-1,0,0));
        beams.push_back(Beam(fromX,fromY,fromZ,0,0,1));
        beams.push_back(Beam(fromX,fromY,fromZ,0,0,-1));
        reloadedTime = currentTime + reloadingSpeed;
    }
}
static std::vector<Beam> specialBeams;
static std::vector<Shot> shots;
int shotReloadingSpeed = 200;
int bulletLifetime = 2000;
long long shotReloadingTimer = 0;
void shoot(long long currentTime,float fromX,float fromY,float fromZ,float targetX,float targetY,float targetZ){
    if(currentTime >= shotReloadingTimer){
        shots.push_back(Shot(fromX,fromY,fromZ,targetX,targetY,targetZ,currentTime + bulletLifetime));
        shotReloadingTimer = currentTime + shotReloadingSpeed;
    }
}

double calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));
}

void moveStarsBeamsSpecialEtc(float moveX, float moveY, float moveZ){
    for(Block& block:yStars){
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
    }
    for(Block& block:beams){
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
    }
    for(Block& block:specialBeams){
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
    }
    for(Block& block:shots){
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
    }
    loopAroundOutOfBoundsWorldObjects();
}
void loopAroundOutOfBoundsWorldObjects(){
   for(Block& block:yStars){
        if(block.x > 1000){
            block.x -= 2000;
        }
        if(block.x < -1000){
            block.x += 2000;
        }
        if(block.y > 1000){
            block.y -= 2000;
        }
        if(block.y < -1000){
            block.y += 2000;
        }
        if(block.z > 1000){
            block.z -= 2000;
        }
        if(block.z < -1000){
            block.z += 2000;
        }
    }
    for(Block& block:beams){
        if(block.x > 1000){
            block.x -= 2000;
        }
        if(block.x < -1000){
            block.x += 2000;
        }
        if(block.y > 1000){
            block.y -= 2000;
        }
        if(block.y < -1000){
            block.y += 2000;
        }
        if(block.z > 1000){
            block.z -= 2000;
        }
        if(block.z < -1000){
            block.z += 2000;
        }
    }
    for(Block& block:specialBeams){
        if(block.x > 1000){
            block.x -= 2000;
        }
        if(block.x < -1000){
            block.x += 2000;
        }
        if(block.y > 1000){
            block.y -= 2000;
        }
        if(block.y < -1000){
            block.y += 2000;
        }
        if(block.z > 1000){
            block.z -= 2000;
        }
        if(block.z < -1000){
            block.z += 2000;
        }
    }
    for(Block& block:shots){
        if(block.x > 1000){
            block.x -= 2000;
        }
        if(block.x < -1000){
            block.x += 2000;
        }
        if(block.y > 1000){
            block.y -= 2000;
        }
        if(block.y < -1000){
            block.y += 2000;
        }
        if(block.z > 1000){
            block.z -= 2000;
        }
        if(block.z < -1000){
            block.z += 2000;
        }
    }

}
float shotSpeed = 1.2f;
int loadShots(long long currentTime, int timePassed){
    std::vector<Shot> newShotsList;
    long speed = shotSpeed * (float)timePassed;
    for(Shot& shot:shots){
        if(currentTime >= shot.endTime ){
            continue;
        }
        shot.x = shot.x + (shot.xAcc * speed);
        shot.y = shot.y + (shot.yAcc * speed);
        shot.z = shot.z + (shot.zAcc * speed);
        for(int i = 0;i < yStars.size();i++){
            Y_Star& yStar = yStars[i];
            if(calculateDistance(shot.x,shot.y,shot.z,yStar.x,yStar.y,yStar.z) < sqrt(yStar.blockSize * yStar.blockSize) + sqrt(BEAM_SIZE_SHOTS * BEAM_SIZE_SHOTS)){
                //yStar.blockSize *= 1.42;
                //if(yStar.blockSize > 50.0f){
                    //draw explosion
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,0,1,0));
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,1,0,0));
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,0,-1,0));
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,-1,0,0));
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,0,0,1));
                    beams.push_back(Beam(yStar.x,yStar.y,yStar.z,0,0,-1));
                    //move the ystar and reset its size
                    yStar.x = rand() % 2000 - 1000;
                    yStar.y = rand() % 2000 - 1000;
                    yStar.z = rand() % 2000 - 1000;
                    yStar.blockSize = 20.0f;
                    goto continueOuterLoop0;

                //}
            }
        }
        newShotsList.push_back(shot);
        continueOuterLoop0:
    }
    shots = newShotsList;
    return shots.size();
}
float beamSpeed = .42f;
int loadBeams(int timePassed){
   std::vector<Beam> newBeamsList;
    long speed = beamSpeed * ((float)timePassed);
    for(Beam& beam:beams){
        beam.timer += timePassed;
        if(beam.timer > 500){
            float fromX = beam.x;
            float fromY = beam.y;
            float fromZ = beam.z;
            specialBeams.push_back(Beam(fromX,fromY,fromZ,0,1,0));
            specialBeams.push_back(Beam(fromX,fromY,fromZ,1,0,0));
            specialBeams.push_back(Beam(fromX,fromY,fromZ,0,-1,0));
            specialBeams.push_back(Beam(fromX,fromY,fromZ,-1,0,0));
            specialBeams.push_back(Beam(fromX,fromY,fromZ,0,0,1));
            specialBeams.push_back(Beam(fromX,fromY,fromZ,0,0,-1));
            continue;
        }
        beam.x = beam.x + (beam.xAcc * speed);
        beam.y = beam.y + (beam.yAcc * speed);
        beam.z = beam.z + (beam.zAcc * speed);
        newBeamsList.push_back(beam);
    }
    beams = newBeamsList;
    return beams.size();
}
int loadBeamsSpecial(int timePassed){
    long speed = beamSpeed * ((float)timePassed);
    std::vector<Beam> newBeamsListSpecial;
    for(Beam& beam:specialBeams){
        beam.timer += timePassed;
        if(beam.timer > 500){
            continue;
        }
        beam.x = beam.x + (beam.xAcc * speed);
        beam.y = beam.y + (beam.yAcc * speed);
        beam.z = beam.z + (beam.zAcc * speed);
        newBeamsListSpecial.push_back(beam);
    }
    specialBeams = newBeamsListSpecial;
    return specialBeams.size();
}
const float BEAM_SIZE = 9.10f;
void drawBeams(){
    for(Beam& beam:beams){
        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,-BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE + beam.x,-BEAM_SIZE + beam.y,BEAM_SIZE + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);
    }
}
const float BEAM_SIZE_SPECIAL = 7.10f;
void drawBeamsSpecial(){
    for(Beam& beam:specialBeams){
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,-BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SPECIAL + beam.x,-BEAM_SIZE_SPECIAL + beam.y,BEAM_SIZE_SPECIAL + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);
    }
}
void drawY_Stars(){
    for(Y_Star& beam:yStars){
        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-beam.blockSize + beam.x,beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,-beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(beam.blockSize + beam.x,-beam.blockSize + beam.y,beam.blockSize + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);
    }
}
void drawShots(){
    for(Block& beam:shots){
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,-BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_SHOTS + beam.x,-BEAM_SIZE_SHOTS + beam.y,BEAM_SIZE_SHOTS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);
    }
}
const float SKYBOX_SIZE = 1000;//change later to big number like 1000
void drawSky(float x, float y, float z){
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z); //OO
    GX_Color1u32(0xFFFFFFFF);                                           //XO
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);  //OO
    GX_Color1u32(0xFFFFFFFF);                                           //OX
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);   //OX
    GX_Color1u32(0xFFFFFFFF);                                           //OO
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);  //XO
    GX_Color1u32(0xFFFFFFFF);                                           //OO
    GX_TexCoord2f32(0.0f,0.0f);



    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);



    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);



    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);



    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);



    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);



/*
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);  //XO
    GX_Color1u32(0xFFFFFFFF);                                           //OO
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);   //OX
    GX_Color1u32(0xFFFFFFFF);                                           //OO
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);  //OO
    GX_Color1u32(0xFFFFFFFF);                                           //OX
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z); //OO
    GX_Color1u32(0xFFFFFFFF);                                           //XO
    GX_TexCoord2f32(0.0f,1.0f);

    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);

    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);

    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);

    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);

    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,-SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,0.0f);
    GX_Position3f32(-SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(1.0f,1.0f);
    GX_Position3f32(SKYBOX_SIZE + x,-SKYBOX_SIZE + y,SKYBOX_SIZE + z);
    GX_Color1u32(0xFFFFFFFF);
    GX_TexCoord2f32(0.0f,1.0f);
    */
}
