
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
	int Hieroffset;
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



struct Reduced_AnimHeader {
	//char stackName[STACK_NAME_SIZE];
	int numStacks;
	int FramesPerStack[MAX_STACK_COUNT];
	int bonesPerFrame;

	int stackOffset;
	int frameOffset;
	int boneOffset;
};


#endif

//--- End of File ------------------------------------------------------------

