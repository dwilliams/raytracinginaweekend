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
        //return Color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
        //return Color(1, 1, 1) * noise.turb(p, 7);
        return Color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
    }

private:
    Perlin noise;
    double scale;
};
