#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Component.h"
#include "../../Data Structures/Texture2D.h"

class Sprite : public Component
{
public:
	Sprite(GameObject* owner);
	~Sprite();

	void Initialize() override;

	bool LoadSprite(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* fileName);

	ID3D11ShaderResourceView* GetTexture() { return mp_texture->GetTexture(); }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

private:
	//The sprites texture
	Texture2D* mp_texture;

	//The width of the sprite
	int m_width;

	//The height of the sprite
	int m_height;

	//Clear up function
	void ReleaseTexture();
};

#endif