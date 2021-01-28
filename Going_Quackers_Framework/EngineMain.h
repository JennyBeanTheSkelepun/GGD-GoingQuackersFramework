#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#include <iostream>

class EngineMain
{
public:
	EngineMain();
	~EngineMain();

	void Update();
	void Render();

	bool mb_isClosed = false;

	bool IsClosed() { return mb_isClosed; };

private:


};

#endif // !ENGINE_MAIN_H

