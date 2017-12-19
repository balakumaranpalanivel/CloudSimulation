#pragma once

// TODO: Use GLM instead of defining your own Vector Class
template<typename T>
class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(T xx) : x(xx), y(xx), z(xx) {}
	Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
	Vector3 operator * (const T& r) const { return Vector3(x * r, y * r, z * r); }
	Vector3 operator * (const Vector3<T> &v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	template<typename U>
	Vector3 operator / (const Vector3<U>& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }
	friend Vector3 operator / (const T r, const Vector3& v)
	{
		return Vector3(r / v.x, r / v.y, r / v.z);
	}
	const T& operator [] (size_t i) const { return (&x)[i]; }
	T& operator [] (size_t i) { return (&x)[i]; }
	T length2() const { return x * x + y * y + z * z; }
	T length() const { return std::sqrt(length2()); }
	Vector3& operator += (const Vector3<T>& v) { x += v.x, y += v.y, z += v.z; return *this; }
	Vector3& operator *= (const float& r) { x *= r, y *= r, z *= r; return *this; }
	friend Vector3 operator * (const float&r, const Vector3& v)
	{
		return Vector3(v.x * r, v.y * r, v.z * r);
	}
	friend std::ostream& operator << (std::ostream& os, const Vector3<T>& v)
	{
		os << v.x << " " << v.y << " " << v.z << std::endl; return os;
	}
	T x, y, z;
};

// Method to normalise the vector
template<typename T>
void normalize(Vector3<T>& vec)
{
	T len2 = vec.length();
	if (len2 > 0)
	{
		T invLen = 1 / std::sqrt(len2);
		vec.x *= invLen;
		vec.y *= invLen;
		vec.z *= invLen;
	}
}

// Adding Dot Product
template<typename T>
T dot(const Vector3<T>& va, const Vector3<T>& vb)
{
	return va.x*vb.x + va.y*vb.y + va.z*vb.z;
}

// Adding a typedef
using Vec3f = Vector3<float>;