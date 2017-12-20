#pragma once
#include "Vector3.h"

class Atmosphere
{
public:
	Atmosphere(
		Vec3f sd = Vec3f(0, 1, 0),
		float er = 6360e3,
		float ar = 6420e3,
		float hr = 7994,
		float hm = 1200)
	{}

	// Compute Color of Sky for a given camera ray
	Vec3f ComputeIncidentLight(const Vec3f& orig, const Vec3f& dir, float tmin, float tmax) const
	{
		return Vec3f(0, 0, 0);
	}

	Vec3f mSunDirection;		// The direction of the Sun (normailised)
	float mEarthRadius;			// The Radius of the earth
	float mAtmoshpereRadius;	// The Radius of the atmoshere - (from center of earth)
	float mHR;					// Thickness (Height) of atmoshphere for Rayleigh Scattering effect
	float mHM;					// Thickness (Height) of area from earth till where Mie Scattering is observed

	static const Vec3f mBetaR;
	static const Vec3f mBetaM;
};
