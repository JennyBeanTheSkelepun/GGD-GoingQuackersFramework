#include "KeyboardEvents.h"

KeyboardEvents::KeyboardEvents():type(EventTypes::Invalid)
{
}

KeyboardEvents::KeyboardEvents(const EventTypes type) : type(type)
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

