#pragma once

#include "material.h"

namespace tapley {

static Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}

class Metal : public Material
{
public:
	Metal(const Vec3 &al, value_type f=1.0) : albedo(al) {
		if (f < 1) {
			fuzz = f; 
		}
		else {
			fuzz = 1;
		}
	}
	Metal() = delete;
	virtual ~Metal() = default;
	Metal(const Metal &) = default;
	Metal(Metal &&) = default;
	Metal &operator=(const Metal &) = default;
	Metal &operator=(Metal &&) = default;
	
	bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const override
	{
        auto reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    } 
	
	Vec3 albedo;
	value_type fuzz;
};

}