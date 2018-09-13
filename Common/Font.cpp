#include "Font.h"
#include "Image.h"

CFont::CFont()
{
	m_pFonts = new FontData[1];
	m_iInitSize = new uint32[1];
	m_iFontsCount = 0;
	m_iTexureId = 0;
	m_iSpace = 0;
	m_iFontHeight = 0;
	m_iFontTexture = 0;
}

CFont::~CFont()
{
	m_iTexureId = 0;
	m_iSpace = 0;
	m_iFontHeight = 0;
	SAFE_DELETE_ARRAY(m_vTexCoords)
	SAFE_DELETE_ARRAY(m_vSizes)
	SAFE_DELETE_ARRAY(m_pFonts);
	SAFE_DELETE_ARRAY(m_iInitSize);
}

uint32 CFont::CreateTextureForSymbols(CRender *render,Str text,Str ttf_name,uint32 font_height,uint32 font_space,uint32 texture_width,uint32 texture_height)
{
	m_iSpace = font_space;
	m_iFontHeight = font_height;
	m_iHeightStep = font_height*2;
	m_iWidthStep = font_height*2;
	m_sSymbols = text;

	m_iTextureWidth = texture_width;
	m_iTextureHeight = texture_height;

	m_iScreenWidth = render->GetScreenWidth();
	m_iScreenHeight = render->GetScreenHeight();

	FT_Library library;
	FT_Face face;
	FT_Glyph glyph;
	uint32 h = font_height;

	FT_Init_FreeType(&library);
	FT_New_Face(library,ttf_name,0,&face);
	FT_Set_Char_Size(face,h << 6,h << 6,96,96);

	CImage font_img;
	font_img.CreateEmptyTexture(texture_width,texture_height,CRender::FORMAT_RGBA8,1,0,0,0,0);
	m_iFontTexture = render->CreateTexture(CRender::TEXTURE_TYPE_2D,font_img.GetWidth(),font_img.GetHeight(),font_img.GetChannelCount(),font_img.GetFormat(),font_img.GetDepth(),0,0);
	render->FillTexture(m_iFontTexture,font_img.GetData());

	uint32 width_count = texture_width/(font_height*2);
	uint32 height_count = texture_height/(font_height*2);

	m_fDeltaXStep = (Float32)1.0f/(width_count);
	m_fDeltaYStep = (Float32)1.0f/(height_count);

	m_vTexCoords = new Vec2f[width_count*height_count];
	m_vSizes = new Vec2f[width_count*height_count];

	uint32 width_iter = 0;
	for(uint32 f=0,fw=0;f<text.Size();f++,fw++)
	{
		if(f >= width_count*height_count)
		{
			break;
		}

		FT_Load_Glyph( face, FT_Get_Char_Index( face, text[f] ), FT_LOAD_DEFAULT );
		FT_Get_Glyph( face->glyph, &glyph );
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		uint32 width = font_height*2; //bitmap.width;
		uint32 height = font_height*2;//bitmap.rows;


		uint8* expanded_data = new uint8[width * height * font_img.GetChannelCount()];

		for(uint32 i=0;i<width * height * font_img.GetChannelCount();i+=font_img.GetChannelCount())
		{
			if(font_img.GetChannelCount() > 0)expanded_data[i] = 0;
			if(font_img.GetChannelCount() > 1)expanded_data[i+1] = 0;
			if(font_img.GetChannelCount() > 2)expanded_data[i+2] = 0;
			if(font_img.GetChannelCount() > 3)expanded_data[i+3] = 0;
		}

		uint32 jj=(uint32)bitmap_glyph->top+(font_height/2);
		for(uint32 j=0;j<height;j++) 
		{
			for(uint32 i=0;i<width;i++)
			{
				expanded_data[font_img.GetChannelCount()*(i+jj*width)] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+1] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+2] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+3] =((int32)i>=bitmap.width || (int32)j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			}
			if(jj == 0)break;
			jj--;
		}
	
		render->FillPartTexture(m_iFontTexture,expanded_data,0,width*fw,height*width_iter,width,height);
		m_vTexCoords[text[f]] = Vec2f((Float32)fw/(width_count),(Float32)width_iter/(height_count));
		
		if(Str(text[f]) == Str(" "))
		{
			m_vSizes[text[f]] = Vec2f((Float32)font_height*0.5f,(Float32)font_height);
		}
		else
		{
			m_vSizes[text[f]] = Vec2f((Float32)bitmap.width,(Float32)bitmap.rows);
		}
		

		if(fw == (texture_width/width)-1)
		{
			fw=-1;
			++width_iter;
		}

		SAFE_DELETE_ARRAY(expanded_data);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return m_iFontTexture;
}

uint32 CFont::ChangeFont(CRender *render,Str text,Str ttf_name,uint32 font_height,uint32 font_space)
{
	m_iSpace = font_space;
	m_iFontHeight = font_height;
	m_iHeightStep = font_height*2;
	m_iWidthStep = font_height*2;

	m_iScreenWidth = render->GetScreenWidth();
	m_iScreenHeight = render->GetScreenHeight();

	FT_Library library;
	FT_Face face;
	FT_Glyph glyph;
	uint32 h = font_height;

	FT_Init_FreeType(&library);
	FT_New_Face(library,ttf_name,0,&face);
	FT_Set_Char_Size(face,h << 6,h << 6,96,96);

	CImage font_img;
	font_img.CreateEmptyTexture(m_iTextureWidth,m_iTextureHeight,CRender::FORMAT_RGBA8,1,0,0,0,0);
	render->FillTexture(m_iFontTexture,font_img.GetData());

	uint32 width_count = m_iTextureWidth/(font_height*2);
	uint32 height_count = m_iTextureHeight/(font_height*2);

	m_fDeltaXStep = (Float32)1.0f/(width_count);
	m_fDeltaYStep = (Float32)1.0f/(height_count);

	m_vTexCoords = new Vec2f[width_count*height_count];
	m_vSizes = new Vec2f[width_count*height_count];

	uint32 width_iter = 0;
	for(uint32 f=0,fw=0;f<text.Size();f++,fw++)
	{
		if(f >= width_count*height_count)
		{
			break;
		}

		FT_Load_Glyph( face, FT_Get_Char_Index( face, text[f] ), FT_LOAD_DEFAULT );
		FT_Get_Glyph( face->glyph, &glyph );
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		uint32 width = font_height*2; //bitmap.width;
		uint32 height = font_height*2;//bitmap.rows;


		uint8* expanded_data = new uint8[width * height * font_img.GetChannelCount()];

		for(uint32 i=0;i<width * height * font_img.GetChannelCount();i+=font_img.GetChannelCount())
		{
			if(font_img.GetChannelCount() > 0)expanded_data[i] = 0;
			if(font_img.GetChannelCount() > 1)expanded_data[i+1] = 0;
			if(font_img.GetChannelCount() > 2)expanded_data[i+2] = 0;
			if(font_img.GetChannelCount() > 3)expanded_data[i+3] = 0;
		}

		uint32 jj=(uint32)bitmap_glyph->top+(font_height/2);
		for(uint32 j=0;j<height;j++) 
		{
			for(uint32 i=0;i<width;i++)
			{
				expanded_data[font_img.GetChannelCount()*(i+jj*width)] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+1] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+2] =
				expanded_data[font_img.GetChannelCount()*(i+jj*width)+3] =((int32)i>=bitmap.width || (int32)j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			}
			if(jj == 0)break;
			jj--;
		}
	
		render->FillPartTexture(m_iFontTexture,expanded_data,0,width*fw,height*width_iter,width,height);
		m_vTexCoords[text[f]] = Vec2f((Float32)fw/(width_count),(Float32)width_iter/(height_count));
		if(Str(text[f]) == Str(" "))
		{
			m_vSizes[text[f]] = Vec2f((Float32)font_height*0.5f,(Float32)font_height);
		}
		else
		{
			m_vSizes[text[f]] = Vec2f((Float32)bitmap.width,(Float32)bitmap.rows);
		}

		if(fw == (m_iTextureWidth/width)-1)
		{
			fw=-1;
			++width_iter;
		}

		SAFE_DELETE_ARRAY(expanded_data);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return 1;
}

uint32 CFont::InitStaticText(CRender *render,Str text,uint32 size,uint32 space,uint32 bettween_line_space)
{
	uint32 font_id = 0;
	++m_iFontsCount;
	m_pFonts = (FontData*)ReAlloc(m_pFonts,m_iFontsCount * sizeof(FontData));
	m_iInitSize = (uint32*)ReAlloc(m_iInitSize,m_iFontsCount * sizeof(uint32));
	font_id = m_iFontsCount-1;
	Bool slash_flag = false;

	m_pFonts[font_id].m_iFontSpace = space;
	m_pFonts[font_id].m_iFontSize = size == 0 ? GetFontHeight() : size;
	m_pFonts[font_id].m_iFontBettweenLineSpace = bettween_line_space;

	uint32 text_length = text.Size();
	m_iInitSize[font_id] = text_length;

	Vec3f *vdata = new Vec3f[text_length*6];
	Vec2f *tdata = new Vec2f[text_length*6];

	int32 width_start = 0;
	int32 height_start = 0;
	Float32 scale = 0.0f;
	scale = Float32(m_pFonts[font_id].m_iFontSize)/Float32(GetFontHeight());

	m_pFonts[font_id].m_iStringMaxWidth = 0;
	m_pFonts[font_id].m_iStringMaxHeight = int32(m_iFontHeight*scale);

	uint32 symbol_id;
	for(uint32 i=0,j=0;i<text_length*6;i+=6,j++)
	{
		if(Str(text[j]) == Str("\\"))
		{
			slash_flag = true;
		}
		else
		{
			if(!slash_flag)
			{
				symbol_id = text[j];
				Float32 dx = Float32(32/m_vSizes[symbol_id].v[0]);
				Float32 dy = 1.2f;

				tdata[i] =   Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+1] = Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]);
				tdata[i+2] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+3] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);
				tdata[i+4] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+5] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);

				vdata[i] = Vec3f((Float32)width_start,(Float32)height_start,0.0f);
				vdata[i+1] = Vec3f((Float32)width_start,(Float32)height_start-m_iFontHeight*scale,0.0f);
				vdata[i+2] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start,0.0f);
				vdata[i+3] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start-m_iFontHeight*scale,0.0f);
				vdata[i+4] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start,0.0f);
				vdata[i+5] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start-m_iFontHeight*scale,0.0f);

				width_start += int32(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace;
				if(m_pFonts[font_id].m_iStringMaxWidth < width_start)
				{
					m_pFonts[font_id].m_iStringMaxWidth = width_start;
				}
			}
			if(slash_flag)
			{
				if(Str(text[j]) == Str("n"))
				{
					slash_flag = false;
					height_start-=int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
					
					m_pFonts[font_id].m_iStringMaxHeight += int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
					
					width_start = 0;
				}
			}
		}
	}

	int32 *idata = new int32[text_length*6];

	for(int32 i=0;i<int32(text_length*6);i++)
	{
		idata[i] = i;
	}

	m_pFonts[font_id].m_iFontGeometryId = render->CreateGeometry();
	render->AddVertexDeclaration(m_pFonts[font_id].m_iFontGeometryId,CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(m_pFonts[font_id].m_iFontGeometryId,CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	render->AddVerexToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,vdata);
	render->AddTexCoordsToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,tdata);
	render->AddIndexToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,idata);
	render->SetStartNewGeometry(m_pFonts[font_id].m_iFontGeometryId);

	render->CreateGeometryVertexBuffer(m_pFonts[font_id].m_iFontGeometryId,CRender::BUFFERACCESS::STATIC);
	render->LoadToVertexBufferGeometry(m_pFonts[font_id].m_iFontGeometryId);
	render->CreateGeometryIndexBuffer(m_pFonts[font_id].m_iFontGeometryId,CRender::BUFFERACCESS::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(m_pFonts[font_id].m_iFontGeometryId);
		
	SAFE_DELETE_ARRAY(vdata);
	SAFE_DELETE_ARRAY(tdata);
	SAFE_DELETE_ARRAY(idata);

	return font_id;
}

uint32 CFont::InitDynamicText(CRender *render,Str text,uint32 size,uint32 space,uint32 bettween_line_space)
{
	uint32 font_id = 0;
	++m_iFontsCount;
	m_pFonts = (FontData*)ReAlloc(m_pFonts,m_iFontsCount * sizeof(FontData));
	m_iInitSize = (uint32*)ReAlloc(m_iInitSize,m_iFontsCount * sizeof(uint32));
	font_id = m_iFontsCount-1;
	Bool slash_flag = false;
	m_pFonts[font_id].m_iFontSpace = space;
	m_pFonts[font_id].m_iFontSize = size == 0 ? GetFontHeight() : size;
	m_pFonts[font_id].m_iFontBettweenLineSpace = bettween_line_space;

	m_pFonts[font_id].m_iFontGeometryId = render->CreateGeometry();
	render->AddVertexDeclaration(m_pFonts[font_id].m_iFontGeometryId,CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(m_pFonts[font_id].m_iFontGeometryId,CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	uint32 text_length = text.Size();
	m_iInitSize[font_id] = text_length;

	Vec3f *vdata = new Vec3f[text_length*6];
	Vec2f *tdata = new Vec2f[text_length*6];

	int32 width_start = 0;
	int32 height_start = 0;
	Float32 scale = 0.0f;
	scale = Float32(m_pFonts[font_id].m_iFontSize)/Float32(GetFontHeight());

	m_pFonts[font_id].m_iStringMaxWidth = 0;
	m_pFonts[font_id].m_iStringMaxHeight = int32(m_iFontHeight*scale);

	uint32 symbol_id;
	for(uint32 i=0,j=0;i<text_length*6;i+=6,j++)
	{
		if(Str(text[j]) == Str("\\"))
		{
			slash_flag = true;
		}
		else
		{
			if(!slash_flag)
			{
				symbol_id = text[j];
				Float32 dx = Float32(32/m_vSizes[symbol_id].v[0]);
				Float32 dy = 1.2f;

				tdata[i] =   Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+1] = Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]);
				tdata[i+2] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+3] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);
				tdata[i+4] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
				tdata[i+5] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);

				vdata[i] = Vec3f((Float32)width_start,(Float32)height_start,0.0f);
				vdata[i+1] = Vec3f((Float32)width_start,(Float32)height_start-m_iFontHeight*scale,0.0f);
				vdata[i+2] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start,0.0f);
				vdata[i+3] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start-m_iFontHeight*scale,0.0f);
				vdata[i+4] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start,0.0f);
				vdata[i+5] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start-m_iFontHeight*scale,0.0f);

				width_start += int32(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace;
				if(m_pFonts[font_id].m_iStringMaxWidth < width_start)
				{
					m_pFonts[font_id].m_iStringMaxWidth = width_start;
				}
			}
			if(slash_flag)
			{
				if(Str(text[j]) == Str("n"))
				{
					slash_flag = false;
					height_start-=int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
					
					m_pFonts[font_id].m_iStringMaxHeight += int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
					
					width_start = 0;
				}
			}
		}
	}
	int32 *idata = new int32[text_length*6];
	for(int32 i=0;i<int32(text_length*6);i++)
	{
		idata[i] = i;
	}

	render->AddVerexToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,vdata);
	render->AddTexCoordsToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,tdata);
	render->AddIndexToGeometry(m_pFonts[font_id].m_iFontGeometryId,text_length*6,idata);
	render->SetStartNewGeometry(m_pFonts[font_id].m_iFontGeometryId);

	render->CreateGeometryVertexBuffer(m_pFonts[font_id].m_iFontGeometryId,CRender::BUFFERACCESS::DYNAMIC);
	render->LoadToVertexBufferGeometry(m_pFonts[font_id].m_iFontGeometryId);
	render->CreateGeometryIndexBuffer(m_pFonts[font_id].m_iFontGeometryId,CRender::BUFFERACCESS::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(m_pFonts[font_id].m_iFontGeometryId);

	SAFE_DELETE_ARRAY(vdata);
	SAFE_DELETE_ARRAY(tdata);
	SAFE_DELETE_ARRAY(idata);

	return font_id;
}

void CFont::DrawStaticText(CRender *render,uint32 font_id,int32 x,int32 y)
{
	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(m_pFonts[font_id].m_iFontGeometryId);
	render->SetIndexBuffer(m_pFonts[font_id].m_iFontGeometryId,0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();

	Float32 scale = Float32(m_pFonts[font_id].m_iFontSize)/Float32(GetFontHeight());
	
	render->Translate2D((Float32)x,(Float32)y);
	
	render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,m_pFonts[font_id].m_iFontGeometryId,0);

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->OnOffBlending(0);
	render->OnOffLighting(1);
	
}

void CFont::DrawDynamicText(CRender *render,uint32 font_id,int32 x,int32 y,Str text)
{
	if(m_pFonts[font_id].m_iFontChanged == 1)
	{
		m_pFonts[font_id].m_iFontChanged = 0;
		Bool slash_flag = false;
		uint32 text_length = text.Size();
		Str space = Str(" ");

		Vec3f *vdata = new Vec3f[m_iInitSize[font_id]*6];
		Vec2f *tdata = new Vec2f[m_iInitSize[font_id]*6];

		int32 width_start = 0;
		int32 height_start = 0;
		Float32 scale = 0.0f;
		scale = Float32(m_pFonts[font_id].m_iFontSize)/Float32(GetFontHeight());

		m_pFonts[font_id].m_iStringMaxWidth = 0;
		m_pFonts[font_id].m_iStringMaxHeight = int32(m_iFontHeight*scale);

		uint32 symbol_id = 0;
		for(uint32 i=0,j=0;i<text.Size()*6;i+=6,j++)
		{
			if(Str(text[j]) == Str("\\"))
			{
				slash_flag = true;
			}
			else
			{
				if(!slash_flag)
				{
					symbol_id = space[0];

					if(i < text_length*6)
					{
						symbol_id = text[j];
					}
					
					Float32 dx = Float32(32/m_vSizes[symbol_id].v[0]);
					Float32 dy = 1.2f;

					tdata[i] =   Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
					tdata[i+1] = Vec2f(m_vTexCoords[symbol_id].v[0],m_vTexCoords[symbol_id].v[1]);
					tdata[i+2] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
					tdata[i+3] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);
					tdata[i+4] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]+(m_fDeltaYStep/dy));
					tdata[i+5] = Vec2f(m_vTexCoords[symbol_id].v[0]+(m_fDeltaXStep/dx),m_vTexCoords[symbol_id].v[1]);

					vdata[i] = Vec3f((Float32)width_start,(Float32)height_start,0.0f);
					vdata[i+1] = Vec3f((Float32)width_start,(Float32)height_start-m_iFontHeight*scale,0.0f);
					vdata[i+2] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start,0.0f);
					vdata[i+3] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale),(Float32)height_start-m_iFontHeight*scale,0.0f);
					vdata[i+4] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start,0.0f);
					vdata[i+5] = Vec3f((Float32)width_start+(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace,(Float32)height_start-m_iFontHeight*scale,0.0f);

					width_start += int32(m_vSizes[symbol_id].v[0]*scale)+m_pFonts[font_id].m_iFontSpace;
					if(m_pFonts[font_id].m_iStringMaxWidth < width_start)
					{
						m_pFonts[font_id].m_iStringMaxWidth = width_start;
					}
				}
				if(slash_flag)
				{
					if(Str(text[j]) == Str("n"))
					{
						slash_flag = false;
						height_start-=int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
						
						m_pFonts[font_id].m_iStringMaxHeight += int32(m_pFonts[font_id].m_iFontBettweenLineSpace+m_iFontHeight*scale);
						
						width_start = 0;
					}
				}
			}
		}
		
		render->AddVerexToGeometry(m_pFonts[font_id].m_iFontGeometryId,m_iInitSize[font_id]*6,vdata,0);
		render->AddTexCoordsToGeometry(m_pFonts[font_id].m_iFontGeometryId,m_iInitSize[font_id]*6,tdata,0);

		render->LoadToVertexBufferGeometry(m_pFonts[font_id].m_iFontGeometryId);

		SAFE_DELETE_ARRAY(vdata);
		SAFE_DELETE_ARRAY(tdata);
		DrawStaticText(render,font_id,x,y);
	}
	else
	{
		DrawStaticText(render,font_id,x,y);
	}
}

void CFont::DeInit()
{
}