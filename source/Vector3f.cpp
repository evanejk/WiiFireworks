#include "Vector3f.h"
#include <math.h>

Vector3f::Vector3f(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3f::normalize(){
    float magnitude = sqrt(x * x + y * y + z * z);

    if (magnitude > 0.00001f) {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
    } else {
            // Handle the zero vector case (often just leave it as is)
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
    }
}
