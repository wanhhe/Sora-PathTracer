#include "accel_structure.h"
#include <algorithm>

AABB unionBound(const AABB& a, const AABB& b) {
	AABB c;
	c.lowerBound = lower(a.lowerBound, b.lowerBound);
	c.upperBound = upper(a.upperBound, b.upperBound);
	return c;
}

NaiveAccelStructure::NaiveAccelStructure(vector<Object*> objects) {
	for (auto object : objects) {
		container.insert(container.end(), object->primitives.begin(), object->primitives.end());
	}
}

bool NaiveAccelStructure::intersect(const Ray& ray, Intersection& intersection) {
	float indexNearset = -1;
	float tNear = std::numeric_limits<float>::max();
	float b1, b2;
	for (int i = 0; i < container.size(); i++) {
		float t, u, v;
		if (container[i]->intersect(ray, t, u, v) && t < tNear && t > 0) {
			tNear = t;
			b1 = u;
			b2 = v;
			indexNearset = i;
		}
	}
	if (indexNearset == -1) return false;

	intersection.happended = true;
	intersection.distance = tNear;
	intersection.coords = ray.origin + tNear * ray.direction;
	intersection.normal = container[indexNearset]->getNormal(1 - b1 - b2, b1, b2);
	intersection.material = container[indexNearset]->getMaterial();
	return true;
}


BVHAccelStructure::BVHAccelStructure(vector<Object*> objects) {
	for (auto object : objects) {
		container.insert(container.end(), object->primitives.begin(), object->primitives.end());
	}
	vector<int> indices;
	// ÐÞ¸Äcapacity´óÐ¡
	indices.reserve(container.size());
	for (int i = 0; i < container.size(); i++) {
		indices.emplace_back(i);
		auto t = container[i];
		AABB box;
		box.lowerBound = lower(lower(t->v0, t->v1), t->v2);
		box.upperBound = upper(upper(t->v0, t->v1), t->v2);

		if (box.lowerBound.x == box.upperBound.x) box.upperBound.x += 0.01f;
		if (box.lowerBound.y == box.upperBound.y) box.upperBound.y += 0.01f;
		if (box.lowerBound.z == box.upperBound.z) box.upperBound.z += 0.01f;

		containerAABB.emplace_back(box);
	}
	root = buildBVHTree(indices);
}

bool BVHAccelStructure::intersect(const Ray& ray, Intersection& intersection) {
	int indexNearest = -1;
	float tNear = std::numeric_limits<float>::max();
	float b1Near, b2Near;

	stack<TreeNode*> stackTreeNode;
	stackTreeNode.push(root);
	while (!stackTreeNode.empty()) {
		TreeNode* currentTreeNode = stackTreeNode.top();
		stackTreeNode.pop();

		if (currentTreeNode->isLeaf) {
			int index = currentTreeNode->triangleIndex;
			float t, b1, b2;
			if (container[index]->intersect(ray, t, b1, b2) && t < tNear && t > 0) {
				tNear = t;
				b1Near = b1;
				b2Near = b2;
				indexNearest = index;
			}
		}
		else {
			stackTreeNode.push(currentTreeNode->leftChild);
			stackTreeNode.push(currentTreeNode->rightChild);
		}
	}

	if (indexNearest == -1) return false;
	intersection.coords = ray.origin + tNear * ray.direction;
	intersection.normal = container[indexNearest]->getNormal(1 - b1Near - b2Near, b1Near, b2Near);
	intersection.distance = tNear;
	intersection.happended = true;
	intersection.material = container[indexNearest]->getMaterial();
	return true;
}

TreeNode* BVHAccelStructure::buildBVHTree(vector<int>& indices) {
	if (indices.empty()) throw std::runtime_error("TriangleIndices is empty.");
	if (indices.size() == 1) {
		AABB box = containerAABB[indices[0]];
		return new TreeNode(box, true, indices[0], nullptr, nullptr);
	}

	AABB box;
	for (auto index : indices) {
		box = unionBound(box, containerAABB[index]);
	}
	float lengthX = box.upperBound.x - box.lowerBound.x;
	float lengthY = box.upperBound.y - box.lowerBound.y;
	float lengthZ = box.upperBound.z - box.lowerBound.z;

	if (lengthX >= lengthY && lengthX >= lengthZ) {
		std::sort(indices.begin(), indices.end(), [this](int a, int b) {
			return container[a]->getBarycenterX() < container[b]->getBarycenterX();
			});
	}
	else if (lengthY >= lengthX && lengthY >= lengthZ) {
		std::sort(indices.begin(), indices.end(), [this](int a, int b) {
			return container[a]->getBarycenterY() < container[b]->getBarycenterY();
			});
	}
	else {
		std::sort(indices.begin(), indices.end(), [this](int a, int b) {
			return container[a]->getBarycenterZ() < container[b]->getBarycenterZ();
			});
	}

	auto begin = indices.begin();
	auto middle = indices.begin() + indices.size() / 2;
	auto end = indices.end();

	auto indicesLeft = vector<int>(begin, middle);
	auto indicesRight = vector<int>(middle, end);

	TreeNode* leftNode = buildBVHTree(indicesLeft);
	TreeNode* rightNode = buildBVHTree(indicesRight);
	return new TreeNode(box, false, -1, leftNode, rightNode);
}


SAHAccelStructure::SAHAccelStructure(vector<Object*> objects) {

}

bool SAHAccelStructure::intersect(const Ray& ray, Intersection& intersection) {
	return true;
}