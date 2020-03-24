#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <string>

class Material
{
public:
	std::string name;
	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 ke;
	glm::vec3 tf;
	float ns;
	float ni;
	Material(){};
};
