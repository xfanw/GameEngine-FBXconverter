#ifndef FBX_STL_H
#define FBX_STL_H

// Inside FBX structure

#include <assert.h>
#include <vector>
#include <algorithm>
#include"MathEngine.h"


struct FBX_Bone_Pose :public Align16
{
	Vect T;
	Quat Q;
	Vect S;
};

struct FBX_Frame
{
	//FBX_Frame* prev;
	//FBX_Frame* next;
	int	   KeyTime;
	std::vector<FBX_Bone_Pose> FrameBoneList;
	//FBX_Bone* pBone;
	bool operator == (const FBX_Frame& b)
	{
		return this->KeyTime == b.KeyTime;
	}
	
};

struct FBX_Stack {
	int frameCount;
	int layerNum;
	char name[16];
	std::vector<FBX_Frame> StackFrameList;
	int boneCount;
	void reduce();
	bool isTheSame(FBX_Frame& fa, FBX_Frame& fb, int t, FBX_Frame& fs);
};

struct FBX_Frame_Reduced
{
	int	   KeyTime;
	FBX_Bone_Pose firstBone;
	std::vector<Quat> restBoneQ;
	FBX_Frame_Reduced(FBX_Frame & pFrame) {
		this->KeyTime = pFrame.KeyTime;
		this->firstBone = pFrame.FrameBoneList[0];
		int boneCount = pFrame.FrameBoneList.size();
		for (int i = 1; i < boneCount; i++) {
			restBoneQ.push_back(pFrame.FrameBoneList[i].Q);
		}
	};
	bool operator == (const FBX_Frame_Reduced& b)
	{
		return this->KeyTime == b.KeyTime;
	}

};


struct FBX_Stack_Reduced {
	int frameCount;
	int layerNum;
	char name[16];
	int boneCount;
	std::vector<Vect> restBoneT;
	std::vector<FBX_Frame_Reduced> reducedFrameList;
	FBX_Stack_Reduced(FBX_Stack& origStack) {
		this->frameCount = origStack.frameCount;
		this->layerNum = origStack.layerNum;
		strcpy(this->name, origStack.name);
		this->boneCount = origStack.boneCount;
		for (int i = 1; i < boneCount; i++) {
			restBoneT.push_back(origStack.StackFrameList[0].FrameBoneList[i].T);
		}
		for (int i = 0; i < frameCount; i++) {
			reducedFrameList.push_back(FBX_Frame_Reduced(origStack.StackFrameList[i]));
		}
	}
};
struct OUT_FRAME_Reduced {
	int	KeyTime;
	FBX_Bone_Pose pFirstBone;
};
struct OUT_STACK_Reduced {
	int frameCount;
};
struct FBX_Vertex
{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;

};


struct FBX_Vertex_vsn_Index
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;
	int index;

	// Frank use index
		bool operator == (const FBX_Vertex_vsn_Index &b)
		{
			return this->index == b.index;
		}
		bool operator < (const FBX_Vertex_vsn_Index &b) const
		{
			return this->index < b.index;
			/*if (this->x < b.x)
			{
				return true;
			}
			else if (this->x > b.x)
			{
				return false;
			}

			if (this->y < b.y)
			{
				return true;
			}
			else if (this->y > b.y)
			{
				return false;
			}

			if (this->z < b.z)
			{
				return true;
			}
			else if (this->z > b.z)
			{
				return false;
			}

			if (this->s < b.s)
			{
				return true;
			}
			else if (this->s > b.s)
			{
				return false;
			}

			if (this->t < b.t)
			{
				return true;
			}
			else if (this->t > b.t)
			{
				return false;
			}

			if (this->nx < b.nx)
			{
				return true;
			}
			else if (this->nx > b.nx)
			{
				return false;
			}

			if (this->ny < b.ny)
			{
				return true;
			}
			else if (this->ny > b.ny)
			{
				return false;
			}

			if (this->nz < b.nz)
			{
				return true;
			}
			else if (this->nz > b.nz)
			{
				return false;
			}
			else
			{
				return false;
			}*/
		}

};
struct FBX_Vertex_vsn
{
	float x;
	float y;
	float z;
	float s;
	float t;
	float nx;
	float ny;
	float nz;

	FBX_Vertex_vsn(FBX_Vertex_vsn_Index vsn) {
		x = vsn.x;
		y = vsn.y;
		z = vsn.z;
		s = vsn.s;
		t = vsn.t;
		nx = vsn.nx;
		ny = vsn.ny;
		nz = vsn.nz;
	}
};
struct FBX_Trilist
{
	int v0;
	int v1;
	int v2;
};


#endif