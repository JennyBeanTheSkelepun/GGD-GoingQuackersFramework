#ifndef _LINE_RENDERER_H_
#define _LINE_RENDERER_H_

#include "../../Data Structures/Vectors.h"
#include "Component.h"

class LineRenderer : public Component
{
public:
	LineRenderer(GameObject* owner);
	~LineRenderer();

	//- Basic Loops -//
	void OnDestroy() override;
	void Update() override;

	//- ImGui UpdateLoop -//
	void ImGUIUpdate() override;

	//- Scene Save and Load -//
	json* SceneSave() override;
	void SceneLoad(json* componentJSON) override;

	Vector2* GetStartPos() { return &m_startPos; };
	Vector2* GetEndPos() { return &m_endPos; };
	float* GetWidth() { return &mf_width; };
	Vector3* GetColour() { return &m_colour; };

	void SetStartPos(Vector2 a_startPos) { m_startPos = a_startPos; };
	void SetEndPos(Vector2 a_endPos) { m_endPos = a_endPos; };
	void SetWidth(float af_width) { mf_width = af_width; };
	void SetColour(Vector3 a_colour) { m_colour = a_colour; }

private:
	Vector2 m_startPos;
	Vector2 m_endPos;
	float mf_width;
	Vector3 m_colour;
};

#endif //- _LINE_RENDERER_H_ -//