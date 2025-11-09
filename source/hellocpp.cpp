#include <vector>
#include <grrlib.h>
#include <string>
#include <cctype>    // Required for ::toupper
#include <math.h>

#include "hellocpp.h"
#include "Beam.h"
#include "Y_Star.h"

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
int reloadingSpeed = 142;
int reloadingTimer = reloadingSpeed;
void beamBlocks(float fromX, float fromY, float fromZ, int timePassed){
    reloadingTimer -= timePassed;
    if(reloadingTimer <= 0){
        beams.push_back(Beam(fromX,fromY,fromZ,0,1,0));
        beams.push_back(Beam(fromX,fromY,fromZ,1,0,0));
        beams.push_back(Beam(fromX,fromY,fromZ,0,-1,0));
        beams.push_back(Beam(fromX,fromY,fromZ,-1,0,0));
        beams.push_back(Beam(fromX,fromY,fromZ,0,0,1));
        beams.push_back(Beam(fromX,fromY,fromZ,0,0,-1));
        reloadingTimer = reloadingSpeed;
    }
}
static std::vector<Beam> specialBeams;
int specialReloadingSpeed = 3000;
int specialReloadingTimer = specialReloadingSpeed;
void special(int timePassed){
    specialReloadingTimer -= timePassed;
    if(specialReloadingTimer <= 0){
        for(Beam beam : beams){
            float fromX = beam.x;
            float fromY = beam.y;
            float fromZ = beam.z;
            beams.push_back(Beam(fromX,fromY,fromZ,0,1,0));
            beams.push_back(Beam(fromX,fromY,fromZ,1,0,0));
            beams.push_back(Beam(fromX,fromY,fromZ,0,-1,0));
            beams.push_back(Beam(fromX,fromY,fromZ,-1,0,0));
            beams.push_back(Beam(fromX,fromY,fromZ,0,0,1));
            beams.push_back(Beam(fromX,fromY,fromZ,0,0,-1));

        }
        specialReloadingTimer = specialReloadingSpeed;
    }
}

double calculateDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));
}

void moveStarsBeamsSpecial(float moveX, float moveY, float moveZ){
    for(Block& block:yStars){
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
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
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
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
        block.x += moveX;
        block.y += moveY;
        block.z += moveZ;
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

float beamSpeed = .42f;
int loadBeams(int timePassed){
   std::vector<Beam> newBeamsList;
    long speed = beamSpeed * ((float)timePassed);
    for(Beam& beam:beams){
        beam.timer += timePassed;
        if(beam.timer > 1000){
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
        if(beam.timer > 1000){
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
const float BEAM_SIZE = 4.20f;
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
const float BEAM_SIZE_SPECIAL = 1.20f;
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
const float BEAM_SIZE_Y_STARS = 9.10f;
void drawY_Stars(){
    for(Block& beam:yStars){
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);

        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,-BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,0.0f);
        GX_Position3f32(-BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f,1.0f);
        GX_Position3f32(BEAM_SIZE_Y_STARS + beam.x,-BEAM_SIZE_Y_STARS + beam.y,BEAM_SIZE_Y_STARS + beam.z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f,1.0f);
    }
}
