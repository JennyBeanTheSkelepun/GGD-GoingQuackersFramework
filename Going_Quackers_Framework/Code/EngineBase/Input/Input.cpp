#include "Input.h"

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
