//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SSBO_H
#define SSBO_H

#include <sb7.h>

// Shader Storage Buffer Object
class SSBO 
{
public:
	enum class Access
	{
		READ_ONLY  = 0x88B8,
		WRITE_ONLY = 0x88B9,
		READ_WRITE = 0x88BA
	};
	
public:
	//Big 4
	SSBO();
	SSBO(const SSBO &) = delete;
	const SSBO & operator = (const SSBO &) = delete;
	~SSBO();



	void Set(unsigned int elementCount, unsigned int size, void * inData);

	unsigned int GetLocation() const;
	void Bind(unsigned bindLocation);
	void *Map(SSBO::Access accessType);
	void Unmap();

private:
	GLuint ssbo;
	unsigned int elementCount;
	unsigned int elementSize;
	void * pBuffer;
};

#endif

//--- End of File -------------------------------------------------------------
