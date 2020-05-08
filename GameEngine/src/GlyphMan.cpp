
#include <assert.h>
#include "GlyphMan.h"
#include <fstream>
#include <string>
#include "Framework.h"
GlyphMan::GlyphMan() {
	this->active = 0;
}
void GlyphMan::Add(Glyph* pGlyph)
{
	// Get the instance to the manager
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);

	// Create a Image
	assert(pGlyph);

	// Now add it to the manager
	pGlyphMan->privAddToFront(pGlyph, pGlyphMan->active);

}

void GlyphMan::Create()
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);
}

void GlyphMan::Destroy()
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);

	Glyph* pLink = pGlyphMan->active;

	while (pLink != nullptr)
	{
		Glyph* pTmp = pLink;
		pLink = (Glyph*)pLink->next;
		pGlyphMan->privRemove(pTmp, pGlyphMan->active);
		delete pTmp;
	}
}

Glyph* GlyphMan::Find(int key)
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);

	Glyph* pNode = (Glyph*)pGlyphMan->active;
	while (pNode != 0)
	{
		if (pNode->key == key)
		{
			// found it
			break;
		}

		pNode = (Glyph*)pNode->next;
	}

	return pNode;
}

void GlyphMan::AddXml()
{
	using namespace std;
	// read xml file and create all objects in an object pool
	
	ifstream infile;
	Glyph* pFont;
	infile.open("Consolas36pt.xml");
	assert(infile);
	while (!infile.eof()) // To get you all the lines.
	{
		string line;
		unsigned int keyPos;
		getline(infile, line); // Saves the line in STRING.
		

		keyPos = line.find("key");
		
		if (keyPos != string::npos) {
			int objKey = GlyphMan::getValue(line, '\"', '\"');
			getline(infile, line);
			int objX = GlyphMan::getValue(line, '>', '<');
			//Trace::out("X is %d\n", objX);
			getline(infile, line);
			int objY = GlyphMan::getValue(line, '>', '<');
			//Trace::out("Y is %d\n", objY);
			getline(infile, line);
			int objW = GlyphMan::getValue(line, '>', '<');
			//Trace::out("width is %d\n", objW);
			getline(infile, line);
			int objH = GlyphMan::getValue(line, '>', '<');
			//Trace::out("height is %d\n", objH);
			//Trace::out("%d\n", objKey);
			pFont = new Glyph((float) objX, (float) objY, (float) objW, (float) objH, objKey);
			assert(pFont);
			GlyphMan::Add(pFont);
		}
	}
	infile.close();


	//Glyph* pFont = new Glyph(Rect(16, 0, 10, 41), 33);
	//GlyphMan::Add(pFont);
	//pFont = new Glyph(Rect(26, 0, 10, 41), 34);
	//GlyphMan::Add(pFont);
}

GlyphMan* GlyphMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static GlyphMan glyphMan;
	return &glyphMan;
}

void GlyphMan::privAddToFront(Glyph* node, Glyph*& head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void GlyphMan::privRemove(Glyph* node, Glyph*& head)
{
	assert(node);

	if (node->prev != nullptr)
	{	// middle or last node
		node->prev->next = node->next;
	}
	else
	{  // first
		head = (Glyph*)node->next;
	}

	if (node->next != nullptr)
	{	// middle node
		node->next->prev = node->prev;
	}
}

int GlyphMan::getValue(std::string& line, char startChar, char endChar)
{
	using namespace std;
	unsigned int start = line.find(startChar);
	unsigned int end = line.find(endChar, start + 1);
	//Trace::out("\" is: %d,%d\n", start, end);
	string keyInString = line.substr(start + 1, end - start - 1);
	return stoi(keyInString);
}
