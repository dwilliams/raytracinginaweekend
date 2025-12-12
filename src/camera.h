#pragma once

#include <fstream>
#include <string>

#include <spdlog/spdlog.h>

#include "rtweekend.h"

#include "color.h"
#include "hitrecord.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class Camera {
public:
    double aspect_ratio = 1.0; // Ratio of image width over height
    int image_width = 100; // Rendered image width in pixel count
    int samples_per_pixel = 10; // Count of random samples for each pixel
    int max_depth = 10; // Maximum number of ray bounces into scene

    double vfov = 90; // Vertical view angle (field of view)
    Point3 lookfrom = Point3(0, 0, 0); // Point the camera is looking from (camera location)
    Point3 lookat = Point3(0, 0, -1); // Point the camera is looking at (camera direction)
    Vec3 vup = Vec3(0, 1, 0); // Camera-relative "up" direction (camera orientation)

    double defocus_angle = 0; // Variation angle of rays through each pixel
    double focus_distance = 10; // Distance from camera lookfrom point to plane of perfect focus

    std::string image_filename = "image.ppm";  // Filename of the output image.

    void render(const Hittable& world) {
        initialize();

        // Going to output directly to a file so a logger can be used.
        std::ofstream image_file;
        image_file.open(image_filename);

        image_file << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                spdlog::info("Scanlines Remaining: {}, Pixels Remaining: {}", image_height - j, image_width - i);
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(image_file, pixel_samples_scale * pixel_color);
            }
        }

        image_file.close();

        spdlog::info("Done");
    }

private:
    int image_height; // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum pf pixel samples
    Point3 center; // Camera center
    Point3 pixel_zero_loc; // Location on pixel 0, 0
    Vec3 pixel_delta_u; // Offset to pixel to the right
    Vec3 pixel_delta_v; // Offset to pixel below
    Vec3 u; // Camera from basis vectors
    Vec3 v;
    Vec3 w;
    Vec3 defocus_disk_u; // Defocus disk horizontal radius
    Vec3 defocus_disk_v; // Defocus disk vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);
        double viewport_height = 2 * h * focus_distance;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u, v, w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        Vec3 viewport_u = viewport_width * u; // Vector across viewport horizontal edge
        Vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel
        Point3 viewport_upper_left = center - (focus_distance * w) - (viewport_u / 2) - (viewport_v / 2);
        pixel_zero_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));
        
        // Calculate the camera defocus dosk basis vectors
        double defocus_radius = focus_distance * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    Vec3 sample_square() const {
        // Returns the vector to a random point in the [-0.5, -0.5] - [+0.5, +0.5] unit square
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly sampled
        // point around the pixel location i, j

        Vec3 offset = sample_square();
        Point3 pixel_sample = pixel_zero_loc + ((i + offset[0]) * pixel_delta_u) + ((j + offset[1]) * pixel_delta_v);

        Point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        Vec3 ray_direction = pixel_sample - ray_origin;
        double ray_time = random_double();

        return Ray(ray_origin, ray_direction, ray_time);
    }

    Point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk
        Vec3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    Color ray_color(const Ray& r, int depth, const Hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered
        if (depth <= 0) {
            return Color(0, 0, 0);
        }

        HitRecord rec;

        if (world.hit(r, Interval(0.001, infinity), rec)) {
            Ray scattered;
            Color attenuation;
            if(rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return Color(0, 0, 0);
        }

        Vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5 * (unit_direction[1] + 1.0);
        return ((1.0 - a) * Color(1.0, 1.0, 1.0)) + (a * Color(0.5, 0.7, 1.0));
    }
};
