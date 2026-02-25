#pragma once

// 3D Vector class definition
#include <cmath>
#include <iostream>

class Vec3 {
public:
    // Default constructor
    Vec3() = default;
    // Parameterized constructor
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    // Basic vector operations
    // Vector addition
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    // Vector subtraction
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    // Scalar multiplication
    Vec3 operator*(double scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    // Scalar division
    Vec3 operator/(double scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Compound assignment operators
    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
    Vec3& operator*=(double scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }
    Vec3& operator/=(double scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // Dot product
    double dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    // Cross product
    Vec3 cross(const Vec3& other) const {
        return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    
    // Magnitude of the vector (also known as length)
    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    // Normalize the vector
    Vec3 normalize() const {
        double mag = magnitude();
        if (mag > 0) {
            return Vec3(x / mag, y / mag, z / mag);
        }
        return Vec3(0.0, 0.0, 0.0); // Return zero vector if magnitude is zero
    }

    // Get the x, y, z components of the vector
    double get_x() const { return x; }
    double get_y() const { return y; }
    double get_z() const { return z; }  

    // Print the vector (for debugging)
    void print() const {
        std::cout << "Vec3(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

private:
    double x{0.0}, y{0.0}, z{0.0};
};