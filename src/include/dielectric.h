#pragma once

#include "material.h"

namespace tapley {

class Dielectric : public Material
{
public:
	Dielectric(value_type ri) : ref_idx(ri) {}
	Dielectric() = delete;
	virtual ~Dielectric() = default;
	Dielectric(const Dielectric &) = default;
	Dielectric(Dielectric &&) = default;
	Dielectric &operator=(const Dielectric &) = default;
	Dielectric &operator=(Dielectric &&) = default;
	
	bool scatter(const Ray& r_in, const hit_record& rec,
                             Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		value_type ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 0.0);
		Vec3 refracted;

		value_type reflect_prob;
        value_type cosine;
		
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal)
                        / r_in.direction().length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal)
                        / r_in.direction().length();
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
               reflect_prob = schlick(cosine, ref_idx);
		}
		else {
		   reflect_prob = 1.0;
		}

		if (random_double() < reflect_prob) {
		   scattered = Ray(rec.p, reflected);
		}
		else {
		   scattered = Ray(rec.p, refracted);
		}

		return true;
    } 
	
	value_type ref_idx;
};

}