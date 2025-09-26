#include <fstream>
#include <iostream>
#include <memory>

#include <spdlog/spdlog.h>

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int main(void) {
    // Parse Command Arguments

    // Setup Logging
    spdlog::set_level(spdlog::level::debug);

    // World
    HittableList world;

    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 256;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);

    return 0;
}
