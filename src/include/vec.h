#pragma once

#include "types.h"

#include <cmath>
#include <sstream>

namespace tapley {
  
class Vec3 {
public:
    Vec3() {}
    Vec3(value_type e0, value_type e1, value_type e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
    inline value_type x() const { return e[0]; }
    inline value_type y() const { return e[1]; }
    inline value_type z() const { return e[2]; }
    inline value_type r() const { return e[0]; }
    inline value_type g() const { return e[1]; }
    inline value_type b() const { return e[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline value_type operator[](int i) const { return e[i]; }
    inline value_type& operator[](int i) { return e[i]; }

    inline bool operator==(const Vec3 &v2) const;
    inline bool operator!=(const Vec3 &v2) const;
    inline Vec3& operator+=(const Vec3 &v2);
    inline Vec3& operator-=(const Vec3 &v2);
    inline Vec3& operator*=(const Vec3 &v2);
    inline Vec3& operator/=(const Vec3 &v2);
    inline Vec3& operator*=(const value_type t);
    inline Vec3& operator/=(const value_type t);

    inline value_type length() const { return std::hypot(e[0], e[1], e[2]); }
    inline value_type squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    inline void make_unit_vector() {
		const value_type k = 1.0 / length();
		e[0] *= k; e[1] *= k; e[2] *= k;
	}

    value_type e[3];
};

inline bool Vec3::operator==(const Vec3 &v2) const
{
    return e[0] == v2.e[0] && e[1] == v2.e[1] && e[2] == v2.e[2];
}

inline bool Vec3::operator!=(const Vec3 &v2) const
{
    return e[0] != v2.e[0] || e[1] != v2.e[1] || e[2] != v2.e[2];
}

inline std::istream& operator>>(std::istream &is, Vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator*(value_type t, const Vec3 &v) {
    return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, value_type t) {
    return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator/(Vec3 v, value_type t) {
    return Vec3(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline value_type dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.e[0]*v2.e[0]
         + v1.e[1]*v2.e[1]
         + v1.e[2]*v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3& Vec3::operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const value_type t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const value_type t) {
    value_type k = 1.0/t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline Vec3 unit_vector(const Vec3 &v) {
    return v / v.length();
}

}