#pragma once

#include "hittable.h"
#include "util.h"

namespace tapley {

class Material {
public:
	Material() = default;
	virtual ~Material() = default;
	Material(const Material &) = default;
	Material(Material &&) = default;
	Material &operator=(const Material &) = default;
	Material &operator=(Material &&) = default;

	virtual bool scatter(const Ray &rayIn, const hit_record &record, Vec3 &attenuation, Ray &scattered) const = 0;
};
	
}