#pragma once

#include "color.h"
#include "hitrecord.h"
#include "material.h"
#include "ray.h"
#include "solid_color_texture.h"
#include "texture.h"

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> tex) : tex(tex) {}
    DiffuseLight(const Color& emit) : tex(std::make_shared<SolidColorTexture>(emit)) {}

    Color emitted(double u, double v, const Point3& p) const override {
        return tex->value(u, v, p);
    }

private:
    std::shared_ptr<Texture> tex;
};
