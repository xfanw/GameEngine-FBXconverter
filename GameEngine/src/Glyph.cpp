#include "Glyph.h"

Glyph::Glyph()
{
	key = 0;
}

Glyph::Glyph(Rect rect, int key):Image(Image::Name::FONT, Texture::Name::CONSOLAS, rect )
{
	this->key = key;
}

Glyph::Glyph(float x, float y, float w, float h, int key): Image(Image::Name::FONT, Texture::Name::CONSOLAS, Rect(x, y, w, h))
{
	this->key = key;
}
