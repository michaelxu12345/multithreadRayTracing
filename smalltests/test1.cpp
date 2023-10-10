#include <iostream>

using namespace std;
int main() {
    int width = 256, height = 256;
    cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            double r = (double)i / (width - 1);
            double g = 0;
            double b = (double)j / (height - 1);

            int ir = (int)(255.999 * r);
            int ig = (int)(255.999 * g);
            int ib = (int)(255.999 * b);
            cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}