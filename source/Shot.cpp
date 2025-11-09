#include "Shot.h"

Shot::Shot(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc, long long endTime) {
    x = fromX;
    y = fromY;
    z = fromZ;
    this->xAcc = xAcc;
    this->yAcc = yAcc;
    this->zAcc = zAcc;
    this->endTime = endTime;
}
