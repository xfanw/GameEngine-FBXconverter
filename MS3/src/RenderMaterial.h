//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef RENDER_MATERIAL_H
#define RENDER_MATERIAL_H

class RenderMaterial 
{
protected:
	virtual void SetState() = 0;
	virtual void SetDataGPU() = 0;
	virtual void Draw() = 0;
	virtual void RestoreState() = 0;

};

#endif

//--- End of File -------------------------------------------------------------

