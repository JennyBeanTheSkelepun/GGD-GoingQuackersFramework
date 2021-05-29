#include "Rope.h"

Rope::Rope(GameObject* ap_owner) : Component(ap_owner, ComponentTypes::ROPE, "Rope") {
}

Rope::~Rope() {
}

void Rope::OnDestroy() {
	this->~Rope();
}

void Rope::Update() {
}

void Rope::ImGUIUpdate() {
}

json* Rope::SceneSave() {
	json* returnObj = new json({
		});

	return returnObj;
}

void Rope::SceneLoad(json* componentJSON) {
}

GameObject* Rope::GetRopeEnd() {
	return nullptr;
}
