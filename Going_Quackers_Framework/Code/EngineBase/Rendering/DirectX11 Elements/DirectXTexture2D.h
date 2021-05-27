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

	bool Initialize(ID3D11Device* ap_device, ID3D11DeviceContext* ap_deviceContext, const char* apc_filename);
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

	bool LoadTarga(const char* apc_filename, int& ari_height, int& ari_width);

	unsigned char* mpc_targaData;
	ID3D11Texture2D* mp_texture;
	ID3D11ShaderResourceView* mp_textureView;
};

#endif // !_TEXTURE_CLASS_H_