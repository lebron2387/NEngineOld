#include "Gui.h"



CGuiManager::CGuiManager()
{
	m_iGuiCount = 0;
	m_iTextureElementsCount = 0;
	m_iDrawElementOrderIterator = 0;
	m_iSkinsCount = 0;
	m_pFont = new CFont();
	m_pGui = new CGui*[1];
	m_pTextureElements = new TextureElementData[1];
	m_pDrawElements = new DrawElementsData[1];
	m_pDrawElementOrder = new int32[1];
	m_iSkinTextureId = new uint32[1];
	m_pSkinTexturePacker = new CTexturePacker();
}

CGuiManager::~CGuiManager()
{
	SAFE_DELETE(m_pFont);
	for(uint32 i=0;i<m_iGuiCount;i++)
	{
		SAFE_DELETE(m_pGui[i]);
	}
	SAFE_DELETE_ARRAY(m_pGui);
	SAFE_DELETE_ARRAY(m_pTextureElements);
	SAFE_DELETE_ARRAY(m_pDrawElements);
	SAFE_DELETE_ARRAY(m_pDrawElementOrder);
	SAFE_DELETE_ARRAY(m_iSkinTextureId);
	SAFE_DELETE(m_pSkinTexturePacker);
}

CGui *CGuiManager::GetGuiElement(uint32 gui_element_id)
{
	if(gui_element_id >= m_iGuiCount)return NULL;
	return m_pGui[gui_element_id];
}

void CGuiManager::RemoveGuiElement(uint32 gui_element_id)
{
	if(gui_element_id >= m_iGuiCount)return;
	SAFE_DELETE(m_pGui[gui_element_id]);
}

void CGuiManager::PreDraw()
{
	Bool grab_flag = false;

	m_iDrawElementOrderIterator = 0;
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pDrawElementOrder[i] = -1;
	}

	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pGui[i]->GetActive() == CGui::GUIACTIVE::GUI_GRAB)
		{
			grab_flag = true;
			for(uint32 j=0;j<GetGuiCount();j++)
			{
				if(i != j)
				{
					m_pGui[j]->SetActive(CGui::GUIACTIVE::GUI_PASSIVE);
				}
			}
		}
	}

	if(!grab_flag)
	{
		m_iGrabDeltaX = 0;
		m_iGrabDeltaY = 0;
		m_bGrabDeltaFlag = true;
		for(uint32 i=0;i<GetGuiCount();i++)
		{
			m_pGui[i]->SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
		}
	}
}

void CGuiManager::DrawGuiElement(uint32 gui_element_id,Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render,uint32 window,Bool draw_element,Bool draw_font)
{
	int32 windowx = 0;
	int32 windowy = 0;

	int32 scissorsx = 0;
	int32 scissorsy = 0;
	int32 scissorsw = 0;
	int32 scissorsh = 0;

	int32 windowx_border = 0;
	int32 windowy_border = 0;

	
	if(window != -1 && (m_pGui[window]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_SCROLL_AREA || m_pGui[window]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_WINDOW))
	{
		windowx = (int32)m_pGui[window]->GetPositionX();
		windowy = (int32)m_pGui[window]->GetPositionY();

		windowx_border = (int32)m_pGui[window]->GetLeftBorderWidth();
		windowy_border = (int32)m_pGui[window]->GetTopBorderWidth();
		
		if(windowx < 0)
		{
			scissorsx = 0;
			scissorsw = m_pGui[window]->GetWidth()+windowx;
		}
		else
		{
			scissorsx = windowx;
			scissorsw = m_pGui[window]->GetWidth();
		}

		
		if(windowy < 0)
		{
			scissorsy = 0;
			scissorsh = m_pGui[window]->GetHeight()+windowy;
		}
		else
		{
			scissorsy = windowy;
			scissorsh = m_pGui[window]->GetHeight();
		}

		if(m_pGui[window]->GetVisibleAreaX() > scissorsx+m_pGui[window]->GetLeftBorderWidth())
		{
			scissorsx = m_pGui[window]->GetVisibleAreaX()-m_pGui[window]->GetLeftBorderWidth();
			scissorsw = m_pGui[window]->GetWidth() - (scissorsx - windowx);
		}
		else if(scissorsx + scissorsw - m_pGui[window]->GetRightBorderWidth() > m_pGui[window]->GetVisibleAreaX()+m_pGui[window]->GetVisibleAreaW())
		{
			scissorsw -= (scissorsx + scissorsw  - m_pGui[window]->GetRightBorderWidth()) - (m_pGui[window]->GetVisibleAreaX()+m_pGui[window]->GetVisibleAreaW());
		}

		if(m_pGui[window]->GetVisibleAreaY() > scissorsy+m_pGui[window]->GetTopBorderWidth())
		{
			scissorsy = m_pGui[window]->GetVisibleAreaY()-m_pGui[window]->GetTopBorderWidth();
			scissorsh = m_pGui[window]->GetHeight() - (scissorsy - windowy);
		}
		else if(scissorsy + scissorsh - m_pGui[window]->GetBottomBorderWidth() > m_pGui[window]->GetVisibleAreaY()+m_pGui[window]->GetVisibleAreaH())
		{
			scissorsh -= (scissorsy + scissorsh - m_pGui[window]->GetBottomBorderWidth()) - (m_pGui[window]->GetVisibleAreaY()+m_pGui[window]->GetVisibleAreaH());

		}

		if(scissorsw <= 0 || scissorsh <= 0)
		{
			scissorsx = 0;
			scissorsw = 1;
			scissorsy = 0;
			scissorsh = 1;
		}
		
		render->OnOffScissorsTest(0);
		render->OnOffScissorsTest(1);
		if(scissorsw-m_pGui[window]->GetRightBorderWidth() < 1 || scissorsh-m_pGui[window]->GetBottomBorderWidth() < 1)
		{
			render->SetScissors(
				scissorsx+(windowx >= 0 ? m_pGui[window]->GetLeftBorderWidth() : (m_pGui[window]->GetLeftBorderWidth()+windowx > 0 ? m_pGui[window]->GetLeftBorderWidth()+windowx : 0)),
				scissorsy+(windowy >= 0 ? m_pGui[window]->GetTopBorderWidth() : (m_pGui[window]->GetTopBorderWidth()+windowy > 0 ? m_pGui[window]->GetTopBorderWidth()+windowy : 0)),
				1,1);
		}
		else
		{
			render->SetScissors(
				scissorsx+(windowx >= 0 ? m_pGui[window]->GetLeftBorderWidth() : (m_pGui[window]->GetLeftBorderWidth()+windowx > 0 ? m_pGui[window]->GetLeftBorderWidth()+windowx : 0)),
				scissorsy+(windowy >= 0 ? m_pGui[window]->GetTopBorderWidth() : (m_pGui[window]->GetTopBorderWidth()+windowy > 0 ? m_pGui[window]->GetTopBorderWidth()+windowy : 0)),
				scissorsw-(m_pGui[window]->GetLeftBorderWidth()+m_pGui[window]->GetRightBorderWidth()) + (windowx < 0 && m_pGui[window]->GetLeftBorderWidth() > math_abs(windowx) ? math_abs(windowx) : (windowx < 0 ? m_pGui[window]->GetRightBorderWidth() : 0)),
				scissorsh-(m_pGui[window]->GetTopBorderWidth()+m_pGui[window]->GetBottomBorderWidth()) + (windowy < 0 && m_pGui[window]->GetTopBorderWidth() > math_abs(windowy) ? math_abs(windowy) : (windowy < 0 ? m_pGui[window]->GetBottomBorderWidth() : 0)));
		}
		
		m_pGui[gui_element_id]->SetVisibleArea(
			scissorsx+(windowx >= 0 ? m_pGui[window]->GetLeftBorderWidth() : (m_pGui[window]->GetLeftBorderWidth()+windowx > 0 ? m_pGui[window]->GetLeftBorderWidth()+windowx : 0)),
			scissorsy+(windowy >= 0 ? m_pGui[window]->GetTopBorderWidth() : (m_pGui[window]->GetTopBorderWidth()+windowy > 0 ? m_pGui[window]->GetTopBorderWidth()+windowy : 0)),
			scissorsw-(m_pGui[window]->GetLeftBorderWidth()+m_pGui[window]->GetRightBorderWidth()) + (windowx < 0 && m_pGui[window]->GetLeftBorderWidth() > math_abs(windowx) ? math_abs(windowx) : (windowx < 0 ? m_pGui[window]->GetRightBorderWidth() : 0)),
			scissorsh-(m_pGui[window]->GetTopBorderWidth()+m_pGui[window]->GetBottomBorderWidth()) + (windowy < 0 && m_pGui[window]->GetTopBorderWidth() > math_abs(windowy) ? math_abs(windowy) : (windowy < 0 ? m_pGui[window]->GetBottomBorderWidth() : 0)));

		if(m_pGui[window]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_SCROLL_AREA)
		{
			x-=m_pGui[window]->GetContentXPos();
			y-=m_pGui[window]->GetContentYPos();
			
			if(m_pGui[window]->GetMaxInsideX() < x+m_pGui[window]->GetContentXPos()+m_pGui[gui_element_id]->GetWidth())
			{
				m_pGui[window]->SetMaxInsideX(x+m_pGui[window]->GetContentXPos()+m_pGui[gui_element_id]->GetWidth());
			}
			if(m_pGui[window]->GetMaxInsideY() < y+m_pGui[window]->GetContentYPos()+m_pGui[gui_element_id]->GetHeight())
			{
				m_pGui[window]->SetMaxInsideY(y+m_pGui[window]->GetContentYPos()+m_pGui[gui_element_id]->GetHeight());
			}
		}

		if(m_bGrabDeltaFlag && m_pGui[gui_element_id]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_WINDOW && m_pGui[window]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_WINDOW && m_pGui[window]->GetActive() == CGui::GUIACTIVE::GUI_GRAB)
		{
			m_bGrabDeltaFlag = false;
			m_iGrabDeltaX = (int32)m_pGui[gui_element_id]->GetPositionX() - windowx;
			m_iGrabDeltaY = (int32)m_pGui[gui_element_id]->GetPositionY() - windowy;
		}

		if(m_pGui[gui_element_id]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_WINDOW)
		{
			if(m_iGrabDeltaX != 0 || m_iGrabDeltaX != 0)
			{
				m_pGui[gui_element_id]->SetPositionX(Float32(windowx + m_iGrabDeltaX));
				m_pGui[gui_element_id]->SetPositionY(Float32(windowy + m_iGrabDeltaY));
			}
		}
	}
	else
	{
		m_pGui[gui_element_id]->SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
		render->OnOffScissorsTest(0);
	}

	if(draw_element)m_pGui[gui_element_id]->Draw(windowx + windowx_border + x,windowy + windowy_border + y,mousex,mousey,mouse_button,render);
	if(draw_font)m_pGui[gui_element_id]->DrawFont(windowx + windowx_border + x,windowy + windowy_border + y,render,m_pFont);


	if(m_pGui[gui_element_id]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_SCROLL_AREA)
	{
		m_pGui[gui_element_id]->SetMaxInsideX(0.0f);
		m_pGui[gui_element_id]->SetMaxInsideY(0.0f);
	}
}

void CGuiManager::SetDrawPosition(uint32 gui_element_id,Float32 x,Float32 y,uint32 window)
{
	m_pDrawElementOrder[m_iDrawElementOrderIterator] = gui_element_id;
	m_iDrawElementOrderIterator++;
	m_pDrawElements[gui_element_id].x = x;
	m_pDrawElements[gui_element_id].y = y;
	m_pDrawElements[gui_element_id].window = window;
}

void CGuiManager::DrawAll(uint32 skin_id,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pDrawElementOrder[i] >=0)
		{
			if(m_pGui[m_pDrawElementOrder[i]]->GetType() != GUITYPE::GUI_TYPE_CURSOR)
			{
				BindSkin(skin_id,render);
				DrawGuiElement(m_pDrawElementOrder[i],m_pDrawElements[m_pDrawElementOrder[i]].x,m_pDrawElements[m_pDrawElementOrder[i]].y,mousex,mousey,mouse_button,render,m_pDrawElements[m_pDrawElementOrder[i]].window,true,false);
				render->BindTexture(m_pFont->GetFontTexture());
				DrawGuiElement(m_pDrawElementOrder[i],m_pDrawElements[m_pDrawElementOrder[i]].x,m_pDrawElements[m_pDrawElementOrder[i]].y,mousex,mousey,mouse_button,render,m_pDrawElements[m_pDrawElementOrder[i]].window,false,true);
			}
		}
	}
	/*
	render->BindTexture(m_pFont->GetFontTexture());
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pDrawElementOrder[i] >=0)
		{
			if(m_pGui[m_pDrawElementOrder[i]]->GetType() != GUITYPE::GUI_TYPE_CURSOR)
			{
				DrawGuiElement(m_pDrawElementOrder[i],m_pDrawElements[m_pDrawElementOrder[i]].x,m_pDrawElements[m_pDrawElementOrder[i]].y,mousex,mousey,mouse_button,render,m_pDrawElements[m_pDrawElementOrder[i]].window,false,true);
			}
		}
	}
	*/
	BindSkin(skin_id,render);
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pGui[i]->GetType() == GUITYPE::GUI_TYPE_CURSOR)
		{
			if(IfEditableMoved())
			{
				m_pGui[i]->SetCursorType(CCursor::CURSORTYPE::CURSOR_TYPE_EDIT);
			}
			else
			{
				m_pGui[i]->SetCursorType(CCursor::CURSORTYPE::CURSOR_TYPE_ARROW);
			}
			DrawGuiElement(i,0.0f,0.0f,mousex,mousey,mouse_button,render,-1,true,false);
		}
	}
}

void CGuiManager::CalculateStateGuiElement(uint32 gui_element_id,int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 group_id = 0;
	group_id = m_pGui[gui_element_id]->CalculateState(mousex,mousey,mouse_button);
	if( group_id > 0 && m_pGui[gui_element_id]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_RADIO_BOX)
	{
		for(uint32 i=0;i<GetGuiCount();i++)
		{
			if(gui_element_id != i && m_pGui[i]->GetGroupId() == group_id)
			{
				if(m_pGui[i]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_RADIO_BOX)
				{
					m_pGui[i]->SetState(CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_UNCHECKED);
				}
			}
		}
	}
}

void CGuiManager::CalculateStateForAll(uint32 gui_element_id,int32 mousex,int32 mousey,uint32 mouse_button)
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pDrawElementOrder[i] >=0)
		{
			if(m_pDrawElements[m_pDrawElementOrder[i]].window == -1)
			{
				CalculateStateGuiElement(m_pDrawElementOrder[i],mousex,mousey,mouse_button);
			}
			else
			{
				if(m_pGui[m_pDrawElements[m_pDrawElementOrder[i]].window]->CalculateState(mousex,mousey,mouse_button))
				{
					CalculateStateGuiElement(m_pDrawElementOrder[i],mousex,mousey,mouse_button);
				}
			}
			if(m_pGui[m_pDrawElementOrder[i]]->GetLabel() != NULL)
			{
				CalculateStateGuiElement(m_pDrawElementOrder[i],mousex,mousey,mouse_button);
			}
		}
	}
}

void CGuiManager::CursorHomeForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorHome();
	}
}

void CGuiManager::CursorEndForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorEnd();
	}
}

void CGuiManager::CursorLeftForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorLeft();
	}
}

void CGuiManager::CursorRightForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorRight();
	}
}

void CGuiManager::CursorUpForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorUp();
	}
}

void CGuiManager::CursorDownForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->CursorDown();
	}
}

void CGuiManager::EnterCharForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->EnterChar();
	}
}

void CGuiManager::SpaceCharForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->SpaceChar();
	}
}

void CGuiManager::AddCharForAll(Str ch)
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->AddChar(ch);
	}
}

void CGuiManager::BackspaceCharForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->BackspaceChar();
	}
}

void CGuiManager::DeleteCharForAll()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		m_pGui[i]->DeleteChar();
	}
}

Bool CGuiManager::IfEditableMoved()
{
	for(uint32 i=0;i<GetGuiCount();i++)
	{
		if(m_pGui[i]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_EDIT)
		{
			if(m_pGui[i]->GetState() == CEdit::EDITSTATE::EDIT_STATE_MOVED)
			{
				return true;
			}
		}
		else if(m_pGui[i]->GetType() == CGuiManager::GUITYPE::GUI_TYPE_TEXT_AREA)
		{
			if(m_pGui[i]->GetState() == CTextArea::TEXTAREASTATE::TEXTAREA_STATE_MOVED)
			{
				return true;
			}
		}
	}
	return false;
}

void CGuiManager::AddTexture(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Str filename)
{
	++m_iTextureElementsCount;
	m_pTextureElements = (TextureElementData*)ReAlloc(m_pTextureElements,m_iTextureElementsCount * sizeof(TextureElementData));

	m_pTextureElements[m_iTextureElementsCount-1].m_iSkinId = skin_id;
	m_pTextureElements[m_iTextureElementsCount-1].m_iGuiSkinElement = gui_skin_element;
	m_pTextureElements[m_iTextureElementsCount-1].m_iGuiSkinElementPart = gui_skin_element_part;
	m_pTextureElements[m_iTextureElementsCount-1].image_id = m_pSkinTexturePacker->AddImage(filename);
	m_pTextureElements[m_iTextureElementsCount-1].m_iWidth = m_pSkinTexturePacker->GetImageWidth(m_pTextureElements[m_iTextureElementsCount-1].image_id);
	m_pTextureElements[m_iTextureElementsCount-1].m_iHeight = m_pSkinTexturePacker->GetImageHeight(m_pTextureElements[m_iTextureElementsCount-1].image_id);
}


void CGuiManager::BindSkin(uint32 skin_id,CRender *render)
{
	if(skin_id >= m_iSkinsCount)return;
	render->BindTexture(m_iSkinTextureId[skin_id]);
}

uint32 CGuiManager::AddSkin()
{
	++m_iSkinsCount;
	m_iSkinTextureId = (uint32*)ReAlloc(m_iSkinTextureId,m_iSkinsCount * sizeof(uint32));
	m_iSkinTextureId[m_iSkinsCount-1] = 0;
	return m_iSkinsCount-1;
}

void CGuiManager::CreateSkin(uint32 skin_id,CRender *render)
{
	if(skin_id >= m_iSkinsCount)return;
	m_pSkinTexturePacker->AddCropSize(128);
	m_pSkinTexturePacker->AddCropSize(256);
	m_pSkinTexturePacker->AddCropSize(512);
	m_pSkinTexturePacker->AddCropSize(1024);
	m_iSkinTextureId[skin_id] = m_pSkinTexturePacker->CreateTexture(render,512,512,0);
}

Float32 CGuiManager::GetUCoords(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Float32 u_coord)
{
	if(skin_id >= m_iSkinsCount)return 0.0f;
	for(uint32 i=0;i<m_iTextureElementsCount;i++)
	{
		if(m_pTextureElements[i].m_iSkinId == skin_id)
		{
			if(m_pTextureElements[i].m_iGuiSkinElement == gui_skin_element)
			{
				if(m_pTextureElements[i].m_iGuiSkinElementPart == gui_skin_element_part)
				{
					return m_pSkinTexturePacker->GetUCoord(m_pTextureElements[i].image_id,u_coord);
				}
			}
		}
	}
	return 0.0f;
}

Float32 CGuiManager::GetVCoords(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part,Float32 v_coord)
{
	if(skin_id >= m_iSkinsCount)return 0.0f;
	for(uint32 i=0;i<m_iTextureElementsCount;i++)
	{
		if(m_pTextureElements[i].m_iSkinId == skin_id)
		{
			if(m_pTextureElements[i].m_iGuiSkinElement == gui_skin_element)
			{
				if(m_pTextureElements[i].m_iGuiSkinElementPart == gui_skin_element_part)
				{
					return m_pSkinTexturePacker->GetVCoord(m_pTextureElements[i].image_id,v_coord);
				}
			}
		}
	}
	return 0.0f;
}

Bool CGuiManager::GetTextureExist(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part)
{
	if(skin_id >= m_iSkinsCount)return false;
	for(uint32 i=0;i<m_iTextureElementsCount;i++)
	{
		if(m_pTextureElements[i].m_iSkinId == skin_id)
		{
			if(m_pTextureElements[i].m_iGuiSkinElement == gui_skin_element)
			{
				if(m_pTextureElements[i].m_iGuiSkinElementPart == gui_skin_element_part)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int32 CGuiManager::GetTextureWidth(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part)
{
	if(skin_id >= m_iSkinsCount)return false;
	for(uint32 i=0;i<m_iTextureElementsCount;i++)
	{
		if(m_pTextureElements[i].m_iSkinId == skin_id)
		{
			if(m_pTextureElements[i].m_iGuiSkinElement == gui_skin_element)
			{
				if(m_pTextureElements[i].m_iGuiSkinElementPart == gui_skin_element_part)
				{
					return m_pTextureElements[i].m_iWidth;
				}
			}
		}
	}
	return 0;
}

int32 CGuiManager::GetTextureHeight(uint32 skin_id,uint32 gui_skin_element,uint32 gui_skin_element_part)
{
	if(skin_id >= m_iSkinsCount)return false;
	for(uint32 i=0;i<m_iTextureElementsCount;i++)
	{
		if(m_pTextureElements[i].m_iSkinId == skin_id)
		{
			if(m_pTextureElements[i].m_iGuiSkinElement == gui_skin_element)
			{
				if(m_pTextureElements[i].m_iGuiSkinElementPart == gui_skin_element_part)
				{
					return m_pTextureElements[i].m_iHeight;
				}
			}
		}
	}
	return 0;
}

uint32 CGuiManager::CreateButton(Bool is_switch,uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render)
{
	if(skin_id >= m_iSkinsCount)skin_id = 0;

	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	if(!is_switch)
	{
		m_pGui[m_iGuiCount-1] = new CButton(width,height,CButton::BUTTONTYPE::BUTTON_TYPE_BUTTON,build_type,skin_id,render,this);
	}
	else
	{
		m_pGui[m_iGuiCount-1] = new CButton(width,height,CButton::BUTTONTYPE::BUTTON_TYPE_SWITCH,build_type,skin_id,render,this);
	}
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_BUTTON);


	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateCheckBox(uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	m_pGui[m_iGuiCount-1] = new CCheckRadioBox(width,height,CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_CHECK,build_type,skin_id,render,this);
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_CHECK_BOX);


	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateRadioButton(uint32 width,uint32 height,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	m_pGui[m_iGuiCount-1] = new CCheckRadioBox(width,height,CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_RADIO,build_type,skin_id,render,this);
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_RADIO_BOX);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateSlider(bool is_vertical,int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	if(!is_vertical)
	{
		m_pGui[m_iGuiCount-1] = new CHSlider(min_pos,max_pos,bar_width,bar_height,button_width,button_height,build_type,skin_id,render,this);
		m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_HSLIDER);
	}
	else
	{
		m_pGui[m_iGuiCount-1] = new CVSlider(min_pos,max_pos,bar_width,bar_height,button_width,button_height,build_type,skin_id,render,this);
		m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_VSLIDER);
	}

	return m_iGuiCount-1;

}

uint32 CGuiManager::CreateLabel(Bool is_static,uint32 edit_type,uint32 size,uint32 space,uint32 bettween_line_space,uint32 max_text_length,Str text,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	if(is_static)
	{
		m_pGui[m_iGuiCount-1] = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_STATIC,edit_type,size,space,bettween_line_space,max_text_length,text,build_type,skin_id,render,m_pFont,this);
	}
	else
	{
		m_pGui[m_iGuiCount-1] = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_DYNAMIC,edit_type,size,space,bettween_line_space,max_text_length,text,build_type,skin_id,render,m_pFont,this);
	}
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_LABEL);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateEdit(Bool is_readonly,int32 width,int32 height,uint32 max_text_length,uint32 text_space,Str text,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	if(!is_readonly)
	{
		m_pGui[m_iGuiCount-1] = new CEdit(CEdit::EDITTYPE::EDIT_TYPE_READWRITE,width,height,max_text_length,text_space,text,build_type,skin_id,render,m_pFont,this);
	}
	else
	{
		m_pGui[m_iGuiCount-1] = new CEdit(CEdit::EDITTYPE::EDIT_TYPE_READONLY,width,height,max_text_length,text_space,text,build_type,skin_id,render,m_pFont,this);
	}
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_EDIT);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateTextArea(Bool is_readonly,Bool wrap,int32 width,int32 height,uint32 font_size,uint32 hscroll_width,uint32 vscroll_width,uint32 max_text_length,uint32 text_space,uint32 bettween_line_space,Str text,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	if(!is_readonly)
	{
		m_pGui[m_iGuiCount-1] = new CTextArea(CEdit::EDITTYPE::EDIT_TYPE_READWRITE,wrap,width,height,font_size,hscroll_width,vscroll_width,max_text_length,text_space,bettween_line_space,text,build_type,skin_id,render,m_pFont,this);
	}
	else
	{
		m_pGui[m_iGuiCount-1] = new CTextArea(CEdit::EDITTYPE::EDIT_TYPE_READONLY,wrap,width,height,font_size,hscroll_width,vscroll_width,max_text_length,text_space,bettween_line_space,text,build_type,skin_id,render,m_pFont,this);
	}
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_TEXT_AREA);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateCursor(uint32 type,int32 width,int32 height,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	m_pGui[m_iGuiCount-1] = new CCursor(type,width,height,build_type,skin_id,render,this);
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_CURSOR);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateGuiWindow(Bool is_movable,int32 width,int32 height,uint32 title_height,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	m_pGui[m_iGuiCount-1] = new CWindow(width,height,title_height,build_type,skin_id,render,this);
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_WINDOW);
	m_pGui[m_iGuiCount-1]->SetWindowMovable(is_movable);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateScrollArea(int32 width,int32 height,int32 hscroll_bar_width,int32 vscroll_bar_width,int32 min_scroll_button_size,int32 hbuttons_step,int32 vbuttons_step,uint32 build_type,uint32 skin_id,CRender *render)
{
	++m_iGuiCount;
	m_pGui = (CGui**)ReAlloc(m_pGui,m_iGuiCount * sizeof(CGui));
	m_pDrawElements = (DrawElementsData*)ReAlloc(m_pDrawElements,m_iGuiCount * sizeof(DrawElementsData));
	m_pDrawElementOrder = (int32*)ReAlloc(m_pDrawElementOrder,m_iGuiCount * sizeof(int32));

	m_pGui[m_iGuiCount-1] = new CScrollArea(width,height,hscroll_bar_width,vscroll_bar_width,min_scroll_button_size,hbuttons_step,vbuttons_step,build_type,skin_id,render,this);
	m_pGui[m_iGuiCount-1]->SetType(CGuiManager::GUITYPE::GUI_TYPE_SCROLL_AREA);

	return m_iGuiCount-1;
}

uint32 CGuiManager::CreateFont(CRender *render,Str font_symbols,Str ttf_name,uint32 font_height,uint32 font_space,uint32 texture_width,uint32 texture_height)
{
	m_pFont->CreateTextureForSymbols(render,font_symbols,ttf_name,font_height,font_space,texture_width,texture_height);
	return 1;
}

Str CGuiManager::GetCharByIndex(uint32 index)
{
	return m_pFont->GetCharByIndex(index);
}

void CGuiManager::SetColorToAllGuiElements(Vec4f color)
{
	for(uint32 i=0;i<m_iGuiCount;i++)
	{
		m_pGui[i]->SetColor(color);
	}
}

CGui::CGui()
{
	m_iScreenWidth = 0;
	m_iScreenHeight = 0;
	m_iState = 0;
	m_iType = 0;
	m_iBuildType = 0;
	m_iGroupId = 0;
	m_vColor = Vec4f(255.0f,255.0f,255.0f,255.0f);
	m_vFontColor = Vec4f(255.0f,255.0f,255.0f,255.0f);
	m_bVisible = true;
	m_bMovedFromInit = false;
	m_vVisibleArea = Vec4i(0,0,0,0);
	m_iTopBorderWidth = 0;
	m_iBottomBorderWidth = 0;
	m_iLeftBorderWidth = 0;
	m_iRightBorderWidth = 0;

	m_iActive = CGui::GUIACTIVE::GUI_ACTIVE;
	m_fPositionX = 0.0f;
	m_fPositionY = 0.0f;
	m_fMaxInsideX = 0.0f;
	m_fMaxInsideY = 0.0f;
	m_fContentXPos = 0.0f;
	m_fContentYPos = 0.0f;
	m_iWidth = 0;
	m_iHeight = 0;
	m_fXScale = 1.0f;
	m_fYScale = 1.0f;
	m_iGeometry = 0;
}

CGui::~CGui()
{
}

Bool CGui::InVisibleArea()
{
	if(GetVisible())
	{
		if(GetVisibleAreaW() != 0 && GetVisibleAreaH() != 0)
		{
			if( int32(GetPositionX() + GetWidth()) < GetVisibleAreaX() || 
				int32(GetPositionY() + GetHeight()) < GetVisibleAreaY() ||
				GetPositionX() > int32(GetVisibleAreaX() + GetVisibleAreaW()) ||
				GetPositionY() > int32(GetVisibleAreaY() + GetVisibleAreaH()))
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CGui::AddGuiQuadToGeometry(uint32 geometry_id,uint32 build_type,uint32 skin_id,uint32 default_skin_type,uint32 skin_type,int32 width,int32 height,CRender *render,CGuiManager *gui_manager)
{
	if(build_type == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID)
	{
		Float32 u = 0.0f;
		Float32 v = 0.0f;

		int32 *index_data;
		Vec3f *geometry_data;
		Vec2f *texcoord_data;

		index_data = new int32[4];
		geometry_data = new Vec3f[4];
		texcoord_data = new Vec2f[4];

		geometry_data[0] = Vec3f(0,0,0);
		geometry_data[1] = Vec3f(0,(Float32)-height,0);
		geometry_data[2] = Vec3f((Float32)width,0,0);
		geometry_data[3] = Vec3f((Float32)width,(Float32)-height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,build_type))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,build_type,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,build_type,1.0f);
			texcoord_data[0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,build_type,0.0f);
			texcoord_data[1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,build_type,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,build_type,1.0f);
			texcoord_data[2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,build_type,0.0f);
			texcoord_data[3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,build_type,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,build_type,1.0f);
			texcoord_data[0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,build_type,0.0f);
			texcoord_data[1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,build_type,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,build_type,1.0f);
			texcoord_data[2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,build_type,0.0f);
			texcoord_data[3] = Vec2f(u,v);
		}
		
		index_data[0] = 0;
		index_data[1] = 1;
		index_data[2] = 2;
		index_data[3] = 3;
		
		render->AddVerexToGeometry(geometry_id,4,geometry_data);
		render->AddTexCoordsToGeometry(geometry_id,4,texcoord_data);
		render->AddIndexToGeometry(geometry_id,4,index_data);
		render->SetStartNewGeometry(geometry_id);

		SAFE_DELETE_ARRAY(geometry_data);
		SAFE_DELETE_ARRAY(texcoord_data);
		SAFE_DELETE_ARRAY(index_data);
	}
	else if(build_type == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER)
	{
		Float32 u = 0.0f;
		Float32 v = 0.0f;

		int32 *index_data;
		Vec3f *geometry_data;
		Vec2f *texcoord_data;

		int32 count = 36;
		geometry_data = new Vec3f[count];
		texcoord_data = new Vec2f[count];
		index_data = new int32[count];

		Float32 startx = 0.0f;
		Float32 starty = 0.0f;

		int32 part_tl_width = gui_manager->GetTextureWidth(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tl_height = gui_manager->GetTextureHeight(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tr_width = gui_manager->GetTextureWidth(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_tr_height = gui_manager->GetTextureHeight(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_bl_width = gui_manager->GetTextureWidth(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_bl_height = gui_manager->GetTextureHeight(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_br_width = gui_manager->GetTextureWidth(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		int32 part_br_height = gui_manager->GetTextureHeight(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER))
		{
			part_tl_width = gui_manager->GetTextureWidth(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER))
		{
			part_tl_height = gui_manager->GetTextureHeight(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER))
		{
			part_tr_width = gui_manager->GetTextureWidth(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER))
		{
			part_tr_height = gui_manager->GetTextureHeight(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER))
		{
			part_bl_width = gui_manager->GetTextureWidth(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER))
		{
			part_bl_height = gui_manager->GetTextureHeight(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER))
		{
			part_br_width = gui_manager->GetTextureWidth(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		}
		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER))
		{
			part_br_height = gui_manager->GetTextureHeight(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		}

		while(part_tl_width + part_tr_width > width)
		{
			if(part_tl_width > 0)part_tl_width--;
			if(part_tr_width > 0)part_tr_width--;
			if(part_bl_width > 0)part_bl_width--;
			if(part_br_width > 0)part_br_width--;
		}

		while(part_tl_height + part_tr_height > height)
		{
			if(part_tl_height > 0)part_tl_height--;
			if(part_tr_height > 0)part_tr_height--;
			if(part_bl_height > 0)part_bl_height--;
			if(part_br_height > 0)part_br_height--;
		}


		int32 i=0;

		//TL Corner
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_tl_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_tl_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_tl_width,(Float32)starty-part_tl_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx+=part_tl_width;
		i+=4;
		//T Border
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_tl_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+(m_iWidth-part_tl_width-part_tr_width),starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+(m_iWidth-part_tl_width-part_tr_width),(Float32)starty-part_tl_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_T_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx+=width-part_tl_width-part_tr_width;
		i+=4;
		//TR Corner
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_tr_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_tr_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_tr_width,(Float32)starty-part_tr_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx = 0;
		starty-=part_tl_height;
		i+=4;
		//L Border
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-(height-part_tl_height-part_bl_height),0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_tl_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_tl_width,(Float32)starty-(height-part_tl_height-part_bl_height),0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_L_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx += part_tl_width;
		i+=4;
		//Center
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-(height-part_tl_height-part_bl_height),0);
		geometry_data[i+2] = Vec3f((Float32)startx+(width-part_tl_width-part_tr_width),starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+(width-part_tl_width-part_tr_width),(Float32)starty-(height-part_tl_height-part_bl_height),0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_CENTER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx += (width-part_tl_width-part_tr_width);
		i+=4;
		//R Border
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-(height-part_tr_height-part_br_height),0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_tr_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_tr_width,(Float32)starty-(height-part_tr_height-part_br_height),0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_R_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx = 0;
		starty-=(height-part_tl_height-part_bl_height);
		i+=4;
		//BL Corner
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_bl_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_bl_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_bl_width,(Float32)starty-part_bl_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx+=part_bl_width;
		i+=4;
		//B Border
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_bl_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+(width-part_bl_width-part_br_width),starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+(width-part_bl_width-part_br_width),(Float32)starty-part_bl_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_B_BORDER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

		startx+=(width-part_bl_width-part_br_width);
		i+=4;

		//BR Corner
		geometry_data[i+0] = Vec3f(startx,starty,0);
		geometry_data[i+1] = Vec3f(startx,(Float32)starty-part_br_height,0);
		geometry_data[i+2] = Vec3f((Float32)startx+part_br_width,starty,0);
		geometry_data[i+3] = Vec3f((Float32)startx+part_br_width,(Float32)starty-part_br_height,0);

		if(gui_manager->GetTextureExist(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER))
		{
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}
		else
		{
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			texcoord_data[i+0] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			texcoord_data[i+1] = Vec2f(u,v);
			u = gui_manager->GetUCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,1.0f);
			texcoord_data[i+2] = Vec2f(u,v);
			v = gui_manager->GetVCoords(skin_id,default_skin_type,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER,0.0f);
			texcoord_data[i+3] = Vec2f(u,v);
		}

	
		for(i=0;i<count;i++)
		{				
			index_data[i] = i;
		}

		render->AddVerexToGeometry(GetGeometryId(),count,geometry_data);
		render->AddTexCoordsToGeometry(GetGeometryId(),count,texcoord_data);
		render->AddIndexToGeometry(GetGeometryId(),count,index_data);
		render->SetStartNewGeometry(GetGeometryId());

		SAFE_DELETE_ARRAY(geometry_data);
		SAFE_DELETE_ARRAY(texcoord_data);
		SAFE_DELETE_ARRAY(index_data);
	}
}

//Button

CButton::CButton(uint32 width,uint32 height,uint32 type,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iWidth = width;
	m_iHeight = height;
	m_iGuiType = type;
	m_iSwitchHolder = 0;
	SetSkinId(skin_id);
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	//button
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,m_iWidth,m_iHeight,render,gui_manager);
	//button moved
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON_OVER,m_iWidth,m_iHeight,render,gui_manager);
	//button pressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_BUTTON_PRESSED,m_iWidth,m_iHeight,render,gui_manager);
	
	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CButton::~CButton()
{

}

uint32 CButton::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());

	if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_BUTTON)
	{
		if(GetState() == CButton::BUTTONSTATE::BUTTON_STATE_PRESSED)
		{
			render->DrawGeometry(CRender::DRAWPRIMITIVE::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
		}
		else if(GetState() == CButton::BUTTONSTATE::BUTTON_STATE_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}
	else if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_SWITCH)
	{
		if(GetState() == CButton::BUTTONSTATE::BUTTON_STATE_SWITCHED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);
	return ret;
}

uint32 CButton::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;
	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;
	if(mouse_button == 0)m_iSwitchHolder = 0;

	if(mousex > GetPositionX() && mousex < GetPositionX() + GetWidth() && mousey > GetPositionY() && mousey < GetPositionY() + GetHeight())
	{
		if(mouse_button == 1)
		{
			if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_SWITCH)
			{
				if(m_iSwitchHolder == 0)
				{
					GetState() == CButton::BUTTONSTATE::BUTTON_STATE_SWITCHED ? SetState(CButton::BUTTONSTATE::BUTTON_STATE_NONE) : SetState(CButton::BUTTONSTATE::BUTTON_STATE_SWITCHED);
					ret = GetGroupId();
					m_iSwitchHolder = 1;
				}
			}
			else if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_BUTTON)
			{
				SetState(CButton::BUTTONSTATE::BUTTON_STATE_PRESSED);
				ret = GetGroupId();	
			}
		}
		else
		{
			if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_BUTTON)
			{
				SetState(CButton::BUTTONSTATE::BUTTON_STATE_MOVED);
			}
		}
	}
	else
	{
		if(mouse_button == 0)
		{
			if(m_iGuiType == CButton::BUTTONTYPE::BUTTON_TYPE_BUTTON)
			{
				SetState(CButton::BUTTONSTATE::BUTTON_STATE_NONE);
			}
		}
	}


	return ret;
}


CCheckRadioBox::CCheckRadioBox(uint32 width,uint32 height,uint32 type,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iWidth = width;
	m_iHeight = height;
	m_iGuiType = type;
	m_iSwitchHolder = 0;
	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);
	
	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_CHECK)
	{
		//checkbox unchecked
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX,CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX,m_iWidth,m_iHeight,render,gui_manager);
		//checkbox checked
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX_CHECKED,CGuiManager::GUISKINTYPE::GUI_SKIN_CHECK_BOX_CHECKED,m_iWidth,m_iHeight,render,gui_manager);
	}

	if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_RADIO)
	{
		//radio unchecked
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX,CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX,m_iWidth,m_iHeight,render,gui_manager);
		//radio checked
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX_CHECKED,CGuiManager::GUISKINTYPE::GUI_SKIN_RADIO_BUTTON_BOX_CHECKED,m_iWidth,m_iHeight,render,gui_manager);
	}

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CCheckRadioBox::~CCheckRadioBox()
{}

uint32 CCheckRadioBox::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);
	
	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());
	
	if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_CHECK)
	{
		if(GetState() == CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_CHECKED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}
	else if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_RADIO)
	{
		if(GetState() == CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_CHECKED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);
	return ret;
}

uint32 CCheckRadioBox::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;
	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;
	if(mouse_button == 0)m_iSwitchHolder = 0;

	if(mousex > GetPositionX() && mousex < GetPositionX() + GetWidth() && mousey > GetPositionY() && mousey < GetPositionY() + GetHeight())
	{
		if(mouse_button == 1)
		{
			if(m_iSwitchHolder == 0)
			{
				if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_CHECK)
				{
					GetState() == CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_CHECKED ? SetState(CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_UNCHECKED) : SetState(CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_CHECKED);
					ret = GetGroupId();
				}
				else if(m_iGuiType == CCheckRadioBox::CHECKRADIOTYPE::CHECK_RADIO_TYPE_RADIO)
				{
					
					SetState(CCheckRadioBox::CHECKRADIOSTATE::CHECK_RADIO_STATE_CHECKED);
					ret = GetGroupId();
				}
				m_iSwitchHolder = 1;
			}
		}
	}
	return ret;
}

CHSlider::CHSlider(int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{	
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_fSliderPosition = 0.0f;
	m_iSliderButtonPosition = 0;
	m_iMinPosition = min_pos;
	m_iMaxPosition = max_pos;
	m_iWidth = bar_width;
	m_iHeight = bar_height;
	m_iBarWidth = bar_width;
	m_iBarHeight = bar_height;
	m_iButtonWidth = button_width;
	m_iButtonHeight = button_height;
	m_iGuiType = 0;
	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	m_fSliderStep = Float32(Float32(m_iMaxPosition) - Float32(m_iMinPosition))/Float32(Float32(bar_width));
	
	//slider bar unpressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR,m_iBarWidth,m_iBarHeight,render,gui_manager);	
	//slider bar pressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR_PRESSED,m_iBarWidth,m_iBarHeight,render,gui_manager);	
	//moved
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BAR_OVER,m_iBarWidth,m_iBarHeight,render,gui_manager);
	
	//slider button
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON,m_iButtonWidth,m_iButtonHeight,render,gui_manager);	
	//pressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON_PRESSED,m_iButtonWidth,m_iButtonHeight,render,gui_manager);	
	//moved
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_HSLIDER_BUTTON_OVER,m_iButtonWidth,m_iButtonHeight,render,gui_manager);		

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());

	CalculateState(0,0,0);
}

CHSlider::~CHSlider()
{
}

uint32 CHSlider::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->Translate2D(x,y);

	if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_PRESSED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
	}
	else if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_MOVED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
	}
	else
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
	}

	render->AddTranslate2D(m_iSliderButtonPosition-m_iButtonWidth*0.5f,-Float32(Float32(m_iButtonHeight*0.25f)));

	if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_PRESSED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),4);
	}
	else if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_MOVED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),5);
	}
	else
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);
	return ret;
}

uint32 CHSlider::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;

	if(!InVisibleArea()) return ret;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;

	if(mouse_button == 0)
	{
		m_iSwitchHolder = 0;
		if(GetActive() == CGui::GUIACTIVE::GUI_GRAB)
		{
			SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
		}
	}

	if(m_iSwitchHolder == 1)
	{
		if(mousex <= GetPositionX())
		{
			m_iSliderButtonPosition = 0;
		}
		else if(mousex>= GetPositionX()+m_iBarWidth)
		{
			m_iSliderButtonPosition = m_iBarWidth;
		}
		else
		{
			m_iSliderButtonPosition = mousex - (int32)GetPositionX();
		}
	}
	else
	{
		SetState(CHSlider::SLIDERSTATE::SLIDER_STATE_NONE);
		if(mousex > GetPositionX() && mousex < GetPositionX() + m_iBarWidth && mousey > GetPositionY() && mousey < GetPositionY() + m_iBarHeight)
		{
			SetState(CHSlider::SLIDERSTATE::SLIDER_STATE_MOVED);
			if(mouse_button == 1)
			{
				SetState(CHSlider::SLIDERSTATE::SLIDER_STATE_PRESSED);
				if(m_iSwitchHolder == 0)
				{
					m_iSliderButtonPosition = mousex - (int32)GetPositionX();
					m_iSwitchHolder = 1;
					SetActive(CGui::GUIACTIVE::GUI_GRAB);
				}
			}
		}
	}

	if(m_iSliderButtonPosition == 0)
	{
		m_fSliderPosition = (Float32)m_iMinPosition;
	}
	else if(m_iSliderButtonPosition >= m_iBarWidth)
	{
		m_fSliderPosition = (Float32)m_iMaxPosition;
	}
	else
	{
		m_fSliderPosition = Float32(m_iSliderButtonPosition+m_iMinPosition) * m_fSliderStep;
	}

	return ret;
}

//VSlider

CVSlider::CVSlider(int32 min_pos,int32 max_pos,uint32 bar_width,uint32 bar_height,uint32 button_width,uint32 button_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iSliderButtonPosition = 0;
	m_iMinPosition = min_pos;
	m_iMaxPosition = max_pos;
	m_iWidth = bar_width;
	m_iHeight = bar_height;
	m_iBarWidth = bar_width;
	m_iBarHeight = bar_height;
	m_iButtonWidth = button_width;
	m_iButtonHeight = button_height;
	m_iGuiType = 0;
	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	m_fSliderStep = Float32(m_iMaxPosition - m_iMinPosition)/Float32(bar_height);

	//slider bar unpressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR,m_iBarWidth,m_iBarHeight,render,gui_manager);	
	//slider bar pressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR_PRESSED,m_iBarWidth,m_iBarHeight,render,gui_manager);	
	//moved
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BAR_OVER,m_iBarWidth,m_iBarHeight,render,gui_manager);
	
	//slider button
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON,m_iButtonWidth,m_iButtonHeight,render,gui_manager);	
	//pressed
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON_PRESSED,m_iButtonWidth,m_iButtonHeight,render,gui_manager);	
	//moved
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_VSLIDER_BUTTON_OVER,m_iButtonWidth,m_iButtonHeight,render,gui_manager);		
	

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());

	CalculateState(0,0,0);
}

CVSlider::~CVSlider()
{
}

uint32 CVSlider::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->Translate2D(x,y);


	if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_PRESSED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
	}
	else if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_MOVED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
	}
	else
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
	}

	render->AddTranslate2D(-Float32(Float32(m_iButtonWidth*0.25f)),m_iSliderButtonPosition-m_iButtonHeight*0.5f);

	if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_PRESSED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),4);
	}
	else if(GetState() == CHSlider::SLIDERSTATE::SLIDER_STATE_MOVED)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),5);
	}
	else
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);
	return ret;
}

uint32 CVSlider::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;
	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;
	if(mouse_button == 0)
	{
		m_iSwitchHolder = 0;
		if(GetActive() == CGui::GUIACTIVE::GUI_GRAB)
		{
			SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
		}
	}

	if(m_iSwitchHolder == 1)
	{
		if(mousey <= GetPositionY())
		{
			m_iSliderButtonPosition = 0;
		}
		else if(mousey >= GetPositionY()+m_iBarHeight)
		{
			m_iSliderButtonPosition = m_iBarHeight;
		}
		else
		{
			m_iSliderButtonPosition = mousey - (int32)GetPositionY();
		}
	}
	else
	{
		SetState(CVSlider::SLIDERSTATE::SLIDER_STATE_NONE);
		if(mousex > GetPositionX() && mousex < GetPositionX() + m_iBarWidth && mousey > GetPositionY() && mousey < GetPositionY() + m_iBarHeight)
		{
			SetState(CVSlider::SLIDERSTATE::SLIDER_STATE_MOVED);
			if(mouse_button == 1)
			{
				SetState(CVSlider::SLIDERSTATE::SLIDER_STATE_PRESSED);
				if(m_iSwitchHolder == 0)
				{
					m_iSliderButtonPosition = mousey - (int32)GetPositionY();
					m_iSwitchHolder = 1;
					SetActive(CGui::GUIACTIVE::GUI_GRAB);
				}
			}
		}
	}


	if(m_iSliderButtonPosition == 0)
	{
		m_fSliderPosition = (Float32)m_iMinPosition;
	}
	else if(m_iSliderButtonPosition >= m_iBarHeight)
	{
		m_fSliderPosition = (Float32)m_iMaxPosition;
	}
	else
	{
		m_fSliderPosition = Float32(m_iSliderButtonPosition+m_iMinPosition) * m_fSliderStep;
	}

	return ret;
}

CLabel::CLabel(uint32 type,uint32 edit_type,uint32 size,uint32 space,uint32 bettween_line_space,uint32 max_text_length,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
	m_iGuiType = type;
	m_iEditType = edit_type;
	m_iTextSpace = space;
	m_iFontSize = size == 0 ? font->GetFontHeight() : size;
	m_iBettweenLineSpace = bettween_line_space;
	m_sText = text;
	m_iMaxTextLength = max_text_length;
	m_iTextChanged = 1;

	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);

	m_fSymbolSizes = new Float32[m_iMaxTextLength];
	
	m_fNextStep = 0.0f;
	m_fPrevStep = 0.0f;
	m_fCursorX = 0.0f;
	m_fCursorY = 0.0f;
	m_iCursorColumn = 0;
	m_iCursorRow = 0;

	m_fFontScale = Float32(m_iFontSize)/Float32(font->GetFontHeight());
	
	if(m_iGuiType == CLabel::LABELTYPE::LABEL_TYPE_STATIC)
	{
		if(m_iMaxTextLength < m_sText.Size())
		{
			m_sText = m_sText.SubStr(0,m_iMaxTextLength);
		}
		m_iFont = font->InitStaticText(render,m_sText,m_iFontSize,m_iTextSpace,m_iBettweenLineSpace);
		for(uint32 i=0;i<m_sText.Size();i++)
		{
			m_fSymbolSizes[i]=font->GetSymbolWidth(m_sText[i]);
		}
	}
	else if(m_iGuiType == CLabel::LABELTYPE::LABEL_TYPE_DYNAMIC)
	{
		if(m_iMaxTextLength > m_sText.Size())
		{
			while(m_iMaxTextLength > m_sText.Size())
			{
				m_sText += Str("a");
			}
		}
		else if(m_iMaxTextLength < m_sText.Size())
		{
			m_sText = m_sText.SubStr(0,m_iMaxTextLength);
		}
		m_iFont = font->InitDynamicText(render,m_sText,m_iFontSize,m_iTextSpace,m_iBettweenLineSpace);
		for(uint32 i=0;i<m_sText.Size();i++)
		{
			m_fSymbolSizes[i]=font->GetSymbolWidth(m_sText[i]);
		}
	}

	m_fNextStep = GetSymbolWidth(0)*m_fFontScale;

	if(m_iEditType == CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE || m_iEditType == CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE_SELECTABLE)
	{
		SetGeometryId(render->CreateGeometry());
		render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
		render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

		//edit cursor
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR_EDIT,2,m_iFontSize,render,gui_manager);	
		
		render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
		render->LoadToVertexBufferGeometry(GetGeometryId());
		render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
		render->LoadToIndexBufferGeometry(GetGeometryId());
	}
}

CLabel::~CLabel()
{
	SetPositionX(0);
	SetPositionY(0);
	m_iTextSpace = 0;
	m_iFontSize = 0;
	m_iFont = 0;
	m_iGuiType = 0;
	m_iMaxTextLength = 0;
	m_iTextChanged = 0;

	m_fNextStep = 0.0f;
	m_fPrevStep = 0.0f;
	m_fCursorX = 0.0f;
	m_fCursorY = 0.0f;
	m_iCursorColumn = 0;
	m_iCursorRow = 0;
	SAFE_DELETE_ARRAY(m_fSymbolSizes);
}

uint32 CLabel::DrawFont(Float32 x,Float32 y,CRender *render,CFont *font)
{
	uint32 ret = 0;

	if(!InVisibleArea()) return ret;

	render->SetColor(GetFontColor().v[0],GetFontColor().v[1],GetFontColor().v[2],GetFontColor().v[3]);

	if(m_iTextChanged == 1)
	{
		font->SetTextChanged(m_iFont,m_iTextChanged);
		m_iTextChanged = 0;
	}

	if(m_iGuiType == CLabel::LABELTYPE::LABEL_TYPE_STATIC)
	{
		font->DrawStaticText(render,m_iFont,(uint32)x,(uint32)y);
		m_iWidth = font->GetStringWidth(m_iFont);
		m_iHeight = font->GetStringHeight(m_iFont);
	}
	else if(m_iGuiType == CLabel::LABELTYPE::LABEL_TYPE_DYNAMIC)
	{
		font->DrawDynamicText(render,m_iFont,(uint32)x,(uint32)y,m_sText);

		if(m_sAddChar.Size() > 0)
		{
			m_fNextStep = math_floor(font->GetSymbolWidth(m_sAddChar[0])*m_fFontScale) + m_iTextSpace;
			m_sAddChar.Clear();
		}

		for(uint32 i=0;i<m_sText.Size();i++)
		{
			m_fSymbolSizes[i]=font->GetSymbolWidth(m_sText[i]);
		}

		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}
		Float32 cursor_x = 0.0f;
		Float32 mid_cursor_x = 0.0f;
		for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
		{
			cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
			mid_cursor_x += math_floor(GetSymbolWidth(i)*0.5f*m_fFontScale);
			
			if(m_fCursorX >= mid_cursor_x && m_fCursorX <= cursor_x)
			{
				m_fNextStep = math_floor(GetSymbolWidth(i+1)*m_fFontScale) + m_iTextSpace;
				m_fPrevStep = math_floor(GetSymbolWidth(i)*m_fFontScale) + m_iTextSpace;

				m_iCursorColumn = i-start;
				m_fCursorX = cursor_x;
				break;
			}
			cursor_x += m_iTextSpace;
		}

		m_iWidth = font->GetStringWidth(m_iFont);
		m_iHeight = font->GetStringHeight(m_iFont);
	}
	render->UnSetColor();
	return ret;
}

uint32 CLabel::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(m_iEditType == CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE || m_iEditType == CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE_SELECTABLE)
	{
		if(!InVisibleArea()) return ret;

		render->OnOffLighting(0);
		render->OnOffBlending(1);
		render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
		render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
		render->SetDrawable(GetGeometryId());
		render->SetIndexBuffer(GetGeometryId(),0);
		render->OnOffDepthTest(0);
		render->SwitchTo2D();
		render->PushMatrix();
		
		render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());
		
		if(GetState() == CLabel::LABELSTATE::LABEL_STATE_EDIT)
		{
			//Draw cursor
			render->AddTranslate2D(m_fCursorX,m_fCursorY);
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}

		render->PopMatrix();
		render->SwitchTo3D();
		render->OnOffDepthTest(1);
		render->UnSetColor();
		render->OnOffBlending(0);
		render->OnOffLighting(1);
	}

	return ret;
}

uint32 CLabel::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;
	if(m_iEditType == CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_NONE)return ret;
	uint32 start = 0;
	
	if(mousex > GetPositionX() && mousex < GetPositionX()+GetWidth() && mousey > GetPositionY() && mousey < GetPositionY() + GetHeight())
	{
		if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)
		{
			SetState(CLabel::LABELSTATE::LABEL_STATE_MOVED);
		}

		if(mouse_button == 1)
		{
			SetState(CLabel::LABELSTATE::LABEL_STATE_EDIT);
			for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
			{
				start += (j == 0 ? 0 : (m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2);
				if(mousey - GetPositionY() > j * (m_iFontSize+m_iBettweenLineSpace) && mousey - GetPositionY() < (j+1) * (m_iFontSize+m_iBettweenLineSpace))
				{
					m_iCursorRow = j;

					m_fCursorY = j * Float32(m_iFontSize+m_iBettweenLineSpace);
					m_fCursorX = 0.0f;
					m_iCursorColumn = -1;
					Float32 cursor_x = 0.0f;
					Float32 mid_cursor_x = 0.0f;
					for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),j).Size();i++)
					{
						cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
						mid_cursor_x += math_floor(GetSymbolWidth(i)*0.5f*m_fFontScale);
						if(mousex - GetPositionX() >= mid_cursor_x && mousex - GetPositionX() <= cursor_x)
						{
							m_fNextStep = math_floor(GetSymbolWidth(i+1)*m_fFontScale) + m_iTextSpace;
							m_fPrevStep = math_floor(GetSymbolWidth(i)*m_fFontScale) + m_iTextSpace;
			
							m_iCursorColumn = i-start;
							m_fCursorX = cursor_x;
							break;
						}
						cursor_x += m_iTextSpace;
						mid_cursor_x += m_iTextSpace;
					}
				}
			}
		}
	}
	else
	{
		if(mouse_button == 1)
		{
			SetState(CLabel::LABELSTATE::LABEL_STATE_NONE);
		}
	}

	return ret;
}

void CLabel::AddChar(Str ch)
{
	m_sAddChar = ch;
}

void CLabel::CursorHome()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorColumn >= 0)
	{ 
		m_fCursorX = 0.0f;
		m_iCursorColumn = -1;
		
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}
		m_fNextStep = math_floor(GetSymbolWidth(start+m_iCursorColumn+1)*m_fFontScale);
	}
}

void CLabel::CursorEnd()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorColumn <= (int32)m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-2)
	{
		for(uint32 i=m_iCursorColumn+1;i<m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
		{
			CursorRight();
		}
	}
}

void CLabel::CursorLeft()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorColumn >= 0)
	{ 
		m_iCursorColumn--;
		m_fCursorX -= m_fPrevStep;
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}
		m_fNextStep = math_floor(GetSymbolWidth(start+m_iCursorColumn+1)*m_fFontScale) + m_iTextSpace;
		m_fPrevStep = math_floor(GetSymbolWidth(start+m_iCursorColumn)*m_fFontScale) + m_iTextSpace;
	}
}

void CLabel::CursorRight()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorColumn <= (int32)m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-2)
	{
		m_iCursorColumn++;
		m_fCursorX += m_fNextStep;
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}
		m_fNextStep = math_floor(GetSymbolWidth(start+m_iCursorColumn+1)*m_fFontScale) + m_iTextSpace;
		m_fPrevStep = math_floor(GetSymbolWidth(start+m_iCursorColumn)*m_fFontScale) + m_iTextSpace;
	}
}

void CLabel::CursorUp()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorRow > 0)
	{
		m_iCursorRow--;
		m_fCursorY = m_iCursorRow * Float32(m_iFontSize+m_iBettweenLineSpace);

		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}
		
		if(m_iCursorColumn >= (int32)m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-1)
		{
			Float32 cursor_x = 0.0f;
			m_iCursorColumn = m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-1;
			for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
			{
				cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
				cursor_x += m_iTextSpace;
			}
			m_fCursorX = cursor_x - m_iTextSpace;

			m_fNextStep = math_floor(GetSymbolWidth(start+m_iCursorColumn+1)*m_fFontScale) + m_iTextSpace;
			m_fPrevStep = math_floor(GetSymbolWidth(start+m_iCursorColumn)*m_fFontScale) + m_iTextSpace;
		}
		else
		{
			Float32 cursor_x = 0.0f;
			Float32 mid_cursor_x = 0.0f;
			for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
			{
				cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
				mid_cursor_x += math_floor(GetSymbolWidth(i)*0.5f*m_fFontScale);
				if(m_fCursorX >= mid_cursor_x && m_fCursorX <= cursor_x)
				{
					m_fNextStep = math_floor(GetSymbolWidth(i+1)*m_fFontScale) + m_iTextSpace;
					m_fPrevStep = math_floor(GetSymbolWidth(i)*m_fFontScale) + m_iTextSpace;

					m_iCursorColumn = i-start;
					m_fCursorX = cursor_x;
					break;
				}
				cursor_x += m_iTextSpace;
			}
		}
	}
}
	
void CLabel::CursorDown()
{
	if(GetState() != CLabel::LABELSTATE::LABEL_STATE_EDIT)return;
	if(m_iCursorRow < (int32)m_sText.GetExpandCount(Str("\\n"))-1)
	{
		m_iCursorRow++;
		m_fCursorY = m_iCursorRow * Float32(m_iFontSize+m_iBettweenLineSpace);
		
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_iCursorRow == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_iCursorRow == j)break;
		}

		if(m_iCursorColumn >= (int32)m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-1)
		{
			Float32 cursor_x = 0.0f;
			m_iCursorColumn = m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size()-1;
			for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
			{
				cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
				cursor_x += m_iTextSpace;
			}
			m_fCursorX = cursor_x - m_iTextSpace;
			m_fNextStep = math_floor(GetSymbolWidth(start+m_iCursorColumn+1)*m_fFontScale) + m_iTextSpace;
			m_fPrevStep = math_floor(GetSymbolWidth(start+m_iCursorColumn)*m_fFontScale) + m_iTextSpace;
		}
		else
		{
			Float32 cursor_x = 0.0f;
			Float32 mid_cursor_x = 0.0f;
			for(uint32 i=start;i<start+m_sText.GetExpandPart(Str("\\n"),m_iCursorRow).Size();i++)
			{
				cursor_x += math_floor(GetSymbolWidth(i)*m_fFontScale);
				mid_cursor_x += math_floor(GetSymbolWidth(i)*0.5f*m_fFontScale);
				if(m_fCursorX >= mid_cursor_x && m_fCursorX <= cursor_x)
				{
					m_fNextStep = math_floor(GetSymbolWidth(i+1)*m_fFontScale) + m_iTextSpace;
					m_fPrevStep = math_floor(GetSymbolWidth(i)*m_fFontScale) + m_iTextSpace;

					m_iCursorColumn = i-start;
					m_fCursorX = cursor_x;
					break;
				}
				cursor_x += m_iTextSpace;
			}
		}
	}
}

CEdit::CEdit(uint32 type,int32 width,int32 height,uint32 max_text_length,uint32 text_space,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iTextSpace = text_space;
	m_iHeight = height;
	m_iWidth = width;
	m_iMaxTextLength = max_text_length;
	m_iGuiType = type;
	m_sText = text;
	m_iTextChanged = 1;
	m_bInsertFlag = false;

	m_iXOffset = 0;
	m_iYOffset = 0;
	m_iWidthOffset = 0;
	m_iHeightOffset = 0;

	m_fFontXOffset = 0.0f;

	m_sText = text;

	if(m_iGuiType == CEdit::EDITTYPE::EDIT_TYPE_READWRITE)
	{
		m_pLabel = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_DYNAMIC,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE_SELECTABLE,
			m_iHeight,m_iTextSpace,0,m_iMaxTextLength,m_sText,build_type,skin_id,render,font,gui_manager);
	}
	else
	{
		m_pLabel = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_STATIC,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_SELECTABLE,
			m_iHeight,m_iTextSpace,0,m_iMaxTextLength,m_sText,build_type,skin_id,render,font,gui_manager);
	}

	SetSkinId(skin_id);
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID)
	{
		SetTopBorderWidth(2);
		SetBottomBorderWidth(2);
		SetLeftBorderWidth(5);
		SetRightBorderWidth(5);
		
		//edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,m_iWidth,m_iHeight,render,gui_manager);	
		//edit moved
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_OVER,m_iWidth,m_iHeight,render,gui_manager);	
		//edit edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_PRESSED,m_iWidth,m_iHeight,render,gui_manager);	
		//edit readonly
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_READ_ONLY,m_iWidth,m_iHeight,render,gui_manager);	
	}
	else if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER)
	{
		int32 part_tl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tr_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_tr_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_bl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_bl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_br_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		int32 part_br_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);

		SetTopBorderWidth(part_tl_height);
		SetBottomBorderWidth(part_br_height);
		SetLeftBorderWidth(part_tl_width);
		SetRightBorderWidth(part_br_width);

		//edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,m_iWidth,m_iHeight,render,gui_manager);	
		//edit moved
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_OVER,m_iWidth,m_iHeight,render,gui_manager);	
		//edit edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_PRESSED,m_iWidth,m_iHeight,render,gui_manager);	
		//edit readonly
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_READ_ONLY,m_iWidth,m_iHeight,render,gui_manager);	
	}

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CEdit::~CEdit()
{
	SAFE_DELETE(m_pLabel)
}

uint32 CEdit::DrawFont(Float32 x,Float32 y,CRender *render,CFont *font)
{
	uint32 ret = 0;

	if(!InVisibleArea()) return ret;

	render->SetColor(GetFontColor().v[0],GetFontColor().v[1],GetFontColor().v[2],GetFontColor().v[3]);

	render->OnOffScissorsTest(1);
	
	m_iXOffset = GetVisibleAreaX() - (int32)x;
	if(m_iXOffset < 0)m_iXOffset = 0;

	m_iYOffset = GetVisibleAreaY() - (int32)y;
	if(m_iYOffset < 0)m_iYOffset = 0;

	m_iWidthOffset = int32((x-GetVisibleAreaX()) + m_iXOffset + GetWidth()) - GetVisibleAreaW();
	if(m_iWidthOffset < 0)m_iWidthOffset = 0;

	m_iHeightOffset = int32((y-GetVisibleAreaY()) + m_iYOffset + GetHeight()) - GetVisibleAreaH();
	if(m_iHeightOffset < 0)m_iHeightOffset = 0;

	if(GetWidth()-m_iWidthOffset-GetRightBorderWidth()-GetLeftBorderWidth() < 1 || GetHeight()-m_iHeightOffset < 1 ||
		GetVisibleAreaX()+1 > int32(x+GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()) ||
		GetVisibleAreaY()+1 > int32(y+GetHeight()-GetTopBorderWidth()-GetBottomBorderWidth()))
	{
		render->SetScissors((int32)x + m_iXOffset + GetLeftBorderWidth(),
						(int32)y + m_iYOffset,
						1,1);
	}
	else
	{
		render->SetScissors((int32)x + m_iXOffset + GetLeftBorderWidth(),
						(int32)y + m_iYOffset,
						GetWidth()-m_iWidthOffset-GetRightBorderWidth()-GetLeftBorderWidth() + (int32(x-GetVisibleAreaX()) < 0 ? int32(x-GetVisibleAreaX()) : 0),
						GetHeight()-m_iHeightOffset + (int32(y-GetVisibleAreaY()) < 0 ? int32(y-GetVisibleAreaY()) : 0));
	}


	m_pLabel->SetLabelText(m_sText);
	m_pLabel->DrawFont(x+GetLeftBorderWidth()+m_fFontXOffset,y+GetTopBorderWidth(),render,font);
	if(m_bInsertFlag)
	{
		CursorRight();
		m_bInsertFlag = false;
	}
	render->OnOffScissorsTest(0);
	render->UnSetColor();

	return ret;
}

uint32 CEdit::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());

	if(m_iGuiType == CEdit::EDITTYPE::EDIT_TYPE_READONLY)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
	}
	else
	{
		if(GetState() == CEdit::EDITSTATE::EDIT_STATE_EDIT)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
		}
		else if(GetState() == CEdit::EDITSTATE::EDIT_STATE_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);

	render->OnOffScissorsTest(1);
	render->SetScissors((int32)x + m_iXOffset,(int32)y + m_iYOffset,GetWidth()-m_iWidthOffset-GetRightBorderWidth(),GetHeight()-m_iHeightOffset);
	m_pLabel->Draw(x+GetLeftBorderWidth()+m_fFontXOffset,y,mousex,mousey,mouse_button,render);
	render->OnOffScissorsTest(0);
	return ret;
}

uint32 CEdit::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;

	int32 width_offset = 0;
	int32 height_offset = 0;
	if(GetPositionX()+GetWidth() > GetVisibleAreaX()+GetVisibleAreaW())
	{
		width_offset = int32(GetPositionX()+GetWidth())-(GetVisibleAreaX()+GetVisibleAreaW());
	}

	if(GetPositionY()+GetHeight() > GetVisibleAreaY()+GetVisibleAreaH())
	{
		height_offset = int32(GetPositionY()+GetHeight())-(GetVisibleAreaY()+GetVisibleAreaH());
	}

	if(mousex > GetPositionX()+m_iXOffset && mousex < GetPositionX()+GetWidth()-width_offset && mousey > GetPositionY()+m_iYOffset && mousey < GetPositionY() + GetHeight() - height_offset)
	{
		m_pLabel->CalculateState(mousex,mousey,mouse_button);
		SetState(m_pLabel->GetState());	
	}
	else
	{
		if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)
		{
			m_pLabel->SetState(CLabel::LABELSTATE::LABEL_STATE_NONE);
			SetState(CEdit::EDITSTATE::EDIT_STATE_NONE);	
		}
		if(mouse_button == 1)
		{
			m_pLabel->SetState(CLabel::LABELSTATE::LABEL_STATE_NONE);
			SetState(CEdit::EDITSTATE::EDIT_STATE_NONE);	
		}
	}

	
	return ret;
}

void CEdit::EnterChar()
{
	SetState(CEdit::EDITSTATE::EDIT_STATE_NONE);
}

void CEdit::SpaceChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	if(m_iMaxTextLength > m_sText.Size())
	{
		m_sText = m_sText.InsertStr(Str(" "),m_pLabel->GetCursorColumn());
		m_iTextChanged = 1;
		m_bInsertFlag = true;
	}
}

void CEdit::AddChar(Str ch)
{
	if(ch.Size() <= 0 || ch == Str("\\"))return;
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	
	if(m_iMaxTextLength > m_sText.Size())
	{
		m_sText = m_sText.InsertStr(ch,m_pLabel->GetCursorColumn());
		m_iTextChanged = 1;
		m_bInsertFlag = true;
	}
}

void CEdit::DeleteChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	
	if(m_sText.Size() > 0)
	{
		if(m_pLabel->GetCursorColumn() < (int32)m_sText.Size())
		{
			m_sText = m_sText.RemoveStr(m_pLabel->GetCursorColumn(),1);
			m_iTextChanged = 1;
		}
	}
}

void CEdit::BackspaceChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;

	if(m_sText.Size() > 0)
	{
		if(m_pLabel->GetCursorColumn() > 0)
		{
			m_sText = m_sText.RemoveStr(m_pLabel->GetCursorColumn()-1,1);
			CursorLeft();
			m_iTextChanged = 1;
		}
	}
}

void CEdit::CursorHome()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorHome();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	if(m_fFontXOffset < 0.0f && cursor < (Float32)GetLeftBorderWidth())
	{
		m_fFontXOffset += ((Float32)GetLeftBorderWidth() - cursor);
	}
	if(m_fFontXOffset < -m_pLabel->GetWidth())m_fFontXOffset = 0.0f;
}

void CEdit::CursorEnd()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorEnd();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	Float32 border = Float32(GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()-m_iTextSpace);
	if(cursor >= border)
	{
		m_fFontXOffset -= (cursor-border);
	}
}

void CEdit::CursorLeft()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorLeft();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	if(m_fFontXOffset < 0.0f && cursor < (Float32)GetLeftBorderWidth())
	{
		m_fFontXOffset += ((Float32)GetLeftBorderWidth() - cursor);
	}
	if(m_fFontXOffset < -m_pLabel->GetWidth())m_fFontXOffset = 0.0f;
}

void CEdit::CursorRight()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorRight();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	Float32 border = Float32(GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()-m_iTextSpace);
	if(cursor >= border)
	{
		m_fFontXOffset -= (cursor-border);
	}
}

CTextArea::CTextArea(uint32 type,Bool wrap,int32 width,int32 height,uint32 font_size,uint32 hscroll_width,uint32 vscroll_width,uint32 max_text_length,uint32 text_space,uint32 bettween_line_space,Str text,uint32 build_type,uint32 skin_id,CRender *render,CFont *font,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iTextSpace = text_space;
	m_iHeight = height;
	m_iWidth = width;
	m_iMaxTextLength = max_text_length;
	m_iGuiType = type;
	m_sText = text;
	m_iTextChanged = 1;
	m_bInsertFlag = false;
	m_bEndLineFlag = false;
	m_bWrap = wrap;

	m_iXOffset = 0;
	m_iYOffset = 0;
	m_iWidthOffset = 0;
	m_iHeightOffset = 0;

	m_iHScrollWidth = hscroll_width;
	m_iVScrollWidth = vscroll_width;
	m_iBettweenLineSpace = bettween_line_space;

	m_fFontXOffset = 0.0f;
	m_fFontYOffset = 0.0f;

	m_sText = text;

	if(m_iGuiType == CTextArea::TEXTAREATYPE::TEXTAREA_TYPE_READWRITE)
	{
		m_pLabel = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_DYNAMIC,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_EDITABLE_SELECTABLE,
			font_size,m_iTextSpace,m_iBettweenLineSpace,m_iMaxTextLength,m_sText,build_type,skin_id,render,font,gui_manager);
	}
	else
	{
		m_pLabel = new CLabel(CLabel::LABELTYPE::LABEL_TYPE_STATIC,CLabel::LABELEDITTYPE::LABEL_EDIT_TYPE_SELECTABLE,
			font_size,m_iTextSpace,m_iBettweenLineSpace,m_iMaxTextLength,m_sText,build_type,skin_id,render,font,gui_manager);
	}

	SetSkinId(skin_id);
	SetBuildType(build_type);
	
	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID)
	{
		SetTopBorderWidth(5);
		SetBottomBorderWidth(5);
		SetLeftBorderWidth(5);
		SetRightBorderWidth(5);
		//textarea
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea moved
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_OVER,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_PRESSED,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea readonly
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_READ_ONLY,m_iWidth,m_iHeight,render,gui_manager);	
	}
	else if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER)
	{
		int32 part_tl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tr_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_tr_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_bl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_bl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_br_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		int32 part_br_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);

		SetTopBorderWidth(part_tl_height);
		SetBottomBorderWidth(part_br_height);
		SetLeftBorderWidth(part_tl_width);
		SetRightBorderWidth(part_br_width);

		//textarea
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea moved
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_OVER,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea edit
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_PRESSED,m_iWidth,m_iHeight,render,gui_manager);	
		//textarea readonly
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT,CGuiManager::GUISKINTYPE::GUI_SKIN_EDIT_READ_ONLY,m_iWidth,m_iHeight,render,gui_manager);	
	}

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());

	m_pScrollArea = new CScrollArea(GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth(),GetHeight()-GetTopBorderWidth()-GetBottomBorderWidth(),m_iHScrollWidth,m_iVScrollWidth,m_iVScrollWidth,font_size,font_size,build_type,skin_id,render,gui_manager);
}

CTextArea::~CTextArea()
{
	SAFE_DELETE(m_pLabel)
	SAFE_DELETE(m_pScrollArea)
}

uint32 CTextArea::DrawFont(Float32 x,Float32 y,CRender *render,CFont *font)
{
	uint32 ret = 0;

	if(!InVisibleArea()) return ret;

	render->SetColor(GetFontColor().v[0],GetFontColor().v[1],GetFontColor().v[2],GetFontColor().v[3]);

	render->OnOffScissorsTest(1);

	m_iXOffset = GetVisibleAreaX() - (int32)x;
	if(m_iXOffset < 0)m_iXOffset = 0;

	m_iYOffset = GetVisibleAreaY() - (int32)y;
	if(m_iYOffset < 0)m_iYOffset = 0;

	m_iWidthOffset = int32((x-GetVisibleAreaX()) + m_iXOffset + GetWidth()) - GetVisibleAreaW();
	if(m_iWidthOffset < 0)m_iWidthOffset = 0;

	m_iHeightOffset = int32((y-GetVisibleAreaY()) + m_iYOffset + GetHeight()) - GetVisibleAreaH();
	if(m_iHeightOffset < 0)m_iHeightOffset = 0;

	if(GetWidth()-m_iWidthOffset-GetRightBorderWidth()-GetLeftBorderWidth() < 1 || GetHeight()-m_iHeightOffset < 1 ||
		GetVisibleAreaX()+1 > int32(x+GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()) ||
		GetVisibleAreaY()+1 > int32(y+GetHeight()-GetTopBorderWidth()-GetBottomBorderWidth()))
	{
		render->SetScissors(int32(x) + m_iXOffset + GetLeftBorderWidth(),
			int32(y) + m_iYOffset + GetTopBorderWidth(),
			1,1);
	}
	else
	{
		render->SetScissors(int32(x) + m_iXOffset + GetLeftBorderWidth(),
							int32(y) + m_iYOffset + GetTopBorderWidth(),
							GetWidth()-m_iWidthOffset-GetLeftBorderWidth()-GetRightBorderWidth()-m_iHScrollWidth + (int32(x-GetVisibleAreaX()) < 0 ? int32(x-GetVisibleAreaX()) : 0),
							GetHeight()-m_iHeightOffset-GetTopBorderWidth()-GetBottomBorderWidth()-m_iVScrollWidth + (int32(y-GetVisibleAreaY()) < 0 ? int32(y-GetVisibleAreaY()) : 0));
	}
		
	m_pLabel->SetLabelText(m_sText);
	m_fFontXOffset = -m_pScrollArea->GetContentXPos();
	m_fFontYOffset = -m_pScrollArea->GetContentYPos();
	m_pLabel->DrawFont(x + GetLeftBorderWidth()+m_fFontXOffset,y+GetTopBorderWidth()+m_fFontYOffset,render,font);
	m_pScrollArea->SetMaxInsideX((Float32)m_pLabel->GetWidth());
	m_pScrollArea->SetMaxInsideY((Float32)m_pLabel->GetHeight());
	
	if(m_bEndLineFlag)
	{
		CursorDown();
		m_bEndLineFlag = false;
	}
	if(m_bInsertFlag)
	{
		CursorRight();
		m_bInsertFlag = false;
	}
	render->OnOffScissorsTest(0);
	render->UnSetColor();

	return ret;
}

uint32 CTextArea::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());

	if(m_iGuiType == CTextArea::TEXTAREATYPE::TEXTAREA_TYPE_READONLY)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
	}
	else
	{
		if(GetState() == CTextArea::TEXTAREASTATE::TEXTAREA_STATE_EDIT)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
		}
		else if(GetState() == CTextArea::TEXTAREASTATE::TEXTAREA_STATE_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		}
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);

	

	render->OnOffScissorsTest(1);
	render->SetScissors((int32)x + m_iXOffset,(int32)y + m_iYOffset + GetTopBorderWidth(),GetWidth()-m_iWidthOffset-GetRightBorderWidth()-m_iVScrollWidth,GetHeight()-m_iHeightOffset-GetBottomBorderWidth()-GetTopBorderWidth()-m_iHScrollWidth);
	m_pLabel->Draw(x+GetLeftBorderWidth()+m_fFontXOffset,y+GetTopBorderWidth()+m_fFontYOffset,mousex,mousey,mouse_button,render);
	render->OnOffScissorsTest(0);

	m_pScrollArea->Draw(x+GetLeftBorderWidth(),y+GetTopBorderWidth(),mousex,mousey,mouse_button,render);

	return ret;
}

uint32 CTextArea::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret = 0;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;

	int32 width_offset = 0;
	int32 height_offset = 0;
	if(GetPositionX()+GetWidth() > GetVisibleAreaX()+GetVisibleAreaW())
	{
		width_offset = int32(GetPositionX()+GetWidth())-(GetVisibleAreaX()+GetVisibleAreaW());
	}

	if(GetPositionY()+GetHeight() > GetVisibleAreaY()+GetVisibleAreaH())
	{
		height_offset = int32(GetPositionY()+GetHeight())-(GetVisibleAreaY()+GetVisibleAreaH());
	}

	if(mousex > GetPositionX()+m_iXOffset && mousex < GetPositionX()+GetWidth()-width_offset && mousey > GetPositionY()+m_iYOffset && mousey < GetPositionY() + GetHeight() - height_offset)
	{
		m_pLabel->CalculateState(mousex,mousey,mouse_button);
		SetState(m_pLabel->GetState());
		m_pScrollArea->CalculateState(mousex,mousey,mouse_button);
	}
	else
	{
		if(GetState() != CTextArea::TEXTAREASTATE::TEXTAREA_STATE_EDIT)
		{
			m_pLabel->SetState(CLabel::LABELSTATE::LABEL_STATE_NONE);
			SetState(CTextArea::TEXTAREASTATE::TEXTAREA_STATE_NONE);	
		}
		if(mouse_button == 1)
		{
			m_pLabel->SetState(CLabel::LABELSTATE::LABEL_STATE_NONE);
			SetState(CTextArea::TEXTAREASTATE::TEXTAREA_STATE_NONE);	
		}
	}
	
	return ret;
}

void CTextArea::EnterChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	
	uint32 start = 0;
	for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
	{
		if(j>0)start += (m_pLabel->GetCursorRow() == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
		if(m_pLabel->GetCursorRow() == j)break;
	}
	if(m_iMaxTextLength > m_sText.Size())
	{
		m_sText = m_sText.InsertStr(Str("\\n"),start + m_pLabel->GetCursorColumn());
		m_iTextChanged = 1;
		m_pLabel->SetLabelText(m_sText);
		m_bEndLineFlag = true;
	}
}

void CTextArea::SpaceChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;

	uint32 start = 0;
	for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
	{
		if(j>0)start += (m_pLabel->GetCursorRow() == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
		if(m_pLabel->GetCursorRow() == j)break;
	}
	if(m_iMaxTextLength > m_sText.Size()+2)
	{
		m_sText = m_sText.InsertStr(Str(" "),start + m_pLabel->GetCursorColumn());
		m_iTextChanged = 1;
		m_bInsertFlag = true;
	}
}

void CTextArea::AddChar(Str ch)
{
	if(ch.Size() <= 0 || ch == Str("\\"))return;
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->AddChar(ch);
	uint32 start = 0;
	for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
	{
		if(j>0)start += (m_pLabel->GetCursorRow() == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
		if(m_pLabel->GetCursorRow() == j)break;
	}
	if(m_iMaxTextLength > m_sText.Size()+2)
	{
		m_sText = m_sText.InsertStr(ch,start + m_pLabel->GetCursorColumn());
		m_iTextChanged = 1;
		m_bInsertFlag = true;
	}
}

void CTextArea::DeleteChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	
	if(m_sText.Size() > 0)
	{
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_pLabel->GetCursorRow() == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_pLabel->GetCursorRow() == j)break;
		}
		if(start + m_pLabel->GetCursorColumn() < (int32)m_sText.Size())
		{
			if(m_pLabel->GetCursorColumn() >= (int32)m_sText.GetExpandPart(Str("\\n"),m_pLabel->GetCursorRow()).Size())
			{
				m_sText = m_sText.RemoveStr(start + m_pLabel->GetCursorColumn(),2);
			}
			else
			{
				m_sText = m_sText.RemoveStr(start + m_pLabel->GetCursorColumn(),1);
			}
			m_iTextChanged = 1;
		}
	}
}

void CTextArea::BackspaceChar()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;

	if(m_sText.Size() > 0)
	{
		uint32 start = 0;
		for(uint32 j=0;j<m_sText.GetExpandCount(Str("\\n"));j++)
		{
			if(j>0)start += (m_pLabel->GetCursorRow() == 0 ? 0 : m_sText.GetExpandPart(Str("\\n"),j-1).Size())+2;
			if(m_pLabel->GetCursorRow() == j)break;
		}
		if(start + m_pLabel->GetCursorColumn() > 0)
		{
			if(m_pLabel->GetCursorColumn() == 0)
			{
				int32 col_tmp = m_pLabel->GetCursorColumn();
				m_pLabel->CursorUp();
				m_pLabel->CursorEnd();
				m_sText = m_sText.RemoveStr(start + col_tmp-2,2);
			}
			else
			{
				m_sText = m_sText.RemoveStr(start + m_pLabel->GetCursorColumn()-1,1);
				CursorLeft();
			}
			
			m_iTextChanged = 1;
		}
	}
}

void CTextArea::CursorRight()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorRight();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	Float32 border = Float32(GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()-m_iTextSpace-m_iVScrollWidth);
	if(cursor >= border)
	{
		if(m_bWrap)
		{
			if(m_bInsertFlag)
			{
				CursorLeft();
				EnterChar();
			}
			else
			{
				m_pScrollArea->SetContentXPos(m_pScrollArea->GetContentXPos()+(cursor-border));
			}
			return;
		}

		m_pScrollArea->SetContentXPos(m_pScrollArea->GetContentXPos()+(cursor-border));
	}
}

void CTextArea::CursorLeft()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorLeft();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	if(m_fFontXOffset < 0.0f &&  cursor <= (Float32)GetLeftBorderWidth())
	{
		m_pScrollArea->SetContentXPos(m_pScrollArea->GetContentXPos()-((Float32)GetLeftBorderWidth()-cursor));
	}
	if(m_fFontXOffset < -m_pLabel->GetWidth())
	{
		m_fFontXOffset = 0.0f;
		m_pScrollArea->SetContentXPos(0.0f);
	}
}

void CTextArea::CursorUp()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorUp();
	Float32 cursor = m_pLabel->GetCursorY()+m_fFontYOffset;
	if(m_fFontYOffset < 0.0f &&  cursor <= (Float32)GetTopBorderWidth())
	{
		m_pScrollArea->SetContentYPos(m_pScrollArea->GetContentYPos()-((Float32)GetTopBorderWidth()-cursor));
	}
}

void CTextArea::CursorDown()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorDown();
	Float32 cursor = m_pLabel->GetCursorY()+m_fFontYOffset+m_pLabel->GetFontSize();
	Float32 border = Float32(GetHeight()-GetTopBorderWidth()-GetBottomBorderWidth()-m_iBettweenLineSpace-m_iHScrollWidth);
	if(cursor >= border)
	{
		m_pScrollArea->SetContentYPos(m_pScrollArea->GetContentYPos()+(cursor-border));
	}
}

void CTextArea::CursorHome()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorHome();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	if(m_fFontXOffset < 0.0f &&  cursor <= (Float32)GetLeftBorderWidth())
	{
		m_pScrollArea->SetContentXPos(m_pScrollArea->GetContentXPos()-((Float32)GetLeftBorderWidth()-cursor));
	}
}

void CTextArea::CursorEnd()
{
	if(GetState() != CEdit::EDITSTATE::EDIT_STATE_EDIT)return;
	m_pLabel->CursorEnd();
	Float32 cursor = m_pLabel->GetCursorX()+m_fFontXOffset;
	Float32 border = Float32(GetWidth()-GetLeftBorderWidth()-GetRightBorderWidth()-m_iTextSpace-m_iHScrollWidth);
	if(cursor >= border)
	{
		m_pScrollArea->SetContentXPos(m_pScrollArea->GetContentXPos()+(cursor-border));
	}
}


CCursor::CCursor(uint32 type,int32 width,int32 height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iWidth = width;
	m_iHeight = height;
	m_iGuiType = CCursor::CURSORTYPE::CURSOR_TYPE_ARROW;

	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	//default
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),type,type,m_iWidth,m_iHeight,render,gui_manager);	
	//edit
	AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),type,CGuiManager::GUISKINTYPE::GUI_SKIN_CURSOR_EDIT,2,m_iHeight,render,gui_manager);	

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CCursor::~CCursor()
{
}

uint32 CCursor::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;
	if(GetCursorType() == CCursor::CURSORTYPE::CURSOR_TYPE_NONE)return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D((Float32)mousex,(Float32)mousey,GetScaleX(),GetScaleY());
	
	if(GetCursorType() == CCursor::CURSORTYPE::CURSOR_TYPE_ARROW)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
	}
	else if(GetCursorType() == CCursor::CURSORTYPE::CURSOR_TYPE_EDIT)
	{
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
	}

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);

	return ret;
}

uint32 CCursor::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret=0;
	if(!InVisibleArea()) return ret;
	return ret;
}

CWindow::CWindow(int32 width,int32 height,int32 title_height,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{	
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iDeltaX = 0;
	m_iDeltaY = 0;
	m_bMovable = 0;
	m_bGrabFlag = 0;
	m_iTitleHeight = title_height;
	m_iWidth = width;
	m_iHeight = height;
	m_iGuiType = 0;

	SetSkinId(skin_id);
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID)
	{
		SetTopBorderWidth(1);
		SetBottomBorderWidth(1);
		SetLeftBorderWidth(1);
		SetRightBorderWidth(1);

		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,m_iWidth,m_iHeight,render,gui_manager);	
	}
	else if(GetBuildType() == CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_BORDER)
	{
		
		int32 part_tl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TL_CORNER);
		int32 part_tr_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_tr_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_TR_CORNER);
		int32 part_bl_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_bl_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BL_CORNER);
		int32 part_br_width = gui_manager->GetTextureWidth(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);
		int32 part_br_height = gui_manager->GetTextureHeight(GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINPART::GUI_SKIN_PART_BR_CORNER);

		SetTopBorderWidth(part_tl_height);
		SetBottomBorderWidth(part_br_height);
		SetLeftBorderWidth(part_tl_width);
		SetRightBorderWidth(part_br_width);

		m_iTitleHeight == 0 ? m_iTitleHeight = part_tl_height : m_iTitleHeight = title_height;
		
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,CGuiManager::GUISKINTYPE::GUI_SKIN_WINDOW,m_iWidth,m_iHeight,render,gui_manager);	
	}

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CWindow::~CWindow()
{
}

uint32 CWindow::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
	if(!GetMovedFromInit())
	{
		SetPositionX(x);
		SetPositionY(y);
	}

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(GetPositionX(),GetPositionY(),GetScaleX(),GetScaleY());
	
	render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);

	render->PopMatrix();
	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);

	return ret;
}

uint32 CWindow::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret=0;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;

	if(mouse_button == 0 && m_bGrabFlag == 1)
	{
		if(GetActive() == CGui::GUIACTIVE::GUI_GRAB)SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
		if(mousex+GetLeftBorderWidth()+1 > GetVisibleAreaX() && 
			mousex-GetRightBorderWidth()-1 < GetVisibleAreaX()+GetVisibleAreaW() &&
			mousey+GetTopBorderWidth()+1 > GetVisibleAreaY() &&
			mousey-GetBottomBorderWidth()-1 < GetVisibleAreaY()+GetVisibleAreaH())
		{
			SetPositionX(Float32(mousex-m_iDeltaX));
			SetPositionY(Float32(mousey-m_iDeltaY));
		}
		m_bGrabFlag = 0;
	}

	if(mouse_button == 0 && GetState() == CWindow::WINDOWSTATE::WINDOW_STATE_PRESSED)
	{
		SetState(CWindow::WINDOWSTATE::WINDOW_STATE_NONE);
	}

	if(mousex > GetPositionX() && mousex < GetPositionX() + GetWidth() && mousey > GetPositionY() && mousey < GetPositionY() + GetHeight())
	{
		ret = 1;
		SetState(CWindow::WINDOWSTATE::WINDOW_STATE_MOVED);
		if(mouse_button == 1)
		{
			SetState(CWindow::WINDOWSTATE::WINDOW_STATE_PRESSED);
		}
	}
	else if(GetState() == CWindow::WINDOWSTATE::WINDOW_STATE_MOVED)
	{
		SetState(CWindow::WINDOWSTATE::WINDOW_STATE_NONE);
	}

	if(m_bMovable == 1 && (GetActive() == CGui::GUIACTIVE::GUI_GRAB || GetActive() == CGui::GUIACTIVE::GUI_ACTIVE) && mousex > GetPositionX() && mousex < GetPositionX() + GetWidth() && mousey > GetPositionY() && mousey < GetPositionY()+m_iTitleHeight)
	{
		SetState(CWindow::WINDOWSTATE::WINDOW_STATE_TITLE_MOVED);
		if(mouse_button == 1 && m_bGrabFlag == 0)
		{
			SetActive(CGui::GUIACTIVE::GUI_GRAB);
			SetMovedFromInit(true);
			m_bGrabFlag = 1;
			m_iDeltaX = mousex - (int32)GetPositionX();
			m_iDeltaY = mousey - (int32)GetPositionY();
		}
	}
	else if(GetState() == CWindow::WINDOWSTATE::WINDOW_STATE_TITLE_MOVED)
	{
		SetState(CWindow::WINDOWSTATE::WINDOW_STATE_NONE);
	}
	
	if(m_bGrabFlag)
	{
		if(mousex+GetLeftBorderWidth()+1 > GetVisibleAreaX() && 
			mousex-GetRightBorderWidth()-1 < GetVisibleAreaX()+GetVisibleAreaW() &&
			mousey+GetTopBorderWidth()+1 > GetVisibleAreaY() &&
			mousey-GetBottomBorderWidth()-1 < GetVisibleAreaY()+GetVisibleAreaH())
		{
			SetPositionX(Float32(mousex-m_iDeltaX));
			SetPositionY(Float32(mousey-m_iDeltaY));
		}
	}

	return ret;
}

//CScrollArea

CScrollArea::CScrollArea(int32 width,int32 height,int32 hscroll_bar_width,int32 vscroll_bar_width,int32 min_scroll_button_size,int32 hbuttons_step,int32 vbuttons_step,uint32 build_type,uint32 skin_id,CRender *render,CGuiManager *gui_manager)
{
	SetVisibleArea(0,0,render->GetScreenWidth(),render->GetScreenHeight());
	m_iHeight = height;
	m_iWidth = width;
	m_iGuiType = 0;
	hbuttons_step == 0 ? m_iHButtonsStep = 1 : m_iHButtonsStep = hbuttons_step;
	vbuttons_step == 0 ? m_iVButtonsStep = 1 : m_iVButtonsStep = vbuttons_step;
	m_iHScrollBarWidth = hscroll_bar_width;
	m_iVScrollBarWidth = vscroll_bar_width;
	SetRightBorderWidth(vscroll_bar_width);
	SetBottomBorderWidth(hscroll_bar_width);
	hscroll_bar_width > 0 ? m_bEnableHScroll = true : m_bEnableHScroll = false;
	vscroll_bar_width > 0 ? m_bEnableVScroll = true : m_bEnableVScroll = false;
	m_fHScrollButtonPosition = 0.0f;
	m_fVScrollButtonPosition = 0.0f;
	m_iHSwitchHolder = 0;
	m_iVSwitchHolder = 0;
	m_fDeltaX = 0.0f;
	m_fDeltaY = 0.0f;
	m_fHButtonScale = 1.0f;
	m_fVButtonScale = 1.0f;
	m_iMinScrollButtonSize = min_scroll_button_size;

	if(m_bEnableHScroll && !m_bEnableVScroll)
	{
		m_iHBarWidth = m_iWidth-m_iHScrollBarWidth*2;
		m_iVBarHeight = m_iHeight-m_iHScrollBarWidth;

	}
	else if(!m_bEnableHScroll && m_bEnableVScroll)
	{
		m_iHBarWidth = m_iWidth-m_iVScrollBarWidth;
		m_iVBarHeight = m_iHeight-m_iVScrollBarWidth*2;
	}
	else
	{
		m_iHBarWidth = m_iWidth-m_iHScrollBarWidth*3;
		m_iVBarHeight = m_iHeight-m_iVScrollBarWidth*3;
	}
	
	SetSkinId(skin_id);
	build_type = CGuiManager::GUIBUILDTYPE::GUI_BUILD_TYPE_SOLID;
	SetBuildType(build_type);

	SetGeometryId(render->CreateGeometry());
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_POSITION,CRender::VERTEX_FORMAT_TYPE_FLOAT3);
	render->AddVertexDeclaration(GetGeometryId(),CRender::VERTEX_FORMAT_TEXCOORD,CRender::VERTEX_FORMAT_TYPE_FLOAT2);

	if(m_bEnableHScroll)
	{
		//hbar
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBAR,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBAR,m_iHBarWidth,GetBottomBorderWidth(),render,gui_manager);	
		//hbutton
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON,m_iHBarWidth,GetBottomBorderWidth(),render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON_OVER,m_iHBarWidth,GetBottomBorderWidth(),render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_HBUTTON_PRESSED,m_iHBarWidth,GetBottomBorderWidth(),render,gui_manager);	

		//left
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_OVER,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_LEFT_PRESSED,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
		//right
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_OVER,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_RIGHT_PRESSED,GetBottomBorderWidth(),GetBottomBorderWidth(),render,gui_manager);	
	}

	if(m_bEnableVScroll)
	{
		//vbar
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBAR,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBAR,GetRightBorderWidth(),m_iVBarHeight,render,gui_manager);	
		//vbutton
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON,GetRightBorderWidth(),m_iVBarHeight,render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON_OVER,GetRightBorderWidth(),m_iVBarHeight,render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_VBUTTON_PRESSED,GetRightBorderWidth(),m_iVBarHeight,render,gui_manager);	
		
		//up
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP_OVER,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_UP_PRESSED,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
		//down
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
		//over
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_OVER,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
		//pressed
		AddGuiQuadToGeometry(GetGeometryId(),GetBuildType(),GetSkinId(),CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN,CGuiManager::GUISKINTYPE::GUI_SKIN_SCROLL_AREA_BUTTON_DOWN_PRESSED,GetRightBorderWidth(),GetRightBorderWidth(),render,gui_manager);	
	}

	render->CreateGeometryVertexBuffer(GetGeometryId(),CRender::STATIC);
	render->LoadToVertexBufferGeometry(GetGeometryId());
	render->CreateGeometryIndexBuffer(GetGeometryId(),CRender::STATIC,CRender::INDEX_FORMAT16);
	render->LoadToIndexBufferGeometry(GetGeometryId());
}

CScrollArea::~CScrollArea()
{
}

uint32 CScrollArea::Draw(Float32 x,Float32 y,int32 mousex,int32 mousey,uint32 mouse_button,CRender *render)
{
	uint32 ret = 0;
		
	SetPositionX(x);
	SetPositionY(y);

	if(!InVisibleArea()) return ret;

	render->OnOffLighting(0);
	render->OnOffBlending(1);
	render->SetColor(GetColor().v[0],GetColor().v[1],GetColor().v[2],GetColor().v[3]);
	render->SetBlending(CRender::BLENDING::SRC_ALPHA,CRender::BLENDING::ONE_MINUS_SRC_ALPHA);
	render->SetDrawable(GetGeometryId());
	render->SetIndexBuffer(GetGeometryId(),0);
	render->OnOffDepthTest(0);
	render->SwitchTo2D();
	render->PushMatrix();
	
	render->TranslateScale2D(x,y,GetScaleX(),GetScaleY());
	
	if(!m_bEnableHScroll && m_bEnableVScroll)
	{
		render->AddTranslate2D((Float32)m_iHBarWidth+m_iHScrollBarWidth*2,0);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),5);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),6);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),4);
		}

		render->AddTranslate2D(0,(Float32)m_iVScrollBarWidth);
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);

		render->AddTranslate2D(0,(Float32)m_iVBarHeight);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),8);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),9);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),7);
		}
		
	}
	else if(m_bEnableHScroll && !m_bEnableVScroll)
	{
		render->AddTranslate2D((Float32)m_iHBarWidth+m_iHScrollBarWidth*2,(Float32)m_iVBarHeight);

		render->AddTranslate2D((Float32)-m_iHScrollBarWidth,(Float32)m_iVScrollBarWidth);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),8);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),9);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),7);
		}

		render->AddTranslate2D((Float32)-m_iHBarWidth,0);
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		
		render->AddTranslate2D((Float32)-m_iHScrollBarWidth,0);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),5);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),6);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),4);
		}
	}
	else
	{
		render->AddTranslate2D((Float32)m_iHBarWidth+m_iHScrollBarWidth*2,0);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),15);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),16);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),14);
		}

		render->AddTranslate2D(0,(Float32)m_iVScrollBarWidth);
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),10);

		render->AddTranslate2D(0,(Float32)m_iVBarHeight);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),18);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),19);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),17);
		}
		

		render->AddTranslate2D((Float32)-m_iHScrollBarWidth,(Float32)m_iVScrollBarWidth);

		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),8);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),9);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),7);
		}		

		render->AddTranslate2D((Float32)-m_iHBarWidth,0);
		render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),0);
		
		render->AddTranslate2D((Float32)-m_iHScrollBarWidth,0);
		if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_MOVED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),5);
		}
		else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_PRESSED)
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),6);
		}
		else
		{
			render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),4);
		}
	}

	render->PopMatrix();

	if(m_bEnableHScroll)
	{
		if(m_iHBarWidth * Float32(Float32(m_iHBarWidth)/(GetMaxInsideX() - m_iHScrollBarWidth*2)) >= m_iMinScrollButtonSize)
		{
			m_fHButtonScale = Float32(m_iHBarWidth)/(GetMaxInsideX()-m_iHScrollBarWidth*2);
		}

		m_fHScrollStep = Float32(GetMaxInsideX() - Float32(m_iHBarWidth+m_iHScrollBarWidth*2))/Float32(Float32(m_iHBarWidth)-Float32(Float32(m_iHBarWidth)*m_fHButtonScale));
		
		//Horisontal bar
		if(m_fHButtonScale < 1.0f)
		{
			if(m_iHSwitchHolder == 1)
			{
				m_fHScrollButtonPosition = mousex - m_fDeltaX;
				if(m_fHScrollButtonPosition <= 0.0f)
				{
					m_fHScrollButtonPosition = 0.0f;
				}
				else if(m_fHScrollButtonPosition >= m_iHBarWidth-(m_iHBarWidth*m_fHButtonScale))
				{
					m_fHScrollButtonPosition = m_iHBarWidth-(m_iHBarWidth*m_fHButtonScale);
				}
			}
			else
			{
				m_fHScrollButtonPosition = GetContentXPos()/m_fHScrollStep;
				if(m_fHScrollButtonPosition <= 0.0f)
				{
					m_fHScrollButtonPosition = 0.0f;
				}
				else if(m_fHScrollButtonPosition >= m_iHBarWidth-(m_iHBarWidth*m_fHButtonScale))
				{
					m_fHScrollButtonPosition = m_iHBarWidth-(m_iHBarWidth*m_fHButtonScale);
				}
			}
			
			SetContentXPos(m_fHScrollButtonPosition * m_fHScrollStep);
		}
	}

	if(m_bEnableVScroll)
	{
		if(m_iVBarHeight * Float32(Float32(m_iVBarHeight)/(GetMaxInsideY() - m_iVScrollBarWidth*2)) >= m_iMinScrollButtonSize)
		{
			m_fVButtonScale = Float32(m_iVBarHeight)/(GetMaxInsideY() - m_iVScrollBarWidth*2);
		}

		m_fVScrollStep = Float32(GetMaxInsideY() - Float32(m_iVBarHeight+m_iVScrollBarWidth*2))/Float32(Float32(m_iVBarHeight)-Float32(Float32(m_iVBarHeight)*m_fVButtonScale));
		
		//Vertical Bar
		if(m_fVButtonScale < 1.0f)
		{
			if(m_iVSwitchHolder == 1)
			{
				m_fVScrollButtonPosition = mousey - m_fDeltaY;

				if(m_fVScrollButtonPosition <= 0)
				{
					m_fVScrollButtonPosition = 0;
				}
				else if(m_fVScrollButtonPosition >= m_iVBarHeight-(m_iVBarHeight*m_fVButtonScale))
				{
					m_fVScrollButtonPosition = m_iVBarHeight-(m_iVBarHeight*m_fVButtonScale);
				}			
			}
			else
			{
				m_fVScrollButtonPosition = GetContentYPos()/m_fVScrollStep;
				if(m_fVScrollButtonPosition <= 0.0f)
				{
					m_fVScrollButtonPosition = 0.0f;
				}
				else if(m_fVScrollButtonPosition >= m_iVBarHeight-(m_iVBarHeight*m_fVButtonScale))
				{
					m_fVScrollButtonPosition = m_iVBarHeight-(m_iVBarHeight*m_fVButtonScale);
				}
			}
			
			SetContentYPos(m_fVScrollButtonPosition * m_fVScrollStep);
		}
	}

	if(!m_bEnableVScroll && m_bEnableHScroll)
	{
		if(m_fHButtonScale < 1.0f)
		{
			render->PushMatrix();		
			render->TranslateScale2D(x+m_fHScrollButtonPosition+m_iHScrollBarWidth,y+m_iVBarHeight+m_iVScrollBarWidth*2,m_fHButtonScale,1.0f);
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
			}
			else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
			}
			else
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
			}
			render->PopMatrix();
		}
	}
	else if(m_bEnableVScroll && !m_bEnableHScroll)
	{
		if(m_fVButtonScale < 1.0f)
		{
			render->PushMatrix();
			render->TranslateScale2D(x+m_iHBarWidth+m_iHScrollBarWidth*2,y+m_fVScrollButtonPosition+m_iVScrollBarWidth,1.0f,m_fVButtonScale);
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
			}
			else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
			}
			else
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
			}
			render->PopMatrix();
		}
	}
	else
	{
		if(m_fHButtonScale < 1.0f)
		{
			render->PushMatrix();		
			render->TranslateScale2D(x+m_fHScrollButtonPosition+m_iHScrollBarWidth,y+m_iVBarHeight+m_iVScrollBarWidth*2,m_fHButtonScale,1.0f);
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),2);
			}
			else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),3);
			}
			else
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),1);
			}
			render->PopMatrix();
		}

		if(m_fVButtonScale < 1.0f)
		{
			render->PushMatrix();
			render->TranslateScale2D(x+m_iHBarWidth+m_iHScrollBarWidth*2,y+m_fVScrollButtonPosition+m_iVScrollBarWidth,1.0f,m_fVButtonScale);
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),12);
			}
			else if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED)
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),13);
			}
			else
			{
				render->DrawGeometry(CRender::DRAW_PRIMITIVE_TRIANGLE_STRIP,GetGeometryId(),11);
			}
			render->PopMatrix();
		}
	}
	

	render->SwitchTo3D();
	render->OnOffDepthTest(1);
	render->UnSetColor();
	render->OnOffBlending(0);
	render->OnOffLighting(1);

	return ret;
}


uint32 CScrollArea::CalculateState(int32 mousex,int32 mousey,uint32 mouse_button)
{
	uint32 ret=0;

	if(GetActive() == CGui::GUIACTIVE::GUI_PASSIVE)return ret;
	if(!InVisibleArea()) return ret;
	if(mouse_button == 0)
	{
		m_iHSwitchHolder = 0;
		m_iVSwitchHolder = 0;
		if(GetActive() == CGui::GUIACTIVE::GUI_GRAB)SetActive(CGui::GUIACTIVE::GUI_ACTIVE);
	}

	if(mousex > GetPositionX() && mousex < GetPositionX() + (GetWidth()-GetRightBorderWidth()) && mousey > GetPositionY() && mousey < GetPositionY() + (GetHeight()-GetBottomBorderWidth()))
	{
		ret = 1;
	}


	int32 m1,m2 = 0;



	if(m_bEnableHScroll && m_bEnableVScroll)
	{
		m1 = 2;
		m2 = 3;
	}
	else
	{
		m1 = 0;
		m2 = 1;
	}
	if(m_bEnableHScroll)
	{
		//Left
		if(mousex > GetPositionX() && mousex < GetPositionX()+m_iHScrollBarWidth &&
					mousey > GetPositionY() + m_iVBarHeight+m_iHScrollBarWidth*m1 &&
					mousey < GetPositionY() + m_iVBarHeight+m_iHScrollBarWidth*m2)
		{
			SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_MOVED);
			if(mouse_button == 1)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_PRESSED);
				if(GetContentXPos() > 0)
				{
					SetContentXPos(GetContentXPos()-m_iHButtonsStep);
				}
			}
		}
		else
		{
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_MOVED ||
				GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_LEFT_PRESSED)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
			}
		}

		//Right
		if(mousex > GetPositionX()+m_iHBarWidth+m_iHScrollBarWidth && mousex < GetPositionX()+m_iHBarWidth+m_iHScrollBarWidth*2 &&
					mousey > GetPositionY()+m_iVBarHeight+m_iHScrollBarWidth*m1 && mousey < GetPositionY() + m_iVBarHeight+m_iHScrollBarWidth*m2)
		{
			SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_MOVED);
			if(mouse_button == 1)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_PRESSED);
				if(GetContentXPos() < (m_iHBarWidth-(m_iHBarWidth*m_fHButtonScale))*m_fHScrollStep)
				{
					SetContentXPos(GetContentXPos()+m_iHButtonsStep);
				}
			}
		}
		else
		{
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_MOVED ||
				GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_RIGHT_PRESSED)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
			}
		}

	}

	if(m_bEnableVScroll)
	{
		//Up
		if(mousex > GetPositionX()+m_iHBarWidth+m_iVScrollBarWidth*m1 && mousex < GetPositionX()+m_iHBarWidth+m_iVScrollBarWidth*m2 &&
			mousey > GetPositionY() && mousey < GetPositionY() + m_iVScrollBarWidth)
		{
			SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_MOVED);
			if(mouse_button == 1)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_PRESSED);
				if(GetContentYPos() > 0)
				{
					SetContentYPos(GetContentYPos()-m_iVButtonsStep);
				}
			}
		}
		else
		{
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_MOVED ||
				GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_UP_PRESSED)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
			}
		}

		//Down
		if(mousex > GetPositionX()+m_iHBarWidth+m_iVScrollBarWidth*m1 && mousex < GetPositionX()+m_iHBarWidth+m_iVScrollBarWidth*m2 &&
			mousey > GetPositionY()+m_iVBarHeight+m_iVScrollBarWidth && mousey < GetPositionY()+m_iVBarHeight+m_iVScrollBarWidth*2)
		{
			SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_MOVED);
			if(mouse_button == 1)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_PRESSED);
				if(GetContentYPos() < (m_iVBarHeight-(m_iVBarHeight*m_fVButtonScale))*m_fVScrollStep)
				{
					SetContentYPos(GetContentYPos()+m_iVButtonsStep);
				}
			}
		}
		else
		{
			if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_MOVED ||
				GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_DOWN_PRESSED)
			{
				SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
			}
		}
	}

	//Horisontal bar
	if(m_bEnableHScroll && !m_bEnableVScroll)
	{
		if(m_fHButtonScale < 1.0f)
		{
			if(m_iHSwitchHolder == 0)
			{
				if(mousex > GetPositionX()+m_fHScrollButtonPosition+m_iHScrollBarWidth && mousex < GetPositionX()+m_fHScrollButtonPosition+m_iHScrollBarWidth+(m_iHBarWidth*m_fHButtonScale) &&
					mousey > GetPositionY() + m_iVBarHeight && mousey < GetPositionY() + m_iVBarHeight+m_iHScrollBarWidth)
				{
					SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED);
					if(mouse_button == 1)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED);
						if(m_iHSwitchHolder == 0)
						{
							SetActive(CGui::GUIACTIVE::GUI_GRAB);
							m_fDeltaX = mousex - m_fHScrollButtonPosition;
							m_iHSwitchHolder = 1;
						}
					}
				}
				else
				{
					if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED ||
						GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
					}
				}
			}
		}
	}
	else if(m_bEnableVScroll && !m_bEnableHScroll)
	{
		//Vertical Bar
		if(m_fVButtonScale < 1.0f)
		{
			if(m_iVSwitchHolder == 0)
			{
				if(mousex > GetPositionX()+m_iHBarWidth && mousex < GetPositionX()+m_iHBarWidth+m_iVScrollBarWidth &&
					mousey > GetPositionY() + m_fVScrollButtonPosition+m_iVScrollBarWidth && mousey < GetPositionY() + m_fVScrollButtonPosition+m_iVScrollBarWidth+(m_iVBarHeight*m_fVButtonScale))
				{
					SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED);
					if(mouse_button == 1)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED);
						if(m_iVSwitchHolder == 0)
						{
							SetActive(CGui::GUIACTIVE::GUI_GRAB);
							m_fDeltaY = mousey - m_fVScrollButtonPosition;
							m_iVSwitchHolder = 1;
						}
					}
				}
				else
				{
					if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED ||
						GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
					}
				}
			}
		}
	}
	else
	{
		if(m_fHButtonScale < 1.0f)
		{
			if(m_iHSwitchHolder == 0)
			{
				if(mousex > GetPositionX()+m_fHScrollButtonPosition+m_iHScrollBarWidth && mousex < GetPositionX()+m_fHScrollButtonPosition+m_iHScrollBarWidth+(m_iHBarWidth*m_fHButtonScale) &&
					mousey > GetPositionY() + m_iVBarHeight + m_iVScrollBarWidth*2 && mousey < GetPositionY() + m_iVBarHeight+ + m_iVScrollBarWidth*3)
				{
					SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED);
					if(mouse_button == 1)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED);
						if(m_iHSwitchHolder == 0)
						{
							SetActive(CGui::GUIACTIVE::GUI_GRAB);
							m_fDeltaX = mousex - m_fHScrollButtonPosition;
							m_iHSwitchHolder = 1;
						}
					}
				}
				else
				{
					if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_MOVED ||
						GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_HBUTTON_PRESSED)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
					}
				}
			}
		}

		//Vertical Bar
		if(m_fVButtonScale < 1.0f)
		{
			if(m_iVSwitchHolder == 0)
			{
				if(mousex > GetPositionX()+m_iHBarWidth+m_iHScrollBarWidth*2 && mousex < GetPositionX()+m_iHBarWidth+m_iHScrollBarWidth*3 &&
					mousey > GetPositionY() + m_fVScrollButtonPosition+m_iVScrollBarWidth && mousey < GetPositionY() + m_fVScrollButtonPosition+m_iVScrollBarWidth+(m_iVBarHeight*m_fVButtonScale))
				{
					SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED);
					if(mouse_button == 1)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED);
						if(m_iVSwitchHolder == 0)
						{
							SetActive(CGui::GUIACTIVE::GUI_GRAB);
							m_fDeltaY = mousey - m_fVScrollButtonPosition;
							m_iVSwitchHolder = 1;
						}
					}
				}
				else
				{
					if(GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_MOVED ||
						GetState() == CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_VBUTTON_PRESSED)
					{
						SetState(CScrollArea::SCROLLAREASTATE::SCROLLAREA_STATE_NONE);
					}
				}
			}
		}
	}

	return ret;
}