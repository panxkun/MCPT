#pragma once

#include <vector>
#include "Base.h"
#include "Light.h"
#include "Camera.h"
#include "KdTree.h"
#include "Model.h"

class Light;
class MonteCarloPathTracer;

class Engine
{
public:

	static int width, height;

	std::vector<Light>* lights;
	
	KdTree* kdTree;
	Camera* pCamera;
	Model* pModel;
	static float* colors;
	static MonteCarloPathTracer* pPathTracer;

	Engine() {};
	Engine(Camera *_pCamera, Model *_pModel, MonteCarloPathTracer *_pPathTracer);
	~Engine();

	Ray getRays(float x, float y);
	glm::vec3 directLighting(Intersection& intersection, Ray& ray);
	void savePic(int &iteration);
	void saveData(int &iteration);
	void info();
	static void run();
	static void update();
	static void render();
};

