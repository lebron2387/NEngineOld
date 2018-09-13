#include "Image.h"

CImage::CImage()
{
	Depth = 1;
	Format = 0;
	Width = 0;
	Height = 0;
}


CImage::CImage(CImage &img)
{
	Depth = img.Depth;
	Format = img.Format;
	Width = img.Width;
	Height = img.Height;
	data = img.data;
}

CImage::CImage(uint32 depth,uint32 format,uint32 width,uint32 height,uChar *newdata)
{
	Depth = depth;
	Format = format;
	Width = width;
	Height = height;
	data = newdata;
}

CImage::~CImage()
{
	Depth = 0;
	Format = 0;
	Width = 0;
	Height = 0;
	SAFE_DELETE_ARRAY(data);
}

uint32 CImage::GetChannelCount(uint32 format)
{
	if(format == -1)format = Format;
	uint32 data[] = 
	{
		0,
		1, 2, 3, 4,       //  8-bit unsigned
		1, 2, 3, 4,       // 16-bit unsigned
		1, 2, 3, 4,       //  8-bit signed
		1, 2, 3, 4,       // 16-bit signed
		1, 2, 3, 4,       // 16-bit float
		1, 2, 3, 4,       // 32-bit float
		1, 2, 3, 4,       // 16-bit unsigned integer
		1, 2, 3, 4,       // 32-bit unsigned integer
		1, 2, 3, 4,       // 16-bit signed integer
		1, 2, 3, 4,       // 32-bit signed integer
		3, 3, 3, 3, 4, 4, // Packed
		1, 1, 2, 1,       // Depth
		3, 4, 4, 1, 2,    // Compressed
	};
	return data[format];
}

uint32 CImage::GetBytesPerPixel(uint32 format)
{
	if(format == -1)format = Format;
	uint32 data[] = 
	{
		0,
		1, 2, 3, 4,       //  8-bit unsigned
		2, 4, 6, 8,       // 16-bit unsigned
		1, 2, 3, 4,       //  8-bit signed
		2, 4, 6, 8,       // 16-bit signed
		2, 4, 6, 8,       // 16-bit float
		4, 8, 12, 16,     // 32-bit float
		2, 4, 6, 8,       // 16-bit unsigned integer
		4, 8, 12, 16,     // 32-bit unsigned integer
		2, 4, 6, 8,       // 16-bit signed integer
		4, 8, 12, 16,     // 32-bit signed integer
		4, 4, 4, 2, 2, 4, // Packed
		2, 4, 4, 4,       // Depth
	};
	return data[format];
}

uint32 CImage::GetBytesPerChannel(uint32 format)
{
	if(format == -1)format = Format;
	uint32 bytes[] = {
		1, //  8-bit unsigned
		2, // 16-bit unsigned
		1, //  8-bit signed
		2, // 16-bit signed
		2, // 16-bit float
		4, // 32-bit float
		2, // 16-bit unsigned integer
		4, // 32-bit unsigned integer
		2, // 16-bit signed integer
		4, // 32-bit signed integer
	};

	return bytes[(format + 1) >> 2];
}

void CImage::CreateEmptyTexture(uint32 width,uint32 height,uint32 format,uint32 depth,uint32 fill_r,uint32 fill_g,uint32 fill_b,uint32 fill_a)
{
	Width = width;
	Height = height;
	Format = format;
	Depth = depth;
	data = new uChar[Width*Height*GetChannelCount(Format)];
	for(uint32 i=0;i<Width*Height*GetChannelCount(Format);i+=GetChannelCount(Format))
	{
		if(GetChannelCount(Format) > 0)data[i] = fill_r;
		if(GetChannelCount(Format) > 1)data[i+1] = fill_g;
		if(GetChannelCount(Format) > 2)data[i+2] = fill_b;
		if(GetChannelCount(Format) > 3)data[i+3] = fill_a;
	}
}

void CImage::FlipV()
{
	uint32 i,j=0;
	uChar *newdata = new uChar[Width*Height*GetChannelCount(Format)];
	for(i=0,j=(Height*Width*GetChannelCount(Format))-(Width*GetChannelCount(Format));i<Height*Width*GetChannelCount(Format);i+=Width*GetChannelCount(Format),j-=Width*GetChannelCount(Format))
	{
		for(uint32 k=0;k<Width*GetChannelCount(Format);k+=GetChannelCount(Format))
		{
			if(GetChannelCount(Format)>0)newdata[i+k] = data[j+k];
			if(GetChannelCount(Format)>1)newdata[i+k+1] = data[j+k+1];
			if(GetChannelCount(Format)>2)newdata[i+k+2] = data[j+k+2];
			if(GetChannelCount(Format)>3)newdata[i+k+3] = data[j+k+3];
		}
	}
	SAFE_DELETE_ARRAY(data);
	data = newdata;
}

void CImage::FlipH()
{}

void CImage::SwapChannels(uint32 ch1,uint32 ch2)
{
	for(uint32 i = 0, j = 0; i < Width * Height; i++, j += GetChannelCount(Format))
	{
		uChar tmp = data[j+ch2];
		data[j+ch2] = data[j+ch1];
		data[j+ch1] = tmp;
	}
}

void CImage::ConvertToRGB()
{
	if(GetChannelCount(Format) == 3)return;
	uChar *newdata = new uChar[Width*Height*3];
	uint32 size = Width * Height;
	for(uint32 i = 0, j = 0; i < size; i++, j += 3)
	{
		newdata[j] = data[j];
		newdata[j+1] = data[j+1];
		newdata[j+2] = data[j+2];
	}
	Format = CRender::FORMAT_RGB8;
	SAFE_DELETE_ARRAY(data);
	data = newdata;
}

void CImage::ConvertToRGBA()
{
	if(GetChannelCount(Format) == 4)return;
	uChar *newdata = new uChar[Width*Height*4];
	uint32 size = Width * Height;
	for(uint32 i = 0, j = 0, k = 0; i < size; i++, k += 3, j += 4)
	{
		newdata[j] = data[k];
		newdata[j+1] = data[k+1];
		newdata[j+2] = data[k+2];
		newdata[j+3] = 255;
	}
	Format = CRender::FORMAT_RGBA8;
	SAFE_DELETE_ARRAY(data);
	data = newdata;
	
}

void CImage::QuarterImage()
{
	uint32 width = Width/2;
	uint32 height = Height/2;
	uChar *new_data = new uChar[width*height*GetChannelCount()];

	uint32 k = Width*GetChannelCount();
	for(uint32 r=0,nr=0;nr<width*height*GetChannelCount();r+=Width*2*GetChannelCount(),nr+=width*GetChannelCount())
	{
		for(uint32 i=0,j=0;j<(width*GetChannelCount());i+=GetChannelCount()*2,j+=GetChannelCount())
		{
			if(GetChannelCount() > 0)new_data[j+nr] =   (data[i+r] + data[i+r+4]+data[i+r+k] + data[i+r+k+4])/4;
			if(GetChannelCount() > 1)new_data[j+nr+1] = (data[i+r+1]+data[i+r+5]+data[i+r+k+1]+data[i+r+k+5])/4;
			if(GetChannelCount() > 2)new_data[j+nr+2] = (data[i+r+2]+data[i+r+6]+data[i+r+k+2]+data[i+r+k+6])/4;
			if(GetChannelCount() > 3)new_data[j+nr+3] = (data[i+r+3]+data[i+r+7]+data[i+r+k+3]+data[i+r+k+7])/4;
		}
	}

	Width = width;
	Height = height;
	SAFE_DELETE_ARRAY(data);
	data = new uChar[width*height*GetChannelCount()];
	memcpy(data,new_data,width*height*GetChannelCount());
	SAFE_DELETE_ARRAY(new_data);
}

uint32 CImage::GetPixelCount()
{
	uint32 w = Width;
	uint32 h = Height;
	uint32 d = Depth;
	uint32 size = w * h * d;
	w >>= 1;
	h >>= 1;
	d >>= 1;
	if (w + h + d != 0)
	{
		if (w == 0) w = 1;
		if (h == 0) h = 1;
		if (d == 0) d = 1;
	}
	return (d == 0)? 6 * size : size;
}

Bool CImage::RemoveChannels(Bool ch0,Bool ch1,Bool ch2,Bool ch3)
{
	return false;
}

Bool CImage::GrayScale()
{
	int32 nChannels = GetChannelCount(Format);

	//!isPlainFormat(format) || isFloatFormat(format) ||
	if(nChannels < 3)return false;

	uint32 len = GetPixelCount();
	uint32 size = len;

	if( Format == CRender::FORMAT::FORMAT_R8 || 
		Format == CRender::FORMAT::FORMAT_RG8 || 
		Format == CRender::FORMAT::FORMAT_RGB8 || 
		Format == CRender::FORMAT::FORMAT_RGBA8)
	{
		uChar *src = data, *dest = data;
		do
		{
			*dest++ = (77 * src[0] + 151 * src[1] + 28 * src[2] + 128) >> 8;
			src += nChannels;
		}
		while (--len);

		Format = CRender::FORMAT::FORMAT_R8;
	} 
	/*
	else 
	{
		uint16 *src = (uint16 *) data, *dest = (uint16 *) data;
		do 
		{
			*dest++ = (77 * src[0] + 151 * src[1] + 28 * src[2] + 128) >> 8;
			src += nChannels;
		}
		while (--len);

		Format = CRender::FORMAT::FORMAT_R16;
		size *= 2;
	}*/
	data = (uChar *)ReAlloc(data, size);
	return true;
}

Bool CImage::NormalMap(Float32 sZ)
{
	return false;
}

int32 CImage::LoadImageFromFile(Str filename)
{
	if(strstr(filename,".jpg") || strstr(filename,".JPG"))
	{
		return LoadImageDataFromFileJPG(filename);
	}
	else if(strstr(filename,".tga") || strstr(filename,".TGA"))
	{
		return LoadImageDataFromFileTGA(filename);
	}
	else if(strstr(filename,".png") || strstr(filename,".PNG"))
	{
		return LoadImageDataFromFilePNG(filename);
	}
	else if(strstr(filename,".dds") || strstr(filename,".DDS"))
	{
		return LoadImageDataFromFileDDS(filename);
	}

	return 0;
}
int32 CImage::LoadImageDataFromFileJPG(Str filename) { return 0; }
int32 CImage::SaveImageDataInFileJPG(Str filename) { return 0; }
/*
struct my_error_mgr 
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

static void my_error_exit(j_common_ptr cinfo) 
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(myerr->setjmp_buffer,1);
}

int32 CImage::LoadImageDataFromFileJPG(Str filename)
{
	FILE *file = fopen(filename,"rb");
	if(file == NULL) 
	{
		//Error("Can`t open (%s) file",filename);
		Format = 0;
		Width = 0;
		Height = 0;
		data = 0;
		return 0;
	}

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if(setjmp(jerr.setjmp_buffer)) 
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(file);
		return 0;
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);		
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	switch (cinfo.num_components)
	{
		case 1:
			Format = CRender::FORMAT_R8;
			break;
		case 3:
			Format = CRender::FORMAT_RGB8;
			break;
		case 4:
			Format = CRender::FORMAT_RGBA8;
			break;
	}
	
	
	Width = cinfo.image_width;
	Height = cinfo.image_height;
	Depth = 1;

	int32 rowSpan = cinfo.image_width * cinfo.num_components;
	
	data = new uChar[rowSpan * Height];
	uChar** rowPtr = new uChar*[Height];

	for(uint32 i = 0; i < Height; i++)
	{
		rowPtr[i] = &(data[i*rowSpan]);
	}

	int32 rowsRead = 0;
	while(cinfo.output_scanline < cinfo.output_height) 
	{
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	}
	
	SAFE_DELETE_ARRAY(rowPtr);
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	FlipV();

	fclose(file);

	ConvertToRGBA();

	return 1;
}

int32 CImage::SaveImageDataInFileJPG(Str filename)
{
	if(Format != CRender::FORMAT::FORMAT_R8 && Format != CRender::FORMAT::FORMAT_RGB8 && Format != CRender::FORMAT::FORMAT_RGBA8)return false;

	
	FILE *file = fopen(filename, "wb");
	if(file == NULL)
	{
		return 0;
	}

	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	int32 nChannels = GetChannelCount(Format);

	cinfo.in_color_space = (nChannels == 1)? JCS_GRAYSCALE : JCS_RGB;
	jpeg_set_defaults(&cinfo);

	cinfo.input_components = nChannels;
	cinfo.num_components   = nChannels;
	cinfo.image_width  = Width;
	cinfo.image_height = Height;
	cinfo.data_precision = 8;
	cinfo.input_gamma = 1.0;

	int32 quality = 12;
	jpeg_set_quality(&cinfo, quality, FALSE);

	jpeg_stdio_dest(&cinfo, file);
	jpeg_start_compress(&cinfo, TRUE);

	uChar *curr_scanline = data;

	for (uint32 y = 0; y < Height; y++)
	{
		jpeg_write_scanlines(&cinfo, &curr_scanline, 1);
		curr_scanline += nChannels * Width;
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	fclose(file);
	return 1;
}
*/
#pragma pack(1)
struct tga_header 
{
	unsigned char id_length;
	unsigned char colormap_type;
	unsigned char image_type;
	unsigned short colormap_index;
	unsigned short colormap_length;
	unsigned char colormap_size;
	unsigned short x_orign;
	unsigned short y_orign;
	unsigned short width;
	unsigned short height;
	unsigned char pixel_size;
	unsigned char attributes;
};
#pragma pack(4)


int32 CImage::LoadImageDataFromFileTGA(Str filename)
{
	FILE *file = fopen(filename,"rb");
	if(file == NULL)
	{
		//GetEngine()->Error("Can`t open (%s) file",filename);
		Format = 0;
		Width = 0;
		Height = 0;
		data = 0;
		return 0;
	}
	else
	{
		tga_header header;
		fread(&header,sizeof(header),1,file);
		Width = header.width;
		Height = header.height;
		
		// skip comment
		fseek(file,header.id_length,SEEK_CUR);
		
		// colormap
		unsigned char *colormap = NULL;
		if(header.colormap_length > 0) 
		{
			int size = header.colormap_length * header.colormap_size / 8;
			colormap = new unsigned char[size];
			fread(colormap,size,1,file);
		}
		
		// image
		unsigned char *img_data = new unsigned char[Width * Height * 4];
		Format = CRender::FORMAT_RGBA8;
		// uncompressed colormapped and uncompressed monochrome image
		if(header.image_type == 1 || header.image_type == 3)
		{
			uint32 size = Width * Height;
			unsigned char *buf = new unsigned char[size];
			fread(buf,sizeof(unsigned char),size,file);
			// colormapped image
			if(header.image_type == 1)
			{
				for(uint32 i = 0, j = 0; i < size; i++, j += 4)
				{
					if(header.colormap_size == 8)
					{
						img_data[j + 0] = colormap[buf[i]];
						img_data[j + 1] = colormap[buf[i]];
						img_data[j + 2] = colormap[buf[i]];
						img_data[j + 3] = 255;
					}
					else if(header.colormap_size == 24)
					{
						img_data[j + 0] = colormap[buf[i] * 3 + 2];
						img_data[j + 1] = colormap[buf[i] * 3 + 1];
						img_data[j + 2] = colormap[buf[i] * 3 + 0];
						img_data[j + 3] = 255;
					}
					else if(header.colormap_size == 32)
					{
						img_data[j + 0] = colormap[buf[i] * 4 + 3];
						img_data[j + 1] = colormap[buf[i] * 4 + 2];
						img_data[j + 2] = colormap[buf[i] * 4 + 1];
						img_data[j + 3] = colormap[buf[i] * 4 + 0];
					}
				}
			}
			// monochrome image
			else
			{
				for(uint32 i = 0, j = 0; i < size; i++, j += 4)
				{
					img_data[j + 0] = buf[i];
					img_data[j + 1] = buf[i];
					img_data[j + 2] = buf[i];
					img_data[j + 3] = 255;
				}
			}
			SAFE_DELETE_ARRAY(buf);
		}
		// uncompressed and compressed RGB/RGBA image
		else if(header.image_type == 2 || header.image_type == 10)
		{
			uint32 components = 0;
			if(header.pixel_size == 24)
			{
				components = 3;
			}
			else if(header.pixel_size == 32)
			{
				components = 4;
			}
			else
			{
				//GetEngine()->Error("Only 24 and 32 bpp images are suported : (%s) file",filename);
				fclose(file);
				if(colormap) SAFE_DELETE_ARRAY(colormap);
				SAFE_DELETE_ARRAY(img_data);
				Width = 0;
				Height = 0;
				data = 0;
				return 0;
			}
			uint32 size = Width * Height * components;
			unsigned char *buf = new unsigned char[size];
			// uncompressed
			if(header.image_type == 2)
			{
				fread(buf,sizeof(unsigned char),size,file);
			}
			// RLE compressed
			else {
				unsigned char *ptr = buf;
				for(uint32 i = 0; i < size;)
				{
					unsigned char rep = fgetc(file);
					if(rep & 0x80) 
					{
						rep ^= 0x80;
						fread(ptr,sizeof(unsigned char),components,file);
						ptr += components;
						for(uint32 j = 0; j < rep * components; j++)
						{
							*ptr = *(ptr - components);
							ptr++;
						}
						i += components * (rep + 1);
					} 
					else
					{
						int j = components * (rep + 1);
						fread(ptr,sizeof(unsigned char),j,file);
						ptr += j;
						i += j;
					}
				}
			}
			// convert to RGBA
			for(uint32 i = 0, j = 0; i < size; i += components, j += 4)
			{
				img_data[j] = buf[i + 2];
				img_data[j + 1] = buf[i + 1];
				img_data[j + 2] = buf[i];
				if(components == 3) img_data[j + 3] = 255;
				else img_data[j + 3] = buf[i + 3];
			}
			SAFE_DELETE_ARRAY(buf);
		}
		else 
		{
			//GetEngine()->Error("unsupported image type (%s) in file",(char*)header.image_type);
			//GetEngine()->Error("unsupported image type in (%s) file",filename);
			fclose(file);
			if(colormap) SAFE_DELETE_ARRAY(colormap);
			SAFE_DELETE_ARRAY(img_data);
			Format = 0;
			Width = 0;
			Height = 0;
			data = 0;
			return 0;
		}
		
		// flip image
	
		if((header.attributes & 0x20) == 0)
		{
			for(uint32 y = 0; y < Height / 2; y++)
			{
				int c;
				int *s = (int*)&img_data[Width * y * 4];
				int *d = (int*)&img_data[(Height - y - 1) * Width * 4];
				for(uint32 x = 0; x < Width; x++)
				{
					c = *d;
					*d++ = *s;
					*s++ = c;
				}
			}
		}		
		if(colormap) SAFE_DELETE_ARRAY(colormap);
		data = img_data;
	}

	fclose(file);

	FlipV();

	ConvertToRGBA();

	return 1;
}

int32 CImage::SaveImageDataInFileTGA(Str filename)
{
	FILE *file = fopen(filename,"wb");
	if(!file) 
	{
		return 0;
	}
	uint32 i,j;
	uChar *buf;
	buf = new uChar[18 + Width * Height * 4];
	memset(buf,0,18);
	buf[2] = 2;
	buf[12] = Width % 256;
	buf[13] = Width / 256;
	buf[14] = Height % 256;
	buf[15] = Height / 256;
	buf[16] = 32;
	buf[17] = 0x28;
	memcpy(buf + 18,data,Width * Height * 4);
	for(i = 18; i < 18 + Width * Height * 4; i += 4) 
	{
		j = buf[i];
		buf[i] = buf[i + 2];
		buf[i + 2] = j;
	}
	fwrite(buf,1,18 + Width * Height * 4,file);
	fclose(file);
	SAFE_DELETE(buf);
	return 1;
}

//PNG

void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	fwrite(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	fread(data, length, 1, (FILE *) png_get_io_ptr(png_ptr));
}

void user_flush_data(png_structp png_ptr)
{
	fflush((FILE *) png_get_io_ptr(png_ptr));
}

png_voidp malloc_fn(png_structp png_ptr, png_size_t size)
{
	return malloc(size);
}

void free_fn(png_structp png_ptr, png_voidp ptr)
{
	free(ptr);
}

int32 CImage::LoadImageDataFromFilePNG(Str filename)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
    FILE *file;

    png_byte pbSig[8];
    int iBitDepth, iColorType;
    png_byte **ppbRowPointers;

    if ((file = fopen(filename, "rb")) == NULL)
	{
		//Error("can`t open (%s) file",filename);
		Width = 0;
		Height = 0;
		data = 0;
		return 0;
	}

    // first check the eight byte PNG signature
    fread(pbSig, 1, 8, file);
    if (!png_check_sig(pbSig, 8))
	{
		//Error("can`t first check the eight byte PNG signature (%s) file",filename);
		fclose(file);
		Width = 0;
		Height = 0;
		data = 0;
		return 0;
	}

    // create the two png(-info) structures
    if ((png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn)) == NULL)
	{
		//Error("can`t create the two png(-info) structures (%s) file",filename);
		fclose(file);
        Width = 0;
		Height = 0;
		data = 0;
		return 0;
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL)
	{
		//GetEngine()->Error("can`t create info_struct (%s) file",filename);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(file);
        Width = 0;
		Height = 0;
		data = 0;
		return 0;
    }

	// initialize the png structure
	png_set_read_fn(png_ptr, file, user_read_data);
	png_set_sig_bytes(png_ptr, 8);
	
	// read all PNG info up to image data
	png_read_info(png_ptr, info_ptr);

	// get width, height, bit-depth and color-type
	png_uint_32 w, h;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &iBitDepth, &iColorType, NULL, NULL, NULL);

	Width = w;
	Height = h;
	Depth = 1;
	int depth = 1;
    int	nMipMaps = 1;

	int nChannels = png_get_channels(png_ptr, info_ptr);
	switch (nChannels)
	{
		case 1:
			Format = (iBitDepth > 8)? CRender::FORMAT_R16 : CRender::FORMAT_R8;
			break;
		case 2:
			Format = (iBitDepth > 8)? CRender::FORMAT_RG16 : CRender::FORMAT_RG8;
			break;
		case 3:
			Format = (iBitDepth > 8)? CRender::FORMAT_RGB16 : CRender::FORMAT_RGB8;
			break;
		case 4:
			Format = (iBitDepth > 8)? CRender::FORMAT_RGBA16 : CRender::FORMAT_RGBA8;
			break;
	}
	int rowSize = Width * nChannels * iBitDepth / 8;

	// now we can allocate memory to store the image
	data = new uChar[rowSize * Height];
	
	// set the individual row-pointers to point at the correct offsets
	ppbRowPointers = new png_bytep[Height];
	for (uint32 i = 0; i < Height; i++)
	{
		ppbRowPointers[i] = data + i * rowSize;
	}
	
	// now we can go ahead and just read the whole image
	png_read_image(png_ptr, ppbRowPointers);
	
	// read the additional chunks in the PNG file (not really needed)
	png_read_end(png_ptr, NULL);
	
	SAFE_DELETE(ppbRowPointers);

	if (iColorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_colorp palette;
		int num_palette;
		png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

		uChar *newPixels = new uChar[Width * Height * 3];
		if (iBitDepth == 4)
		{
			for (uint32 i = 0; i < rowSize * Height; i++)
			{
				uint32 i0 = data[i] >> 4;
				uint32 i1 = data[i] & 0xF;
				newPixels[6 * i    ] = palette[i0].red;
				newPixels[6 * i + 1] = palette[i0].green;
				newPixels[6 * i + 2] = palette[i0].blue;
				newPixels[6 * i + 3] = palette[i1].red;
				newPixels[6 * i + 4] = palette[i1].green;
				newPixels[6 * i + 5] = palette[i1].blue;
			}
		}
		else 
		{
			for (uint32 i = 0; i < rowSize * Height; i++)
			{
				newPixels[3 * i    ] = palette[data[i]].red;
				newPixels[3 * i + 1] = palette[data[i]].green;
				newPixels[3 * i + 2] = palette[data[i]].blue;
			}
		}

		Format = CRender::FORMAT_RGB8;
		SAFE_DELETE_ARRAY(data);
		data = newPixels;
	}
	// and we're done
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);

	FlipV();
	ConvertToRGBA();


	return 1;
}

int32 CImage::SaveImageDataInFilePNG(Str filename)
{

	FILE *file = fopen(filename, "wb");
	if(file == NULL)
	{
		return 0;
	}

	int32 type;

	switch (Format)
	{
		case CRender::FORMAT::FORMAT_R8:
			case CRender::FORMAT::FORMAT_R16:
				type = PNG_COLOR_TYPE_GRAY;
				break;
		case CRender::FORMAT::FORMAT_RG8:
			case CRender::FORMAT::FORMAT_RG16:
				type = PNG_COLOR_TYPE_GRAY_ALPHA;
				break;
		case CRender::FORMAT::FORMAT_RGB8:
			case CRender::FORMAT::FORMAT_RGB16:
				type = PNG_COLOR_TYPE_RGB;
				break;
		case CRender::FORMAT::FORMAT_RGBA8:
			case CRender::FORMAT::FORMAT_RGBA16:
				type = PNG_COLOR_TYPE_RGBA;
				break;
		default:
			return 0;
	}

	png_structp png_ptr;
	png_infop info_ptr;

    if ((png_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, malloc_fn, free_fn)) == NULL)
	{
		fclose(file);
        return 0;
    }

    if ((info_ptr = png_create_info_struct(png_ptr)) == NULL)
	{
        png_destroy_write_struct(&png_ptr, NULL);
		fclose(file);
        return 0;
    }

	png_set_write_fn(png_ptr, file, user_write_data, user_flush_data);

	int32 bpp = (Format == CRender::FORMAT::FORMAT_R16)? 16 : 8;

	png_set_IHDR(png_ptr, info_ptr, Width, Height, bpp, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	//png_set_compression_level(png_ptr, 9);
	png_write_info(png_ptr, info_ptr);

	int32 nElements = Width * GetChannelCount(Format);
	if(Format >= CRender::FORMAT::FORMAT_R16)
	{
		uint16 *line = new uint16[nElements];
		for (uint32 y = 0; y < Height; y++)
		{
			uint16 *src = ((uint16 *) data) + y * nElements;
			// Swap endian
			for (int32 x = 0; x < nElements; x++)
			{
				line[x] = (src[x] >> 8) | (src[x] << 8);
			}
			png_write_row(png_ptr, (uChar *) line);
		}
		SAFE_DELETE_ARRAY(line);
	} 
	else 
	{
		for(uint32 y = 0; y < Height; y++)
		{
			png_write_row(png_ptr, data + y * nElements);
		}
	}

	png_write_end(png_ptr, info_ptr);
	
    png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);
	return 1;
}

int32 CImage::LoadImageDataFromFileDDS(Str filename)
{
	
	FILE *file = fopen(filename,"rb");
	if(file == NULL)
	{
		//GetEngine()->Error("Cant`t open (%s) file",filename);
		Format = 0;
		Width = 0;
		Height = 0;
		data = 0;
		return 0;
	}
	else
	{
	
		dds_header header;
		fread(&header,sizeof(dds_header),1,file);
		if(header.magic != ('D' | 'D' << 8 | 'S' << 16 | ' ' << 24)) 
		{
		//	GetEngine()->Error("Error in magic header (%s) file",filename);
			Format = 0;
			Width = 0;
			Height = 0;
			data = 0;
			return 0;
		}
		
		Width = header.dwWidth;
		Height = header.dwHeight;
		int32 frmt = DDS_ERROR;
		
		if(header.dwPFFlags & DDPF_FOURCC)
		{
			if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('1' << 24))) frmt = DDS_DXT1;
			else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('2' << 24))) frmt = DDS_DXT2;
			else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('3' << 24))) frmt = DDS_DXT3;
			else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('4' << 24))) frmt = DDS_DXT4;
			else if(header.dwFourCC == ('D' | ('X' << 8) | ('T' << 16) | ('5' << 24))) frmt = DDS_DXT5;
		}
		else if(header.dwPFFlags == DDPF_RGB && header.dwRGBBitCount == 24)
		{
			Format = CRender::FORMAT_RGB8;
			frmt = DDS_RGB;
		}
		else if(header.dwPFFlags == DDPF_RGBA && header.dwRGBBitCount == 32)
		{
			Format = CRender::FORMAT_RGBA8;
			frmt = DDS_RGBA;
		}
		
		if(frmt == DDS_ERROR)
		{
			//GetEngine()->Error("unknown texture format in (%s) file",filename);
			Format = 0;
			Width = 0;
			Height = 0;
			data = 0;
			return 0;
		}
		
		unsigned char *img_data = new unsigned char[Width * Height * 4];
		if(frmt == DDS_RGB) 
		{
			Format = CRender::FORMAT_RGB8;
			unsigned char *buf = new unsigned char[Width * Height * 3];
			fread(buf,sizeof(unsigned char),Width * Height * 3,file);
			unsigned char *src = buf;
			unsigned char *dest = img_data;
			for(uint32 y = 0; y < Height; y++)
			{
				for(uint32 x = 0; x < Width; x++) 
				{
					*dest++ = *src++;
					*dest++ = *src++;
					*dest++ = *src++;
					*dest++ = 255;
				}
			}
			SAFE_DELETE_ARRAY(buf);
		}
		else if(frmt == DDS_RGBA)
		{
			Format = CRender::FORMAT_RGBA8;
			unsigned char *buf = new unsigned char[Width * Height * 4];
			fread(buf,sizeof(unsigned char),Width * Height * 4,file);
			unsigned char *src = buf;
			unsigned char *dest = img_data;
			for(uint32 y = 0; y < Height; y++)
			{
				for(uint32 x = 0; x < Width; x++)
				{
					*dest++ = *src++;
					*dest++ = *src++;
					*dest++ = *src++;
					*dest++ = *src++;
				}
			}
			SAFE_DELETE_ARRAY(buf);
		}
		else 
		{
			Format = CRender::FORMAT_RGBA8;
			unsigned char *buf = new unsigned char[Width * Height];
			unsigned char *src = buf;
			fread(buf,sizeof(unsigned char),Width * Height,file);
			for(uint32 y = 0; y < Height; y += 4)
			{
				for(uint32 x = 0; x < Width; x += 4)
				{
					unsigned long alpha = 0;
					unsigned int a0 = 0;
					unsigned int a1 = 0;
					if(frmt == DDS_DXT3)
					{
						alpha = *(unsigned long*)src;
						src += 8;
					} 
					else if(frmt == DDS_DXT5) 
					{
						alpha = (*(unsigned long*)src) >> 16;
						a0 = src[0];
						a1 = src[1];
						src += 8;
					}
					unsigned int c0 = *(unsigned short*)(src + 0);
					unsigned int c1 = *(unsigned short*)(src + 2);
					src += 4;
					dds_color color[4];
					color[0].r = ((c0 >> 11) & 0x1f) << 3;
					color[0].g = ((c0 >> 5) & 0x3f) << 2;
					color[0].b = (c0 & 0x1f) << 3;
					color[1].r = ((c1 >> 11) & 0x1f) << 3;
					color[1].g = ((c1 >> 5) & 0x3f) << 2;
					color[1].b = (c1 & 0x1f) << 3;
					if(c0 > c1)
					{
						color[2].r = (color[0].r * 2 + color[1].r) / 3;
						color[2].g = (color[0].g * 2 + color[1].g) / 3;
						color[2].b = (color[0].b * 2 + color[1].b) / 3;
						color[3].r = (color[0].r + color[1].r * 2) / 3;
						color[3].g = (color[0].g + color[1].g * 2) / 3;
						color[3].b = (color[0].b + color[1].b * 2) / 3;
					}
					else 
					{
						color[2].r = (color[0].r + color[1].r) / 2;
						color[2].g = (color[0].g + color[1].g) / 2;
						color[2].b = (color[0].b + color[1].b) / 2;
						color[3].r = 0;
						color[3].g = 0;
						color[3].b = 0;
					}
					for(uint32 i = 0; i < 4; i++)
					{
						unsigned int index = *src++;
						unsigned char *dest = img_data + (Width * (y + i) + x) * 4;
						for(int j = 0; j < 4; j++) {
							*dest++ = color[index & 0x03].r;
							*dest++ = color[index & 0x03].g;
							*dest++ = color[index & 0x03].b;
							if(frmt == DDS_DXT1) 
							{
								*dest++ = ((index & 0x03) == 3 && c0 <= c1) ? 0 : 255;
							} 
							else if(frmt == DDS_DXT3)
							{
								*dest++ = (unsigned char)((alpha & 0x0f) << 4);
								alpha >>= 4;
							} 
							else if(frmt == DDS_DXT5)
							{
								unsigned int a = (unsigned int)(alpha & 0x07);
								if(a == 0) *dest++ = a0;
								else if(a == 1) *dest++ = a1;
								else if(a0 > a1) *dest++ = ((8 - a) * a0 + (a - 1) * a1) / 7;
								else if(a > 5) *dest++ = (a == 6) ? 0 : 255;
								else *dest++ = ((6 - a) * a0 + (a - 1) * a1) / 5;
								alpha >>= 3;
							}
							else
							{
								*dest++ = 255;
							}
							index >>= 2;
						}
					}
				}
			}
			SAFE_DELETE_ARRAY(buf);
		}

		data = img_data;
	}
	
	fclose(file);

	FlipV();

	ConvertToRGBA();

	return 1;
}

int32 CImage::SaveImageDataInFileDDS(Str filename)
{
	
	return 0;
}