#ifndef _INPUT_H_
#define _INPUT_H_
//#include "KeyboardEvents.h"

#include "../Data Structures/Vectors.h"

enum class KeyCode
{
	LeftMouse = 0x01,
	RightMouse = 0x02,
	MiddleMouse = 0x04,
	Backspace = 0x08,
	Tab = 0x09,
	Clear = 0x0C,
	Enter = 0x0D,
	Shift = 0x10,
	Control = 0x11,
	CapsLock = 0x14,
	ESCAPE = 0X1B,
	Space = 0x20,
	ArrowLeft = 0x25,
	ArrowUp = 0x26,
	ArrowRight = 0x27,
	ArrowDown = 0x28,
	Delete = 0X2E,
	Zero = 0X30,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine = 0X39,
	A = 0X41,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z = 0X5A
};

class Input
{
public:
	//- Singleton Decliration -//
public:
	static Input* getInstance();

	Input(Input const&) = delete;
	void operator=(Input const&) = delete;

private:
	static Input* SingletonInstance;
	Input();

public:
	~Input();

	void KeyDown(unsigned int ai_input);
	void KeyUp(unsigned int ai_input);

	bool isKeyHeldDown(KeyCode ai_key);
	bool isKeyHeldUp(KeyCode ai_key);

	bool isKeyPressedDown(KeyCode ai_key);
	bool isKeyPressedUp(KeyCode ai_key);

	void Update();

	Vector2 GetScreenSpaceMousePos();
	Vector3 GetWorldSpaceMousePos();

private:
	bool* mb_heldKeys;
	bool* mb_pressedDownKeys;
	bool* mb_pressedUpKeys;

	Vector2 mousePos;
};

#endif