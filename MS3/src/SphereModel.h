//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include "Model.h"

class SphereModel : public Model
{
public:
	SphereModel( const char * const modelFileName );
	~SphereModel();

	// tells the compiler do not create or allow it to be used, c++11
	SphereModel(const SphereModel &) = delete;
	SphereModel& operator=(const SphereModel & other) = delete;

private:
	void privCreateVAO( const char * const modelFileName  ) override;

};

#endif

// --- End of File ---------------------------------------------
