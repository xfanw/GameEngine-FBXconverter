#ifndef BONE_WEIGHT_H
#define BONE_WEIGHT_H

// -----------------------------------------
//  Weights
// -----------------------------------------

			#include <vector>
#include <MathEngine.h>
			struct VertWeight
			{
				int   iVertIndex;
				float fBoneWeight;
			};

			struct BoneWeight
			{
				int   iBoneIndex;
				float fBoneWeight;
			};


			struct FinalVertEntry
			{
			public:
				BoneWeight A;
				BoneWeight B;
				BoneWeight C;
				BoneWeight D;
			};

			struct VertEntry
			{
			public:
				std::vector< BoneWeight >  vBoneWeight;
			};

			struct BoneEntry
			{
			public:
				static const unsigned int StringSize = 64;

			public:
				BoneEntry() = delete;
				BoneEntry(int Index, const char * const pName);

				void PrintMe();

			public:
				int iBoneIndex;
				char poBoneName[StringSize];
				std::vector< VertWeight >  vVertWeight;
			};

			struct PoseInv {
				int index;
				Matrix mInv;
				PoseInv(int i, Matrix M) {
					index = i;
					mInv = M;
				}
			};

#endif // !BONE_WEIGHT_H
