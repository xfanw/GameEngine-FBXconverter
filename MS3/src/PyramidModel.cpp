#include <sb7.h>
#include <assert.h>

#include "PyramidModel.h"



#define TRILIST_NUM_TRIANGLES (sizeof(pyramidTriList)/sizeof(Tri_index))


Tri_index pyramidTriList[] =
{
	{0,1,2},
	{2,3,0},
	{4,5,6},
	{5,7,6},
	{8,9,10},
	{9,11,10},
	{12,13,14},
	{13,15,14},
	{16,17,18},
	{18,19,16},
	{20,21,22},
	{22,23,20}
};



#define PYRAMID_DATA_NUM_VERTS (sizeof(pyramidData)/sizeof(Vert_xyzuvn))



Vert_xyzuvn  pyramidData[] =
{
	// Triangle 0
	{-0.25f,  0.0f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f  }, //0
	{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f  },// 1
	{ 0.25f, -0.0f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f  }, //2

	// Triangle 1

	{ 0.0f,   1.0f, -0.0f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f  },  //4  3


	// Triangle 2
	{ 0.25f, -0.0f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f  }, //6
	{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f  }, //7
	{ 0.0f,  1.0f, -0.0f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f  }, //8

	// Triangle 3

	{ 0.25f,  0.0f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f  }, //9 


	// Triangle 4
	{ 0.25f, -0.0f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f  },  // 12
	{-0.25f, -0.0f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f  },   // 13
	{ 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f  },  // 14

	// Triangle 5

	{ -0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },  //15


	// Triangle 6
	{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //18
	{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //19
	{-0.0f,  1.0f,  0.0f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, //20

	// Triangle 7

	{-0.0f,  1.0f, -0.0f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // 21


	// Triangle 8
	{-0.25f, -0.0f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //24
	{0.25f, -0.0f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f }, //25
	{0.25f, -0.0f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //26

	// Triangle 9

	{-0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f}, // 27


	// Triangle 10
	{-0.0f,  1.0f, -0.0f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }, //30
	{0.0f,  1.0f,  -0.0f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f }, //31
	{0.0f,  1.0f,   0.0f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32

	// Triangle 11

	{-0.0f,  1.0f,  0.0f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f }, //33

};

void PyramidModel::privCreateVAO(const char * const modelFileName)
{
	// future proofing it for a file
	UNUSED_VAR(modelFileName);

	this->numVerts = PYRAMID_DATA_NUM_VERTS;
	this->numTris = TRILIST_NUM_TRIANGLES;

	Matrix M(Matrix::Rot1::X, MATH_PI2);
	for (int i = 0; i < this->numVerts; i++)
	{
		Vect v(pyramidData[i].x, pyramidData[i].y, pyramidData[i].z);
		v = v * M;
		pyramidData[i].x = v[x];
		pyramidData[i].y = v[y];
		pyramidData[i].z = v[z];
	}

	// Remove HACK  Keenan(24)

		//// Make the bone correct size and shape
		//Vect vScale;
		//Vect vUp;
		//Vect vDof;
		//Vect vPos;

		//vScale.set(40.0f, 40.0f, 109.43f);
		//vPos.set(0.0f, 0.0f, 0.0f);
		//vDof.set(1.0f, 0.0f, 0.0f);
		//vUp.set(0.0f, 1.0f, 1.0f);

		//Matrix Scale(Matrix::Scale, vScale);
		//Matrix Trans(Matrix::Trans, vPos);
		//Matrix Rot(Matrix::RotOrient, vDof, vUp);

		//// world matrix
		//M = Scale * Rot * Trans;

		//for (int i = 0; i < this->numVerts; i++)
		//{
		//	Vect v(pyramidData[i].x, pyramidData[i].y, pyramidData[i].z);
		//	v = v * M;
		//	pyramidData[i].x = v[x];
		//	pyramidData[i].y = v[y];
		//	pyramidData[i].z = v[z];
		//}

	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO
	glGenBuffers(2, &this->vbo[0]);
	assert(this->vbo[0] != 0);


	// Load the combined data: ---------------------------------------------------------

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	// load the data to the GPU
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pyramidData, GL_STATIC_DRAW);

	// Vertext data in location 0
	void *offsetVert = (void *)((unsigned int)&pyramidData[0].x - (unsigned int)pyramidData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
	glEnableVertexAttribArray(0);

	// Texture data in location 1
	void *offsetTex = (void *)((unsigned int)&pyramidData[0].u - (unsigned int)pyramidData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	// Normal data in location 2
	void *offsetNorm = (void *)((unsigned int)&pyramidData[0].nx - (unsigned int)pyramidData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);


	// Load the index data: ---------------------------------------------------------

		  /* Bind our 2nd VBO as being the active buffer and storing index ) */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);

	/* Copy the index data to our buffer */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pyramidTriList, GL_STATIC_DRAW);

}

PyramidModel::PyramidModel(const char * const modelFileName)
	: Model()
{
	this->privCreateVAO(modelFileName);
}

PyramidModel::~PyramidModel()
{
	// remove anything dynamic here
}