REM FBXConverter.bat

REM MOST important: STEP 1-->
REM STEP 1 : compile FBXConverter.sln


REM STEP 2 : run me

cd Debug
ImportScene.exe ../../Test/teddy_TGA.fbx > ../../Test/Final_Bear.txt
ImportScene.exe ../../Test/humanoid.fbx> ../../Test/Final_Human.txt

pause
REM STEP 3 : binary file *.zaul are in Test directory