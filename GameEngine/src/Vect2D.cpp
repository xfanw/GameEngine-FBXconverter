#include "Vect2D.h"

Vect2D::Vect2D(float _x, float _y)
	: x(_x), y(_y)
{

}

Vect2D::Vect2D()
	: x(0.0f), y(0.0f)
{

}

Vect2D::~Vect2D()
{
}

void Vect2D::clear()
{
	this->x = 0.0f;
	this->y = 0.0f;
}