#pragma once
#include "Vector3.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif

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

		// Update the view limits
		if (t0 > tmin && t0 > 0)
		{
			tmin = t0;
		}
		if (t1 < tmax)
		{
			tmax = t1;
		}

		// The number of times we are marching along the ray?
		uint32_t numSamples = 16;
		uint32_t numSamplesLight = 8;
		float segmentLight = (tmax - tmin) / numSamples;
		float tCurrent = tmin;

		// Mie and Rayleigh Contribution
		Vec3f sumR(0), sumM(0);
		float opticalDepthR = 0, opticalDepthM = 0;

		// mu - Cosine of angle between sun and ray
		float mu = dot(dir, mSunDirection);
		// Phase value of Rayleigh
		float phaseR = 3.f / (16.f*M_PI) * (1 + mu*mu);
		// Phase value of Mie
		float g = 0.76f;
		float phaseM = 3.f / (8.f * M_PI) * ((1.f - g * g) * (1.f + mu * mu)) / ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f));

		// why numSamples ?? - Integeration
		for (int i = 0; i < numSamples; ++i)
		{
			Vec3f samplePosition = orig + (tCurrent + segmentLight*0.5f)*dir;
			float height = samplePosition.length() - mEarthRadius;
			// compute the optical depth for light
			float hr = exp(-height / mHR)*segmentLight;
			float hm = exp(-height / mHM)*segmentLight;
			opticalDepthR += hr;
			opticalDepthM += hm;

			// Light optical depth
			float t0Light, t1Light;
			Utils::RaySphereIntersect(samplePosition, mSunDirection, mAtmoshpereRadius, t0Light, t1Light);
			float segmentLengthLight = t1Light / numSamplesLight, tCurrentLight = 0;
			float opticalDepthLightR = 0, opticalDepthLightM = 0;
			int j;
			for (j = 0; j < numSamplesLight; j++)
			{
				Vec3f samplePositionLight = samplePosition + (tCurrentLight + segmentLengthLight*0.5f)*mSunDirection;
				float heightLight = samplePositionLight.length() - mEarthRadius;
				if (heightLight < 0)
				{
					break;
				}
				opticalDepthLightR += exp(-heightLight / mHR) * segmentLengthLight;
				opticalDepthLightM += exp(-heightLight / mHM) * segmentLengthLight;
				tCurrentLight += segmentLengthLight;
			}

			if (j == numSamplesLight)
			{
				// Adding the Rayleight and Mie effects
				Vec3f tau = (mBetaR * (opticalDepthR + opticalDepthLightR))
					+ (mBetaM * 1.1f * (opticalDepthM + opticalDepthLightM));
				Vec3f attenuation(exp(-tau.x), exp(-tau.y), exp(-tau.z));
				sumR += attenuation * hr;
				sumM += attenuation * hm;
			}
			tCurrent += segmentLight;
		}

		// Consider the intensity of the sun as 20
		return (sumR * mBetaR * phaseR * sumM * mBetaM * phaseM) * 20;
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
