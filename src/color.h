#pragma once

#include <iostream>

#include "interval.h"
#include "vec3.h"

using Color = Vec3;

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color[0];
    auto g = pixel_color[1];
    auto b = pixel_color[2];

    // Translate the [0, 1] component values to the byte range [0, 255].
    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << " " << gbyte << " " << bbyte << "\n";
}
