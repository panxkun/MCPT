#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "Material.h"

class Intersection
{
public:
	Material* pMaterial;
	glm::vec3 point;//交点坐标
	glm::vec3 normal;//交点处的法向量
    Intersection() {};
};
