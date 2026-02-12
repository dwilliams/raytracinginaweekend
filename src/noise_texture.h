#pragma once

#include "color.h"
#include "perlin.h"
#include "texture.h"
#include "vec3.h"

class NoiseTexture : public Texture {
public:
    NoiseTexture() : scale(1.0) {}

    NoiseTexture(double scale) : scale(scale) {}

    Color value(double u, double v, const Point3& p) const override {
        return Color(1, 1, 1) * noise.noise(scale * p);
    }

private:
    Perlin noise;
    double scale;
};
