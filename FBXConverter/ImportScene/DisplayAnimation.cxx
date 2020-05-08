/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayCommon.h"
#include "DisplayAnimation.h"
#include "FBX_STL.h"

#if defined (FBXSDK_ENV_MAC)
// disable the "format not a string literal and no format arguments" warning since
// the FBXSDK_printf calls made here are all valid calls and there is no security risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

std::vector<FBX_Frame> FrameList;
extern std::vector<FBX_Stack> StackList;

//set up Frame
// it will be used in several functions and i don't want to pass the argument
FBX_Frame tmpFrame;
FBX_Stack tmpStack;
static int stackCount;
// (10)
static int index = 0;

// (11)
FbxAnimEvaluator *pAnimEvaluator;

// (8) - add FbxTime lkeyTime to display
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher, FbxTime lKeyTime);
void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher, FbxTime lKeyTime);

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, int lLayers, bool isSwitcher = false);
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);

void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayAnimation(FbxScene* pScene)
{
  int i = 0;

  // (11)
  pAnimEvaluator = pScene->GetAnimationEvaluator();
  stackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
  //StackList.reserve(stackCount);
  
	for (i = 0; i < stackCount; i++)
    {
        FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

		// (11) RAM - RULES good find  <--- He read the manual
		pScene->SetCurrentAnimationStack(lAnimStack);

        FbxString lOutputString = "Animation Stack Name: ";
        lOutputString += lAnimStack->GetName();
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);

		FbxTakeInfo* takeInfo = pScene->GetTakeInfo(lAnimStack->GetName());
		FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
		FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	printf("**********Here to check Stack********\n");
		//printf("%lld\n",start);
		//printf("%lld\n",end);
		FbxLongLong startFrame = start.GetFrameCount(FbxTime::eFrames30);
		FbxLongLong endFrame = end.GetFrameCount(FbxTime::eFrames30);
		printf("%lld\n", startFrame);
		printf("%lld\n", endFrame);
		int lLayers = (int)(endFrame - startFrame);

		//set up stack name and layer of frames
		memcpy_s(tmpStack.name , 16, lAnimStack->GetName(), 16);
		tmpStack.frameCount = lLayers;

		//int n = sizeof(FBX_Frame);


       DisplayAnimation(lAnimStack, pScene->GetRootNode(), lLayers);
	   
	   
    }
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, int lLayers, bool isSwitcher)
{
    int l;
    int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	int nbAudioLayers = pAnimStack->GetMemberCount<FbxAudioLayer>();
    FbxString lOutputString;

    lOutputString = "   contains ";
	if (nbAnimLayers==0 && nbAudioLayers==0)
		lOutputString += "no layers";

	if (nbAnimLayers)
	{
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer";
		if (nbAnimLayers > 1)
			lOutputString += "s";
	}

	if (nbAudioLayers)
	{
		if (nbAnimLayers)
			lOutputString += " and ";

		lOutputString += nbAudioLayers;
		lOutputString += " Audio Layer";
		if (nbAudioLayers > 1)
			lOutputString += "s";
	}
	lOutputString += "\n\n";
    FBXSDK_printf(lOutputString);

	StackList.reserve(stackCount * nbAudioLayers);

    for (l = 0; l < nbAnimLayers; l++)
    {
		  FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		  if (isSwitcher == false)
		  {
			  lOutputString = "AnimLayer ";
			  lOutputString += l;
			  lOutputString += "\n";
			  FBXSDK_printf(lOutputString);
		  }

		  char    lTimeString[256];
		  FbxTime   lKeyTime;
		  int     lCount; 
		  
		  // Frame list set up (1)
		  FrameList.reserve(lLayers);

		  for (lCount = 0; lCount < lLayers; lCount++)
		  {
			  // Frame KeyTime (1)
			  tmpFrame.KeyTime = lCount;
			  // (11) - Note this is hard coded... problem for non-30Hz
			  lKeyTime.SetTime(0, 0, 0, lCount, 0, 0, FbxTime::eFrames30);

			  if (isSwitcher == false)
			  {
				  lOutputString = "\n\n ------- Key Time: ";
				  lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));

				  lOutputString += "  ms: ";
				  int msTime = (int)lKeyTime.GetMilliSeconds();
				  lOutputString += msTime;
				  lOutputString += " ------------------ \n\n";
				  printf(lOutputString);
			  }

			  index = 0;
			  DisplayAnimation(lAnimLayer, pNode, isSwitcher, lKeyTime);

			  // setup count
			  tmpStack.boneCount = tmpFrame.FrameBoneList.size();
			  FrameList.push_back(tmpFrame);
			  tmpFrame.FrameBoneList.clear();
		  }
		  tmpStack.layerNum = l;
		  tmpStack.StackFrameList = FrameList;
		  FrameList.clear();
		  // Frank : Hack only use layer ZERO
		  if (l==0)  StackList.push_back(tmpStack);
	}

	// (4) - ignoring Audio layers
	for (l = 0; l < nbAudioLayers; l++)
	{
		FbxAudioLayer* lAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(l);

		lOutputString = "AudioLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAudioLayer, isSwitcher);
		FBXSDK_printf("\n");
	}
}

void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool )
{
    int lClipCount;
    FbxString lOutputString;

	lClipCount = pAudioLayer->GetMemberCount<FbxAudio>();
	
    lOutputString = "     Name: ";
    lOutputString += pAudioLayer->GetName();
	lOutputString += "\n\n";
	lOutputString += "     Nb Audio Clips: ";
	lOutputString += lClipCount;
    lOutputString += "\n";
    FBXSDK_printf(lOutputString);

	for (int i = 0; i < lClipCount; i++)
	{
		FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
		lOutputString = "        Clip[";
		lOutputString += i;
		lOutputString += "]:\t";
		lOutputString += lClip->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}

// (8) - api change
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher, FbxTime lKeyTime)
{
	int lModelCount;
	FbxString lOutputString;

	// (8)
	if (isSwitcher == false)
	{
		// lOutputString = "Node Name: ";
		lOutputString = pNode->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}

	// (8) add lKeyTime
	FbxString name = pNode->GetName();

	// only output useful nodes
	//printf(name);
	//printf("%d,%d\n", name.Find("Bip"), name.Find(":"));
	if (((name.Find("Bip")>=0) && (name.Find(":")==-1)) ||(name.Find("Humanoid:")>=0&&(name.Find("MaxToFilmbox_Root")==-1&&name.Find("Reference")==-1))){
		DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher, lKeyTime);
	}
	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{

		  DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher, lKeyTime);

	}
}

#define MATH_PI_180  0.0174532925f

// (8) - api change
void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher, FbxTime lKeyTime)
{
	FbxAnimCurve* lAnimCurve = NULL;
	char	buff[256];
	FbxString lOutputString;

	// (10)
	

	// setup Bone Pose
	FBX_Bone_Pose tmpBone;


	// Display general curves.
	if (!isSwitcher)
	{
		// char    lTimeString[256];

		FbxAMatrix matrix = pNode->EvaluateLocalTransform(lKeyTime);

		FbxVector4 Trans = matrix.GetT();
		FbxVector4 RotXYZ = matrix.GetR();
		FbxVector4 Scale = matrix.GetS();
		
		//lOutputString = "pTmp->pBone[";
		//lOutputString += index;
		//lOutputString += "].T = Vect( ";
		//sprintf(buff, "%ff", Trans[0]);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", Trans[1]);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", Trans[2]);
		//lOutputString += buff;
		//lOutputString += " );\n";
		//printf(lOutputString);

		// setup T
		tmpBone.T = Vect(Trans[0], Trans[1], Trans[2]);
		printf("T: ");
		printf("%f, %f, %f\n", tmpBone.T[x], tmpBone.T[y], tmpBone.T[z]);


		//lOutputString = "pTmp->pBone[";
		//lOutputString += index;
		//lOutputString += "].Q = Quat( Quat::Rot3::XYZ,";
		//sprintf(buff, "%ff", RotXYZ[0] * MATH_PI_180);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", RotXYZ[1] * MATH_PI_180);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", RotXYZ[2] * MATH_PI_180);
		//lOutputString += buff;
		//lOutputString += " );\n";
		//printf(lOutputString);

		// set up Q
		tmpBone.Q = Quat(Quat::Rot3::XYZ, RotXYZ[0] * MATH_PI_180, RotXYZ[1] * MATH_PI_180, RotXYZ[2] * MATH_PI_180);


		//lOutputString = "pTmp->pBone[";
		//lOutputString += index;
		//lOutputString += "].S = Vect( ";
		//sprintf(buff, "%ff", Scale[0]);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", Scale[1]);
		//lOutputString += buff;
		//lOutputString += ", ";
		//sprintf(buff, "%ff", Scale[2]);
		//lOutputString += buff;
		//lOutputString += " );\n\n";

		// setup S
		tmpBone.S = Vect(Scale[0], Scale[1], Scale[2]);
		printf("S: ");
		printf("%f, %f, %f\n", tmpBone.S[x], tmpBone.S[y], tmpBone.S[z]);


		printf(lOutputString);

	} // if
	// add to list from index 0
	tmpFrame.FrameBoneList.push_back(tmpBone);
	index++;
}


static int InterpolationFlagToIndex(int flags)
{
	if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
    if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
	if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
    return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
    if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
    return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
    if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
    if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
    if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
    if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
    if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
    return 0;
}

static int TangentweightFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
    if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
    if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
    return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
    if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
    if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
    return 0;
}

void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
    static const char* interpolation[] = { "?", "constant", "linear", "cubic"};
    static const char* constantMode[] =  { "?", "Standard", "Next" };
    static const char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
    static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

    FbxTime   lKeyTime;
    float   lKeyValue;
    char    lTimeString[256];
    FbxString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();
	
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));

		lOutputString += "           In ms: ";
		long long msTime = lKeyTime.GetMilliSeconds();
		lOutputString += (int)msTime;

        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " [ ";
        lOutputString += interpolation[ InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount)) ];
        if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
        {
            lOutputString += " | ";
            lOutputString += constantMode[ ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount)) ];
        }
        else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
        {
            lOutputString += " | ";
            lOutputString += cubicMode[ TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount)) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode()) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode()) ];
        }
        lOutputString += " ]";
        lOutputString += "\n";
        printf (lOutputString);
    }
}

void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
    FbxTime   lKeyTime;
    int     lKeyValue;
    char    lTimeString[256];
    FbxString lListValue;
    FbxString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " (";
        lOutputString += pProperty->GetEnumValue(lKeyValue);
        lOutputString += ")";

        lOutputString += "\n";
        printf (lOutputString);
    }
}
