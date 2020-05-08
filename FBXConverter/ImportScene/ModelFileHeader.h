//----------------------------------------------------------------------------
// Copyright  2019
//----------------------------------------------------------------------------

#ifndef MODEL_FILE_HEADER_H
#define MODEL_FILE_HEADER_H

#include <string.h>

#define OBJECT_NAME_SIZE   32
#define TEXTURE_NAME_SIZE  32
#define STACK_NAME_SIZE  32
#define MAX_STACK_COUNT  32

struct Header {
	char	objName[OBJECT_NAME_SIZE];
	int VBOheaderOffset;
	int AnimHeaderOffset;
	int HierarchyHeaderOffset;
	int Skin_Vert_Offset;
	int ReducedHierOffset;
};

struct HierarchyHeader{
	int numBones;
	int offset;
};
struct AnimHeader{
	//char stackName[STACK_NAME_SIZE];
	int numStacks;
	int FramesPerStack[MAX_STACK_COUNT];
	int bonesPerFrame;

	int stackOffset;
	int frameOffset;
	int boneOffset;
	AnimHeader()
	{
		memset(this, 0x0, sizeof(AnimHeader));
	}
};

struct VBOHeader
{
	// object name
	char	objName[OBJECT_NAME_SIZE];

	// vertex buffer
	int		numVerts;
	unsigned long 	vertBufferOffset;

	// trilist index 
	int		numTriList;
	unsigned long 	triListBufferOffset;

	VBOHeader()
	{
		memset(this, 0x0, sizeof(VBOHeader));
	}
};

struct SkinHeader {
	char objName[OBJECT_NAME_SIZE];
	int numVerts;
	unsigned long vertWeightListOffset;

	int numPoseInv;
	unsigned long poseInvOffset;
	SkinHeader()
	{
		memset(this, 0x0, sizeof(SkinHeader));
	}
};

struct Reduced_AnimHeader {
	//char stackName[STACK_NAME_SIZE];
	int numStacks;
	int FramesPerStack[MAX_STACK_COUNT];
	int bonesPerFrame;

	int stackOffset;
	int frameOffset;
	int boneOffset;
	Reduced_AnimHeader()
	{
		memset(this, 0x0, sizeof(Reduced_AnimHeader));
	}
};

#endif

//--- End of File ------------------------------------------------------------

