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
		float hm = 1200):
		mSunDirection(sd),
		mEarthRadius(er),
		mAtmoshpereRadius(ar),
		mHR(hr),
		mHM(hm)
	{
	}

	// Compute Color of Sky for a given camera ray
	/*
		This is a volume based rendering model using - Raymarch
		We first Raymarch along the primary ray - that is the ray which starts
		from the camera origin and exits the atmosphere or intersect with the
		planetory body.
		From each sample along the primary ray. A ray is cast from that point
		to the Sun and Raymarch along that ray as well. This is to account the
		effect of the light from sun at different point in the ray from camera
	*/
	Vec3f ComputeIncidentLight(const Vec3f& orig, const Vec3f& dir, float tmin, float tmax) const
	{
		float t0, t1;
		// Need not procees computation if the ray does not intersect the atmoshphere
		if (!Utils::RaySphereIntersect(orig, dir, mAtmoshpereRadius, t0, t1) || t1 < 0)
		{
			return 0;
		}

		// TODO - Remove after implementation
		return Vec3f(0, 0, 0);
	}

	Vec3f mSunDirection;		// The direction of the Sun (normailised)
	float mEarthRadius;			// The Radius of the earth
	float mAtmoshpereRadius;	// The Radius of the atmoshere - (from center of earth)
	float mHR;					// Thickness (Height) of atmoshphere for Rayleigh Scattering effect
	float mHM;					// Thickness (Height) of area from earth till where Mie Scattering is observed

	static const Vec3f mBetaR;	// Exponential Coefficient for Rayleigh Scattering
	static const Vec3f mBetaM;	// Exponential Coefficient for Mie Scattering
};

const Vec3f Atmosphere::mBetaR(3.8e-6f, 13.5e-6f, 33.1e-6f);
const Vec3f Atmosphere::mBetaM(21e-6f);
