#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_

#include <d3d11.h>
#include <stdio.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(char*, int&, int&);

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

#endif // !_TEXTURE_CLASS_H_