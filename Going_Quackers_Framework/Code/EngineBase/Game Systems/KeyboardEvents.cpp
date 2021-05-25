#include "KeyboardEvents.h"

KeyboardEvents::KeyboardEvents():type(EventTypes::Invalid),key(0u)
{
}

KeyboardEvents::KeyboardEvents(const EventTypes type,unsigned int key) : type(type),key(key)
{
}

bool KeyboardEvents::IsPressed() const
{
	return this->type == EventTypes::Press;
}

bool KeyboardEvents::IsReleased() const
{
	return this->type == EventTypes::Release;
}

bool KeyboardEvents::IsValid() const
{
	return this->type != EventTypes::Invalid;
}

unsigned int KeyboardEvents::GetKey() const
{
	return this->key;
}

