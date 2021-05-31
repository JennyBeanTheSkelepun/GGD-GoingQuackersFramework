#ifndef _LINE_RENDERER_H_
#define _LINE_RENDERER_H_

#include "../../Data Structures/Vectors.h"
#include "Component.h"
#include "Transform.h"

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

	inline int GetTexture() { return mi_Texture; };
	inline int GetShader() { return mi_Shader; };

	void SetStartPos(Vector2 a_startPos) { m_startPos = a_startPos; };
	void SetEndPos(Vector2 a_endPos) { m_endPos = a_endPos; };
	void SetWidth(float af_width) { mf_width = af_width; };

	Transform* GetLocalTransform();

private:
	Vector2 m_startPos;
	Vector2 m_endPos;
	float mf_width;

	char* TextureSelectionInput;
	char* ShaderSelectionInput;
	int mi_Texture, mi_Shader;
	std::string m_TextureLocation, m_ShaderLocation;

	Transform* LocalTransform;
	void CalculateRequiredPositionRoatationScale();

};

#endif //- _LINE_RENDERER_H_ -//