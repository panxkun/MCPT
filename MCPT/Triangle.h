#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <vector>
#include "Base.h"
#include "Model.h"




class Triangle :public Base {
public:

	Mesh *mesh;
	glm::vec3 normal;
	AABB box;
	std::vector<unsigned int> indexes;
	glm::mat3x3 M;

	Triangle() {};
	Triangle(Mesh *_mesh, int idx) {

		mesh = _mesh;

		indexes.push_back(mesh->indices[idx + 0]);
		indexes.push_back(mesh->indices[idx + 1]);
		indexes.push_back(mesh->indices[idx + 2]);

		glm::vec3 &pt0 = mesh->vertices[indexes[0]];
		glm::vec3 &pt1 = mesh->vertices[indexes[1]];
		glm::vec3 &pt2 = mesh->vertices[indexes[2]];

		glm::vec3 edge1 = pt1 - pt0;
		glm::vec3 edge2 = pt2 - pt0;

		getM(pt0, pt1, pt2, M);

		normal = glm::normalize(glm::cross(edge1, edge2));

		box = AABB::merge(pt0, pt1);
		box.merge(pt2);



	}
	AABB getAABB() {
		return box;
	}

	//Picking in DirectX SDK
	//https://www.cnblogs.com/graphics/archive/2010/08/09/1795348.html
	bool intersect(Ray &ray, Intersection &intersection) {

		if (abs(glm::dot(normal, ray.direction)) < EPS)
			return false;

		glm::vec3 &origin = mesh->vertices[indexes[0]];
		glm::vec3 &pt1 = mesh->vertices[indexes[1]];
		glm::vec3 &pt2 = mesh->vertices[indexes[2]];

		glm::vec3 edge1 = pt1 - origin;
		glm::vec3 edge2 = pt2 - origin;

		glm::vec3 v = origin - ray.origin;

		glm::vec3 tmp1 = glm::cross(edge2, ray.direction);
		float beta = -glm::dot(tmp1, v) / glm::dot(tmp1, edge1);

		glm::vec3 tmp2 = glm::cross(edge1, ray.direction);
		float gamma = -glm::dot(tmp2, v) / glm::dot(tmp2, edge2);

		float t = glm::dot(normal, v) / glm::dot(normal, ray.direction);

		if (beta > 0 && gamma > 0 && beta + gamma < 1 &&
			ray.tmax > t && ray.tmin < t){
			intersection.point = ray.origin + ray.direction * t;;
			intersection.pMaterial = this->pMaterial;
			
			glm::vec3 weight = M * intersection.point;

			glm::vec3 &normal1 = mesh->normals[indexes[0]];
			glm::vec3 &normal2 = mesh->normals[indexes[1]];
			glm::vec3 &normal3 = mesh->normals[indexes[2]];

			intersection.normal = glm::normalize(glm::vec3(weight.x*normal1 + weight.y*normal2 + weight.z*normal3));

			ray.tmax = t;
			return true;
		}

		return false;

	}
};