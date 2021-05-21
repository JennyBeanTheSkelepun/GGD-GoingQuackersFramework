#include "Input.h"
#include <iostream>

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

//
//bool Input::IsKeyQueueEmpty()
//{
//	if (KeyQueue.empty())
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//int Input::queueSize()
//{
//	return KeyQueue.size();
//}
//
//KeyboardEvents Input::readKeyQueue()
//{
//	if (KeyQueue.empty())
//	{
//		return KeyboardEvents();
//	}
//	else
//	{
//		KeyboardEvents keyEvent = KeyQueue.front();
//		KeyQueue.pop();
//		return keyEvent;
//	}
//}
//
////function for changing the movement keys
////takes the key pressed and the current movement key i.e. the current moveLeft can be taken and it will also take the key being pressed
//void Input::changeKey(unsigned int ai_key, unsigned char changed_key)
//{
//	if (isKeyHeldDown(ai_key))
//	{
//		//changes the key carried in i.e. moveLeft and changes it's value to the key pressed
//		changed_key = ai_key;
//	}
//}