#pragma once

#include "common.h"
#include "color.h"
#include <iostream>
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "safequeue.h"
#include <thread>

class camera {
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width = 100;  // Rendered image width in pixel count
    int    samples_per_pixel = 10; // random samples per pixel for antialiasing
    int    max_depth = 10;    // max number of ray bounces into scene
    int    num_threads = 10;  // number of threads

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                if (samples_per_pixel == 0) {
                    ray r = get_single_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }

        std::clog << "\rDone.                 \n";
    }

    void render_multithreaded(const hittable_list& world) {
        initialize();
        image = std::vector<std::vector<color>>(
            image_width, std::vector<color>(image_height)
        );
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                queue.enqueue(std::make_pair(i, j));
            }
        }
        queue.enqueue(std::make_pair(-1, -1));

        for (int i = 0; i < num_threads; i++) {
            threads.push_back(std::thread(&camera::render_colors, this, world));
        }
        std::clog << "Threads started" << std::endl;
        for (int i = 0; i < num_threads; i++) {
            threads[i].join();
        }

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                write_color(std::cout, image[i][j], samples_per_pixel);
            }
        }
        std::clog << "\rDone.                 \n";
    }



private:
    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    SafeQueue<std::pair<int, int>> queue; // Thread safe queue
    std::vector<std::thread> threads;
    std::vector<std::vector<color>> image;

    void render_colors(const hittable_list& world) {
        while (true) {
            std::pair<int, int> current = queue.dequeue();
            int i = current.first;
            int j = current.second;
            if (i == -1) {
                queue.enqueue(std::make_pair(-1, -1));
                break;
            }
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            if (samples_per_pixel == 0) {
                ray r = get_single_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            image[i][j] = pixel_color;
            // std::clog << "\rFinished: " << i << ' ' << j << std::flush;

        }
    }

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for pixel at i, j

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    ray get_single_ray(int i, int j) const {
        // Get a non-randomly sampled camera ray for pixel at i, j

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center;

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Returns random point in square surrounding a pixel at the origin
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) {
        hit_record rec;

        // if past depth, don't gather more light
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth - 1, world);
            }
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

