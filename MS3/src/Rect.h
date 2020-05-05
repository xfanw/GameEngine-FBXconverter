#ifndef RECT_H
#define RECT_H

struct Rect
{
	Rect( float x, float y, float width, float height);
	Rect();

	void clear();

	float x;
	float y;
	float width;
	float height;
};


#endif