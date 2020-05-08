//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GRAPHIC_OBJECT_MAN_H
#define GRAPHIC_OBJECT_MAN_H

#include "GraphicsObject.h"
#include "PCSTree.h"


// Singleton
class GraphicsObjectMan
{
public:
	static void Add( GraphicsObject *pObj);
	static void Destroy();
	~GraphicsObjectMan();

private:  // methods

	static GraphicsObjectMan* privGetInstance();
	GraphicsObjectMan();
	PCSTree *poRootTree;
};

#endif

//--- End of File ------------------------------------------------------------
