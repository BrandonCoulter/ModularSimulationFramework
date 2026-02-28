#pragma once

// Quaternion class definition
#include <cmath>
#include <iostream>
#include "Vec3.hpp"

class Quat {
public:
    // Default constructor (identity quaternion)
    Quat() : w(1), x(0), y(0), z(0) {}
    // Parameterized constructor
    Quat(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    // Quaternion common operations
    // Quaternion addition
    Quat operator+(const Quat& other) const {
        return Quat(w + other.w, x + other.x, y + other.y, z + other.z);
    }
    // Quaternion subtraction
    Quat operator-(const Quat& other) const {
        return Quat(w - other.w, x - other.x, y - other.y, z - other.z);
    }
    // Quaternion Magnitude (also known as norm)
    double magnitude() const {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }
    // Normalize the quaternion
    void normalize() {
        double mag = magnitude();
        if (mag == 0) {
            w = 1; x = 0; y = 0; z = 0; // Reset to identity if magnitude is zero
            return;
        }   
        w /= mag; x /= mag; y /= mag; z /= mag;
    }
    // Conjugate of the quaternion
    Quat conjugate() const {
        return Quat(w, -x, -y, -z);
    }
    // Non-commutative quaternion multiplication (this * other) (Hamilton product)
    Quat operator*(const Quat& other) const {
        return Quat(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w
        );
    }
    // Quaternion scalar multiplication
    Quat operator*(double scalar) const {
        return Quat(w * scalar, x * scalar, y * scalar, z * scalar);
    }
    // Quaternion Inversion
    Quat inverse() const {
        double mag_squared = w * w + x * x + y * y + z * z;
        return Quat(w / mag_squared, -x / mag_squared, -y / mag_squared, -z / mag_squared);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Quat& q) {
        os << "Quat(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
        return os;
    }

    // Quaternion interpolation (slerp)
    static Quat slerp(const Quat& q1, const Quat& q2, double t) {
        // Compute the cosine of the angle between the two quaternions
        double cos_half_theta = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
        // If q1 and q2 are the same, return q1
        if (std::abs(cos_half_theta) >= 1.0) {
            return q1;
        }
        // Calculate temporary values
        double half_theta = std::acos(cos_half_theta);
        double sin_half_theta = std::sqrt(1.0 - cos_half_theta * cos_half_theta);
        // If the angle is small, use linear interpolation        
        if (std::abs(sin_half_theta) < 0.001) {
            return Quat(
                q1.w * 0.5 + q2.w * 0.5,
                q1.x * 0.5 + q2.x * 0.5,
                q1.y * 0.5 + q2.y * 0.5,
                q1.z * 0.5 + q2.z * 0.5
            );
        }
        double ratio_a = std::sin((1 - t) * half_theta) / sin_half_theta;
        double ratio_b = std::sin(t * half_theta) / sin_half_theta;
        return Quat(
            q1.w * ratio_a + q2.w * ratio_b,
            q1.x * ratio_a + q2.x * ratio_b,
            q1.y * ratio_a + q2.y * ratio_b,
            q1.z * ratio_a + q2.z * ratio_b
        );
    }

    // Rotate a vector by this quaternion
    Vec3 rotate(const Vec3& v) const {
        // get the x, y, z components of the vector
        double vx = v.get_x();
        double vy = v.get_y();
        double vz = v.get_z();
        Quat v_quat(0, vx, vy, vz);
        Quat result = (*this) * v_quat * this->conjugate();
        return Vec3(result.x, result.y, result.z);
    }
    // Print the quaternion (for debugging)
    void print() const {
        std::cout << "Quat(" << w << ", " << x << ", " << y << ", " << z << ")" << std::endl;
    }
private:

    // w is the scalar part, x, y, z are the vector part of the quaternion
    double w{1.0}, x{0.0}, y{0.0}, z{0.0};
};  