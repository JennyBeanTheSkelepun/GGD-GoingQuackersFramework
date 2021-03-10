#include "DirectXRenderLoop.h"

DirectXRenderLoop::DirectXRenderLoop(DirectXClass& ar_DirectXClass) 
{
	mr_directXClass = ar_DirectXClass;
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
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	ar_DirectX.mp_deviceContext->OMSetRenderTargets(1, &ar_DirectX.mp_renderTextureRenderTargetView, ar_DirectX.mp_depthStencilView);

	// Clear the buffers to begin the scene.
	ar_DirectX.BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	float color[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
	ar_DirectX.GetDeviceContext()->ClearRenderTargetView(ar_DirectX.mp_renderTextureRenderTargetView, color);

	// Generate the view matrix based on the camera's position.
	ar_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	ar_DirectX.GetWorldMatrix(worldMatrix);
	ar_Camera.GetViewMatrix(viewMatrix);
	ar_DirectX.GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t i = 0; i < GameObjectsToRender.size(); i++)
	{
		// Vertex and index
		//GameObjectsToRender[i]->RenderBuffers(ar_DirectX.GetDeviceContext());

		//Sprite* temp = GameObjectsToRender[i]->mp_spriteReference;

		//GameObject* temp1 = GameObjectsToRender[i]->GetOwner();

		ar_Shader.GetShader(temp->GetTexture())->Render(ar_DirectX.GetDeviceContext(), 6, temp1->GetTransform()->GetLocalToWorldMatrix(), viewMatrix, projectionMatrix, ar_Shader.GetShader(temp1->GetComponent<Sprite>()->GetTexture())->);

		// Render the model using the color shader.
		//result = ar_Shader.Render(ar_DirectX.GetDeviceContext(), 6, gameObjects[i]->GetTransform()->GetLocalToWorldMatrix(), viewMatrix, projectionMatrix, gameObjects[i]->GetComponent<Sprite>()->GetTexture());
		//if (!result)
		//{
		//	return false;
		//}
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
	ar_DirectX.GetWorldMatrix(worldMatrix);
	ar_Camera.GetViewMatrix(viewMatrix);
	ar_DirectX.GetProjectionMatrix(projectionMatrix);

	//- ImGui Call to render -//
	ar_ImGui.Render();
	ar_DirectX.EndScene();
}

//- Game View -//
bool DirectXRenderLoop::ActiveGameRender(DirectXClass& ar_DirectX, DirectXCamera& ar_Camera, DirectXShaderManager& ar_Shader, 
										 DirectXTextureManager& ar_texture, DirectXImGui& ar_ImGui)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	ar_DirectX.mp_deviceContext->OMSetRenderTargets(1, &ar_DirectX.mp_renderTargetView, ar_DirectX.mp_depthStencilView);

	// Clear the buffers to begin the scene.
	ar_DirectX.BeginGameScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	ar_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	ar_DirectX.GetWorldMatrix(worldMatrix);
	ar_Camera.GetViewMatrix(viewMatrix);
	ar_DirectX.GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (size_t i = 0; i < GameObjectsToRender.size(); i++)
	{
		//gameObjects[i]->Render();

		//// Render the model using the color shader.
		//result = ar_Shader.Render(ar_DirectX.GetDeviceContext(), 6, worldMatrix, viewMatrix, projectionMatrix, gameObjects[i]->GetComponent<Sprite>()->GetTexture());
		//if (!result)
		//{
		//	return false;
		//}
	}

	ar_ImGui.Render();
	ar_DirectX.EndScene();
}

void DirectXRenderLoop::SetObjectToRender(SpriteRenderer* ObjToRender)
{
	GameObjectsToRender.push_back(ObjToRender);
}
