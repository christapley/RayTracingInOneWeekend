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
    
    Vec3 at(const value_type t) const {
        return _origin + (_direction * t);
    }
};

static Vec3 color(const Ray& ray) {
    const auto unitDirection = unit_vector(ray._direction);
    value_type t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t ) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

bool hit_sphere(const Vec3& center, value_type radius, const Ray& r) {
    Vec3 oc = r.origin() - center;
    value_type a = dot(r.direction(), r.direction());
    value_type b = 2.0 * dot(oc, r.direction());
    value_type c = dot(oc, oc) - radius*radius;
    value_type discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

Vec3 color_sphere(const Ray& r) {
    if (hit_sphere(Vec3(0,0,-1), 0.5, r))
        return Vec3(1, 0, 0);
    const auto unit_direction = unit_vector(r.direction());
    value_type t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
}

}