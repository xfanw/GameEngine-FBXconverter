
#include <assert.h>

#include "CameraNode.h"
#include "Camera.h"

CameraNode::CameraNode( )
{
	this->poCamera = nullptr;
}

void CameraNode::set( Camera *camera )
{
	assert(camera);
	this->poCamera = camera;
}

CameraNode::~CameraNode()
{
	delete this->poCamera;
}
//--- End of File -------------------------------------------------------------
