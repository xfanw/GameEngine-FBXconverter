/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include <fbxsdk.h>
#include <vector>
#include "FBX_VBO.h"
#include "BoneWeight.h"
extern std::vector< BoneEntry > vBoneTable;
int BoneCount = 0;
#if defined (FBXSDK_ENV_MAC)
// disable the "format not a string literal and no format arguments" warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
#include <string>

extern std::vector<BoneHierarchy> HiList;
// Local functions prototype.
void DisplayHierarchy(FbxNode* pNode, int pDepth);

// Keenan(7) - add parent
static int displayHierarchyIndex = 0;
void DisplayHierarchy(FbxNode* pParent, FbxNode* pNode, int pDepth);

void DisplayHierarchy(FbxScene* pScene)
{
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();

	for (i = 0; i < lRootNode->GetChildCount(); i++)
	{
		// Keenan(7)
		DisplayHierarchy(lRootNode, lRootNode->GetChild(i), 0);
	}
	printf("\n\n Bone Table \n");
	for (size_t j = 0; j < vBoneTable.size(); j++)
		//for (auto j = 0; j < 20; j++)
	{
		vBoneTable[j].PrintMe();

		//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[0].iVertIndex, vBoneTable[j].vVertWeight[0].fBoneWeight);
		//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[1].iVertIndex, vBoneTable[j].vVertWeight[1].fBoneWeight);
		//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[2].iVertIndex, vBoneTable[j].vVertWeight[2].fBoneWeight);
		//printf("i:%d w:%f \n", vBoneTable[j].vVertWeight[3].iVertIndex, vBoneTable[j].vVertWeight[3].fBoneWeight);
	}
}


// Keenan(9) - rework below
void DisplayHierarchy(FbxNode* pParent, FbxNode* pNode, int pDepth)
{
	FbxString lString;
	// -------------------------------------------------------------------------
	// Keenan(2) Weights
	// -------------------------------------------------------------------------

	// Big Hack to remove "Humanoid:" from each name
	char buff[64];
	char buff2[64];
	char origBuff[64];

	// Get original
	// Clean up house cleaning... remove extra stuff on Humanoid
	// Replace spaces with "_" so the names are valid
	strcpy(origBuff, pNode->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff, origBuff);
	}
	for (int i = 0; i < (int)strlen(buff); i++)
	{
		// Find a space... replace with an underscore -- quick and dirty
		if (buff[i] == 32)
		{
			buff[i] = 95;
		}
	}
	/*
	reset parent name
	*/
	// Get original
	strcpy(origBuff, pParent->GetName());

	// test if contains string...
	if (strncmp(origBuff, "Humanoid:", 9) == 0)
	{
		// trim it.
		strcpy(buff2, origBuff + 9);
	}
	else
	{
		// leave it
		strcpy(buff2, origBuff);
	}
	for (int i = 0; i < (int)strlen(buff); i++)
	{
		// Find a space... replace with an underscore -- quick and dirty
		if (buff2[i] == 32)
		{
			buff2[i] = 95;
		}
	}

	/* Goal:

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
	p1 = new SimpleGameObject(pGraphics_TextureLight);
	p1->SetName("Green");
	p1->SetIndex(1);

	GameObjectMan::Add(p1, p0);

	*/
	std::string s(buff);
	if( ((s.find("Bip") != std::string::npos) && (s.find(":") == -1)) || ((strncmp(origBuff, "Humanoid:", 9) == 0)&&(s.find("Reference")== std::string::npos) && (s.find("Max") == std::string::npos))){
		BoneHierarchy tmpH;
		//lString += "pGraphics_TextureLight = new GraphicsObject_TextureLight( pPyramidModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);\n";
		lString += "SimpleGameObject *p";
		lString += buff;
		lString += " = new SimpleGameObject( pGraphics_TextureLight );\n";

		lString += "p";
		lString += buff;
		lString += "->SetIndex(";
		lString += displayHierarchyIndex;
		lString += ");\n";

		lString += "p";
		lString += buff;
		lString += "->SetName(\"";
		lString += buff;
		lString += "\");\n";

		lString += "GameObjectMan::Add(  p";
		lString += buff;
		lString += ", p";
		lString += buff2;
		lString += " );\n\n";

		//printf(lString.Buffer());
		memcpy_s(tmpH.name, 64, buff, 64);
		memcpy_s(tmpH.parentName, 64, buff2, 64);
		tmpH.index = displayHierarchyIndex++;
		tmpH.depth = pDepth;
		HiList.push_back(tmpH);
		// weight
		printf("Bone name[%d]: %s \tParent:%s\tDepth:%d\n", BoneCount, buff, buff2, pDepth);
		vBoneTable.push_back(BoneEntry(BoneCount, pNode->GetName()));
		BoneCount++;
	}
	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode, pNode->GetChild(i), pDepth + 1);
	}
}