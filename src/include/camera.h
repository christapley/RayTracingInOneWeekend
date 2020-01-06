#pragma once

#include "ray.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace tapley {

class Camera {
public:
	Camera() {
		lower_left_corner = Vec3(-2.0, -1.0, -1.0);
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
		origin = Vec3(0.0, 0.0, 0.0);
	}
	
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, value_type vfov, value_type aspect) {
		// vfov is top to bottom in degrees
		Vec3 u, v, w;
		value_type theta = vfov*M_PI/180;
		value_type half_height = tan(theta/2);
		value_type half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width*u - half_height*v - w;
		horizontal = 2*half_width*u;
		vertical = 2*half_height*v;
	}
	
	Ray get_ray(value_type u, value_type v) {
		return {
			origin,
			lower_left_corner + u*horizontal + v*vertical - origin
		};
	}

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};

}