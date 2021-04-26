#pragma once
class KeyboardEvents
{
public:
	enum class EventTypes
	{
		Press,
		Release,
		Invalid
	};
	KeyboardEvents();
	KeyboardEvents(const EventTypes type,unsigned int key);
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsValid() const;
	unsigned int GetKey() const;
private:
	EventTypes type;
	unsigned int key;
};
