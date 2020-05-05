#include "SpeedControl.h"
#include "InputMan.h"
#include "Keyboard.h"
#include "Framework.h"
#include "AnimManager.h"
#include "CameraMan.h"
void SpeedControl::SpeedInput()
{

	Keyboard* key = InputMan::GetKeyboard();
	if (key->GetKeyState(Keyboard::KEY_ARROW_UP))
	{
		increment();
	}

	if (key->GetKeyState(Keyboard::KEY_ARROW_DOWN))
	{
		decrement();
	}

	if (key->GetKeyState(Keyboard::KEY_B))
	{
		backward();
	}

	if (key->GetKeyState(Keyboard::KEY_R))
	{
		reset();
	}

	if (key->GetKeyState(Keyboard::KEY_1)) {
		AnimManager::SetCurrent(Anim::Name::BEAR_WALK);
		CameraMan::SetCurrent(Camera::Name::CAMERA_BEAR, Camera::Type::PERSPECTIVE_3D);
	}
	if (key->GetKeyState(Keyboard::KEY_2)) {
		AnimManager::SetCurrent(Anim::Name::BEAR_IDLE);
		CameraMan::SetCurrent(Camera::Name::CAMERA_BEAR, Camera::Type::PERSPECTIVE_3D);
	}
	if (key->GetKeyState(Keyboard::KEY_3)) {
		AnimManager::SetCurrent(Anim::Name::WLAK_RUN);
		CameraMan::SetCurrent(Camera::Name::CAMERA_HUMAN, Camera::Type::PERSPECTIVE_3D);
	}
	if (key->GetKeyState(Keyboard::KEY_4)) {
		AnimManager::SetCurrent(Anim::Name::HUMAN_RUN);
		CameraMan::SetCurrent(Camera::Name::CAMERA_HUMAN, Camera::Type::PERSPECTIVE_3D);
	}
	if (key->GetKeyState(Keyboard::KEY_5)) {
		AnimManager::SetCurrent(Anim::Name::HUMAN_PUNCK);
		CameraMan::SetCurrent(Camera::Name::CAMERA_HUMAN, Camera::Type::PERSPECTIVE_3D);
	}
	if (key->GetKeyState(Keyboard::KEY_6)) {
		AnimManager::SetCurrent(Anim::Name::HUMAN_SHOT);
		CameraMan::SetCurrent(Camera::Name::CAMERA_HUMAN, Camera::Type::PERSPECTIVE_3D);
	}
	   
}
