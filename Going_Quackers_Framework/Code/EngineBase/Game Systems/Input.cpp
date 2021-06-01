#include "Input.h"
#include <iostream>
#include "../ImGui/ImGui SourceCode/imgui.h"
#include "../Rendering/Graphics.h"

#include "Debug.h"

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
	firstUpdate = false;
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
	if (firstUpdate)
	{
		//- resetting both arrays to false via low level memory manipulation -//
		memset(&mb_pressedDownKeys[0], false, sizeof(bool) * 256);
		memset(&mb_pressedUpKeys[0], false, sizeof(bool) * 256);

		//- ImGui Mouse Pos Get -//
		ImGuiIO& io = ImGui::GetIO();
		screenMousePos = Vector2(io.MousePos.x, io.MousePos.y);

		VirtualCamera* temp = Graphics::getInstance()->GetActiveCamera();
		Vector3 camPos = Vector3(0, 0, -5);
		Vector2 winDim = Graphics::getInstance()->GetWindowDimensions();

		if (temp != nullptr)
		{
			camPos = temp->GetPosition();
		}

		worldMousePos.X = camPos.X + ((screenMousePos.X - (winDim.X / 2)) / winDim.X);
		worldMousePos.Y = camPos.Y + ((-screenMousePos.Y + (winDim.Y / 2)) / winDim.Y);
		worldMousePos.Z = -5;

		//Debug::getInstance()->Log(worldMousePos);

		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
		{
			if (ImGui::IsMouseClicked(i))
			{
				switch (i)
				{
				case 0:
					mb_heldKeys[(int)KeyCode::LeftMouse] = true;
					mb_pressedDownKeys[(int)KeyCode::LeftMouse] = true;
					break;
				case 1:
					mb_heldKeys[(int)KeyCode::RightMouse] = true;
					mb_pressedDownKeys[(int)KeyCode::RightMouse] = true;
					break;
				}
			}

			if (ImGui::IsMouseReleased(i))
			{
				switch (i)
				{
				case 0:
					mb_heldKeys[(int)KeyCode::LeftMouse] = false;
					mb_pressedUpKeys[(int)KeyCode::LeftMouse] = true;
					break;
				case 1:
					mb_heldKeys[(int)KeyCode::RightMouse] = false;
					mb_pressedUpKeys[(int)KeyCode::RightMouse] = true;
					break;
				}
			}
		}
	}
	else firstUpdate = true;
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
