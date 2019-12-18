#include "ray.h"
#include "gtest/gtest.h"

#include "BmpEncoder.h"

namespace tapley {

TEST(QAray, background)
{
    int nx = 200;
    int ny = 100;
    
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
    auto *bmpData = rawBmp.data();
    
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            
            value_type u = value_type(i) / value_type(nx);
            value_type v = value_type(j) / value_type(ny);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            Vec3 col = color(r);
            
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder("background.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

TEST(QAray, sphere)
{
    int nx = 200;
    int ny = 100;
    
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
    auto *bmpData = rawBmp.data();
    
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            
            value_type u = value_type(i) / value_type(nx);
            value_type v = value_type(j) / value_type(ny);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            Vec3 col = color_sphere(r);
            
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder("sphere.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}


}