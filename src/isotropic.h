#pragma once

#include "color.h"
#include "hit_record.h"
#include "material.h"
#include "ray.h"
#include "solid_color_texture.h"
#include "texture.h"

class Isotropic : public Material {
public:
    Isotropic(const Color & albedo) : tex(std::make_shared<SolidColorTexture>(albedo)) {}

    Isotropic(std::shared_ptr<Texture> tex) : tex(tex) {}

    bool scatter(const Ray & r_in, const HitRecord & rec, Color & attenuation, Ray & scattered) const override {
        scattered = Ray(rec.p, random_unit_vector(), r_in.time());
        attenuation = tex->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    std::shared_ptr<Texture> tex;
};
