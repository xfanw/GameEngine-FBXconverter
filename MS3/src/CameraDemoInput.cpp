#include "CameraDemoInput.h"
#include "InputMan.h"
#include "Keyboard.h"
#include "CameraMan.h"

void CameraDemoInput()
{
	Keyboard *key = InputMan::GetKeyboard();

	if (key->GetKeyState(Keyboard::KEY_SPACE))
	{
		Camera *pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

		if (pCamera->getName() == Camera::Name::CAMERA_0)
		{
			CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::PERSPECTIVE_3D);
		}
		else
		{
			CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
		}
	}


	if (key->GetKeyState(Keyboard::KEY_0))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_1))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_2))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_2, Camera::Type::PERSPECTIVE_3D);
	}

	if (key->GetKeyState(Keyboard::KEY_3))
	{
		CameraMan::SetCurrent(Camera::Name::CAMERA_3, Camera::Type::PERSPECTIVE_3D);
	}


}

//--- End of File -------------------------------------------------------------
