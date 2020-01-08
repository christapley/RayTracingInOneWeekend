#include "hittablelist.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "gtest/gtest.h"

#include "BmpEncoder.h"

#include <limits>
#include <thread>
#include <vector>
#include <chrono>

namespace tapley {

static Vec3 hittable_material_color(const Ray& r, Hittable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<value_type>::max(), rec)) {
		Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation*hittable_material_color(scattered, world, depth+1);
        }
        else {
            return { 0,0,0 };
        }
    }
    else {
        Vec3 unit_direction = unit_vector(r.direction());
        value_type t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}

TEST(QAcamera, viewpoint)
{
    int nx = 1000;
    int ny = 500;
    int ns = 100;
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
	HittableList hittables{
		new Sphere({0, 0, -1}, 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere({0,-100.5,-1}, 100, new Lambertian({0.8, 0.8, 0.0})),
		new Sphere({1, 0, -1}, 0.5, new Metal({0.8, 0.6, 0.2})),
		new Sphere({-1, 0, -1}, 0.5, new Dielectric(1.5)),
		new Sphere({-1, 0, -1}, -0.45, new Dielectric(1.5))
	};
	
	Vec3 lookfrom(3,3,2);
	Vec3 lookat(0,0,-1);
	value_type dist_to_focus = (lookfrom-lookat).length();
	value_type aperture = 12.0;
	
	Camera cam(lookfrom, lookat, { 0,1,0 }, 90, value_type(nx)/value_type(ny), aperture, dist_to_focus);
    
	std::vector<std::thread> threads;
	int threadCount = 12;
	
	for(int i = 0; i < threadCount; i++) {
		threads.emplace_back([](int threadId, int threadCount, Camera &cam, HittableList &hittables, uint8_t *bmpData, int nx, int ny, int ns) {
			for (int j = ny-(1+threadId); j >= 0; j-=threadCount) {
				for (int i = 0; i < nx; i++) {
					Vec3 col(0, 0, 0);
					for (int s = 0; s < ns; s++) {
						const auto u = value_type(i + random_double()) / value_type(nx);
						const auto v = value_type(j + random_double()) / value_type(ny);
						auto r = cam.get_ray(u, v);
						col += hittable_material_color(r, &hittables, 0);
					}
					col /= value_type(ns);
					col = Vec3( std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]) );
					
					auto flippedJ = (ny - 1) - j;
					auto *pixelInBmp = bmpData + (flippedJ * nx * 3) + (i * 3);
					
					*pixelInBmp++ = int(255.99*col[2]);
					*pixelInBmp++ = int(255.99*col[1]);
					*pixelInBmp = int(255.99*col[0]);
				}
			}
		}, i, threadCount, cam, hittables, rawBmp.data(), nx, ny, ns);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	for(auto &thread : threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
    CBmpEncoder bmpEncoder("viewpoint.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

TEST(QAcamera, fov)
{
    int nx = 1000;
    int ny = 500;
    int ns = 100;
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
	HittableList hittables{
		new Sphere({0, 0, -1}, 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere({0,-100.5,-1}, 100, new Lambertian({0.8, 0.8, 0.0})),
		new Sphere({1, 0, -1}, 0.5, new Metal({0.8, 0.6, 0.2})),
		new Sphere({-1, 0, -1}, 0.5, new Dielectric(1.5)),
		new Sphere({-1, 0, -1}, -0.45, new Dielectric(1.5))
	};
	
	Vec3 lookfrom(3,3,2);
	Vec3 lookat(0,0,-1);
	value_type dist_to_focus = (lookfrom-lookat).length();
	value_type aperture = 12.0;
	
	Camera cam(lookfrom, lookat, { 0,1,0 }, 20, value_type(nx)/value_type(ny), aperture, dist_to_focus);
    
	std::vector<std::thread> threads;
	int threadCount = 12;
	
	for(int i = 0; i < threadCount; i++) {
		threads.emplace_back([](int threadId, int threadCount, Camera &cam, HittableList &hittables, uint8_t *bmpData, int nx, int ny, int ns) {
			for (int j = ny-(1+threadId); j >= 0; j-=threadCount) {
				for (int i = 0; i < nx; i++) {
					Vec3 col(0, 0, 0);
					for (int s = 0; s < ns; s++) {
						const auto u = value_type(i + random_double()) / value_type(nx);
						const auto v = value_type(j + random_double()) / value_type(ny);
						auto r = cam.get_ray(u, v);
						col += hittable_material_color(r, &hittables, 0);
					}
					col /= value_type(ns);
					col = Vec3( std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]) );
					
					auto flippedJ = (ny - 1) - j;
					auto *pixelInBmp = bmpData + (flippedJ * nx * 3) + (i * 3);
					
					*pixelInBmp++ = int(255.99*col[2]);
					*pixelInBmp++ = int(255.99*col[1]);
					*pixelInBmp = int(255.99*col[0]);
				}
			}
		}, i, threadCount, cam, hittables, rawBmp.data(), nx, ny, ns);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	for(auto &thread : threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
    CBmpEncoder bmpEncoder("fov.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

TEST(QAcamera, aperture)
{
    int nx = 1000;
    int ny = 500;
    int ns = 100;
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
	HittableList hittables{
		new Sphere({0, 0, -1}, 0.5, new Lambertian({0.8, 0.3, 0.3})),
		new Sphere({0,-100.5,-1}, 100, new Lambertian({0.8, 0.8, 0.0})),
		new Sphere({1, 0, -1}, 0.5, new Metal({0.8, 0.6, 0.2})),
		new Sphere({-1, 0, -1}, 0.5, new Dielectric(1.5)),
		new Sphere({-1, 0, -1}, -0.45, new Dielectric(1.5))
	};
	
	Vec3 lookfrom(3,3,2);
	Vec3 lookat(0,0,-1);
	value_type dist_to_focus = (lookfrom-lookat).length();
	value_type aperture = 2.0;
	
	Camera cam(lookfrom, lookat, { 0,1,0 }, 20, value_type(nx)/value_type(ny), aperture, dist_to_focus);
    
	std::vector<std::thread> threads;
	int threadCount = 12;
	
	for(int i = 0; i < threadCount; i++) {
		threads.emplace_back([](int threadId, int threadCount, Camera &cam, HittableList &hittables, uint8_t *bmpData, int nx, int ny, int ns) {
			for (int j = ny-(1+threadId); j >= 0; j-=threadCount) {
				for (int i = 0; i < nx; i++) {
					Vec3 col(0, 0, 0);
					for (int s = 0; s < ns; s++) {
						const auto u = value_type(i + random_double()) / value_type(nx);
						const auto v = value_type(j + random_double()) / value_type(ny);
						auto r = cam.get_ray(u, v);
						col += hittable_material_color(r, &hittables, 0);
					}
					col /= value_type(ns);
					col = Vec3( std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]) );
					
					auto flippedJ = (ny - 1) - j;
					auto *pixelInBmp = bmpData + (flippedJ * nx * 3) + (i * 3);
					
					*pixelInBmp++ = int(255.99*col[2]);
					*pixelInBmp++ = int(255.99*col[1]);
					*pixelInBmp = int(255.99*col[0]);
				}
			}
		}, i, threadCount, cam, hittables, rawBmp.data(), nx, ny, ns);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	for(auto &thread : threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
    CBmpEncoder bmpEncoder("aperture.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

TEST(QAcamera, randomScene)
{
    int nx = 1000;
    int ny = 500;
    int ns = 100;
    std::vector<uint8_t> rawBmp(nx * ny * 3);
    
	HittableList hittables{
		new Sphere({0,-1000,0}, 1000, new Lambertian({0.5, 0.5, 0.5})),
		new Sphere({0, 1, 0}, 1.0, new Dielectric(1.5)),
		new Sphere({-4, 1, 0}, 1.0, new Lambertian({0.4, 0.2, 0.1})),
		new Sphere({4, 1, 0}, 1.0, new Metal({0.7, 0.6, 0.5}, 0.0))
	};
	
	int n = 500;
	int i = 1;
	for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vec3 center(a+0.9*random_double(),0.2,b+0.9*random_double());
            if ((center-Vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    hittables._hittables.push_back(new Sphere(center, 0.2,
                        new Lambertian({random_double()*random_double(),
                                        random_double()*random_double(),
										random_double()*random_double()}
                        )
					));
                }
                else if (choose_mat < 0.95) { // metal
                    hittables._hittables.push_back(new Sphere(center, 0.2,
                            new Metal({0.5*(1 + random_double()),
                                       0.5*(1 + random_double()),
									   0.5*(1 + random_double())},
                                      0.5*random_double())));
                }
                else {  // glass
                    hittables._hittables.push_back(
						new Sphere(center, 0.2, new Dielectric(1.5))
					);
                }
            }
        }
    }
	
	
	Vec3 lookfrom(4,1.5,4);
	Vec3 lookat(0,0,-1);
	value_type dist_to_focus = (lookfrom-lookat).length();
	value_type aperture = 0.02;
	
	Camera cam(lookfrom, lookat, { 0,1,0 }, 90, value_type(nx)/value_type(ny), aperture, dist_to_focus);
    
	std::vector<std::thread> threads;
	int threadCount = 12;
	
	for(int i = 0; i < threadCount; i++) {
		threads.emplace_back([](int threadId, int threadCount, Camera &cam, HittableList &hittables, uint8_t *bmpData, int nx, int ny, int ns) {
			for (int j = ny-(1+threadId); j >= 0; j-=threadCount) {
				for (int i = 0; i < nx; i++) {
					Vec3 col(0, 0, 0);
					for (int s = 0; s < ns; s++) {
						const auto u = value_type(i + random_double()) / value_type(nx);
						const auto v = value_type(j + random_double()) / value_type(ny);
						auto r = cam.get_ray(u, v);
						col += hittable_material_color(r, &hittables, 0);
					}
					col /= value_type(ns);
					col = Vec3( std::sqrt(col[0]), std::sqrt(col[1]), std::sqrt(col[2]) );
					
					auto flippedJ = (ny - 1) - j;
					auto *pixelInBmp = bmpData + (flippedJ * nx * 3) + (i * 3);
					
					*pixelInBmp++ = int(255.99*col[2]);
					*pixelInBmp++ = int(255.99*col[1]);
					*pixelInBmp = int(255.99*col[0]);
				}
			}
		}, i, threadCount, cam, hittables, rawBmp.data(), nx, ny, ns);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	for(auto &thread : threads) {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
    CBmpEncoder bmpEncoder("random_scene.bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}

}