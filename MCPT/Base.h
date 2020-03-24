#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include "Intersection.h"
#include "AABB.h"
#include "Ray.h"
#include "Material.h"

class Intersection;
class AABB;

class Base
{
public:
    Material *pMaterial;
    virtual AABB getAABB() = 0;
    virtual bool intersect(Ray& ray, Intersection& intersection) = 0;
    virtual bool shadowRayIntersect(Ray& ray)
    {
		Intersection intersection;
        return intersect(ray, intersection);
    }

};
