#pragma once
#include "Anim.h"

	// Singleton
class AnimManager
{
	public:
		static void Add(Anim::Name name, Anim* pAnim);
		static Anim* Find(Anim::Name name);
		static void SetCurrent(Anim::Name name);
		static Anim* GetCurrent();
		static void Create();
		static void Destroy();

	private:  // methods

		static AnimManager* privGetInstance();
		AnimManager();
		void privAddToFront(AnimLink* node, AnimLink*& head);
		void privRemove(AnimLink* node, AnimLink*& head);

	private:  // add

		AnimLink* active;
		Anim* currAnim;

};

