#include "Beam.h"

Beam::Beam(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc) {
    x = fromX;
    y = fromY;
    z = fromZ;
    this->xAcc = xAcc;
    this->yAcc = yAcc;
    this->zAcc = zAcc;
    timer = 0;
}
