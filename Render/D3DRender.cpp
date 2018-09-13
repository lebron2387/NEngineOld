/*
*/

#include "D3DRender.h"
#include "../Common/Image.h"

/*
*/

CD3D9Render::CD3D9Render()
{
	m_iRenderName = DIRECT3D9_RENDER;
	m_pRenderContext = NULL; 
	m_pDeviceContext = NULL;
	m_pTextures = new TextureData[1];
	m_pGeometrys = new GeometryData[1];
	m_pLights = new LightData[1];
	m_pMaterials = new MaterialData[1];
	m_pShaders = new ShaderData[1];
	m_pShaderParameters = new ShaderParameterData[1];
	m_pCgVertexProgram = new CGprogram[1];
    m_pCgFragmentProgram = new CGprogram[1];
	m_pParameters = new CGparameter[MAX_CG_PARAMETERS];
	D3DXCreateMatrixStack(0,&m_pMatrixStack);
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

/*
*/

CD3D9Render::~CD3D9Render(void)
{
	//Textures
	for(uint32 i=0;i<m_iTexturesCount;i++)
	{
		SAFE_RELEASE(m_pTextures[i].TextureId);
	}
	SAFE_DELETE_ARRAY(m_pTextures);
	//Geometrys
	for(uint32 i=0;i<m_iGeometrysCount;i++)
	{
		for(uint32 j=0;j<m_pGeometrys[i].NumIndexBuffers;j++)
		{
			SAFE_RELEASE(m_pGeometrys[i].pIndexBuffer[j]);
		}
		SAFE_RELEASE(m_pGeometrys[i].pVertexBuffer);
		SAFE_RELEASE(m_pGeometrys[i].pVertexDecl);
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
		SAFE_DELETE_ARRAY(m_pGeometrys[i].pVertexDecl);
	}
	SAFE_DELETE_ARRAY(m_pGeometrys);
	//Lights
	SAFE_DELETE_ARRAY(m_pLights);
	//Materials
	SAFE_DELETE_ARRAY(m_pMaterials);

	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pRenderContext);
	ZeroMemory( &d3dDisplayMode, sizeof(d3dDisplayMode) );
	ZeroMemory( &d3dPresentParameters, sizeof(d3dPresentParameters) );

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(uint32 i=0;i<m_iCgShadersCount;i++)
		{
			cgDestroyProgram(m_pCgVertexProgram[i]);
			cgDestroyProgram(m_pCgFragmentProgram[i]);
		}
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		//Shaders
		for(uint32 i=0;i<m_iShadersCount;i++)
		{
			SAFE_RELEASE(m_pShaders[i].m_pVertexShader);
			SAFE_RELEASE(m_pShaders[i].m_pConstantTableVS);
			SAFE_RELEASE(m_pShaders[i].m_pFragmentShader);
			SAFE_RELEASE(m_pShaders[i].m_pConstantTableFS);
			SAFE_RELEASE(m_pShaders[i].m_pBufferErrors);

		}
	}
	//Shaders
	SAFE_DELETE_ARRAY(m_pShaderParameters);
	SAFE_DELETE_ARRAY(m_pShaders);
	//Cg
	cgDestroyContext(m_pCgContext);
	cgD3D9SetDevice(NULL);
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

/*
*/

Bool CD3D9Render::Init(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync,HWND hwnd)
{
	m_pRenderContext = Direct3DCreate9( D3D_SDK_VERSION );
	
    m_pRenderContext->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3dDisplayMode);

	m_pRenderContext->GetAdapterIdentifier(D3DADAPTER_DEFAULT,0,&d3dAdapterIdentifier);

    ZeroMemory( &d3dPresentParameters, sizeof(d3dPresentParameters) );
    d3dPresentParameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
    d3dPresentParameters.BackBufferFormat = d3dDisplayMode.Format;
	d3dPresentParameters.EnableAutoDepthStencil = true;
    d3dPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;

	if(!fullscreen)
	{
		d3dPresentParameters.Windowed = true;
	}
	else
	{
		d3dPresentParameters.Windowed=false;
		d3dPresentParameters.BackBufferWidth	= screenwidth;
		d3dPresentParameters.BackBufferHeight	= screenheight;
		d3dPresentParameters.BackBufferCount	= 2;
	}

	if(!vsync)
	{
		d3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	if( FAILED( m_pRenderContext->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps ) ) )
	{
		if( m_pRenderContext != NULL )m_pRenderContext->Release();
		return false;
	}

	DWORD dwBehaviorFlags = 0;

	if(m_iRenderType == CRender::HW_RENDER)
	{
		dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if( FAILED( m_pRenderContext->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, dwBehaviorFlags,&d3dPresentParameters, &m_pDeviceContext )))
		{
			return false;
		}
	}
	else if(m_iRenderType == CRender::SW_RENDER)
	{
		dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		if( FAILED( m_pRenderContext->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, dwBehaviorFlags,&d3dPresentParameters, &m_pDeviceContext )))
		{
			return false;
		}
	}

	cgD3D9SetDevice(m_pDeviceContext);

	SetViewport(0,0,screenwidth,screenheight);

	return true;
}

/*
*/

void CD3D9Render::DeInit(Bool fullscreen,HWND hwnd)
{
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pRenderContext);
}

/*
*/

void CD3D9Render::RenderStart()
{
	m_pDeviceContext->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0 );
	m_pDeviceContext->BeginScene();
}

void CD3D9Render::Render()
{
}

void CD3D9Render::RenderEnd()
{
	m_pDeviceContext->EndScene();
    m_pDeviceContext->Present( NULL, NULL, NULL, NULL );
}

void CD3D9Render::SetProjection(Float32 fov, Float32 aspect,Float32 znear,Float32 zfar)
{
	Mat4f mat;
	mat.perspective(fov,aspect,znear,zfar);

	matProjection._11 = mat.mat[0];
	matProjection._12 = mat.mat[1];
	matProjection._13 = mat.mat[2];
	matProjection._14 = mat.mat[3];
	matProjection._21 = mat.mat[4];
	matProjection._22 = mat.mat[5];
	matProjection._23 = mat.mat[6];
	matProjection._24 = mat.mat[7];
	matProjection._31 = mat.mat[8];
	matProjection._32 = mat.mat[9];
	matProjection._33 = mat.mat[10];
	matProjection._34 = mat.mat[11];
	matProjection._41 = mat.mat[12];
	matProjection._42 = mat.mat[13];
	matProjection._43 = mat.mat[14];
	matProjection._44 = mat.mat[15];

    m_pDeviceContext->SetTransform( D3DTS_PROJECTION, &matProjection );
}

void CD3D9Render::SetLook(Vec3f pos,Vec3f up,Vec3f view)
{
	Mat4f matView;
	matView.look_at(pos,view,up);

	D3DMATRIX mat;
	mat._11 = matView.mat[0];
	mat._12 = matView.mat[1];
	mat._13 = matView.mat[2];
	mat._14 = matView.mat[3];
	mat._21 = matView.mat[4];
	mat._22 = matView.mat[5];
	mat._23 = matView.mat[6];
	mat._24 = matView.mat[7];
	mat._31 = matView.mat[8];
	mat._32 = matView.mat[9];
	mat._33 = matView.mat[10];
	mat._34 = matView.mat[11];
	mat._41 = matView.mat[12];
	mat._42 = matView.mat[13];
	mat._43 = matView.mat[14];
	mat._44 = matView.mat[15];

	m_pDeviceContext->SetTransform( D3DTS_VIEW, &mat);
}

void CD3D9Render::SetViewport(int32 x,int32 y,int32 width,int32 height)
{
	m_iScreenWidth = width;
	m_iScreenHeight = height;
	D3DVIEWPORT9 viewport;
	viewport.X = x;
	viewport.Y = y;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	m_pDeviceContext->SetViewport(&viewport);
}

void CD3D9Render::PushMatrix()
{
	m_pMatrixStack->Push();
}

void CD3D9Render::PopMatrix()
{
	m_pMatrixStack->Pop();
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

//Moving 2D
void CD3D9Render::Scale2D(Float32 x,Float32 y)
{
	m_pMatrixStack->Scale(x,y,1.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddScale2D(Float32 x,Float32 y)
{
	m_pMatrixStack->Scale(x,y,1.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::Translate2D(Float32 x,Float32 y)
{
	m_pMatrixStack->Translate(x-GetScreenWidth()*0.5f,(-y+(GetScreenHeight()*0.5f)),0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddTranslate2D(Float32 x,Float32 y)
{
	m_pMatrixStack->Translate(x,-y,0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::Rotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddRotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::TranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->Translate(x-GetScreenWidth()*0.5f,(-y+(GetScreenHeight()*0.5f)),0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddTranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->Translate(x,-y,0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::TranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pMatrixStack->Translate(x-GetScreenWidth()*0.5f,(-y+(GetScreenHeight()*0.5f)),0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddTranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pMatrixStack->Translate(x,-y,0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::ScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::TranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pMatrixStack->Translate(x-GetScreenWidth()*0.5f,(-y+(GetScreenHeight()*0.5f)),0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::AddTranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->Scale(sx,sy,1.0f);
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pMatrixStack->Translate(x,-y,0.0f);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

//Moving 3D
void CD3D9Render::Translate3D(Float32 x,Float32 y,Float32 z)
{
	m_pMatrixStack->Translate(x,y,z);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::TranslateScale3D(Float32 x,Float32 y,Float32 z,Float32 sx,Float32 sy,Float32 sz)
{
	m_pMatrixStack->Scale(sx,sy,sz);
	m_pMatrixStack->Translate(x,y,z);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::Scale3D(Float32 x,Float32 y,Float32 z)
{
	m_pMatrixStack->Scale(x,y,z);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::Rotate3D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z)
{
	m_pMatrixStack->RotateAxis(&D3DXVECTOR3(axis_x,axis_y,axis_z),DEG2RAD*angle);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

void CD3D9Render::SetTransformMatrix(Mat4f mat)
{
	m_pMatrixStack->MultMatrix(&(D3DXMATRIX)mat.mat);
	m_pDeviceContext->SetTransform(D3DTS_WORLD,m_pMatrixStack->GetTop());
}

//Transforms states
void CD3D9Render::SwitchTo2D()
{
	D3DXMATRIXA16 matOrtho;
	D3DXMatrixIdentity(&matOrtho);
	m_pDeviceContext->SetTransform(D3DTS_VIEW,&matOrtho);
	m_pDeviceContext->SetTransform(D3DTS_PROJECTION,&matOrtho);	
	D3DXMatrixOrthoRH(&matOrtho,FLOAT(m_iScreenWidth),FLOAT(m_iScreenHeight),0.0f,1.0f);
	m_pDeviceContext->SetTransform(D3DTS_PROJECTION,&matOrtho);	
}

void CD3D9Render::SwitchTo3D()
{
	m_pDeviceContext->SetTransform(D3DTS_PROJECTION,&matProjection);
}

Mat4f CD3D9Render::GetTransformStateMatrix(TRANSFORMSTATES state)
{
	Mat4f ret;
	D3DMATRIX mat;
	switch(state)
	{
		case VIEW:
			m_pDeviceContext->GetTransform(D3DTS_VIEW,&mat);
			break;
		case PROJECTION:
			m_pDeviceContext->GetTransform(D3DTS_PROJECTION,&mat);
			break;
		case TEXTURE0:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE0,&mat);
			break;
		case TEXTURE1:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE1,&mat);
			break;
		case TEXTURE2:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE2,&mat);
			break;
		case TEXTURE3:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE3,&mat);
			break;
		case TEXTURE4:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE4,&mat);
			break;
		case TEXTURE5:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE5,&mat);
			break;
		case TEXTURE6:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE6,&mat);
			break;
		case TEXTURE7:
			m_pDeviceContext->GetTransform(D3DTS_TEXTURE7,&mat);
			break;
	}
	ret[0] = mat._11;ret[1] = mat._12;ret[2] = mat._13;ret[3] = mat._14;
	ret[4] = mat._21;ret[5] = mat._22;ret[6] = mat._23;ret[7] = mat._24;
	ret[8] = mat._31;ret[9] = mat._32;ret[10] = mat._33;ret[11] = mat._34;
	ret[12] = mat._41;ret[13] = mat._42;ret[14] = mat._43;ret[15] = mat._44;
	return ret;
}

void CD3D9Render::SetTransformStateMatrix(TRANSFORMSTATES state,Mat4f mat)
{
	switch(state)
	{
		case VIEW:
			m_pDeviceContext->SetTransform(D3DTS_VIEW,&(D3DXMATRIX)mat.mat);
			break;
		case PROJECTION:
			m_pDeviceContext->SetTransform(D3DTS_PROJECTION,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE0:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE0,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE1:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE1,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE2:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE2,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE3:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE3,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE4:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE4,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE5:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE5,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE6:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE6,&(D3DXMATRIX)mat.mat);
			break;
		case TEXTURE7:
			m_pDeviceContext->SetTransform(D3DTS_TEXTURE7,&(D3DXMATRIX)mat.mat);
			break;
	}
}

//Textures
uint32 CD3D9Render::CreateTexture(TEXTURETYPE type,uint32 width,uint32 height,uint32 num_channels,uint32 format,uint32 depth,uint32 flags,uint32 mip_levels)
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

	m_pDeviceContext->CreateTexture(width,height,mip_levels,0,GetFormat(format),D3DPOOL_MANAGED,&m_pTextures[texture_id].TextureId,NULL);	
	//m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, m_pTextures[texture_id].NumMipLevels - 1);

	return texture_id;
}

void CD3D9Render::DestroyTexture(uint32 id)
{
	RemoveTexture(id);
	m_pTextureRemove[id] = 0;
}

uint32 CD3D9Render::RemoveTexture(uint32 texture_id)
{
	SAFE_RELEASE(m_pTextures[texture_id].TextureId);
	return 1;
}

uint32 CD3D9Render::FillPartTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 offsetx,uint32 offsety,uint32 mip_width,uint32 mip_height)
{
	if(mip_level >= m_pTextures[texture_id].NumMipLevels)return 0;
	if(mip_width == 0)mip_width = m_pTextures[texture_id].Width;
	if(mip_height == 0)mip_height = m_pTextures[texture_id].Height;

	for(uint32 i = 0, j = 0; i < mip_width*mip_height; i++, j += 4)
	{
		uChar tmp = data[j+2];
		data[j+2] = data[j+0];
		data[j+0] = tmp;
	}
	
	switch(m_pTextures[texture_id].Type)
	{
		case CRender::TEXTURE_TYPE_1D:
			break;
		case CRender::TEXTURE_TYPE_2D:
			{
				D3DLOCKED_RECT rect;
				RECT pRect;
				pRect.top = offsety;
				pRect.left = offsetx;
				pRect.bottom = mip_height;
				pRect.right = mip_width;
				
				m_pTextures[texture_id].TextureId->LockRect(mip_level, &rect, &pRect, 0);
				uint8 *pSrc = (uint8*)rect.pBits;
				for(uint32 i=0;i<mip_height;i++)
				{
					memcpy(pSrc,data,mip_width*m_pTextures[texture_id].NumChannels);
					pSrc+=m_pTextures[texture_id].Width*m_pTextures[texture_id].NumChannels;
					data+=mip_width*m_pTextures[texture_id].NumChannels;
				}
				m_pTextures[texture_id].TextureId->UnlockRect(mip_level);
			}
			break;
		case CRender::TEXTURE_TYPE_3D:
			break;
		case CRender::TEXTURE_TYPE_CUBE:
			break;
	}
	return 0;
}

uint32 CD3D9Render::FillTexture(uint32 texture_id,uChar *data)
{
	return FillTexture(texture_id,data,0,0,0);
}

uint32 CD3D9Render::FillTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 mip_width,uint32 mip_height)
{
	if(mip_level >= m_pTextures[texture_id].NumMipLevels)return 0;
	if(mip_width == 0)mip_width = m_pTextures[texture_id].Width;
	if(mip_height == 0)mip_height = m_pTextures[texture_id].Height;
	
	for(uint32 i = 0, j = 0; i < mip_width*mip_height; i++, j += 4)
	{
		uChar tmp = data[j+2];
		data[j+2] = data[j+0];
		data[j+0] = tmp;
	}
	
	switch(m_pTextures[texture_id].Type)
	{
		case CRender::TEXTURE_TYPE_1D:
			break;
		case CRender::TEXTURE_TYPE_2D:
			{
				D3DLOCKED_RECT rect;
				m_pTextures[texture_id].TextureId->LockRect(mip_level, &rect, NULL, 0);
				memcpy(rect.pBits,data,mip_width*mip_height*m_pTextures[texture_id].NumChannels);
				m_pTextures[texture_id].TextureId->UnlockRect(mip_level);
			}
			break;
		case CRender::TEXTURE_TYPE_3D:
			break;
		case CRender::TEXTURE_TYPE_CUBE:
			break;
	}
	
	return 1;
}


uint32 CD3D9Render::BindTexture(uint32 texture_id,uint32 texture_stage)
{
	m_pDeviceContext->SetTexture(texture_stage,m_pTextures[texture_id].TextureId);
	m_pDeviceContext->SetTextureStageState( texture_stage, D3DTSS_TEXCOORDINDEX, 0 );	
	return 1;
}

uint32 CD3D9Render::UnBindTexture(uint32 texture_id,uint32 texture_stage)
{
	m_pDeviceContext->SetTexture(texture_stage,0);
	return 1;
}

uint32 CD3D9Render::GenerateMipMapLevels(uint32 texture_id,uint32 hardware)
{
	if(m_pTextures[texture_id].NumMipLevels == 0)return 0;

	if(hardware)
	{
		return 0;
	}

	Float32 w = Float32(m_pTextures[texture_id].Width);
	Float32 h = Float32(m_pTextures[texture_id].Height);
	uint8 *data = new uint8[int32(w)*int32(h)*m_pTextures[texture_id].NumChannels];
	
	D3DLOCKED_RECT rect;
	m_pTextures[texture_id].TextureId->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
	memcpy(data,rect.pBits,size_t(w*h*m_pTextures[texture_id].NumChannels));
	m_pTextures[texture_id].TextureId->UnlockRect(0);
	
	CImage mip_img = CImage(m_pTextures[texture_id].Depth,m_pTextures[texture_id].Format,uint32(w),uint32(h),data);
	for(uint32 i=1;i<m_pTextures[texture_id].NumMipLevels+1;i++)
	{
		if(w == 1 || h == 1)break;
		w *= 0.5f;
		h *= 0.5f;
		mip_img.QuarterImage();
		FillTexture(texture_id,mip_img.GetData(),i,uint32(w),uint32(h));
	}
	return 1;
}

void CD3D9Render::SetTextureFilter(TEXTUREFILTER min_filter,TEXTUREFILTER mag_filter,TEXTUREFILTER mip_filter)
{
	switch(min_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		break;
		case TEXTURE_FILTER_LINEAR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 1 ? 1 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 2 ? 2 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 4 ? 4 : d3dCaps.MaxAnisotropy);		
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 8 ? 8 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 16 ? 16 : d3dCaps.MaxAnisotropy);
		break;
	}

	switch(mag_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		break;
		case TEXTURE_FILTER_LINEAR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 1 ? 1 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 2 ? 2 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 4 ? 4 : d3dCaps.MaxAnisotropy);		
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 8 ? 8 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 16 ? 16 : d3dCaps.MaxAnisotropy);
		break;
	}

	switch(mip_filter)
	{
		case TEXTURE_FILTER_NEAREST:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		break;
		case TEXTURE_FILTER_LINEAR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		break;
		case TEXTURE_FILTER_ANISOTROPY1:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 1 ? 1 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY2:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 2 ? 2 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY4:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 4 ? 4 : d3dCaps.MaxAnisotropy);		
		break;
		case TEXTURE_FILTER_ANISOTROPY8:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 8 ? 8 : d3dCaps.MaxAnisotropy);
		break;
		case TEXTURE_FILTER_ANISOTROPY16:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_MAXANISOTROPY,d3dCaps.MaxAnisotropy >= 16 ? 16 : d3dCaps.MaxAnisotropy);
		break;
	}
}

void CD3D9Render::SetTextureWrapState(TEXTUREWRAP wrap_s,TEXTUREWRAP wrap_t,TEXTUREWRAP wrap_r)
{
	switch(wrap_s)
	{
		case TEXTURE_WRAP_WRAP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		break;
		case TEXTURE_WRAP_CLAMP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
		break;
		case TEXTURE_WRAP_BORDER:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
		break;
	}

	switch(wrap_t)
	{
		case TEXTURE_WRAP_WRAP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		break;
		case TEXTURE_WRAP_CLAMP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		break;
		case TEXTURE_WRAP_BORDER:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
		break;
	}

	switch(wrap_r)
	{
		case TEXTURE_WRAP_WRAP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		break;
		case TEXTURE_WRAP_CLAMP:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
		break;
		case TEXTURE_WRAP_MIRROR:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_MIRROR);
		break;
		case TEXTURE_WRAP_BORDER:
			m_pDeviceContext->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_BORDER);
		break;
	}
}
//Geometry

uint32 CD3D9Render::AddVertexDeclaration(uint32 geometry_id,VERTEXFORMAT format,VERTEXFORMATTYPE type)
{
	++m_pGeometrys[geometry_id].NumVertexDeclAttrib;
	m_pGeometrys[geometry_id].pVertextElement = (D3DVERTEXELEMENT9*)ReAlloc(m_pGeometrys[geometry_id].pVertextElement,m_pGeometrys[geometry_id].NumVertexDeclAttrib*sizeof(D3DVERTEXELEMENT9));

	D3DDECLUSAGE formats[] =
	{
		D3DDECLUSAGE_POSITION,
		D3DDECLUSAGE_NORMAL,
		D3DDECLUSAGE_BINORMAL,
		D3DDECLUSAGE_TANGENT,
		D3DDECLUSAGE_TEXCOORD,
	};

	D3DDECLTYPE types [] = 
	{
		D3DDECLTYPE_FLOAT1,
		D3DDECLTYPE_FLOAT2,
		D3DDECLTYPE_FLOAT3,
		D3DDECLTYPE_FLOAT4,
	};

	uint32 types_size [] = 
	{
		sizeof(Float32),
		sizeof(Float32)*2,
		sizeof(Float32)*3,
		sizeof(Float32)*4,
	};

	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Stream = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Offset = m_pGeometrys[geometry_id].VertexFormatShift;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Type = types[type];
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Method = D3DDECLMETHOD_DEFAULT;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].Usage = formats[format];
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib-1].UsageIndex = m_pGeometrys[geometry_id].AttribIndex[format]; 
	++m_pGeometrys[geometry_id].AttribIndex[format];

	m_pGeometrys[geometry_id].VertexFormatShift += types_size[type];

	return m_pGeometrys[geometry_id].NumVertexDeclAttrib - 1;
}

uint32 CD3D9Render::SetStartNewGeometry(uint32 geometry_id)
{
	++m_pGeometrys[geometry_id].GeomertyCount;
	
	m_pGeometrys[geometry_id].GeomertyVertices = (uint32*)ReAlloc(m_pGeometrys[geometry_id].GeomertyVertices,m_pGeometrys[geometry_id].GeomertyCount*sizeof(uint32));
	m_pGeometrys[geometry_id].GeomertyVertices[m_pGeometrys[geometry_id].GeomertyCount-1] = m_pGeometrys[geometry_id].NumVertex;

	m_pGeometrys[geometry_id].GeomertyIndices = (uint32*)ReAlloc(m_pGeometrys[geometry_id].GeomertyIndices,m_pGeometrys[geometry_id].GeomertyCount*sizeof(uint32));
	m_pGeometrys[geometry_id].GeomertyIndices[m_pGeometrys[geometry_id].GeomertyCount-1] = m_pGeometrys[geometry_id].NumIndex;


	return 1;
}
uint32 CD3D9Render::CreateGeometry()
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

void CD3D9Render::DestroyGeometry(uint32 id)
{
	ReleaseGeometryVertexBuffer(id);
	for(uint32 i=0;i<m_pGeometrys[id].NumIndexBuffers;i++)
	{
		ReleaseGeometryIndexBuffer(id,i);
	}
	m_pGeometrys[id].DeInit();
	m_pGeometryRemove[id] = 0;
}

uint32 CD3D9Render::CreateGeometryVertexBuffer(uint32 geometry_id,BUFFERACCESS access,uint32 num_vertex)
{
	if(num_vertex == 0)num_vertex = m_pGeometrys[geometry_id].NumVertex;

	m_pGeometrys[geometry_id].pVertextElement = (D3DVERTEXELEMENT9*)ReAlloc(m_pGeometrys[geometry_id].pVertextElement,(m_pGeometrys[geometry_id].NumVertexDeclAttrib+1)*sizeof(D3DVERTEXELEMENT9));
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].Stream = 0xFF;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].Offset = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].Type = D3DDECLTYPE_UNUSED;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].Method = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].Usage = 0;
	m_pGeometrys[geometry_id].pVertextElement[m_pGeometrys[geometry_id].NumVertexDeclAttrib].UsageIndex = 0;
	m_pDeviceContext->CreateVertexDeclaration(m_pGeometrys[geometry_id].pVertextElement, &m_pGeometrys[geometry_id].pVertexDecl);
	m_pDeviceContext->SetVertexDeclaration(m_pGeometrys[geometry_id].pVertexDecl);
	m_pGeometrys[geometry_id].GlobalNumVertex = num_vertex;
	if(FAILED(m_pDeviceContext->CreateVertexBuffer(num_vertex * m_pGeometrys[geometry_id].VertexFormatShift,GetBufferAccess(access),0,access == CRender::STATIC ? D3DPOOL_MANAGED:D3DPOOL_DEFAULT,&m_pGeometrys[geometry_id].pVertexBuffer,NULL)))return 0;	

	return 1;
}

uint32 CD3D9Render::ReleaseGeometryVertexBuffer(uint32 geometry_id)
{
	if(geometry_id < m_iGeometrysCount)
	{
		SAFE_RELEASE(m_pGeometrys[geometry_id].pVertexBuffer);
	}
	return 1;
}

uint32 CD3D9Render::CreateGeometryIndexBuffer(uint32 geometry_id,BUFFERACCESS access,INDEXFORMAT index_format,uint32 num_index)
{
	++m_pGeometrys[geometry_id].NumIndexBuffers;
	m_pGeometrys[geometry_id].pIndexBuffer = (LPDIRECT3DINDEXBUFFER9*)ReAlloc(m_pGeometrys[geometry_id].pIndexBuffer,m_pGeometrys[geometry_id].NumIndexBuffers*sizeof(LPDIRECT3DINDEXBUFFER9));
	uint32 index_buffer_id = m_pGeometrys[geometry_id].NumIndexBuffers-1;
	if(num_index == 0)num_index = m_pGeometrys[geometry_id].NumIndex;
	m_pGeometrys[geometry_id].GlobalNumIndex = num_index;
	m_pGeometrys[geometry_id].IndexFormat = index_format;
	if(FAILED(m_pDeviceContext->CreateIndexBuffer(num_index * GetIndexFormatSize(index_format),GetBufferAccess(access),GetIndexFormat(index_format),access == CRender::STATIC ? D3DPOOL_MANAGED:D3DPOOL_DEFAULT,&m_pGeometrys[geometry_id].pIndexBuffer[index_buffer_id], NULL)))return 0;

	return index_buffer_id;
}

uint32 CD3D9Render::ReleaseGeometryIndexBuffer(uint32 geometry_id,uint32 buffer_id)
{
	if(geometry_id < m_iGeometrysCount)
	{
		if(buffer_id < m_pGeometrys[geometry_id].NumIndexBuffers)
		{
			SAFE_RELEASE(m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]);
		}
	}
	return 1;
}

uint32 CD3D9Render::AddVerexToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
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


uint32 CD3D9Render::AddTexCoordsToGeometry(uint32 geometry_id,uint32 num,Vec2f *data,uint32 object_id)
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

uint32 CD3D9Render::AddNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
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

uint32 CD3D9Render::AddBiNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
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

uint32 CD3D9Render::AddTangentToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id)
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

uint32 CD3D9Render::AddIndexToGeometry(uint32 geometry_id,uint32 num,int32 *data,uint32 object_id)
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

uint32 CD3D9Render::LoadToVertexBufferGeometry(uint32 geometry_id,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].NumVertex <= 0)return 0;
	if(object_id > m_pGeometrys[geometry_id].GeomertyCount-1)return 0;

	uint32 element_num = m_pGeometrys[geometry_id].VertexFormatShift/sizeof(FLOAT);
	VOID *pData;
	FLOAT *data;
	uint32 iter = 0,start = 0,vsize = 0,isize = 0;
	DWORD dwLockFlags = D3DLOCK_DISCARD | D3DUSAGE_WRITEONLY;
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
	
	data = new FLOAT[vsize*element_num];

	for(uint32 i=start,j=0;i<(start+vsize);i++,j+=element_num)
	{
		iter = 0;
		for(uint32 attr=0;attr<m_pGeometrys[geometry_id].NumVertexDeclAttrib;attr++)
		{
			if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == D3DDECLUSAGE_POSITION)
			{
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].x;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].y;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Vertex[i].z;
				++iter;
			}
			else if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == D3DDECLUSAGE_NORMAL)
			{
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].x;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].y;
				++iter;
				data[j+iter] = m_pGeometrys[geometry_id].Normal[i].z;
				++iter;
			}
			else if(m_pGeometrys[geometry_id].pVertextElement[attr].Usage == D3DDECLUSAGE_TEXCOORD)
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
		m_pGeometrys[geometry_id].pVertexBuffer->Lock(0,vsize*m_pGeometrys[geometry_id].VertexFormatShift,&pData,dwLockFlags);
		memcpy(pData,data,vsize*m_pGeometrys[geometry_id].VertexFormatShift);
		m_pGeometrys[geometry_id].pVertexBuffer->Unlock();
	}
	else
	{
		m_pGeometrys[geometry_id].pVertexBuffer->Lock(m_pGeometrys[geometry_id].GeomertyVertices[l-1]*m_pGeometrys[geometry_id].VertexFormatShift,vsize*m_pGeometrys[geometry_id].VertexFormatShift,&pData,dwLockFlags);
		memcpy(pData,data,vsize*m_pGeometrys[geometry_id].VertexFormatShift);
		m_pGeometrys[geometry_id].pVertexBuffer->Unlock();
	}
	SAFE_DELETE_ARRAY(data);
	

	return 1;
}

uint32 CD3D9Render::LoadToVertexBufferGeometry(uint32 geometry_id)
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

uint32 CD3D9Render::LoadToIndexBufferGeometry(uint32 geometry_id,uint32 object_id,uint32 buffer_id)
{
	if(buffer_id >m_pGeometrys[geometry_id].NumIndexBuffers-1)return 0;
	if(m_pGeometrys[geometry_id].NumIndex <= 0)return 0;
	if(object_id > m_pGeometrys[geometry_id].GeomertyCount-1)return 0;

	uint32 element_num = m_pGeometrys[geometry_id].VertexFormatShift/sizeof(FLOAT);
	VOID *pData;
	uint16 *indices;
	uint32 iter = 0,start = 0,vsize = 0,isize = 0;
	DWORD dwLockFlags = D3DLOCK_DISCARD | D3DUSAGE_WRITEONLY;
	uint32 l = object_id;
	

	if(l>0)
	{
		start = m_pGeometrys[geometry_id].GeomertyIndices[l-1];
		isize = m_pGeometrys[geometry_id].GeomertyIndices[l]-m_pGeometrys[geometry_id].GeomertyIndices[l-1];
	}
	else
	{
		start = 0;
		isize = m_pGeometrys[geometry_id].GeomertyIndices[l];
	}

	indices = new uint16[isize];

	for(uint32 ii=start,jj=0;ii<(start+isize);ii++,jj++)
	{
		indices[jj] = m_pGeometrys[geometry_id].Index[ii];
	}
    
	if(m_pGeometrys[geometry_id].NumIndex > m_pGeometrys[geometry_id].GlobalNumIndex)
	{
		return 0;
	}

	if(l==0)
	{
		m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]->Lock(0,isize * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),&pData,dwLockFlags);
		memcpy(pData,indices,isize * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat));
		m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]->Unlock();
	}
	else
	{
		m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]->Lock(m_pGeometrys[geometry_id].GeomertyIndices[l-1] * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),isize * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat),&pData,dwLockFlags);
		memcpy(pData,indices,isize * GetIndexFormatSize(m_pGeometrys[geometry_id].IndexFormat));
		m_pGeometrys[geometry_id].pIndexBuffer[buffer_id]->Unlock();
	}
	SAFE_DELETE_ARRAY(indices);
	

	return 1;
}

uint32 CD3D9Render::LoadToIndexBufferGeometry(uint32 geometry_id)
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

uint32 CD3D9Render::SetDrawable(uint32 geometry_id)
{
	m_pDeviceContext->SetVertexDeclaration(m_pGeometrys[geometry_id].pVertexDecl);
	m_pDeviceContext->SetStreamSource(0,m_pGeometrys[geometry_id].pVertexBuffer,0,m_pGeometrys[geometry_id].VertexFormatShift);
	
	return 1;
}

uint32 CD3D9Render::SetIndexBuffer(uint32 geometry_id,uint32 index_buffer_id)
{
	if(m_pGeometrys[geometry_id].GlobalNumIndex>0)
	{
		m_pDeviceContext->SetIndices(m_pGeometrys[geometry_id].pIndexBuffer[index_buffer_id]);
	}
	return 1;
}

uint32 CD3D9Render::DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id)
{
	for(uint32 i=0;i<m_pGeometrys[geometry_id].GeomertyCount;i++)
	{
		DrawGeometry(primitive_type,geometry_id,i);
	}
	
	return 1;
}

uint32 CD3D9Render::DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id,uint32 object_id)
{
	if(m_pGeometrys[geometry_id].GlobalNumIndex>0)
	{
		uint32 primitive_count = 0;

		if(object_id == 0)
		{
			switch(primitive_type)
			{
				case DRAW_PRIMITIVE_TRIANGLE_LIST:
					primitive_count = m_pGeometrys[geometry_id].GeomertyIndices[object_id]/3;
					break;
				case DRAW_PRIMITIVE_TRIANGLE_STRIP:
					primitive_count = m_pGeometrys[geometry_id].GeomertyIndices[object_id]-2;
					break;
				case DRAW_PRIMITIVE_LINES:
					primitive_count = m_pGeometrys[geometry_id].GeomertyIndices[object_id];
					break;
				case DRAW_PRIMITIVE_LINES_STRIP:
					primitive_count = m_pGeometrys[geometry_id].GeomertyIndices[object_id]-1;
					break;
				case DRAW_PRIMITIVE_POINTS:
					primitive_count = m_pGeometrys[geometry_id].GeomertyIndices[object_id];
					break;
			}
			//m_pDeviceContext->DrawIndexedPrimitive((D3DPRIMITIVETYPE)GetPrimitiveType(primitive_type),0,0,m_pGeometrys[geometry_id].GeomertyVertices[object_id],0,m_pGeometrys[geometry_id].GeometryPrimitives[object_id]);
			m_pDeviceContext->DrawIndexedPrimitive((D3DPRIMITIVETYPE)GetPrimitiveType(primitive_type),0,0,m_pGeometrys[geometry_id].GeomertyVertices[object_id],0,primitive_count);
		}
		else
		{
			switch(primitive_type)
			{
				case DRAW_PRIMITIVE_TRIANGLE_LIST:
					primitive_count = (m_pGeometrys[geometry_id].GeomertyIndices[object_id] - m_pGeometrys[geometry_id].GeomertyIndices[object_id-1])/3;
					break;
				case DRAW_PRIMITIVE_TRIANGLE_STRIP:
					primitive_count = (m_pGeometrys[geometry_id].GeomertyIndices[object_id] - m_pGeometrys[geometry_id].GeomertyIndices[object_id-1])-2;
					break;
				case DRAW_PRIMITIVE_LINES:
					primitive_count = (m_pGeometrys[geometry_id].GeomertyIndices[object_id] - m_pGeometrys[geometry_id].GeomertyIndices[object_id-1]);
					break;
				case DRAW_PRIMITIVE_LINES_STRIP:
					primitive_count = (m_pGeometrys[geometry_id].GeomertyIndices[object_id] - m_pGeometrys[geometry_id].GeomertyIndices[object_id-1])-1;
					break;
				case DRAW_PRIMITIVE_POINTS:
					primitive_count = (m_pGeometrys[geometry_id].GeomertyIndices[object_id] - m_pGeometrys[geometry_id].GeomertyIndices[object_id-1]);
					break;
			}
			//m_pDeviceContext->DrawIndexedPrimitive((D3DPRIMITIVETYPE)GetPrimitiveType(primitive_type),m_pGeometrys[geometry_id].GeomertyVertices[object_id-1],0,m_pGeometrys[geometry_id].GeomertyVertices[object_id] - m_pGeometrys[geometry_id].GeomertyVertices[object_id-1],m_pGeometrys[geometry_id].GeomertyIndices[object_id-1],m_pGeometrys[geometry_id].GeometryPrimitives[object_id] - m_pGeometrys[geometry_id].GeometryPrimitives[object_id-1]);			
			m_pDeviceContext->DrawIndexedPrimitive((D3DPRIMITIVETYPE)GetPrimitiveType(primitive_type),m_pGeometrys[geometry_id].GeomertyVertices[object_id-1],0,m_pGeometrys[geometry_id].GeomertyVertices[object_id] - m_pGeometrys[geometry_id].GeomertyVertices[object_id-1],m_pGeometrys[geometry_id].GeomertyIndices[object_id-1],primitive_count);
		}
	}

	return 1;
}

//Lights

uint32 CD3D9Render::CreateLight()
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
	ZeroMemory(&m_pLights[light_id].pLight,sizeof(m_pLights[light_id].pLight));
	m_pLights[light_id].pLight.Range = FLT_MAX;
	return light_id;
}

void CD3D9Render::DestroyLight(uint32 id)
{
	m_pLightRemove[id] = 0;
}

uint32 CD3D9Render::SetLight(uint32 light_id)
{
	m_pDeviceContext->SetLight(light_id,&m_pLights[light_id].pLight);
	return 1;
}

void CD3D9Render::SetLightEnable(uint32 light_id)
{
	m_pDeviceContext->LightEnable(light_id,TRUE);
}

void CD3D9Render::SetLightDisable(uint32 light_id)
{
	m_pDeviceContext->LightEnable(light_id,FALSE);
}

void CD3D9Render::SetLightType(uint32 light_id,LIGHTTYPE Type)
{
	switch(Type)
	{
		case LIGHT_TYPE_POINT:
			m_pLights[light_id].pLight.Type = D3DLIGHT_POINT;
		break;
		case LIGHT_TYPE_SPOT:
			m_pLights[light_id].pLight.Type = D3DLIGHT_SPOT; 
		break;
	}
}

void CD3D9Render::SetLightDiffuse(uint32 light_id,Vec4f Diffuse)
{
	m_pLights[light_id].pLight.Diffuse.r = Diffuse.v[0]/255.0f;
	m_pLights[light_id].pLight.Diffuse.g = Diffuse.v[1]/255.0f;
	m_pLights[light_id].pLight.Diffuse.b = Diffuse.v[2]/255.0f;
	m_pLights[light_id].pLight.Diffuse.a = Diffuse.v[3]/255.0f;
}

void CD3D9Render::SetLightSpecular(uint32 light_id,Vec4f Specular)
{
	m_pLights[light_id].pLight.Specular.r = Specular.v[0]/255.0f;
	m_pLights[light_id].pLight.Specular.g = Specular.v[1]/255.0f;
	m_pLights[light_id].pLight.Specular.b = Specular.v[2]/255.0f;
	m_pLights[light_id].pLight.Specular.a = Specular.v[3]/255.0f;
}

void CD3D9Render::SetLightAmbient(uint32 light_id,Vec4f Ambient)
{
	m_pLights[light_id].pLight.Ambient.r = Ambient.v[0]/255.0f;
	m_pLights[light_id].pLight.Ambient.g = Ambient.v[1]/255.0f;
	m_pLights[light_id].pLight.Ambient.b = Ambient.v[2]/255.0f;
	m_pLights[light_id].pLight.Ambient.a = Ambient.v[3]/255.0f;
}

void CD3D9Render::SetLightPosition(uint32 light_id,Vec3f Position)
{
	D3DXVECTOR3 pos;
	pos.x = Position.v[0];
	pos.y = Position.v[1];
	pos.z = Position.v[2];
	m_pLights[light_id].pLight.Position = pos;
}

void CD3D9Render::SetLightDirection(uint32 light_id,Vec3f Direction)
{
	D3DXVECTOR3 dir;
	dir.x = Direction.v[0];
	dir.y = Direction.v[1];
	dir.z = Direction.v[2];
	m_pLights[light_id].pLight.Direction = dir;
}

void CD3D9Render::SetLightAttenuation0(uint32 light_id,Float32 Attenuation0)
{
	m_pLights[light_id].pLight.Attenuation0 = Attenuation0;
}

void CD3D9Render::SetLightAttenuation1(uint32 light_id,Float32 Attenuation1)
{
	m_pLights[light_id].pLight.Attenuation1 = Attenuation1;
}

void CD3D9Render::SetLightAttenuation2(uint32 light_id,Float32 Attenuation2)
{
	m_pLights[light_id].pLight.Attenuation2 = Attenuation2;
}

void CD3D9Render::SetLightCutoff(uint32 light_id,Float32 Cutoff)
{
	m_pLights[light_id].pLight.Phi = DEG2RAD*Cutoff*2;
}

void CD3D9Render::SetLightExponent(uint32 light_id,Float32 Exponent)
{
	m_pLights[light_id].pLight.Theta = DEG2RAD*Exponent*2;
}


//Materials
uint32 CD3D9Render::CreateMaterial()
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
		ZeroMemory(&m_pMaterials[material_id].pMaterial, sizeof(D3DMATERIAL9));
		material_id = m_iMaterialsCount-1;
	}
	m_pMaterialRemove[material_id] = 1;
	return material_id;
}

void CD3D9Render::DestroyMaterial(uint32 id)
{
	m_pMaterialRemove[id] = 0;
}

uint32 CD3D9Render::SetMaterial(uint32 material_id)
{
	m_pDeviceContext->SetMaterial(&m_pMaterials[material_id].pMaterial);
	return 1;
}

void CD3D9Render::SetMaterialAmbient(uint32 material_id,Vec4f Ambient)
{
	m_pMaterials[material_id].pMaterial.Ambient.r = Ambient.v[0]/255.0f;
	m_pMaterials[material_id].pMaterial.Ambient.g = Ambient.v[1]/255.0f;
	m_pMaterials[material_id].pMaterial.Ambient.b = Ambient.v[2]/255.0f;
	m_pMaterials[material_id].pMaterial.Ambient.a = Ambient.v[3]/255.0f;
}

void CD3D9Render::SetMaterialDiffuse(uint32 material_id,Vec4f Diffuse)
{
	m_pMaterials[material_id].pMaterial.Diffuse.r = Diffuse.v[0]/255.0f;
	m_pMaterials[material_id].pMaterial.Diffuse.g = Diffuse.v[1]/255.0f;
	m_pMaterials[material_id].pMaterial.Diffuse.b = Diffuse.v[2]/255.0f;
	m_pMaterials[material_id].pMaterial.Diffuse.a = Diffuse.v[3]/255.0f;
}

void CD3D9Render::SetMaterialSpecular(uint32 material_id,Vec4f Specular)
{
	m_pMaterials[material_id].pMaterial.Specular.r = Specular.v[0]/255.0f;
	m_pMaterials[material_id].pMaterial.Specular.g = Specular.v[1]/255.0f;
	m_pMaterials[material_id].pMaterial.Specular.b = Specular.v[2]/255.0f;
	m_pMaterials[material_id].pMaterial.Specular.a = Specular.v[3]/255.0f;
	m_pDeviceContext->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
}

void CD3D9Render::SetMaterialEmissive(uint32 material_id,Vec4f Emissive)
{
	m_pMaterials[material_id].pMaterial.Emissive.r = Emissive.v[0]/255.0f;
	m_pMaterials[material_id].pMaterial.Emissive.g = Emissive.v[1]/255.0f;
	m_pMaterials[material_id].pMaterial.Emissive.b = Emissive.v[2]/255.0f;
	m_pMaterials[material_id].pMaterial.Emissive.a = Emissive.v[3]/255.0f;
}

void CD3D9Render::SetMaterialPower(uint32 material_id,Float32 Power)
{
	m_pMaterials[material_id].pMaterial.Power = Power;
}

//Shaders
uint32 CD3D9Render::CreateShader(Str vertex_shader_text,Str vertex_profile,Str fragment_shader_text,Str fragment_profile)
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

		if(vertex_profile.Size() == 0){m_pCgVertexProfile = cgD3D9GetLatestVertexProfile();}else{SetShaderProfile(vertex_profile,Str(""));}
		if(fragment_profile.Size() == 0){m_pCgFragmentProfile = cgD3D9GetLatestPixelProfile();}else{SetShaderProfile(Str(""),fragment_profile);}

		const char **profileOpts;
		if(vertex_shader_text.Size() > 0 && cgIsProfileSupported(m_pCgVertexProfile) && cgGetProfileProperty(m_pCgVertexProfile,CG_IS_VERTEX_PROFILE) && (cgGetProfileProperty(m_pCgVertexProfile,CG_IS_HLSL_PROFILE) || cgGetProfileProperty(m_pCgVertexProfile,CG_IS_DIRECT3D_9_PROFILE)))
		{
			profileOpts = cgD3D9GetOptimalOptions(m_pCgVertexProfile);
			m_pCgVertexProgram[shader_id] = cgCreateProgram(m_pCgContext,CG_SOURCE,vertex_shader_text,m_pCgVertexProfile,NULL,profileOpts);
			GetShaderLastError(shader_id,Str("Create Vertex Program"));
		}
		if(fragment_shader_text.Size() > 0 && cgIsProfileSupported(m_pCgFragmentProfile) && cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_FRAGMENT_PROFILE) && (cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_HLSL_PROFILE) || cgGetProfileProperty(m_pCgFragmentProfile,CG_IS_DIRECT3D_9_PROFILE)))
		{
			profileOpts = cgD3D9GetOptimalOptions(m_pCgFragmentProfile);
			m_pCgFragmentProgram[shader_id] = cgCreateProgram(m_pCgContext,CG_SOURCE,fragment_shader_text,m_pCgFragmentProfile,NULL,profileOpts);
			GetShaderLastError(shader_id,Str("Create Fragment Program"));
		}
  	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		//Check destroyed geometrys
		for(uint32 i=0;i<m_iShadersCount;i++)
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
			m_iShadersCount++;
			shader_id = m_iShadersCount - 1;
			m_pShaders = (ShaderData*)ReAlloc(m_pShaders,m_iShadersCount+sizeof(ShaderData));
			m_pShaderRemove = (uint32*)ReAlloc(m_pShaderRemove,m_iShadersCount * sizeof(uint32));
		}
		m_pShaderRemove[shader_id] = 1;
		m_pShaders[shader_id].m_dwShaderFlags = 0;
		m_pShaders[shader_id].m_iErrorsStatus = 0;
		m_pShaders[shader_id].m_pBufferErrors = NULL;
		m_pShaders[shader_id].m_pConstantTableFS = NULL;
		m_pShaders[shader_id].m_pConstantTableVS = NULL;
		m_pShaders[shader_id].m_pFragmentShader = NULL;
		m_pShaders[shader_id].m_pFSCode = NULL;
		m_pShaders[shader_id].m_pVertexShader = NULL;
		m_pShaders[shader_id].m_pVSCode = NULL;

		//Trace("%s,%s\n",D3DXGetVertexShaderProfile(m_pDeviceContext),D3DXGetPixelShaderProfile(m_pDeviceContext));
		if(vertex_shader_text.Size() > 0)
		{
			if(FAILED(D3DXCompileShader(vertex_shader_text.GetData(),vertex_shader_text.Size(),NULL,NULL,"main",vertex_profile.GetData(),m_pShaders[shader_id].m_dwShaderFlags,&m_pShaders[shader_id].m_pVSCode,&m_pShaders[shader_id].m_pBufferErrors,&m_pShaders[shader_id].m_pConstantTableVS)))
			{
				m_pShaders[shader_id].m_iErrorsStatus = 1;
				GetShaderLastError(shader_id,"HLSL Compile Vertex Shader");
			}
		}
		if(fragment_shader_text.Size() > 0)
		{
			if(FAILED(D3DXCompileShader(fragment_shader_text.GetData(),fragment_shader_text.Size(),NULL,NULL,"main",fragment_profile.GetData(),m_pShaders[shader_id].m_dwShaderFlags,&m_pShaders[shader_id].m_pFSCode,&m_pShaders[shader_id].m_pBufferErrors,&m_pShaders[shader_id].m_pConstantTableFS)))
			{
				m_pShaders[shader_id].m_iErrorsStatus = 1;
				GetShaderLastError(shader_id,"HLSL Compile Fragment Shader");
			}			   
		}
	}
	return shader_id;
}

void CD3D9Render::DestroyShader(uint32 id)
{
	UnLoadShader(id);
	ReleaseShader(id);
	m_pShaderRemove[id] = 0;
}

void CD3D9Render::ReleaseShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgDestroyProgram(m_pCgVertexProgram[shader_id]);
		cgDestroyProgram(m_pCgFragmentProgram[shader_id]);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		SAFE_RELEASE(m_pShaders[shader_id].m_pVertexShader);
		SAFE_RELEASE(m_pShaders[shader_id].m_pConstantTableVS);
		SAFE_RELEASE(m_pShaders[shader_id].m_pFragmentShader);
		SAFE_RELEASE(m_pShaders[shader_id].m_pConstantTableFS);
		SAFE_RELEASE(m_pShaders[shader_id].m_pBufferErrors);
	}
}

void CD3D9Render::LoadShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgD3D9LoadProgram(m_pCgVertexProgram[shader_id], true, 0);
		GetShaderLastError(shader_id,Str("Load Vertex Program"));
		cgD3D9LoadProgram(m_pCgFragmentProgram[shader_id], true, 0);
		GetShaderLastError(shader_id,Str("Load Fragment Program"));
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			m_pDeviceContext->CreateVertexShader((DWORD*)m_pShaders[shader_id].m_pVSCode->GetBufferPointer(),&m_pShaders[shader_id].m_pVertexShader);
			SAFE_RELEASE(m_pShaders[shader_id].m_pVSCode);
			m_pDeviceContext->CreatePixelShader((DWORD*)m_pShaders[shader_id].m_pFSCode->GetBufferPointer(),&m_pShaders[shader_id].m_pFragmentShader);
			SAFE_RELEASE(m_pShaders[shader_id].m_pFSCode);
		}
	}
}

void CD3D9Render::UnLoadShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgD3D9UnloadProgram(m_pCgVertexProgram[shader_id]);
		cgD3D9UnloadProgram(m_pCgFragmentProgram[shader_id]);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		SAFE_RELEASE(m_pShaders[shader_id].m_pVertexShader);
		SAFE_RELEASE(m_pShaders[shader_id].m_pConstantTableVS);
		SAFE_RELEASE(m_pShaders[shader_id].m_pFragmentShader);
		SAFE_RELEASE(m_pShaders[shader_id].m_pConstantTableFS);
		SAFE_RELEASE(m_pShaders[shader_id].m_pBufferErrors);
	}
}

void CD3D9Render::BindShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgD3D9BindProgram(m_pCgVertexProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Bind Vertex Program"));
		cgD3D9BindProgram(m_pCgFragmentProgram[shader_id]);
		GetShaderLastError(shader_id,Str("Bind Fragment Program"));
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		m_pDeviceContext->SetVertexShader(m_pShaders[shader_id].m_pVertexShader);
		m_pDeviceContext->SetPixelShader(m_pShaders[shader_id].m_pFragmentShader);
	}
}

void CD3D9Render::UnBindShader(uint32 shader_id)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		cgD3D9BindProgram(NULL);
		m_pDeviceContext->SetVertexShader(NULL);
		cgD3D9BindProgram(NULL);
		m_pDeviceContext->SetPixelShader(NULL);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		m_pDeviceContext->SetVertexShader(NULL);
        m_pDeviceContext->SetPixelShader(NULL);
	}
}

uint32 CD3D9Render::AddShaderVertexParameter(uint32 shader_id,Str name_in_shader)
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
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return 0;
		m_iParametersCount++;
		parameter_id = m_iParametersCount - 1;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			m_pShaderParameters = (ShaderParameterData*)ReAlloc(m_pShaderParameters,m_iParametersCount*sizeof(ShaderParameterData));
			m_pShaderParameters[parameter_id].m_pType = 1;
			m_pShaderParameters[parameter_id].m_pShaderId = shader_id;
			m_pShaderParameters[parameter_id].m_pParameterId = m_pShaders[shader_id].m_pConstantTableVS->GetConstantByName(NULL,name_in_shader.GetData());
		}
		
	}
	return parameter_id;
}

uint32 CD3D9Render::AddShaderFragmentParameter(uint32 shader_id,Str name_in_shader)
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
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return 0;
		m_iParametersCount++;
		parameter_id = m_iParametersCount - 1;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			m_pShaderParameters = (ShaderParameterData*)ReAlloc(m_pShaderParameters,m_iParametersCount*sizeof(ShaderParameterData));
			m_pShaderParameters[parameter_id].m_pType = 2;
			m_pShaderParameters[parameter_id].m_pShaderId = shader_id;
			m_pShaderParameters[parameter_id].m_pParameterId = m_pShaders[shader_id].m_pConstantTableFS->GetConstantByName(NULL,name_in_shader.GetData());
		}
	}
	return parameter_id;
}

void CD3D9Render::GetShaderLastError(uint32 shader_id,Str situation)
{
	if(!m_bShadersDebugMode)return;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if(error != CG_NO_ERROR && error != 1000) 
		{
			Trace("%d-%s:%s\n",shader_id,situation.GetData(),string);
			if(error == CG_COMPILER_ERROR) 
			{
				Trace("%s\n", cgGetLastListing(m_pCgContext));
			}
		}
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		Trace("%d-%s:%s\n",shader_id,situation.GetData(),(const char*)m_pShaders[shader_id].m_pBufferErrors->GetBufferPointer());
	}
}

void CD3D9Render::SetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameteric(m_pParameters[parameter_id],matrix);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
		}
	}
}

void CD3D9Render::SetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameterfc(m_pParameters[parameter_id],matrix);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
		}
	}
}

void CD3D9Render::SetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetMatrixParameterdc(m_pParameters[parameter_id],matrix);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(shader_id >= m_iShadersCount)return;
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetMatrix(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,(D3DXMATRIX *)matrix);
			}
		}
	}
}

void CD3D9Render::GetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameteric(m_pParameters[parameter_id],matrix);
	}
}

void CD3D9Render::GetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameterfc(m_pParameters[parameter_id],matrix);
	}
}

void CD3D9Render::GetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetMatrixParameterdc(m_pParameters[parameter_id],matrix);
	}
}

void CD3D9Render::SetShaderParameter1i(uint32 shader_id,uint32 parameter_id,int32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1i(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetInt(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetInt(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iParametersCount)return;
		cgSetParameter1iv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,1);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,1);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter1f(uint32 shader_id,uint32 parameter_id,Float32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1f(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloat(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloat(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1fv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,1);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,1);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter1d(uint32 shader_id,uint32 parameter_id,Double32 param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1d(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter1f(shader_id,parameter_id,(Float32)param);
	}
}

void CD3D9Render::SetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter1dv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter1fv(shader_id,parameter_id,(Float32*)param);
	}
}

void CD3D9Render::SetShaderParameter2i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2i(m_pParameters[parameter_id],param1,param2);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec2i(param1,param2).v,2);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec2i(param1,param2).v,2);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2iv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,2);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,2);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter2f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2f(m_pParameters[parameter_id],param1,param2);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec2f(param1,param2).v,2);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec2f(param1,param2).v,2);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2fv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,2);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,2);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter2d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2d(m_pParameters[parameter_id],param1,param2);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter2f(shader_id,parameter_id,(Float32)param1,(Float32)param2);
	}
}

void CD3D9Render::SetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter2dv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter2fv(shader_id,parameter_id,(Float32*)param);
	}
}

void CD3D9Render::SetShaderParameter3i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3i(m_pParameters[parameter_id],param1,param2,param3);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec3i(param1,param2,param3).v,3);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec3i(param1,param2,param3).v,3);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3iv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,3);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,3);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter3f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3f(m_pParameters[parameter_id],param1,param2,param3);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec3f(param1,param2,param3).v,3);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec3f(param1,param2,param3).v,3);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3fv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,3);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,3);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter3d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3d(m_pParameters[parameter_id],param1,param2,param3);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter3f(shader_id,parameter_id,(Float32)param1,(Float32)param2,(Float32)param3);
	}
}

void CD3D9Render::SetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter3dv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter3fv(shader_id,parameter_id,(Float32*)param);
	}
}

void CD3D9Render::SetShaderParameter4i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3,int32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4i(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec4i(param1,param2,param3,param4).v,4);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec4i(param1,param2,param3,param4).v,4);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4iv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,4);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetIntArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,4);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter4f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3,Float32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4f(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec4f(param1,param2,param3,param4).v,4);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,Vec4f(param1,param2,param3,param4).v,4);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4fv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		if(parameter_id >= m_iParametersCount)return;
		if(!m_pShaders[shader_id].m_iErrorsStatus)
		{
			if(m_pShaderParameters[parameter_id].m_pType == 1)
			{
				m_pShaders[shader_id].m_pConstantTableVS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,4);
			}
			else if(m_pShaderParameters[parameter_id].m_pType == 2)
			{
				m_pShaders[shader_id].m_pConstantTableFS->SetFloatArray(m_pDeviceContext,m_pShaderParameters[parameter_id].m_pParameterId,param,4);
			}
		}
	}
}

void CD3D9Render::SetShaderParameter4d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3,Double32 param4)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4d(m_pParameters[parameter_id],param1,param2,param3,param4);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter4f(shader_id,parameter_id,(Float32)param1,(Float32)param2,(Float32)param3,(Float32)param4);
	}
}

void CD3D9Render::SetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgSetParameter4dv(m_pParameters[parameter_id],param);
	}
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		SetShaderParameter4fv(shader_id,parameter_id,(Float32*)param);
	}
}

void CD3D9Render::GetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],1,param);
	}
}
void CD3D9Render::GetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],1,param);
	}
}
void CD3D9Render::GetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],1,param);
	}
}
void CD3D9Render::GetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],2,param);
	}
}
void CD3D9Render::GetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],2,param);
	}
}
void CD3D9Render::GetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],2,param);
	}
}
void CD3D9Render::GetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],3,param);
	}
}
void CD3D9Render::GetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],3,param);
	}
}
void CD3D9Render::GetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],3,param);
	}
}
void CD3D9Render::GetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValueir(m_pParameters[parameter_id],4,param);
	}
}
void CD3D9Render::GetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuefr(m_pParameters[parameter_id],4,param);
	}
}
void CD3D9Render::GetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param)
{
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(parameter_id >= m_iCgParametersCount)return;
		cgGetParameterValuedr(m_pParameters[parameter_id],4,param);
	}
}

void CD3D9Render::SetShaderParameterTexture(uint32 shader_id,uint32 parameter_id,uint32 texture_id)
{
	if(texture_id >= m_iTexturesCount)return;
	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		if(shader_id >= m_iCgShadersCount)return;
		if(parameter_id >= m_iCgParametersCount)return;
		cgD3D9SetTextureParameter(m_pParameters[parameter_id],m_pTextures[texture_id].TextureId);
	}
}

Str CD3D9Render::GetShaderSupportedVertexProfiles()
{
	Str ret = Str("");

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(int32 i=0; i<cgGetNumSupportedProfiles(); i++)
		{
			if(cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_VERTEX_PROFILE) && (cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_HLSL_PROFILE) || cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_DIRECT3D_9_PROFILE)))
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
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		ret = Str("vs_1_1,vs_2_0,vs_2_sw");
	}

	return ret;
}

Str CD3D9Render::GetShaderSupportedFragmentProfiles()
{
	Str ret = Str("");

	if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_CG)
	{
		for(int32 i=0; i<cgGetNumSupportedProfiles(); i++)
		{
			if(cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_FRAGMENT_PROFILE) && (cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_HLSL_PROFILE) || cgGetProfileProperty(cgGetSupportedProfile(i),CG_IS_DIRECT3D_9_PROFILE)))
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
	else if(m_iShaderLanguage == CRender::SHADER_LANGUAGE_TYPE::SHADER_LANGUAGE_TYPE_D3D9_HLSL)
	{
		ret = Str("ps_1_1,ps_1_2,ps_1_3,ps_1_4,ps_2_0,ps_2_sw");
	}

	return ret;
}

void CD3D9Render::SetShaderProfile(Str vertex_profile,Str fragment_profile)
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
				m_pCgVertexProfile = cgD3D9GetLatestVertexProfile();
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
				m_pCgFragmentProfile = cgD3D9GetLatestPixelProfile();
			}
		}
	}
}

//DebugDraw
void CD3D9Render::DrawPoint(Vec3f point,Float32 size)
{
	struct vp_struct
	{
		FLOAT x,y,z;
		FLOAT psize;
	};
	vp_struct vertex[3];
	vertex[0].x = point.x;vertex[0].y = point.y;vertex[0].z = point.z;vertex[0].psize=size;

	m_pDeviceContext->SetFVF(D3DFVF_XYZ|D3DFVF_PSIZE);
	m_pDeviceContext->DrawPrimitiveUP((D3DPRIMITIVETYPE)GetPrimitiveType(CRender::DRAWPRIMITIVE::DRAW_PRIMITIVE_POINTS),1,vertex,sizeof(vp_struct));
}

void CD3D9Render::DrawLine(Vec3f start,Vec3f end)
{
	struct vl_struct
	{
		FLOAT x,y,z;
		DWORD color;
	};
	vl_struct vertex[3];
	vertex[0].x = start.x;vertex[0].y = start.y;vertex[0].z = start.z;
	vertex[1].x = end.x;vertex[1].y = end.y;vertex[1].z = end.z;

	m_pDeviceContext->SetFVF(D3DFVF_XYZ);
	m_pDeviceContext->DrawPrimitiveUP((D3DPRIMITIVETYPE)GetPrimitiveType(CRender::DRAWPRIMITIVE::DRAW_PRIMITIVE_LINES),1,vertex,sizeof(vl_struct));
}

void CD3D9Render::DrawTriangle(Vec3f v0,Vec3f v1,Vec3f v2,Vec3f n)
{
	struct vt_struct
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		DWORD color;
	};
	vt_struct vertex[3];
	vertex[0].x = v0.x;vertex[0].y = v0.y;vertex[0].z = v0.z;
	vertex[0].nx = n.x;vertex[0].ny = n.y;vertex[0].nz = n.z;
	vertex[0].color = D3DCOLOR_ARGB(255,255,255,255);
	vertex[1].x = v1.x;vertex[1].y = v1.y;vertex[1].z = v1.z;
	vertex[1].nx = n.x;vertex[1].ny = n.y;vertex[1].nz = n.z;
	vertex[1].color = D3DCOLOR_ARGB(255,255,255,255);
	vertex[2].x = v2.x;vertex[2].y = v2.y;vertex[2].z = v2.z;
	vertex[2].nx = n.x;vertex[2].ny = n.y;vertex[2].nz = n.z;
	vertex[2].color = D3DCOLOR_ARGB(255,255,255,255);

	m_pDeviceContext->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
	m_pDeviceContext->DrawPrimitiveUP((D3DPRIMITIVETYPE)GetPrimitiveType(CRender::DRAWPRIMITIVE::DRAW_PRIMITIVE_TRIANGLE_LIST),1,vertex,sizeof(vt_struct));
}

D3DFORMAT CD3D9Render::GetFormat(uint32 format)
{
	D3DFORMAT formats[] = 
	{
		D3DFMT_UNKNOWN,

		// Unsigned formats
		D3DFMT_L8,
		D3DFMT_A8L8,
		D3DFMT_X8R8G8B8,
		D3DFMT_A8R8G8B8,

		D3DFMT_L16,
		D3DFMT_G16R16,
		D3DFMT_UNKNOWN, // RGB16 not directly supported
		D3DFMT_A16B16G16R16,

		// Signed formats
		D3DFMT_UNKNOWN,
		D3DFMT_V8U8,
		D3DFMT_UNKNOWN,
		D3DFMT_Q8W8V8U8,

		D3DFMT_UNKNOWN,
		D3DFMT_V16U16,
		D3DFMT_UNKNOWN,
		D3DFMT_Q16W16V16U16,

		// Float formats
		D3DFMT_R16F,
		D3DFMT_G16R16F,
		D3DFMT_UNKNOWN, // RGB16F not directly supported
		D3DFMT_A16B16G16R16F,

		D3DFMT_R32F,
		D3DFMT_G32R32F,
		D3DFMT_UNKNOWN, // RGB32F not directly supported
		D3DFMT_A32B32G32R32F,

		// Signed integer formats
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,

		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,

		// Unsigned integer formats
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,

		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,
		D3DFMT_UNKNOWN,

		// Packed formats
		D3DFMT_UNKNOWN, // RGBE8 not directly supported
		D3DFMT_UNKNOWN, // RGB9E5 not supported
		D3DFMT_UNKNOWN, // RG11B10F not supported
		D3DFMT_R5G6B5,
		D3DFMT_A4R4G4B4,
		D3DFMT_A2B10G10R10,

		// Depth formats
		D3DFMT_D16,
		D3DFMT_D24X8,
		D3DFMT_D24S8,
		D3DFMT_D32F_LOCKABLE,

		// Compressed formats
		D3DFMT_DXT1,
		D3DFMT_DXT3,
		D3DFMT_DXT5,
		(D3DFORMAT) '1ITA', // 3Dc 1 channel
		(D3DFORMAT) '2ITA', // 3Dc 2 channels
	};
	return formats[format];
}

uint32 CD3D9Render::GetPrimitiveType(DRAWPRIMITIVE types)
{
	uint32 ret = 0;
	switch(types)
	{
		case DRAW_PRIMITIVE_TRIANGLE_LIST:
			ret = D3DPT_TRIANGLELIST;
			break;
		case DRAW_PRIMITIVE_TRIANGLE_STRIP:
			ret = D3DPT_TRIANGLESTRIP;
			break;
		case DRAW_PRIMITIVE_LINES:
			ret = D3DPT_LINELIST;
			break;
		case DRAW_PRIMITIVE_LINES_STRIP:
			ret = D3DPT_LINESTRIP;
			break;
		case DRAW_PRIMITIVE_POINTS:
			ret = D3DPT_POINTLIST;
			break;
	}
	return ret;
}


D3DFORMAT CD3D9Render::GetIndexFormat(INDEXFORMAT format)
{
	D3DFORMAT ret;
	switch(format)
	{
		case INDEX_FORMAT16:
			ret = D3DFMT_INDEX16;
			break;
		case INDEX_FORMAT32:
			ret = D3DFMT_INDEX32;
			break;
	}
	return ret;
}

uint32 CD3D9Render::GetIndexFormatSize(INDEXFORMAT format)
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


uint32 CD3D9Render::GetBufferAccess(BUFFERACCESS data)
{
	uint32 ret = 0;
	switch(data)
	{
		case STATIC:
			ret = 0;
			break;
		case DYNAMIC:
			ret = D3DUSAGE_DYNAMIC;
			break;
		case STREAM:
			ret = D3DUSAGE_DYNAMIC;
			break;
	}
	return ret;
}


void CD3D9Render::SetBlending(BLENDING src,BLENDING dst)
{
	switch(src)
	{
		case ZERO:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
			break;
		case ONE:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
			break;
		case SRC_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
			break;
		case SRC_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
			break;
		case DST_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTALPHA);
			break;
		case DST_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_DESTCOLOR);
			break;
		case SRC_ALPHA_SATURATE:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHASAT);
			break;
		case ONE_MINUS_SRC_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCCOLOR);
			break;
		case ONE_MINUS_SRC_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);
			break;
		case ONE_MINUS_DST_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTCOLOR);
			break;
		case ONE_MINUS_DST_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVDESTALPHA);
			break;
		default:
			break;		
		//m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHSRCALPHA);
		//m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHINVSRCALPHA);
		//m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BLENDFACTOR);
		//m_pDeviceContext->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVBLENDFACTOR);
	}

	switch(dst)
	{
		case ZERO:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
			break;
		case ONE:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
			break;
		case SRC_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCCOLOR);
			break;
		case SRC_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);
			break;
		case DST_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTALPHA);
			break;
		case DST_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
			break;
		case SRC_ALPHA_SATURATE:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHASAT);
			break;
		case ONE_MINUS_SRC_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);
			break;
		case ONE_MINUS_SRC_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
			break;
		case ONE_MINUS_DST_COLOR:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
			break;
		case ONE_MINUS_DST_ALPHA:
			m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTALPHA);
			break;
		default:
			break;
		//m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_BOTHSRCALPHA);
		//m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_BOTHINVSRCALPHA);
		//m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_BLENDFACTOR);
		//m_pDeviceContext->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
	}
}

void CD3D9Render::SetBlendingOp(BLENDING data)
{
	switch(data)
	{
		case ADD:
			m_pDeviceContext->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
			break;
		case SUBTRACT:
			m_pDeviceContext->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_SUBTRACT);
			break;
		case REVERSE_SUBTRACT:
			m_pDeviceContext->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
			break;
		case MIN:
			m_pDeviceContext->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_MIN);
			break;
		case MAX:
			m_pDeviceContext->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_MAX);
			break;
		default:
			break;
	}
}

void CD3D9Render::SetBlendingConstantColor(uint32 r,uint32 g,uint32 b,uint32 a)
{
	m_pDeviceContext->SetRenderState(D3DRS_BLENDFACTOR,D3DCOLOR_RGBA(r,g,b,a));
}

void CD3D9Render::SetShadeModel(SHADEMODEL data)
{
	switch(data)
	{
		case FLAT:
			m_pDeviceContext->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_FLAT);
			break;
		case SMOOTH:
			m_pDeviceContext->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);
			break;
	}
}

void CD3D9Render::SetDepthTest(DEPTHTEST data)
{
	switch(data)
	{
		case NEVER:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_NEVER);
			break;
		case LESS:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESS);
			break;
		case EQUAL:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_EQUAL);
			break;
		case LEQUAL:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
			break;
		case GREATER:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_GREATER);
			break;
		case NOTEQUAL:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_NOTEQUAL);
			break;
		case GEQUAL:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_GREATEREQUAL);
			break;
		case ALWAYS:
			m_pDeviceContext->SetRenderState(D3DRS_ZFUNC,D3DCMP_ALWAYS);
			break;
	}
}


void CD3D9Render::SetCulling(CULLING data)
{
	switch(data)
	{
		case CULL_BACK:
			m_pDeviceContext->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);
			break;
		case CULL_FRONT:
			m_pDeviceContext->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
			break;
	}
}


void CD3D9Render::SetFillMode(FILLMODE data)
{
	switch(data)
	{
		case SOLID:
			m_pDeviceContext->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
			break;
		case WIREFRAME:
			m_pDeviceContext->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
			break;
		case POINT:
			m_pDeviceContext->SetRenderState(D3DRS_FILLMODE,D3DFILL_POINT);
			break;
	}
}

void CD3D9Render::SetColor(Float32 r,Float32 g,Float32 b,Float32 a)
{
	m_pDeviceContext->SetRenderState( D3DRS_TEXTUREFACTOR , D3DCOLOR_ARGB(uint32(a),uint32(r),uint32(g),uint32(b)));
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR);

	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TFACTOR);
}

void CD3D9Render::UnSetColor()
{
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_CURRENT); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_CURRENT); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_TEXTURE); 
	m_pDeviceContext->SetTextureStageState(0,D3DTSS_COLOROP, D3DTOP_MODULATE); 
}

void CD3D9Render::OnOffBlending(Bool state)
{
	switch(state)
	{
		case 0:
			m_pDeviceContext->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
			break;
		case 1:
			m_pDeviceContext->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			break;
	}
}

void CD3D9Render::OnOffCulling(Bool state)
{
	switch(state)
	{
		case 0:
			m_pDeviceContext->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
			break;
		//case 1:	
		//	break;
	}
}

void CD3D9Render::OnOffDepthTest(Bool state)
{
	switch(state)
	{
		case 0:
			m_pDeviceContext->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			m_pDeviceContext->SetRenderState( D3DRS_ZENABLE, FALSE );
			break;
		case 1:
			m_pDeviceContext->SetRenderState( D3DRS_ZENABLE, TRUE );
			m_pDeviceContext->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			break;
	}
}

void CD3D9Render::OnOffLighting(Bool state)
{
	switch(state)
	{
		case 0:
			m_pDeviceContext->SetRenderState(D3DRS_LIGHTING,FALSE);
			break;
		case 1:
			m_pDeviceContext->SetRenderState(D3DRS_LIGHTING,TRUE);
			break;
	}
}

void CD3D9Render::OnOffScissorsTest(Bool state)
{
	switch(state)
	{
		case 0:
			m_pDeviceContext->SetRenderState(D3DRS_SCISSORTESTENABLE,FALSE);
			break;
		case 1:
			m_pDeviceContext->SetRenderState(D3DRS_SCISSORTESTENABLE,TRUE);
			break;
	}
}

void CD3D9Render::SetScissors(int32 x,int32 y,int32 w,int32 h)
{
	RECT rect;
	
	rect.top = y;
	rect.bottom = y+h;
	
	rect.left = x;
	rect.right = x+w;
	
	m_pDeviceContext->SetScissorRect(&rect);
}
/*
*/
