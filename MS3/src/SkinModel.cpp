#include <sb7.h>
#include <assert.h>

#include "SkinModel.h"


void SkinModel::privLoadVBO0Data(const VBOHeader* pVBOHeader)
{
	this->numVerts = pVBOHeader->numVerts;
	//Trace::out("Verts: %d\n", numVerts);
	skinData = (Vert_xyzuvn*)((unsigned char*)pVBOHeader + pVBOHeader->vertBufferOffset);

	// FRANK Step 1.1 : Bind VBO0
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
	// FRANK Step 1.2 : Load Data
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), skinData, GL_STATIC_DRAW);
	// Frank Step 1.3 :Set layout
	void* offsetVert = (void*)((unsigned int)&skinData[0].x - (unsigned int)skinData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
	glEnableVertexAttribArray(0);

	void* offsetTex = (void*)((unsigned int)&skinData[0].u - (unsigned int)skinData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	void* offsetNorm = (void*)((unsigned int)&skinData[0].nx - (unsigned int)skinData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);
	//Frank Verify

	//Vert_xyzuvn* tmp = skinData;
	//for (int i = 0; i < numVerts; i++) {
	//	Trace::out("vert[%d]: %f, %f, %f; %f, %f; %f, %f, %f\n", i,
	//		tmp->x, tmp->y, tmp->z, tmp->u, tmp->v, tmp->nx, tmp->ny, tmp->nz);
	//	tmp++;
	//}
}

void SkinModel::privLoadVBO1Data(const VBOHeader* pVBOHeader)
{
	this->numTris = pVBOHeader->numTriList;
	//Trace::out("Tris: %d\n", numTris);

	skinTriList = (Tri_index*)((unsigned char*)pVBOHeader + pVBOHeader->triListBufferOffset);

	// FRANK Step 2.1 : Bind VBO1
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	// Frank Step 2.2 :Load data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), skinTriList, GL_STATIC_DRAW);


	//Frank Verify

	//Tri_index* tmp = skinTriList;
	//for (int i = 0; i < numTris; i++) {
	//	Trace::out("vert[%d]: %d, %d, %d;\n", i,
	//		tmp->v0, tmp->v1, tmp->v2);
	//	tmp++;
	//}
}

void SkinModel::priveLoadVBO2Data(const SkinHeader* pSkinHeader)
{
	this->indexNum = pSkinHeader->numVerts;
	this->poseNum = pSkinHeader->numPoseInv;
	//Trace::out("Indexs:%d, Poses: %d\n", indexNum, poseNum);
	weightIndex = (Skin_Weight_Index*)((unsigned char*)pSkinHeader + pSkinHeader->vertWeightListOffset);

	// FRANK Step 3.1 : Bind VBO2
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo2);
	// FRANK Step 3.2 : Load Data
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Skin_Weight_Index) * this->indexNum), weightIndex, GL_STATIC_DRAW);

	// Frank Step 3.2 :Set layout
	void* offsetWeight = (void*)((unsigned int)&weightIndex[0].weightA - (unsigned int)weightIndex);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Skin_Weight_Index), offsetWeight);
	glEnableVertexAttribArray(3);

	void* offsetIndex = (void*)((unsigned int)&weightIndex[0].indexA - (unsigned int)weightIndex);
	glVertexAttribIPointer(4, 4, GL_INT, sizeof(Skin_Weight_Index), offsetIndex);
	glEnableVertexAttribArray(4);

	//Frank Verify

	//Skin_Weight_Index* tmp = weightIndex;
	//for (int i = 0; i < indexNum; i++) {
	//	Trace::out("vert[%d]:\n %d: %f\n %d: %f\n %d: %f\n %d: %f\n", i,
	//		tmp->indexA, tmp->weightA, tmp->indexB, tmp->weightB, tmp->indexC, tmp->weightC, tmp->indexD, tmp->weightD);
	//	tmp++;
	//}
}



void SkinModel::privCreateVAO(const char* const modelFileName)
{
	UNUSED_VAR(modelFileName);
	// Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	// Create a VBO0,1
	glGenBuffers(2, &this->vbo[0]);
	assert(this->vbo[0] != 0);

	// Creat VBO2
	glGenBuffers(1, &this->vbo2);
	assert(this->vbo2 != 0);
}

SkinModel::SkinModel(const char* const modelFileName, const SkinHeader* pSkinHeader, const VBOHeader* pVBOHeader)
	: Model()
{

	this->privCreateVAO(modelFileName);
	privLoadVBO0Data(pVBOHeader);
	privLoadVBO1Data(pVBOHeader);
	priveLoadVBO2Data(pSkinHeader);
	   	 
	
}

SkinModel::~SkinModel()
{
	//delete skinData;
	//delete skinTriList;
	//delete weightIndex;
}