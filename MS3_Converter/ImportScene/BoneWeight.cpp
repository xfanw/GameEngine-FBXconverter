
#include <vector>
#include <assert.h>
#include <string.h>

// ------------------------------------------------------
// Keenan Weights
// ------------------------------------------------------

			#include "BoneWeight.h"

			BoneEntry::BoneEntry(int Index, const char * const pName)
			{
				assert(Index >= 0);
				this->iBoneIndex = Index;

				// set everything to 0
				memset(this->poBoneName, 0x0, StringSize);

				// copy n-1 to the buffer
				strncpy_s(this->poBoneName, StringSize, pName, StringSize - 1);

				// Leave this alone... its has no data  
				// this->vVertWeight;

			}

			void BoneEntry::PrintMe()
			{
				printf("Bone[%d] %s \n", this->iBoneIndex, this->poBoneName);
			}