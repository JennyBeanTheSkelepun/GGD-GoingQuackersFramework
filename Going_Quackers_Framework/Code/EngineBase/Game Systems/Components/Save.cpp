#include "Save.h"

Save::Save(GameObject* owner) : Component(owner, ComponentTypes::SAVE, "Save") {
	SaveManager::getInstance()->AddToSaving(mp_owner);
}

Save::~Save() {
	SaveManager::getInstance()->RemoveFromSaving(mp_owner);
}
