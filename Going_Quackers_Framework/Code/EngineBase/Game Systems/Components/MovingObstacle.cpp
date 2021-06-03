#include "MovingObstacle.h"

MovingObstacle::MovingObstacle(GameObject* owner) : Component(owner, ComponentTypes::MOVINGOBSTACLE, "Moving Obstacle")
{

}
MovingObstacle::~MovingObstacle()
{

}

void MovingObstacle::OnDestroy()
{
	// was complaining without class specifier
	MovingObstacle::~MovingObstacle();
}
void MovingObstacle::Update()
{

}
void MovingObstacle::ImGUIUpdate()
{

}
json* MovingObstacle::SceneSave()
{
	return nullptr;
}
void MovingObstacle::SceneLoad(json* componentJSON)
{

}