#pragma once

#include "vec3.h"

#include <fmt/format.h>

class Ray {
public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction), tm(0) {}
    Ray(const Point3& origin, const Vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}

    const Point3& origin() const { return orig; }
    const Vec3& direction() const { return dir; }
    double time() const { return tm; }

    Point3 at(double t) const {
        return orig + (t * dir);
    }
    
    std::string to_string() const {
        return fmt::format("Ray{{ orig: {}, dir: {} }}", orig.to_string(), dir.to_string());
    }

private:
    Point3 orig;
    Vec3 dir;
    double tm;
};
