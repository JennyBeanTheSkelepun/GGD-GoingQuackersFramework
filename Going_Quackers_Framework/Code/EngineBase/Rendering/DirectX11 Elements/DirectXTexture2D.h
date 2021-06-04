#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_

#include <d3d11.h>
#include <stdio.h>

class Texture2D
{
public:
	Texture2D();
	Texture2D(const Texture2D&);
	~Texture2D();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	bool LoadDDSTexture(ID3D11Device* device, const wchar_t* filename);

private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	bool LoadTarga(const char* filename, int& height, int& width);

	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

#endif // !_TEXTURE_CLASS_H_