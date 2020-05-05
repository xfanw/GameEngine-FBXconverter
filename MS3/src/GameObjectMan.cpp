//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "NullModel.h"
#include "GraphicsObject_Null.h"
#include "ShaderObject.h"

#include "GameObjectAnim.h"
#include "GameObjectMan.h"
#include "PCSTreeForwardIterator.h"
#include "PCSTreeReverseIterator.h"
#include "GameObjectNull.h"
#include "ModelMan.h"
#include "ShaderMan.h"

void GameObjectMan::Add(GameObject *pObj, GameObject *pParent)
{
	assert(pObj != 0);
	assert(pParent != 0);

	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();

	// insert object to root
	pGOM->poRootTree->Insert(pObj, pParent);
}

//void GameObjectMan::SetCurrent(Anim::Name name)
//{
//	GameObjectMan* pGOM = GameObjectMan::privGetInstance();
//	pGOM->pActiveTree = GameObjectMan::treeMap.find(name)->second;
//}

PCSTree *GameObjectMan::GetPCSTree()
{
	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	// Get root node
	return pGOM->poRootTree;
}

void GameObjectMan::Create()
{
	// Get singleton
	GameObjectMan* pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);
}

GameObject * GameObjectMan::GetRoot()
{
	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	GameObject *pGameObj = (GameObject *)pGOM->poRootTree->GetRoot();
	assert(pGameObj);

	return pGameObj;
}

void GameObjectMan::Update(Time currentTime)
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode *pRootNode = pGOM->poRootTree->GetRoot()/*->GetChild()*/;
	assert(pRootNode);

	PCSTreeForwardIterator pIter(pRootNode);

	PCSNode *pNode = pIter.First();

	GameObject *pGameObj = 0;

	while (!pIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObject *)pNode;
		pGameObj->Update(currentTime);

		pNode = pIter.Next();
	}

}

GameObjectAnim* GameObjectMan::FindAnim(const char* name)
{
	GameObjectMan* pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode* pRootNode = pGOM->poRootTree->GetRoot();
	assert(pRootNode);

	PCSTreeForwardIterator pIter(pRootNode/*->GetChild()*/);

	PCSNode* pNode = pIter.First();

	GameObjectAnim* pGameObj = 0;

	while (!pIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObjectAnim*)pNode;
		
		if (strcmp(pGameObj->GetAnimName(), name) == 0) {
			return pGameObj;
		}

		pNode = pIter.Next();
	}
	return nullptr;
}

void GameObjectMan::Draw()
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode *pRootNode = pGOM->poRootTree->GetRoot();
	assert(pRootNode);

	PCSTreeForwardIterator pForwardIter(pRootNode);
	PCSNode *pNode = pForwardIter.First();

	GameObject *pGameObj = 0;

	while (!pForwardIter.IsDone())
	{
		assert(pNode);
		// Update the game object
		pGameObj = (GameObject *)pNode;
		
		if ( pGameObj->GetDrawEnable() )
		{
			pGameObj->Draw();
		}

		pNode = pForwardIter.Next();
	}
}

GameObjectMan::GameObjectMan( )
{
	// Create the root node (null object)
	NullModel *pModel = new NullModel(0);
	ModelMan::Add(Model::NAME::NULLMODEL, pModel);
	ShaderObject *pShader = new ShaderObject(ShaderObject::NAME::NULL_SHADER, "nullRender", ShaderObject::TYPE::VERTEX);
	ShaderMan::Add(pShader);
	GraphicsObject_Null *pGraphicsObject = new GraphicsObject_Null(pModel, pShader);
	GameObjectNull *pGameRoot = new GameObjectNull(pGraphicsObject);
	pGameRoot->SetName("GameObjectNull_Root");

	// Create the tree
	this->poRootTree = new PCSTree();
	assert(this->poRootTree);

	// Attach the root node
	this->poRootTree->Insert(pGameRoot, this->poRootTree->GetRoot());
}
	
GameObjectMan * GameObjectMan::privGetInstance( void )
{
	// This is where its actually stored (BSS section)
	static GameObjectMan gom;
	return &gom;
}
void GameObjectMan::Destroy()
{
	// Get singleton
	GameObjectMan* pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSTree* pTree = pGOM->poRootTree;
	
	PCSNode* pNode = nullptr;

	PCSTreeForwardIterator pForIter(pTree->GetRoot());
	pNode = pForIter.First();
	PCSNode* pTmp = nullptr;
	GameObject* pGameObj = nullptr;
	while (!pForIter.IsDone())
	{
		pTmp = pForIter.CurrentItem();
		pGameObj = (GameObject*)pNode;
		pNode = pForIter.Next();
		delete pGameObj;
	}	
}
GameObjectMan::~GameObjectMan()
{
	delete this->poRootTree;
}
//--- End of File ------------------------------------------------------------

