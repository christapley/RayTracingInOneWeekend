#pragma once

#include "vec.h"

namespace tapley {

inline Vec3 random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0*Vec3(random_double(), random_double(), random_double()) - Vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

inline bool refract(const Vec3& v, const Vec3& n, value_type ni_over_nt, Vec3& refracted) {
    Vec3 uv = unit_vector(v);
    const auto dt = dot(uv, n);
    value_type discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else {
        return false;
	}
}

inline value_type schlick(value_type cosine, value_type ref_idx) {
    value_type r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

}