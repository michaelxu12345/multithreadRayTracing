#pragma once

#include "vec3.h"

#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linear_component) {
    return sqrt(linear_component);
    // return linear_component * linear_component;
}

// Write the translated [0,255] value of each color component.
void write_color(std::ostream& out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply linear to gamma transform (is this needed???)

    // r = linear_to_gamma(r);
    // g = linear_to_gamma(g);
    // b = linear_to_gamma(b);

    // divide color by # of samples
    auto scale = 1.0 / samples_per_pixel;
    if (samples_per_pixel == 0) {
        scale = 1;
    }
    r *= scale;
    g *= scale;
    b *= scale;

    // write out translated value of color component

    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}