#include "LineRenderer.h"
#include "../../../Code/EngineBase/Rendering/Graphics.h"

LineRenderer::LineRenderer(GameObject* owner) : Component(owner, ComponentTypes::LINERENDERER, "Line Renderer")
{
	m_startPos = Vector2();
	m_endPos = Vector2();
	mf_width = 10;

	Graphics::getInstance()->AddLineRenderer(this);

	TextureSelectionInput = new char[100]{ "Assets/stone.tga" };
	ShaderSelectionInput = new char[100]{ "Assets/Shaders/TextureSimple.fx" };

	LocalTransform = new Transform(nullptr);
	CalculateRequiredPositionRoatationScale();
}

LineRenderer::~LineRenderer()
{
	delete LocalTransform;
	LocalTransform = nullptr;

	Graphics::getInstance()->RemoveLineRenderer(this);
}

void LineRenderer::OnDestroy()
{
	this->~LineRenderer();
}

void LineRenderer::Update()
{
	LocalTransform->Update();
	CalculateRequiredPositionRoatationScale();
}

void LineRenderer::ImGUIUpdate()
{
	ImGui::InputText("Enter Texture Location", TextureSelectionInput, 128);
	if (ImGui::Button("Update Texture"))
	{
		std::string s(ShaderSelectionInput);
		std::wstring ws;
		ws.assign(s.begin(), s.end());

		if (mi_Texture != -1)
		{
			mi_Texture = Graphics::getInstance()->RemoveTexture(mi_Texture);
		}
		if (mi_Shader != -1)
		{
			mi_Shader = Graphics::getInstance()->RemoveShader(mi_Shader);
		}

		m_TextureLocation = m_TextureLocation;
		m_ShaderLocation.assign(ws.begin(), ws.end());
		Graphics* temp = Graphics::getInstance();

		//- If Texture/mp_shader/Object is instantiated then dont try and remake it  -//
		if (mi_Texture == -1)
		{
			mi_Texture = Graphics::getInstance()->LoadTexture(m_TextureLocation);
		}

		if (mi_Shader == -1)
		{
			mi_Shader = Graphics::getInstance()->LoadShader(ws);
		}

		m_TextureLocation = std::string(TextureSelectionInput);
		m_ShaderLocation = std::string(ShaderSelectionInput);
	}

	ImGui::Text("Line Points");
	float* PosOne[2] = { &m_startPos.X, &m_startPos.Y };
	ImGui::InputFloat2("Start Pos", PosOne[0]);

	float* PosTwo[2] = { &m_endPos.X, &m_endPos.Y };
	ImGui::InputFloat2("End Pos", PosTwo[0]);

	ImGui::InputFloat("Width", &mf_width);
}

//- Scene Save / Load -//
void LineRenderer::SceneLoad(json* componentJSON)
{
	m_startPos.X = (*componentJSON)["StartX"];
	m_startPos.Y = (*componentJSON)["StartY"];
	m_endPos.X = (*componentJSON)["EndX"];
	m_endPos.Y = (*componentJSON)["EndY"];
	mf_width = (*componentJSON)["Width"];
	m_TextureLocation = (*componentJSON)["TextureLocation"];
	
	CalculateRequiredPositionRoatationScale();
}

Transform* LineRenderer::GetLocalTransform()
{
	return LocalTransform;
}

void LineRenderer::CalculateRequiredPositionRoatationScale()
{
	//- Var Decliration -//
	Vector2 Pos, Scale, start = m_startPos, end = m_endPos;
	float roatation = 0, angleOffset = 90;

	

	//- Position -//
	Pos = start;

	//- Rotation -//
	Vector2 LocalisedEndPos = start - end;
	Vector2 Up = Vector2(start.X, start.Y + 1);
	float LocalEndPosLength = LocalisedEndPos.Length();
	float UpLength = Up.Length();

	if (LocalEndPosLength != 0 && UpLength != 0)
	{
		LocalisedEndPos.Normalize();
		Up.Normalize();
		float dot = Up.Dot(LocalisedEndPos);
		float determinant = Up.X * LocalisedEndPos.Y - Up.Y * LocalisedEndPos.X;
		float angle = std::atan2f(dot, determinant);
		angle *= 180 / 3.1415;

		roatation = angle - 76;
	}
	else
	{
		roatation = 0;
	}

	//- Scale -//
	Scale.X = m_startPos.Distance(m_endPos);
	Scale.Y = mf_width;

	LocalTransform->SetLocalPosition(Pos);
	LocalTransform->SetLocalRotation(roatation);
	LocalTransform->SetLocalScale(Scale);
}

json* LineRenderer::SceneSave()
{
	json* returnObj = new json({
		{"StartX", m_startPos.X},
		{"StartY", m_startPos.Y},
		{"EndX", m_endPos.X},
		{"EndY", m_endPos.Y},
		{"Width", mf_width},
		{"TextureLocation", m_TextureLocation}
	});
	return returnObj;
}