
#include "Framework.h"
#include "Game.h"
#include "MathEngine.h"

#include "CameraMan.h"
#include "GameObjectAnim.h"
#include "GraphicsDebugMessage.h"


#include "Timer.h"
#include "InputMan.h"
#include "GameObjectMan.h"
#include "SpeedControl.h"
#include "AnimManager.h"
#include "AnimFileHeader.h"
#include "ModelMan.h"
#include "TextureMan.h"
#include "ShaderMan.h"

// FONT and 2D
#include "Image.h"
#include "GraphicsObject_Sprite.h"
#include "SpriteModel.h"
#include "GameObject2D.h"
#include "ImageMan.h"
#include "GlyphMan.h"
#include "GameObjectRIgid.h"
#include "Font.h"
#include "FontMan.h"

// SPHERE & Cube
#include "SphereModel.h"
#include "GraphicsObject_Wireframe.h"
#include "GameObjectSimple.h"
#include "CubeModel.h"
#include "GraphicsObject_TextureLight.h"

//Skin
#include <SkinModel.h>
#include <GraphicsObject_SkinConstColor.h>
#include <SkinGameObject.h>

// Compute shader
#include "SSBO.h"

// TO DO: ----------------------------------
SpeedControl speedController;
unsigned char* bearBuffer;
unsigned char* humanBuffer;
unsigned char* clipBuffer;
int counting;
SkinGameObject* pHumanSkinObj;
SkinGameObject* pBearSkinObj;
SSBO* pSSBO_in = nullptr;
SSBO* pSSBO_out = nullptr;
//-----------------------------------------------------------------------------
//  Game::Game()
//		Game Engine Constructor
//-----------------------------------------------------------------------------
Game::Game(const char* const pWindowName, const int Width, const int Height)
	:Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
	this->globalTimer.Tic();
	this->intervalTimer.Tic();
}


//-----------------------------------------------------------------------------
// Game::Initialize()
//		Allows the engine to perform any initialization it needs to before 
//      starting to run.  This is where it can query for any required services 
//      and load any non-graphic related content. 
//-----------------------------------------------------------------------------
void Game::Initialize()
{

}


//-----------------------------------------------------------------------------
// Game::LoadContent()
//		Allows you to load all content needed for your engine,
//	    such as objects, graphics, etc.
//-----------------------------------------------------------------------------
void Game::LoadContent()
{
	// Start the managers
	InputMan::Create();
	CameraMan::Create();
	TextureMan::Create();
	ModelMan::Create();
	ShaderMan::Create();
	GameObjectMan::Create();
	AnimManager::Create();
	ImageMan::Create();

	GlyphMan::Create();
	FontMan::Create();
	// Camera setup

	// For teddy
	Camera* pBear = new Camera(Camera::Type::PERSPECTIVE_3D);
	pBear->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pBear->setPerspective(35.0f, float(pBear->getScreenWidth()) / float(pBear->getScreenHeight()), 1.0f, 20000.0f);
	pBear->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 50.0f), Vect(1000.0f, 100.0f, 0.0f));
	
	// For Human
	Camera* pHuman = new Camera(Camera::Type::PERSPECTIVE_3D);
	pHuman->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pHuman->setPerspective(35.0f, float(pHuman->getScreenWidth()) / float(pHuman->getScreenHeight()), 1.0f, 20000.0f);
	pHuman->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(2000.0f, 0.0f, 0.0f));

	pBear->updateCamera();
	pHuman->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_BEAR, pBear);
	CameraMan::Add(Camera::Name::CAMERA_HUMAN, pHuman);
	CameraMan::SetCurrent(Camera::Name::CAMERA_BEAR, Camera::Type::PERSPECTIVE_3D);
	// For General
	Camera* pCam = new Camera(Camera::Type::PERSPECTIVE_3D);
	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(50.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 0.10f, 1000.0f);
	pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, 0.0f, 4.0f));
	pCam->updateCamera();
	CameraMan::Add(Camera::Name::CAMERA_0, pCam);
	//---------------------------------------------------------------------------------------------------------
	// Setup the current 2D orthographic Camera
	//---------------------------------------------------------------------------------------------------------
	Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

	pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam2D->setOrthographic(-pCam2D->getScreenWidth() / 2.0f, pCam2D->getScreenWidth() / 2.0f,
		-pCam2D->getScreenHeight() / 2.0f, pCam2D->getScreenHeight() / 2.0f,
		1.0f, 1000.0f);
	pCam2D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));
	pCam2D->updateCamera();
	// Holder for the current 2D  cameras
	CameraMan::Add(Camera::Name::CAMERA_1, pCam2D);
	CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);
	
	// Load the model
	SpriteModel* pSpriteModel = new SpriteModel("SpriteModel.azul");
	assert(pSpriteModel);
	ModelMan::Add(Model::NAME::SPRITE, pSpriteModel);

	// Create/Load Shader 
	ShaderObject* pShaderObject_sprite = new ShaderObject(ShaderObject::NAME::SPRITE, "spriteRender", ShaderObject::TYPE::VERTEX);
	assert(pShaderObject_sprite);
	ShaderMan::Add(pShaderObject_sprite);


	// Textures
	TextureMan::Add("Rocks.tga", Texture::Name::ROCKS);
	TextureMan::Add("Stone.tga", Texture::Name::STONES);
	TextureMan::Add("RedBrick.tga", Texture::Name::RED_BRICK);
	TextureMan::Add("Duckweed.tga", Texture::Name::DUCKWEED);
	TextureMan::Add("Aliens.tga", Texture::Name::INVADERS);

	/******************************************
					SPRITE
	******************************************/
	// Images
	Image* pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(554.0f, 63.0f, 98.0f, 64.0f));

	assert(pImageGreen);
	ImageMan::Add(pImageGreen);

	// Sprite
	GraphicsObject_Sprite* pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rect(1010, 650, 50, 50));
	GameObject2D* pA1 = new GameObject2D(pGraphics_Sprite);
	GameObjectMan::Add(pA1, GameObjectMan::GetRoot());
	//pA1->posX = 1010.0f;
	//pA1->posY = 650.0f;

	pA1->SetName("ALIEN_RED");
	pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rect(136.0f, 64.0f, 85.0f, 63.0f));
	ImageMan::Add(pImageGreen);

	// Sprite
	pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rect(1010, 550, 50, 50));
	pA1 = new GameObject2D(pGraphics_Sprite);
	GameObjectMan::Add(pA1, GameObjectMan::GetRoot());
	//pA1->posX = 1010.0f;
	//pA1->posY = 550.0f;
	pA1->SetName("ALIEN_GREEN");

	/******************************************
					FONT
	*******************************************/
	//Add Font
	TextureMan::Add("Consolas36pt.tga", Texture::Name::CONSOLAS);
	GlyphMan::AddXml();
	
	
	char red[] = "RED";
	Font* p = new Font(red, sizeof (red), 1000, 600);
	FontMan::Add(p);

	char green[] = "GREEN";
	p = new Font(green, sizeof(green), 980, 500);
	FontMan::Add(p);

	char count[] = "Count";
	p = new Font(count, sizeof(count), 500, 200);
	FontMan::Add(p);
	//Dynamic
	char dynMsg[] = "";
	p = new Font();
	p->SetDynamicMessage(Font::NAME::DYNAMIC_1, dynMsg, sizeof(dynMsg), 600, 200);
	FontMan::Add(p);


	/******************************************
				More Rigid Model
	*******************************************/
	// Sphere

	SphereModel* pSphereModel = new SphereModel("sphereModel.azul");
	ModelMan::Add(Model::NAME::SPHERE, pSphereModel);
	ShaderObject* pShaderObject_color = new ShaderObject(ShaderObject::NAME::COLOR_LIGHT, "colorRender", ShaderObject::TYPE::VERTEX);
	ShaderMan::Add(pShaderObject_color);
	GraphicsObject_Wireframe* pGraphics_Wireframe = new GraphicsObject_Wireframe(pSphereModel, pShaderObject_color);
	GameObjectSimple*  pGameObj = new GameObjectSimple(pGraphics_Wireframe);
	pGameObj->SetScale(Vect(100.0f, 100.0f, 100.0f));
	pGameObj->SetPos(Vect(-800.0f, -800.0f, 500.0f));
	pGameObj->SetDeltaRot(0.050f);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	char shpere[] = "Shpere";
	p = new Font(shpere, sizeof(shpere), 150, 630);
	FontMan::Add(p);

	// Cube
	CubeModel* pCubeModel = new CubeModel("cubeModel.azul");
	ModelMan::Add(Model::NAME::CUBE, pCubeModel);
	ShaderObject* pShaderObject_textureLight = new ShaderObject(ShaderObject::NAME::TEXTURE_POINT_LIGHT, "texturePointLightDiff", ShaderObject::TYPE::VERTEX);
	ShaderMan::Add(pShaderObject_textureLight);

	Vect color(1.50f, 1.50f, 1.50f, 1.0f);
	Vect pos(1, 1, 1);
	GraphicsObject_TextureLight* pGraphicsObj = new GraphicsObject_TextureLight(pCubeModel, pShaderObject_textureLight, Texture::Name::DUCKWEED, color, pos);
	pGameObj = new GameObjectSimple(pGraphicsObj);
	pGameObj->SetScale(Vect(200.0f, 200.0f, 200.0f));
	pGameObj->SetPos(Vect(-800.0f, -800.0f, 100.0f));
	pGameObj->SetDeltaRot(-0.025f);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	char cube[] = "Cube";
	p = new Font(cube, sizeof(cube), 160, 430);
	FontMan::Add(p);







	/******************************************
				Animation
	*******************************************/


	// setup relationship and hierarchy

	// Read from file

	bearBuffer = readFile("teddy_TGA.azul");
	Header* headerBear = (Header*)bearBuffer;
	HierarchyHeader* hireHeader = (HierarchyHeader*)(bearBuffer + headerBear->HierarchyHeaderOffset);
	//AnimHeader* pAnimHeader = (AnimHeader*)(bearBuffer + headerBear->AnimHeaderOffset);
	Reduced_AnimHeader* preducedAnimBear = (Reduced_AnimHeader*)((unsigned char*)headerBear + headerBear->ReducedHierOffset);
	//Trace::out("Stacks:%d", pAnimHeader->numStacks);

	Anim* anim1 = new Anim();
	AnimManager::Add(Anim::Name::BEAR_WALK, anim1);
	//AnimManager::SetCurrent(Anim::Name::BEAR_WALK);
	anim1->SetAnimationHierarchy( hireHeader);
	//anim1->SetAnimationData(pAnimHeader, 0);
	anim1->SetAnimationData_Reduced(preducedAnimBear, 0);

	Anim* anim2 = new Anim();
	AnimManager::Add(Anim::Name::BEAR_IDLE, anim2);
	//AnimManager::SetCurrent(Anim::Name::BEAR_IDLE);
	anim2->SetAnimationHierarchy(hireHeader);
	//anim2->SetAnimationData(pAnimHeader, 1);
	anim2->SetAnimationData_Reduced(preducedAnimBear, 1);



	humanBuffer = readFile("humanoid.azul");
	
	Header* headerHuman = (Header*)humanBuffer;
	HierarchyHeader* hireHeaderHuman = (HierarchyHeader*)(humanBuffer + headerHuman->HierarchyHeaderOffset);
	//AnimHeader* pAnimHeaderHuman = (AnimHeader*)(humanBuffer + headerHuman->AnimHeaderOffset);
	Reduced_AnimHeader* preducedAnimHuman = (Reduced_AnimHeader*)(humanBuffer + headerHuman->ReducedHierOffset);
	Anim* anim4 = new Anim();
	AnimManager::Add(Anim::Name::HUMAN_RUN, anim4);
	//AnimManager::SetCurrent(Anim::Name::HUMAN_RUN);
	anim4->SetAnimationHierarchy(hireHeaderHuman);
	//anim4->SetAnimationData(pAnimHeaderHuman, 0);
	anim4->SetAnimationData_Reduced(preducedAnimHuman, 0);

	Anim* anim5 = new Anim();
	AnimManager::Add(Anim::Name::HUMAN_PUNCK, anim5);
	//AnimManager::SetCurrent(Anim::Name::HUMAN_PUNCK);
	anim5->SetAnimationHierarchy(hireHeaderHuman);
	//anim5->SetAnimationData(pAnimHeaderHuman, 1);
	anim5->SetAnimationData_Reduced(preducedAnimHuman, 1);

	Anim* anim6 = new Anim();
	AnimManager::Add(Anim::Name::HUMAN_SHOT, anim6);
	AnimManager::SetCurrent(Anim::Name::HUMAN_SHOT);
	anim6->SetAnimationHierarchy(hireHeaderHuman);
	//anim6->SetAnimationData(pAnimHeaderHuman, 2);
	
	anim6->SetAnimationData_Reduced(preducedAnimHuman, 2);
	//PCSTree* pTree = GameObjectMan::GetPCSTree();
	//pTree->Print();

	/******************************************
				Skin Model
	*******************************************/	 
	// LoadData
	VBOHeader* pHumanVBOHeader = (VBOHeader*)(humanBuffer + headerHuman->VBOheaderOffset);
	SkinHeader* pHumanSkinHeader = (SkinHeader*)(humanBuffer + headerHuman->Skin_Vert_Offset);
	SkinModel* pHumanSkinModel = new SkinModel("skinModel.azul", pHumanSkinHeader, pHumanVBOHeader);

	//pSkinModel->LoadVBO0Data(pHumanVBOHeader);
	//pSkinModel->LoadVBO1Data(pHumanVBOHeader);
	//pSkinModel->LoadVBO2Data(pHumanSkinHeader);

	ModelMan::Add(Model::NAME::SKIN, pHumanSkinModel);
	ShaderObject* pSkinShader = new ShaderObject(ShaderObject::NAME::SKIN_COLOR_LIGHT, "SkinColorLightRender", ShaderObject::TYPE::VERTEX);
	ShaderMan::Add(pSkinShader);

	GraphicsObject_SkinConstColor* pSkinGrapgObj = new GraphicsObject_SkinConstColor(pHumanSkinModel, pSkinShader, color, pos);
	pSkinGrapgObj->LoadPoseInv(pHumanSkinHeader);
	pHumanSkinObj = new SkinGameObject(pSkinGrapgObj);

	GameObjectMan::Add(pHumanSkinObj, GameObjectMan::GetRoot());
	//pHumanSkinObj->SetDrawEnable(true);


	// Bear
	VBOHeader* pBearVBOHeader = (VBOHeader*)(bearBuffer + headerBear->VBOheaderOffset);
	SkinHeader* pBearSkinHeader = (SkinHeader*)(bearBuffer + headerBear->Skin_Vert_Offset);
	SkinModel* pBearSkinModel = new SkinModel("skinModel.azul", pBearSkinHeader, pBearVBOHeader);

	//pSkinModel->LoadVBO0Data(pHumanVBOHeader);
	//pSkinModel->LoadVBO1Data(pHumanVBOHeader);
	//pSkinModel->LoadVBO2Data(pHumanSkinHeader);

	ModelMan::Add(Model::NAME::SKIN_BEAR, pBearSkinModel);
	//ShaderObject* pSkinShader = new ShaderObject(ShaderObject::NAME::SKIN_COLOR_LIGHT, "SkinColorLightRender", ShaderObject::TYPE::VERTEX);
	//ShaderMan::Add(pSkinShader);

	GraphicsObject_SkinConstColor* pBearSkinGrapgObj = new GraphicsObject_SkinConstColor(pBearSkinModel, pSkinShader, color, pos);
	pBearSkinGrapgObj->LoadPoseInv(pBearSkinHeader);
	pBearSkinObj = new SkinGameObject(pBearSkinGrapgObj);

	GameObjectMan::Add(pBearSkinObj, GameObjectMan::GetRoot());

	/******************************************
				 Compute Shader
	*******************************************/


	 


	/******************************************
			 WALK 2 RUN
	*******************************************/
	clipBuffer = readFile("CLIP_WALK_2_RUN.azul");

	Header* ClipHeader = (Header*)clipBuffer;
	Reduced_AnimHeader* pAnimclip = (Reduced_AnimHeader*)(clipBuffer + ClipHeader->ReducedHierOffset);
	Anim* anim3 = new Anim();
	AnimManager::Add(Anim::Name::WLAK_RUN, anim3);
	//AnimManager::SetCurrent(Anim::Name::BEAR_TAKE);
	anim3->SetAnimationHierarchy(hireHeaderHuman);
	anim3->SetAnimationData_Reduced(pAnimclip, 0);
}

unsigned char* Game::readFile(const char* fileName)
{
	FILE* pFileHandle;
	fopen_s(&pFileHandle, fileName, "rb");
	assert(pFileHandle != 0);
	fseek(pFileHandle, 0, SEEK_END);
	unsigned int fileSize = (unsigned int)ftell(pFileHandle);
	rewind(pFileHandle);
	unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * fileSize);
	//unsigned char* inBuffer = new unsigned char[fileSize];	
	assert(buffer != 0);
	fread(buffer, fileSize, 1, pFileHandle);
	fclose(pFileHandle);
	return buffer;
}

//-----------------------------------------------------------------------------
// Game::Update()
//      Called once per frame, update data, tranformations, etc
//      Use this function to control process order
//      Input, AI, Physics, Animation, and Graphics
//-----------------------------------------------------------------------------
void Game::Update(float)
{
	// ------------------------------------------------
	//  Compute Shader Test
	// ------------------------------------------------


	// ------------------------------------------------
	// Camera update
	// ------------------------------------------------

	static Time tCurrent(Time::ZERO);

	// Mark our end time.
	this->intervalTimer.Toc();
	this->intervalTimer.Tic();

	/******************************************
				FONT_DYNAMIC
	*******************************************/
	Font* dynFont = FontMan::Find(Font::NAME::DYNAMIC_1);

	counting++;
	int tmp = counting;
	unsigned int count = 0;
	while (tmp != 0) {
		tmp = tmp / 10;
		count++;
	}

	//Trace::out("%d, %d\n", i, count);
	char strInt[10];
	_itoa_s(counting, strInt, count + 1, 10);
	dynFont->UpdateMessage(strInt, count);

	// ------------------------------------------------
	// Animate ME
	// ------------------------------------------------
	speedController.SpeedInput();
	Anim* pAnim = AnimManager::GetCurrent();
	Anim::Name name = pAnim->getName();

	if (name == Anim::Name::BEAR_IDLE || name == Anim::Name::BEAR_WALK ) {
		CameraMan::SetCurrent(Camera::Name::CAMERA_BEAR, Camera::Type::PERSPECTIVE_3D);
		
		pHumanSkinObj->SetDrawEnable(false);
		pBearSkinObj->SetDrawEnable(true);
	}
	else {
		CameraMan::SetCurrent(Camera::Name::CAMERA_HUMAN, Camera::Type::PERSPECTIVE_3D);
		pHumanSkinObj->SetDrawEnable(true);
		pBearSkinObj->SetDrawEnable(false);
	}


	pAnim->ProcessAnimationGPU(tCurrent);
	pAnim->SetupBoneWorldGPU();

	GameObjectMan::Update(this->globalTimer.Toc());
	pAnim->SetAnimationPose(pAnim->pFirstBone, tCurrent);

	// -----------------------------------------------
	// update the time
	// ----------------------------------------------

	Time deltaTime = speedController.speed/*0.5f*/ * Time(Time::NTSC_30_FRAME);
	Time maxTime;

	tCurrent += deltaTime;

	AnimManager::GetCurrent()->FindMaxTime(maxTime);
	
	

	//delete[] strInt;
	// protection for time values for looping
	if (tCurrent < Time(Time::ZERO))
	{
		tCurrent = maxTime;
	}
	else if (tCurrent > maxTime)
	{
		tCurrent = Time(Time::ZERO);
	}
	else
	{
		// do nothing
	}

}

//-----------------------------------------------------------------------------
// Game::Draw()
//		This function is called once per frame
//	    Use this for draw graphics to the screen.
//      Only do rendering here
//-----------------------------------------------------------------------------
void Game::Draw()
{
	GameObjectMan::Draw();
}


//-----------------------------------------------------------------------------
// Game::UnLoadContent()
//       unload content (resources loaded above)
//       unload all content that was loaded before the Engine Loop started
//-----------------------------------------------------------------------------
void Game::UnLoadContent()
{
	// Todo fix this - verify removal
	delete bearBuffer;
	delete humanBuffer;
	delete clipBuffer;
	delete pSSBO_in;
	delete pSSBO_out;

	TextureMan::Destroy();
	InputMan::Destroy();
	ModelMan::Destroy();
	ShaderMan::Destroy();
	AnimManager::Destroy();
	CameraMan::Destroy();
	GameObjectMan::Destroy();
	ImageMan::Destroy();
	GlyphMan::Destroy();
	FontMan::Destroy();
}

//------------------------------------------------------------------
// Game::ClearBufferFunc()
// Allows user to change the way the clear buffer function works
//------------------------------------------------------------------
void Game::ClearBufferFunc()
{
	const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, grey);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

// --- End of File --------------------------------------------------
