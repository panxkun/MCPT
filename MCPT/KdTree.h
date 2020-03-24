#pragma once

#include "Base.h"
#include "AABB.h"
#include <vector>


class KdTree :public Base
{
public:

	KdTree() {};
    KdTree(std::vector<Base*>& element_list, int _axis = 2);

    void build(std::vector<Base*>& element_list, int _axis = 2);
    void divide(int axis, const std::vector<Base*>& element_list,
        std::vector<Base*>* lowRoot, std::vector<Base*>* highRoot);

    AABB getAABB();
    bool intersect(Ray& ray, Intersection& intersection);
    bool shadowRayIntersect(Ray& ray);

private:
	AABB box;
    int axis;//ªÆ∑÷÷·
	Base *low = NULL;
	Base *high = NULL;
};

