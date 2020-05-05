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

TEST( Matrix_SET_Identity, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.set(IDENTITY);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 1.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 1.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 1.0f );
}

TEST( Matrix_SET_Zero, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.set(ZERO);

	CHECK( A[m0] == 0.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 0.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 0.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 0.0f );
}

TEST( Matrix_SET_Trans, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.set(TRANS, 2.0f, 3.0f, 4.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 1.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 1.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 2.0f );
	CHECK( A[m13] == 3.0f );
	CHECK( A[m14] == 4.0f );
	CHECK( A[m15] == 1.0f );
}


TEST(Matrix_SET_Trans_Vect, matrix_tests)
{
	Vect V0(17.0f, 2.0f, 3.0f, 4.0f);
	Vect V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vect V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vect V3(13.0f, 14.0f, 15.0f, 16.0f);

	Matrix A(V0, V1, V2, V3);

	CHECK(A[m0] == 17.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	CHECK(A[m3] == 4.0f);
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	CHECK(A[m7] == 8.0f);
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);
	CHECK(A[m11] == 12.0f);
	CHECK(A[m12] == 13.0f);
	CHECK(A[m13] == 14.0f);
	CHECK(A[m14] == 15.0f);
	CHECK(A[m15] == 16.0f);

	Vect vTrans(2.0f, 3.0f, 4.0f);
	A.set(TRANS, vTrans);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 0.0f);
	CHECK(A[m2] == 0.0f);
	CHECK(A[m3] == 0.0f);
	CHECK(A[m4] == 0.0f);
	CHECK(A[m5] == 1.0f);
	CHECK(A[m6] == 0.0f);
	CHECK(A[m7] == 0.0f);
	CHECK(A[m8] == 0.0f);
	CHECK(A[m9] == 0.0f);
	CHECK(A[m10] == 1.0f);
	CHECK(A[m11] == 0.0f);
	CHECK(A[m12] == 2.0f);
	CHECK(A[m13] == 3.0f);
	CHECK(A[m14] == 4.0f);
	CHECK(A[m15] == 1.0f);
}

TEST( Matrix_SET_SCALE, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.set(SCALE, 2.0f, 3.0f, 4.0f);

	CHECK( A[m0] == 2.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 3.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 4.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 1.0f );
}


TEST(Matrix_SET_SCALE_Vect, matrix_tests)
{
	Vect V0(17.0f, 2.0f, 3.0f, 4.0f);
	Vect V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vect V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vect V3(13.0f, 14.0f, 15.0f, 16.0f);

	Matrix A(V0, V1, V2, V3);

	CHECK(A[m0] == 17.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	CHECK(A[m3] == 4.0f);
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	CHECK(A[m7] == 8.0f);
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);
	CHECK(A[m11] == 12.0f);
	CHECK(A[m12] == 13.0f);
	CHECK(A[m13] == 14.0f);
	CHECK(A[m14] == 15.0f);
	CHECK(A[m15] == 16.0f);

	Vect vScale(2.0f, 3.0f, 4.0f);
	A.set(SCALE, vScale);

	CHECK(A[m0] == 2.0f);
	CHECK(A[m1] == 0.0f);
	CHECK(A[m2] == 0.0f);
	CHECK(A[m3] == 0.0f);
	CHECK(A[m4] == 0.0f);
	CHECK(A[m5] == 3.0f);
	CHECK(A[m6] == 0.0f);
	CHECK(A[m7] == 0.0f);
	CHECK(A[m8] == 0.0f);
	CHECK(A[m9] == 0.0f);
	CHECK(A[m10] == 4.0f);
	CHECK(A[m11] == 0.0f);
	CHECK(A[m12] == 0.0f);
	CHECK(A[m13] == 0.0f);
	CHECK(A[m14] == 0.0f);
	CHECK(A[m15] == 1.0f);
}


TEST( RotX_SET, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Rx(V0,V1,V2,V3);

	CHECK( Rx[m0] == 17.0f );
	CHECK( Rx[m1] == 2.0f );
	CHECK( Rx[m2] == 3.0f );
	CHECK( Rx[m3] == 4.0f );
	CHECK( Rx[m4] == 5.0f );
	CHECK( Rx[m5] == 6.0f );
	CHECK( Rx[m6] == 7.0f );	
	CHECK( Rx[m7] == 8.0f );
	CHECK( Rx[m8] == 9.0f );
	CHECK( Rx[m9] == 10.0f );
	CHECK( Rx[m10] == 11.0f );
	CHECK( Rx[m11] == 12.0f );
	CHECK( Rx[m12] == 13.0f );
	CHECK( Rx[m13] == 14.0f );
	CHECK( Rx[m14] == 15.0f );
	CHECK( Rx[m15] == 16.0f );

	// Rot_X Type Constructor:
	Rx.set(ROT_X, 1.0471975512f );	

	CHECK( Rx[m0] ==  1.0f );
	CHECK( Rx[m1] ==  0.0f );
	CHECK( Rx[m2] ==  0.0f );
	CHECK( Rx[m3] ==  0.0f );
	CHECK( Rx[m4] ==  0.0f );
	CHECK( eq(Rx[m5],  0.5f,  MATH_TOLERANCE) );
	CHECK( eq(Rx[m6], 0.866f, MATH_TOLERANCE) );
	CHECK( Rx[m7] == 0.0f );
	CHECK( Rx[m8] == 0.0f );
	CHECK( eq(Rx[m9],-0.866f, MATH_TOLERANCE) );
	CHECK( eq(Rx[m10],0.5f  , MATH_TOLERANCE) );
	CHECK( Rx[m11] == 0.0f );
	CHECK( Rx[m12] == 0.0f );
	CHECK( Rx[m13] == 0.0f );
	CHECK( Rx[m14] == 0.0f );
	CHECK( Rx[m15] == 1.0f );
}


TEST( RotY_SET, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Ry(V0,V1,V2,V3);

	CHECK( Ry[m0] == 17.0f );
	CHECK( Ry[m1] == 2.0f );
	CHECK( Ry[m2] == 3.0f );
	CHECK( Ry[m3] == 4.0f );
	CHECK( Ry[m4] == 5.0f );
	CHECK( Ry[m5] == 6.0f );
	CHECK( Ry[m6] == 7.0f );	
	CHECK( Ry[m7] == 8.0f );
	CHECK( Ry[m8] == 9.0f );
	CHECK( Ry[m9] == 10.0f );
	CHECK( Ry[m10] == 11.0f );
	CHECK( Ry[m11] == 12.0f );
	CHECK( Ry[m12] == 13.0f );
	CHECK( Ry[m13] == 14.0f );
	CHECK( Ry[m14] == 15.0f );
	CHECK( Ry[m15] == 16.0f );

	Ry.set(ROT_Y,  1.0471975512f );	

	CHECK( eq(Ry[m0],0.5f,  MATH_TOLERANCE) );
	CHECK( Ry[m1] ==  0.0f );
	CHECK( eq(Ry[m2],-0.866f,MATH_TOLERANCE) );
	CHECK( Ry[m3] == 0.0f );
	CHECK( Ry[m4] == 0.0f );
	CHECK( Ry[m5] == 1.0f );
	CHECK( Ry[m6] == 0.0f );
	CHECK( Ry[m7] == 0.0f );
	CHECK( eq(Ry[m8],0.866f,MATH_TOLERANCE) );
	CHECK( Ry[m9] ==  0.0f );
	CHECK( eq(Ry[m10],0.5f,   MATH_TOLERANCE) );
	CHECK( Ry[m11] == 0.0f );
	CHECK( Ry[m12] == 0.0f );
	CHECK( Ry[m13] == 0.0f );
	CHECK( Ry[m14] == 0.0f );
	CHECK( Ry[m15] == 1.0f );
}


TEST( RotZ_SET, matrix_tests )
{
	Vect V0(17.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Rz(V0,V1,V2,V3);

	CHECK( Rz[m0] == 17.0f );
	CHECK( Rz[m1] == 2.0f );
	CHECK( Rz[m2] == 3.0f );
	CHECK( Rz[m3] == 4.0f );
	CHECK( Rz[m4] == 5.0f );
	CHECK( Rz[m5] == 6.0f );
	CHECK( Rz[m6] == 7.0f );	
	CHECK( Rz[m7] == 8.0f );
	CHECK( Rz[m8] == 9.0f );
	CHECK( Rz[m9] == 10.0f );
	CHECK( Rz[m10] == 11.0f );
	CHECK( Rz[m11] == 12.0f );
	CHECK( Rz[m12] == 13.0f );
	CHECK( Rz[m13] == 14.0f );
	CHECK( Rz[m14] == 15.0f );
	CHECK( Rz[m15] == 16.0f );

	Rz.set(ROT_Z, 1.0471975512f);	

	CHECK( eq(Rz[m0],0.5f,   MATH_TOLERANCE) );
	CHECK( eq(Rz[m1],0.866f,MATH_TOLERANCE) );
	CHECK( Rz[m2] == 0.0f );
	CHECK( Rz[m3] == 0.0f );
	CHECK( eq(Rz[m4],-0.866f,MATH_TOLERANCE) );
	CHECK( eq(Rz[m5],0.5f,  MATH_TOLERANCE) );
	CHECK( Rz[m6] == 0.0f );
	CHECK( Rz[m7] == 0.0f );
	CHECK( Rz[m8] == 0.0f );
	CHECK( Rz[m9] == 0.0f );
	CHECK( Rz[m10] == 1.0f );
	CHECK( Rz[m11] == 0.0f );
	CHECK( Rz[m12] == 0.0f );
	CHECK( Rz[m13] == 0.0f );
	CHECK( Rz[m14] == 0.0f );
	CHECK( Rz[m15] == 1.0f );
}

TEST( set_ROW_0, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	A.set(ROW_0, V);

	CHECK( A[m0] == 20.0f );
	CHECK( A[m1] == 30.0f );
	CHECK( A[m2] == 40.0f );
	CHECK( A[m3] == 50.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}


TEST( set_ROW_1, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	A.set(ROW_1, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 20.0f );
	CHECK( A[m5] == 30.0f );
	CHECK( A[m6] == 40.0f );	
	CHECK( A[m7] == 50.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_ROW_2, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	A.set(ROW_2, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 20.0f );
	CHECK( A[m9] == 30.0f );
	CHECK( A[m10] == 40.0f );
	CHECK( A[m11] == 50.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}


TEST( set_ROW_3, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	A.set(ROW_3, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 20.0f );
	CHECK( A[m13] == 30.0f );
	CHECK( A[m14] == 40.0f );
	CHECK( A[m15] == 50.0f );
}


TEST( get_ROW_0, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.get(ROW_0);

	CHECK( V[x] == 1.0f);
	CHECK( V[y] == 2.0f);
	CHECK( V[z] == 3.0f);
	CHECK( V[w] == 4.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}


TEST( get_ROW_1, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.get(ROW_1);

	CHECK( V[x] == 5.0f);
	CHECK( V[y] == 6.0f);
	CHECK( V[z] == 7.0f);
	CHECK( V[w] == 8.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( get_ROW_2, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.get(ROW_2);

	CHECK( V[x] == 9.0f);
	CHECK( V[y] == 10.0f);
	CHECK( V[z] == 11.0f);
	CHECK( V[w] == 12.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}


TEST( get_ROW_3, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vect V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.get(ROW_3);

	CHECK( V[x] == 13.0f);
	CHECK( V[y] == 14.0f);
	CHECK( V[z] == 15.0f);
	CHECK( V[w] == 16.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_Vectors, matrix_tests )
{
	Vect V0(1.0f,2.0f,3.0f,4.0f);
	Vect V1(5.0f,6.0f,7.0f,8.0f);
	Vect V2(9.0f,10.0f,11.0f,12.0f);
	Vect V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(ZERO);

	CHECK( A[m0] == 0.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 0.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 0.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 0.0f );

	A.set(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

// ---  End of File ---------------
