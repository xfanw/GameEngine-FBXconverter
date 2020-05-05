
#include <assert.h>
#include "FontMan.h"
#include "Framework.h"
FontMan::FontMan() {
	this->active = 0;
}
void FontMan::Add(Font* pFont)
{
	// Get the instance to the manager
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);

	// Create a Image
	assert(pFont);

	// Now add it to the manager
	pFontMan->privAddToFront(pFont, pFontMan->active);

}

void FontMan::Create()
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);
}

void FontMan::Destroy()
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);

	Font* pLink = pFontMan->active;

	while (pLink != nullptr)
	{
		Font* pTmp = pLink;
		pLink = (Font*)pLink->next;
		pFontMan->privRemove(pTmp, pFontMan->active);
		delete pTmp;
	}
}

Font* FontMan::Find(Font::NAME _name)
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);

	Font* pNode = (Font*)pFontMan->active;
	while (pNode != 0)
	{
		if (pNode->name == _name)
		{
			// found it
			break;
		}

		pNode = (Font*)pNode->next;
	}

	return pNode;
}


FontMan* FontMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static FontMan FontMan;
	return &FontMan;
}

void FontMan::privAddToFront(Font* node, Font*& head)
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

void FontMan::privRemove(Font* node, Font*& head)
{
	assert(node);

	if (node->prev != nullptr)
	{	// middle or last node
		node->prev->next = node->next;
	}
	else
	{  // first
		head = (Font*)node->next;
	}

	if (node->next != nullptr)
	{	// middle node
		node->next->prev = node->prev;
	}
}


