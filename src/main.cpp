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

    std::shared_ptr<Material> material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    std::shared_ptr<Material> material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    std::shared_ptr<Material> material_left = std::make_shared<Dielectric>(1.5);
    std::shared_ptr<Material> material_bubble = std::make_shared<Dielectric>(1.0 / 1.5);
    std::shared_ptr<Material> material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.01);

    world.add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    //double R = std::cos(pi/4);

    //std::shared_ptr<Material> material_left = std::make_shared<Lambertian>(Color(0, 0, 1));
    //std::shared_ptr<Material> material_right = std::make_shared<Lambertian>(Color(1, 0, 0));

    //world.add(std::make_shared<Sphere>(Point3(-R, 0, -1), R, material_left));
    //world.add(std::make_shared<Sphere>(Point3(R, 0, -1), R, material_right));

    // Camera
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 256;
    //cam.image_width = 1920;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookfrom = Point3(-2, 2, 1);
    cam.lookat = Point3(0, 0, -1);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 10.0;
    cam.focus_distance = 3.4;

    cam.render(world);

    return 0;
}
