#include <iostream>
#include "Atmosphere.h"

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
	for (unsigned j = 0; j < height; ++j)
	{
		// TODO: What is this y?
		float y = 2.f*(j + 0.5f) / float(height - 1) - 1.f;
		for (unsigned i = 0; i < width; ++i, ++p)
		{
			// TODO: What is this x?
			float x = 2.f*(i + 0.5f) / float(width - 1) - 1.f;
			float z2 = x*x + y*y;
			if (z2 <= 1)
			{
				float phi = std::atan2(y, x);
				float theta = std::acos(1 - z2);
				Vec3f dir(sin(theta)*cos(phi), cos(theta),
					sin(theta)*sin(phi));
				// 1 meter above sea level
				*p = atmosphere.ComputeIncidentLight(Vec3f(0, atmosphere.mEarthRadius + 1, 0), dir, 0, kInfinity);
			}
		}
		fprintf(stderr, "\b\b\b\b\%3d%c", (int)(100 * j / (width - 1)), '%');
	}

	std::cout << "\b\b\b\b" << ((std::chrono::duration<float>)(std::chrono::high_resolution_clock::now() - t0)).count() << " seconds" << std::endl;

	// Rendering the image out to a ppm file
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	p = image;
	for (unsigned j = 0; j < height; ++j)
	{
		for (unsigned i = 0; i < width; ++i, ++p)
		{
			// Apply Tone mapping
			(*p)[0] = (*p)[0] < 1.413f ? pow((*p)[0] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[0]);
			(*p)[1] = (*p)[1] < 1.413f ? pow((*p)[1] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[1]);
			(*p)[2] = (*p)[2] < 1.413f ? pow((*p)[2] * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-(*p)[2]);

			ofs << (unsigned char)(std::min(1.f, (*p)[0]) * 255)
				<< (unsigned char)(std::min(1.f, (*p)[1]) * 255)
				<< (unsigned char)(std::min(1.f, (*p)[2]) * 255);
		}
	}
	ofs.close();
	delete[] image;
}

// Method to render a sequence of images
void RenderImageSequence(unsigned int numOfAngles)
{
	for (unsigned i = 0; i < numOfAngles; i++)
	{
		char filename[1024];
		sprintf(filename, "./skydome.%04d.ppm", i);
		float angle = i / float(numOfAngles - 1) * M_PI * 0.6;
		fprintf(stderr, "Rendering image %d, angle = %0.2f\n", i,
			angle * 180 / M_PI);
		RenderSkyDome_FishEyeCamera(Vec3f(0, cos(angle), -sin(angle)),
			filename);
	}
}


void main()
{
	// 128
	unsigned int nangles = 128;
	RenderImageSequence(nangles);
}