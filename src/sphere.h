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
        
        spdlog::trace("Sphere - Hit Check:");
        spdlog::trace(" - center: {}", center.to_string());
        spdlog::trace(" - radius: {}", radius);
        spdlog::trace(" - a: {}", a);
        spdlog::trace(" - h: {}", h);
        spdlog::trace(" - c: {}", c);
        spdlog::trace(" - discriminant: {}", discriminant);
        spdlog::trace(" - r: {}", r.to_string());

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
        
        spdlog::trace("Hit Detected");
        spdlog::trace(" - sqrtd: {}", sqrtd);
        spdlog::trace(" - root: {}", root);
        spdlog::trace(" - rec.t: {}", rec.t);
        spdlog::trace(" - rec.p: {}", rec.p.to_string());
        spdlog::trace(" - outward_normal: {}", outward_normal.to_string());
        spdlog::trace(" - rec.normal: {}", rec.normal.to_string());
        spdlog::trace(" - rec.front_face: {}", rec.front_face);

        return true;
    }

private:
    Point3 center;
    double radius;
};