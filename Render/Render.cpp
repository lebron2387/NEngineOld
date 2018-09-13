#include "Render.h"

CRender::CRender(void)
{
	m_iShaderLanguage = 0;
	m_iRenderType = SW_RENDER;
	m_iGeometrysCount = 0;
	m_iLightCount = 0;
	m_iMaterialsCount = 0;
	m_iTexturesCount = 0;
	m_iCgShadersCount = 0;
	m_iShadersCount = 0;
	m_iCgParametersCount = 0;
	m_iParametersCount = 0;
}