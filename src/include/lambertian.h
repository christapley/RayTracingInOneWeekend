#pragma once

#include "material.h"

namespace tapley {

class Lambertian : public Material
{
public:
	Lambertian(const Vec3 &al) : albedo(al) {}
	Lambertian() = delete;
	virtual ~Lambertian() = default;
	Lambertian(const Lambertian &) = default;
	Lambertian(Lambertian &&) = default;
	Lambertian &operator=(const Lambertian &) = default;
	Lambertian &operator=(Lambertian &&) = default;
	
	bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const override
	{
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    } 
	
	Vec3 albedo;
};

}