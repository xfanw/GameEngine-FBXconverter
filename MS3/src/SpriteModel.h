#ifndef SPRITE_MODEL_H
#define SPRITE_MODEL_H

#include "Model.h"

class SpriteModel : public Model
{
public:
	SpriteModel(const char * const modelFileName);
	~SpriteModel();

	// tells the compiler do not create or allow it to be used, c++11
	SpriteModel(const SpriteModel &) = delete;
	SpriteModel& operator=(const SpriteModel & other) = delete;

private:
	void privCreateVAO(const char * const modelFileName) override;



};

#endif