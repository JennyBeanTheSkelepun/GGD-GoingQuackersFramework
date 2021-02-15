#include "Sprite.h"

Sprite::Sprite(GameObject* owner) : Component(owner)
{
}

Sprite::~Sprite()
{
	ReleaseTexture();
}

void Sprite::Initialize()
{
	mp_texture = nullptr;
	m_width = 0;
	m_height = 0;
}

bool Sprite::LoadSprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName)
{
	bool result;

	// Create the texture object.
	mp_texture = new Texture2D;
	if (!mp_texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = mp_texture->Initialize(device, deviceContext, fileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void Sprite::ReleaseTexture()
{
	// Release the texture object.
	if (mp_texture)
	{
		mp_texture->Shutdown();
		delete mp_texture;
		mp_texture = 0;
	}

	return;
}
