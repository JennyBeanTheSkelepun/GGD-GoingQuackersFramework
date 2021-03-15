#include "Input.h"
#include <iostream>

Input::Input()
{
}

Input::Input(const Input& other)
{
}

Input::~Input()
{
}

void Input::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		mb_keys[i] = false;
	}
	DefaultForward = 'w';
	DefaultBack = 's';
	DefaultLeft = 'a';
	DefaultRight = 'd';
	if (!MoveForward)
	{
		MoveForward = DefaultForward;
	}
	if (!MoveBack)
	{
		MoveBack = DefaultBack;
	}
	if (!MoveLeft)
	{
		MoveLeft = DefaultLeft;
	}
	if (!MoveRight)
	{
		MoveRight = DefaultRight;
	}
}

void Input::KeyDown(unsigned int ai_input)
{
	mb_keys[ai_input] = true;
}

void Input::KeyUp(unsigned int ai_input)
{
	mb_keys[ai_input] = false;
}

bool Input::isKeyDown(unsigned int ai_key)
{
	return mb_keys[ai_key];
}

bool Input::isKeyUp(unsigned int ai_key)
{
	if (mb_keys[ai_key] == true)
	{
		return false;
	}
	else if (mb_keys[ai_key] == false)
	{
		return true;
	}
	else
	{
		return true;
	}
}

bool Input::isKeyHeld(unsigned int ai_key)
{
	if (isKeyDown(ai_key)&&!isKeyUp(ai_key))
	{
		return mb_keys[ai_key];
	}
	else
	{
		return false;
	}
}

//function for changing the movement keys
//takes the key pressed and the current movement key i.e. the current moveLeft can be taken and it will also take the key being pressed
void Input::changeKey(unsigned int ai_key, unsigned char changed_key)
{
	if (isKeyDown(ai_key))
	{
		//changes the key carried in i.e. moveLeft and changes it's value to the key pressed
		changed_key = ai_key; 
	}
}

void Input::movement(unsigned int ai_key)
{
	if (isKeyHeld(ai_key))
	{
		
	}
	else if(isKeyDown(ai_key))
	{

	}
}