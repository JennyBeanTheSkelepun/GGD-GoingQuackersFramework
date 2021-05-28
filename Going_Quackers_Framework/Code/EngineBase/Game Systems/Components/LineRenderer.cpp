#include "LineRenderer.h"
#include "../../Rendering/Graphics.h"


LineRenderer::LineRenderer(GameObject* owner) : Component(owner, ComponentTypes::LINERENDERER, "Line Renderer")
{
	m_startPos = Vector2();
	m_endPos = Vector2();
	mf_width = 10;
	m_colour = Vector3(255,0,0);

	Graphics::getInstance()->AddLineRenderer(this);
}

LineRenderer::~LineRenderer()
{
	Graphics::getInstance()->RemoveLineRenderer(this);
}

void LineRenderer::OnDestroy()
{
	this->~LineRenderer();
}

void LineRenderer::Update()
{
}

void LineRenderer::ImGUIUpdate()
{
	ImGui::Text("Line Points");
	float* PosOne[2] = { &m_startPos.X, &m_startPos.Y};
	ImGui::InputFloat2("Start Pos", PosOne[0]);

	float* PosTwo[2] = { &m_endPos.X, &m_endPos.Y};
	ImGui::InputFloat2("End Pos", PosTwo[0]);

	ImGui::InputFloat("Width", &mf_width);

	float* colour[3] = {&m_colour.X, &m_colour.Y, &m_colour.Z};
	ImGui::ColorPicker3("Line Colour", colour[0]);
}

//- Scene Save / Load -//
void LineRenderer::SceneLoad(json* componentJSON)
{	
	m_startPos.X = ((float)(*componentJSON)["StartX"]);
	m_startPos.Y = ((float)(*componentJSON)["StartY"]);

	m_endPos.X = ((float)(*componentJSON)["EndX"]);
	m_endPos.Y = ((float)(*componentJSON)["EndY"]);

	mf_width = ((float)(*componentJSON)["Width"]);

	m_colour.X = ((float)(*componentJSON)["ColourX"]);
	m_colour.Y = ((float)(*componentJSON)["ColourY"]);
	m_colour.Z = ((float)(*componentJSON)["ColourZ"]);

}

json* LineRenderer::SceneSave()
{
	json* returnObj = new json(
	{
		{"StartX", m_startPos.X},
		{"StartY", m_startPos.Y},
		{"EndX", m_endPos.X},
		{"EndY", m_endPos.Y},
		{"Width", mf_width},
		{"ColourX", m_colour.X},
		{"ColourY", m_colour.Y},
		{"ColourZ", m_colour.Y}
	});

	return returnObj;
}

//- Custom Functions -//