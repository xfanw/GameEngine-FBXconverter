//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

#include "sb7.h"

#include "MathEngine.h"
#include "Camera.h"

Camera::CullResult Camera::CullTest(const Sphere &Sphere)
{
	CullResult result = CULL_INSIDE;

	// first test
	Vect A = Sphere.cntr - this->nearTopLeft;

	if ((A.dot(this->topNorm) > Sphere.rad) ||
		(A.dot(this->leftNorm) > Sphere.rad) ||
		(A.dot(this->frontNorm) > Sphere.rad))
	{
		result = Camera::CULL_OUTSIDE;
	}
	else
	{
		Vect B = Sphere.cntr - this->farBottomRight;

		if ((B.dot(this->backNorm) > Sphere.rad) ||
			(B.dot(this->rightNorm) > Sphere.rad) ||
			(B.dot(this->bottomNorm) > Sphere.rad))
		{
			result = Camera::CULL_OUTSIDE;
		}
	}

	return result;
}


void Camera::getHelper(Vect &up, Vect &tar, Vect &pos, Vect &upNorm, Vect &forwardNorm, Vect &pRightNorm)
{
	this->getPos(pos);
	this->getLookAt(tar);
	this->getUp(upNorm);
	up = pos + upNorm;

	forwardNorm = tar - pos;
	forwardNorm.norm();

	this->getRight(pRightNorm);
}

void Camera::setHelper(Vect &up, Vect &tar, Vect &pos)
{
	Vect upVect = up - pos;
	this->setOrientAndPosition(upVect, tar, pos);
}

// default constructor
Camera::Camera(Camera::Type _camType)
{
	//out("Camera(): ---------\n");
	this->name = Camera::Name::NOT_INITIALIZED;
	this->camType = _camType;
}


Camera::~Camera()
{

}

Camera::Name Camera::getName() const
{
	return this->name;
}
void Camera::setName(Camera::Name inName)
{
	this->name = inName;
}

// critical informational knobs for the perspective matrix
// Field of View Y is in degrees (copying lame OpenGL)
void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
{
	this->aspectRatio = Aspect;
	this->fovy = Fovy;
	this->nearDist = NearDist;
	this->farDist = FarDist;
}

int Camera::getScreenWidth() const
{
	return this->viewport_width;
}

int Camera::getScreenHeight() const
{
	return this->viewport_height;
}

void Camera::setOrthographic(const float _xMin, const float _xMax, const float _yMin, const float _yMax, const float _zMin, const float _zMax)
{
	assert(this->camType == Camera::Type::ORTHOGRAPHIC_2D);

	this->xMin = _xMin;
	this->yMin = _yMin;
	this->zMin = _zMin;

	this->xMax = _xMax;
	this->yMax = _yMax;
	this->zMax = _zMax;

	this->far_height = this->yMax - this->yMin;
	this->far_width = this->xMax - this->xMin;
	this->near_width = this->yMax - this->yMin;
	this->nearDist = this->zMin;
	this->farDist = this->zMax;
};

// Just a pass through to setup the view port screen sub window
void Camera::setViewport(const int inX, const int inY, const int width, const int height)
{
	this->viewport_x = inX;
	this->viewport_y = inY;
	this->viewport_width = width;
	this->viewport_height = height;

	this->privSetViewState();
};

// Simple wrapper
void Camera::privSetViewState(void)
{
	glViewport(this->viewport_x, this->viewport_y, this->viewport_width, this->viewport_height);
};

// Goal, calculate the near height / width, same for far plane 
void Camera::privCalcPlaneHeightWidth(void)
{

	this->near_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->nearDist;
	this->near_width = this->near_height * this->aspectRatio;

	this->far_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->farDist;
	this->far_width = this->far_height * this->aspectRatio;

};

void Camera::setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &inPos)
{
	// Remember the up, lookAt and right are unit, and are perpendicular.
	// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
	// Make sure that all vectors are unit vectors.

	this->vLookAt = inLookAt;

	// Point out of the screen into your EYE
	this->vDir = inPos - inLookAt;
	this->vDir.norm();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross(this->vDir);
	this->vRight.norm();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.norm();

	this->vPos = inPos;
};

void Camera::privCalcFrustumVerts(void)
{
	// Top Left corner and so forth.  In this form to see the pattern
	// Might be confusing (remember the picture) vDir goes from screen into your EYE
	// so distance from the eye is "negative" vDir
	this->nearTopLeft =			this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearTopRight =		this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->nearBottomLeft =		this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearBottomRight =		this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->farTopLeft =			this->vPos - this->vDir * this->farDist  + this->vUp * this->far_height * 0.5f  - this->vRight * this->far_width * 0.5f;
	this->farTopRight =			this->vPos - this->vDir * this->farDist  + this->vUp * this->far_height * 0.5f  + this->vRight * this->far_width * 0.5f;
	this->farBottomLeft =		this->vPos - this->vDir * this->farDist  - this->vUp * this->far_height * 0.5f  - this->vRight * this->far_width * 0.5f;
	this->farBottomRight =		this->vPos - this->vDir * this->farDist  - this->vUp * this->far_height * 0.5f  + this->vRight * this->far_width * 0.5f;
};

void Camera::privCalcFrustumCollisionNormals(void)
{
	// Normals of the frustum around nearTopLeft
	Vect A = this->nearBottomLeft - this->nearTopLeft;
	Vect B = this->nearTopRight - this->nearTopLeft;
	Vect C = this->farTopLeft - this->nearTopLeft;

	this->frontNorm = A.cross(B);
	this->frontNorm.norm();

	this->leftNorm = C.cross(A);
	this->leftNorm.norm();

	this->topNorm = B.cross(C);
	this->topNorm.norm();

	// Normals of the frustum around farBottomRight
	A = this->farBottomLeft - this->farBottomRight;
	B = this->farTopRight - this->farBottomRight;
	C = this->nearBottomRight - this->farBottomRight;

	this->backNorm = A.cross(B);
	this->backNorm.norm();

	this->rightNorm = B.cross(C);
	this->rightNorm.norm();

	this->bottomNorm = C.cross(A);
	this->bottomNorm.norm();
};


// The projection matrix (note it's invertable)
void Camera::privUpdateProjectionMatrix(void)
{
	if (this->camType == Camera::Type::PERSPECTIVE_3D)
	{
		this->projMatrix[m0] = 2.0f * this->nearDist / this->near_width;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = 2.0f * this->nearDist / this->near_height;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
		this->projMatrix[m11] = -1.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
		this->projMatrix[m15] = 0.0f;
	}
	else
	{
		this->projMatrix[m0] = 2.0f / (xMax - xMin);
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = 2.0f / (yMax - yMin);
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = -2.0f / (zMax - zMin);
		this->projMatrix[m11] = 0.0f;

		this->projMatrix[m12] = 0;
		this->projMatrix[m13] = 0;
		this->projMatrix[m14] = -(zMax + zMin) / (zMax - zMin);
		this->projMatrix[m15] = 1.0f;

	}

};

void Camera::privUpdateViewMatrix(void)
{
	// This functions assumes the your vUp, vRight, vDir are still unit
	// And perpendicular to each other
	//  view = Rot(orient) * trans(-(eye.basis) )

	this->viewMatrix[m0] = this->vRight[x];
	this->viewMatrix[m1] = this->vUp[x];
	this->viewMatrix[m2] = this->vDir[x];
	this->viewMatrix[m3] = 0.0f;

	this->viewMatrix[m4] = this->vRight[y];
	this->viewMatrix[m5] = this->vUp[y];
	this->viewMatrix[m6] = this->vDir[y];
	this->viewMatrix[m7] = 0.0f;

	this->viewMatrix[m8] = this->vRight[z];
	this->viewMatrix[m9] = this->vUp[z];
	this->viewMatrix[m10] = this->vDir[z];
	this->viewMatrix[m11] = 0.0f;

	// Change of basis (dot with the basis vectors)
	this->viewMatrix[m12] = -vPos.dot(vRight);
	this->viewMatrix[m13] = -vPos.dot(vUp);
	this->viewMatrix[m14] = -vPos.dot(vDir);
	this->viewMatrix[m15] = 1.0f;
};


// Update everything (make sure it's consistent)
void Camera::updateCamera(void)
{
	// First find the near height/width, far height/width
	this->privCalcPlaneHeightWidth();

	// Find the frustum physical verts
	this->privCalcFrustumVerts();

	// find the frustum collision normals
	this->privCalcFrustumCollisionNormals();

	// update the projection matrix
	this->privUpdateProjectionMatrix();

	// update the view matrix
	this->privUpdateViewMatrix();
}


// Accessor mess:
Camera::Type Camera::getType() const
{
	return this->camType;
}

Matrix & Camera::getViewMatrix(void)
{
	return this->viewMatrix;
}

Matrix & Camera::getProjMatrix(void)
{
	return this->projMatrix;
}

void Camera::getPos(Vect &outPos) const
{
	outPos = this->vPos;
}

void  Camera::getDir(Vect &outDir) const
{
	outDir = this->vDir;
}

void  Camera::getUp(Vect &outUp) const
{
	outUp = this->vUp;
}

void Camera::getLookAt(Vect &outLookAt) const
{
	outLookAt = this->vLookAt;
}

void Camera::getRight(Vect &outRight) const
{
	outRight = this->vRight;
}

void Camera::getFieldOfView(float &Value) const
{
	Value = this->fovy;
};

void Camera::setFieldOfView(const float Value)
{
	this->fovy = Value;
};

void Camera::getNearDist(float &Value) const
{
	Value = this->nearDist;
}

void Camera::setNearDist(const float Value)
{
	this->nearDist = Value;
}

void Camera::getNearTopLeft(Vect &vOut) const
{
	vOut = this->nearTopLeft;
}
void Camera::getNearTopRight(Vect &vOut) const
{
	vOut = this->nearTopRight;
}
void Camera::getNearBottomLeft(Vect &vOut)const
{
	vOut = this->nearBottomLeft;
}
void Camera::getNearBottomRight(Vect &vOut) const
{
	vOut = this->nearBottomRight;
}
void Camera::getFarTopLeft(Vect &vOut) const
{
	vOut = this->farTopLeft;
}
void Camera::getFarTopRight(Vect &vOut) const
{
	vOut = this->farTopRight;
}
void Camera::getFarBottomLeft(Vect &vOut) const
{
	vOut = this->farBottomLeft;
}
void Camera::getFarBottomRight(Vect &vOut)const
{
	vOut = this->farBottomRight;
}


//--- End of File -------------------------------------------------------------
