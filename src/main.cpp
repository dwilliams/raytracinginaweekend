#include <fstream>
#include <iostream>
#include <memory>

#include <spdlog/spdlog.h>

#include "rtweekend.h"

#include "camera.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "sphere.h"
#include "vec3.h"

int main(void) {
    // Parse Command Arguments

    // Setup Logging
    spdlog::set_level(spdlog::level::debug);

    // World
    HittableList world;

    // --- Three Sphere Render
    //std::shared_ptr<Material> material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    //std::shared_ptr<Material> material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    //std::shared_ptr<Material> material_left = std::make_shared<Dielectric>(1.5);
    //std::shared_ptr<Material> material_bubble = std::make_shared<Dielectric>(1.0 / 1.5);
    //std::shared_ptr<Material> material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.01);
    //
    //world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1), 100.0, material_ground));
    //world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    //world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    //world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    //world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    // --- Simple Render
    //double R = std::cos(pi/4);
    //
    //std::shared_ptr<Material> material_left = std::make_shared<Lambertian>(Color(0, 0, 1));
    //std::shared_ptr<Material> material_right = std::make_shared<Lambertian>(Color(1, 0, 0));
    //
    //world.add(std::make_shared<Sphere>(Point3(-R, 0, -1), R, material_left));
    //world.add(std::make_shared<Sphere>(Point3(R, 0, -1), R, material_right));

    // --- Final Render
    std::shared_ptr<Material> ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            Point3 center(a + (0.9 * random_double()), 0.2, b + (0.9 * random_double()));

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    Color albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    Color albedo = Color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    std::shared_ptr<Material> material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    std::shared_ptr<Material> material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    std::shared_ptr<Material> material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    // Camera
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 256;
    //cam.image_width = 1920;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    
    cam.vfov = 20;
    cam.lookfrom = Point3(13, 2, 3);
    cam.lookat = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_distance = 10.0;

    cam.render(world);

    return 0;
}
