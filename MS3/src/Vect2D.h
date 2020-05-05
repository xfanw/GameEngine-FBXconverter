#ifndef VECT_2D_H
#define VECT_2D_H

struct Vect2D
{
	Vect2D(float x, float y);
	Vect2D();
	~Vect2D();

	void clear();

	float x;
	float y;
};


#endif