#include "ray.h"
#include "gtest/gtest.h"

#include "BmpEncoder.h"

namespace tapley {

struct backgound_color_type
{
	inline static Vec3 apply(Ray &ray) {
		return color(ray);
	}
};

struct sphere_color_type
{
	inline static Vec3 apply(Ray &ray) {
		return color_sphere(ray);
	}
};

struct sphere_color_normal
{
	inline static Vec3 apply(Ray &ray) {
		return color_sphere_normal(ray);
	}
};



template<typename color_type>
void render_scene(const std::string &name)
{
	int nx = 1000;
    int ny = 500;
    
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
            Vec3 col = color_type::apply(r);
            
            *bmpData++ = int(255.99*col[2]);
            *bmpData++ = int(255.99*col[1]);
            *bmpData++ = int(255.99*col[0]);
        }
    }
    CBmpEncoder bmpEncoder(name + ".bmp", rawBmp.data(), nx, ny, false);
    bmpEncoder.Write();
}


TEST(QAray, background)
{
    render_scene<backgound_color_type>("background");
}

TEST(QAray, sphere)
{
    render_scene<sphere_color_type>("sphere");
}

TEST(QAray, sphere_normal)
{
	render_scene<sphere_color_normal>("sphere_normal");
}

}