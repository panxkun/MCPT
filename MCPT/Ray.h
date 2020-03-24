#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "utils.h"

enum SOURCE{ 
	O, //origin 一般光线，从眼睛发出去
	D, //diffuse 散射
	S, //specular 镜面反射
	T //transmisson 折射
};

class Ray{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 inv_direction;
	float tmin, tmax;
	SOURCE source;

	Ray(glm::vec3 _origin = glm::vec3(0),
		glm::vec3 _direction = glm::vec3(0),
		SOURCE _source = SOURCE::O) {
		origin = _origin;
		direction = glm::normalize(_direction);
		inv_direction = 1.f / direction;
		source = _source;
		tmin = EPS;
		tmax = FLT_MAX;
	}
};
