#include "FBX_STL.h"
#include "MathApp.h"

// 	std::vector<FBX_Frame> StackFrameList;

void FBX_Stack::reduce()
{
	std::vector<FBX_Frame> finalFrameList;
	// 1. push first frame to the vector
	finalFrameList.push_back(StackFrameList[0]);
	// 1.1 prepare 
	int size = StackFrameList.size();
	int keyKeep = 0;

	// 2. iterate keyCandidatefrom 2 to size -1
	// 2.1 when a key is pushed to the final stack, candidate is KeyKeep+2;
	for (int keyCandidate = keyKeep+2; keyCandidate < size; keyCandidate++) {

		//2.2 Key Test loop form Keep +1 to Candidate -1
		for (int keyTest = keyKeep + 1; keyTest < keyCandidate; keyTest++) {
			// 3. verify if test calculated by keep and candidate is the dame as original-> continue
			if (isTheSame(StackFrameList[keyKeep], StackFrameList[keyCandidate], keyTest, StackFrameList[keyTest])) {

			}
			// 3.1 otherwise candidate is not good, and we keep the previous one 
			else {
				keyKeep = keyCandidate - 1;
				finalFrameList.push_back(StackFrameList[keyCandidate]);
				break;
			}
		}
	}
	// 6. Always keep last frame
	if (!(finalFrameList.back() == StackFrameList.back())){
		finalFrameList.push_back(StackFrameList[size - 1]);
	}
	
	// veryfy:
	printf("Frank Verify reduced Frame\n");
	int sizeTest = finalFrameList.size();
	//for (int i = 0; i < sizeTest; i++) {
	//	printf("Frame Key Time: %d\n", finalFrameList[i].KeyTime);
	//}
	printf("Original frame count %d, new frame count %d\n", this->frameCount, finalFrameList.size());
	this->StackFrameList.clear();
	this->StackFrameList = finalFrameList;
	this->frameCount =  StackFrameList.size();
}

bool FBX_Stack::isTheSame(FBX_Frame& fa, FBX_Frame& fb, int t, FBX_Frame& fs) {
	// 4. get slerp of time Key test
	FBX_Frame fresult = fa;
	float ts = (float)(t - fa.KeyTime) / (float)(fb.KeyTime - fa.KeyTime);
	int boneCount = fa.FrameBoneList.size();
	for (int i = 0; i < boneCount; i++) {
		QuatApp::Slerp(fresult.FrameBoneList[i].Q, fa.FrameBoneList[i].Q, fb.FrameBoneList[i].Q, ts);
	}

	// 5. verify if they result is the same as standard
	float a = 0.0f;
	float b = 0.0f;
	for (int i = 1; i < boneCount; i++) {
		// 5.1 get angles between bone and bone[0]
		a = fresult.FrameBoneList[i].Q.dot(fresult.FrameBoneList[0].Q);
		b = fs.FrameBoneList[i].Q.dot(fs.FrameBoneList[0].Q);

		// 5.2 if error > 0.005 false
		if (abs(a - b) / b > 0.05) {
			return false;
		}

	}
	return true;
}