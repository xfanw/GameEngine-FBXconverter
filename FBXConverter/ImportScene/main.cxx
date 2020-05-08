/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"
#include "ModelFileHeader.h"



// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

void SaveToFile(const FbxString lFilePath);

void CreateIndex_Weight();


// Verts
static bool gVerbose = true;

#include "FBX_STL.h"
#include "FBX_VBO.h"
#include <cassert>
#include <map>

// read from FBX_DISPLAY_... File
std::vector<FBX_Vertex_vsn_Index> originalVert;	//Vert fat
std::vector<FBX_Trilist> originalTri;
//std::vector<FBX_Stack> StackList;
//std::vector<BoneHierarchy> HiList;

// setup header
Header header;
static int headerOffset = sizeof(Header);
int VBOfileSize;
int HierFileSize;
int PoseFileSize;
int SkinfileSize;
int reducedHierSize;

// vectors writing to file
// Vert and tris
std::vector<FBX_Trilist> VBO_tri;
std::vector<FBX_Vertex_vsn_Index> VBO_vert;
std::vector< FBX_Vertex_vsn>VBO_vert_out;

// Anim
std::vector<FBX_Stack> StackList;
std::vector<BoneHierarchy> HiList;

// Reduced Anim
std::vector<FBX_Stack_Reduced> reducedStackList;


// Weights
// #include <vector>
#include <algorithm>
#include "BoneWeight.h"
#include "MathEngine.h"
std::vector< BoneEntry > vBoneTable;
std::vector< VertEntry > vBoneWeightTable;
std::vector< FinalVertEntry > vFinalBoneWeightTable;
std::vector<Skin_Weight_Index> vOut_Weight_Index;
int totalNumVerts = 0;
void CreateIndex_Weight();
// pose inverse
std::vector<PoseInv> poseInvList;
std::vector<Matrix> vFinalPoseInvmatrix;

bool Vect_sort(BoneWeight const& v1, BoneWeight const& v2)
{
	if (v1.fBoneWeight > v2.fBoneWeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PoseInv_Sort(PoseInv const& p1, PoseInv const& p2) {
	if (p1.index < p2.index) {
		return true;
	}
	else {
		return false;
	}

}

int main(int argc, char** argv)
{
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	// Load the scene.

	// The example can take a FBX file as an argument.
	FbxString lFilePath("");
	for (int i = 1, c = argc; i < c; ++i)
	{
		if (FbxString(argv[i]) == "-test") gVerbose = false;
		else if (lFilePath.IsEmpty()) lFilePath = argv[i];
	}

	if (lFilePath.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}
	else
	{
		// Display the scene.
		DisplayMetaData(lScene);

		// Triangulate
		FbxGeometryConverter lGeomConverter(lSdkManager);
		// Replace = true
		lGeomConverter.Triangulate(lScene, true);

		FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

		if (gVerbose) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

		FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

		if (gVerbose) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

		FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

		if (gVerbose) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

		FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

		if (gVerbose) DisplayHierarchy(lScene);

		FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

		if (gVerbose) DisplayContent(lScene);
		CreateIndex_Weight();


		FBXSDK_printf("\n\n----\nPose\n----\n\n");

		if (gVerbose) DisplayPose(lScene);

		FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

		if (gVerbose) {
			DisplayAnimation(lScene);
		}
		//now display generic information
		// FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
		// if( gVerbose ) DisplayGenericInfo(lScene);	}





	//write to file
		SaveToFile(lFilePath);

		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(lSdkManager, lResult);
		return 0;
	}
}

void CreateIndex_Weight() {
	// weight
	printf("\n");
	printf("\n");
	printf("Original Bone centric -  \n");
	for (size_t j = 0; j < vBoneTable.size(); j++)
		//for (auto j = 0; j < 20; j++)
	{
		vBoneTable[j].PrintMe();

		for (unsigned int k = 0; k < vBoneTable[j].vVertWeight.size(); k++)
		{
			printf("      i:%d w:%f \n", vBoneTable[j].vVertWeight[k].iVertIndex, vBoneTable[j].vVertWeight[k].fBoneWeight);
		}
	}

	// Vert centric table
	vBoneWeightTable.reserve(totalNumVerts);

	for (auto j = 0; j < totalNumVerts; j++)
	{
		VertEntry v;
		vBoneWeightTable.push_back(v);
	}

	printf("\n");
	printf("\n");
	printf("\n\n Vert centric -  \n");

	for (size_t j = 0; j < vBoneTable.size(); j++)
		//for (auto j = 0; j < 20; j++)
	{
		//vBoneTable[j].PrintMe();

		for (unsigned int k = 0; k < vBoneTable[j].vVertWeight.size(); k++)
		{
			int boneIndex = j;
			int vertIndex = vBoneTable[j].vVertWeight[k].iVertIndex;
			float boneWeight = vBoneTable[j].vVertWeight[k].fBoneWeight;

			BoneWeight bw;
			bw.iBoneIndex = boneIndex;
			bw.fBoneWeight = boneWeight;

			vBoneWeightTable[vertIndex].vBoneWeight.push_back(bw);

		}
	}

	for (auto j = 0; j < totalNumVerts; j++)
	{
		printf("vert:%d\n", j);

		for (unsigned int k = 0; k < vBoneWeightTable[j].vBoneWeight.size(); k++)
		{
			printf("      bone_i:%d w:%f \n", vBoneWeightTable[j].vBoneWeight[k].iBoneIndex, vBoneWeightTable[j].vBoneWeight[k].fBoneWeight);

		}
	}

	printf("\n");
	printf("\n");
	printf(" SORTED by weight - \n");

	for (auto j = 0; j < totalNumVerts; j++)
	{
		//printf("vert:%d\n", j);
		std::sort(vBoneWeightTable[j].vBoneWeight.begin(), vBoneWeightTable[j].vBoneWeight.end(), Vect_sort);

	}

	for (auto j = 0; j < totalNumVerts; j++)
	{
		printf("vert:%d\n", j);

		for (unsigned int k = 0; k < vBoneWeightTable[j].vBoneWeight.size(); k++)
		{
			printf("      bone_i:%d w:%f \n", vBoneWeightTable[j].vBoneWeight[k].iBoneIndex, vBoneWeightTable[j].vBoneWeight[k].fBoneWeight);

		}
	}

	printf("\n");
	printf("\n");
	printf(" Final SORTED by weight \n");

	for (auto j = 0; j < totalNumVerts; j++)
	{
		BoneWeight bw;
		bw.iBoneIndex = 0;
		bw.fBoneWeight = 0.0f;

		// Add more... in case there is only one entry
		vBoneWeightTable[j].vBoneWeight.push_back(bw);
		vBoneWeightTable[j].vBoneWeight.push_back(bw);
		vBoneWeightTable[j].vBoneWeight.push_back(bw);
		vBoneWeightTable[j].vBoneWeight.push_back(bw);

		// Load it
		FinalVertEntry fbw;
		Skin_Weight_Index fout;
		fbw.A.iBoneIndex = vBoneWeightTable[j].vBoneWeight[0].iBoneIndex;
		fbw.A.fBoneWeight = vBoneWeightTable[j].vBoneWeight[0].fBoneWeight;

		fbw.B.iBoneIndex = vBoneWeightTable[j].vBoneWeight[1].iBoneIndex;
		fbw.B.fBoneWeight = vBoneWeightTable[j].vBoneWeight[1].fBoneWeight;

		fbw.C.iBoneIndex = vBoneWeightTable[j].vBoneWeight[2].iBoneIndex;
		fbw.C.fBoneWeight = vBoneWeightTable[j].vBoneWeight[2].fBoneWeight;

		fbw.D.iBoneIndex = vBoneWeightTable[j].vBoneWeight[3].iBoneIndex;
		fbw.D.fBoneWeight = vBoneWeightTable[j].vBoneWeight[3].fBoneWeight;

		// Normalize it
		float sum = fbw.A.fBoneWeight + fbw.B.fBoneWeight + fbw.C.fBoneWeight + fbw.D.fBoneWeight;

		fbw.A.fBoneWeight /= sum;
		fbw.B.fBoneWeight /= sum;
		fbw.C.fBoneWeight /= sum;
		fbw.D.fBoneWeight /= sum;

		// Frank Added
		fout.weightA = fbw.A.fBoneWeight;
		fout.weightB = fbw.B.fBoneWeight;
		fout.weightC = fbw.C.fBoneWeight;
		fout.weightD = fbw.D.fBoneWeight;

		fout.indexA = fbw.A.iBoneIndex;
		fout.indexB = fbw.B.iBoneIndex;
		fout.indexC = fbw.C.iBoneIndex;
		fout.indexD = fbw.D.iBoneIndex;

		// push back
		vFinalBoneWeightTable.push_back(fbw);
		vOut_Weight_Index.push_back(fout);

	}

	for (auto j = 0; j < totalNumVerts; j++)
	{
		float sum = vFinalBoneWeightTable[j].A.fBoneWeight
			+ vFinalBoneWeightTable[j].B.fBoneWeight
			+ vFinalBoneWeightTable[j].C.fBoneWeight
			+ vFinalBoneWeightTable[j].D.fBoneWeight;

		printf("vert:%d                     sum:%f\n", j, sum);

		printf("      bone_i:%d w:%f \n", vFinalBoneWeightTable[j].A.iBoneIndex, vFinalBoneWeightTable[j].A.fBoneWeight);
		printf("      bone_i:%d w:%f \n", vFinalBoneWeightTable[j].B.iBoneIndex, vFinalBoneWeightTable[j].B.fBoneWeight);
		printf("      bone_i:%d w:%f \n", vFinalBoneWeightTable[j].C.iBoneIndex, vFinalBoneWeightTable[j].C.fBoneWeight);
		printf("      bone_i:%d w:%f \n", vFinalBoneWeightTable[j].D.iBoneIndex, vFinalBoneWeightTable[j].D.fBoneWeight);
	}
}

void SortVBO()
{
	// 1. put old value int to a map<int, verts>
	// 2. vector sort()
	// 3. put sorted value into map<verts, int> 
	// 3.1 if map contains verts:  do nothing
	// 3.2 map does not contain verts: put to map, increment int put to out Verts

	// 4. based on old tri get new tri
	// iterate old tri
	// 4.1 old tri-> old map -> vert
	// 4.2 vert -> newmap -> new tri

	// 1.
	std::map<int, FBX_Vertex_vsn_Index> oldVerts;
	int size = originalVert.size();
	for (int i = 0; i < size; i++) {
		oldVerts.insert(std::pair<int, FBX_Vertex_vsn_Index>(i, originalVert[i]));
	}

	// 2.
	std::vector<FBX_Vertex_vsn_Index> sortedVerts(originalVert);
	std::sort(sortedVerts.begin(), sortedVerts.end());

	// 3.
	std::map< FBX_Vertex_vsn_Index, int> newIntMap;
	int newId = 0;
	for (int i = 0; i < size; i++) {
		FBX_Vertex_vsn_Index tmpVerts = sortedVerts[i];
		if (newIntMap.find(tmpVerts) == newIntMap.end()) {
			newIntMap.insert(std::pair<FBX_Vertex_vsn_Index, int>(tmpVerts, newId));
			VBO_vert.push_back(tmpVerts);
			newId++;
		}
	}
	//assert((newIntMap.find(VBO_vert[0])->second) == 0);
	//assert((newIntMap.find(VBO_vert[100])->second) == 100);
	// FRANK Add 3.5 from index to no_index
	size = VBO_vert.size();
	printf("\nFRANK1 VERIFT\n");
	for (int i = 0; i < size; i++) {
		FBX_Vertex_vsn_Index tmpVerts = VBO_vert[i];
		FBX_Vertex_vsn tmp = FBX_Vertex_vsn(tmpVerts);

		VBO_vert_out.push_back(tmp);
		printf("vbo_vert[%d]  : v: %f %f %f s: %f %f n:%f %f %f \n",
			i, tmp.x, tmp.y, tmp.z, tmp.s, tmp.t, tmp.nx, tmp.ny, tmp.nz);
	}
	// 4.
	size = originalTri.size();
	for (int i = 0; i < size; i++) {
		FBX_Trilist inTmp = originalTri[i];
		FBX_Trilist outTmp;
		outTmp.v0 = newIntMap.find(oldVerts.find(inTmp.v0)->second)->second;
		outTmp.v1 = newIntMap.find(oldVerts.find(inTmp.v1)->second)->second;;
		outTmp.v2 = newIntMap.find(oldVerts.find(inTmp.v2)->second)->second;;
		VBO_tri.push_back(outTmp);
	}

	//assert(VBO_tri.size() == originalTri.size());
	//assert(VBO_vert[VBO_tri[0].v0] == originalVert[originalTri[0].v0]);
	//assert(VBO_vert[VBO_tri[100].v2] == originalVert[originalTri[100].v2]);
}
void NotSortVBO() {
	VBO_vert = originalVert;
	VBO_tri = originalTri;
}
unsigned char* SaveVBO()
{
	VBOHeader vboHeader;
	strcpy_s(vboHeader.objName, header.objName);

	SortVBO();
	//NotSortVBO();
	//assert(pFileHandle != 0);
	int vboCount = VBO_vert_out.size();
	int triCount = VBO_tri.size();
	VBOfileSize = sizeof(VBOHeader) + vboCount * sizeof(FBX_Vertex_vsn) + triCount * sizeof(FBX_Trilist);

	vboHeader.numVerts = vboCount;
	vboHeader.numTriList = triCount;
	vboHeader.vertBufferOffset = sizeof(VBOHeader);
	vboHeader.triListBufferOffset = sizeof(VBOHeader) + vboHeader.numVerts * sizeof(FBX_Vertex_vsn);

	unsigned char* buffer = new unsigned char[VBOfileSize];
	unsigned char* bufferPtr = buffer;

	// copy Header
	memcpy_s(bufferPtr, sizeof(VBOHeader), &vboHeader, sizeof(VBOHeader));
	// verify and put to console or.txt file
	// VBOHeader* tmp = (VBOHeader*)bufferPtr;

	//copy verts
	bufferPtr += sizeof(VBOHeader);
	printf("FRANK_VERIFY VBO_Verts\n");
	for (int i = 0; i < vboCount; i++, bufferPtr += sizeof(FBX_Vertex_vsn))
	{
		memcpy_s(bufferPtr, sizeof(FBX_Vertex_vsn), &VBO_vert_out[i], sizeof(FBX_Vertex_vsn));
		// verify and put to console or.txt file
		FBX_Vertex_vsn* vtmp = (FBX_Vertex_vsn*)bufferPtr;
		printf("vbo_vert[%d]: v: %f %f %f s: %f %f n:%f %f %f \n",
			i, vtmp->x, vtmp->y, vtmp->z, vtmp->s, vtmp->t, vtmp->nx, vtmp->ny, vtmp->nz);
	}
	//printf("\n\n");

	//copy tri
	printf("FRANK_VERIFY VBO_Tris\n");
	for (int i = 0; i < triCount; i++, bufferPtr += sizeof(FBX_Trilist))
	{
		memcpy_s(bufferPtr, sizeof(FBX_Trilist), &VBO_tri[i], sizeof(FBX_Trilist));
		// verify and put to console or.txt file
		FBX_Trilist* vtmp = (FBX_Trilist*)bufferPtr;
		printf("VBO_tri[%d]: %d %d %d\n", i, vtmp->v0, vtmp->v1, vtmp->v2);
	}
	//printf("\n\n");

	//fwrite(buffer, fileSize, 1, pFileHandle);
	//delete[] buffer;
	header.VBOheaderOffset = headerOffset;
	headerOffset += VBOfileSize;
	return buffer;

}

unsigned char* SaveBonePose()
{
	// START writing frame and bone pose
	AnimHeader animHeader;
	int sCount = StackList.size();
	animHeader.numStacks = sCount;
	//assert(animHeader.numStacks == 3);

	// all animation has same number of bones
	animHeader.bonesPerFrame = StackList[0].boneCount;
	//assert(animHeader.bonesPerFrame == 20);

	// offset
	animHeader.stackOffset = sizeof(AnimHeader);
	int frameOffset = animHeader.stackOffset + sizeof(OUT_STACK) * sCount;
	animHeader.frameOffset = frameOffset;
	int boneOffSet = 0;
	boneOffSet += frameOffset;	// set up later


	// Set up arry to write to file
	std::vector<OUT_STACK> stacksToWrite;

	stacksToWrite.reserve(sCount);
	int totalFramCounts = 0;
	int totalBoneCounts = 0;


	//iterate the stack_list
	for (int i = 0; i < sCount; i++) {
		// set up out header
		//StackList[i].reduce();
		FBX_Stack tmpStack = StackList[i];


		int fCount = tmpStack.frameCount;
		animHeader.FramesPerStack[i] = fCount;

		//setup out stack array
		OUT_STACK outStack;
		outStack.frameCount = fCount;

		// get size to reserve
		totalFramCounts += fCount;
		// name 
		memcpy_s(outStack.name, 16, tmpStack.name, 16);
		stacksToWrite.push_back(outStack);

		//calc boneOffset
		boneOffSet += fCount * sizeof(OUT_FRAME);
	}
	//setup boneOffset
	animHeader.boneOffset = boneOffSet;

	// each fram has same bone nums
	int eachFrameSize = animHeader.bonesPerFrame * sizeof(OUT_BONE_POSE);
	totalBoneCounts = animHeader.bonesPerFrame * totalFramCounts;

	std::vector<OUT_FRAME> framesToWrite;
	std::vector<OUT_BONE_POSE> bonesToWrite;
	framesToWrite.reserve(totalFramCounts);
	bonesToWrite.reserve(totalBoneCounts);

	for (int i = 0; i < sCount; i++) {
		FBX_Stack tmpStack = StackList[i];
		//write each frame to frame list
		int fCount = tmpStack.frameCount;
		for (int j = 0; j < fCount; j++) {
			FBX_Frame tmpFrame = tmpStack.StackFrameList[j];
			OUT_FRAME outFrame;
			outFrame.KeyTime = tmpFrame.KeyTime;
			//set up pointers
			if (j == 0) {
				//deal with first node
				outFrame.prevBucket = 0;
				outFrame.nextBucket = (OUT_FRAME*)((j + 1) * sizeof(OUT_FRAME));
			}
			else if (j == (fCount - 1)) {
				// deal with last node
				outFrame.prevBucket = (OUT_FRAME*)((j - 1) * sizeof(OUT_FRAME));
				outFrame.nextBucket = 0;
			}
			else {
				// normal node
				outFrame.prevBucket = (OUT_FRAME*)((j - 1) * sizeof(OUT_FRAME));
				outFrame.nextBucket = (OUT_FRAME*)((j + 1) * sizeof(OUT_FRAME));
			}


			outFrame.pBone = (OUT_BONE_POSE*)boneOffSet;
			boneOffSet += eachFrameSize;


			framesToWrite.push_back(outFrame);

			// write bone to bone list
			for (int k = 0; k < animHeader.bonesPerFrame; k++) {
				OUT_BONE_POSE outBone;
				FBX_Bone_Pose tmpBone = tmpFrame.FrameBoneList[k];
				outBone.T = tmpBone.T;
				outBone.Q = tmpBone.Q;
				outBone.S = tmpBone.S;
				bonesToWrite.push_back(outBone);
			}//end bone
		}//end frame
		frameOffset += fCount * sizeof(OUT_FRAME);
	}//end stack
	// make sure size are good
	//assert(bonesToWrite.size() == totalBoneCounts);
	//assert(framesToWrite.size() == totalFramCounts);
	//assert(animHeader.FramesPerStack[0] == 41);
	//assert(animHeader.FramesPerStack[1] == 139);
	//assert(animHeader.FramesPerStack[2] == 150);
	//assert(animHeader.boneOffset == (sizeof(AnimHeader) + sCount * sizeof(OUT_STACK) + (41 + 139 + 150) * sizeof(OUT_FRAME)));
	//assert((int)(framesToWrite[200].pBone) == (animHeader.boneOffset + 200 * eachFrameSize));
	//make sure pointers are good


	//assert(framesToWrite[200].nextBucket == framesToWrite[202].prevBucket);

	//assert(framesToWrite[40].nextBucket == 0);
	//assert(framesToWrite[41].prevBucket == 0);
	//assert(framesToWrite[179].nextBucket == 0);
	//assert(framesToWrite[180].prevBucket == 0);
	//assert(framesToWrite[329].nextBucket == 0);
	//assert(framesToWrite[0].KeyTime == 0);
	//assert(framesToWrite[20].KeyTime == 20);
	//assert(framesToWrite[40].KeyTime == 40);
	//assert(framesToWrite[41].KeyTime == 0);
	//assert(framesToWrite[180].KeyTime == 0);
	// write to binary file
	PoseFileSize = sizeof(AnimHeader)
		+ stacksToWrite.size() * sizeof(OUT_STACK)
		+ framesToWrite.size() * sizeof(OUT_FRAME)
		+ bonesToWrite.size() * sizeof(OUT_BONE_POSE);
	unsigned char* buffer = new unsigned char[PoseFileSize];
	unsigned char* bufferPtr = buffer;
	// copy Header
	memcpy_s(bufferPtr, sizeof(AnimHeader), &animHeader, sizeof(AnimHeader));
	bufferPtr += sizeof(AnimHeader);
	printf("FRANK_VERIFY STACK\n");
	//copy stacks
	for (int i = 0; i < sCount; i++, bufferPtr += sizeof(OUT_STACK)) {
		memcpy_s(bufferPtr, sizeof(OUT_STACK), &stacksToWrite[i], sizeof(OUT_STACK));
		// verify and put to console or.txt file
		OUT_STACK* tmp = (OUT_STACK*)bufferPtr;
		printf("OUT_STACK[%d]: %s %d \n", i, tmp->name, tmp->frameCount);
	}
	printf("FRANK_VERIFY FRAME\n");
	//copy frames
	for (int i = 0; i < totalFramCounts; i++, bufferPtr += sizeof(OUT_FRAME)) {
		memcpy_s(bufferPtr, sizeof(OUT_FRAME), &framesToWrite[i], sizeof(OUT_FRAME));
		OUT_FRAME* tmp = (OUT_FRAME*)bufferPtr;
		printf("OUT_FRAME[%d]: prev:%d next%d T:%d pB:%d\n", i, (int)tmp->prevBucket, (int)tmp->nextBucket, tmp->KeyTime, (int)tmp->pBone);
	}

	//copy bones
	printf("FRANK_VERIFY BONEPOSE\n");
	for (int i = 0; i < totalBoneCounts; i++, bufferPtr += sizeof(OUT_BONE_POSE)) {
		memcpy_s(bufferPtr, sizeof(OUT_BONE_POSE), &bonesToWrite[i], sizeof(OUT_BONE_POSE));
		OUT_BONE_POSE* tmp = (OUT_BONE_POSE*)bufferPtr;
		printf("OUT_BONE_POSE[%d]: T:%f %f %f \n\t\t Q:%f %f %f \n\t\t S:%f %f %f \n", i,
			tmp->T[x], tmp->T[y], tmp->T[z], tmp->Q[x], tmp->Q[y], tmp->Q[z], tmp->S[x], tmp->S[y], tmp->S[z]);
	}

	header.AnimHeaderOffset = headerOffset;
	headerOffset += PoseFileSize;
	return buffer;
}

unsigned char* SaveHierarchy() {
	HierarchyHeader hHeader;
	int n = HiList.size();
	hHeader.numBones = n;
	hHeader.offset = sizeof(HierarchyHeader);
	// write to binary file
	HierFileSize = sizeof(HierarchyHeader) + HiList.size() * sizeof(BoneHierarchy);
	unsigned char* buffer = new unsigned char[HierFileSize];
	unsigned char* bufferPtr = buffer;

	// copy Header
	memcpy_s(bufferPtr, sizeof(HierarchyHeader), &hHeader, sizeof(HierarchyHeader));
	bufferPtr += sizeof(HierarchyHeader);
	printf("FRANK_VERIFY HIERARCHY\n");
	//copy Hierarchys
	for (int i = 0; i < n; i++, bufferPtr += sizeof(BoneHierarchy)) {
		memcpy_s(bufferPtr, sizeof(BoneHierarchy), &HiList[i], sizeof(BoneHierarchy));
		// verify and put to console or.txt file
		BoneHierarchy* tmp = (BoneHierarchy*)bufferPtr;
		printf("HIER[%d] Depth %d: this: %s\t parent:%s\n", tmp->index, tmp->depth, tmp->name, tmp->parentName);
	}

	header.HierarchyHeaderOffset = headerOffset;
	headerOffset += HierFileSize;

	return buffer;
}


unsigned char* SaveIndexPose() {

	SkinHeader skinHeader;
	strcpy_s(skinHeader.objName, header.objName);

	std::sort(poseInvList.begin(), poseInvList.end(), PoseInv_Sort);


	printf("sorted pose Inv Frank\n");
	for (int i = 0; i < poseInvList.size(); i++) {
		printf("\n%d Pose Inverse matrix [%d]:\n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f\n",
			i, poseInvList[i].index,
			poseInvList[i].mInv.get(Matrix::Row::i0)[x], poseInvList[i].mInv.get(Matrix::Row::i0)[y], poseInvList[i].mInv.get(Matrix::Row::i0)[z], poseInvList[i].mInv.get(Matrix::Row::i0)[w],
			poseInvList[i].mInv.get(Matrix::Row::i1)[x], poseInvList[i].mInv.get(Matrix::Row::i1)[y], poseInvList[i].mInv.get(Matrix::Row::i1)[z], poseInvList[i].mInv.get(Matrix::Row::i1)[w],
			poseInvList[i].mInv.get(Matrix::Row::i2)[x], poseInvList[i].mInv.get(Matrix::Row::i2)[y], poseInvList[i].mInv.get(Matrix::Row::i2)[z], poseInvList[i].mInv.get(Matrix::Row::i2)[w],
			poseInvList[i].mInv.get(Matrix::Row::i3)[x], poseInvList[i].mInv.get(Matrix::Row::i3)[y], poseInvList[i].mInv.get(Matrix::Row::i3)[z], poseInvList[i].mInv.get(Matrix::Row::i3)[w]
		);
	}
	int size = vBoneTable.size();
	vFinalPoseInvmatrix.reserve(size);
	int  j = 0;
	int oldSize = poseInvList.size();
	for (int i = 0; i < size; i++) {

		if (j < oldSize && poseInvList[j].index == i) {
			vFinalPoseInvmatrix.push_back(poseInvList[j].mInv);
			j++;
		}
		else {
			vFinalPoseInvmatrix.push_back(Matrix());
		}
	}
	// Verify
	for (int i = 0; i < size; i++) {

		printf("\nPose Inverse matrix [%d]:\n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f\n",
			i,
			vFinalPoseInvmatrix[i].get(Matrix::Row::i0)[x], vFinalPoseInvmatrix[i].get(Matrix::Row::i0)[y], vFinalPoseInvmatrix[i].get(Matrix::Row::i0)[z], vFinalPoseInvmatrix[i].get(Matrix::Row::i0)[w],
			vFinalPoseInvmatrix[i].get(Matrix::Row::i1)[x], vFinalPoseInvmatrix[i].get(Matrix::Row::i1)[y], vFinalPoseInvmatrix[i].get(Matrix::Row::i1)[z], vFinalPoseInvmatrix[i].get(Matrix::Row::i1)[w],
			vFinalPoseInvmatrix[i].get(Matrix::Row::i2)[x], vFinalPoseInvmatrix[i].get(Matrix::Row::i2)[y], vFinalPoseInvmatrix[i].get(Matrix::Row::i2)[z], vFinalPoseInvmatrix[i].get(Matrix::Row::i2)[w],
			vFinalPoseInvmatrix[i].get(Matrix::Row::i3)[x], vFinalPoseInvmatrix[i].get(Matrix::Row::i3)[y], vFinalPoseInvmatrix[i].get(Matrix::Row::i3)[z], vFinalPoseInvmatrix[i].get(Matrix::Row::i3)[w]
		);
	}
	int poseCount = vFinalPoseInvmatrix.size();
	int indexWeightCount = vFinalBoneWeightTable.size();
	SkinfileSize = sizeof(SkinHeader) + poseCount * sizeof(Matrix) + indexWeightCount * sizeof(Skin_Weight_Index);

	skinHeader.numPoseInv = poseCount;
	skinHeader.numVerts = indexWeightCount;

	skinHeader.poseInvOffset = sizeof(SkinHeader);
	skinHeader.vertWeightListOffset = sizeof(SkinHeader) + poseCount * sizeof(Matrix);

	unsigned char* buffer = new unsigned char[SkinfileSize];
	unsigned char* bufferPtr = buffer;

	// copy Header
	memcpy_s(bufferPtr, sizeof(SkinHeader), &skinHeader, sizeof(SkinHeader));

	//copy verts
	bufferPtr += sizeof(SkinHeader);
	printf("FRANK_VERIFY InvPose\n");
	for (int i = 0; i < poseCount; i++, bufferPtr += sizeof(Matrix))
	{
		memcpy_s(bufferPtr, sizeof(Matrix), &vFinalPoseInvmatrix[i], sizeof(Matrix));

		// verify and put to console or.txt file
		Matrix* vtmp = (Matrix*)bufferPtr;
		printf("Inverse matrix In Memory [%d]:\n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f \n\t %f %f %f %f\n",
			i,
			vtmp->get(Matrix::Row::i0)[x], vtmp->get(Matrix::Row::i0)[y], vtmp->get(Matrix::Row::i0)[z], vtmp->get(Matrix::Row::i0)[w],
			vtmp->get(Matrix::Row::i1)[x], vtmp->get(Matrix::Row::i1)[y], vtmp->get(Matrix::Row::i1)[z], vtmp->get(Matrix::Row::i1)[w],
			vtmp->get(Matrix::Row::i2)[x], vtmp->get(Matrix::Row::i2)[y], vtmp->get(Matrix::Row::i2)[z], vtmp->get(Matrix::Row::i2)[w],
			vtmp->get(Matrix::Row::i3)[x], vtmp->get(Matrix::Row::i3)[y], vtmp->get(Matrix::Row::i3)[z], vtmp->get(Matrix::Row::i3)[w]
		);
	}
	//printf("\n\n");

	//copy tri
	printf("FRANK_VERIFY WEIGHT&INDEX\n");
	for (int i = 0; i < indexWeightCount; i++, bufferPtr += sizeof(Skin_Weight_Index))
	{
		memcpy_s(bufferPtr, sizeof(Skin_Weight_Index), &vOut_Weight_Index[i], sizeof(Skin_Weight_Index));

		// verify and put to console or.txt file
		Skin_Weight_Index* vtmp = (Skin_Weight_Index*)bufferPtr;
		printf("Weight&Index[%d]: \n %d: %f\n %d: %f\n %d: %f\n %d: %f\n",
			i, vtmp->indexA, vtmp->weightA, vtmp->indexB, vtmp->weightB,
			vtmp->indexC, vtmp->weightC, vtmp->indexD, vtmp->weightD);
	}
	printf("\n\n");

	//fwrite(buffer, fileSize, 1, pFileHandle);
	//delete[] buffer;
	header.Skin_Vert_Offset = headerOffset;
	headerOffset += SkinfileSize;
	return buffer;
}
unsigned char* SaveReducedBonePose() {
	Reduced_AnimHeader reducedAnimHeader;
	// START writing frame and bone pose
	int sCount = StackList.size();
	reducedAnimHeader.numStacks = sCount;
	reducedAnimHeader.bonesPerFrame = StackList[0].boneCount;

	for (int i = 0; i < sCount; i++) {
		StackList[i].reduce();
		reducedStackList.push_back(FBX_Stack_Reduced(StackList[i]));
	}


	// offset
	reducedAnimHeader.stackOffset = sizeof(Reduced_AnimHeader);
	//Frame offset = stack head + (size of stack + size of 79 T vect) * stack count
	int inFrameOffset = reducedAnimHeader.stackOffset + sizeof(OUT_STACK_Reduced) * sCount; // setup inside pointer
	int frameOffset = reducedAnimHeader.stackOffset + (sizeof(OUT_STACK_Reduced) + sizeof(Vect) * (reducedAnimHeader.bonesPerFrame - 1)) * sCount;
	reducedAnimHeader.frameOffset = frameOffset;
	// bone offset will be calculated later
	int boneOffSet = 0;
	boneOffSet += frameOffset;	// set up later


	// Set up arry to write to file
	std::vector<OUT_STACK_Reduced> stacksToWrite;
	std::vector<Vect> stackTs;
	stacksToWrite.reserve(sCount);
	int totalFramCounts = 0;
	int totalBoneCounts = 0;

	//iterate the stack_list
	for (int i = 0; i < sCount; i++) {

		FBX_Stack_Reduced tmpStack = reducedStackList[i];


		int fCount = tmpStack.frameCount;
		reducedAnimHeader.FramesPerStack[i] = fCount;

		//setup out stack array
		OUT_STACK_Reduced outStack;
		outStack.frameCount = fCount;

		for (int j = 0; j < reducedAnimHeader.bonesPerFrame - 1; j++) {
			stackTs.push_back(tmpStack.restBoneT[j]);
		}

		// get size to reserve
		totalFramCounts += fCount;

		stacksToWrite.push_back(outStack);

		//calc boneOffset
		boneOffSet += fCount * sizeof(OUT_FRAME_Reduced);
	}
	//setup boneOffset
	reducedAnimHeader.boneOffset = boneOffSet;

	// each fram has same bone nums
	int eachFrameSize = (reducedAnimHeader.bonesPerFrame - 1) * sizeof(Quat);

	// set up total bone counts
	totalBoneCounts = (reducedAnimHeader.bonesPerFrame - 1) * totalFramCounts;

	std::vector<OUT_FRAME_Reduced> framesToWrite;
	std::vector<Quat> bonesToWrite;


	// Set up data
	for (int i = 0; i < sCount; i++) {
		FBX_Stack_Reduced tmpStack = reducedStackList[i];
		//write each frame to frame list
		int fCount = tmpStack.frameCount;
		for (int j = 0; j < fCount; j++) {
			FBX_Frame_Reduced tmpFrame = tmpStack.reducedFrameList[j];
			OUT_FRAME_Reduced outFrame;
			outFrame.KeyTime = tmpFrame.KeyTime;
			outFrame.pFirstBone = tmpFrame.firstBone;

			framesToWrite.push_back(outFrame);

			// write bone to bone list
			for (int k = 0; k < reducedAnimHeader.bonesPerFrame - 1; k++) {
				Quat outBoneQ = tmpFrame.restBoneQ[k];

				bonesToWrite.push_back(outBoneQ);
			}//end bone
		}//end frame
	}//end stack

	// Finally Total file size
	reducedHierSize = sizeof(reducedAnimHeader)
		+ stacksToWrite.size() * sizeof(OUT_STACK_Reduced)
		+ stackTs.size() * sizeof(Vect)
		+ framesToWrite.size() * sizeof(OUT_FRAME_Reduced)
		+ bonesToWrite.size() * sizeof(Quat);

	unsigned char* buffer = new unsigned char[PoseFileSize];
	unsigned char* bufferPtr = buffer;
	// copy Header
	memcpy_s(bufferPtr, sizeof(reducedAnimHeader), &reducedAnimHeader, sizeof(reducedAnimHeader));
	bufferPtr += sizeof(reducedAnimHeader);

	printf("FRANK_VERIFY Reduced STACK\n");
	//copy stacks
	for (int i = 0; i < sCount; i++, bufferPtr += sizeof(OUT_STACK_Reduced)) {
		memcpy_s(bufferPtr, sizeof(OUT_STACK_Reduced), &stacksToWrite[i], sizeof(OUT_STACK_Reduced));
		// verify and put to console or.txt file
		OUT_STACK_Reduced* tmp = (OUT_STACK_Reduced*)bufferPtr;
		printf("OUT_STACK[%d]: %d \n", i,  tmp->frameCount);
	}

	// copy const Ts
	printf("FRANK_VERIFY Reduced Vect\n");
	int vectSize = stackTs.size();
	for (int i = 0; i < vectSize; i++, bufferPtr += sizeof(Vect)) {
		memcpy_s(bufferPtr, sizeof(Vect), &stackTs[i], sizeof(Vect));
		// verify and put to console or.txt file
		Vect* tmp = (Vect*)bufferPtr;
		printf("Vect[%d]: T: %f %f %f %f \n", i, tmp->x(), tmp->y(), tmp->z(), tmp->w());
	}

	printf("FRANK_VERIFY Reduced FRAME\n");
	//copy frames
	for (int i = 0; i < totalFramCounts; i++, bufferPtr += sizeof(OUT_FRAME_Reduced)) {
		memcpy_s(bufferPtr, sizeof(OUT_FRAME_Reduced), &framesToWrite[i], sizeof(OUT_FRAME_Reduced));
		OUT_FRAME_Reduced* tmp = (OUT_FRAME_Reduced*)bufferPtr;
		printf("OUT_FRAME_Reduced[%d]:T:%d\n", i,  tmp->KeyTime);
	}

	//copy bones
	printf("FRANK_VERIFY Reduced BONEPOSE\n");
	for (int i = 0; i < totalBoneCounts; i++, bufferPtr += sizeof(Quat)) {
		memcpy_s(bufferPtr, sizeof(Quat), &bonesToWrite[i], sizeof(Quat));
		Quat* tmp = (Quat*)bufferPtr;
		printf("Quat[%d]:  Q:%f %f %f %f\n", i,
			tmp->qx(), tmp->qy(), tmp->qz(), tmp->real());
	}

	header.ReducedHierOffset = headerOffset;
	headerOffset += reducedHierSize;
	return buffer;
}
void SaveToFile(FbxString  lFilePath) {
	// get output file name
	//copy input mane without .fbx
	size_t inputLen = strlen(lFilePath);
	const char* append = ".azul";
	char* outFile = (char*)malloc(sizeof(char) * (inputLen + 2));
	if (outFile) {
		unsigned int i = 0;
		for (; i < inputLen - 4; i++) {
			outFile[i] = lFilePath[i];
		}
		outFile[i] = '\0';
		strcpy_s(header.objName, outFile);

		// add .azul
		for (; i < strlen(append) + inputLen - 4; i++) {
			outFile[i] = append[i - inputLen + 4];
		}
		outFile[i] = '\0';

		FILE* pFileHandle = fopen(outFile, "wb");

		unsigned char* vboBuffer = SaveVBO();

		unsigned char* poseBuffer = SaveBonePose();

		unsigned char* HierBuffer = SaveHierarchy();

		unsigned char* IndexWeightBuffer = SaveIndexPose();

		unsigned char* reducedHierBuffer = SaveReducedBonePose();

		// Frank Check File Size
		int totalFileSize = VBOfileSize + HierFileSize + PoseFileSize + SkinfileSize + reducedHierSize;
		int originalSize = VBOfileSize + HierFileSize + PoseFileSize + SkinfileSize;
		int reducedSize = VBOfileSize + HierFileSize + SkinfileSize + reducedHierSize;
		printf("\n\nFileSize: %d\n\t\t VBOSize(Verts + Tris) %d\n\t\t HierarchySize(Smallest) %d \n\t\t PoseSize(Stacks, frames, boneTQS) %d\n\t\t SkinSize(invPose + WeightIndex) %d\n\t\t ReducedPose: %d\n",
			totalFileSize, VBOfileSize, HierFileSize, PoseFileSize, SkinfileSize, reducedHierSize
		);
		printf("Original vs Reduced Ratio: \n\t\t Whole file: %1.3f \treduced: %2.2f%%\n\t\t Only Animation: %1.3f \treduce:%2.2f%%\n",
			(float)originalSize/(float)reducedSize, 100 * ((float)originalSize - (float)reducedSize)/ (float)originalSize, 
			(float)PoseFileSize / (float)reducedHierSize, 100 * ((float)(PoseFileSize - reducedHierSize))/ (float)PoseFileSize
		);

		fwrite(&header, sizeof(Header), 1, pFileHandle);

		fwrite(vboBuffer, VBOfileSize, 1, pFileHandle);
		delete[] vboBuffer;

		fwrite(poseBuffer, PoseFileSize, 1, pFileHandle);
		delete[] poseBuffer;

		fwrite(HierBuffer, HierFileSize, 1, pFileHandle);
		delete[] HierBuffer;

		fwrite(IndexWeightBuffer, SkinfileSize, 1, pFileHandle);
		delete[] IndexWeightBuffer;


		fwrite(reducedHierBuffer, reducedHierSize, 1, pFileHandle);
		delete[] reducedHierBuffer;

		fclose(pFileHandle);
		free(outFile);
	}

}

void DisplayContent(FbxScene* pScene)
{
	int i;
	FbxNode* lNode = pScene->GetRootNode();

	if (lNode)
	{
		for (i = 0; i < lNode->GetChildCount(); i++)
		{
			DisplayContent(lNode->GetChild(i));
		}
	}
}

void DisplayContent(FbxNode* pNode)
{
	FbxNodeAttribute::EType lAttributeType;
	int i;

	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL Node Attribute\n\n");
	}
	else
	{
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		default:
			break;
		case FbxNodeAttribute::eMarker:
			DisplayMarker(pNode);
			break;

		case FbxNodeAttribute::eSkeleton:
			DisplaySkeleton(pNode);
			break;

		case FbxNodeAttribute::eMesh:
			DisplayMesh(pNode);
			break;

		case FbxNodeAttribute::eNurbs:
			DisplayNurb(pNode);
			break;

		case FbxNodeAttribute::ePatch:
			DisplayPatch(pNode);
			break;

		case FbxNodeAttribute::eCamera:
			DisplayCamera(pNode);
			break;

		case FbxNodeAttribute::eLight:
			DisplayLight(pNode);
			break;

		case FbxNodeAttribute::eLODGroup:
			DisplayLodGroup(pNode);
			break;
		}
	}

	DisplayUserProperties(pNode);
	DisplayTarget(pNode);
	DisplayPivotsAndLimits(pNode);
	DisplayTransformPropagation(pNode);
	DisplayGeometricTransform(pNode);

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}

void DisplayTarget(FbxNode* pNode)
{
	if (pNode->GetTarget() != NULL)
	{
		DisplayString("    Target Name: ", (char*)pNode->GetTarget()->GetName());
	}
}

void DisplayTransformPropagation(FbxNode* pNode)
{
	FBXSDK_printf("    Transformation Propagation\n");

	// 
	// Rotation Space
	//
	EFbxRotationOrder lRotationOrder;
	pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

	FBXSDK_printf("        Rotation Space: ");

	switch (lRotationOrder)
	{
	case eEulerXYZ:
		FBXSDK_printf("Euler XYZ\n");
		break;
	case eEulerXZY:
		FBXSDK_printf("Euler XZY\n");
		break;
	case eEulerYZX:
		FBXSDK_printf("Euler YZX\n");
		break;
	case eEulerYXZ:
		FBXSDK_printf("Euler YXZ\n");
		break;
	case eEulerZXY:
		FBXSDK_printf("Euler ZXY\n");
		break;
	case eEulerZYX:
		FBXSDK_printf("Euler ZYX\n");
		break;
	case eSphericXYZ:
		FBXSDK_printf("Spheric XYZ\n");
		break;
	}

	//
	// Use the Rotation space only for the limits
	// (keep using eEulerXYZ for the rest)
	//
	FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
		pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


	//
	// Inherit Type
	//
	FbxTransform::EInheritType lInheritType;
	pNode->GetTransformationInheritType(lInheritType);

	FBXSDK_printf("        Transformation Inheritance: ");

	switch (lInheritType)
	{
	case FbxTransform::eInheritRrSs:
		FBXSDK_printf("RrSs\n");
		break;
	case FbxTransform::eInheritRSrs:
		FBXSDK_printf("RSrs\n");
		break;
	case FbxTransform::eInheritRrs:
		FBXSDK_printf("Rrs\n");
		break;
	}
}

void DisplayGeometricTransform(FbxNode* pNode)
{
	FBX_UNUSED(pNode);



	FbxVector4 lTmpVector;

	FBXSDK_printf("    Geometric Transformations\n");

	//
	// Translation
	//
	lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Rotation
	//
	lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Scaling
	//
	lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	// (7)


}

void DisplayMetaData(FbxScene* pScene)
{
	FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo)
	{
		FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
		FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
		FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
		FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
		FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
		FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

		FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			FBXSDK_printf("    Thumbnail:\n");

			switch (thumbnail->GetDataFormat())
			{
			case FbxThumbnail::eRGB_24:
				FBXSDK_printf("        Format: RGB\n");
				break;
			case FbxThumbnail::eRGBA_32:
				FBXSDK_printf("        Format: RGBA\n");
				break;
			}

			switch (thumbnail->GetSize())
			{
			default:
				break;
			case FbxThumbnail::eNotSet:
				FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e64x64:
				FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e128x128:
				FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
			}
		}
	}
}



