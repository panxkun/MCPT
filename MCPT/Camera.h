#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
public:
	int width;
	int height;

	glm::vec3 origin;
	glm::vec3 direction;

	float fov;
	glm::vec3 front, up, right;
	glm::vec3 view_x, view_y, view_z;

	Camera() {
		lookAt(glm::vec3(0), glm::vec3(0), glm::vec3(0));
		calcViewPort(60, 600, 600);
	};

	Camera(
		glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up,
		float _fov, float _width, float _height) {
		lookAt(_eye, _center, _up);
		calcViewPort(_fov, _width, _height);
	}

	void lookAt(glm::vec3 _eye, glm::vec3 _center, glm::vec3 _up) {
		origin = _eye;
		direction = glm::normalize(_center - _eye);
		up = _up;
		right = glm::normalize(glm::cross(direction, up));
		up = glm::normalize(glm::cross(right, direction));
	}

	void calcViewPort(float _fov, float _width, float _height) {
		fov = _fov;
		width = (int)_width;
		height = (int)_height;

		float aspect = _height / _width;

		view_x = right * 2.f * tan(fov*PI / 360);
		view_y = up * 2.f * tan(fov*aspect*PI / 360);
		view_z = direction;
	}

	Ray getRay(float x, float y) {
		glm::vec3 direction = view_z + (x - 0.5f)*view_x + (y - 0.5f)*view_y;
		return Ray(origin, direction);
	}
};

