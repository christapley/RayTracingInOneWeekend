#pragma once

#include "vec.h"

namespace tapley {

struct Ray {
    Vec3 _origin;
    Vec3 _direction;

    const Vec3 &origin() const { return _origin; }
    const Vec3 &direction() const { return _direction; }
    
    Ray() = default;
    Ray(const Ray &) = default;
    Ray(const Vec3& a, const Vec3& b) : _origin(a), _direction(b)
    {}
    
	Vec3 point_at_parameter(value_type t) const { return _origin + t*_direction; }
	
};

static Vec3 color(const Ray& Ray) {
    const auto unitDirection = unit_vector(Ray._direction);
    value_type t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t ) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

static bool hit_sphere(const Vec3& center, value_type radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    value_type a = dot(r.direction(), r.direction());
    value_type b = 2.0 * dot(oc, r.direction());
    value_type c = dot(oc, oc) - radius*radius;
    value_type discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

static Vec3 color_sphere(const Ray& r) {
    if (hit_sphere(Vec3(0,0,-1), 0.5, r))
        return Vec3(1, 0, 0);
    const auto unit_direction = unit_vector(r.direction());
    value_type t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}

static value_type hit_sphere_normal(const Vec3& center, value_type radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    value_type a = dot(r.direction(), r.direction());
    value_type b = 2.0 * dot(oc, r.direction());
    value_type c = dot(oc, oc) - radius*radius;
    value_type discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - std::sqrt(discriminant) ) / (2.0*a);
    }
}

static Vec3 color_sphere_normal(const Ray& r) {
    value_type t = hit_sphere_normal(Vec3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        Vec3 N = unit_vector(r.point_at_parameter(t) - Vec3(0,0,-1));
        return 0.5*Vec3(N.x()+1, N.y()+1, N.z()+1);
    }
	return color(r);
}

}