#pragma once

#include <memory>

#include <spdlog/spdlog.h>

#include "aabb.h"
#include "hitrecord.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    // Stationary Sphere
    Sphere(const Point3& static_center, double radius, std::shared_ptr<Material> material)
    : center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(material) {
        Vec3 rvec = Vec3(radius, radius, radius);
        bbox = AABB(static_center - rvec, static_center + rvec);
    }

    // Moving Sphere
    Sphere(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> material)
    : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(material) {
        Vec3 rvec = Vec3(radius, radius, radius);
        AABB box1(center.at(0) - rvec, center.at(0) + rvec);
        AABB box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = AABB(box1, box2);
    }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        Point3 current_center = center.at(r.time());
        Vec3 oc = current_center - r.origin();
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
        if (! ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (! ray_t.surrounds(root)) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;
        
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

    AABB bounding_box() const override { return bbox; }

private:
    Ray center;
    double radius;
    std::shared_ptr<Material> mat;
    AABB bbox;

    static void get_sphere_uv(const Point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        double theta = std::acos(-p.y());
        double phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};