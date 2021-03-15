#ifndef _INPUT_H_
#define _INPUT_H_
//#include "../Input/KeyboardEvents.h"
#include <queue>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Initialize();


	void KeyDown(unsigned int ai_input);
	void KeyUp(unsigned int ai_input);

	bool isKeyDown(unsigned int ai_key);
	bool isKeyUp(unsigned int ai_key);
	bool isKeyHeld(unsigned int ai_key);

	void movement(unsigned int ai_key);

	void changeKey(unsigned int ai_key, unsigned char changed_key);

	unsigned char MoveForward;
	unsigned char MoveLeft;
	unsigned char MoveRight;
	unsigned char MoveBack;
	

private:
	bool mb_keys[256];
	unsigned char DefaultForward;
	unsigned char DefaultLeft;
	unsigned char DefaultRight;
	unsigned char DefaultBack;
};

#endif