
#ifndef ANIM_H
#define ANIM_H

#include "Timer.h"
#include "GameObjectAnim.h"
#include "FrameBucket.h"
#include "AnimFileHeader.h"
#include "GameObjectRigid.h"

// SSBO
#include "SSBO.h"
#include "ShaderMan.h"
//#include <sb7.h>

#include "ShaderObject.h"
// For Teddy
#define NUMS_BONE_BEAR 20
#define BONE_WIDTH 10.0f

// For Human
#define NUM_BONES_HUMAN 80
//#define BONE_WIDTH 30.0f
class AnimLink {
public:
	AnimLink()
	{
		this->next = 0;
		this->prev = 0;
	}
	virtual ~AnimLink() {};
	AnimLink* next;
	AnimLink* prev;
};

class Anim {

public:

	enum class Name
	{
		BEAR_WALK,
		BEAR_IDLE,
		WLAK_RUN,
		HUMAN_RUN,
		HUMAN_PUNCK,
		HUMAN_SHOT,
		NOT_INITIALIZED
	};
public:
	Anim();
	//Anim(Name name);
	~Anim();
	
	void setBonePose(GameObjectAnim* node);
	void SetAnimationPose(GameObjectAnim* root, Time tCurr);

	void SetAnimationHierarchy( HierarchyHeader* hireHeader);
	void SetAnimationData(AnimHeader* pAnimHeaser, int stackNum);
	void SetAnimationData_Reduced(Reduced_AnimHeader* pAnimHeaser, int stackNum);
	//  animation is only at a specific time
	void ProcessAnimation(Time tCurr);
	void FindMaxTime(Time& tMax);

	// FRANK SKIN
	void SetupBoneWorld();

	// Frank GPU
	void ProcessAnimationGPU(Time tCurr);
	void SetupMagicTable();
	void SetupBoneWorldGPU();

	//accessors
	void setName(Name name);
	Name getName();

public:
	GameObjectRigid* pGameRigid;
	FrameBucket* pFrameHead = 0;
	GameObjectAnim* pFirstBone;
	PCSNode* root = 0;
	Name animName;
	int BoneNums = 0; //rows

	// Frank Skin
	BoneHierarchy* pHierTable; // in big alloc
	int* poMagicTable;	//need destructor
	Matrix* poWorldAll;

private:
	//int rows = 0;
	int cols = 0;
	SSBO* poSSBO_bA;
	SSBO* poSSBO_bB;
	SSBO* poSSBO_bResult;
	ShaderObject* poMixerShader;
	ShaderObject* poWorldShader;
	SSBO* poSSBO_BoneTQRs;
	SSBO* poSSBO_MagicTable;
	SSBO* poSSBO_bWorldAll;


};

class AnimNode : public AnimLink
{
public:
	AnimNode() { pAnim = 0; };
	void set(Anim* pA) { pAnim = pA; }
	virtual ~AnimNode() { 
		delete pAnim; 
	}
public:
	Anim* pAnim;

	
};

#endif

//--- End of File -------------------------------------------------------------
