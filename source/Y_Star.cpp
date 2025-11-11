#include "Y_Star.h"
#include <math.h>

Y_Star::Y_Star(float fromX, float fromY, float fromZ){
    x = fromX;
    y = fromY;
    z = fromZ;

    blockSize = 10 + (rand() % 12);

}
