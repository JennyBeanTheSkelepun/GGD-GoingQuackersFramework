#include "KillPlayer.h"

KillPlayer::KillPlayer(GameObject* owner) : Component(owner, ComponentTypes::KillPLAYER, "Kill Player")
{
	KillObject = this->GetOwner();
}
KillPlayer::~KillPlayer()
{
	KillObject = nullptr;
}
void KillPlayer::OnDestroy()
{
	this->~KillPlayer();
}
void KillPlayer::Update()
{

}
void KillPlayer::ImGUIUpdate()
{

}
json* KillPlayer::SceneSave()
{

}
void KillPlayer::SceneLoad(json* componentJSON)
{

}
void KillPlayer::KillThePlayer()
{
	if (KillObject->GetComponent<Rigidbody>() == nullptr)
	{
		Debug::getInstance()->LogError("the player doesn't have a rigidbody component");
	}
	else if (KillObject->GetComponent<Rigidbody>()->GetCollidedObjects().empty() != true)
	{
		playerObj = KillObject->GetComponent<Rigidbody>()->GetCollidedObjects();
		
		for (GameObject* obj : playerObj)
		{
			if (obj->GetComponent<Player>() != nullptr)
			{
				obj->GetComponent<Player>()->Die();
			}
		}
	}
}