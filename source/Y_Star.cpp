#include "Y_Star.h"
#include <math.h>

Y_Star::Y_Star(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc){
    x = fromX;
    y = fromY;
    z = fromZ;

    blockSize = 37 + (rand() % 5);

    this->xAcc = xAcc;
    this->yAcc = yAcc;
    this->zAcc = zAcc;

    float magnitude = sqrt(xAcc * xAcc + yAcc * yAcc + zAcc * zAcc);

    if (magnitude > 0.00001f) {
            xAcc /= magnitude;
            yAcc /= magnitude;
            zAcc /= magnitude;
    } else {
            // Handle the zero vector case (often just leave it as is)
            xAcc = 0.0f;
            yAcc = 0.0f;
            zAcc = 0.0f;
    }

}
