#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "utils.h"

enum SOURCE{ 
	O, //origin һ����ߣ����۾�����ȥ
	D, //diffuse ɢ��
	S, //specular ���淴��
	T //transmisson ����
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
