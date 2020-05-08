/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#if defined (FBXSDK_ENV_MAC)
// disable the �format not a string literal and no format arguments� warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif
// weight
#include <string.h>
#include <cassert>
#include <vector>
#include "BoneWeight.h"

extern std::vector< BoneEntry > vBoneTable;
#include <MathEngine.h>
extern std::vector<PoseInv> poseInvList;
void DisplayLink(FbxGeometry* pGeometry)
{
    //Display cluster now

    //int i, lLinkCount;
    //FbxCluster* lLink;

    int i, j;
    int lSkinCount=0;
    int lClusterCount=0;
    FbxCluster* lCluster;

    lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);

    poseInvList.reserve(vBoneTable.size());

    //lLinkCount = pGeometry->GetLinkCount();
    for(i=0; i!=lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        for (j = 0; j != lClusterCount; ++j)
        {
            DisplayInt("    Cluster ", j);

            lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
            //lLink = pGeometry->GetLink(i);    

            const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

            if(lCluster->GetLink() != NULL)
            {
                DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
            }

            FbxString lString1 = "        Link Indices: ";
            FbxString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            // -------------------------------------------------------------------------
             // (2) Weights
             // -------------------------------------------------------------------------
            printf(": bone:%s \n", lCluster->GetLink()->GetName());

            // Find the index given string name
            // YES - this is horrible 
            int boneIndex = -1;
            for (unsigned int n = 0; n < vBoneTable.size(); n++)
            {
                if (strcmp(vBoneTable[n].poBoneName, lCluster->GetLink()->GetName()) == 0)
                {
                    boneIndex = n;
                    DisplayString("");
                    // Weight
                    // get link and transfer matrix

                    FbxAMatrix transformMatrix;

                    lCluster->GetTransformMatrix(transformMatrix);
                    //Display3DVector("        Transform Link Translation: ", transformMatrix.GetT());
                    //Display3DVector("        Transform Link Rotation: ", transformMatrix.GetR());
                    //Display3DVector("        Transform Link Scaling: ", transformMatrix.GetS());
                    //printf("\n");
                    //Display4DVector("        Transform Translation: ", transformMatrix.GetT());
                    //Display4DVector("        Transform Rotation: ", transformMatrix.GetR());
                    //Display4DVector("        Transform Scaling: ", transformMatrix.GetS());
                    printf("\n");
                    Display4DVector("        Transform Row1: ", transformMatrix.GetRow(0));
                    Display4DVector("        Transform Row2: ", transformMatrix.GetRow(1));
                    Display4DVector("        Transform Row3: ", transformMatrix.GetRow(2));
                    Display4DVector("        Transform Row4: ", transformMatrix.GetRow(3));


                    FbxAMatrix linkMatrix;
                    lCluster->GetTransformLinkMatrix(linkMatrix);
                    //Display3DVector("        Transform Translation: ", linkMatrix.GetT());
                    //Display3DVector("        Transform Rotation: ", linkMatrix.GetR());
                    //Display3DVector("        Transform Scaling: ", linkMatrix.GetS());
                    //printf("\n");
                    //Display4DVector("        Transform Translation: ", linkMatrix.GetT());
                    //Display4DVector("        Transform Rotation: ", linkMatrix.GetR());
                    //Display4DVector("        Transform Scaling: ", linkMatrix.GetS());
                    printf("\n");
                    Display4DVector("        Transform Row1: ", linkMatrix.GetRow(0));
                    Display4DVector("        Transform Row2: ", linkMatrix.GetRow(1));
                    Display4DVector("        Transform Row3: ", linkMatrix.GetRow(2));
                    Display4DVector("        Transform Row4: ", linkMatrix.GetRow(3));

                    FbxAMatrix invMatrix;
                    invMatrix = linkMatrix.Inverse() * transformMatrix;
                    DisplayString("\n        Frank invmatrix: ");
                    Display4DVector("        invMatrix Model Row1: ", invMatrix.GetRow(0));
                    Display4DVector("        invMatrix Model Row2: ", invMatrix.GetRow(1));
                    Display4DVector("        invMatrix Model Row3: ", invMatrix.GetRow(2));
                    Display4DVector("        invMatrix Model Row4: ", invMatrix.GetRow(3));

                    Vect row0 = Vect((float)invMatrix.Get(0, 0), (float)invMatrix.Get(0, 1), (float)invMatrix.Get(0, 2), (float)invMatrix.Get(0, 3));
                    Vect row1 = Vect((float)invMatrix.Get(1, 0), (float)invMatrix.Get(1, 1), (float)invMatrix.Get(1, 2), (float)invMatrix.Get(1, 3));
                    Vect row2 = Vect((float)invMatrix.Get(2, 0), (float)invMatrix.Get(2, 1), (float)invMatrix.Get(2, 2), (float)invMatrix.Get(2, 3));
                    Vect row3 = Vect((float)invMatrix.Get(3, 0), (float)invMatrix.Get(3, 1), (float)invMatrix.Get(3, 2), (float)invMatrix.Get(3, 3));

                    printf("\nFrank Verify inv matrix\n");
                    printf("\t\trow1 %f %f %f %f\n", row0[x], row0[y], row0[z], row0[w]);
                    printf("\t\trow2 %f %f %f %f\n", row1[x], row1[y], row1[z], row1[w]);
                    printf("\t\trow3 %f %f %f %f\n", row2[x], row2[y], row2[z], row2[w]);
                    printf("\t\trow4 %f %f %f %f\n", row3[x], row3[y], row3[z], row3[w]);

                    Matrix tmpInvMatrix(row0, row1, row2, row3);
                    poseInvList.push_back (PoseInv(boneIndex, tmpInvMatrix));
                    break;
                }
            }


            for (k = 0; k < lIndexCount; k++)
            {
                // ----------------------------------------------------
                // (2) Weights
                // ----------------------------------------------------
                VertWeight vw;
                vw.iVertIndex = lIndices[k];
                vw.fBoneWeight = (float)lWeights[k];

                if (boneIndex != -1)
                {
                    vBoneTable[boneIndex].vVertWeight.push_back(vw);
                }
                else
                {
                   assert(0);
                }


                lString1 += lIndices[k];
                lString2 += (float)lWeights[k];

                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }

            lString1 += "\n";
            lString2 += "\n";

            FBXSDK_printf(lString1);
            FBXSDK_printf(lString2);

           
            // export inverMatrx;

            //if (lCluster->GetAssociateModel() != NULL)
            //{
            //    linkMatrix = lCluster->GetTransformAssociateModelMatrix(linkMatrix);
            //    DisplayString("        Associate Model: ", (char*)lCluster->GetAssociateModel()->GetName());
            //    Display3DVector("        Associate Model Translation: ", linkMatrix.GetT());
            //    Display3DVector("        Associate Model Rotation: ", linkMatrix.GetR());
            //    Display3DVector("        Associate Model Scaling: ", linkMatrix.GetS());
            //}





            DisplayString("");
        }
    }
}


