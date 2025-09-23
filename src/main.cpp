#include <iostream>

using namespace std;

int main(void) {

    // Image

    int image_width = 1024;
    int image_height = 1024;

    // Render

    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        clog << "\rScanlines Remaining: " << (image_height - j) << " " << flush;
        for (int i = 0; i < image_width; i++) {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.0;
            
            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);
            
            cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    clog << "\rDone                  \n";

    return 0;
}
