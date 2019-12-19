#pragma once

#include "hittable.h"

namespace tapley {

class Sphere: public Hittable  {
public:
    Sphere() = delete;
    Sphere(Vec3 cen, value_type r) : center(cen), radius(r) {}
	virtual ~Sphere() = default;

    Sphere(const Sphere &) = default;
	Sphere(Sphere &&) = default;
	Sphere &operator=(const Sphere &) = default;
	Sphere &operator=(Sphere &&) = default;
	
	bool hit(const Ray& r, value_type t_min, value_type t_max, hit_record& rec) const override
	{
		Vec3 oc = r.origin() - center;
		const auto a = dot(r.direction(), r.direction());
		const auto b = dot(oc, r.direction());
		const auto c = dot(oc, oc) - radius*radius;
		const auto discriminant = b*b - a*c;
		if (discriminant > 0) {
			auto temp = (-b - sqrt(discriminant))/a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				return true;
			}
		}
		return false;
	}
    
	Vec3 center;
    value_type radius;
};


}