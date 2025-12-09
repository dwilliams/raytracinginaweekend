#pragma once

#include "aabb.h"
#include "interval.h"
#include "hitrecord.h"
#include "ray.h"

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;

    virtual AABB bounding_box() const = 0;
};
