#include "hittablelist.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"

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

TEST(QAhittable, hittableList)
{
    int nx = 100;
    int ny = 50;
    
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
	HittableList hittables{
		new Sphere(Vec3(0,0,-1), 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere(Vec3(0,-100.5,-1), 100, new Lambertian({0.8, 0.8, 0.0}))
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

TEST(QAhittable, antialias)
{
    int nx = 100;
    int ny = 50;
    int ns = 100;
	
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
	HittableList hittables{
		new Sphere(Vec3(0,0,-1), 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere(Vec3(0,-100.5,-1), 100, new Lambertian({0.8, 0.8, 0.0}))
	};
	Camera cam;
    auto *bmpData = rawBmp.data();
    	
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            
			Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                value_type u = value_type(i + random_double()) / value_type(nx);
                value_type v = value_type(j + random_double()) / value_type(ny);
                auto r = cam.get_ray(u, v);
                col += hittable_color(r, &hittables);
            }
            col /= value_type(ns);
			 
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder("antialias.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

Vec3 hittable_color_diffuse(const Ray& r, Hittable *world) {
    hit_record rec;
    if (world->hit(r, 0.0001, std::numeric_limits<value_type>::max(), rec)) {
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * hittable_color(Ray(rec.p, target - rec.p), world);
    }
    else {
        Vec3 unit_direction = unit_vector(r.direction());
        value_type t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}

TEST(QAhittable, diffuse)
{
    int nx = 100;
    int ny = 50;
    int ns = 200;
	
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
    Vec3 lower_left_corner(-2.0, -1.0, -1.0);
    Vec3 horizontal(4.0, 0.0, 0.0);
    Vec3 vertical(0.0, 2.0, 0.0);
    Vec3 origin(0.0, 0.0, 0.0);
    
	HittableList hittables{
		new Sphere(Vec3(0,0,-1), 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere(Vec3(0,-100.5,-1), 100, new Lambertian({0.8, 0.8, 0.0}))
	};
	Camera cam;
    auto *bmpData = rawBmp.data();
    	
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            
			Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                value_type u = value_type(i + random_double()) / value_type(nx);
                value_type v = value_type(j + random_double()) / value_type(ny);
                auto r = cam.get_ray(u, v);
                col += hittable_color_diffuse(r, &hittables);
            }
            col /= value_type(ns);
			col = Vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder("diffuse.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}
	
}