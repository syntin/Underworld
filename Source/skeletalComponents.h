#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class SkeletalComponents
{
public:
	SkeletalComponents();
	virtual ~SkeletalComponents();

public:
	void Initialize();
};


struct Bone
{
	std::string name;
	int parentIndex;		// -1 if root
	glm::mat4 inverseBindMatrix; // from gLTF
	glm::mat4 localTransform;	//from animation
	glm::mat4 worldTransform;  //combined with parent
};

struct Skeleton
{
	std::vector<Bone> bones;		// all bones
	std::vector<glm::mat4> skinMatrices; // final matrices for GPU

	bool dirty = true;
};
