#pragma once

#include <algorithm>
#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Intersection.h"

class Intersection;
class AABB
{
public:

	glm::vec3 low = glm::vec3(FLT_MAX);
	glm::vec3 high = glm::vec3(-FLT_MAX);

	AABB() {};
	AABB(glm::vec3 _low, glm::vec3 _high) :low(_low), high(_high) {};

	static AABB merge(const AABB &box1, const AABB &box2) {
		glm::vec3 pt1, pt2;

		pt1.x = std::min(box1.low.x, box2.low.x);
		pt1.y = std::min(box1.low.y, box2.low.y);
		pt1.z = std::min(box1.low.z, box2.low.z);

		pt2.x = std::max(box1.high.x, box2.high.x);
		pt2.y = std::max(box1.high.y, box2.high.y);
		pt2.z = std::max(box1.high.z, box2.high.z);

		return AABB(pt1, pt2);
	}

	static AABB merge(const glm::vec3 &v1, const glm::vec3 &v2) {
		glm::vec3 pt1, pt2;

		pt1.x = std::min(v1.x, v2.x);
		pt1.y = std::min(v1.y, v2.y);
		pt1.z = std::min(v1.z, v2.z);

		pt2.x = std::max(v1.x, v2.x);
		pt2.y = std::max(v1.y, v2.y);
		pt2.z = std::max(v1.z, v2.z);

		return AABB(pt1, pt2);

	}

	void merge(const glm::vec3 &v) {

		low.x = low.x < v.x ? low.x : v.x;
		low.y = low.y < v.y ? low.y : v.y;
		low.z = low.z < v.z ? low.z : v.z;

		high.x = high.x > v.x ? high.x : v.x;
		high.y = high.y > v.y ? high.y : v.y;
		high.z = high.z > v.z ? high.z : v.z;
	}

	float getMaxAxis(int axis) {
		if (axis == 0) return high.x;
		if (axis == 1) return high.y;
		if (axis == 2) return high.z;
	}

	float getMinAxis(int axis) {
		if (axis == 0) return low.x;
		if (axis == 1) return low.y;
		if (axis == 2) return low.z;
	}


	bool intersect(Ray &ray) {

		glm::vec3 &inv = ray.inv_direction;
;

		float tmin_x = (low.x - ray.origin.x)*inv.x;
		float tmax_x = (high.x - ray.origin.x)*inv.x;
		float tmin_y = (low.y - ray.origin.y)*inv.y;
		float tmax_y = (high.y - ray.origin.y)*inv.y;
		float tmin_z = (low.z - ray.origin.z)*inv.z;
		float tmax_z = (high.z - ray.origin.z)*inv.z;

		float tmin = std::max(std::max(std::min(tmin_x, tmax_x), std::min(tmin_y, tmax_y)),std::min(tmin_z, tmax_z));
		float tmax = std::min(std::min(std::max(tmin_x, tmax_x), std::max(tmin_y, tmax_y)), std::max(tmin_z, tmax_z));

		if (tmax < 0 || tmin > tmax) return false;
		return (tmin < ray.tmax && tmax > ray.tmin);

		//float tmin_x = ray.direction.x > 0 ? (low.x - ray.origin.x)*ray.inv_direction.x : (high.x - ray.origin.x)*ray.inv_direction.x;
		//float tmax_x = ray.direction.x < 0 ? (low.x - ray.origin.x)*ray.inv_direction.x : (high.x - ray.origin.x)*ray.inv_direction.x;
		//float tmin_y = ray.direction.y > 0 ? (low.y - ray.origin.y)*ray.inv_direction.y : (high.y - ray.origin.y)*ray.inv_direction.y;
		//float tmax_y = ray.direction.y < 0 ? (low.y - ray.origin.y)*ray.inv_direction.y : (high.y - ray.origin.y)*ray.inv_direction.y;
		//float tmin_z = ray.direction.z > 0 ? (low.z - ray.origin.z)*ray.inv_direction.z : (high.z - ray.origin.z)*ray.inv_direction.z;
		//float tmax_z = ray.direction.z < 0 ? (low.z - ray.origin.z)*ray.inv_direction.z : (high.z - ray.origin.z)*ray.inv_direction.z;
		//
		//float tmax = fmin(tmax_x, fmin(tmax_y, tmax_z));
		//float tmin = fmax(tmin_x, fmax(tmin_y, tmin_z));
		//
		//return tmin < tmax;
	}

};

