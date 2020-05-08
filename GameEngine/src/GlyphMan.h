#pragma once

#include "Glyph.h"
#include <string>
// Singleton
class GlyphMan
{

public:
	static void Add(Glyph* pModel);
	static void Create();
	static void Destroy();

	static Glyph* Find(int key);
	static void AddXml();

private:  // methods

	static GlyphMan* privGetInstance();
	GlyphMan();

	void privAddToFront(Glyph* node, Glyph*& head);
	void privRemove(Glyph* node, Glyph*& head);
	static int getValue(std::string& line, char start, char end);

private:  // add

	Glyph* active;

};