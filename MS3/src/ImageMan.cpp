
#include <assert.h>

#include "ImageMan.h"

ImageMan::ImageMan()
{
	this->active = 0;
}

ImageMan * ImageMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static ImageMan imageMan;
	return &imageMan;
}

void ImageMan::Create()
{
	ImageMan *pImageMan = ImageMan::privGetInstance();
	assert(pImageMan);
}

void ImageMan::Destroy()
{
	ImageMan *pImageMan = ImageMan::privGetInstance();
	assert(pImageMan);

	Image *pLink = pImageMan->active;

	while (pLink != nullptr)
	{
		Image *pTmp = pLink;
		pLink = (Image *)pLink->next;
		pImageMan->privRemove(pTmp, pImageMan->active);
		delete pTmp;
	}
}

void ImageMan::Add(Image *pImage)
{
	// Get the instance to the manager
	ImageMan *pImageMan = ImageMan::privGetInstance();
	assert(pImageMan);

	// Create a Image
	assert(pImage);

	// Now add it to the manager
	pImageMan->privAddToFront(pImage, pImageMan->active);
}

void ImageMan::privAddToFront(Image *node, Image *&head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void ImageMan::privRemove(Image *pNode, Image *&poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (Image *)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}

//--- End of File -------------------------------------------------------------
