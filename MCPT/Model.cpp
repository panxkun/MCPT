#include "Model.h"
#include "utils.h"
#include "Triangle.h"
#include <iostream>
#include "utils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

Model::~Model() {
	for (int i = 0, len = pTriangles.size(); i < len; ++i)
	{
		if (pTriangles[i] != NULL)
		{
			delete pTriangles[i];
		}
	}
}

Model::Model(const std::string filename) {
	bbox = AABB(glm::vec3(FLT_MAX), glm::vec3(-FLT_MAX));
	load(filename);
}



bool Model::load(const std::string filename) {

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shape_list;
	std::vector<tinyobj::material_t> material_list;

	std::string warn, err;

	int pos = filename.find_last_of('/');
	name = filename.substr(pos + 1, filename.size() - pos - 5);

	std::string mtl_base_path = filename.substr(0, pos);

	if (!tinyobj::LoadObj(&attrib, &shape_list, &material_list, &warn, &err, filename.c_str(), mtl_base_path.c_str())) {
		throw std::runtime_error(warn + err);
	}

	meshes.resize(shape_list.size());
	materials.resize(material_list.size());

	for (int i = 0; i < materials.size(); i++) {

		tinyobj::material_t &material_ = material_list[i];
		Material &material = materials[i];

		material.name = material_.name;
		material.ka = glm::vec3(material_.ambient[0], material_.ambient[1], material_.ambient[2]);
		material.kd = glm::vec3(material_.diffuse[0], material_.diffuse[1], material_.diffuse[2]);
		material.ke = glm::vec3(material_.emission[0], material_.emission[1], material_.emission[2]);
		material.ks = glm::vec3(material_.specular[0], material_.specular[1], material_.specular[2]);
		material.tf = glm::vec3(material_.transmittance[0], material_.transmittance[1], material_.transmittance[2]);
		material.ns = material_.shininess;
		material.ni = material_.ior;

	}

	for (int i = 0; i < shape_list.size(); i++) {

		tinyobj::shape_t &shape = shape_list[i];
		tinyobj::mesh_t &mesh_ = shape.mesh;
		Mesh &mesh = meshes[i];


		int num_face = mesh_.num_face_vertices.size();
		int num_vertex = num_face * 3;
		int offset = 0;

		for (int f = 0; f < num_face; f++) {

			int num_face_vertices = mesh_.num_face_vertices[f];


			tinyobj::index_t idx;
			int vertex_index;
			int normal_index;

			mesh.materialIndex.push_back(shape.mesh.material_ids[f]);


			for (int v = 0; v < num_face_vertices; v++) {

				idx = mesh_.indices[offset + v];
				vertex_index = idx.vertex_index;
				normal_index = idx.normal_index;

				float px = attrib.vertices[vertex_index * 3 + 0];
				float py = attrib.vertices[vertex_index * 3 + 1];
				float pz = attrib.vertices[vertex_index * 3 + 2];

				mesh.vertices.push_back(glm::vec3(px, py, pz));

				float nx = attrib.normals[normal_index * 3 + 0];
				float ny = attrib.normals[normal_index * 3 + 1];
				float nz = attrib.normals[normal_index * 3 + 2];

				mesh.normals.push_back(glm::vec3(nx, ny, nz));

				mesh.indices.push_back(offset + v);

			}

			offset += num_face_vertices;
		}
	}

	int cnt = 0;
	for (Mesh &m : meshes) {

		for (int i = 0; i < m.indices.size(); i += 3) {
			
			Triangle *tri = new Triangle(&m, i);
			tri->pMaterial = &materials[m.materialIndex[i / 3]];
			pTriangles.push_back(tri);

			if (tri->pMaterial->ka != glm::vec3(0)) {
				cnt++;
				if (cnt % 1 == 0) {
					glm::vec3 &origin = tri->mesh->vertices[tri->indexes[0]];
					glm::vec3 &pt1 = tri->mesh->vertices[tri->indexes[1]];
					glm::vec3 &pt2 = tri->mesh->vertices[tri->indexes[2]];

					glm::vec3 edge1 = pt1 - origin;
					glm::vec3 edge2 = pt2 - origin;

					lights.push_back(Light(origin, edge1, edge2, tri->pMaterial->ka));
				}

			}

		}
	}

	kdTree.build(pTriangles);

	bbox = kdTree.getAABB();

}

AABB Model::getAABB() {
	return kdTree.getAABB();
}
bool Model::intersect(Ray &ray, Intersection &intersection) {
	return kdTree.intersect(ray, intersection);
}


