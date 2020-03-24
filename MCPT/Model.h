#pragma once

#include <vector>
#include <string>

#include "Material.h"
#include "AABB.h"
#include "Base.h"
#include "KdTree.h"
#include "Light.h"
class Mesh {

public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> normals;
	std::vector<int> materialIndex;

};

class Model :public Base {
public:
	std::vector<glm::vec3> vertices;
	std::vector<Base*> pTriangles;
	std::vector<glm::vec3> normals;
	std::vector<Material> materials;
	std::vector<Mesh> meshes;
	std::vector<Light> lights;

	std::string name;
	AABB bbox;
	KdTree kdTree;

	Model() {};
	~Model();
	Model(const std::string filename);

	bool load(const std::string filename);
	AABB getAABB();
	bool intersect(Ray &ray, Intersection &intersection);
};

