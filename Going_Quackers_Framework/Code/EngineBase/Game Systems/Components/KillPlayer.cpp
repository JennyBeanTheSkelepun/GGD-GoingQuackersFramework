#include "KillPlayer.h"

KillPlayer::KillPlayer(GameObject* owner) : Component(owner, ComponentTypes::KILLPLAYER, "Kill Player")
{
	KillObj = this->GetOwner();
	if (KillObj->GetComponent<Rigidbody>() == nullptr)
	{
		Debug::getInstance()->LogError("the kill object doesn't have a rigidbody component");
	}
	else
		killRB = KillObj->GetComponent<Rigidbody>();
}
KillPlayer::~KillPlayer()
{
	KillObj = nullptr;
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
	if (killRB == nullptr)
	{
		return;
	}
	else if (killRB->GetCollidedObjects().empty() != true)
	{
		playerObj = killRB->GetCollidedObjects();
		
		for (GameObject* obj : playerObj)
		{
			if (obj->GetComponent<Player>() != nullptr)
			{
				obj->GetComponent<Player>()->Die();
			}
		}
	}
}