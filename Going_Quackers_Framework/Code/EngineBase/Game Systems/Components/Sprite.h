#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Component.h"
#include "../../Rendering/Graphics.h"

class Sprite : public Component
{
public:
	Sprite(GameObject* owner);
	~Sprite();

	void Initialize() override;

	bool LoadSprite(const char* fileName);

	int GetTexture() { return mi_texture; }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

private:

	//Texture Index
	int mi_texture;

	//The width of the sprite
	int m_width;

	//The height of the sprite
	int m_height;

	//Clear up function
	void ReleaseTexture();
};

#endif