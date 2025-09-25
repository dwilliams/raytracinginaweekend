#pragma once

#include <fstream>
#include <string>

#include <spdlog/spdlog.h>

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:
    double aspect_ratio = 1.0; // Ratio of image width over height
    int image_width = 100; // Rendered image width in pixel count
    std::string image_filename = "image.ppm";  // Filename of the output image.

    void render(const Hittable& world) {
        initialize();

        // Going to output directly to a file so a logger can be used.
        std::ofstream image_file;
        image_file.open(image_filename);

        image_file << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            spdlog::info("Scanlines Remaining: {}", image_height - j);
            for (int i = 0; i < image_width; i++) {
                Point3 pixel_center = pixel_zero_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                Vec3 ray_direction = pixel_center - center;
                Ray r(center, ray_direction);

                Color pixel_color = ray_color(r, world);
                write_color(image_file, pixel_color);
            }
        }
        
        image_file.close();

        spdlog::info("Done");
    }

private:
    int image_height; // Rendered image height
    Point3 center; // Camera center
    Point3 pixel_zero_loc; // Location on pixel 0, 0
    Vec3 pixel_delta_u; // Offset to pixel to the right
    Vec3 pixel_delta_v; // Offset to pixel below

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = Point3(0, 0, 0);

        // Determine viewport dimensions
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        Vec3 viewport_u = Vec3(viewport_width, 0, 0);
        Vec3 viewport_v = Vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel
        Point3 viewport_upper_left = center - Vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
        pixel_zero_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));
    }

    Color ray_color(const Ray& r, const Hittable& world) const {
        HitRecord rec;

        if (world.hit(r, Interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        Vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction[1] + 1.0);
        return ((1.0 - a) * Color(1.0, 1.0, 1.0)) + (a * Color(0.5, 0.7, 1.0));
    }
};
