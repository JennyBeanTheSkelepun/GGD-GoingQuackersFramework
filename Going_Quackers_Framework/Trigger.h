#include "Collision.h"
#include "Code/EngineBase/Game Systems/Components/Component.h"

class Trigger : public Component
{
public:
	void Update() override;

	bool GetCollidingBool() { return m_isColliding; }
	std::vector<GameObject*> GetCollidingObjects() { return m_collidiedObjects; }

private:
	std::vector<GameObject*> m_collidiedObjects;
	bool m_isColliding;

	Collision collision;
};

