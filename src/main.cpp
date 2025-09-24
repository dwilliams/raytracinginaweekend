#include <iostream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

bool hit_sphere(const Point3& center, double radius, const Ray& r) {
    Vec3 oc = center - r.origin();
    double a = dot(r.direction(), r.direction());
    double b = -2.0 * dot(r.direction(), oc);
    double c = dot(oc, oc) - (radius * radius);
    double discriminant = (b * b) - (4 * a * c);
    return (discriminant >= 0);
}

Color ray_color(const Ray& r) {
    if (hit_sphere(Point3(0, 0, -1), 0.5, r)) {
        return Color(1, 0, 0);
    }
    
    // Background
    Vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);
    return ((1.0 - a) * Color(1.0, 1.0, 1.0)) + (a * Color(0.5, 0.7, 1.0));
}

int main(void) {

    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 1024;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

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

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines Remaining: " << (image_height - j) << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            
            Point3 pixel_center = pixel_zero_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            
            Vec3 ray_direction = pixel_center - camera_center;
            
            Ray r(camera_center, ray_direction);
            
            Color pixel_color = ray_color(r);
            
            write_color(std::cout, pixel_color);
            
        }
    }

    std::clog << "\rDone                  \n";

    return 0;
}
