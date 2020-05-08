
#include "Framework.h"
#include "Anim.h"
#include "Constants.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "FrameBucket.h"

#include "GameObjectMan.h"
#include "GameObjectAnim.h"

//// SSBO
//#include "SSBO.h"
//#include "ShaderMan.h"
////#include <sb7.h>
//
//#include "ShaderLink.h"

Anim::Anim() {
	poSSBO_bA = new SSBO();
	poSSBO_bB = new SSBO();
	poSSBO_bResult = new SSBO();
	poSSBO_BoneTQRs = new SSBO();
	poSSBO_MagicTable = new SSBO();
	poSSBO_bWorldAll = new SSBO();

	poMixerShader = new ShaderObject(ShaderObject::NAME::COMPUTE_MIXER, "ComputeMixer", ShaderObject::TYPE::COMPUTE);
	poWorldShader = new ShaderObject(ShaderObject::NAME::COMPUTE_WORLD, "ComputeWorldMatrix", ShaderObject::TYPE::COMPUTE);
}
void Anim::ProcessAnimationGPU(Time tCurr)
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

	// OLD WAY
	//bResult = pFrameHead->poBone;;
//  // Interpolate to tS time, for all bones
   ////Trace::out("Original Value: %d\n", BoneNums);
   //for( int i = 0; i < BoneNums; i++ )
   //{
   //	// interpolate ahoy!
   //	VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
   //	QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
   //	VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
   //	//Trace::out("Bone[%d]\nT: %f %f %f %f\n Q: %f %f %f %f\n S: %f %f %f %f\n", i,
   //	//	bResult->T.x(), bResult->T.y(), bResult->T.z(), bResult->T.w(),
   //	//	bResult->Q.qx(), bResult->Q.qy(), bResult->Q.qz(), bResult->Q.real(),
   //	//	bResult->S.x(), bResult->S.y(), bResult->S.z(), bResult->S.w());
   //	// advance the pointer
   //	bA++;
   //	bB++;
   //	bResult++;

//  }
	
	// GPU MIXER
	poMixerShader->SetActive();
	glUniform1f(poMixerShader->GetLocation("ts"), tS);
	glUniform1i(poMixerShader->GetLocation("boneCount"), BoneNums);

	//SSBO* pSSBO_bA = new SSBO();
	poSSBO_bA->Set(BoneNums, sizeof(Bone), bA);
	poSSBO_bA->Bind(0);

	//SSBO* pSSBO_bB = new SSBO();
	poSSBO_bB->Set(BoneNums, sizeof(Bone), bB);
	poSSBO_bB->Bind(1);

	//SSBO* pSSBO_bResult = new SSBO();
	poSSBO_bResult->Set(BoneNums, sizeof(Bone), bResult);
	poSSBO_bResult->Bind(2);

	poMixerShader->Dispatch(BoneNums / 20, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	Bone* bResult_SSBO_out = (Bone*)poSSBO_bResult->Map(SSBO::Access::READ_ONLY);
	 //Verify
	//Trace::out("SSBO VALUE\n");
	//for (int i = 0; i < BoneNums; i++) {
	//	Trace::out("Bone[%d]\nT: %f %f %f\n Q: %f %f %f\n S: %f %f %f\n", i,
	//		bResult_SSBO_out[i].T.x(), bResult_SSBO_out[i].T.y(), bResult_SSBO_out[i].T.z(),
	//		bResult_SSBO_out[i].Q.qx(), bResult_SSBO_out[i].Q.qy(), bResult_SSBO_out[i].Q.qz(),
	//		bResult_SSBO_out[i].S.x(), bResult_SSBO_out[i].S.y(), bResult_SSBO_out[i].S.z());
	//}

	 //Copy out to bresult
	for (int i = 0; i < BoneNums; i++) {
		bResult[i] = bResult_SSBO_out[i];
		//Trace::out("Bone[%d]\nT: %f %f %f %f\n Q: %f %f %f %f\n S: %f %f %f %f\n", i,
		//	bResult->T.x(), bResult->T.y(), bResult->T.z(), bResult->T.w(),
		//	bResult->Q.qx(), bResult->Q.qy(), bResult->Q.qz(), bResult->Q.real(),
		//	bResult->S.x(), bResult->S.y(), bResult->S.z(), bResult->S.w());
		//bResult++;
		
	}

	poSSBO_bResult->Unmap();
	

}

void Anim::SetupMagicTable()
{
	//1. Frank Verify BoneTable
	//BoneHierarchy* tmpH = this->pHierTable;
	//Trace::out("rows %d, cols %d\n", BoneNums, cols);
	//for (int i = 0; i < BoneNums; i++) {
	//	Trace::out("HIER[%d] Depth %d: this: %s\t parent:%s\n", tmpH->index, tmpH->depth, tmpH->name, tmpH->parentName);
	//	tmpH++;
	//}

	
	/*Matrix* pMagicCell = poMagicTable;*/
	// set up all identity matrix
	for (int r = 0; r < BoneNums; r++) {
		for (int c = 0; c < cols; c++) {
			poMagicTable[ r * cols + c] = -1;
		}
	}
	// first row setup
	poMagicTable[0] = 2;
	poMagicTable[1] = 0;
	for (int r = 1; r < BoneNums; r++) {
		// 1. colum[0] depth
		int d = pHierTable[r].depth;
		poMagicTable[r * cols + 0] = d;
		// 2. for each row, set up the last element
		poMagicTable[r * cols + d -1] = r;

		// 3. iterate to find parent
		int p = r;
		for (int c = d-2; c > 0; c--) {
			// 3.1 from 0 to current
			for (int tmp = 0; tmp < r; tmp++) {
				if (strcmp(this->pHierTable[tmp].name, this->pHierTable[p].parentName) == 0) {
					p = this->pHierTable[tmp].index;
				}
			}			
			poMagicTable[r * cols + c] = p;
		}
	}
	//Trace::out("Frank Verify MagicTable\n");
	//for (int r = 0; r < BoneNums; r++) {
	//	Trace::out("Row[%d]:", r);
	//	for (int c = 0; c < cols; c++) {
	//		Trace::out(" %d", poMagicTable[r * cols + c]);
	//	}
	//	Trace::out("\n");
	//}
}

void Anim::SetupBoneWorldGPU()
{
	// GPU MIXER
	poWorldShader->SetActive();
	glUniform1i(poWorldShader->GetLocation("hierarchyDepth"), cols);
	glUniform1i(poWorldShader->GetLocation("boneCount"), BoneNums);
	glUniformMatrix4fv(poWorldShader->GetLocation("rigidWorld"), 1, GL_FALSE, (float*)this->pGameRigid->GetWorld());
	//SSBO* pSSBO_bA = new SSBO();
	poSSBO_BoneTQRs->Set(BoneNums, sizeof(Bone), this->pFrameHead->poBone);
	poSSBO_BoneTQRs->Bind(0);

	//SSBO* pSSBO_bB = new SSBO();
	poSSBO_MagicTable->Set(BoneNums * cols, sizeof(int) , poMagicTable);
	poSSBO_MagicTable->Bind(1);

	//SSBO* pSSBO_bResult = new SSBO();
	poSSBO_bWorldAll->Set(BoneNums, sizeof(Matrix), this->poWorldAll);
	poSSBO_bWorldAll->Bind(2);

	poWorldShader->Dispatch(BoneNums / 20, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	Matrix* bResult_SSBO_out = (Matrix*)poSSBO_bWorldAll->Map(SSBO::Access::READ_ONLY);
	//Verify
   //Trace::out("SSBO VALUE\n");
   //for (int i = 0; i < BoneNums; i++) {
   //	Trace::out("Bone[%d]\nT: %f %f %f\n Q: %f %f %f\n S: %f %f %f\n", i,
   //		bResult_SSBO_out[i].T.x(), bResult_SSBO_out[i].T.y(), bResult_SSBO_out[i].T.z(),
   //		bResult_SSBO_out[i].Q.qx(), bResult_SSBO_out[i].Q.qy(), bResult_SSBO_out[i].Q.qz(),
   //		bResult_SSBO_out[i].S.x(), bResult_SSBO_out[i].S.y(), bResult_SSBO_out[i].S.z());
   //}

	//Copy out to bresult
	for (int i = 0; i < BoneNums; i++) {
		poWorldAll[i] = bResult_SSBO_out[i];
		//Trace::out("Bone[%d]\nT: %f %f %f %f\n Q: %f %f %f %f\n S: %f %f %f %f\n", i,
		//	bResult->T.x(), bResult->T.y(), bResult->T.z(), bResult->T.w(),
		//	bResult->Q.qx(), bResult->Q.qy(), bResult->Q.qz(), bResult->Q.real(),
		//	bResult->S.x(), bResult->S.y(), bResult->S.z(), bResult->S.w());
		//bResult++;

	}

	poSSBO_bWorldAll->Unmap();

}

//--- End of File -------------------------------------------------------------
