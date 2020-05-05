#pragma once
#include "GameObject2D.h"

class FontLink
{
public:
	FontLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~FontLink()
	{

	}

	FontLink* next;
	FontLink* prev;
};

class FontObj : public FontLink {
public :
	FontObj(GameObject2D* pFont) { pGameObj = pFont; }
public:
	GameObject2D* pGameObj;
};
class Font : public FontLink{
public :
	enum class NAME {
		DYNAMIC_1,
		DYNAMIC_2,
		STATIC,
		UNDEFINED
	};

public:
	Font();
	Font(char* message, int len, int x, int y);
	~Font();
	void UpdateMessage(char* newMessage, int size);
	void SetPosition(int x, int y);
	void SetMessageWithPosition(char* newMessage, int size, int x, int y);
	void SetDynamicMessage(Font::NAME name, char* newMessage, int size, int x, int y);
private:
	void Add(FontObj* pGameObj);
	void Remove(FontObj* pGameObj);
public:
	NAME name;
	int posX;
	int posY;
private:
	FontObj* poMsgList = nullptr;
	int messagelength = 0;


};