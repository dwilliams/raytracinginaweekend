#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include "ray.h"
#include "vec3.h"

class Material;

// FIXME: Should this be in its own header file?
class HitRecord {
public:
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        // Sets the hit record normal vector
        // NOTE: the parameter 'outward_normal' is assumed to have unit length

        //front_face = dot(r.direction(), outward_normal) < 0;
        //normal = front_face ? outward_normal : -outward_normal;
        double d = dot(r.direction(), outward_normal);
        if (d > 0.0) {
            // ray is inside the sphere
            normal = -outward_normal;
            front_face = false;
        } else {
            // ray is outside the sphere
            normal = outward_normal;
            front_face = true;
        }
        spdlog::trace("set_face_normal");
        spdlog::trace(" - r.direction().length(): {}", r.direction().length());
        spdlog::trace(" - outward_normal.length(): {}", outward_normal.length());
        spdlog::trace(" - d: {}", d);
    }
};
