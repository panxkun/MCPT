#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "Material.h"

class Intersection
{
public:
	Material* pMaterial;
	glm::vec3 point;//��������
	glm::vec3 normal;//���㴦�ķ�����
    Intersection() {};
};
