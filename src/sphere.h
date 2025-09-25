#pragma once

#include <spdlog/spdlog.h>

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
        Vec3 oc = center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - (radius * radius);
        double discriminant = (h * h) - (a * c);
        
        spdlog::debug("Sphere - Hit Check:");
        spdlog::debug(" - center: {}", center.to_string());
        spdlog::debug(" - radius: {}", radius);
        spdlog::debug(" - a: {}", a);
        spdlog::debug(" - h: {}", h);
        spdlog::debug(" - c: {}", c);
        spdlog::debug(" - discriminant: {}", discriminant);
        spdlog::debug(" - r: {}", r.to_string());

        if (discriminant < 0) {
            return false;
        }

        double sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies on the acceptable range
        double root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        
        spdlog::debug("Hit Detected");
        spdlog::debug(" - sqrtd: {}", sqrtd);
        spdlog::debug(" - root: {}", root);
        spdlog::debug(" - rec.t: {}", rec.t);
        spdlog::debug(" - rec.p: {}", rec.p.to_string());
        spdlog::debug(" - outward_normal: {}", outward_normal.to_string());
        spdlog::debug(" - rec.normal: {}", rec.normal.to_string());
        spdlog::debug(" - rec.front_face: {}", rec.front_face);

        return true;
    }

private:
    Point3 center;
    double radius;
};