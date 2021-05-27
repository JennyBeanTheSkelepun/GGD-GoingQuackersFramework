#include "DirectXTexture2D.h"
#include "../../Game Systems/Debug.h"

Texture2D::Texture2D()
{
	mpc_targaData = 0;
	mp_texture = 0;
	mp_textureView = 0;
}

Texture2D::Texture2D(const Texture2D&)
{
}

Texture2D::~Texture2D()
{
}

bool Texture2D::Initialize(ID3D11Device* ap_device, ID3D11DeviceContext* ap_deviceContext, const char* acp_filename)
{
	bool lb_result;
	int li_height, li_width;
	D3D11_TEXTURE2D_DESC l_textureDesc;
	HRESULT l_hResult;
	unsigned int li_rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC l_srvDesc;
	
	// Load the targa image data into memory.
	lb_result = LoadTarga(acp_filename, li_height, li_width);
	if (!lb_result)
	{
		return false;
	}
	
	// Setup the description of the mp_texture.
	l_textureDesc.Height = li_height;
	l_textureDesc.Width = li_width;
	l_textureDesc.MipLevels = 0;
	l_textureDesc.ArraySize = 1;
	l_textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	l_textureDesc.SampleDesc.Count = 1;
	l_textureDesc.SampleDesc.Quality = 0;
	l_textureDesc.Usage = D3D11_USAGE_DEFAULT;
	l_textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	l_textureDesc.CPUAccessFlags = 0;
	l_textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty mp_texture.
	l_hResult = ap_device->CreateTexture2D(&l_textureDesc, NULL, &mp_texture);
	if (FAILED(l_hResult))
	{
		return false;
	}

	// Set the row pitch of the targa image data.
	li_rowPitch = (li_width * 4) * sizeof(unsigned char);
	
	// Copy the targa image data into the mp_texture.
	ap_deviceContext->UpdateSubresource(mp_texture, 0, NULL, mpc_targaData, li_rowPitch, 0);
	
	// Setup the shader resource view description.
	l_srvDesc.Format = l_textureDesc.Format;
	l_srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	l_srvDesc.Texture2D.MostDetailedMip = 0;
	l_srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the mp_texture.
	l_hResult = ap_device->CreateShaderResourceView(mp_texture, &l_srvDesc, &mp_textureView);
	if (FAILED(l_hResult))
	{
		return false;
	}

	// Generate mipmaps for this mp_texture.
	ap_deviceContext->GenerateMips(mp_textureView);

	// Release the targa image data now that the image data has been loaded into the mp_texture.
	delete[] mpc_targaData;
	mpc_targaData = 0;

	return true;
}

void Texture2D::Shutdown()
{
	// Release the mp_texture view resource.
	if (mp_textureView)
	{
		mp_textureView->Release();
		mp_textureView = 0;
	}

	// Release the mp_texture.
	if (mp_texture)
	{
		mp_texture->Release();
		mp_texture = 0;
	}

	// Release the targa data.
	if (mpc_targaData)
	{
		delete[] mpc_targaData;
		mpc_targaData = 0;
	}
}

ID3D11ShaderResourceView* Texture2D::GetTexture()
{
	return mp_textureView;
}

bool Texture2D::LoadTarga(const char* acp_filename, int& ari_height, int& ari_width)
{
	int li_error, li_bpp, li_imageSize, li_index, li_i, li_j, li_k;
	FILE* lp_filePtr;
	unsigned int li_count;
	TargaHeader l_targaFileHeader;
	unsigned char* lpc_targaImage;


	// Open the targa file for reading in binary.
	li_error = fopen_s(&lp_filePtr, acp_filename, "rb");
	if (li_error != 0)
	{
		Debug::getInstance()->LogError("ERROR Cant Find Texture File");
		return false;
	}

	// Read in the file header.
	li_count = (unsigned int)fread(&l_targaFileHeader, sizeof(TargaHeader), 1, lp_filePtr);
	if (li_count != 1)
	{
		Debug::getInstance()->LogError("ERROR Reading in Texture Header File");
		return false;
	}

	// Get the important information from the header.
	ari_height = (int)l_targaFileHeader.height;
	ari_width = (int)l_targaFileHeader.width;
	li_bpp = (int)l_targaFileHeader.bpp;

	// Check that it is 32 bit and not 24 bit.
	if (li_bpp != 32)
	{
		Debug::getInstance()->LogError("The texture you want to load is a 24Bit Tga it needs to be a 32Bit Tga");
		return false;
	}

	// Calculate the size of the 32 bit image data.
	li_imageSize = ari_width * ari_height * 4;

	// Allocate memory for the targa image data.
	lpc_targaImage = new unsigned char[li_imageSize];
	if (!lpc_targaImage)
	{
		Debug::getInstance()->LogError("ERROR Allocating temp memory space for texture");
		return false;
	}

	// Read in the targa image data.
	li_count = (unsigned int)fread(lpc_targaImage, 1, li_imageSize, lp_filePtr);
	if (li_count != li_imageSize)
	{
		Debug::getInstance()->LogError("ERROR Reading in Texture File");
		return false;
	}

	// Close the file.
	li_error = fclose(lp_filePtr);
	if (li_error != 0)
	{
		Debug::getInstance()->LogError("ERROR Cant Close Texture File");
		return false;
	}

	// Allocate memory for the targa destination data.
	mpc_targaData = new unsigned char[li_imageSize];
	if (!mpc_targaData)
	{
		Debug::getInstance()->LogError("ERROR Allocating Heap memory space for texture");
		return false;
	}

	// Initialize the li_index into the targa destination data array.
	li_index = 0;

	// Initialize the li_index into the targa image data.
	li_k = (ari_width * ari_height * 4) - (ari_width * 4);

	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
	for (li_j = 0; li_j < ari_height; li_j++)
	{
		for (li_i = 0; li_i < ari_width; li_i++)
		{
			mpc_targaData[li_index + 0] = lpc_targaImage[li_k + 2];  // Red.
			mpc_targaData[li_index + 1] = lpc_targaImage[li_k + 1];  // Green.
			mpc_targaData[li_index + 2] = lpc_targaImage[li_k + 0];  // Blue
			mpc_targaData[li_index + 3] = lpc_targaImage[li_k + 3];  // Alpha

			// Increment the indexes into the targa data.
			li_k += 4;
			li_index += 4;
		}

		// Set the targa image data li_index back to the preceding row at the beginning of the column since its reading it in upside down.
		li_k -= (ari_width * 8);
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] lpc_targaImage;
	lpc_targaImage = 0;

	return true;
}
