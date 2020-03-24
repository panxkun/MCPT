#include "KdTree.h"
#include <algorithm>
#include <iostream>
using namespace std;


KdTree::KdTree(vector<Base*>& element_list, int _split_axis){
    build(element_list, _split_axis);
}

void KdTree::build(vector<Base*>& element_list, int _split_axis){
    axis = _split_axis;
    unsigned int length = element_list.size();

    if (length == 1) {
        low = element_list[0];
        high = NULL;
        box = low->getAABB();
    }
    else if (length == 2) {
        low = element_list[0];
        high = element_list[1];
        box = AABB::merge(low->getAABB(), high->getAABB());
    }
    else {
        vector<Base*> lowRoot, highRoot;
        divide(axis, element_list, &lowRoot, &highRoot);
        low = lowRoot.size() > 0 ? new KdTree(lowRoot, (axis + 1) % 3) : NULL;
        high = highRoot.size() > 0 ? new KdTree(highRoot, (axis + 1) % 3) : NULL;
		box = low == NULL ? high->getAABB() : (high == NULL ? low->getAABB() : AABB::merge(low->getAABB(), high->getAABB()));
    }
}


void KdTree::divide(int axis, const vector<Base*>& element_list, vector<Base*>* lowRoot, vector<Base*>* highRoot) {

    float minVal, maxVal;
    minVal = FLT_MAX;
    maxVal = -FLT_MAX;

    for (unsigned int i = 0; i < element_list.size(); i++){
        AABB bbox = element_list[i]->getAABB();
        minVal = min(minVal, bbox.getMinAxis(axis));
        maxVal = max(maxVal, bbox.getMaxAxis(axis));
    }

    float pivot = (maxVal + minVal) / 2;

    for (unsigned int i = 0; i < element_list.size(); i++){
        AABB bbox = element_list[i]->getAABB();
		(bbox.getMaxAxis(axis) + bbox.getMinAxis(axis)) / 2 < pivot ? lowRoot->push_back(element_list[i]) : highRoot->push_back(element_list[i]);
    }
    if (highRoot->size() == element_list.size()){
        float minCoord = FLT_MAX;
        unsigned int index;
		Base* obj;
        for (unsigned int i = 0; i < highRoot->size(); i++){
			auto &root = (*highRoot)[i];
            if ((root->getAABB().getMaxAxis(axis) + root->getAABB().getMinAxis(axis)) / 2 < minCoord) {
                minCoord = (root->getAABB().getMaxAxis(axis) + root->getAABB().getMinAxis(axis)) / 2;
                index = i;
                obj = root;
            }
        }
		highRoot->erase(highRoot->begin() + index);
		lowRoot->push_back(obj);
    }
    else if (lowRoot->size() == element_list.size()){
        float maxCoord = -FLT_MAX;
        unsigned int index;
		Base* obj;
        for (unsigned int i = 0; i < lowRoot->size(); i++) {
			auto &root = (*lowRoot)[i];
            if ((root->getAABB().getMaxAxis(axis) + root->getAABB().getMinAxis(axis)) / 2 > maxCoord){
                maxCoord = (root->getAABB().getMaxAxis(axis) + root->getAABB().getMinAxis(axis)) / 2;
                index = i;
                obj = root;
            }
        }
		lowRoot->erase(lowRoot->begin() + index);
		highRoot->push_back(obj);
    }
}


AABB KdTree::getAABB(){
    return box;
}

bool KdTree::intersect(Ray& ray, Intersection& intersection){
    if (!box.intersect(ray)) 
		return false;
    bool isHit = false;
    if (getAxis(ray.direction, axis) >= 0){
        if (low != NULL) isHit |= low->intersect(ray, intersection);
        if (high != NULL) isHit |= high->intersect(ray, intersection);
    }
    else{
        if (high != NULL) isHit |= high->intersect(ray, intersection);
        if (low != NULL) isHit |= low->intersect(ray, intersection);
    }
    return isHit;
}

bool KdTree::shadowRayIntersect(Ray& ray){
    if (!box.intersect(ray)) 
		return false;
    bool isHit = false;
    if (getAxis(ray.direction, axis) >= 0.0){
        if (low != NULL) isHit |= low->shadowRayIntersect(ray);
        if (!isHit&&high != NULL) isHit |= high->shadowRayIntersect(ray);
    }
    else{
        if (high != NULL) isHit |= high->shadowRayIntersect(ray);
        if (!isHit&&low != NULL) isHit |= low->shadowRayIntersect(ray);
    }
    return isHit;
}
