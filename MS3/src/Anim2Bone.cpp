#include <assert.h>

#include "PyramidModel.h"
#include "GraphicsObject_TextureLight.h"
#include "GameObjectMan.h"
#include "GameObjectAnim.h"
#include "Anim.h"

#include "ShaderObject.h"
#include "PCSTreeForwardIterator.h"

#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "ModelMan.h"
#include "ShaderMan.h"
void Anim::SetAnimationHierarchy( HierarchyHeader* hireHeader)
{
	// Load the model
	PyramidModel* pPyramidModel = new PyramidModel("pyramidModel.azul");
	assert(pPyramidModel);
	ModelMan::Add(Model::NAME::PYRAMID, pPyramidModel);

	// Create/Load Shader 
	ShaderObject* pShaderObject_textureLight = new ShaderObject(ShaderObject::NAME::TEXTURE_POINT_LIGHT, "texturePointLightDiff", ShaderObject::TYPE::VERTEX);
	assert(pShaderObject_textureLight);
	ShaderMan::Add(pShaderObject_textureLight);

	// GraphicsObject for a specific instance
	GraphicsObject_TextureLight* pGraphics_TextureLight;

	// Create GameObject
	Vect color;
	Vect pos(1, 1, 1);
	//GameObjectMan::SetCurrent(name);
	PCSTree* pTree = GameObjectMan::GetPCSTree();
	root = pTree->GetRoot();

	// Rigid object
	NullModel* pNullModel = new NullModel(0);
	GraphicsObject_Null* pGraphicsObjNull = new GraphicsObject_Null(pNullModel, pShaderObject_textureLight);
	pGameRigid = new GameObjectRigid(pGraphicsObjNull);
	pGameRigid->SetName("Rigid");
	ModelMan::Add(Model::NAME::NULLMODEL, pNullModel);
	GameObjectMan::Add(pGameRigid, GameObjectMan::GetRoot());

	// Teddy  T_013.txt
	color.set(1.0f, 0.0f, 0.0f, 1.0f);

	BoneHierarchy* boneHier = (BoneHierarchy*)((unsigned char*)hireHeader + hireHeader->Hieroffset);
	this->pHierTable = boneHier;
	BoneNums = hireHeader->numBones;

	int off_set = 0;
	if (BoneNums == NUMS_BONE_BEAR) {
		Matrix Ry = Matrix(Matrix::Rot1::Y, - MATH_PI2);
		Matrix Rz = Matrix(Matrix::Rot1::Z, - 1.7f);
		Matrix S = Matrix(Matrix::Scale::XYZ, 2, 2, 2);
		Matrix M =S* Ry * Rz;
		// Matrix M = Matrix(Matrix::Special::Identity);
		*pGameRigid->poWorld = M;
		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim* pBip01 = new GameObjectAnim(pGraphics_TextureLight);
		pBip01->SetIndex(0);
		pBip01->SetName("Bip01");
		pBip01->SetAnimName("Bip01");
		pBip01->SetDrawEnable(false);  // Suppress visual on root bone skeleton
		GameObjectMan::Add(pBip01, pGameRigid);

		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim* pBip01_Pelvis = new GameObjectAnim(pGraphics_TextureLight);
		pBip01_Pelvis->SetIndex(1);
		pBip01_Pelvis->SetName("Bip01_Pelvis");
		pBip01_Pelvis->SetAnimName("Bip01_Pelvis");
		pBip01_Pelvis->SetDrawEnable(false);  // Suppress visual on root bone skeleton
		GameObjectMan::Add(pBip01_Pelvis, pBip01);
		off_set = 2;
		boneHier += off_set;
		pFirstBone = pBip01;
	}
	if (BoneNums == NUM_BONES_HUMAN) {
		Vect Trans = Vect(-280.172638f, 1288.917847f - 500, 526.212402f);
		Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
		Matrix R = Matrix(Matrix::Rot1::Z, 1.5f);
		Matrix S = Matrix(Matrix::Scale::XYZ, 0.5, 0.5, 0.5);

		*pGameRigid->poWorld = T * R * S;
		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim* pHips = new GameObjectAnim(pGraphics_TextureLight);
		pHips->SetIndex(0);
		pHips->SetName("Hips");
		pHips->SetAnimName("Hips");	
		GameObjectMan::Add(pHips, pGameRigid);
		off_set = 1;
		boneHier += off_set;
		pFirstBone = pHips;
	}
	for (int i = off_set; i < BoneNums; i++) {
		pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
		GameObjectAnim* pChild = new GameObjectAnim(pGraphics_TextureLight);
		pChild->SetIndex(boneHier->index);
		pChild->SetName(boneHier->name);
		pChild->SetAnimName(boneHier->name);
		GameObjectAnim* pParent = GameObjectMan::FindAnim(boneHier->parentName);
		GameObjectMan::Add(pChild, pParent);
		if (this->cols < boneHier->depth) {
			this->cols = boneHier->depth;
		}
		boneHier++;
	}
	
	//Alocate GPU tables
	poWorldAll = (Matrix*)malloc(BoneNums * sizeof(Matrix));
	poMagicTable = (int*)malloc(BoneNums * cols * sizeof(Matrix));
	SetupMagicTable();
}


void Anim::SetAnimationPose(GameObjectAnim* pRoot, Time tCurr)
{
	PCSTreeForwardIterator pIter(pRoot);
	PCSNode* pNode = pIter.First();
	GameObjectAnim* pGameObj = 0;

	// walks the anim node does the pose for everything that
	while (pNode != 0)
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObjectAnim*)pNode;
		setBonePose(pGameObj);

		pNode = pIter.Next();
	}
}

void Anim::setName(Anim::Name name)
{
	this->animName = name;
}

Anim::Name Anim::getName()
{
	return animName;
}


Anim::~Anim()

{
	//ProcessAnimationGPU(Time(Time::ZERO));

	FrameBucket* tmp = pFrameHead;
	while(tmp != nullptr)
	{
		free(tmp->poBone);
		tmp = tmp->next;
	}
	
	free(pFrameHead);
	
	
	free (poMagicTable);
	free(poWorldAll);


	delete poSSBO_bA;
	delete poSSBO_bB;
	delete poSSBO_bResult;
	delete poSSBO_BoneTQRs;
	delete poSSBO_MagicTable;
	delete poSSBO_bWorldAll;

	delete poMixerShader;
	delete poWorldShader;
}

void Anim::setBonePose(GameObjectAnim* node)
{
	// Now get the world matrices
	// getting the parent from current node
	GameObjectAnim* childNode = (GameObjectAnim*)node;
	GameObjectAnim* parentNode;

	if (node->GetParent()->GetParent()->GetParent() != nullptr)
		parentNode = (GameObjectAnim*)node->GetParent();
	else
		parentNode = nullptr;

	if (parentNode == root)
		return;

	if (parentNode != 0 && childNode != 0)
	{
		// starting point
		Vect start(0.0f, 0.0f, 0.0f);

		//  At this point, Find the two bones initial positions in world space
		//  Now get the length and directions

		Vect ptA = start * *parentNode->GetWorld();
		Vect ptB = start * *childNode->GetWorld();

		// direction between the anchor points of the respective bones
		Vect dir = (ptA - ptB);

		// length of the bone 0
		float mag = dir.mag();

		Matrix S(Matrix::Scale::XYZ, BONE_WIDTH, BONE_WIDTH, mag);
		Quat Q(Quat::Orient::LocalToWorld, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f));
		Matrix T(Matrix::Trans::XYZ, ptB);

		Matrix BoneOrient = S * Q * T;

		childNode->SetBoneOrientation(BoneOrient);
	}
}

void Anim::SetAnimationData(AnimHeader* pAnimHeader, int stackNum)
{
	//(void*)pAnimHeader;
	BoneNums = pAnimHeader->bonesPerFrame;
	//Trace::out("bone nums:%d, stack num:%d\n", BoneNums, stackNum);
	int frames = pAnimHeader->FramesPerStack[stackNum];
	//Trace::out("BoneOffset:%d\n", pAnimHeader->boneOffset);
	Trace::out("Frames:%d\n", frames);
	FrameBucket* pBucket = (FrameBucket*)((unsigned char*)pAnimHeader + pAnimHeader->frameOffset);
	
	Bone* pBone = (Bone*)((unsigned char*)pAnimHeader + pAnimHeader->boneOffset);

	for (int i = 0; i < stackNum; i++) {
		pBucket += pAnimHeader->FramesPerStack[i];
		pBone += pAnimHeader->FramesPerStack[i] * BoneNums;
	}

	//Trace::out("prev: %d\n", pBucket->prevBucket);
	//Trace::out("next: %d\n", pBucket->nextBucket);
	//Trace::out("next: %d\n", pBucket->KeyTime);
	//Trace::out("first bone: %d\n", pBucket->pBone);


	// --------  Result Frame  ----------------------
	pFrameHead = new FrameBucket();
	pFrameHead->prev = nullptr;
	pFrameHead->next = pBucket;
	pFrameHead->KeyTime = 0;
	//pFrameHead->pBone = new Bone[BONE_NUMS];
	pFrameHead->poBone = (Bone*)malloc(sizeof(Bone) * BoneNums);
	//Trace::out("bonenum vs rows\n, %d, %d", BoneNums, rows);
	// first bucket is special because linked to an internal head note
	// so deal with it seperately
	pBucket->prev = pFrameHead;
	pBucket->next = (FrameBucket*)(((unsigned int)pBucket) + (unsigned int)(pBucket->next));
	pBucket->poBone = pBone;
	// 
	unsigned int* tmpPtr = (unsigned int*)(pBucket + 1);
	//fix frame pointers
	for (int i = 1; i < frames; i++) {
		*tmpPtr = (unsigned int)pBucket + (unsigned int)(*tmpPtr);// pointer to next

		tmpPtr++; //->prev
		*tmpPtr = (unsigned int)pBucket + (unsigned int)(*tmpPtr);// pointer to prev

		tmpPtr++; //->time
		tmpPtr++; //-> pBone
		// ***********fix pBone pointer**********
		*tmpPtr = (unsigned int)(pBone + BoneNums * i);

		tmpPtr++; //->pad
		tmpPtr++;//->(next) next	
	}
	tmpPtr -= 5;
	*tmpPtr = 0; // next of last node is nullptr
	// --------  Frame 0  ----------------------------
	//Frame_Bucket * tmp = pFrameHead;
	//while (tmp != nullptr) {
	//	Trace::out("Key Time:%d\n", tmp->KeyTime);
	//	Trace::out("Bone[1]: T:%f %f %f\n", tmp->pBone[0].T[x], tmp->pBone[0].T[y], tmp->pBone[0].T[z]);
	//	tmp = tmp->nextBucket;
	//}
}



// End of File
