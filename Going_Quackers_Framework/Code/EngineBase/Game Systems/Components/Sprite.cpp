#include "Sprite.h"

Sprite::Sprite(GameObject* owner) : Component(owner, ComponentTypes::SPRITE)
{
}

Sprite::~Sprite()
{
	ReleaseTexture();
}

void Sprite::Initialize()
{
	//- Set Texture As Base Texture (stone.tga) -//
	mi_texture = 0;
	m_width = 0;
	m_height = 0;
}

bool Sprite::LoadSprite(const char* fileName)
{
	bool result;

	// Create the texture object.
	mi_texture = Graphics::getInstance()->LoadTexture<DirectXGraphics*>(fileName);

	return true;
}

void Sprite::ReleaseTexture()
{
	// Release the texture object.

	Graphics::getInstance()->RemoveTexture<DirectXGraphics*>(inedx);

	return;
}
