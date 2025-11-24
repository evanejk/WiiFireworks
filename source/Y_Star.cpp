#include "Y_Star.h"
#include <math.h>


struct Vector3 {
    float x, y, z;
};
float magnitude(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
void normalize(Vector3& v) {
    float mag = magnitude(v);

    // Guard against division by zero (for the zero vector)
    if (mag > 0.00001f) {
        v.x /= mag;
        v.y /= mag;
        v.z /= mag;
    }
    // If mag is zero, the vector remains {0, 0, 0}
}
float dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vector3 cross(const Vector3& v1, const Vector3& v2) {
    return {
        v1.y * v2.z - v1.z * v2.y, // new x
        v1.z * v2.x - v1.x * v2.z, // new y
        v1.x * v2.y - v1.y * v2.x  // new z
    };
}
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


Y_Star::Y_Star(int level, float fromX, float fromY, float fromZ, float xAcc, float yAcc, float zAcc){
    x = fromX;
    y = fromY;
    z = fromZ;

    blockSize = 37 + (rand() % 5);

    this->xAcc = xAcc;
    this->yAcc = yAcc;
    this->zAcc = zAcc;

    this->level = level;

    normalizeAcc();

    reloadedTime = 0;
}
const float MAX_TURN_ANGLE = 0.055f; // Approximately 2.86 degrees, adjust as needed.
const float MAX_WOBBLE_ANGLE = 0.04f; // Very small rotation, adjust as needed.
float random_float() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

// Function to get a random float in a specified range [-min_val, +min_val]
float random_range(float range) {
    // Generates a float between -range and +range
    return (2.0f * random_float() - 1.0f) * range;
}
void Y_Star::changeDirection(float playerX, float playerY, float playerZ){
    Vector3 vectorToTarget = {playerX - x,playerY - y,playerZ - z};
    normalize(vectorToTarget);
    Vector3 original_vector = {xAcc, yAcc, zAcc};
    //fill in code here to rotate original_vector towards vectorToTarget
    // a. Calculate the rotation axis (Cross Product)
    Vector3 rotationAxis = cross(original_vector, vectorToTarget);
    normalize(rotationAxis); // Normalize the axis
    // b. Calculate the angle (Dot Product)
    float cosTheta = dot(original_vector, vectorToTarget);
    cosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));

    float angle_rad = std::acos(cosTheta);
    // Determine the actual angle to turn in this step
    float turn_angle = angle_rad;
    if (turn_angle > MAX_TURN_ANGLE) {
        // If the full rotation needed is greater than our limit,
        // we only rotate by the limit (MAX_TURN_ANGLE).
        turn_angle = MAX_TURN_ANGLE;
    }
    // c. Create the quaternion
    Quaternion rotation = Quaternion(rotationAxis, turn_angle);

    // --- NEW: Add Random Rotation ---

    // Create a random rotation quaternion (Wobble)
    // We choose a random axis (unit vector) and a small random angle.
    Vector3 wobble_axis = {random_range(1.0f), random_range(1.0f), random_range(1.0f)};
    normalize(wobble_axis);

    float wobble_angle = random_range(MAX_WOBBLE_ANGLE);
    Quaternion wobble_rotation = Quaternion(wobble_axis, wobble_angle);


    // f. Combine the rotations
    // Order matters! (Wobble * Controlled) means:
    // 1. First rotate towards the player (Controlled_rotation)
    // 2. Then apply the slight wobble relative to that new orientation (Wobble_rotation)
    // Use the multiplication operator: q = q1 * q2
    Quaternion total_rotation = wobble_rotation * rotation;


    // d. Apply the rotation to the Y_Star's acceleration/direction
    Vector3 new_direction = total_rotation.rotate(original_vector);

    // 4. Update the Y_Star's direction
    // This assumes the acceleration vectors (xAcc, yAcc, zAcc) ARE the forward direction vector.
    this->xAcc = new_direction.x;
    this->yAcc = new_direction.y;
    this->zAcc = new_direction.z;

    // You must also normalizeAcc() again if the magnitude of the rotation is not exactly 1 (it should be, but safety first)
    // The normalizeAcc() function you provided already exists and handles this.
    normalizeAcc();
}
