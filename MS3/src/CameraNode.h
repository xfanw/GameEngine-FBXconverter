
#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

class Camera;

class CameraLink
{
public:
	CameraLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	CameraLink *next;
	CameraLink *prev;
	virtual ~CameraLink() {}
};

class CameraNode: public CameraLink
{
public:
	CameraNode();
	void set( Camera *pCamera);
	virtual ~CameraNode();
public:
	Camera *poCamera;

};

#endif

//--- End of File -------------------------------------------------------------
