#include "AnimManager.h"
AnimManager::AnimManager()
{
	this->active = 0;
	this->currAnim = 0;
}

AnimManager* AnimManager::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static AnimManager AnimManager;
	return &AnimManager;
}

void AnimManager::Add(Anim::Name name, Anim* pAnim)
{
	// Get the instance to the manager
	AnimManager* pAnimManager = AnimManager::privGetInstance();
	assert(pAnimManager);

	// Create a TextureNode
	AnimNode* pNode = new AnimNode();
	assert(pNode);

	// initialize it
	assert(pAnim);
	pAnim->setName(name);
	pNode->set(pAnim);

	// Now add it to the manager
	pAnimManager->privAddToFront(pNode, pAnimManager->active);
}

Anim* AnimManager::Find(Anim::Name _name)
{
	// Get the instance to the manager
	AnimManager* pAnimManager = AnimManager::privGetInstance();
	assert(pAnimManager);

	AnimNode* pNode = (AnimNode*)pAnimManager->active;
	while (pNode != 0)
	{
		if (pNode->pAnim->getName() == _name)
		{
			// found it
			break;
		}

		pNode = (AnimNode*)pNode->next;
	}
	assert(pNode);
	return pNode->pAnim;
}

void AnimManager::SetCurrent(const Anim::Name name)
{
	AnimManager* pAnimManager = AnimManager::privGetInstance();
	assert(pAnimManager);

	Anim* pAnim = AnimManager::Find(name);
	assert(pAnim);

	pAnimManager->currAnim = pAnim;
}

Anim* AnimManager::GetCurrent()
{
	AnimManager* pAnimManager = AnimManager::privGetInstance();
	assert(pAnimManager);

	assert(pAnimManager->currAnim);
	return pAnimManager->currAnim;
}

void AnimManager::Create()
{
	AnimManager* pAnimManager = AnimManager::privGetInstance();
	assert(pAnimManager);
}

void AnimManager::Destroy()
{

	AnimManager* pAnimMan = AnimManager::privGetInstance();
	assert(pAnimMan);

	AnimLink* pLink = pAnimMan->active;

	while (pLink != nullptr)
	{
		AnimLink* pTmp = pLink;
		pLink = (AnimLink*)pLink->next;
		pAnimMan->privRemove(pTmp, pAnimMan->active);
		delete pTmp;
	}
}
void AnimManager::privRemove(AnimLink* pNode, AnimLink*& poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (AnimLink*)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}
void AnimManager::privAddToFront(AnimLink* node, AnimLink*& head)
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