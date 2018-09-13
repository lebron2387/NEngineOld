
#include "../Core/Engine.h"
#include "../Render/Render.h"
#include "Font.h"
#include "Image.h"
#include "TexturePacker.h"

class CGuiManager;
class CButton;
class CCheckRadioBox;
class CHSlider;
class CVSlider;
class CLabel;
class CEdit;
class CTextArea;
class CCursor;
class CWindow;
class CScrollArea;

class CGui
{
public:
	CGui();
	virtual ~CGui();

	enum GUIACTIVE
	{
		GUI_PASSIVE = 0,
		GUI_ACTIVE = 1,
		GUI_GRAB = 2,
	};

	virtual uint32 DrawFont(Float32 x,Float32 y,CRender *render,CFont *font){return 0;}
	virtual uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render){return 0;}
	virtual uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button){return 0;}
	__forceinline uint32 GetState(){return m_iState;}
	__forceinline void SetState(uint32 state){m_iState = state;}
	__forceinline uint32 GetType(){return m_iType;}
	__forceinline void SetType(uint32 type){m_iType = type;}
	__forceinline uint32 GetGroupId(){return m_iGroupId;}
	__forceinline void SetGroupId(uint32 group_id){m_iGroupId = group_id;}

	virtual void SetSliderPosition(Float32 value){}
	virtual Float32 GetSliderPosition(){return 0;}

	virtual void SetLabelText(Str text){}
	virtual Str GetLabelText(){return 0;}
	virtual void SetCursorType(uint32 type){}
	virtual uint32 GetCursorType(){return 0;}

	virtual int32 GetCursorColumn(){return 0;}
	virtual int32 GetCursorRow(){return 0;}
	virtual Float32 GetPrevStep(){return 0.0f;}
	virtual Float32 GetNextStep(){return 0.0f;}
	virtual Float32 GetCursorX(){return 0.0f;}
	virtual Float32 GetCursorY(){return 0.0f;}

	virtual void EnterChar(){}
	virtual void SpaceChar(){}
	virtual void AddChar(Str ch){}
	virtual void BackspaceChar(){}
	virtual void DeleteChar(){}
	virtual void CursorHome(){}
	virtual void CursorEnd(){}
	virtual void CursorLeft(){}
	virtual void CursorRight(){}
	virtual void CursorUp(){}
	virtual void CursorDown(){}
	virtual void SetWindowMovable(Bool movable){}
	virtual Bool GetWindowMovable(){return 0;}

	virtual Float32 GetSymbolWidth(uint32 ind){return 0.0f;}
	virtual uint32 GetFontSize(){return 0;}
	
	__forceinline void SetActive(uint32 value){m_iActive=value;}
	__forceinline uint32 GetActive(){return m_iActive;}
	
	__forceinline void SetColor(Vec4f color){m_vColor = color;}
	__forceinline Vec4f GetColor(){return m_vColor;}

	__forceinline void SetFontColor(Vec4f color){m_vFontColor = color;}
	__forceinline Vec4f GetFontColor(){return m_vFontColor;}

	__forceinline void SetPositionX(Float32 x){m_fPositionX = x;}
	__forceinline Float32 GetPositionX(){return m_fPositionX;}

	__forceinline void SetPositionY(Float32 y){m_fPositionY = y;}
	__forceinline Float32 GetPositionY(){return m_fPositionY;}

	__forceinline Float32 GetMaxInsideX(){return m_fMaxInsideX;}
	__forceinline void SetMaxInsideX(Float32 x){m_fMaxInsideX = x;}
	__forceinline Float32 GetMaxInsideY(){return m_fMaxInsideY;}
	__forceinline void SetMaxInsideY(Float32 y){m_fMaxInsideY = y;}

	__forceinline Float32 GetContentXPos(){return m_fContentXPos;}
	__forceinline void SetContentXPos(Float32 x){m_fContentXPos = x;}
	__forceinline Float32 GetContentYPos(){return m_fContentYPos;}
	__forceinline void SetContentYPos(Float32 y){m_fContentYPos = y;}

	__forceinline int32 GetTopBorderWidth(){return m_iTopBorderWidth;}
	__forceinline void SetTopBorderWidth(int32 w){m_iTopBorderWidth = w;}
	__forceinline int32 GetBottomBorderWidth(){return m_iBottomBorderWidth;}
	__forceinline void SetBottomBorderWidth(int32 w){m_iBottomBorderWidth = w;}
	__forceinline int32 GetLeftBorderWidth(){return m_iLeftBorderWidth;}
	__forceinline void SetLeftBorderWidth(int32 w){m_iLeftBorderWidth = w;}
	__forceinline int32 GetRightBorderWidth(){return m_iRightBorderWidth;}
	__forceinline void SetRightBorderWidth(int32 w){m_iRightBorderWidth = w;}

	__forceinline int32 GetWidth(){return int32(m_iWidth*m_fXScale);}
	__forceinline void SetWidth(int32 w){ m_fXScale = Float32(Float32(w)/Float32(m_iWidth));}
	__forceinline int32 GetHeight(){return int32(m_iHeight*m_fYScale);}
	__forceinline void SetHeight(int32 h){ m_fYScale = Float32(Float32(h)/Float32(m_iHeight));}

	__forceinline Float32 GetScaleX(){return m_fXScale;}
	__forceinline void SetScaleX(Float32 sx){ m_fXScale = sx;}
	__forceinline Float32 GetScaleY(){return m_fYScale;}
	__forceinline void SetScaleY(Float32 sy){ m_fYScale = sy;}

	__forceinline void SetVisible(Bool value){m_bVisible = value;}
	__forceinline Bool GetVisible(){ return m_bVisible;}

	__forceinline void SetMovedFromInit(Bool value){m_bMovedFromInit = value;}
	__forceinline Bool GetMovedFromInit(){ return m_bMovedFromInit;}

	__forceinline void SetVisibleArea(int32 x,int32 y,int32 w,int32 h){m_vVisibleArea = Vec4i(x,y,w,h);}
	__forceinline int32 GetVisibleAreaX(){return m_vVisibleArea.v[0] < 0 ? 0 : m_vVisibleArea.v[0];}
	__forceinline int32 GetVisibleAreaY(){return m_vVisibleArea.v[1] < 0 ? 0 : m_vVisibleArea.v[1];}
	__forceinline int32 GetVisibleAreaW(){return m_vVisibleArea.v[2] < 0 ? 0 : m_vVisibleArea.v[2];}
	__forceinline int32 GetVisibleAreaH(){return m_vVisibleArea.v[3] < 0 ? 0 : m_vVisibleArea.v[3];}

	Bool InVisibleArea();

	__forceinline void SetGeometryId(uint32 id){m_iGeometry = id;}
	__forceinline uint32 GetGeometryId(){return m_iGeometry;}

	__forceinline void SetBuildType(uint32 build_type){m_iBuildType = build_type;}
	__forceinline uint32 GetBuildType(){return m_iBuildType;}

	__forceinline void SetSkinId(uint32 skin_id){m_iSkinId = skin_id;}
	__forceinline uint32 GetSkinId(){return m_iSkinId;}

	void AddGuiQuadToGeometry(uint32 geometry_id,uint32 build_type,uint32 skin_id,uint32 default_skin_type,uint32 skin_type,int32 width,int32 height,CRender *render,CGuiManager *gui_manager);

	__forceinline CLabel *GetLabel(){if(m_pLabel == NULL){return NULL;}return m_pLabel;}
public:
	int32 m_iWidth;
	int32 m_iHeight;
protected:
		CLabel *m_pLabel;
private:
	Vec4i m_vVisibleArea;
	Bool m_bMovedFromInit;
	Bool m_bVisible;
	int32 m_iTopBorderWidth;
	int32 m_iBottomBorderWidth;
	int32 m_iLeftBorderWidth;
	int32 m_iRightBorderWidth;
	Vec4f m_vColor;
	Vec4f m_vFontColor;
	uint32 m_iState;
	uint32 m_iType;
	uint32 m_iBuildType;
	uint32 m_iSkinId;
	uint32 m_iActive;
	uint32 m_iGroupId;
	int32 m_iScreenWidth;
	int32 m_iScreenHeight;
	Float32 m_fPositionX;
	Float32 m_fPositionY;
	Float32 m_fMaxInsideX;
	Float32 m_fMaxInsideY;
	Float32 m_fContentXPos;
	Float32 m_fContentYPos;
	Float32 m_fXScale;
	Float32 m_fYScale;
	uint32 m_iGeometry;
protected:
	
};

class CGuiManager
{
public:
	CGuiManager();
	~CGuiManager();

	enum GUISKINTYPE
	{
		GUI_SKIN_BUTTON							= 0,
		GUI_SKIN_BUTTON_PRESSED					= 1,
		GUI_SKIN_BUTTON_OVER					= 2,
		GUI_SKIN_CHECK_BOX						= 3,
		GUI_SKIN_CHECK_BOX_OVER					= 4,
		GUI_SKIN_CHECK_BOX_CHECKED				= 5,
		GUI_SKIN_CHECK_BOX_CHECKED_OVER			= 6,
		GUI_SKIN_RADIO_BUTTON_BOX				= 7,
		GUI_SKIN_RADIO_BUTTON_BOX_OVER			= 8,
		GUI_SKIN_RADIO_BUTTON_BOX_CHECKED		= 9,
		GUI_SKIN_RADIO_BUTTON_BOX_CHECKED_OVER	= 10,
		GUI_SKIN_HSLIDER_BAR					= 11,
		GUI_SKIN_HSLIDER_BAR_PRESSED			= 12,
		GUI_SKIN_HSLIDER_BAR_OVER				= 13,
		GUI_SKIN_HSLIDER_BUTTON					= 14,
		GUI_SKIN_HSLIDER_BUTTON_PRESSED			= 15,
		GUI_SKIN_HSLIDER_BUTTON_OVER			= 16,
		GUI_SKIN_VSLIDER_BAR					= 17,
		GUI_SKIN_VSLIDER_BAR_PRESSED			= 18,
		GUI_SKIN_VSLIDER_BAR_OVER				= 19,
		GUI_SKIN_VSLIDER_BUTTON					= 20,
		GUI_SKIN_VSLIDER_BUTTON_PRESSED			= 21,
		GUI_SKIN_VSLIDER_BUTTON_OVER			= 22,
		GUI_SKIN_EDIT							= 23,
		GUI_SKIN_EDIT_PRESSED					= 24,
		GUI_SKIN_EDIT_OVER						= 25,
		GUI_SKIN_EDIT_READ_ONLY					= 26,
		GUI_SKIN_WINDOW							= 27,
		GUI_SKIN_SCROLL_AREA_BUTTON_UP			= 28,
		GUI_SKIN_SCROLL_AREA_BUTTON_UP_PRESSED	= 29,
		GUI_SKIN_SCROLL_AREA_BUTTON_UP_OVER		= 30,
		GUI_SKIN_SCROLL_AREA_BUTTON_DOWN		= 31,
		GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_PRESSED= 32,
		GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_OVER	= 33,
		GUI_SKIN_SCROLL_AREA_BUTTON_LEFT		= 34,
		GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_PRESSED= 35,
		GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_OVER	= 36,
		GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT		= 37,
		GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_PRESSED= 38,
		GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_OVER	= 39,
		GUI_SKIN_SCROLL_AREA_HBAR				= 40,
		GUI_SKIN_SCROLL_AREA_HBUTTON			= 41,
		GUI_SKIN_SCROLL_AREA_HBUTTON_PRESSED	= 42,
		GUI_SKIN_SCROLL_AREA_HBUTTON_OVER		= 43,
		GUI_SKIN_SCROLL_AREA_VBAR				= 44,
		GUI_SKIN_SCROLL_AREA_VBUTTON			= 45,
		GUI_SKIN_SCROLL_AREA_VBUTTON_PRESSED	= 46,
		GUI_SKIN_SCROLL_AREA_VBUTTON_OVER		= 47,
		GUI_SKIN_CURSOR							= 48,
		GUI_SKIN_CURSOR_EDIT					= 49,
	};

	enum GUISKINPART
	{
		GUI_SKIN_PART_SOLID = 0,
		GUI_SKIN_PART_CENTER = 1,
		GUI_SKIN_PART_TL_CORNER = 2,
		GUI_SKIN_PART_T_BORDER = 3,
		GUI_SKIN_PART_TR_CORNER = 4,
		GUI_SKIN_PART_R_BORDER = 5,
		GUI_SKIN_PART_BR_CORNER = 6,
		GUI_SKIN_PART_B_BORDER = 7,
		GUI_SKIN_PART_BL_CORNER = 8,
		GUI_SKIN_PART_L_BORDER = 9,
	};

	enum GUIBUILDTYPE
	{
		GUI_BUILD_TYPE_SOLID = 0,
		GUI_BUILD_TYPE_BORDER = 1,
	};

	enum GUITYPE
	{
		GUI_TYPE_BUTTON = 0,
		GUI_TYPE_CHECK_BOX = 1,
		GUI_TYPE_RADIO_BOX = 2,
		GUI_TYPE_LABEL = 3,
		GUI_TYPE_HSLIDER = 4,
		GUI_TYPE_VSLIDER = 5,
		GUI_TYPE_EDIT = 6,
		GUI_TYPE_CURSOR = 7,
		GUI_TYPE_WINDOW = 8,
		GUI_TYPE_SCROLL_AREA = 9,
		GUI_TYPE_TEXT_AREA = 10,
	};

	__forceinline uint32 GetGuiElementState(uint32 gui_element_id){return m_pGui[gui_element_id]->GetState();}
	__forceinline void SetGuiElementState(uint32 gui_element_id,uint32 state){m_pGui[gui_element_id]->SetState(state);}
	__forceinline uint32 GetGuiElementType(uint32 gui_element_id){return m_pGui[gui_element_id]->GetType();}
	__forceinline void SetGuiElementType(uint32 gui_element_id,uint32 type){m_pGui[gui_element_id]->SetType(type);}
	__forceinline uint32 GetGuiElementGroupId(uint32 gui_element_id){return m_pGui[gui_element_id]->GetGroupId();}
	__forceinline void SetGuiElementGroupId(uint32 gui_element_id,uint32 group_id){m_pGui[gui_element_id]->SetGroupId(group_id);}

	Str GetCharByIndex(uint32 index);

	struct TextureElementData
	{
		uint32 m_iSkinId;
		uint32 m_iGuiSkinElement;
		uint32 m_iGuiSkinElementPart;
		int32 image_id;
		int32 m_iWidth;
		int32 m_iHeight;
	};

	uint32 AddSkin();
	void CreateSkin(uint32 skin_id,CRender *render);
	void BindSkin(uint32 skin_id,CRender *render);
	void AddTexture(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Str filename);
	Float32 GetUCoords(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Float32 u_coord);
	Float32 GetVCoords(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Float32 v_coord);
	Bool GetTextureExist(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part);
	int32 GetTextureWidth(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part);
	int32 GetTextureHeight(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part);

	struct DrawElementsData
	{
		Float32 x;
		Float32 y;
		uint32 window;
	};

	void PreDraw();
	void DrawAll(uint32 skin_id,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	void SetDrawPosition(uint32 gui_element_id,Float32 x,Float32 y,uint32 window = -1);
	void DrawGuiElement(uint32 gui_element_id,Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render,uint32 window = -1,Bool draw_element = true,Bool draw_font = true);
	void CalculateStateGuiElement(uint32 gui_element_id,int32 mousex,int32 mousey,uint32 mouse_button);
	void CalculateStateForAll(uint32 gui_element_id,int32 mousex,int32 mousey,uint32 mouse_button);
	void CursorHomeForAll();
	void CursorEndForAll();
	void CursorLeftForAll();
	void CursorRightForAll();
	void CursorUpForAll();
	void CursorDownForAll();
	void EnterCharForAll();
	void SpaceCharForAll();
	void AddCharForAll(Str ch);
	void BackspaceCharForAll();
	void DeleteCharForAll();
	Bool IfEditableMoved();

	uint32 CreateButton(Bool is_switch,uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateCheckBox(uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateRadioButton(uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateSlider(Bool is_vertical,int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateEdit(Bool is_readonly,int32 width,int32 height,uint32 max_text_length,uint32 text_space,Str text,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateTextArea(Bool is_readonly,Bool wrap,int32 width,int32 height,uint32 font_size,uint32 hscroll_width,uint32 vscroll_width,uint32 max_text_length,uint32 text_space,uint32 bettween_line_space,Str text,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateCursor(uint32 type,int32 width,int32 height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateGuiWindow(Bool is_movable,int32 width,int32 height,uint32 title_height,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateScrollArea(int32 width,int32 height,int32 hscroll_bar_width,int32 vscroll_bar_width,int32 min_scroll_button_size,int32 hbuttons_step,int32 vbuttons_step,uint32 build_type,uint32 skin_id,CRender *render);
	uint32 CreateLabel(Bool is_static,uint32 edit_type,uint32 size,uint32 space,uint32 bettween_line_space,uint32 max_text_length,Str text,uint32 build_type,uint32 skin_id,CRender *render);
	

	uint32 CreateFont(CRender *render,Str font_symbols,Str ttf_name,uint32 font_height,uint32 font_space,uint32 texture_width,uint32 texture_height);

	void SetColorToAllGuiElements(Vec4f color);

	__forceinline uint32 GetGuiCount(){return m_iGuiCount;}
	CGui *GetGuiElement(uint32 gui_element_id);
	void RemoveGuiElement(uint32 gui_element_id);
private:
	int32 m_iGrabDeltaX;
	int32 m_iGrabDeltaY;
	Bool m_bGrabDeltaFlag;
	uint32 m_iGuiCount;
	CGui **m_pGui;
	uint32 m_iTextureElementsCount;
	TextureElementData *m_pTextureElements;
	DrawElementsData *m_pDrawElements;
	int32 *m_pDrawElementOrder;
	uint32 m_iDrawElementOrderIterator;
	CFont *m_pFont;
	uint32 m_iSkinsCount;
	uint32 *m_iSkinTextureId;
	CTexturePacker *m_pSkinTexturePacker;
};

class CButton : public CGui
{
public:
	CButton(uint32 width,uint32 height,uint32 type,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CButton();

	enum BUTTONTYPE
	{
		BUTTON_TYPE_NONE = 0,
		BUTTON_TYPE_BUTTON = 1,
		BUTTON_TYPE_SWITCH = 2,
	};

	enum BUTTONSTATE
	{
		BUTTON_STATE_NONE = 0,
		BUTTON_STATE_MOVED = 1,
		BUTTON_STATE_SWITCHED = 2,
		BUTTON_STATE_PRESSED = 3,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

private:
	uint32 m_iGuiType;
	uint32 m_iSwitchHolder;
protected:
};

class CCheckRadioBox : public CGui
{
public:
	CCheckRadioBox(uint32 width,uint32 height,uint32 type,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CCheckRadioBox();

	enum CHECKRADIOSTATE
	{
		CHECK_RADIO_STATE_NONE = 0,
		CHECK_RADIO_STATE_CHECKED = 1,
		CHECK_RADIO_STATE_UNCHECKED = 2,
	};

	enum CHECKRADIOTYPE
	{
		CHECK_RADIO_TYPE_NONE = 0,
		CHECK_RADIO_TYPE_CHECK = 1,
		CHECK_RADIO_TYPE_RADIO = 2,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

private:
	uint32 m_iGuiType;
	uint32 m_iSwitchHolder;
protected:
};

class CHSlider : public CGui
{
public:
	CHSlider(int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CHSlider();

	enum SLIDERSTATE
	{
		SLIDER_STATE_NONE = 0,
		SLIDER_STATE_PRESSED = 1,
		SLIDER_STATE_MOVED = 2,
	};

	enum SLIDERTYPE
	{
		SLIDER_TYPE_NONE = 0,
		SLIDER_TYPE_BAR = 1,
		SLIDER_TYPE_SLIDER = 2,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	__forceinline void SetSliderPosition(Float32 value)
	{
		m_fSliderPosition = value;
		m_iSliderButtonPosition = int32(m_fSliderPosition / m_fSliderStep);
	}
	__forceinline Float32 GetSliderPosition(){return m_fSliderPosition;}

private:
	uint32 m_iSwitchHolder;
	int32 m_iMinPosition;
	int32 m_iMaxPosition;
	int32 m_iSliderButtonPosition;
	Float32 m_fSliderPosition;
	Float32 m_fSliderStep;
	int32 m_iBarWidth;
	int32 m_iBarHeight;
	int32 m_iButtonWidth;
	int32 m_iButtonHeight;
	uint32 m_iGuiType;
protected:
};

class CVSlider : public CGui
{
public:
	CVSlider(int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CVSlider();

	enum SLIDERSTATE
	{
		SLIDER_STATE_NONE = 0,
		SLIDER_STATE_PRESSED = 1,
		SLIDER_STATE_MOVED = 2,
	};

	enum SLIDERTYPE
	{
		SLIDER_TYPE_NONE = 0,
		SLIDER_TYPE_BAR = 1,
		SLIDER_TYPE_SLIDER = 2,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	__forceinline void SetSliderPosition(Float32 value){m_fSliderPosition = value;}
	__forceinline Float32 GetSliderPosition(){return m_fSliderPosition;}

private:
	uint32 m_iSwitchHolder;
	int32 m_iMinPosition;
	int32 m_iMaxPosition;
	int32 m_iSliderButtonPosition;
	Float32 m_fSliderPosition;
	Float32 m_fSliderStep;
	int32 m_iBarWidth;
	int32 m_iBarHeight;
	int32 m_iButtonWidth;
	int32 m_iButtonHeight;
	uint32 m_iGuiType;
protected:
};

class CLabel : public CGui
{
public:
	CLabel(uint32 type,uint32 edit_type,uint32 size,uint32 space,uint32 bettween_line_space,uint32 max_text_length,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager);
	~CLabel();

	enum LABELTYPE
	{
		LABEL_TYPE_NONE = 0,
		LABEL_TYPE_STATIC = 1,
		LABEL_TYPE_DYNAMIC = 2,
	};

	enum LABELSTATE
	{
		LABEL_STATE_NONE = 0,
		LABEL_STATE_MOVED = 1,
		LABEL_STATE_EDIT = 2,
	};

	enum LABELEDITTYPE
	{
		LABEL_EDIT_TYPE_NONE = 0,
		LABEL_EDIT_TYPE_EDITABLE = 1,
		LABEL_EDIT_TYPE_SELECTABLE = 2,
		LABEL_EDIT_TYPE_EDITABLE_SELECTABLE = 3,
	};

	uint32 DrawFont(Float32 x,Float32 y,CRender *render,CFont *font);
	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	__forceinline void SetLabelText(Str text)
	{
		if(m_sText != text)
		{
			if(m_iMaxTextLength < text.Size())
			{
				m_sText = text.SubStr(0,m_iMaxTextLength);
			}
			else
			{
				m_sText = text;
			}
			m_iTextChanged = 1;
		}
	}
	__forceinline Str GetLabelText(){return m_sText;}

	__forceinline int32 GetCursorColumn(){return m_iCursorColumn+1;}
	__forceinline int32 GetCursorRow(){return m_iCursorRow;}

	__forceinline uint32 GetFontSize(){return m_iFontSize;}

	__forceinline Float32 GetSymbolWidth(uint32 ind)
	{
		if(ind >= m_sText.Size())return 0.0f;
		return m_fSymbolSizes[ind];
	}

	__forceinline Float32 GetPrevStep(){return m_fPrevStep;}
	__forceinline Float32 GetNextStep(){return m_fNextStep;}
	__forceinline Float32 GetCursorX(){return m_fCursorX;}
	__forceinline Float32 GetCursorY(){return m_fCursorY;}

	void AddChar(Str ch);
	void CursorHome();
	void CursorEnd();
	void CursorLeft();
	void CursorRight();
	void CursorUp();
	void CursorDown();

private:
	Str m_sAddChar;
	uint32 m_iMaxTextLength;
	uint32 m_iTextSpace;
	int32 m_iFontSize;
	Float32 m_fFontScale;
	uint32 m_iBettweenLineSpace;
	uint32 m_iFont;
	uint32 m_iGuiType;
	uint32 m_iEditType;
	uint32 m_iTextChanged;
	Str m_sText;
	Float32 m_fNextStep;
	Float32 m_fPrevStep;
	Float32 m_fCursorX;
	Float32 m_fCursorY;
	int32  m_iCursorColumn;
	int32  m_iCursorRow;
	Float32 *m_fSymbolSizes;
protected:
};

class CEdit : public CGui
{
public:
	CEdit(uint32 type,int32 width,int32 height,uint32 max_text_length,uint32 text_space,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager);
	~CEdit();

	enum EDITTYPE
	{
		EDIT_TYPE_NONE = 0,
		EDIT_TYPE_READWRITE = 1,
		EDIT_TYPE_READONLY = 2,
	};

	enum EDITSTATE
	{
		EDIT_STATE_NONE = 0,
		EDIT_STATE_MOVED = 1,
		EDIT_STATE_EDIT = 2,
	};

	uint32 DrawFont(Float32 x,Float32 y,CRender *render,CFont *font);
	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	void EnterChar();
	void SpaceChar();
	void AddChar(Str ch);
	void BackspaceChar();
	void DeleteChar();

	void CursorHome();
	void CursorEnd();
	void CursorLeft();
	void CursorRight();

	__forceinline void SetEditText(Str text)
	{
		if(m_sText != text)
		{
			if(m_iMaxTextLength < text.Size())
			{
				m_sText = text.SubStr(0,m_iMaxTextLength);
			}
			else
			{
				m_sText = text;
			}
			m_iTextChanged = 1;
		}
	}
	__forceinline Str GetEditText(){return m_sText;}


private:
	Bool m_bInsertFlag;
	int32 m_iXOffset;
	int32 m_iYOffset;
	int32 m_iWidthOffset;
	int32 m_iHeightOffset;
	Float32 m_fFontXOffset;
	Float32 m_fFontScale;
	uint32 m_iMaxTextLength;
	uint32 m_iGuiType;
	Str m_sText;
	uint32 m_iTextSpace;
	uint32 m_iTextChanged;
protected:
};

class CTextArea : public CGui
{
public:
	CTextArea(uint32 type,Bool wrap,int32 width,int32 height,uint32 font_size,uint32 hscroll_width,uint32 vscroll_width,uint32 max_text_length,uint32 text_space,uint32 bettween_line_space,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager);
	~CTextArea();

	enum TEXTAREATYPE
	{
		TEXTAREA_TYPE_NONE = 0,
		TEXTAREA_TYPE_READWRITE = 1,
		TEXTAREA_TYPE_READONLY = 2,
	};

	enum TEXTAREASTATE
	{
		TEXTAREA_STATE_NONE = 0,
		TEXTAREA_STATE_MOVED = 1,
		TEXTAREA_STATE_EDIT = 2,
	};

	uint32 DrawFont(Float32 x,Float32 y,CRender *render,CFont *font);
	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	void EnterChar();
	void SpaceChar();
	void AddChar(Str ch);
	void BackspaceChar();
	void DeleteChar();
	
	void CursorLeft();
	void CursorRight();
	void CursorUp();
	void CursorDown();
	void CursorHome();
	void CursorEnd();

	__forceinline void SetTextAreaText(Str text)
	{
		if(m_sText != text)
		{
			if(m_iMaxTextLength < text.Size())
			{
				m_sText = text.SubStr(0,m_iMaxTextLength);
			}
			else
			{
				m_sText = text;
			}
			m_iTextChanged = 1;
		}
	}
	__forceinline Str GetTextAreaText(){return m_sText;}


private:
	Bool m_bWrap;
	Bool m_bInsertFlag;
	Bool m_bEndLineFlag;
	int32 m_iXOffset;
	int32 m_iYOffset;
	int32 m_iWidthOffset;
	int32 m_iHeightOffset;
	Float32 m_fFontXOffset;
	Float32 m_fFontYOffset;
	uint32 m_iHScrollWidth;
	uint32 m_iVScrollWidth;
	Float32 m_fFontScale;
	uint32 m_iMaxTextLength;
	uint32 m_iGuiType;
	Str m_sText;
	uint32 m_iTextSpace;
	uint32 m_iBettweenLineSpace;
	uint32 m_iTextChanged;

	CScrollArea *m_pScrollArea;
protected:
};


class CCursor : public CGui
{
public:
	CCursor(uint32 type,int32 width,int32 height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CCursor();

	enum CURSORTYPE
	{
		CURSOR_TYPE_NONE = 0,
		CURSOR_TYPE_ARROW = 1,
		CURSOR_TYPE_EDIT = 2,
	};

	enum CURSORSTATE
	{
		CURSOR_STATE_NONE = 0,
	};

	__forceinline void SetCursorType(uint32 type){m_iGuiType = type;}
	__forceinline uint32 GetCursorType(){return m_iGuiType;}

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

private:
	uint32 m_iGuiType;
protected:
};

class CWindow : public CGui
{
public:
	CWindow(int32 width,int32 height,int32 title_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CWindow();

	enum WINDOWTYPE
	{
		WINDOW_TYPE_NONE = 0,
	};

	enum WINDOWSTATE
	{
		WINDOW_STATE_NONE = 0,
		WINDOW_STATE_MOVED = 1,
		WINDOW_STATE_TITLE_MOVED = 2,
		WINDOW_STATE_STRETCH_LEFT = 3,
		WINDOW_STATE_STRETCH_RIGHT = 4,
		WINDOW_STATE_STRETCH_TOP = 5,
		WINDOW_STATE_STRETCH_BOTTOM = 6,
		WINDOW_STATE_STRETCH_LEFT_TOP = 7,
		WINDOW_STATE_STRETCH_LEFT_BOTTOM = 8,
		WINDOW_STATE_STRETCH_RIGHT_TOP = 9,
		WINDOW_STATE_STRETCH_RIGHT_BOTTOM = 10,
		WINDOW_STATE_PRESSED = 11,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

	__forceinline void SetWindowMovable(Bool movable){m_bMovable = movable;}
	__forceinline Bool GetWindowMovable(){return m_bMovable;}

private:

	uint32 m_iGuiType;
	Bool m_bMovable;
	int32 m_iTitleHeight;
	Bool m_bGrabFlag;
	int32 m_iDeltaX;
	int32 m_iDeltaY;
protected:
};

class CScrollArea : public CGui
{
public:
	CScrollArea(int32 width,int32 height,int32 hscroll_bar_width,int32 vscroll_bar_width,int32 min_scroll_button_size,int32 hbuttons_step,int32 vbuttons_step,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager);
	~CScrollArea();

	enum SCROLLAREATYPE
	{
		SCROLLAREA_TYPE_NONE = 0,
	};

	enum SCROLLAREASTATE
	{
		SCROLLAREA_STATE_NONE			= 0,
		SCROLLAREA_STATE_HBUTTON_MOVED	= 1,
		SCROLLAREA_STATE_HBUTTON_PRESSED= 2,
		SCROLLAREA_STATE_VBUTTON_MOVED	= 3,
		SCROLLAREA_STATE_VBUTTON_PRESSED= 4,
		SCROLLAREA_STATE_UP_MOVED		= 5,
		SCROLLAREA_STATE_UP_PRESSED		= 6,
		SCROLLAREA_STATE_DOWN_MOVED		= 7,
		SCROLLAREA_STATE_DOWN_PRESSED	= 8,
		SCROLLAREA_STATE_LEFT_MOVED		= 9,
		SCROLLAREA_STATE_LEFT_PRESSED	= 10,
		SCROLLAREA_STATE_RIGHT_MOVED	= 11,
		SCROLLAREA_STATE_RIGHT_PRESSED	= 12,
	};

	uint32 Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render);
	uint32 CalculateState(int32 mousex,int32 mousey,uint32 mouse_button);

private:
	Bool m_bEnableHScroll;
	Bool m_bEnableVScroll;

	int32 m_iHButtonsStep;
	int32 m_iVButtonsStep;

	int32 m_iMinScrollButtonSize;

	int32 m_iHScrollBarWidth;
	int32 m_iVScrollBarWidth;

	uint32 m_iHSwitchHolder;
	uint32 m_iVSwitchHolder;

	Float32 m_fHScrollButtonPosition;
	Float32 m_fHScrollStep;

	Float32 m_fVScrollButtonPosition;
	Float32 m_fVScrollStep;

	Float32 m_fDeltaX;
	Float32 m_fDeltaY;
	int32 m_iHBarWidth;
	int32 m_iVBarHeight;
	Float32 m_fHButtonScale;
	Float32 m_fVButtonScale;
	uint32 m_iGuiType;
protected:
};

//CProgressBar
//CSpin

//CList
//CDropList
//CImageButton
//CTreeList
//CTab
//CMenu
//ContextMenu