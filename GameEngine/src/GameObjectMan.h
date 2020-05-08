//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"
#include "GameObjectAnim.h"
#include <map>
#include "Anim.h"

// Singleton
class GameObjectMan
{
public:
	static void Add(GameObject *pObj, GameObject *pParent);
	static void Draw( void );
	static void Update(Time currentTime);
	static GameObjectAnim* FindAnim(const char* name);
	static GameObject *GetRoot(void);

	//static void SetCurrent(Anim::Name name);
	static PCSTree *GetPCSTree();
	static void Create();
	static void Destroy();
	~GameObjectMan();
private:
	GameObjectMan();

	static GameObjectMan *privGetInstance();

// data
	PCSTree *poRootTree;	
};

#endif

//--- End of File ------------------------------------------------------------
