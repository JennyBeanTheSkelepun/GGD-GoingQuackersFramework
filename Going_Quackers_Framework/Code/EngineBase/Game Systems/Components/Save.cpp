#include "Save.h"

void Save::Initialize() {
	SaveManager::getInstance()->AddToSaving(mp_owner);
}

void Save::Deconstruct() {
	SaveManager::getInstance()->RemoveFromSaving(mp_owner);
}
