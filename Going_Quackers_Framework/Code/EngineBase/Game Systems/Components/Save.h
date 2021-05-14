#ifndef _Save_H_
#define _Save_H_

#include "Component.h"
#include "../SaveManager.h"

class Save : Component {
public:
	void Initialize() override;
	void Deconstruct() override;
};

#endif

