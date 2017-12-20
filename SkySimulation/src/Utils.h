#pragma once
#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <cmath>

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

	// Render the sky dome. 
	// TODO: Currently Method renders to a file. Change to render to window
	// TODO: Changing to window means optimising the implementation
	// TODO: Move this method to a seperate redering class
	void RenderSkyDome_FishEyeCamera(const Vec3f& sunDir, const char *filename)
	{
		Atmosphere atmosphere(sunDir);
		auto t0 = std::chrono::high_resolution_clock::now();
		// Image size
		const unsigned width = 512, height = 512;
		// Vector to hold the pixels of image
		Vec3f *image = new Vec3f[width*height], *p = image;
		// Initialise the image pointer with 0x0
		memset(image, 0x0, sizeof(Vec3f) * width * height);

		// Main Render loop
		for (unsigned j = 0; j < height; j++)
		{
			// TODO: What is this y?
			float y = 2.f*(j + 0.5f) / float(height - 1) - 1.f;
			for (unsigned i = 0; i < width; i++)
			{
				// TODO: What is this x?
				float x = 2.f*(i + 0.5f) / float(width - 1) - 1.f;
				float z2 = x*x + y*y;
				if (z2 <= 1)
				{
					float phi = std::atan2(y, x);
					float theta = std::acos(1 - z2);
					Vec3f dir(sin(theta*cos(phi)), cos(theta), 
						sin(theta)*sin(phi));
					// 1 meter above sea level
					*p = atmosphere.ComputeIncidentLight(Vec3f(0, atmosphere.mEarthRadius + 1, 0), dir, 0, kInfinity);
				}
			}
			fprintf(stderr, "\b\b\b\b\%3d%c", (int)(100 * j / (width - 1)), '%');
		}
	}
}