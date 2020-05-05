//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "Model.h"

// Singleton
class ModelMan
{
public:
	static void Add(Model::NAME name, Model *pModel);
	static void Create();
	static void Destroy();
	static Model* Find(Model::NAME _name);

private:  // methods

	static ModelMan *privGetInstance();
	ModelMan();

	void privAddToFront(Model *node, Model *&head);
	void privRemove(Model *node, Model *&head);

private:  // add

	Model *active;

};

#endif

//--- End of File -------------------------------------------------------------

