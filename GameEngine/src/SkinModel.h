#ifndef SKIN_MODEL_H
#define SKIN_MODEL_H

#include "Model.h"
#include <AnimFileHeader.h>

struct SkinHeader {
	char objName[OBJECT_NAME_SIZE];
	int numVerts;
	unsigned long vertWeightListOffset;

	int numPoseInv;
	unsigned long poseInvOffset;
	SkinHeader()
	{
		memset(this, 0x0, sizeof(SkinHeader));
	}
};
struct Skin_Weight_Index {

	float weightA;
	float weightB;
	float weightC;
	float weightD;
	int indexA;
	int indexB;
	int indexC;
	int indexD;
};


class SkinModel : public Model
{
public:
	SkinModel(const char* const modelFileName, const SkinHeader* pSkinHeader, const VBOHeader* pVBOHeader);
	virtual ~SkinModel();

	// tells the compiler do not create or allow it to be used, c++11
	SkinModel(const SkinModel&) = delete;
	SkinModel& operator=(const SkinModel& other) = delete;
private:
	void privCreateVAO(const char* const modelFileName) override;
	void privLoadVBO0Data(const VBOHeader* pVBOHeader);
	void privLoadVBO1Data(const VBOHeader* pVBOHeader);
	void priveLoadVBO2Data(const SkinHeader* pShinHeader);

	int indexNum;
	int poseNum;
	Vert_xyzuvn* skinData;
	Tri_index* skinTriList;
	Skin_Weight_Index* weightIndex;
public:
	GLuint vbo2;

};

#endif