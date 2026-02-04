#pragma once

#include "color.h"
#include "texture.h"
#include "vec3.h"

class SolidColorTexture : public Texture {
public:
    SolidColorTexture(const Color& albedo) : albedo(albedo) {}
    SolidColorTexture(double red, double green, double blue) : SolidColorTexture(Color(red, green, blue)) {}

    Color value(double u, double v, const Point3& p) const override {
        return albedo;
    }

private:
    Color albedo;
};
