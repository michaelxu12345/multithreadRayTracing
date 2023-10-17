#include "color.h"
#include "vec3.h"
#include <iostream>

using namespace std;
int main() {
    int width = 256, height = 256;
    cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int i = 0; i < width; i++) {
        clog << "\rScanlines remaining: " << (width - i) << ' ' << flush;
        for (int j = 0; j < height; j++) {
            color pixel_color = color(double(i) / (width - 1), double(j) / (height - 1), 0);
            write_color(cout, pixel_color);
        }
    }
    clog << "\rDone.                 \n";
}