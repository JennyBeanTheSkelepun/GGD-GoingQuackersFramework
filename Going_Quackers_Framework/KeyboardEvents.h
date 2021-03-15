#pragma once
class KeyboardEvents
{
public:
	enum EventTypes
	{
		Press,
		Release,
		Invalid
	};
	KeyboardEvents();
	KeyboardEvents(const EventTypes type);
	bool IsPressed() const;
	bool IsReleased() const;
	bool IsValid() const;
private:
	EventTypes type;
};
