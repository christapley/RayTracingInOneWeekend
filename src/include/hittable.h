#pragma once

#include "ray.h"

namespace tapley {

struct hit_record {
    value_type t;
    Vec3 p;
    Vec3 normal;
};

class Hittable {
public:
	Hittable() = default;
	virtual ~Hittable() = default;
	Hittable(const Hittable &) = default;
	Hittable(Hittable &&) = default;
	
	Hittable &operator=(const Hittable &) = default;
	Hittable &operator=(Hittable &&) = default;

    virtual bool hit(const Ray& r, value_type t_min, value_type t_max, hit_record& rec) const = 0;
};

}