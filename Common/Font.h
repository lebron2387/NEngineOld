
#include "../Core/Engine.h"
#include "../Render/Render.h"

#ifdef _WIN32
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#endif

class CFont
{
public:

	CFont();
	~CFont();

	uint32 CreateTextureForSymbols(CRender *render,Str text,Str ttf_name,uint32 font_height,uint32 font_space,uint32 texture_width,uint32 texture_height);
	uint32 ChangeFont(CRender *render,Str text,Str ttf_name,uint32 font_height,uint32 font_space);
	uint32 InitStaticText(CRender *render,Str text,uint32 size,uint32 space,uint32 bettween_line_space);
	uint32 InitDynamicText(CRender *render,Str text,uint32 size,uint32 space,uint32 bettween_line_space);
	void DeInit();
	void DrawStaticText(CRender *render,uint32 font_id,int32 x,int32 y);
	void DrawDynamicText(CRender *render,uint32 font_id,int32 x,int32 y,Str text);

	void CursorLeft();
	void CursorRight();
	void CursorUp();
	void CursorDown();
	void CursorDelete();
	void CursorBackspace();

	__forceinline Float32 GetSymbolWidth(Char symbol){return Str(symbol) == Str(" ") ? (m_iFontHeight*0.5f) : m_vSizes[uint32(symbol)].x;}
	__forceinline Float32 GetSymbolHeight(Char symbol){return Str(symbol) == Str(" ") ? (m_iFontHeight*0.5f) : m_vSizes[uint32(symbol)].y;}
	__forceinline uint32 GetFontHeight(){return m_iFontHeight;}

	__forceinline uint32 GetStringWidth(uint32 font_id){return m_pFonts[font_id].m_iStringMaxWidth;}
	__forceinline uint32 GetStringHeight(uint32 font_id){return m_pFonts[font_id].m_iStringMaxHeight;}

	__forceinline uint32 GetFontTexture(){return m_iFontTexture;}

	__forceinline Str GetCharByIndex(uint32 index)
	{
		for(uint32 i=0;i<m_sSymbols.Size();i++)
		{
			if(m_sSymbols[i] == index)
			{
				return Str(m_sSymbols[i]); 
			}
		}
		return Str("");
	}

	__forceinline void SetFontSpace(uint32 font_id,uint32 space)
	{
		if(font_id < m_iFontsCount){m_pFonts[font_id].m_iFontSpace = space;m_pFonts[font_id].m_iFontChanged=1;}
	}
	__forceinline uint32 GetFontSpace(uint32 font_id)
	{
		if(font_id < m_iFontsCount)
		{
			return m_pFonts[font_id].m_iFontSpace;
		}
		return 0;
	}
	__forceinline void SetFontSize(uint32 font_id,uint32 size)
	{
		if(font_id < m_iFontsCount){m_pFonts[font_id].m_iFontSize = size;m_pFonts[font_id].m_iFontChanged=1;}
	}
	__forceinline uint32 GetFontSize(uint32 font_id)
	{
		if(font_id < m_iFontsCount){return m_pFonts[font_id].m_iFontSize;}
		return 0;
	}

	__forceinline void SetTextChanged(uint32 font_id,uint32 changed)
	{
		if(font_id < m_iFontsCount){m_pFonts[font_id].m_iFontChanged = changed;}
	}
	__forceinline uint32 GetTextChanged(uint32 font_id)
	{
		if(font_id < m_iFontsCount){return m_pFonts[font_id].m_iFontChanged;}
		return 0;
	}

	struct FontData
	{
		FontData()
		{
			m_iFontGeometryId = 0;
			m_iFontSize = 0;
			m_iFontSpace = 0;
			m_iFontBettweenLineSpace = 0;
			m_iFontChanged = 0;
			m_iStringMaxHeight = 0;
			m_iStringMaxWidth = 0;
		}
		uint32 m_iFontGeometryId;
		uint32 m_iFontSize;
		uint32 m_iFontSpace;
		uint32 m_iFontBettweenLineSpace;
		uint32 m_iFontChanged;
		int32 m_iStringMaxHeight;
		int32 m_iStringMaxWidth;
	};

private:

	uint32 m_iFontTexture;

	uint32 m_iScreenWidth;
	uint32 m_iScreenHeight;

	uint32 m_iTextureWidth;
	uint32 m_iTextureHeight;

	uint32 m_iTexureId;
	uint32 m_iSpace;
	uint32 m_iFontHeight;
	uint32 m_iHeightStep;
	uint32 m_iWidthStep;
	uint32 *m_iInitSize;
	Float32 m_fDeltaXStep;
	Float32 m_fDeltaYStep;
	Vec2f *m_vTexCoords;
	Vec2f *m_vSizes;
	FontData *m_pFonts;
	uint32 m_iFontsCount;
	Str m_sSymbols;
};

__forceinline int next_p2(int a)
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}