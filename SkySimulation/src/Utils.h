#pragma once
#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <cmath>
#include <fstream>

#include "Atmosphere.h"
#include "Vector3.h"

const float kInfinity = std::numeric_limits<float>::max();

namespace Utils
{

	// Method to solve a quadratic equation based on
	// co efficients of equation
	bool SolveQuadratic(float a, float b, float c, float &x1, float &x2)
	{
		if (b == 0)
		{
			// Handle case where the ray and V are perpendicular
			// where V = ray.orig - sphere.centre
			if (a == 0)
			{
				return false;
			}

			x1 = 0;
			x2 = std::sqrtf(-c / a);
			return true;
		}

		float discr = b*b - 4 * a*c;
		if (discr < 0)
		{
			return false;
		}
		float q = (b < 0.f) ? -0.5f*(b - std::sqrtf(discr)) : -0.5f*(b + std::sqrtf(discr));
		x1 = q / a;
		x2 = c / q;

		return true;
	}

	// Method to compute the intersection of ray with a sphere
	// Implemented Analytically considering the equations of a 
	// ray and sphere.
	// Ray - mX + c
	// Sphere - x2 + y2 + z2 = r2
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
	bool RaySphereIntersect(const Vec3f& orig, const Vec3f& dir, const float& radius, float& t0, float& t1)
	{
		float A = dir.x * dir.x + dir.y*dir.y + dir.z*dir.z;
		float B = 2 * (dir.x * orig.x + dir.y*orig.y + dir.z*orig.z);
		float C = orig.x * orig.x + orig.y * orig.y + orig.z*orig.z 
			- radius*radius;

		if (!SolveQuadratic(A, B, C, t0, t1))
		{
			return false;
		}

		// Swap 
		if (t0 > t1)
		{
			std::swap(t0, t1);
		}

		return true;
	}
}