//---------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "UnitTest.h"
#include "MathEngine.h"

//---------------------------------------------------------------------------
// Sample TESTS:
//---------------------------------------------------------------------------

TEST(SampleTest, vector_tests)
{
	float x(5.0f);

	CHECK(x == 5.0f);
}

TEST(ConstantCheck, vector_tests)
{
	CHECK(Util::isEqual(MATH_PI2, 1.570796f, MATH_TOLERANCE) == true);
}

// ---  End of File ---------------
