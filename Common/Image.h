#pragma once

#ifdef _WIN32
//#pragma comment(lib,"libjpeg.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"zlib.lib")
#endif

#include "../Core/Engine.h"
#include "../Render/Render.h"

extern "C"{
//#include "../include/Image/jpeglib.h"
}
#include "../include/Image/png.h"

class CImage
{
public:
	CImage();
	CImage(CImage &img);
	CImage(uint32 Depth,uint32 Format,uint32 Width,uint32 Height,uChar *data);
	~CImage();


	//Load
	int32 LoadImageFromFile(Str filename);
	

	//Save
	int32 SaveImageInFile(Str filename);

	//Convert
	void ConvertToRGB();
	void ConvertToRGBA();


	//Get Data
	uint32 GetChannelCount(uint32 format = -1);
	uint32 GetBytesPerPixel(uint32 format = -1);
	uint32 GetBytesPerChannel(uint32 format = -1);
	uint32 GetPixelCount();

	__forceinline uint32 GetDepth(){return Depth;}
	__forceinline uint32 GetFormat(){return Format;}
	__forceinline uint32 GetWidth(){return Width;}
	__forceinline uint32 GetHeight(){return Height;}
	__forceinline uChar *GetData(){return data;}
	__forceinline Bool Is1D(){return (Depth == 1 && Height == 1);}
	__forceinline Bool Is2D(){return (Depth == 1 && Height >  1);}

	//Other
	void CreateEmptyTexture(uint32 width,uint32 height,uint32 format = CRender::FORMAT_RGBA8,uint32 depth = 1,uint32 fill_r = 0,uint32 fill_g = 0,uint32 fill_b = 0,uint32 fill_a = 0);
	void FlipV();
	void FlipH();
	void SwapChannels(uint32 ch1,uint32 ch2);
	void QuarterImage();
	Bool RemoveChannels(Bool ch0,Bool ch1,Bool ch2,Bool ch3);
	Bool GrayScale();
	Bool NormalMap(Float32 sZ);
private:
	int32 LoadImageDataFromFileJPG(Str filename);
	int32 LoadImageDataFromFileTGA(Str filename);
	int32 LoadImageDataFromFilePNG(Str filename);
	int32 LoadImageDataFromFileDDS(Str filename);
	
	int32 SaveImageDataInFileJPG(Str filename);
	int32 SaveImageDataInFileTGA(Str filename);
	int32 SaveImageDataInFilePNG(Str filename);
	int32 SaveImageDataInFileDDS(Str filename);


	struct dds_colorkey 
	{
		unsigned int dwColorSpaceLowValue;
		unsigned int dwColorSpaceHighValue;
	};

	struct dds_header 
	{
		unsigned int magic;
		unsigned int dwSize;
		unsigned int dwFlags;
		unsigned int dwHeight;
		unsigned int dwWidth;
		long lPitch;
		unsigned int dwDepth;
		unsigned int dwMipMapCount;
		unsigned int dwAlphaBitDepth;
		unsigned int dwReserved;
		void *lpSurface;
		dds_colorkey ddckCKDestOverlay;
		dds_colorkey ddckCKDestBlt;
		dds_colorkey ddckCKSrcOverlay;
		dds_colorkey ddckCKSrcBlt;
		unsigned int dwPFSize;
		unsigned int dwPFFlags;
		unsigned int dwFourCC;
		unsigned int dwRGBBitCount;
		unsigned int dwRBitMask;
		unsigned int dwGBitMask;
		unsigned int dwBBitMask;
		unsigned int dwRGBAlphaBitMask;
		unsigned int dwCaps;
		unsigned int dwCaps2;
		unsigned int dwCaps3;
		unsigned int dwVolumeDepth;
		unsigned int dwTextureStage;
	};

	struct dds_color 
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	enum 
	{
		DDS_ERROR = -1,
		DDS_RGB,
		DDS_RGBA,
		DDS_DXT1,
		DDS_DXT2,
		DDS_DXT3,
		DDS_DXT4,
		DDS_DXT5
	};

	enum 
	{
		DDPF_ALPHAPIXELS = 0x01,
		DDPF_FOURCC = 0x04,
		DDPF_RGB = 0x40,
		DDPF_RGBA = 0x41
	};
protected:
	uint32 Depth;
	uint32 Format;
	uint32 Width;
	uint32 Height;
	uChar *data;
};
