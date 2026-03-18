#pragma once

#include "aabb.h"
#include "interval.h"
#include "hitrecord.h"
#include "hittable.h"
#include "ray.h"

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle)
    : object(object) {
        double radians = degrees_to_radians(angle);
        sin_theta = std::sin(radians);
        cos_theta = std::cos(radians);
        bbox = object->bounding_box();

        Point3 min(infinity, infinity, infinity);
        Point3 max(-infinity, -infinity, -infinity);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    double x = (i * bbox.x.max) + ((1 - i) * bbox.x.min);
                    double y = (j * bbox.y.max) + ((1 - j) * bbox.y.min);
                    double z = (k * bbox.z.max) + ((1 - k) * bbox.z.min);

                    double newx = (cos_theta * x) + (sin_theta * z);
                    double newz = (-1 * sin_theta * x) + (cos_theta * z);

                    Vec3 tester(newx, y, newz);

                    for (int c = 0; c < 3; c++) {
                        min[c] = std::fmin(min[c], tester[c]);
                        max[c] = std::fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = AABB(min, max);
    }

    bool hit (const Ray& r, Interval ray_t, HitRecord& rec) const override {
        // Transform the ray from world space to object space.
        Point3 origin = Point3(
            (cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
            r.origin().y(),
            (sin_theta * r.origin().x()) + (cos_theta * r.origin().z())
        );

        Vec3 direction = Vec3(
            (cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
            r.direction().y(),
            (sin_theta * r.direction().x()) + (cos_theta * r.direction().z())
        );

        Ray rotated_r(origin, direction, r.time());

        // Determin whether an intersection exists in object space (and if so, where).
        if (!object->hit(rotated_r, ray_t, rec)) {
            return false;
        }

        // Transform the intersection from object space back to world space.
        rec.p = Point3(
            (cos_theta * rec.p.x()) + (sin_theta * rec.p.z()),
            rec.p.y(),
            (-1 * sin_theta * rec.p.x()) + (cos_theta * rec.p.z())
        );

        rec.normal = Vec3(
            (cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
            rec.normal.y(),
            (-1 * sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z())
        );

        return true;
    }

    AABB bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    AABB bbox;
};
