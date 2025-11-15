#include "Y_Star.h"
#include <math.h>


struct Vector3 {
    float x, y, z;
};

struct Quaternion {
    float w, x, y, z;

    // Default constructor (identity quaternion, no rotation)
    Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

    // Constructor from components
    Quaternion(float w_val, float x_val, float y_val, float z_val)
        : w(w_val), x(x_val), y(y_val), z(z_val) {}

    // Constructor from axis and angle (in radians)
    Quaternion(const Vector3& axis, float angle_rad) {
        float half_angle = angle_rad / 2.0f;
        float s = std::sin(half_angle);
        w = std::cos(half_angle);
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        normalize();
    }

    // Normalize the quaternion to ensure it represents a valid rotation (unit quaternion)
    void normalize() {
        float magnitude = std::sqrt(w * w + x * x + y * y + z * z);
        if (magnitude > 0.0f) {
            w /= magnitude;
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }
    }

    // Quaternion multiplication (non-commutative)
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z, // w
            w * other.x + x * other.w + y * other.z - z * other.y, // x
            w * other.y - x * other.z + y * other.w + z * other.x, // y
            w * other.z + x * other.y - y * other.x + z * other.w  // z
        );
    }

    // Calculate the conjugate (useful for rotation)
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    // Rotate a 3D vector using this quaternion
    Vector3 rotate(const Vector3& v) const {
        // Convert the 3D vector to a pure quaternion (w=0)
        Quaternion p(0.0f, v.x, v.y, v.z);

        // Rotation formula: q * p * q_conjugate
        Quaternion rotated_p = (*this) * p * this->conjugate();

        // The result's x, y, z components form the rotated vector
        return {rotated_p.x, rotated_p.y, rotated_p.z};
    }
};

void Y_Star::normalizeAcc(){
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


Y_Star::Y_Star(float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc){
    x = fromX;
    y = fromY;
    z = fromZ;

    blockSize = 37 + (rand() % 5);

    this->xAcc = xAcc;
    this->yAcc = yAcc;
    this->zAcc = zAcc;

    normalizeAcc();


}

void Y_Star::changeDirection(){
    float roll = ((rand() % 420) - 210) / 210;
    float pitch = ((rand() % 420) - 210) / 210;
    float yaw = ((rand() % 420) - 210) / 210;
    Vector3 axis_of_rotation_roll = {1.0f, 0.0f, 0.0f};
    Vector3 axis_of_rotation_pitch = {0.0f, 1.0f, 0.0f};
    Vector3 axis_of_rotation_yaw = {0.0f, 0.0f, 1.0f};
    Quaternion rotation_quatX(axis_of_rotation_roll, roll);
    Quaternion rotation_quatY(axis_of_rotation_pitch, pitch);
    Quaternion rotation_quatZ(axis_of_rotation_yaw, yaw);



    Quaternion rotation_quat = rotation_quatX * rotation_quatY;
    rotation_quat = rotation_quat * rotation_quatZ;

    Vector3 original_vector = {xAcc, yAcc, zAcc};
    Vector3 rotated_vector = rotation_quat.rotate(original_vector);
    xAcc = rotated_vector.x;
    yAcc = rotated_vector.y;
    zAcc = rotated_vector.z;
    normalizeAcc();
}
