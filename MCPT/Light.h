#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "Intersection.h"
#include "KdTree.h"

class Light {

public:

	glm::vec3 origin;//光源起点
	glm::vec3 dx, dy;//两条边
	glm::vec3 normal;//法向量
	//glm::vec3 emission;
	glm::vec3 ambient;//光强
	float area;


	Light(glm::vec3 _origin, glm::vec3 _dx, glm::vec3 _dy, glm::vec3 _ambient) :
		origin(_origin), dx(_dx), dy(_dy), ambient(_ambient) {
		normal = glm::cross(dx, dy);
		area = glm::length(normal);
		normal = glm::normalize(normal);
	};

	glm::vec3 render(const Ray &ray, Intersection &intersection,KdTree *kdTree) {
		
		if (ambient == glm::vec3(0))
			return glm::vec3(0);

		glm::vec3 color(0.0);

		Material* material = intersection.pMaterial;

		float sx = (float)rand() / RAND_MAX;
		float sy = (float)rand() / RAND_MAX;

		glm::vec3 lightOrigin = origin + dx * sx + dy * sy;
		glm::vec3 direction = lightOrigin - intersection.point;
		float dist = glm::length(direction);
		Ray shadowRay = Ray(intersection.point, direction);
		shadowRay.tmax = dist;

		if (!kdTree->shadowRayIntersect(shadowRay)) {

			glm::vec3 direction_normalized = glm::normalize(direction);

			float cosine_in = std::max(glm::dot(intersection.normal, direction_normalized), 0.0f);
			float cosine_out = std::max(glm::dot(-direction_normalized, normal), 0.0f);
			glm::vec3 intensity = area * ambient * cosine_in * cosine_out / (dist * dist);

			if (material->kd != glm::vec3(0)){
				float temp = glm::dot(direction_normalized, intersection.normal);
				if (temp > 0.0)
					color = temp * material->kd * intensity / PI *2.f;
			}

			if (material->ks != glm::vec3(0)){
				glm::vec3 h = glm::normalize(direction_normalized - ray.direction);
				float temp = glm::dot(h, intersection.normal);
				if (temp > 0.0)
					color = pow(temp, material->ns) * material->ks * intensity * (material->ns + 1) / (2.f * PI) *2.f ;
			}
		}
		return color;
	}
};
