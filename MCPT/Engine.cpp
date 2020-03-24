#include "Engine.h"
#include "MonteCarloPathTracer.h"
#include <gl/glut.h>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>


using namespace std;

int Engine::width = 100;
int Engine::height = 100;
float *Engine::colors = NULL;
MonteCarloPathTracer *Engine::pPathTracer = NULL;

Engine::Engine(Camera *_pCamera, Model *_pModel, MonteCarloPathTracer *_pPathTracer) {

	pCamera = _pCamera;
	kdTree = &_pModel->kdTree;
	lights = &_pModel->lights;
	pPathTracer = _pPathTracer;
	pPathTracer->pEngine = this;
	width = pCamera->width;
	height = pCamera->height;
	pModel = _pModel;

	if (colors != NULL)
		delete[] colors;
	colors = new float[3 * width*height];

	info();
}

Engine::~Engine() {
	if (colors != NULL) {
		delete[] colors;
		colors = NULL;
	}
}

void Engine::info() {
	cout << "file : " << pModel->name << endl;
	cout << "resolution : " << width << " X " << height << endl;
	cout << "triangle : " << pModel->pTriangles.size() << endl;
	cout << "light : " << pModel->lights.size() << endl;
}

Ray Engine::getRays(float x, float y) {
	float sx = (float)rand() / RAND_MAX;
	float sy = (float)rand() / RAND_MAX;
	Ray ray = pCamera->getRay((x + sx) / width, (y + sy) / height);
	return ray;
}

glm::vec3 Engine::directLighting(Intersection& intersection, Ray& ray) {
	glm::vec3 color(0.0);
	for (int i = 0, len = lights->size(); i < len; ++i) {
		color += lights->at(i).render(ray, intersection, this->kdTree);
	}
	return color;
}

void Engine::update() {
	glutPostRedisplay();
}

void Engine::run() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MonteCarloPathTracer");
	glutDisplayFunc(render);
	glutIdleFunc(update);
	glutMainLoop();
}

void Engine::render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
	glRasterPos2i(0, 0);
	pPathTracer->render();
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, (GLvoid *)colors);
	glFlush();
}

void Engine::savePic(int &iteration) {
	char picIdx[10];
	sprintf_s(picIdx, "%03d", iteration);
	string pic_name = (string)picIdx + "_" + pModel->name;

	pic_name = "./results/" + pModel->name + "/" + pic_name + ".png";

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24, 8, 8, 8);
	for (int y = 0; y < height; y++){
		BYTE *bits = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x < width; x++, bits += 3){
			int index = (y * width+ x) * 3;
			bits[0] = colors[index + 2] * 255;//b
			bits[1] = colors[index + 1] * 255;//g
			bits[2] = colors[index + 0] * 255;//r
		}
	}

	bool bSuccess = FreeImage_Save(FIF_PNG, bitmap, pic_name.c_str(), PNG_Z_NO_COMPRESSION);
	if (bSuccess == false)
		cout << "fail to save picture." << endl;
	FreeImage_Unload(bitmap);
}

void Engine::saveData(int &iteration) {
	char dataIdx[10];
	sprintf_s(dataIdx, "%03d", iteration);
	string data_name = dataIdx;

	time_t t;
	time(&t);


	data_name = "./data/" + pModel->name + "/" + data_name + "-" + to_string(t) + ".log";

	ofstream file_writer(data_name);

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int index = (row*width + col) * 3;
			file_writer << colors[index] << " " << colors[index + 1] << " " << colors[index + 2] << endl;
		}
	}

	file_writer.close();
}