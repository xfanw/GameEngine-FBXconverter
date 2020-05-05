#pragma once


#include "Font.h"

// Singleton
class FontMan
{

public:
	static void Add(Font* pModel);
	static void Create();
	static void Destroy();

	static Font* Find(Font::NAME _name);


private:  // methods

	static FontMan* privGetInstance();
	FontMan();

	void privAddToFront(Font* node, Font*& head);
	void privRemove(Font* node, Font*& head);


private:  // add

	Font* active;

};