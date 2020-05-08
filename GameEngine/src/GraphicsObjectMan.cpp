//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObjectMan.h"
#include "PCSTreeForwardIterator.h"

void GraphicsObjectMan::Add(GraphicsObject *pObj)
{
	assert( pObj != 0 );

	// Get singleton
	GraphicsObjectMan *pGOM = GraphicsObjectMan::privGetInstance();

	// Get root node
	PCSNode *pRootNode = pGOM->poRootTree->GetRoot();

	// insert object to root
	pGOM->poRootTree->Insert( pObj, pRootNode );
}


void GraphicsObjectMan::Destroy()
{
	// Get singleton
	GraphicsObjectMan *pGOM = GraphicsObjectMan::privGetInstance();
	assert(pGOM);
	
	PCSTree *pTree = pGOM->poRootTree;
	PCSNode *pNode = nullptr;

	PCSTreeForwardIterator pForIter(pTree->GetRoot());
	pNode = pForIter.First();
	PCSNode *pTmp = nullptr;
	while (!pForIter.IsDone())
	{
		pTmp = pForIter.CurrentItem();
		pNode = pForIter.Next();
		delete pTmp;

	}
}

GraphicsObjectMan::~GraphicsObjectMan()
{
	delete this->poRootTree;
}
	
GraphicsObjectMan * GraphicsObjectMan::privGetInstance( )
{
	// This is where its actually stored (BSS section)
	static GraphicsObjectMan gom;
	return &gom;
}

//--- End of File ------------------------------------------------------------

