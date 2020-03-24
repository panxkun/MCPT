#include <iostream>
#include <gl/glut.h>
#include "Model.h"
#include "MonteCarloPathTracer.h"


using namespace std;



void render_cbox() {

	int width = 512, height = 512;
	float fov = 39.3077;
	glm::vec3 center = glm::vec3(278, 273, -799);
	glm::vec3 eye = glm::vec3(278, 273, -800);
	glm::vec3 up(0.f, 1.f, 0.f);

	Model model("./model/cbox.obj");
	Camera camera(eye, center, up, fov, width, height);
	MonteCarloPathTracer tracer;
	Engine engine(&camera, &model, &tracer);
	glm::vec3 origin1 = glm::vec3(0, 548, 0);
	glm::vec3 origin2 = glm::vec3(556, 548, 559);
	glm::vec3 dx = glm::vec3(556, 0, 0);
	glm::vec3 dy = glm::vec3(0, 0, 559);
	glm::vec3 emission1 = glm::vec3(0.50, 0.50, 0.50);
	glm::vec3 emission2 = glm::vec3(0.50, 0.50, 0.50);
	model.lights.push_back(Light(origin1, dx, dy, emission1));
	model.lights.push_back(Light(origin2, -dx, -dy, emission2));

	engine.run();
}

void render_diningroom() {
	int width = 1280, height = 720;
	float fov = 60;
	glm::vec3 center = glm::vec3(-0.5, 2, 0);
	glm::vec3 eye = glm::vec3(-0.5, 3, 5.5);
	glm::vec3 up(0, 1, 0);
	Model model("./model/diningroom.obj");
	Camera camera(eye, center, up, fov, width, height);
	MonteCarloPathTracer tracer;
	Engine engine(&camera, &model, &tracer);
	model.lights.clear();
	glm::vec3 origin1 = glm::vec3(-3.7, 3.6, -2);
	glm::vec3 origin2 = glm::vec3(2.5, 3.6, -2);
	glm::vec3 dx = glm::vec3(2, 0, 0);
	glm::vec3 dy = glm::vec3(0, 0, 1.5);
	glm::vec3 emission1 = glm::vec3(20.00, 20.00, 15.00);
	glm::vec3 emission2 = glm::vec3(20.00, 15.00, 20.00);
	model.lights.push_back(Light(origin1, dx, dy, emission1));
	model.lights.push_back(Light(origin2, -dx, -dy, emission2));

	engine.run();
}

void render_mis() {
	int width = 768, height = 512;
	float fov = 42;
	glm::vec3 center = glm::vec3(0, -2, 2.5);
	glm::vec3 eye = glm::vec3(0, 2, 15);
	glm::vec3 up(0, 1, 0);
	Model model("./model/mis.obj");
	Camera camera(eye, center, up, fov, width, height);
	MonteCarloPathTracer tracer;
	Engine engine(&camera, &model, &tracer);

	engine.run();
}

void cornell_box_render() {

	int width = 600, height = 500;
	float fov = 55;
	glm::vec3 center = glm::vec3(0, 0.795, -0.025);
	glm::vec3 eye = glm::vec3(0, 0.795,3);
	glm::vec3 up(0.f, 1.f, 0.f);

	Model model("./model/CornellBox-Glossy.obj");
	Camera camera(eye, center, up, fov, width, height);
	MonteCarloPathTracer tracer;
	Engine engine(&camera, &model, &tracer);
	model.lights.clear();
	glm::vec3 origin1 = glm::vec3(1, 1.57, 0.9900);
	glm::vec3 origin2 = glm::vec3(-1, 1.57, -1.0400);
	glm::vec3 dx = glm::vec3(-2, 0, 0);
	glm::vec3 dy = glm::vec3(0, 0, -2.030);
	glm::vec3 emission1 = glm::vec3(5, 5, 5);
	glm::vec3 emission2 = glm::vec3(5, 5, 5);
	model.lights.push_back(Light(origin1, dx, dy, emission1));
	model.lights.push_back(Light(origin2, -dx, -dy, emission2));

	engine.run();
}

int main(){

	//"离线"渲染，为了避免因为长时间运行突然中断，所以将每次采样图片得到的数据保存下来，在最后对数据进行平均采样
	//offline_render(512, 512, "cbox");
	//offline_render(1280, 720, "diningroom");
	//offline_render(768, 512, "mis");
	//offline_render(600, 500, "CornellBox-Glossy");

	render_cbox();

	//render_diningroom();

	//render_mis();

	//cornell_box_render();

	return 0;
}

