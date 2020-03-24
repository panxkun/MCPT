#include "MonteCarloPathTracer.h"
#include "Engine.h"
#include <omp.h>
#include <iostream>
#include <string>
#include <ctime>
#include <random>

using namespace std;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);


MonteCarloPathTracer::MonteCarloPathTracer() {

	MAX_ITERATION = 100;
	MAX_DEPTH = 9;
	Ambient = glm::vec3(20, 20, 20);

}

glm::vec3 importanceSample(const glm::vec3 &normal, const float ns = -1.0) {

	float lambda1 = distribution(generator);
	float lambda2 = distribution(generator);

	float phi = lambda1 * 2 * PI;
	float theta = ns < 0 ? asinf(sqrt(lambda2)) : acosf(powf(lambda2, 1.f / (ns + 1)));

	glm::vec3 sample(sinf(theta)*cosf(phi), cosf(theta), sinf(theta)*sinf(phi));
	glm::vec3 front = abs(normal.x) > abs(normal.y) ? glm::normalize(glm::vec3(normal.z, 0, -normal.x)) : glm::normalize(glm::vec3(0, -normal.z, normal.y));
	glm::vec3  right = glm::cross(normal, front);

	return glm::normalize(sample.x*right + sample.y*normal + sample.z*front);
}

bool russianRoulette(float &probabiliy, float &survival) {
	survival = distribution(generator);
	return survival > probabiliy ? true : false;
}

float schlick(float ni, float ns, float cosine) {
	float r0 = (ni - ns) / (ni + ns);
	r0 *= r0;
	return r0 + (1 - r0)*pow((1.f - cosine), 5);
}


Ray monteCarloSample(Ray& ray, Intersection& intersection)
{
	Material* material = intersection.pMaterial;

	glm::vec3 direction = glm::vec3(0);

	float specular = glm::dot(material->ks, glm::vec3(1.f));
	float diffuse  = glm::dot(material->kd, glm::vec3(1.f));
	float prob = specular / (diffuse + specular);

	if (prob < 0) 
		return Ray(intersection.point, direction);

	if (material->ni != 1.0){

		float cosine = glm::dot(ray.direction, intersection.normal);
		float eta = cosine > 0.0 ? material->ni : 1 / material->ni;
		glm::vec3 normal = cosine <= 0.0 ? intersection.normal : -intersection.normal;

		float prob = schlick(material->ni, material->ns, abs(cosine));
		float survival;
		if (russianRoulette(prob, survival)){
			float discriminant = 1.f - eta * eta*(1.0f - cosine * cosine);
			if (discriminant >= 0.0f) {
				float cosine_ = glm::dot(normal, ray.direction);
				direction = eta * ray.direction - normal *(eta * cosine_ + sqrt(discriminant));
				return Ray(intersection.point - EPS * normal*10.f, direction, SOURCE::T);
			}
			else{
				direction = ray.direction - 2.f*glm::dot(normal, ray.direction)*normal;
				return Ray(intersection.point + normal * EPS, direction, SOURCE::S);
			}
		}
	}

	glm::vec3 normal = intersection.normal;
	float survival;
	if (russianRoulette(prob, survival)){
		direction = importanceSample(intersection.normal);
		return Ray(intersection.point + normal * EPS, direction, SOURCE::D);
	}
	else{
		glm::vec3 prefectReflectDirection = ray.direction - 2.f*glm::dot(normal, ray.direction)*normal;
		direction = importanceSample(prefectReflectDirection, material->ns);
		return Ray(intersection.point + normal * EPS, direction, SOURCE::S);
	}
}

glm::vec3 MonteCarloPathTracer::trace(Ray& ray, int cur_depth){
	
	Intersection intersection;

	if (!pEngine->kdTree->intersect(ray, intersection))
		return glm::vec3(0.0);

	Material* material = intersection.pMaterial;
	glm::vec3 surfaceLight = material->ke;

	if (cur_depth >= MAX_DEPTH)
		return surfaceLight;

	Ray nextRay = monteCarloSample(ray, intersection);

	if (nextRay.source != SOURCE::O) {
		glm::vec3 indirectLight = trace(nextRay, cur_depth + 1);
		if (nextRay.source == SOURCE::D)
			indirectLight *= (material->kd);
		if (nextRay.source == SOURCE::S)
			indirectLight *= material->ks;
		surfaceLight += indirectLight;
	}

	surfaceLight += pEngine->directLighting(intersection, ray) + material->ka*Ambient;
	return surfaceLight;
}

static double total_time = 0;
static int iterations_count = 0;
static int temp_count = 0;
float* MonteCarloPathTracer::render()
{

	++iterations_count;
	//++temp_count;
	if (iterations_count > MAX_ITERATION /*|| temp_count>MAX_ITERATION*/){
		
		return pEngine->colors;
	}
	int width = pEngine->width;
	int height = pEngine->height;


	//fill(pEngine->colors, pEngine->colors+width*height*3, 0);
	//iterations_count = 1;

	cout << "# " << iterations_count << "\t";

	clock_t start = clock();
#pragma omp parallel for schedule(dynamic, 1)
	for (int row = 0; row < height; ++row){
		for (int col = 0; col < width; ++col){

			Ray ray = pEngine->getRays(col, row);

			glm::vec3 single_sample = trace(ray);

			truncation(single_sample, 0.0f, 1.0f);

			int index = row * width * 3 + col * 3;
			
			pEngine->colors[index + 0] = (pEngine->colors[index + 0] * (iterations_count - 1) + single_sample.r) / iterations_count;
			pEngine->colors[index + 1] = (pEngine->colors[index + 1] * (iterations_count - 1) + single_sample.g) / iterations_count;
			pEngine->colors[index + 2] = (pEngine->colors[index + 2] * (iterations_count - 1) + single_sample.b) / iterations_count;

		}
		//if(row%(height/100)==0)
		//	printf("%.1f%%\n", row / (float)height * 100);
	}
	clock_t end = clock();
	double time_cost = (double)(end - start);
	total_time += time_cost;
	cout <<"time : " << time_cost/1000.f << "s  " << endl;
	if (iterations_count == MAX_ITERATION)
		cout << "Total Time : " << total_time / 1000.f << "s" << endl;

	pEngine->savePic(iterations_count);
	pEngine->saveData(iterations_count);
	return pEngine->colors;
}


