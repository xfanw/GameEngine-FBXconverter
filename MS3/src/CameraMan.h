//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "CameraNode.h"

// Singleton
class CameraMan
{
public:
	static void Add(Camera::Name name, Camera *pCamera);
	static Camera * Find( Camera::Name name);
	static void SetCurrent( Camera::Name name, Camera::Type type);
	static Camera * GetCurrent(Camera::Type type);

	static void Create();
	static void Destroy();

private:  // methods

	static CameraMan *privGetInstance();
	CameraMan();
	void privAddToFront(CameraLink *node, CameraLink *&head);
	void privRemove(CameraLink *node, CameraLink *&head);

private:  // add

	CameraLink *active;

	// Data
	Camera *pCamOrthographic;
	Camera *pCamPerspective;

};

#endif

//--- End of File -------------------------------------------------------------
