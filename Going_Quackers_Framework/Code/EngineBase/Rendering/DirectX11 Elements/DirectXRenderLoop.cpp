#include "DirectXRenderLoop.h"

DirectXRenderLoop::DirectXRenderLoop(DirectXClass* ap_DirectX) 
{
	mp_2DModel = new DirectXTwoDObject();
	mp_2DModel->Initialize(ap_DirectX->GetDevice(), ap_DirectX->GetDeviceContext(), RenderObjectType::SQUARE);

	mp_Line = new DirectXTwoDObject();
	mp_Line->Initialize(ap_DirectX->GetDevice(), ap_DirectX->GetDeviceContext(), RenderObjectType::LINE);
}

DirectXRenderLoop::~DirectXRenderLoop()
{
}

bool DirectXRenderLoop::Render(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, 
							   DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui)
{
	if (EngineGuiClass::getInstance()->IsInPlayMode())
		ActiveGameRender(ar_DirectX, ar_Camera, ar_Shader, ar_texture, ar_ImGui);
	else
		EditorRender(ar_DirectX, ar_Camera, ar_Shader, ar_texture, ar_ImGui);

	return false;
}

//---------------------------------------------------------------------------//
//------------ Dont touch below unless your sure what your doing ------------//
//---------------------------------------------------------------------------//

//- Editor View with Render Texture -//
bool DirectXRenderLoop::EditorRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, 
									 DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui)
{
	//- Decliration of matix -//
	DirectX::XMMATRIX l_worldMatrix, l_viewMatrix, l_projectionMatrix;
	bool l_result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	ar_DirectX.mp_deviceContext->OMSetRenderTargets(1, &ar_DirectX.mp_renderTextureRenderTargetView, ar_DirectX.mp_depthStencilView);

	// Clear the buffers to begin the scene.
	ar_DirectX.BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	ar_DirectX.GetDeviceContext()->ClearRenderTargetView(ar_DirectX.mp_renderTextureRenderTargetView, color);

	// Generate the view matrix based on the camera's position.
	ar_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	ar_DirectX.GetWorldMatrix(l_worldMatrix);
	ar_Camera.GetViewMatrix(l_viewMatrix);
	ar_DirectX.GetProjectionMatrix(l_projectionMatrix);

	// Put the model vertex and ai_index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t li_i = 0; li_i < m_gameObjectsToRender.size(); li_i++)
	{
		//- universal Plane for 2d elements -//
		mp_2DModel->Render();
		DirectXShader* lp_tempShader = ar_Shader.GetShader(m_gameObjectsToRender[li_i]->GetShader());
		Texture2D* lp_tempTexture = ar_texture.GetTexture(m_gameObjectsToRender[li_i]->GetTexture());

		GameObject* lp_tempGameObject = m_gameObjectsToRender[li_i]->GetOwner();

		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(lp_tempGameObject->GetTransform()->GetLocalScale().X, lp_tempGameObject->GetTransform()->GetLocalScale().Y, 0.0f);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(lp_tempGameObject->GetTransform()->GetLocalPosition().X, lp_tempGameObject->GetTransform()->GetLocalPosition().Y, 0.0f);
		DirectX::XMMATRIX roation = DirectX::XMMatrixTranslation(lp_tempGameObject->GetTransform()->GetGlobalPosition().X, lp_tempGameObject->GetTransform()->GetGlobalPosition().Y, 0.0f);

		DirectX::XMFLOAT4X4 _world;
		XMStoreFloat4x4(&_world, scale * roation * translation * DirectX::XMMatrixRotationZ((lp_tempGameObject->GetTransform()->GetLocalRotation() * 3.1415)/180));

		DirectX::XMMATRIX worldMatrix;
		worldMatrix = DirectX::XMLoadFloat4x4(&_world);

		l_result = lp_tempShader->Render(ar_DirectX.GetDeviceContext(), 6, worldMatrix, l_viewMatrix, l_projectionMatrix, lp_tempTexture->GetTexture());
		if (!l_result)
		{
			return false;
		}
	}

	// Put the model vertex and ai_index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t li_i = 0; li_i < m_linesToRender.size(); li_i++)
	{
		//- universal line (width 1) for all line elements -//
		mp_Line->Render();

		DirectXShader* lp_tempShader = ar_Shader.GetShader(0);
		Texture2D* lp_tempTexture = ar_texture.GetTexture(m_linesToRender[li_i]->GetTexture());

		GameObject* lp_tempGameObject = m_linesToRender[li_i]->GetOwner();

		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(lp_tempGameObject->GetTransform()->GetLocalScale().X, lp_tempGameObject->GetTransform()->GetLocalScale().Y, 0.0f);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(lp_tempGameObject->GetTransform()->GetLocalPosition().X, lp_tempGameObject->GetTransform()->GetLocalPosition().Y, 0.0f);
		//DirectX::XMMATRIX roation = DirectX::XMMatrixTranslation(lp_tempGameObject->GetTransform()->GetLocalPosition().X, lp_tempGameObject->GetTransform()->GetLocalPosition().Y, 0.0f);
		//DirectX::XMMATRIX roation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX roation = DirectX::XMMatrixTranslation(0.0f, 0.0f, ((lp_tempGameObject->GetTransform()->GetLocalRotation() * 3.1415) / 180));


		DirectX::XMFLOAT4X4 _world;
		XMStoreFloat4x4(&_world, scale * roation * translation /** DirectX::XMMatrixRotationX((lp_tempGameObject->GetTransform()->GetLocalRotation() * 3.1415)/180)*/);
		
		DirectX::XMMATRIX worldMatrix;
		worldMatrix = DirectX::XMLoadFloat4x4(&_world);

		l_result = lp_tempShader->Render(ar_DirectX.GetDeviceContext(), 6, worldMatrix, l_viewMatrix, l_projectionMatrix, lp_tempTexture->GetTexture());
		if (!l_result)
		{
			return false;
		}
	}

	//-----------------//
	//- Engine Render -//
	//-----------------//
	ar_DirectX.mp_deviceContext->OMSetRenderTargets(1, &ar_DirectX.mp_renderTargetView, ar_DirectX.mp_depthStencilView);

	// Clear the buffers to begin the scene.
	ar_DirectX.BeginEditorScene(0.0f, 0.0f, 0.0f, 0.0f); //<-- Background colour for editor

	// Generate the view matrix based on the camera's position.
	ar_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	ar_DirectX.GetWorldMatrix(l_worldMatrix);
	ar_Camera.GetViewMatrix(l_viewMatrix);
	ar_DirectX.GetProjectionMatrix(l_projectionMatrix);

	//- ImGui Call to render -//
	ar_ImGui.Render();
	ar_DirectX.EndScene();
}

//- Game View -//
bool DirectXRenderLoop::ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, 
										 DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui)
{
	DirectX::XMMATRIX l_worldMatrix, l_viewMatrix, l_projectionMatrix;
	bool lb_result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	ar_DirectX.mp_deviceContext->OMSetRenderTargets(1, &ar_DirectX.mp_renderTargetView, ar_DirectX.mp_depthStencilView);

	// Clear the buffers to begin the scene.
	ar_DirectX.BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	ar_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	ar_DirectX.GetWorldMatrix(l_worldMatrix);
	ar_Camera.GetViewMatrix(l_viewMatrix);
	ar_DirectX.GetProjectionMatrix(l_projectionMatrix);

	// Put the model vertex and ai_index buffers on the graphics pipeline to prepare them for drawing.
	//for (size_t li_i = 0; li_i < m_gameObjectsToRender.size(); li_i++)
	//{
	//	//- universal Plane for 2d elements -//
	//	mp_2DModel->Render();
	//	DirectXShader* lp_tempShader = ar_Shader.GetShader(m_gameObjectsToRender[li_i]->GetShader());
	//	Texture2D* lp_tempTexture = ar_texture.GetTexture(m_gameObjectsToRender[li_i]->GetShader());

	//	GameObject* lp_tempGameObject = m_gameObjectsToRender[li_i]->GetOwner();

	//	lb_result = lp_tempShader->Render(ar_DirectX.GetDeviceContext(), 6, lp_tempGameObject->GetTransform()->GetLocalToWorldMatrix(), l_viewMatrix, l_projectionMatrix, lp_tempTexture->GetTexture());
	//	if (!lb_result)
	//	{
	//		return false;
	//	}
	//}

	ar_ImGui.Render();
	ar_DirectX.EndScene();

	return true;
}

int DirectXRenderLoop::SetObjectToRender(SpriteRenderer* ap_ObjToRender)
{
	m_gameObjectsToRender.push_back(ap_ObjToRender);
	return m_gameObjectsToRender.size() - 1;
}

int DirectXRenderLoop::RemoveObjectToRenderLoop(int ai_index)
{
	m_gameObjectsToRender.erase(m_gameObjectsToRender.begin() + ai_index);
	return -1;
}

void DirectXRenderLoop::AddLineRenderer(LineRenderer* lineRenderer)
{
	m_linesToRender.push_back(lineRenderer);
}

LineRenderer* DirectXRenderLoop::RemoveLineRenderer(LineRenderer* lineRenderer)
{
	std::vector<LineRenderer*>::iterator it = std::find(m_linesToRender.begin(), m_linesToRender.end(), lineRenderer);
	if (it != m_linesToRender.end())
	{
		m_linesToRender.erase(it);
	}
	return nullptr;
}
