#include "MathEngine.h"

#ifndef COLOR_H
#define COLOR_H

class Color
{
public:	
	enum class Type : uint8_t
	{
		UserDefined,
		Red,
		Blue,
		Green,
		White,
		Black,
		Yellow
	};


	// Big 4 - defined
	Color();
	Color(const Color &in);
	Color & operator = (const Color &in);
	~Color();	
	
	// Specialized
	Color(Type type);
	Color(int Red, int Green, int Blue, int Alpha = 0xFF);  // Range: 0x0 - 0xFF
	Color(float Red, float Green, float Blue, float Alpha = 1.0f);  //Range: 0.0f-1.0f

	// Accessors 

		// Set
		float &R();
		float &G();
		float &B();
		float &A();

		// Get
		const float R()const;
		const float G()const;
		const float B()const;
		const float A()const;

	// Automated conversion, to work with other functions
	operator Vect ();

private:
	// Data
	Vect		data;
	Color::Type colorType;
	char		pad[15];
};

#endif