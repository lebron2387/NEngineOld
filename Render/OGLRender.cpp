#include "OGLRender.h"
#include "../Common/Image.h"



// attrib arrays
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB = NULL;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB = NULL;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB = NULL;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB = NULL;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB = NULL;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB = NULL;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB = NULL;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB = NULL;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB = NULL;

// vertex buffer object
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;
PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;

//MultiTexture
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = NULL;
PFNGLMULTITEXCOORD1DARBPROC  glMultiTexCoord1d  = NULL;
PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dv = NULL;
PFNGLMULTITEXCOORD1FARBPROC  glMultiTexCoord1f  = NULL;
PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fv = NULL;
PFNGLMULTITEXCOORD1IARBPROC  glMultiTexCoord1i  = NULL;
PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1iv = NULL;
PFNGLMULTITEXCOORD1SARBPROC  glMultiTexCoord1s  = NULL;
PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1sv = NULL;
PFNGLMULTITEXCOORD2DARBPROC  glMultiTexCoord2d  = NULL;
PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dv = NULL;
PFNGLMULTITEXCOORD2FARBPROC  glMultiTexCoord2f  = NULL;
PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2IARBPROC  glMultiTexCoord2i  = NULL;
PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2iv = NULL;
PFNGLMULTITEXCOORD2SARBPROC  glMultiTexCoord2s  = NULL;
PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2sv = NULL;
PFNGLMULTITEXCOORD3DARBPROC  glMultiTexCoord3d  = NULL;
PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dv = NULL;
PFNGLMULTITEXCOORD3FARBPROC  glMultiTexCoord3f  = NULL;
PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fv = NULL;
PFNGLMULTITEXCOORD3IARBPROC  glMultiTexCoord3i  = NULL;
PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3iv = NULL;
PFNGLMULTITEXCOORD3SARBPROC  glMultiTexCoord3s  = NULL;
PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3sv = NULL;
PFNGLMULTITEXCOORD4DARBPROC  glMultiTexCoord4d  = NULL;
PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dv = NULL;
PFNGLMULTITEXCOORD4FARBPROC  glMultiTexCoord4f  = NULL;
PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fv = NULL;
PFNGLMULTITEXCOORD4IARBPROC  glMultiTexCoord4i  = NULL;
PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4iv = NULL;
PFNGLMULTITEXCOORD4SARBPROC  glMultiTexCoord4s  = NULL;
PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4sv = NULL;

//blending
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;

void InitExt()
{
	// attrib arrays
	GET_PROC_ADDRESS(PFNGLENABLEVERTEXATTRIBARRAYARBPROC,glEnableVertexAttribArrayARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIBPOINTERARBPROC,glVertexAttribPointerARB);
	GET_PROC_ADDRESS(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC,glDisableVertexAttribArrayARB);
	GET_PROC_ADDRESS(PFNGLGETATTRIBLOCATIONARBPROC,glGetAttribLocationARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB1FARBPROC,glVertexAttrib1fARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB2FVARBPROC,glVertexAttrib2fvARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB2FARBPROC,glVertexAttrib2fARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB3FVARBPROC,glVertexAttrib3fvARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB3FARBPROC,glVertexAttrib3fARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB4FVARBPROC,glVertexAttrib4fvARB);
	GET_PROC_ADDRESS(PFNGLVERTEXATTRIB4FARBPROC,glVertexAttrib4fARB);
	
	// vertex buffer object
	GET_PROC_ADDRESS(PFNGLGENBUFFERSARBPROC,glGenBuffersARB);
	GET_PROC_ADDRESS(PFNGLDELETEBUFFERSARBPROC,glDeleteBuffersARB);
	GET_PROC_ADDRESS(PFNGLBINDBUFFERARBPROC,glBindBufferARB);
	GET_PROC_ADDRESS(PFNGLBUFFERDATAARBPROC,glBufferDataARB);
	GET_PROC_ADDRESS(PFNGLBUFFERSUBDATAARBPROC,glBufferSubDataARB);
	GET_PROC_ADDRESS(PFNGLMAPBUFFERARBPROC,glMapBufferARB);
	GET_PROC_ADDRESS(PFNGLUNMAPBUFFERARBPROC,glUnmapBufferARB);
	GET_PROC_ADDRESS(PFNGLDRAWRANGEELEMENTSPROC,glDrawRangeElements);

	//MultiTexture
	GET_PROC_ADDRESS(PFNGLACTIVETEXTUREPROC,glActiveTexture);
	GET_PROC_ADDRESS(PFNGLCLIENTACTIVETEXTUREPROC,glClientActiveTexture);

	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1DARBPROC,glMultiTexCoord1d);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1DVARBPROC,glMultiTexCoord1dv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1FARBPROC ,glMultiTexCoord1f );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1FVARBPROC,glMultiTexCoord1fv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1IARBPROC ,glMultiTexCoord1i );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1IVARBPROC,glMultiTexCoord1iv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1SARBPROC ,glMultiTexCoord1s );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD1SVARBPROC,glMultiTexCoord1sv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2DARBPROC ,glMultiTexCoord2d );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2DVARBPROC,glMultiTexCoord2dv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2FARBPROC ,glMultiTexCoord2f );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2FVARBPROC,glMultiTexCoord2fv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2IARBPROC ,glMultiTexCoord2i );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2IVARBPROC,glMultiTexCoord2iv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2SARBPROC ,glMultiTexCoord2s );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD2SVARBPROC,glMultiTexCoord2sv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3DARBPROC ,glMultiTexCoord3d );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3DVARBPROC,glMultiTexCoord3dv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3FARBPROC ,glMultiTexCoord3f );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3FVARBPROC,glMultiTexCoord3fv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3IARBPROC ,glMultiTexCoord3i );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3IVARBPROC,glMultiTexCoord3iv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3SARBPROC ,glMultiTexCoord3s );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD3SVARBPROC,glMultiTexCoord3sv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4DARBPROC ,glMultiTexCoord4d );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4DVARBPROC,glMultiTexCoord4dv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4FARBPROC ,glMultiTexCoord4f );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4FVARBPROC,glMultiTexCoord4fv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4IARBPROC ,glMultiTexCoord4i );
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4IVARBPROC,glMultiTexCoord4iv);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4SARBPROC ,glMultiTexCoord4s);
	GET_PROC_ADDRESS(PFNGLMULTITEXCOORD4SVARBPROC,glMultiTexCoord4sv);

	//blending
	GET_PROC_ADDRESS(PFNGLBLENDEQUATIONPROC ,glBlendEquation);
	
}

COGLRender::COGLRender()
{
	m_iRenderName = OPENGL_RENDER;
	m_pDeviceContext = NULL;
	m_pRenderContext = NULL;
	m_pTextures = new TextureData[1];
	m_pGeometrys = new GeometryData[1];
	m_pLights = new LightData[1];
	m_pMaterials = new MaterialData[1];
	m_pShaders = new ShaderData[1];
	m_pShadersParameters = new ShaderParameterData[1];
	m_pCgVertexProgram = new CGprogram[1];
    m_pCgFragmentProgram = new CGprogram[1];
	m_pParameters = new CGparameter[MAX_CG_PARAMETERS];
	//Cg
	m_pCgContext = cgCreateContext();
	cgSetParameterSettingMode(m_pCgContext,CG_DEFERRED_PARAMETER_SETTING);

	//Removes Array
	m_pLightRemove = new uint32[1];
	m_pMaterialRemove = new uint32[1];
	m_pGeometryRemove = new uint32[1];
	m_pTextureRemove = new uint32[1];
	m_pShaderRemove = new uint32[1];
}

COGLRender::~COGLRender(void)
{
	for(uint32 i=0;i<m_iTexturesCount;i++)
	{
		glDeleteTextures(1,&m_pTextures[i].TextureId);
	}
	SAFE_DELETE_ARRAY(m_pTextures);

	//Geometrys
	for(uint32 i=0;i<m_iGeometrysCount;i++)
	{
		for(uint32 j=0;j<m_pGeometrys[i].NumIndexBuffers;j++)
		{
			glDeleteBuffersARB(1,&m_pGeometrys[i].pIndexBuffer[j]);
		}
		glDeleteBuffersARB(1,&m_pGeometrys[i].pVertexBuffer);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].Vertex);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].Normal);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].BiNormal);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].Tangent);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].TexCoords);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].Index);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].GeomertyVertices);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].GeomertyIndices);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].pIndexBuffer);
		SAFE_DELETE_ARRAY(m_pGeometrys[i].pVertextElement);
	}
	SAFE_DELETE_ARRAY(m_pGeometrys);
	//Lights
	SAFE_DELETE_ARRAY(m_pLights);
	//Materials
	SAFE_DELETE_ARRAY(m_pMaterials);

	m_pDeviceContext = NULL;
	m_pRenderContext = NULL;

	ZeroMemory( &dmScreenSettings, sizeof(dmScreenSettings) );

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(uint32 i=0;i<m_iCgShadersCount;i++)
		{
			cgDestroyProgram(m_pCgVertexProgram[i]);
			cgDestroyProgram(m_pCgFragmentProgram[i]);
		}
		
	}

	//Shaders
	SAFE_DELETE_ARRAY(m_pShadersParameters);
	SAFE_DELETE_ARRAY(m_pShaders);
	//Cg
	cgDestroyContext(m_pCgContext);
	SAFE_DELETE_ARRAY(m_pCgVertexProgram);
	SAFE_DELETE_ARRAY(m_pCgFragmentProgram);
	SAFE_DELETE_ARRAY(m_pParameters);

	//Removes Array
	SAFE_DELETE_ARRAY(m_pLightRemove);
	SAFE_DELETE_ARRAY(m_pMaterialRemove);
	SAFE_DELETE_ARRAY(m_pGeometryRemove);
	SAFE_DELETE_ARRAY(m_pTextureRemove);
	SAFE_DELETE_ARRAY(m_pShaderRemove);
}

Bool COGLRender::Init(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync,HWND hwnd)
{
	if(fullscreen)
	{
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenwidth;
		dmScreenSettings.dmPelsHeight = screenheight;
		dmScreenSettings.dmBitsPerPel = bpp;
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			/*
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
			*/
		}
	}

	unsigned int PixelFormat;
	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bpp,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 16Bit Z-Buffer (Depth Buffer)  
		8,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if(!(m_pDeviceContext=GetDC(hwnd)))
	{
		//MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DeInit(fullscreen,hwnd);
		return 0;
	}

	if(!(PixelFormat=ChoosePixelFormat(m_pDeviceContext,&pfd)))
	{
		//MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DeInit(fullscreen,hwnd);
		return 0;
	}

	if(!SetPixelFormat(m_pDeviceContext,PixelFormat,&pfd))
	{
		//MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DeInit(fullscreen,hwnd);
		return 0;
	}

	if(!(m_pRenderContext=wglCreateContext(m_pDeviceContext)))
	{
		//MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DeInit(fullscreen,hwnd);
		return 0;
	}

	if(!wglMakeCurrent(m_pDeviceContext,m_pRenderContext))
	{		
		//MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		DeInit(fullscreen,hwnd);
		return 0;
	}

	InitExt();

	SetViewport(0,0,screenwidth,screenheight);

	return 1;
};

void COGLRender::DeInit(Bool fullscreen,HWND hwnd)
{
	if(!fullscreen)
	{
		//ChangeDisplaySettings(NULL,0);
		ShowCursor(1);
	}

	if(m_pRenderContext)
	{
		if(!wglMakeCurrent(NULL,NULL))
		{
			//MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if(!wglDeleteContext(m_pRenderContext))
		{
			//MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		m_pRenderContext=NULL;
	}

	if (m_pDeviceContext && !ReleaseDC(hwnd,m_pDeviceContext))
	{
		//MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		m_pDeviceContext=NULL;
	}
}

void COGLRender::RenderStart()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
};

void COGLRender::Render()
{	
};

void COGLRender::RenderEnd()
{
	SwapBuffers(m_pDeviceContext);
};

void COGLRender::SetProjection(Float32 fov, Float32 aspect,Float32 znear,Float32 zfar)
{
	matProjection.perspective(fov,aspect,znear,zfar);
	glMatrixMode(GL_PROJECTION);						
	glLoadMatrixf(matProjection);									
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void COGLRender::SetLook(Vec3f pos,Vec3f up,Vec3f view)
{
	Mat4f matView;
	matView.look_at(pos,view,up);
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();
	glLoadMatrixf(matView);
}

void COGLRender::SetViewport(int32 x,int32 y,int32 width,int32 height)
{
	m_iScreenWidth = width;
	m_iScreenHeight = height;
	glViewport(x,y,width,height);
}

void COGLRender::PushMatrix()
{
	glPushMatrix();
}

void COGLRender::PopMatrix()
{
	glPopMatrix();
}
//Movind 2D
void COGLRender::Scale2D(Float32 x,Float32 y)
{
	glScalef(x,y,1.0f);
}

void COGLRender::AddScale2D(Float32 x,Float32 y)
{
	glScalef(x,y,1.0f);
}

void COGLRender::Rotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::AddRotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::Translate2D(Float32 x,Float32 y)
{
	glTranslatef(x,GetScreenHeight()-y,0.0f);
}

void COGLRender::AddTranslate2D(Float32 x,Float32 y)
{
	glTranslatef(x,-y,0.0f);
}

void COGLRender::TranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy)
{
	glTranslatef(x,GetScreenHeight()-y,0.0f);
	glScalef(sx,sy,1.0f);
}

void COGLRender::AddTranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy)
{
	glTranslatef(x,-y,0.0f);
	glScalef(sx,sy,1.0f);
}

void COGLRender::TranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glTranslatef(x,y,0.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::AddTranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glTranslatef(x,-y,0.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::ScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glScalef(sx,sy,1.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::AddScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glScalef(sx,sy,1.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::TranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glTranslatef(x,GetScreenHeight()-y,0.0f);
	glScalef(sx,sy,1.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::AddTranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glTranslatef(x,-y,0.0f);
	glScalef(sx,sy,1.0f);
	glRotatef(angle,axis_x,axis_y,axis_z);
}

//Moving3D

void COGLRender::Scale3D(Float32 x,Float32 y,Float32 z)
{
	glScalef(x,y,z);
}

void COGLRender::Translate3D(Float32 x,Float32 y,Float32 z)
{
	glTranslatef(x,y,z);
}

void COGLRender::TranslateScale3D(Float32 x,Float32 y,Float32 z,Float32 sx,Float32 sy,Float32 sz)
{
	glTranslatef(x,y,z);
	glScalef(sx,sy,sz);
}

void COGLRender::Rotate3D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	glRotatef(angle,axis_x,axis_y,axis_z);
}

void COGLRender::SetTransformMatrix(Mat4f mat)
{
	glMultMatrixf(mat);
}

//Textures
uint32 COGLRender::CreateTexture(TEXTURETYPE type,uint32 width,uint32 height,uint32 num_channels,uint32 format,uint32 depth,uint32 flags,uint32 mip_levels)
{
	Bool new_element = true;
	uint32 texture_id = 0;
	//Check destroyed textures
	for(uint32 i=0;i<m_iTexturesCount;i++)
	{
		if(m_pTextureRemove[i] == 0)
		{
			new_element = false;
			texture_id = i;
			break;
		}
	}

	if(new_element)
	{
		++m_iTexturesCount;
		m_pTextures = (TextureData*)ReAlloc(m_pTextures,m_iTexturesCount * sizeof(TextureData));
		m_pTextureRemove = (uint32*)ReAlloc(m_pTextureRemove,m_iTexturesCount * sizeof(uint32));
		m_pTextures[m_iTexturesCount-1].Init();
		texture_id = m_iTexturesCount-1;
	}
	m_pTextureRemove[texture_id] = 1;
	if(mip_levels == 0)mip_levels = 1;

	m_pTextures[texture_id].Type = type;
	m_pTextures[texture_id].Width = width;
	m_pTextures[texture_id].Height = height;
	m_pTextures[texture_id].Format = format;
	m_pTextures[texture_id].Depth = depth;

	//check number of mip levels
	Float32 temp_width = Float32(width < height ? width : height);
	if(temp_width > 1.0f)
	{
		for(uint32 i=1;i<mip_levels+1;i++)
		{
			if(temp_width == 1)
			{
				mip_levels = i-1;
				break;
			}
			temp_width*=0.5f;
		}
	}
	temp_width = 0.0f;

	m_pTextures[texture_id].NumMipLevels = mip_levels;
	m_pTextures[texture_id].NumChannels = num_channels;
	
	glGenTextures(1,&m_pTextures[texture_id].TextureId);
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),m_pTextures[texture_id].TextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_pTextures[texture_id].NumMipLevels - 1);
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),0);
	

	return texture_id;
}

void COGLRender::DestroyTexture(uint32 id)
{
	RemoveTexture(id);
	m_pTextureRemove[id] = 0;
}

uint32 COGLRender::RemoveTexture(uint32 texture_id)
{
	glDeleteTextures(1,&m_pTextures[texture_id].TextureId);
	return 1;
}

uint32 COGLRender::FillPartTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 offsetx,uint32 offsety,uint32 mip_width,uint32 mip_height)
{
	if(mip_level >= m_pTextures[texture_id].NumMipLevels)return 0;

	if(mip_width == 0)mip_width = m_pTextures[texture_id].Width;
	if(mip_height == 0)mip_height = m_pTextures[texture_id].Height;
	
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),m_pTextures[texture_id].TextureId);		
	switch(m_pTextures[texture_id].Type)
	{
		case CRender::TEXTURE_TYPE_1D:
			break;
		case CRender::TEXTURE_TYPE_2D:
				glTexSubImage2D(GetTextureType(m_pTextures[texture_id].Type), mip_level, offsetx,offsety,mip_width,mip_height,GetFormat(m_pTextures[texture_id].Format),GetDataType(m_pTextures[texture_id].Format),data);
			break;
		case CRender::TEXTURE_TYPE_3D:
			break;
		case CRender::TEXTURE_TYPE_CUBE:
			break;
	}
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),0);		
	return 0;
}

uint32 COGLRender::FillTexture(uint32 texture_id,uChar *data)
{
	return FillTexture(texture_id,data,0,0,0);
}

uint32 COGLRender::FillTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 mip_width,uint32 mip_height)
{
	if(mip_level >= m_pTextures[texture_id].NumMipLevels)return 0;

	if(mip_width == 0)mip_width = m_pTextures[texture_id].Width;
	if(mip_height == 0)mip_height = m_pTextures[texture_id].Height;
	
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),m_pTextures[texture_id].TextureId);		
	switch(m_pTextures[texture_id].Type)
	{
		case CRender::TEXTURE_TYPE_1D:
			break;
		case CRender::TEXTURE_TYPE_2D:
				glTexImage2D(GetTextureType(m_pTextures[texture_id].Type), mip_level, GetInternalFormat(m_pTextures[texture_id].Format),mip_width,mip_height,0,GetFormat(m_pTextures[texture_id].Format),GetDataType(m_pTextures[texture_id].Format),data);
			break;
		case CRender::TEXTURE_TYPE_3D:
			break;
		case CRender::TEXTURE_TYPE_CUBE:
			break;
	}
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),0);		
	
	return 1;
}

uint32 COGLRender::BindTexture(uint32 texture_id,uint32 texture_stage)
{
	glActiveTexture(GL_TEXTURE0 + texture_stage);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),m_pTextures[texture_id].TextureId);
	return 1;
}

uint32 COGLRender::UnBindTexture(uint32 texture_id,uint32 texture_stage)
{
	glActiveTexture(GL_TEXTURE0+texture_stage);
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),0);
	return 1;
}

uint32 COGLRender::GenerateMipMapLevels(uint32 texture_id,uint32 hardware)
{
	if(m_pTextures[texture_id].NumMipLevels == 0)return 0;

	if(hardware)
	{
		return 0;
	}

	uint32 w = m_pTextures[texture_id].Width;
	uint32 h = m_pTextures[texture_id].Height;
	uint8 *data = new uint8[w*h*m_pTextures[texture_id].NumChannels];

	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),m_pTextures[texture_id].TextureId);
	glGetTexImage(GetTextureType(m_pTextures[texture_id].Type),0,GetFormat(m_pTextures[texture_id].Format),GetDataType(m_pTextures[texture_id].Format),data);
	glBindTexture(GetTextureType(m_pTextures[texture_id].Type),0);

	CImage mip_img = CImage(m_pTextures[texture_id].Depth,m_pTextures[texture_id].Format,w,h,data);
	for(uint32 i=1;i<m_pTextures[texture_id].NumMipLevels+1;i++)
	{
		if(w == 1 || h == 1)break;
		w /= 2;
		h /= 2;
		mip_img.QuarterImage();
		FillTexture(texture_id,mip_img.GetData(),i,w,h);
	}

	return 1;
}

void COGLRender::SetTextureFilter(TEXTUREFILTER min_filter,TEXTUREFILTER mag_filter,TEXTUREFILTER mip_filter)
{
	switch(min_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		break;
		case TEXTURE_FILTER_LINEAR:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,2);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,4);
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,8);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);
		break;
	}

	switch(mag_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		break;
		case TEXTURE_FILTER_LINEAR:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,1);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,2);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,4);
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,8);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);
		break;
	}

	switch(mip_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
		break;
		case TEXTURE_FILTER_LINEAR:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		break;
	}
}

void COGLRender::SetTextureWrapState(TEXTUREWRAP wrap_s,TEXTUREWRAP wrap_t,TEXTUREWRAP wrap_r)
{
	switch(wrap_s)
	{
		case TEXTURE_WRAP_WRAP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		break;
		case TEXTURE_WRAP_CLAMP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
		break;
		case TEXTURE_WRAP_BORDER:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
		break;
	}

	switch(wrap_t)
	{
		case TEXTURE_WRAP_WRAP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		break;
		case TEXTURE_WRAP_CLAMP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
		break;
		case TEXTURE_WRAP_BORDER:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
		break;
	}

	switch(wrap_r)
	{
		case TEXTURE_WRAP_WRAP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
		break;
		case TEXTURE_WRAP_CLAMP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT);
		break;
		case TEXTURE_WRAP_BORDER:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_BORDER);
		break;
	}
}


//Geometry
uint32 COGLRender::AddVertexDeclaration(uint32 geometry_id,VERTEXFORMAT format,VERTEXFORMATTYPE type)
{
	++m_pGeometrys[geometry_id].NumVertexDeclAttrib;
	m_pGeometrys[geometry_id].pVertextElement = (VertexDeclaration*)ReAlloc(m_pGeometrys[geometry_id].pVertextElement,m_pGeometrys[geometry_id].NumVertexDeclAttrib*sizeof(VertexDeclaration));

	uint32 types_size [] = 
	{
		sizeof(Float32),
		sizeof(Float32)*2,
		sizeof(Float32)*3,
		sizeof(Float32)*4,
	};

	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Stream = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Offset = m_pGeometrys[geometry_id].VertexFormatShift;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Type = type;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Method = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Usage = format;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].UsageIndex = m_pGeometrys[geometry_id].AttribIndex[format]; 
	++m_pGeometrys[geometry_id].AttribIndex[format];

	m_pGeometrys[geometry_id].VertexFormatShift += types_size[type];

	return m_pGeometrys[geometry_id].NumVertexDeclAttrib - 1;
}


uint32 COGLRender::SetStartNewGeometry(uint32 geometry_id)
{
	++m_pGeometrys[geometry_id].GeomertyCount;
	
	m_pGeometrys[geometry_id].GeomertyVertices = (uint32*)ReAlloc(m_pGeometrys[geometry_id].GeomertyVertices,m_pGeometrys[geometry_id].GeomertyCount*sizeof(uint32));
	m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1] = m_pGeometrys[geometry_id].NumVertex;

	m_pGeometrys[geometry_id].GeomertyIndices = (uint32*)ReAlloc(m_pGeometrys[geometry_id].GeomertyIndices,m_pGeometrys[geometry_id].GeomertyCount*sizeof(uint32));
	m_pGeometrys[geometry_id].GeomertyIndices[m_pGeometrys[geometry_id].GeomertyCount-1] = m_pGeometrys[geometry_id].NumIndex;
	
	return 1;
}

uint32 COGLRender::CreateGeometry()
{
	Bool new_element = true;
	uint32 geometry_id = 0;
	//Check destroyed geometrys
	for(uint32 i=0;i<m_iGeometrysCount;i++)
	{
		if(m_pGeometryRemove[i] == 0)
		{
			new_element = false;
			geometry_id = i;
			break;
		}
	}

	if(new_element)
	{
		++m_iGeometrysCount;
		m_pGeometrys = (GeometryData*)ReAlloc(m_pGeometrys,m_iGeometrysCount * sizeof(GeometryData));
		m_pGeometryRemove = (uint32*)ReAlloc(m_pGeometryRemove,m_iGeometrysCount * sizeof(uint32));
		geometry_id = m_iGeometrysCount-1;
	}
	m_pGeometryRemove[geometry_id] = 1;
	m_pGeometrys[geometry_id].Init();
	return geometry_id;
}

void COGLRender::DestroyGeometry(uint32 id)
{
	ReleaseGeometryVertexBuffer(id);
	for(uint32 i=0;i<m_pGeometrys[id].NumIndexBuffers;i++)
	{
		ReleaseGeometryIndexBuffer(id,i);
	}
	m_pGeometrys[id].DeInit();
	m_pGeometryRemove[id] = 0;
}

uint32 COGLRender::CreateGeometryVertexBuffer(uint32 geometry_id,BUFFERACCESS access,uint32 num_vertex)
{
	if(num_vertex == 0)num_vertex = m_pGeometrys[geometry_id].NumVertex;

	m_pGeometrys[geometry_id].GlobalNumVertex = num_vertex;

	glGenBuffersARB(1,&m_pGeometrys[geometry_id].pVertexBuffer);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pVertexBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,num_vertex*m_pGeometrys[geometry_id].VertexFormatShift,0,GetBufferAccess(access));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	

	return 1;
}

uint32 COGLRender::ReleaseGeometryVertexBuffer(uint32 geometry_id)
{
	if(geometry_id < m_iGeometrysCount)
	{
		glDeleteBuffersARB(1,&m_pGeometrys[geometry_id].pVertexBuffer);
	}
	return 1;
}

uint32 COGLRender::CreateGeometryIndexBuffer(uint32 geometry_id,BUFFERACCESS access,INDEXFORMAT index_format,uint32 num_index)
{
	++m_pGeometrys[geometry_id].NumIndexBuffers;
	m_pGeometrys[geometry_id].pIndexBuffer = (uint32*)ReAlloc(m_pGeometrys[geometry_id].pIndexBuffer,m_pGeometrys[geometry_id].NumIndexBuffers*sizeof(uint32));
	uint32 index_buffer_id = m_pGeometrys[geometry_id].NumIndexBuffers-1;

	if(num_index == 0)num_index = m_pGeometrys[geometry_id].NumIndex;
	m_pGeometrys[geometry_id].IndexFormat = index_format;
	m_pGeometrys[geometry_id].GlobalNumIndex = num_index;

	glGenBuffersARB(1,&m_pGeometrys[geometry_id].pIndexBuffer[index_buffer_id]);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pIndexBuffer[index_buffer_id]);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,num_index*GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),0,GetBufferAccess(access));
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	return index_buffer_id;
}

uint32 COGLRender::ReleaseGeometryIndexBuffer(uint32 geometry_id,uint32 buffer_id)
{
	if(geometry_id < m_iGeometrysCount)
	{
		if(buffer_id < m_pGeometrys[geometry_id].NumIndexBuffers)
		{
			glDeleteBuffersARB(1,&m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]);
		}
	}
	return 1;
}


uint32 COGLRender::AddVerexToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
{
	m_pGeometrys[geometry_id].NumVertex += num;
	m_pGeometrys[geometry_id].Vertex = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Vertex,m_pGeometrys[geometry_id].NumVertex*sizeof(Vec3f));

	uint32 start_num = 0;
	if(m_pGeometrys[geometry_id].GeomertyCount == 0)
	{
		start_num = 0;
	}
	else
	{
		start_num = m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1];
	}

	if(object_id != -1)
	{
		object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyVertices[object_id-1]:start_num = 0;
	}

	uint32 i,j;
	for(i=0,j=start_num;i<num;i++,j++)
	{
		m_pGeometrys[geometry_id].Vertex[j] = data[i];
	}
	return 1;
}

uint32 COGLRender::AddTexCoordsToGeometry(uint32 geometry_id,uint32 num,Vec2f *data,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].NumVertex > 0)
	{
		m_pGeometrys[geometry_id].TexCoords = (Vec2f*)ReAlloc(m_pGeometrys[geometry_id].TexCoords,m_pGeometrys[geometry_id].NumVertex*sizeof(Vec2f));
		uint32 start_num = 0;
		if(m_pGeometrys[geometry_id].GeomertyCount == 0)
		{
			start_num = 0;
		}
		else
		{
			start_num = m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1];
		}

		if(object_id != -1)
		{
			object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyVertices[object_id-1]:start_num = 0;
		}

		uint32 i,j;
		for(i=0,j=start_num;i<num;i++,j++)
		{
			m_pGeometrys[geometry_id].TexCoords[j] = data[i];
		}
	}
	else
	{
		return 0;
	}
	return 1;
}

uint32 COGLRender::AddNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
{
	m_pGeometrys[geometry_id].NumNormal += num;
	m_pGeometrys[geometry_id].Normal = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Normal,m_pGeometrys[geometry_id].NumNormal*sizeof(Vec3f));

	uint32 start_num = 0;
	if(m_pGeometrys[geometry_id].GeomertyCount == 0)
	{
		start_num = 0;
	}
	else
	{
		start_num = m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1];
	}

	if(object_id != -1)
	{
		object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyVertices[object_id-1]:start_num = 0;
	}

	uint32 i,j;
	for(i=0,j=start_num;i<num;i++,j++)
	{
		m_pGeometrys[geometry_id].Normal[j] = data[i];
	}
	return 1;
}

uint32 COGLRender::AddBiNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
{
	m_pGeometrys[geometry_id].BiNormal = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].BiNormal,m_pGeometrys[geometry_id].NumNormal*sizeof(Vec3f));

	uint32 start_num = 0;
	if(m_pGeometrys[geometry_id].GeomertyCount == 0)
	{
		start_num = 0;
	}
	else
	{
		start_num = m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1];
	}

	if(object_id != -1)
	{
		object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyVertices[object_id-1]:start_num = 0;
	}

	uint32 i,j;
	for(i=0,j=start_num;i<num;i++,j++)
	{
		m_pGeometrys[geometry_id].BiNormal[j] = data[i];
	}
	return 1;
}

uint32 COGLRender::AddTangentToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
{
	m_pGeometrys[geometry_id].Tangent = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Tangent,m_pGeometrys[geometry_id].NumNormal*sizeof(Vec3f));

	uint32 start_num = 0;
	if(m_pGeometrys[geometry_id].GeomertyCount == 0)
	{
		start_num = 0;
	}
	else
	{
		start_num = m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1];
	}

	if(object_id != -1)
	{
		object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyVertices[object_id-1]:start_num = 0;
	}

	uint32 i,j;
	for(i=0,j=start_num;i<num;i++,j++)
	{
		m_pGeometrys[geometry_id].Tangent[j] = data[i];
	}
	return 1;
}

uint32 COGLRender::AddIndexToGeometry(uint32 geometry_id,uint32 num,int32 *data,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].NumVertex > 0)
	{
		uint32 start_num_vert = m_pGeometrys[geometry_id].NumIndex;
		m_pGeometrys[geometry_id].NumIndex += num;
		m_pGeometrys[geometry_id].Index = (int32*)ReAlloc(m_pGeometrys[geometry_id].Index,m_pGeometrys[geometry_id].NumIndex*sizeof(int32));
		uint32 i,j;
		uint32 start_num = 0;

		if(m_pGeometrys[geometry_id].GeomertyCount == 0)
		{
			start_num = 0;
		}
		else
		{
			start_num = m_pGeometrys[geometry_id].GeomertyIndices[m_pGeometrys[geometry_id].GeomertyCount-1];
		}

		if(object_id != -1)
		{
			object_id > 0 ? start_num = m_pGeometrys[geometry_id].GeomertyIndices[object_id-1]:start_num = 0;
		}

		for(i=0,j=start_num;i<num;i++,j++)
		{
			m_pGeometrys[geometry_id].Index[j] = data[i];
		}
	}
	else
	{
		return 0;
	}
	return 1;
}

uint32 COGLRender::LoadToVertexBufferGeometry(uint32 geometry_id,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].NumVertex <= 0)return 0;
	if(object_id > m_pGeometrys[geometry_id].GeomertyCount-1)return 0;

	uint32 element_num = m_pGeometrys[geometry_id].VertexFormatShift/sizeof(Float32);
	Float32 *data;
	uint32 iter = 0,start = 0,vsize = 0;
	uint32 l = object_id;
	
	if(l>0)
	{
		start = m_pGeometrys[geometry_id].GeomertyVertices[l-1];
		vsize = m_pGeometrys[geometry_id].GeomertyVertices[l]-m_pGeometrys[geometry_id].GeomertyVertices[l-1];
	}
	else
	{
		vsize = m_pGeometrys[geometry_id].GeomertyVertices[l];
	}
	
	data = new Float32[vsize*element_num];

	for(uint32 i=start,j=0;i<(start+vsize);i++,j+=element_num)
	{
		iter = 0;
		for(uint32 attr=0;attr<m_pGeometrys[geometry_id].NumVertexDeclAttrib;attr++)
		{
			if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_POSITION)
			{
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].x;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].y;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].z;
				++iter;
			}
			else if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_NORMAL)
			{
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].x;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].y;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].z;
				++iter;
			}
			else if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_TEXCOORD)
			{
				data[j+iter] = m_pGeometrys[geometry_id].TexCoords[i].x;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].TexCoords[i].y;
				++iter;
			}
		}
	}
    
	if(m_pGeometrys[geometry_id].NumVertex > m_pGeometrys[geometry_id].GlobalNumVertex)
	{
		return 0;
	}

	if(l==0)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pVertexBuffer);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,0,vsize*m_pGeometrys[geometry_id].VertexFormatShift,data);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	else
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pVertexBuffer);
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].GeomertyVertices[l-1]*m_pGeometrys[geometry_id].VertexFormatShift,vsize*m_pGeometrys[geometry_id].VertexFormatShift,data);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	}
	SAFE_DELETE_ARRAY(data);
	

	return 1;
}


uint32 COGLRender::LoadToVertexBufferGeometry(uint32 geometry_id)
{
	for(uint32 i=0;i<m_pGeometrys[geometry_id].GeomertyCount;i++)
	{	
		LoadToVertexBufferGeometry(geometry_id,i);
	}

	m_pGeometrys[geometry_id].NumVertex = 0;
	m_pGeometrys[geometry_id].NumNormal = 0;
	m_pGeometrys[geometry_id].Vertex = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Vertex,sizeof(Vec3f));
	m_pGeometrys[geometry_id].Normal = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Normal,sizeof(Vec3f));
	m_pGeometrys[geometry_id].BiNormal = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].BiNormal,sizeof(Vec3f));
	m_pGeometrys[geometry_id].Tangent = (Vec3f*)ReAlloc(m_pGeometrys[geometry_id].Tangent,sizeof(Vec3f));	
	m_pGeometrys[geometry_id].TexCoords = (Vec2f*)ReAlloc(m_pGeometrys[geometry_id].TexCoords,sizeof(Vec2f));
		
	
	return 1;
}

uint32 COGLRender::LoadToIndexBufferGeometry(uint32 geometry_id,uint32 object_id,uint32 buffer_id)
{
	if(buffer_id >m_pGeometrys[geometry_id].NumIndexBuffers-1)return 0;
	if(m_pGeometrys[geometry_id].NumIndex <= 0)return 0;
	if(object_id > m_pGeometrys[geometry_id].GeomertyCount-1)return 0;

	uint32 element_num = m_pGeometrys[geometry_id].VertexFormatShift/sizeof(FLOAT);
	uint16 *indices;
	uint32 iter = 0,start = 0,vstart = 0,isize = 0;
	uint32 l = object_id;
	

	if(l>0)
	{
		vstart = m_pGeometrys[geometry_id].GeomertyVertices[l-1];
		start = m_pGeometrys[geometry_id].GeomertyIndices[l-1];
		isize = m_pGeometrys[geometry_id].GeomertyIndices[l]-m_pGeometrys[geometry_id].GeomertyIndices[l-1];
	}
	else
	{
		vstart = 0;
		start = 0;
		isize = m_pGeometrys[geometry_id].GeomertyIndices[l];
	}

	indices = new uint16[isize];

	for(uint32 ii=start,jj=0;ii<(start+isize);ii++,jj++)
	{
		indices[jj] = vstart+m_pGeometrys[geometry_id].Index[ii];
	}
    
	if(m_pGeometrys[geometry_id].NumIndex > m_pGeometrys[geometry_id].GlobalNumIndex)
	{
		return 0;
	}

	if(l==0)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]);
		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0,isize*GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),indices);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	}
	else
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]);
		glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].GeomertyIndices[l-1] * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),isize*GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),indices);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0);
	}
	SAFE_DELETE_ARRAY(indices);
	

	return 1;
}

uint32 COGLRender::LoadToIndexBufferGeometry(uint32 geometry_id)
{
	for(uint32 i=0;i<m_pGeometrys[geometry_id].GeomertyCount;i++)
	{
		for(uint32 j=0;j<m_pGeometrys[geometry_id].NumIndexBuffers;j++)
		{
			LoadToIndexBufferGeometry(geometry_id,i,j);
		}
	}

	m_pGeometrys[geometry_id].NumIndex = 0;
	m_pGeometrys[geometry_id].Index = (int32*)ReAlloc(m_pGeometrys[geometry_id].Index,sizeof(int32));	
	return 1;
}

uint32 COGLRender::SetDrawable(uint32 geometry_id)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pVertexBuffer);

	for(uint32 attr=0;attr<m_pGeometrys[geometry_id].NumVertexDeclAttrib;attr++)
	{
		if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_POSITION)
		{
			glVertexPointer(3,GL_FLOAT,m_pGeometrys[geometry_id].VertexFormatShift,(uint32*)m_pGeometrys[geometry_id].pVertextElement[attr].Offset);
		}

		if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_NORMAL)
		{
			glNormalPointer(GL_FLOAT,m_pGeometrys[geometry_id].VertexFormatShift,(uint32*)m_pGeometrys[geometry_id].pVertextElement[attr].Offset);
		}

		if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == VERTEX_FORMAT_TEXCOORD)
		{
			for(uint32 i=0;i<MAX_TEXTURES;i++)
			{
				glClientActiveTexture(GL_TEXTURE0 + i);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2,GL_FLOAT,m_pGeometrys[geometry_id].VertexFormatShift,(uint32*)m_pGeometrys[geometry_id].pVertextElement[attr].Offset);
			}
		}
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
	
	return 1;
}

uint32 COGLRender::SetIndexBuffer(uint32 geometry_id,uint32 index_buffer_id)
{
	if(m_pGeometrys[geometry_id].GlobalNumIndex>0)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,m_pGeometrys[geometry_id].pIndexBuffer[index_buffer_id]);
	}
	return 1;
}

uint32 COGLRender::DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id)
{
	for(uint32 i=0;i<m_pGeometrys[geometry_id].GeomertyCount;i++)
	{
		DrawGeometry(primitive_type,geometry_id,i);
	}
	
	return 1;
}

uint32 COGLRender::DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].GlobalNumIndex>0)
	{
		if(object_id == 0)
		{
			glDrawElements(GetPrimitiveType(primitive_type),m_pGeometrys[geometry_id].GeomertyIndices[object_id],GetIndexFormat(m_pGeometrys[geometry_id].IndexFormat),0);
		}
		else
		{
			glDrawElements(GetPrimitiveType(primitive_type),m_pGeometrys[geometry_id].GeomertyIndices[object_id]-m_pGeometrys[geometry_id].GeomertyIndices[object_id-1],GetIndexFormat(m_pGeometrys[geometry_id].IndexFormat),(Char*)(m_pGeometrys[geometry_id].GeomertyIndices[object_id-1]*GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat)));
		}
	}

	return 1;
}

//Lights

uint32 COGLRender::CreateLight()
{
	Bool new_element = true;
	uint32 light_id = 0;
	//Check destroyed geometrys
	for(uint32 i=0;i<m_iLightCount;i++)
	{
		if(m_pLightRemove[i] == 0)
		{
			new_element = false;
			light_id = i;
			break;
		}
	}

	if(new_element)
	{
		++m_iLightCount;
		m_pLights = (LightData*)ReAlloc(m_pLights,m_iLightCount * sizeof(LightData));
		m_pLightRemove = (uint32*)ReAlloc(m_pLightRemove,m_iLightCount * sizeof(uint32));
		light_id = m_iLightCount-1;
	}
	m_pLightRemove[light_id] = 1;
	return light_id;
}

void COGLRender::DestroyLight(uint32 id)
{
	m_pLightRemove[id] = 0;
}

uint32 COGLRender::SetLight(uint32 light_id)
{
	return 1;
}

void COGLRender::SetLightEnable(uint32 light_id)
{
	glEnable(GL_LIGHT0+light_id);
}

void COGLRender::SetLightDisable(uint32 light_id)
{
	glDisable(GL_LIGHT0+light_id);
}

void COGLRender::SetLightType(uint32 light_id,LIGHTTYPE Type)
{
	m_pLights[light_id].Type = Type;
}

void COGLRender::SetLightDiffuse(uint32 light_id,Vec4f Diffuse)
{
	m_pLights[light_id].Diffuse = Diffuse;
	glLightfv(GL_LIGHT0+light_id,GL_DIFFUSE,Diffuse.v);
}

void COGLRender::SetLightSpecular(uint32 light_id,Vec4f Specular)
{
	m_pLights[light_id].Specular = Specular;
	glLightfv(GL_LIGHT0+light_id,GL_SPECULAR,Specular.v);
}

void COGLRender::SetLightAmbient(uint32 light_id,Vec4f Ambient)
{
	m_pLights[light_id].Ambient = Ambient;
	glLightfv(GL_LIGHT0+light_id,GL_AMBIENT,Ambient.v);
}

void COGLRender::SetLightPosition(uint32 light_id,Vec3f Position)
{
	m_pLights[light_id].Position = Position;
	glLightfv(GL_LIGHT0+light_id,GL_POSITION,Vec4f(m_pLights[light_id].Position.v[0],m_pLights[light_id].Position.v[1],m_pLights[light_id].Position.v[2],1));
}

void COGLRender::SetLightDirection(uint32 light_id,Vec3f Direction)
{
	m_pLights[light_id].Direction = Direction;
	glLightfv(GL_LIGHT0+light_id,GL_SPOT_DIRECTION,Vec4f(m_pLights[light_id].Direction.v[0],m_pLights[light_id].Direction.v[1],m_pLights[light_id].Direction.v[2],1));
}

void COGLRender::SetLightAttenuation0(uint32 light_id,Float32 Attenuation0)
{
	m_pLights[light_id].Attenuation0 = Attenuation0;
	glLightfv(GL_LIGHT0+light_id,GL_CONSTANT_ATTENUATION,&Attenuation0);
}

void COGLRender::SetLightAttenuation1(uint32 light_id,Float32 Attenuation1)
{
	m_pLights[light_id].Attenuation1 = Attenuation1;
	glLightfv(GL_LIGHT0+light_id,GL_LINEAR_ATTENUATION,&Attenuation1);
}

void COGLRender::SetLightAttenuation2(uint32 light_id,Float32 Attenuation2)
{
	m_pLights[light_id].Attenuation2 = Attenuation2;
	glLightfv(GL_LIGHT0+light_id,GL_QUADRATIC_ATTENUATION,&Attenuation2);
}

void COGLRender::SetLightCutoff(uint32 light_id,Float32 Cutoff)
{
	m_pLights[light_id].Cutoff = Cutoff;
	glLightfv(GL_LIGHT0+light_id,GL_SPOT_CUTOFF,&Cutoff);
}

void COGLRender::SetLightExponent(uint32 light_id,Float32 Exponent)
{
	m_pLights[light_id].Exponent = Exponent;
	glLightfv(GL_LIGHT0+light_id,GL_SPOT_EXPONENT,&Exponent);
}

//Materials
uint32 COGLRender::CreateMaterial()
{
	Bool new_element = true;
	uint32 material_id = 0;
	//Check destroyed geometrys
	for(uint32 i=0;i<m_iMaterialsCount;i++)
	{
		if(m_pMaterialRemove[i] == 0)
		{
			new_element = false;
			material_id = i;
			break;
		}
	}

	if(new_element)
	{
		++m_iMaterialsCount;
		m_pMaterials = (MaterialData*)ReAlloc(m_pMaterials,m_iMaterialsCount * sizeof(MaterialData));
		m_pMaterialRemove = (uint32*)ReAlloc(m_pMaterialRemove,m_iMaterialsCount * sizeof(uint32));
		material_id = m_iMaterialsCount-1;
	}
	m_pMaterialRemove[material_id] = 1;
	
	return material_id;
}

void COGLRender::DestroyMaterial(uint32 id)
{
	m_pMaterialRemove[id] = 0;
}

uint32 COGLRender::SetMaterial(uint32 material_id)
{
	return 1;
}

void COGLRender::SetMaterialAmbient(uint32 material_id,Vec4f Ambient)
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,Ambient.v);
	
}

void COGLRender::SetMaterialDiffuse(uint32 material_id,Vec4f Diffuse)
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,Diffuse.v);
}

void COGLRender::SetMaterialSpecular(uint32 material_id,Vec4f Specular)
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular.v);
}

void COGLRender::SetMaterialEmissive(uint32 material_id,Vec4f Emissive)
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emissive.v);
}

void COGLRender::SetMaterialPower(uint32 material_id,Float32 Power)
{
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&Power);
}

//Shaders
uint32 COGLRender::CreateShader(Str vertex_shader_text,Str vertex_profile,Str fragment_shader_text,Str fragment_profile)
{
    uint32 shader_id = 0;
	Bool new_element = true;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		//Check destroyed geometrys
		for(uint32 i=0;i<m_iCgShadersCount;i++)
		{
			if(m_pShaderRemove[i] == 0)
			{
				new_element = false;
				shader_id = i;
				break;
			}
		}

		if(new_element)
		{
			m_iCgShadersCount++;
			shader_id = m_iCgShadersCount - 1;
			m_pCgVertexProgram = (CGprogram*)ReAlloc(m_pCgVertexProgram,m_iCgShadersCount+sizeof(CGprogram));
			m_pCgFragmentProgram = (CGprogram*)ReAlloc(m_pCgFragmentProgram,m_iCgShadersCount+sizeof(CGprogram));
			m_pShaderRemove = (uint32*)ReAlloc(m_pShaderRemove,m_iCgShadersCount * sizeof(uint32));
		}
		m_pShaderRemove[shader_id] = 1;

		if(vertex_profile.Size() == 0)
		{
			m_pCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		}
		else
		{
			SetShaderProfile(vertex_profile,Str(""));
		}

		if(fragment_profile.Size() == 0)
		{
			m_pCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
		}
		else
		{
			SetShaderProfile(Str(""),fragment_profile);
		}

		if(vertex_shader_text.Size() > 0  && cgIsProfileSupported(m_pCgVertexProfile) && cgGetProfileProperty(m_pCgVertexProfile,CG_IS_VERTEX_PROFILE) && (cgGetProfileProperty(m_pCgVertexProfile,CG_IS_GLSL_PROFILE) || cgGetProfileProperty(m_pCgVertexProfile,CG_IS_OPENGL_PROFILE)))
		{
			cgGLSetOptimalOptions(m_pCgVertexProfile);
			m_pCgVertexProgram[shader_id] = cgCreateProgram(m_pCgContext,CG_SOURCE,vertex_shader_text,m_pCgVertexProfile,NULL,NULL);		
			GetShaderLastError(shader_id,Str("Create Vertex Program"));
		}

		if(fragment_shader_text.Size() > 0 && cgIsProfileSupported(m_pCgFragmentProfile) && cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_FRAGMENT_PROFILE) && (cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_GLSL_PROFILE) || cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_OPENGL_PROFILE)))
		{
			cgGLSetOptimalOptions(m_pCgFragmentProfile);
			m_pCgFragmentProgram[shader_id] = cgCreateProgram(m_pCgContext,CG_SOURCE,fragment_shader_text,m_pCgFragmentProfile,NULL,NULL);
			GetShaderLastError(shader_id,Str("Create Fragment Program"));
		}
	}
	return shader_id;
}

void COGLRender::DestroyShader(uint32 id)
{
	UnLoadShader(id);
	ReleaseShader(id);
	m_pShaderRemove[id] = 0;
}

void COGLRender::ReleaseShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgDestroyProgram(m_pCgVertexProgram[shader_id]);
		cgDestroyProgram(m_pCgFragmentProgram[shader_id]);
	}
}

void COGLRender::LoadShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgGLLoadProgram(m_pCgVertexProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Load Vertex Program"));
		cgGLLoadProgram(m_pCgFragmentProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Load Fragment Program"));
	}
}

void COGLRender::UnLoadShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgGLUnloadProgram(m_pCgVertexProgram[shader_id]);
		cgGLUnloadProgram(m_pCgFragmentProgram[shader_id]);
	}
}

void COGLRender::BindShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgGLEnableProfile(m_pCgVertexProfile);
		GetShaderLastError(shader_id,Str("Enable Profile Vertex Program"));
		cgGLEnableProfile(m_pCgFragmentProfile);
		GetShaderLastError(shader_id,Str("Enable Profile Fragment Program"));
		cgGLBindProgram(m_pCgVertexProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Bind Vertex Program"));
		cgGLBindProgram(m_pCgFragmentProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Bind Fragment Program"));
	}
}

void COGLRender::UnBindShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgGLDisableProfile(m_pCgVertexProfile);
		cgGLDisableProfile(m_pCgFragmentProfile);
		cgGLUnbindProgram(m_pCgVertexProfile);
		cgGLUnbindProgram(m_pCgFragmentProfile);		
	}
}

uint32 COGLRender::AddShaderVertexParameter(uint32 shader_id,Str name_in_shader)
{
	uint32 parameter_id = 0;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return 0;
		m_iCgParametersCount++;
		parameter_id = m_iCgParametersCount - 1;
		m_pParameters[parameter_id] = cgGetNamedParameter(m_pCgVertexProgram[shader_id], name_in_shader);
		GetShaderLastError(shader_id,Str("Get Parameter Vertex Program"));
	}
	return parameter_id;
}

uint32 COGLRender::AddShaderFragmentParameter(uint32 shader_id,Str name_in_shader)
{
	uint32 parameter_id = 0;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return 0;
		m_iCgParametersCount++;
		parameter_id = m_iCgParametersCount - 1;
		m_pParameters[parameter_id] = cgGetNamedParameter(m_pCgFragmentProgram[shader_id], name_in_shader);
		GetShaderLastError(shader_id,Str("Get Parameter Fragment Program"));
	}
	return parameter_id;
}

void COGLRender::GetShaderLastError(uint32 shader_id,Str situation)
{
	if(!m_bShadersDebugMode)return;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if(error != CG_NO_ERROR && error != 1000) 
		{
			Trace("%s:%s\n",situation.GetData(),string);
			if(error == CG_COMPILER_ERROR) 
			{
				Trace("%s\n", cgGetLastListing(m_pCgContext));
			}
		}
	}
}

void COGLRender::SetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameteric(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::SetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameterfc(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::SetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameterdc(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::GetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameteric(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::GetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameterfc(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::GetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameterdc(m_pParameters[parameter_id],matrix);
	}
}

void COGLRender::SetShaderParameter1i(uint32 shader_id,uint32 parameter_id,int32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1i(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1iv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter1f(uint32 shader_id,uint32 parameter_id,Float32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1f(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1fv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter1d(uint32 shader_id,uint32 parameter_id,Double32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1d(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1dv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter2i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2i(m_pParameters[parameter_id],param1,param2);
	}
}

void COGLRender::SetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2iv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter2f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2f(m_pParameters[parameter_id],param1,param2);
	}
}

void COGLRender::SetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2fv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter2d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2d(m_pParameters[parameter_id],param1,param2);
	}
}

void COGLRender::SetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2dv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter3i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3i(m_pParameters[parameter_id],param1,param2,param3);
	}
}

void COGLRender::SetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3iv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter3f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3f(m_pParameters[parameter_id],param1,param2,param3);
	}
}

void COGLRender::SetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3fv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter3d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3d(m_pParameters[parameter_id],param1,param2,param3);
	}
}

void COGLRender::SetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3dv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter4i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3,int32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4i(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
}

void COGLRender::SetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4iv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter4f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3,Float32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4f(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
}

void COGLRender::SetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4fv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::SetShaderParameter4d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3,Double32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4d(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
}

void COGLRender::SetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4dv(m_pParameters[parameter_id],param);
	}
}

void COGLRender::GetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],1,param);
	}
}
void COGLRender::GetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],1,param);
	}
}
void COGLRender::GetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],1,param);
	}
}
void COGLRender::GetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],2,param);
	}
}
void COGLRender::GetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],2,param);
	}
}
void COGLRender::GetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],2,param);
	}
}
void COGLRender::GetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],3,param);
	}
}
void COGLRender::GetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],3,param);
	}
}
void COGLRender::GetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],3,param);
	}
}
void COGLRender::GetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],4,param);
	}
}
void COGLRender::GetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],4,param);
	}
}
void COGLRender::GetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],4,param);
	}
}

void COGLRender::SetShaderParameterTexture(uint32 shader_id,uint32 parameter_id,uint32 texture_id)
{
	if(texture_id >= m_iTexturesCount)return;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGLSetTextureParameter(m_pParameters[parameter_id],m_pTextures[texture_id].TextureId);
	}
}

Str COGLRender::GetShaderSupportedVertexProfiles()
{
	Str ret = Str("");

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(int32 i=0; i<cgGetNumSupportedProfiles(); i++)
		{
			if(cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_VERTEX_PROFILE) && (cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_GLSL_PROFILE) || cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_OPENGL_PROFILE)))
			{
				if(ret.Size() == 0)
				{
					ret += Str(cgGetProfileString(cgGetSupportedProfile(i)));
				}
				else
				{
					ret += Str(",")+Str(cgGetProfileString(cgGetSupportedProfile(i)));
				}
			}
		}
	}

	return ret;
}

Str COGLRender::GetShaderSupportedFragmentProfiles()
{
	Str ret = Str("");

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(int32 i=0; i<cgGetNumSupportedProfiles(); i++)
		{
			if(cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_FRAGMENT_PROFILE) && (cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_GLSL_PROFILE) || cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_OPENGL_PROFILE)))
			{
				if(ret.Size() == 0)
				{
					ret += Str(cgGetProfileString(cgGetSupportedProfile(i)));
				}
				else
				{
					ret += Str(",")+Str(cgGetProfileString(cgGetSupportedProfile(i)));
				}
			}
		}
	}

	return ret;
}

void COGLRender::SetShaderProfile(Str vertex_profile,Str fragment_profile)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(vertex_profile.Size() > 0)
		{
			if(cgIsProfileSupported(cgGetProfile(vertex_profile)))
			{
				m_pCgVertexProfile = cgGetProfile(vertex_profile);
			}
			else
			{
				m_pCgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);			
			}
		}

		if(fragment_profile.Size() > 0)
		{
			if(cgIsProfileSupported(cgGetProfile(fragment_profile)))
			{
				m_pCgFragmentProfile = cgGetProfile(fragment_profile);
			}
			else
			{
				m_pCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
			}
		}
	}
}

void COGLRender::SwitchTo2D()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_iScreenWidth, 0, m_iScreenHeight,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void COGLRender::SwitchTo3D()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
//Debug Draw
void COGLRender::DrawPoint(Vec3f point,Float32 size)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex3fv(point);
	glEnd();
}

void COGLRender::DrawLine(Vec3f start,Vec3f end)
{
	glBegin(GL_LINES);
	glVertex3fv(start);
	glVertex3fv(end);
	glEnd();
}

void COGLRender::DrawTriangle(Vec3f v0,Vec3f v1,Vec3f v2,Vec3f n)
{
	glBegin(GL_TRIANGLES);
	glNormal3fv(n);
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);
	glEnd();
}

//Transforms states

Mat4f COGLRender::GetTransformStateMatrix(TRANSFORMSTATES state)
{
	Mat4f ret;
	switch(state)
	{
		case VIEW:
			glGetFloatv(GL_MODELVIEW_MATRIX,ret);
			break;
		case PROJECTION:
			glGetFloatv(GL_PROJECTION_MATRIX,ret);
			break;
		case TEXTURE0:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE1:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE2:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE3:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE4:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE5:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE6:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
		case TEXTURE7:
			glGetFloatv(GL_TEXTURE_MATRIX,ret);
			break;
	}
	return ret;
}

void COGLRender::SetTransformStateMatrix(TRANSFORMSTATES state,Mat4f mat)
{
	switch(state)
	{
		case VIEW:
			glMatrixMode(GL_MODELVIEW_MATRIX);
			glLoadMatrixf(mat);
			break;
		case PROJECTION:
			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(mat);
			break;
		case TEXTURE0:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE1:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE2:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE3:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE4:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE5:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE6:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
		case TEXTURE7:
			glMatrixMode(GL_TEXTURE_MATRIX);
			glLoadMatrixf(mat);
			break;
	}
}

void COGLRender::SetBlending(BLENDING src,BLENDING dst)
{
	GLenum s = 0,d = 0;
	switch(src)
	{
		case ZERO:
			s = GL_ZERO;
			break;
		case ONE:
			s = GL_ONE;
			break;
		case SRC_COLOR:
			s = GL_SRC_COLOR;
			break;
		case ONE_MINUS_SRC_COLOR:
			s = GL_ONE_MINUS_SRC_COLOR;
			break;
		case SRC_ALPHA:
			s = GL_SRC_ALPHA;
			break;
		case ONE_MINUS_SRC_ALPHA:
			s = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case DST_ALPHA:
			s = GL_DST_ALPHA;
			break;
		case ONE_MINUS_DST_ALPHA:
			s = GL_ONE_MINUS_DST_ALPHA;
			break;
	}

	switch(dst)
	{
		case ZERO:
			d = GL_ZERO;
			break;
		case ONE:
			d = GL_ONE;
			break;
		case SRC_COLOR:
			d = GL_SRC_COLOR;
			break;
		case ONE_MINUS_SRC_COLOR:
			d = GL_ONE_MINUS_SRC_COLOR;
			break;
		case SRC_ALPHA:
			d = GL_SRC_ALPHA;
			break;
		case ONE_MINUS_SRC_ALPHA:
			d = GL_ONE_MINUS_SRC_ALPHA;
			break;
		case DST_ALPHA:
			d = GL_DST_ALPHA;
			break;
		case ONE_MINUS_DST_ALPHA:
			d = GL_ONE_MINUS_DST_ALPHA;
			break;
	}

	glBlendFunc(s,d);
}

void COGLRender::SetBlendingOp(BLENDING data)
{
	switch(data)
	{
		case ADD:
			glBlendEquation(GL_FUNC_ADD);
			break;
		case SUBTRACT:
			glBlendEquation(GL_FUNC_SUBTRACT);
			break;
		case REVERSE_SUBTRACT:
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
		case MIN:
			glBlendEquation(GL_MIN);
			break;
		case MAX:
			glBlendEquation(GL_MAX);
			break;
	}
}

void COGLRender::SetBlendingConstantColor(uint32 r,uint32 g,uint32 b,uint32 a)
{
	//glBlendColorEXT
}

void COGLRender::SetShadeModel(SHADEMODEL data)
{
	switch(data)
	{
		case FLAT:
			glShadeModel(GL_FLAT);
			break;
		case SMOOTH:
			glShadeModel(GL_SMOOTH);
			break;
	}
}

void COGLRender::SetDepthTest(DEPTHTEST data)
{
	switch(data)
	{
		case NEVER:
			glDepthFunc(GL_NEVER);
			break;
		case LESS:
			glDepthFunc(GL_LESS);
			break;
		case EQUAL:
			glDepthFunc(GL_EQUAL);
			break;
		case LEQUAL:
			glDepthFunc(GL_LEQUAL);
			break;
		case GREATER:
			glDepthFunc(GL_GREATER);
			break;
		case NOTEQUAL:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case GEQUAL:
			glDepthFunc(GL_GEQUAL);
			break;
		case ALWAYS:
			glDepthFunc(GL_ALWAYS);
			break;
	}
}

void COGLRender::SetCulling(CULLING data)
{
	switch(data)
	{
		case CULL_BACK:
			glCullFace(GL_BACK);
			break;
		case CULL_FRONT:
			glCullFace(GL_FRONT);
			break;
	};
}

void COGLRender::SetFillMode(FILLMODE data)
{
	switch(data)
	{
		case SOLID:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		case WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case POINT:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			break;
	};
}

void COGLRender::SetColor(Float32 r,Float32 g,Float32 b,Float32 a)
{
	glColor4f(r,g,b,Float32(1.0f/255.0f)*a);
}

void COGLRender::UnSetColor()
{
	glColor4f(255,255,255,255);
}

void COGLRender::OnOffBlending(Bool state)
{
	switch(state)
	{
		case 0:
			glDisable(GL_BLEND);
			break;
		case 1:
			glEnable(GL_BLEND);
			break;
	}
}

void COGLRender::OnOffCulling(Bool state)
{
	switch(state)
	{
		case 0:
			glDisable(GL_CULL_FACE);
			break;
		case 1:
			glEnable(GL_CULL_FACE);	
			break;
	}
}

void COGLRender::OnOffScissorsTest(Bool state)
{
	switch(state)
	{
		case 0:
			glDisable(GL_SCISSOR_TEST);
			break;
		case 1:
			glEnable(GL_SCISSOR_TEST);	
			break;
	}
}

void COGLRender::SetScissors(int32 x,int32 y,int32 w,int32 h)
{
	glScissor(x,GetScreenHeight()-y-h,w,h);
}

void COGLRender::OnOffDepthTest(Bool state)
{
	switch(state)
	{
		case 0:
			{
				glDisable(GL_DEPTH_TEST);
				glDepthMask(0);
			}
			break;
		case 1:
			{
				glDepthMask(1);
				glEnable(GL_DEPTH_TEST);
			}
			break;
	}
}

void COGLRender::OnOffLighting(Bool state)
{
	switch(state)
	{
		case 0:
			glDisable(GL_LIGHTING);
			break;
		case 1:
			glEnable(GL_LIGHTING);
			break;
	}
}

uint32 COGLRender::GetTextureType(TEXTURETYPE type)
{
	uint32 ret = 0;
	switch(type)
	{
		case TEXTURE_TYPE_1D:
			ret = GL_TEXTURE_1D;
			break;
		case TEXTURE_TYPE_2D:
			ret = GL_TEXTURE_2D;
			break;
		case TEXTURE_TYPE_3D:
			ret = GL_TEXTURE_3D;
			break;
		case TEXTURE_TYPE_CUBE:
			ret = GL_TEXTURE_2D;
			break;
		
	}
	return ret;
}

uint32 COGLRender::GetPrimitiveType(DRAWPRIMITIVE types)
{
	uint32 ret = 0;
	switch(types)
	{
		case DRAW_PRIMITIVE_TRIANGLE_LIST:
			ret = GL_TRIANGLES;
			break;
		case DRAW_PRIMITIVE_TRIANGLE_STRIP:
			ret = GL_TRIANGLE_STRIP;
			break;
		case DRAW_PRIMITIVE_LINES:
			ret = GL_LINES;
			break;
		case DRAW_PRIMITIVE_LINES_STRIP:
			ret = GL_LINE_STRIP;
			break;
		case DRAW_PRIMITIVE_POINTS:
			ret = GL_POINTS;
			break;
	}
	return ret;
}

uint32 COGLRender::GetIndexFormat(INDEXFORMAT format)
{
	uint32 ret = 0;
	switch(format)
	{
		case INDEX_FORMAT16:
			ret = GL_UNSIGNED_SHORT;
			break;
		case INDEX_FORMAT32:
			ret = GL_UNSIGNED_INT;
			break;
	}
	return ret;
}

uint32 COGLRender::GetIndexFormatSize(INDEXFORMAT format)
{
	uint32 ret = 0;
	switch(format)
	{
		case INDEX_FORMAT16:
			ret = sizeof(short);
			break;
		case INDEX_FORMAT32:
			ret = sizeof(int);
			break;
	}
	return ret;
}

uint32 COGLRender::GetBufferAccess(BUFFERACCESS data)
{
	uint32 ret = 0;
	switch(data)
	{
	case STATIC:
		ret = GL_STATIC_DRAW;
		break;
	case DYNAMIC:
		ret = GL_DYNAMIC_DRAW;
		break;
	case STREAM:
		ret = GL_STREAM_DRAW;
		break;

	}
	return ret;
}

uint32 COGLRender::GetInternalFormat(uint32 format)
{
	GLint internal_formats[] = 
	{
		0,
		// Unsigned formats
		GL_INTENSITY8,
		GL_LUMINANCE8_ALPHA8,
		GL_RGB,
		GL_RGBA,

		GL_INTENSITY16,
		GL_LUMINANCE16_ALPHA16,
		GL_RGB16,
		GL_RGBA16,

		// Signed formats
		0,
		0,
		0,
		0,

		0,
		0,
		0,
		0,

		// Float formats
		0, //GL_INTENSITY_FLOAT16_ATI,
		0, //GL_LUMINANCE_ALPHA_FLOAT16_ATI,
		0, //GL_RGB_FLOAT16_ATI,
		0, //GL_RGBA_FLOAT16_ATI,

		0, //GL_INTENSITY_FLOAT32_ATI,
		0, //GL_LUMINANCE_ALPHA_FLOAT32_ATI,
		0, //GL_RGB_FLOAT32_ATI,
		0, //GL_RGBA_FLOAT32_ATI,

		// Signed integer formats
		0, // GL_INTENSITY16I_EXT,
		0, // GL_LUMINANCE_ALPHA16I_EXT,
		0, // GL_RGB16I_EXT,
		0, // GL_RGBA16I_EXT,

		0, // GL_INTENSITY32I_EXT,
		0, // GL_LUMINANCE_ALPHA32I_EXT,
		0, // GL_RGB32I_EXT,
		0, // GL_RGBA32I_EXT,

		// Unsigned integer formats
		0, // GL_INTENSITY16UI_EXT,
		0, // GL_LUMINANCE_ALPHA16UI_EXT,
		0, // GL_RGB16UI_EXT,
		0, // GL_RGBA16UI_EXT,

		0, // GL_INTENSITY32UI_EXT,
		0, // GL_LUMINANCE_ALPHA32UI_EXT,
		0, // GL_RGB32UI_EXT,
		0, // GL_RGBA32UI_EXT,

		// Packed formats
		0, // RGBE8 not directly supported
		0, // GL_RGB9_E5,
		0, // GL_R11F_G11F_B10F,
		GL_RGB5,
		GL_RGBA4,
		GL_RGB10_A2,

		// Depth formats
		GL_DEPTH_COMPONENT16,
		GL_DEPTH_COMPONENT24,
		0, //GL_DEPTH24_STENCIL8_EXT,
		0, // GL_DEPTH_COMPONENT32F,

		// Compressed formats
		0, //GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
		0, //GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
		0, //GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
		0, // ATI1N not yet supported
		0, //GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI,
	};
	return internal_formats[format];
};

uint32 COGLRender::GetFormat(uint32 format)
{
	GLenum formats[] = { 0, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };
	return formats[format];
}

uint32 COGLRender::GetDataType(uint32 format)
{
	GLenum data_types[] = 
	{
		0,
		// Unsigned formats
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,

		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_SHORT,

		// Signed formats
		0,
		0,
		0,
		0,

		0,
		0,
		0,
		0,

		// Float formats
		0,//GL_HALF_FLOAT_ARB,
		0,//GL_HALF_FLOAT_ARB,
		0,//GL_HALF_FLOAT_ARB,
		0,//GL_HALF_FLOAT_ARB,

		GL_FLOAT,
		GL_FLOAT,
		GL_FLOAT,
		GL_FLOAT,

		// Signed integer formats
		0,
		0,
		0,
		0,

		0,
		0,
		0,
		0,

		// Unsigned integer formats
		0,
		0,
		0,
		0,

		0,
		0,
		0,
		0,

		// Packed formats
		0, // RGBE8 not directly supported
		0, // RGBE9E5 not supported
		0, // RG11B10F not supported
		GL_UNSIGNED_SHORT_5_6_5,
		GL_UNSIGNED_SHORT_4_4_4_4_REV,
		GL_UNSIGNED_INT_2_10_10_10_REV,

		// Depth formats
		GL_UNSIGNED_SHORT,
		GL_UNSIGNED_INT,
		0, //GL_UNSIGNED_INT_24_8_EXT,
		0, // D32F not supported

		// Compressed formats
		0,
		0,
		0,
		0,
		0,
	};
	return data_types[format];
}