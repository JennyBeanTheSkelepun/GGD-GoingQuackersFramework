#include "MovingObstacle.h"

MovingObstacle::MovingObstacle(GameObject* owner) : Component(owner, ComponentTypes::MOVINGOBSTACLE, "Moving Obstacle")
{
	startPos, endPos = Vector2(0, 0);
	speed = 1.f;
	delayMax, delayCurrent = 0.f;
	returning = false;
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
	if (EngineGuiClass::getInstance()->IsInPlayMode())
	{
		// staying still
		if (delayCurrent > 0 && delayMax > 0)
		{
			delayCurrent -= Time::GetDeltaTime();
			if (delayCurrent > 0) return;
		}
		// moving
		Vector2 currentPos = GetOwner()->GetTransform()->GetPosition();
		Vector2 deltaPos;
		Vector2 destination;
		if (!returning) destination = endPos;
		else destination = startPos;
		deltaPos = destination - currentPos;
		currentPos += deltaPos.Normalize() * Time::GetDeltaTime() * speed;
		// don't move past destination
		bool reachedX = false;
		bool reachedY = false;
		if ((deltaPos.X > 0 && currentPos.X > destination.X) || (deltaPos.X < 0 && currentPos.X < destination.X))
			currentPos.X = destination.X;
		if (currentPos.X == destination.X) reachedX = true;
		if ((deltaPos.Y > 0 && currentPos.Y > destination.Y) || (deltaPos.Y < 0 && currentPos.Y < destination.Y))
			currentPos.Y = destination.Y;
		if (currentPos.Y == destination.Y) reachedY = true;
		
		GetOwner()->GetTransform()->SetPosition(currentPos);
		// ready to switch sides
		if (reachedX && reachedY)
		{
			returning = !returning;
			if (delayMax > 0) delayCurrent = delayMax;
		}
	}
}
void MovingObstacle::ImGUIUpdate()
{
	float* startPosArr[2] = { &startPos.X, &startPos.Y };
	ImGui::InputFloat2("Start Position", startPosArr[0]);
	float* endPosArr[2] = { &endPos.X,&endPos.Y };
	ImGui::InputFloat2("End Position", endPosArr[0]);
	ImGui::InputFloat("Speed", &speed);
	ImGui::InputFloat("Delay", &delayMax);
}
json* MovingObstacle::SceneSave()
{
	json* returnObj = new json({
		{"Start Position", {{"X", startPos.X },{"Y", startPos.Y }}},
		{"End Position", {{"X", endPos.X },{"Y", endPos.Y }}},
		{"Speed", speed},
		{"Delay",{{"Maximum",delayMax},{"Current",delayCurrent}}},
		{"Is Returning",returning}
		});
	return returnObj;
}
void MovingObstacle::SceneLoad(json* componentJSON)
{
	startPos.X = (*componentJSON)["Start Position"]["X"];
	startPos.Y = (*componentJSON)["Start Position"]["Y"];
	endPos.X = (*componentJSON)["End Position"]["X"];
	endPos.Y = (*componentJSON)["End Position"]["Y"];
	speed = (*componentJSON)["Speed"];
	delayMax = (*componentJSON)["Delay"]["Maximum"];
	delayCurrent = (*componentJSON)["Delay"]["Current"];
	returning = (*componentJSON)["Is Returning"];
}