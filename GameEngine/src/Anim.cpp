
#include "Framework.h"
#include "Anim.h"
#include "Constants.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "FrameBucket.h"

#include "GameObjectMan.h"
#include "GameObjectAnim.h"
void Anim::FindMaxTime(Time &tMax)
{
	FrameBucket *pTmp = pFrameHead->next;

	while (pTmp->next != 0)
	{
		pTmp = pTmp->next;
	}

	tMax = pTmp->KeyTime * Time(Time::NTSC_30_FRAME);
}



void Anim::ProcessAnimation(Time tCurr)
{
	//Trace::out("time:%f\n", tCurr / Time(Time::ONE_SECOND));

	FrameBucket *pTmp = pFrameHead->next;

	// Get the result bone array
	// Remember the firs Frame is the result
	Bone *bResult = pFrameHead->poBone;

	// First one is the result, skip it
	pTmp = pFrameHead->next;
 
	// Find which key frames
	while( tCurr >= pTmp->KeyTime * Time(Time::NTSC_30_FRAME) && pTmp->next != nullptr )
	{
		pTmp = pTmp->next;
	}

	// pTmp is the "B" key frame
	// pTmp->prev is the "A" key frame
	FrameBucket *pA = pTmp->prev;
	FrameBucket *pB = pTmp;

	// find the "S" of the time
	float tS = (tCurr - pA->KeyTime * Time(Time::NTSC_30_FRAME))  /(pB->KeyTime * Time(Time::NTSC_30_FRAME) - pA->KeyTime * Time(Time::NTSC_30_FRAME));
	
	// interpolate to 
	Bone *bA = pA->poBone;
	Bone *bB = pB->poBone;

   // Interpolate to tS time, for all bones
	for( int i = 0; i < BoneNums; i++ )
	{
		// interpolate ahoy!
		VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
		QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
		VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
		
		// advance the pointer
		bA++;
		bB++;
		bResult++;
   }

	// update the time
	//tCurr += deltaTime;

}


void Anim::SetupBoneWorld()
{
	Matrix* tmpWorld = poWorldAll;
	BoneHierarchy* tmpBone = pHierTable;
	for (int r = 0; r < BoneNums; r++) {
		// lock for name
		//Trace::out("%s\n", tmpBone->name);
		GameObjectAnim* tmpObj = GameObjectMan::FindAnim(tmpBone->name);
		*tmpWorld = *tmpObj->poWorld;

		// Next Bone
		tmpWorld++;
		tmpBone++;
	}

}

//--- End of File -------------------------------------------------------------
