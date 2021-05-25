#include "Input.h"
#include <iostream>
#include "../ImGui/ImGui SourceCode/imgui.h"

Input* Input::SingletonInstance = nullptr;

Input* Input::getInstance()
{
	if (SingletonInstance == 0)
		SingletonInstance = new Input();

	return SingletonInstance;
}

Input::Input()
{
	//- Create Required Code -//
	mb_heldKeys = new bool[256]{ false };
	mb_pressedDownKeys = new bool[256]{ false };
	mb_pressedUpKeys = new bool[256]{ false };
}

Input::~Input()
{
	//- Clear all arrays from heap -//
	delete[] mb_heldKeys;
	delete[] mb_pressedDownKeys;
	delete[] mb_pressedUpKeys;
}

void Input::Update()
{
	//- resetting both arrays to false via low level memory manipulation -//
	memset(&mb_pressedDownKeys[0], false, sizeof(bool) * 256);
	memset(&mb_pressedUpKeys[0], false, sizeof(bool) * 256);

	//- ImGui Mouse Pos Get -//
	screenMousePos = ImGui::GetMousePos();

	//worldMousePos.X = (2 * screenMousePos.X - 2 * X - Width) / Width;
	//worldMousePos.Y = (-2 * screenMousePos.Y + 2 * Y + Height) / Height;
	worldMousePos.Z = 1;

	//worldMousePos = (1 / MPx) * (2 * sx - 2 * X - Width) / Width
	//py = (1 / MPy) * (-2 * sy + 2 * Y + Height) / Height
	//pz = depth buffer
}

Vector2 Input::GetScreenSpaceMousePos()
{
	return screenMousePos;
}

Vector3 Input::GetWorldSpaceMousePos()
{
	return worldMousePos;
}

void Input::KeyDown(unsigned int ai_input)
{
	//- Use same function to set both arrays -//
	mb_heldKeys[ai_input] = true;
	mb_pressedDownKeys[ai_input] = true;
}

void Input::KeyUp(unsigned int ai_input)
{
	//- Use same function to set both arrays -//
	mb_heldKeys[ai_input] = false;
	mb_pressedUpKeys[ai_input] = true;
	//- above array is set to true to return true if the keys just been releced this frame-//
}

//- For the functions below using c++ casting from enumClass to unsigned int acsessing it and returning -//
bool Input::isKeyHeldDown(KeyCode ai_key)
{
	return mb_heldKeys[static_cast<unsigned int>(ai_key)];
}

bool Input::isKeyHeldUp(KeyCode ai_key)
{
	return !mb_heldKeys[static_cast<unsigned int>(ai_key)];
}

bool Input::isKeyPressedDown(KeyCode ai_key)
{
	return mb_pressedDownKeys[static_cast<unsigned int>(ai_key)];
}

bool Input::isKeyPressedUp(KeyCode ai_key)
{
	return mb_pressedUpKeys[static_cast<unsigned int>(ai_key)];
}
