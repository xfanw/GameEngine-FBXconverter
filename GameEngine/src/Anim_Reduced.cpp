#include "Anim.h"


void Anim::SetAnimationData_Reduced(Reduced_AnimHeader* pAnimHeader, int stackNum)
{
	//(void*)pAnimHeader;
	BoneNums = pAnimHeader->bonesPerFrame;
	//Trace::out("Reduced_bone nums:%d, stack num:%d\n", BoneNums, stackNum);
	int frames = pAnimHeader->FramesPerStack[stackNum];
	//Trace::out("Reduced_BoneOffset:%d\n", pAnimHeader->boneOffset);
	//Trace::out("Reduced_Frames:%d\n", frames);
	OUT_FRAME_Reduced* pBucket = (OUT_FRAME_Reduced*)((unsigned char*)pAnimHeader + pAnimHeader->frameOffset);
	Vect* pBoneT_79 = (Vect*)((unsigned char*)pBucket - sizeof(Vect)*(BoneNums -1)*(pAnimHeader->numStacks -stackNum));
	Quat* pBoneQ_79 = (Quat*)((unsigned char*)pAnimHeader + pAnimHeader->boneOffset);
	int loc = 0;
	for (int i = 0; i < stackNum; i++) {
		pBucket += pAnimHeader->FramesPerStack[i];
		//pBoneQ_79 += pAnimHeader->FramesPerStack[i] * (BoneNums -1);
		//Trace::out("Stack[%d],Frames: %d\n", i, pAnimHeader->FramesPerStack[i]);
		loc += pAnimHeader->FramesPerStack[i] * (BoneNums - 1) ;
	}
	//Trace::out("i = %d", loc);
	// --------  Result Frame  ----------------------
	pFrameHead = (FrameBucket*)malloc(sizeof(FrameBucket) * (frames + 1));
	pFrameHead->prev = nullptr;
	pFrameHead->next = &pFrameHead[1];
	pFrameHead->KeyTime = 0;
	pFrameHead->poBone = (Bone*)malloc(sizeof(Bone) * BoneNums);
	// for frame = 1 to last-1
	for (int i = 1; i < frames; i++) {
		pFrameHead[i].prev = &pFrameHead[i - 1];
		pFrameHead[i].next = &pFrameHead[i + 1];
		pFrameHead[i].KeyTime = pBucket[i - 1].KeyTime;
		pFrameHead[i].poBone = (Bone*)malloc(sizeof(Bone) * BoneNums);
		// set up first bone
		pFrameHead[i].poBone[0].T = pBucket[i - 1].pFirstBone.T;
		pFrameHead[i].poBone[0].Q = pBucket[i - 1].pFirstBone.Q;
		pFrameHead[i].poBone[0].S = pBucket[i - 1].pFirstBone.S;

		// setup rest bones
		for (int j = 1; j < BoneNums; j++) {
			pFrameHead[i].poBone[j].T = pBoneT_79[j - 1];
			//int sizeQ = (i-1)*sizeof(Quat)*(BoneNums-1) + j - 1;
			pFrameHead[i].poBone[j].Q = pBoneQ_79[loc++];
			pFrameHead[i].poBone[j].S = Vect(1.0, 1.0, 1.0, 1.0);
		}

	}
	// for the last frame
	pFrameHead[frames].prev = &pFrameHead[frames - 1];
	pFrameHead[frames].next = nullptr;
	pFrameHead[frames].KeyTime = pBucket[frames - 1].KeyTime;
	pFrameHead[frames].poBone = (Bone*)malloc(sizeof(Bone) * BoneNums);
	// setup rest bones
	for (int j = 1; j < BoneNums; j++) {
		pFrameHead[frames].poBone[j].T = pBoneT_79[j - 1];
		pFrameHead[frames].poBone[j].Q = pBoneQ_79[loc];
		pFrameHead[frames].poBone[j].S = Vect(1.0, 1.0, 1.0, 1.0);
	}

}