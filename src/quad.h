#pragma once

#include "aabb.h"
#include "hittable.h"
#include "hitrecord.h"
#include "interval.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"

class Quad : public Hittable {
public:
    Quad(const Point3& Q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
        Vec3 n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    virtual void set_bounding_box() {
        // Compute the bounding box of all four vertices.
        AABB bbox_diagonal1 = AABB(Q, Q + u + v);
        AABB bbox_diagonal2 = AABB(Q + u, Q + v);
        bbox = AABB(bbox_diagonal1, bbox_diagonal2);
    }

    AABB bounding_box() const override { return bbox; }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
        double denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < less_zeroish) {
            return false;
        }

        // Return false if the hit point parameter t is outside the ray interval.
        double t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t)) {
            return false;
        }

        // Determine if the hit point lies within the planar shape using its planar coordinates.
        Point3 intersection = r.at(t);
        Vec3 planar_hitpt_vector = intersection - Q;
        double alpha = dot(w, cross(planar_hitpt_vector, v));
        double beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec)) {
            return false;
        }

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }
                                                                                            
    virtual bool is_interior(double a, double b, HitRecord& rec) const {
        Interval unit_interval = Interval(0, 1);
        // Given the hit point in planar coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unit_interval.contains(a) || !unit_interval.contains(b)) {
            return false;
        }

        rec.u = a;
        rec.v = b;
        return true;
    }

private:
    Point3 Q;
    Vec3 u;
    Vec3 v;
    Vec3 w;
    std::shared_ptr<Material> mat;
    AABB bbox;
    Vec3 normal;
    double D;
};
