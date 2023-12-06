#include "common.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main(int argc, char* argv[]) {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;  // set to 0 for no antialiasing
    cam.max_depth = 50;

    if (argc == 1) {
        cam.render(world);
    }
    else {
        cam.num_threads = atoi(argv[1]);
        std::cerr << "Running with " << cam.num_threads << " threads\n";
        cam.render_multithreaded(world);
    }



}