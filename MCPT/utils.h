#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <io.h>
#include <FreeImage.h>
#include <iostream>
#include <vector>

#ifdef _DEBUG
#pragma comment(lib, "FreeImaged.lib")
#else
#pragma comment(lib, "FreeImage.lib")
#endif

using namespace std;

const float EPS = 1e-5f;
const float PI = 3.1415926f;

 
static inline float getAxis(glm::vec3 &v, int axis) {
	if (axis == 0)
		return v.x;
	else if (axis == 1)
		return v.y;
	else
		return v.z;
}

static std::ostream &operator << (std::ostream &out, const glm::vec3 &vec) {
	out << vec.x << " " << vec.y << " " << vec.z;
	return out;
}

static std::ostream &operator << (std::ostream &out, const glm::mat4x4 &mat) {
	out << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << std::endl
		<< mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << std::endl
		<< mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << std::endl
		<< mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3];
	return out;
}

static void truncation(glm::vec3 &v, float min, float max) {
	
	if (v.x < min) v.x = min;
	if (v.y < min) v.y = min;
	if (v.z < min) v.z = min;

	if (v.x > max) v.x = max;
	if (v.y > max) v.y = max;
	if (v.z > max) v.z = max;
}

static void getM(glm::vec3 &pt1, glm::vec3 &pt2, glm::vec3 &pt3, glm::mat3x3 &M) {
	glm::mat3x3 temp,temp_inv;
	temp[0][0] = pt1.x; temp[0][1] = pt1.y; temp[0][2] = pt1.z;
	temp[1][0] = pt2.x; temp[1][1] = pt2.y; temp[1][2] = pt2.z;
	temp[2][0] = pt3.x; temp[2][1] = pt3.y; temp[2][2] = pt3.z;
	temp_inv = glm::inverse(temp);
	
	if (isfinite(temp_inv[0][0]))
		M = temp_inv;
	else
		M = temp;
}

static int fileRead(string path, vector<string> &fileVec)
{
	long long handle = 0;
	struct _finddata_t fileinfo;
	std::string temp_str;
	if ((handle = _findfirst(temp_str.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib&_A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					fileRead(temp_str.assign(path).append(fileinfo.name).c_str(), fileVec);
			}
			else {
				try {
					if (fileinfo.size == 0)
						throw - 1;
					else
						fileVec.push_back(temp_str.assign(path).append("/").append(fileinfo.name));
				}
				catch (int e) {
					if (e == -1)
						std::cout << "file is empty!" << std::endl;
				}
			}
		} while (_findnext(handle, &fileinfo) == 0);
		_findclose(handle);
	}

	return true;
}




static void offline_render(int width,int height,string file_name) {


	vector<string> file_list;
	string path = "./data/" + file_name;
	fileRead(path, file_list);

	float *colors = new float[width*height * 3];

	for (size_t i = 1; i <= file_list.size(); i++) {
		std::cout << file_list[i-1] << std::endl;
		ifstream file_reader(file_list[i-1]);
		float pixel;
		int pos = 0;
		while (file_reader >> pixel) {
			colors[pos] = (colors[pos] * (i - 1) + pixel) / i;
			pos++;
		}
		cout << "# " << i << endl;

		char picIdx[10];
		sprintf_s(picIdx, "%03d", i);

		string pic_name = (string)picIdx + "_" + file_name;

		pic_name = "./results/" + file_name + "/" + pic_name + ".png";

		FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24, 8, 8, 8);
		for (int y = 0; y < height; y++) {
			BYTE *bits = FreeImage_GetScanLine(bitmap, y);
			for (int x = 0; x < width; x++, bits += 3) {
				int index = (y * width + x) * 3;
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

	delete[] colors;


}