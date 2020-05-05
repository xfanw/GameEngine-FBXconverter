#include <assert.h>

#include "Font.h"
#include "GraphicsObject_Sprite.h"
#include "GameObject2D.h"
#include "GameObjectMan.h"
#include "GlyphMan.h"
#include "SpriteModel.h"
#include "ModelMan.h"
#include "ShaderMan.h"
#include "ShaderObject.h"
#include "PCSTree.h"
Font::Font()
{
	this->name = NAME::STATIC;

	this->posX = 0;
	this->posY = 0;
}

Font::Font(char* newMessage, int len, int x, int y)
{
	this->name = NAME::STATIC;
	SetMessageWithPosition(newMessage, len, x, y);
}

Font::~Font()
{
	FontObj* pLink = poMsgList;

	while (pLink != nullptr)
	{
		FontObj* pTmp = pLink;
		pLink = (FontObj*)pLink->next;
		Remove(pTmp);

		delete pTmp;
	}
}

void Font::UpdateMessage(char* newMessage, int size)
{
	int oldX = this->posX;
	int oldY = this->posY;


	//char msg[10];
	char* tmp = newMessage;
	//for (int i = 0; i < size; i++) {
	//	msg[i] = *newMessage;
	//	tmp++;
	//}
	//FontObj* pLink = poMsgList;
	//PCSTree* pTree = GameObjectMan::GetPCSTree();
	//while (pLink != nullptr)
	//{
	//	FontObj* pTmp = pLink;
	//	pLink = (FontObj*)pLink->next;
	//	Remove(pTmp);
	//	pTree->Remove(pTmp->pGameObj);
	//	delete pTmp->pGameObj;
	//	delete pTmp;
	//}
	//Trace::out("%s, %d\n", newMessage, size);
	//SetDynamicMessage(name, msg, size + 1, this->posX, this->posY);

	Glyph* pGlyph = GlyphMan::Find((int)*tmp);
	FontObj* tmpFontObj = poMsgList;
	int i = 0;
	if (size <= messagelength) {
		for (; i < size; i++) {
			pGlyph = GlyphMan::Find((int)*tmp);
			//Trace::out("%f\n", tmpFontObj->pGameObj->posX);
			//Trace::out("%f\n", tmpFontObj->pGameObj->posY);
			GraphicsObject_Sprite* currSprite = (GraphicsObject_Sprite*)tmpFontObj->pGameObj->poGraphicsObject;
			currSprite->pImage = pGlyph;
			currSprite->origPosX = pGlyph->imageRect.x;
			currSprite->origPosY = pGlyph->imageRect.y;
			currSprite->origWidth = pGlyph->imageRect.width;
			currSprite->origHeight = pGlyph->imageRect.height;
			//Trace::out("%f, %f, %f, %f\n", currSprite->origPosX, currSprite->origPosY, currSprite->origWidth, currSprite->origHeight);
			currSprite->UpdateData();
			tmp++;
			tmpFontObj = (FontObj*)tmpFontObj->next;
		}
		for (i = size; i < messagelength; i++) {
			pGlyph = GlyphMan::Find(32);
			GraphicsObject_Sprite* currSprite = (GraphicsObject_Sprite*)tmpFontObj->pGameObj->poGraphicsObject;
			currSprite->pImage = pGlyph;
			currSprite->origPosX = pGlyph->imageRect.x;
			currSprite->origPosY = pGlyph->imageRect.y;
			currSprite->origWidth = pGlyph->imageRect.width;
			currSprite->origHeight = pGlyph->imageRect.height;
			currSprite->UpdateData();
			tmp++;
			tmpFontObj = (FontObj*)tmpFontObj->next;
		}
	}
	else {//size > message length
		int newX = posX;
		for (; i < messagelength; i++) {
			pGlyph = GlyphMan::Find((int)*tmp);
			//Trace::out("%f\n", tmpFontObj->pGameObj->posX);
			//Trace::out("%f\n", tmpFontObj->pGameObj->posY);
			GraphicsObject_Sprite* currSprite = (GraphicsObject_Sprite*)tmpFontObj->pGameObj->poGraphicsObject;
			currSprite->pImage = pGlyph;
			currSprite->origPosX = pGlyph->imageRect.x;
			currSprite->origPosY = pGlyph->imageRect.y;
			currSprite->origWidth = pGlyph->imageRect.width;
			currSprite->origHeight = pGlyph->imageRect.height;
			//Trace::out("%f, %f, %f, %f\n", currSprite->origPosX, currSprite->origPosY, currSprite->origWidth, currSprite->origHeight);
			currSprite->UpdateData();
			newX +=(int) pGlyph->imageRect.width;
			tmp++;
			tmpFontObj = (FontObj*)tmpFontObj->next;
		}
		char msg[10];
		//char* tmpInit = tmp;
		for (int j = 0; j < size - messagelength; j++) {
			msg[j] = *tmp;
			tmp++;
		}
		SetDynamicMessage(name, msg, size -messagelength + 1, newX, this->posY);
	}
	this->posX = oldX;
	this->posY = oldY;
}

void Font::SetPosition(int x, int y)
{
	this->posX = x;
	this->posY = y;
}

void Font::SetMessageWithPosition(char* newMessage, int size, int x, int y)
{

	char* tmp = newMessage;

	GraphicsObject_Sprite* tmpFontSprite;
	SpriteModel* pSpriteModel = (SpriteModel*)ModelMan::Find(Model::NAME::SPRITE);
	ShaderObject* pShaderObject_sprite = ShaderMan::Find(ShaderObject::NAME::SPRITE);
	Glyph* pGlyph = GlyphMan::Find((int)*tmp);
	if (pGlyph == nullptr) {
		pGlyph = GlyphMan::Find(32);
		messagelength += 1;
	}
	float locX = (float)x;
	float locY = (float)y;
	float locW = 0.0f;
	float locH = 0.0f;
	// Create/Load Shader 

	for (int i = 0; i < size - 1; i++) {
		//Trace::out("%d, %c\n", i, *tmp);
		pGlyph = GlyphMan::Find((int)*tmp);
		locW = pGlyph->imageRect.width;
		locH = pGlyph->imageRect.height;
		tmpFontSprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pGlyph, Rect(locX, locY, locW, locH));

		GameObject2D* pFont_Obj = new GameObject2D(tmpFontSprite);
		////UNUSED_VAR(pFont_Obj);
		GameObjectMan::Add(pFont_Obj, GameObjectMan::GetRoot());

		locX += locW * 0.9f;


		tmp++;
	}
	//Trace::out("%d", size);

	this->posX = x;
	this->posY = y;
}

void Font::SetDynamicMessage(Font::NAME _name, char* newMessage, int size, int x, int y)
{
	char* tmp = newMessage;
	float locX = (float)x;
	float locY = (float)y;
	float locW = 0.0f;
	float locH = 0.0f;

	SpriteModel* pSpriteModel = (SpriteModel*)ModelMan::Find(Model::NAME::SPRITE);
	ShaderObject* pShaderObject_sprite = ShaderMan::Find(ShaderObject::NAME::SPRITE);
	Glyph* pGlyph = GlyphMan::Find((int)*tmp);
	if (pGlyph == nullptr) {
		pGlyph = GlyphMan::Find(32);
		messagelength += 1;
	}
	GraphicsObject_Sprite* tmpFontSprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pGlyph, Rect((float)x, (float)y, pGlyph->imageRect.width, pGlyph->imageRect.height));

	GameObject2D* pFont_Obj = new GameObject2D(tmpFontSprite);
	////UNUSED_VAR(pFont_Obj);
	GameObjectMan::Add(pFont_Obj, GameObjectMan::GetRoot());
	Add(new FontObj(pFont_Obj));
	//GameObjectMan::GetPCSTree()->Print();
	locX = (float)x + pGlyph->imageRect.width;


	// Create/Load Shader 

	for (int i = 1; i < size - 1; i++) {
		tmp++;
		//Trace::out("%d, %c\n", i, *tmp);
		pGlyph = GlyphMan::Find((int)*tmp);

		locW = pGlyph->imageRect.width;
		locH = pGlyph->imageRect.height;
		tmpFontSprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pGlyph, Rect(locX, locY, locW, locH));

		pFont_Obj = new GameObject2D(tmpFontSprite);
		////UNUSED_VAR(pFont_Obj);
		GameObjectMan::Add(pFont_Obj, GameObjectMan::GetRoot());
		Add(new FontObj(pFont_Obj));
		locX += locW;



	}
	//Trace::out("%d", size);

	this->posX = x;
	this->posY = y;
	this->name = _name;
	this->messagelength += size - 1;
}

void Font::Add(FontObj* pGameObj)
{

	if (poMsgList == 0)
	{
		poMsgList = pGameObj;
		pGameObj->next = 0;
		pGameObj->prev = 0;
	}
	else
	{
		FontObj* tmp = poMsgList;
		while (tmp->next != nullptr) {
			tmp = (FontObj*)tmp->next;
		}
		tmp->next = pGameObj;
		pGameObj->prev = tmp;
		pGameObj->next = nullptr;

	}
}

void Font::Remove(FontObj* pGameObj)
{
	if (pGameObj->prev != nullptr)
	{	// middle or last node
		pGameObj->prev->next = pGameObj->next;
	}
	else
	{  // first
		poMsgList = (FontObj*)pGameObj->next;
	}

	if (pGameObj->next != nullptr)
	{	// middle node
		pGameObj->next->prev = pGameObj->prev;
	}
}

