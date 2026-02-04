#pragma once

#include <memory>

#include "color.h"
#include "hitrecord.h"
#include "material.h"
#include "solid_color_texture.h"
#include "texture.h"
#include "ray.h"
#include "vec3.h"

class Lambertian : public Material {
public:
    Lambertian(const Color& albedo) : tex(std::make_shared<SolidColorTexture>(albedo)) {}

    Lambertian(std::shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 scatter_direction = rec.normal + random_unit_vector();

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = Ray(rec.p, scatter_direction, r_in.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    std::shared_ptr<Texture> tex;
};
