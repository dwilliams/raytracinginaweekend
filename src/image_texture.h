#pragma once

#include "color.h"
#include "texture.h"
#include "rtw_stb_image.h"

class ImageTexture : public Texture {
public:
    ImageTexture(const char* filename) : image(filename) {}

    Color value(double u, double v, const Point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) {
            return Color(0, 1, 1);
        }

        // Clamp input texture coordinates to [0, 1] x [1, 0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v); // Flip V to image coordinates

        int i = int(u * image.width());
        int j = int(v * image.height());
        const unsigned char* pixel = image.pixel_data(i, j);

        double color_scale = 1.0 / 255.0;
        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    RtwImage image;

};
