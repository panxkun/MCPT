#pragma once

#include "Ray.h"
#include "Engine.h"
#include "Intersection.h"


class MonteCarloPathTracer
{
private:
	int MAX_ITERATION;
	int MAX_DEPTH;
	glm::vec3 Ambient;
	glm::vec3 trace(Ray& ray, int cur_depth = 0);

public:
	MonteCarloPathTracer();
	float* render();
	Engine* pEngine;
};