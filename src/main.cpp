#include <fstream>
#include <iostream>
#include <memory>

#include <spdlog/spdlog.h>

#include "rtweekend.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Color ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + Color(1, 1, 1));
    }

    // Background
    Vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return ((1.0 - a) * Color(1.0, 1.0, 1.0)) + (a * Color(0.5, 0.7, 1.0));
}

int main(void) {
    // Setup Logging
    spdlog::set_level(spdlog::level::debug);

    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 256;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World
    HittableList world;

    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ( double(image_width) / image_height);
    Point3 camera_center = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    Vec3 viewport_u = Vec3(viewport_width, 0, 0);
    Vec3 viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    Vec3 pixel_delta_u = viewport_u / image_width;
    Vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    Point3 viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    Point3 pixel_zero_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));

    // Render
    // Going to output directly to a file so a logger can be used.
    std::ofstream image_file;
    image_file.open("image.ppm");

    image_file << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        spdlog::info("Scanlines Remaining: {}", image_height - j);
        for (int i = 0; i < image_width; i++) {
            Point3 pixel_center = pixel_zero_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            Vec3 ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            Color pixel_color = ray_color(r, world);
            write_color(image_file, pixel_color);
        }
    }
    
    image_file.close();

    spdlog::info("Done");

    return 0;
}
