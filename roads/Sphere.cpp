#include "Sphere.h"
#include "MeshLoader.h"

Sphere::Sphere(std::string&& name)
	: Object(std::move(name))
{
}
