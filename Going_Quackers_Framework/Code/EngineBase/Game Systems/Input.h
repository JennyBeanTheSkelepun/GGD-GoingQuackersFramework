#ifndef _INPUT_H_
#define _INPUT_H_

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

private:
	bool mb_keys[256];
};

#endif