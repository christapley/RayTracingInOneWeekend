#pragma once

#include <cmath>
#include <sstream>

namespace tapley {

using value_type = double;
	
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

    inline value_type length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
    inline value_type squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
    inline void make_unit_vector();

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

inline void Vec3::make_unit_vector() {
    value_type k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
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
	
/*
using value_type = double;

struct Vec3 {
private:
	value_type _x;
	value_type _y;
	value_type _z;

	friend std::ostream& operator<<(std::ostream& os, const Vec3& obj)
	{
		return os << "x: " << obj._x << " y: " << obj._y << " z: " << obj._z;
	}
	
public:
	constexpr Vec3(const value_type x, const value_type y, const value_type z)
			: _x{x}, _y{y}, _z{z} {}

	constexpr value_type x() const { return this->_x; }
	constexpr value_type y() const { return this->_y; }
	constexpr value_type z() const { return this->_z; }

	const value_type& x() { return this->_x; }
	const value_type& y() { return this->_y; }
	const value_type& z() { return this->_z; }
	
	bool operator==(const Vec3 &other) const 
	{
		return(_x == other._x && _y == other._y && _z == other._z);
	}
	
	bool operator!=(const Vec3 &other) const 
	{
		return(_x != other._x && _y != other._y && _z != other._z);
	}
	
	Vec3 operator*(value_type scalar) const {
		return{
			_x * scalar, 
			_y * scalar, 
			_z * scalar
		};
	}
	
	value_type length() const {
		return std::hypot(this->x(), this->y(), this->z());
	}
};

struct FreeVec3 : Vec3 {
	using Vec3::Vec3;

	constexpr explicit FreeVec3(const Vec3& Vec3) : Vec3::Vec3{Vec3} {}

	constexpr value_type dot(const FreeVec3& other) const {
		return this->x() * other.x() + this->y() * other.y() + this->z() * other.z();
	}

	constexpr FreeVec3 cross(const FreeVec3& other) const {
		return FreeVec3{this->y() * other.z() - this->z() * other.y(),
										this->z() * other.x() - this->x() * other.z(),
										this->x() * other.y() - this->y() * other.x()};
	}

	const FreeVec3& operator+=(const FreeVec3& other) {
		_x += other.x();
		_y += other.y();
		_z += other.z();
		return *this;
	}

	const FreeVec3& operator-=(const FreeVec3& other) {
		_x -= other.x();
		_y -= other.y();
		_z -= other.z();
		return *this;
	}

	const FreeVec3& operator*=(const value_type scalar) {
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		return *this;
	}

	const FreeVec3& operator/=(const value_type scalar) {
		_x /= scalar;
		_y /= scalar;
		_z /= scalar;
		return *this;
	}
};

struct BoundVec3 : Vec3 {
	using Vec3::Vec3;

	constexpr explicit BoundVec3(const Vec3& Vec3) : Vec3::Vec3{Vec3} {}

	constexpr BoundVec3& operator+=(const FreeVec3& other) {
		this->x() += other.x();
		this->y() += other.y();
		this->z() += other.z();
		return *this;
	}

	constexpr BoundVec3& operator-=(const FreeVec3& other) {
		this->x() -= other.x();
		this->y() -= other.y();
		this->z() -= other.z();
		return *this;
	}
};

constexpr FreeVec3 operator-(const BoundVec3& v1, const BoundVec3& v2) {
	return FreeVec3{v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

constexpr BoundVec3 operator+(BoundVec3 v1, const FreeVec3& v2) {
	return v1 += v2;
}

constexpr BoundVec3 operator-(BoundVec3 v1, const FreeVec3& v2) {
	return v1 -= v2;
}

constexpr FreeVec3 operator+(const FreeVec3& v) { return v; }

constexpr FreeVec3 operator-(const FreeVec3& v) {
	return FreeVec3{-v.x(), -v.y(), -v.z()};
}

constexpr FreeVec3 operator+(FreeVec3 v1, const FreeVec3& v2) {
	return v1 += v2;
}

constexpr FreeVec3 operator-(FreeVec3 v1, const FreeVec3& v2) {
	return v1 -= v2;
}

constexpr FreeVec3 operator*(FreeVec3 v, const value_type scalar) {
	return v *= scalar;
}

constexpr FreeVec3 operator/(FreeVec3 v, const value_type scalar) {
	return v /= scalar;
}

struct UnitVec3 {
	UnitVec3(value_type x, value_type y, value_type z)
			: UnitVec3{FreeVec3{x, y, z}} {}

	explicit UnitVec3(const Vec3& Vec3) : UnitVec3{FreeVec3{Vec3}} {}

	explicit UnitVec3(const FreeVec3& Vec3) : inner{Vec3 / Vec3.length()} {}

	constexpr value_type x() const { return this->to_free().x(); }
	constexpr value_type y() const { return this->to_free().y(); }
	constexpr value_type z() const { return this->to_free().z(); }

	constexpr const FreeVec3& to_free() const { return inner; }

private:
	FreeVec3 inner;
};

constexpr FreeVec3 operator*(const UnitVec3& v, const value_type scalar) {
	return v.to_free() * scalar;
}

constexpr FreeVec3 operator/(const UnitVec3& v, const value_type scalar) {
	return v.to_free() / scalar;
}
*/

}