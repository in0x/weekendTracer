#include <fstream>
#include <cassert>
#include <cmath>

#include "Vec3f.h"
#include "ray.h"
#include "Sphere.h"
#include "IntersectableList.h"
#include "PNG.h"
#include "float.h"

// Solve:
// t * t * dot( B, B ) + 2 * t * dot( B, A - C ) + dot( A-C, A-C ) - R * R = 0

vec3 color(const ray& r, IntersectableList& world)
{
	Intersection record;
	if (world.intersect(r, 0.f, std::numeric_limits<float>::max(), record))
	{
		return 0.5f * vec3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
	}
	else
	{
		vec3 unitDir = r.direction().unitVector();
		// Rescale unitDir.y from -1 - 1 to 0 - 1
		float t = 0.5f * (unitDir.y() + 1.f);
		// Lerp white - blue.
		return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.f);
	}
}

int main()
{	
	assert((vec3(1, 0, 0).dot(vec3(0, 1, 0)) == 0.f));

	PNG img(200, 100, 3);

	// Image plane dimensions.
	vec3 origin(0.f,0.f,0.f);
	vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
	vec3 horizontal(4.f, 0.f, 0.f);
	vec3 vertical(0.f, 2.f, 0.f);
	
	IntersectableList world;
	world.Add(new Sphere(vec3(0.f, 0.f, -1.f), 0.5f));
	world.Add(new Sphere(vec3(0.f, -100.5f, -1.f), 100.f));

	// Raymarch through all pixels.
	for (int j = img.y - 1; j >= 0; j--)
	{
		for (int i = 0; i < img.x ; i++)
		{
			float u = float(i) / float(img.x);
			float v = float(j) / img.y;

			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			vec3 col = color(r, world);

			img.setPixel(i, img.y - 1 - j, 255.99f * col.x(), 255.99f * col.y(), 255.99f * col.z());
		}
	}

	if (!img.saveToImage("test.png")) std::cout << "Failed to save image" << std::endl;

    return 0;
}

