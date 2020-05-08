//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <sb7.h>
#include <assert.h>

#include "NullModel.h"


NullModel::NullModel(const char * const modelFileName)
	: Model()
{
	UNUSED_VAR(modelFileName);
}

NullModel::~NullModel()
{
	// remove anything dynamic here
}

void NullModel::privCreateVAO(const char * const modelFileName)
{
	UNUSED_VAR(modelFileName);
}

//--- End of File ------------------------------------------------------------


