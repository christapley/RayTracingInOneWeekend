#pragma once

#include "hittable.h"
#include <vector>
#include <initializer_list>

namespace tapley {

class HittableList : public Hittable
{
public:
	HittableList() = delete;
	HittableList(std::initializer_list<Hittable *> hittables) {
		for(const auto &hittable : hittables) {
			_hittables.push_back(hittable);
		}
	}
	virtual ~HittableList() = default;
	HittableList(const HittableList &) = default;
	HittableList(HittableList &&) = default;
	
	HittableList &operator=(const HittableList &) = default;
	HittableList &operator=(HittableList &&) = default;

    bool hit(const Ray& r, value_type t_min, value_type t_max, hit_record& rec) const override
	{
		hit_record temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;
		for(const auto &hittable : _hittables) {
			if (hittable->hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
	
	std::vector<Hittable *> _hittables;
};

}