#include "Material.h"

using namespace Euler;

Material::Material(Vec3 diffuse, Vec3 specular, float shininess) {
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

Material::~Material() {
}