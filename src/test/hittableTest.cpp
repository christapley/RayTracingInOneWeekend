#include "hittablelist.h"
#include "sphere.h"

#include "gtest/gtest.h"

#include "BmpEncoder.h"

#include <limits>

namespace tapley {

Vec3 hittable_color(const Ray& r, Hittable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, std::numeric_limits<value_type>::max(), rec)) {
        return 0.5*Vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    }
    else {
        Vec3 unit_direction = unit_vector(r.direction());
        value_type t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}

TEST(QAray, hittableList)
{
    int nx = 1000;
    int ny = 500;
    
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
	HittableList hittables{
		new Sphere(Vec3(0,0,-1), 0.5),
		new Sphere(Vec3(0,-100.5,-1), 100)
	};
	
    auto *bmpData = rawBmp.data();
    	
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            
            value_type u = value_type(i) / value_type(nx);
            value_type v = value_type(j) / value_type(ny);
            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            
			//Vec3 p = r.point_at_parameter(2.0);
            Vec3 col = hittable_color(r, &hittables);
            
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder("hittableList.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}
	
}