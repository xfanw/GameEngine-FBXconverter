#pragma once

#include "Image.h"
class Glyph : public Image
{

public:
	// public methods: -------------------------------------------------------------
	Glyph();
	Glyph(Rect rect, int key);
	Glyph(float x, float y, float w, float h, int key);
	

public:
	// data: -----------------------------------------------------------------------
	int key;
};