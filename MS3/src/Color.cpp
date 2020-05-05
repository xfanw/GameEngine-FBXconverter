#include <assert.h>

#include "Color.h"

Color::Color()
	:data(0.0f, 0.0f, 0.0f, 1.0f),
	colorType(Color::Type::UserDefined)
{

}


Color::Color(float Red, float Green, float Blue, float Alpha)
	:data(Red,Green,Blue,Alpha),
	colorType(Color::Type::UserDefined)
{
	assert(Red >= 0.0f && Red <= 1.0f);
	assert(Green >= 0.0f && Green <= 1.0f);
	assert(Blue >= 0.0f && Blue <= 1.0f);
	assert(Alpha >= 0.0f && Alpha <= 1.0f);
}

Color::Color(int Red, int Green, int Blue, int Alpha)
	:data(Red / 255.0f, Green / 255.0f, Blue / 255.0f, Alpha / 255.0f),
	colorType(Color::Type::UserDefined)
{
	assert(Red >= 0 && Red <= 0xFF);
	assert(Green >= 0 && Green <= 0xFF);
	assert(Blue >= 0 && Blue <= 0xFF);
	assert(Alpha >= 0 && Alpha <= 0xFF);
}

Color::Color(const Color &in)
{
	*this = in;
}

Color & Color::operator = (const Color &in)
{
	this->data = in.data;
	this->colorType = in.colorType;
	return *this;
}

Color::~Color()
{
}

Color::Color(Type type)
{
	this->colorType = type;

	switch (type)
	{
	case Color::Type::Red:
		this->data.set(1.0f, 0.0f, 0.0f, 1.0f);
		break;

	case Color::Type::Blue:
		this->data.set(0.0f, 0.0f, 1.0f, 1.0f);
		break;

	case Color::Type::Green:
		this->data.set(0.0f, 1.0f, 0.0f, 1.0f);
		break;

	case Color::Type::White:
		this->data.set(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	case Color::Type::Black:
		this->data.set(0.0f, 0.0f, 0.0f, 1.0f);
		break;

	case Color::Type::Yellow:
		this->data.set(1.0f, 1.0f, 0.0f, 1.0f);
		break;

	case Color::Type::UserDefined:
	default:
		assert(0);
	}

}

Color::operator Vect ()
{
	return this->data;
}

float &Color::R()
{
	return this->data.x();
}

float &Color::G()
{
	return this->data.y();
}

float &Color::B()
{
	return this->data.z();
}

float &Color::A()
{
	return this->data.w();
}

const float Color::R()const
{
	return this->data.x();
}

const float Color::G()const
{
	return this->data.y();
}

const float Color::B()const
{
	return this->data.z();
}

const float Color::A()const
{
	return this->data.w();
}