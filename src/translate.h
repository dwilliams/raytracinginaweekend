#pragma once

#include "aabb.h"
#include "interval.h"
#include "hitrecord.h"
#include "hittable.h"
#include "ray.h"

class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable> object, const Vec3& offset)
    : object(object), offset(offset) {
        bbox = object->bounding_box() + offset;
    }

    bool hit (const Ray& r, Interval ray_t, HitRecord& rec) const override {
        // Move the ray backwards byt he offset
        Ray offset_r(r.origin() - offset, r.direction(), r.time());

        // Determine whether an intersection exists along the offset ray (and if so, where)
        if (!object->hit(offset_r, ray_t, rec)) {
            return false;
        }

        // Move the intersection point forwards by the offset
        rec.p += offset;

        return true;
    }

    AABB bounding_box() const override {
        return bbox;
    }

private:
    std::shared_ptr<Hittable> object;
    Vec3 offset;
    AABB bbox;
};
