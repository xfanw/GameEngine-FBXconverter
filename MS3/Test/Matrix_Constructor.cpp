//---------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------
#include "UnitTest.h"
#include "MathEngine.h"

#define eq	Util::isEqual 

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------
TEST( Matrix_default_constructor, matix_tests )
{
	Matrix M;

	CHECK( M[m0] == 0.0f );
	CHECK( M[m1] == 0.0f );
	CHECK( M[m2] == 0.0f );
	CHECK( M[m3] == 0.0f );
	CHECK( M[m4] == 0.0f );
	CHECK( M[m5] == 0.0f );
	CHECK( M[m6] == 0.0f );	
	CHECK( M[m7] == 0.0f );
	CHECK( M[m8] == 0.0f );
	CHECK( M[m9] == 0.0f );
	CHECK( M[m10] == 0.0f );
	CHECK( M[m11] == 0.0f );
	CHECK( M[m12] == 0.0f );
	CHECK( M[m13] == 0.0f );
	CHECK( M[m14] == 0.0f );
	CHECK( M[m15] == 0.0f );

}

TEST( Matrix_vector_constructor, matix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(7.0f,6.0f,5.0f,3.0f);
	Vect V2(-4.0f,-2.0f,-1.0f,-4.0f);
	Vect V3(9.0f,-7.0f,-2.0f,5.0f);

	CHECK( V0[x] == 1.0f );
	CHECK( V0[y] == 2.0f );
	CHECK( V0[z] == 3.0f );
	CHECK( V0[w] == 4.0f );
	CHECK( V1[x] == 7.0f );
	CHECK( V1[y] == 6.0f );
	CHECK( V1[z] == 5.0f );	
	CHECK( V1[w] == 3.0f );
	CHECK( V2[x] == -4.0f );
	CHECK( V2[y] == -2.0f );
	CHECK( V2[z] == -1.0f );
	CHECK( V2[w] == -4.0f );
	CHECK( V3[x] == 9.0f );
	CHECK( V3[y] == -7.0f );
	CHECK( V3[z] == -2.0f );
	CHECK( V3[w] == 5.0f );

	Matrix M(V0,V1,V2,V3);

	CHECK( M[m0] == 1.0f );
	CHECK( M[m1] == 2.0f );
	CHECK( M[m2] == 3.0f );
	CHECK( M[m3] == 4.0f );
	CHECK( M[m4] == 7.0f );
	CHECK( M[m5] == 6.0f );
	CHECK( M[m6] == 5.0f );	
	CHECK( M[m7] == 3.0f );
	CHECK( M[m8] == -4.0f );
	CHECK( M[m9] == -2.0f );
	CHECK( M[m10] == -1.0f );
	CHECK( M[m11] == -4.0f );
	CHECK( M[m12] == 9.0f );
	CHECK( M[m13] == -7.0f );
	CHECK( M[m14] == -2.0f );
	CHECK( M[m15] == 5.0f );
}

TEST( Matrix_copy_constructor, matix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(7.0f,6.0f,5.0f,3.0f);
	Vect V2(-4.0f,-2.0f,-1.0f,-4.0f);
	Vect V3(9.0f,-7.0f,-2.0f,5.0f);

	CHECK( V0[x] == 1.0f );
	CHECK( V0[y] == 2.0f );
	CHECK( V0[z] == 3.0f );
	CHECK( V0[w] == 4.0f );
	CHECK( V1[x] == 7.0f );
	CHECK( V1[y] == 6.0f );
	CHECK( V1[z] == 5.0f );	
	CHECK( V1[w] == 3.0f );
	CHECK( V2[x] == -4.0f );
	CHECK( V2[y] == -2.0f );
	CHECK( V2[z] == -1.0f );
	CHECK( V2[w] == -4.0f );
	CHECK( V3[x] == 9.0f );
	CHECK( V3[y] == -7.0f );
	CHECK( V3[z] == -2.0f );
	CHECK( V3[w] == 5.0f );

	Matrix M(V0,V1,V2,V3);

	Matrix N( M );

	CHECK( N[m0] == 1.0f );
	CHECK( N[m1] == 2.0f );
	CHECK( N[m2] == 3.0f );
	CHECK( N[m3] == 4.0f );
	CHECK( N[m4] == 7.0f );
	CHECK( N[m5] == 6.0f );
	CHECK( N[m6] == 5.0f );	
	CHECK( N[m7] == 3.0f );
	CHECK( N[m8] == -4.0f );
	CHECK( N[m9] == -2.0f );
	CHECK( N[m10] == -1.0f );
	CHECK( N[m11] == -4.0f );
	CHECK( N[m12] == 9.0f );
	CHECK( N[m13] == -7.0f );
	CHECK( N[m14] == -2.0f );
	CHECK( N[m15] == 5.0f );

	CHECK( M[m0] == 1.0f );
	CHECK( M[m1] == 2.0f );
	CHECK( M[m2] == 3.0f );
	CHECK( M[m3] == 4.0f );
	CHECK( M[m4] == 7.0f );
	CHECK( M[m5] == 6.0f );
	CHECK( M[m6] == 5.0f );	
	CHECK( M[m7] == 3.0f );
	CHECK( M[m8] == -4.0f );
	CHECK( M[m9] == -2.0f );
	CHECK( M[m10] == -1.0f );
	CHECK( M[m11] == -4.0f );
	CHECK( M[m12] == 9.0f );
	CHECK( M[m13] == -7.0f );
	CHECK( M[m14] == -2.0f );
	CHECK( M[m15] == 5.0f );
}

TEST( Destructor_constuctor, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(7.0f,6.0f,5.0f,3.0f);
	Vect V2(-4.0f,-2.0f,-1.0f,-4.0f);
	Vect V3(9.0f,-7.0f,-2.0f,5.0f);

	Matrix M(V0,V1,V2,V3);

	Matrix *pM = &M;
	pM->~Matrix();
	CHECK(1);
}

// ---  End of File ---------------
