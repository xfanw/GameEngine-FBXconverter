#ifndef FRAME_BUCKET
#define FRAME_BUCKET

#include "Timer.h"
#include "Vect.h"
#include "Quat.h"
struct Bone : public Align16
{
	Vect  T;
	Quat  Q;
	Vect  S;
	Bone() {};
	~Bone() {};
};

struct OUT_FRAME_Reduced {
	int	KeyTime;
	Bone pFirstBone;
};
struct OUT_STACK_Reduced {
	int frameCount;
};


struct FrameBucket
{
	FrameBucket *next;
	FrameBucket *prev;
	int		  KeyTime;
	Bone		  *poBone;
	char pad[4];
};

struct BoneHierarchy {
	char name[64];
	char parentName[64];
	int index;
	int depth;
};

struct BoneStack {
	int frameCount;
	int layerNum;
	char name[16];
};



#endif