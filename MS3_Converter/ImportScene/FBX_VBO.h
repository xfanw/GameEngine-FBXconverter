#ifndef FBX_VBO_H
#define FBX_VBO_H

#include"MathEngine.h"
struct BoneHierarchy {
	char name[64];
	char parentName[64];
	int index;
	int depth;
};
// Out put to binary structure
struct OUT_BONE_POSE :public Align16
{
	Vect T;
	Quat Q;
	Vect S;
};

struct OUT_FRAME {
	OUT_FRAME* nextBucket;
	OUT_FRAME* prevBucket;
	int	KeyTime;
	OUT_BONE_POSE* pBone;
	char pad[4];
};

struct OUT_STACK{
	int frameCount;
	int layerNum;
	char name[16];
};


struct Skin_Weight_Index {
	
	float weightA;	
	float weightB;	
	float weightC;	
	float weightD;
	int indexA;
	int indexB;
	int indexC;
	int indexD;
};

#endif