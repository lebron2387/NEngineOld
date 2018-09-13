#pragma once

#include "Render.h"

class CD3D9Render : public CRender
{
public:
	CD3D9Render(void);
	~CD3D9Render(void);
	
	Bool Init(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync,HWND hwnd);
	void DeInit(Bool fullscreen,HWND hwnd);

	void RenderStart();
	void Render();
	void RenderEnd();
	void SetProjection(Float32 fov, Float32 aspect,Float32 znear,Float32 zfar);
	void SetLook(Vec3f pos,Vec3f up,Vec3f view);
	void SetViewport(int32 x,int32 y,int32 width,int32 height);
	__forceinline Str GetVendor(){return d3dAdapterIdentifier.Description;}
	__forceinline Str GetVersion(){return "9";}


	void SetShadeModel(SHADEMODEL data);
	//void SetAlphaTest();
	void SetBlending(BLENDING src,BLENDING dst);
	void SetBlendingOp(BLENDING data);
	void SetBlendingConstantColor(uint32 r,uint32 g,uint32 b,uint32 a);
	void SetDepthTest(DEPTHTEST data);
	//void SetStencilTest();
	void SetCulling(CULLING data);
	void SetFillMode(FILLMODE data);
	void SetColor(Float32 r,Float32 g,Float32 b,Float32 a);
	void UnSetColor();

	void OnOffBlending(Bool state);
	void OnOffCulling(Bool state);
	void OnOffDepthTest(Bool state);
	void OnOffLighting(Bool state);
	void OnOffScissorsTest(Bool state);

	void SetScissors(int32 x,int32 y,int32 w,int32 h);

	uint32 GetPrimitiveType(DRAWPRIMITIVE types);
	D3DFORMAT GetFormat(uint32 format);
	uint32 GetBufferAccess(BUFFERACCESS data);
	D3DFORMAT GetIndexFormat(INDEXFORMAT format);
	uint32 GetIndexFormatSize(INDEXFORMAT format);

	//Moving 2D
	void Scale2D(Float32 x,Float32 y);
	void AddScale2D(Float32 x,Float32 y);
	void Translate2D(Float32 x,Float32 y);
	void AddTranslate2D(Float32 x,Float32 y);
	void Rotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void AddRotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void TranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy);
	void AddTranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy);
	void TranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void AddTranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void ScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void AddScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void TranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void AddTranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);

	//Moving 3D
	void Scale3D(Float32 x,Float32 y,Float32 z);
	void Translate3D(Float32 x,Float32 y,Float32 z);
	void TranslateScale3D(Float32 x,Float32 y,Float32 z,Float32 sx,Float32 sy,Float32 sz);
	void Rotate3D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z);
	void SetTransformMatrix(Mat4f mat);

	//Textures
	struct TextureData
	{
		void Init()
		{
			Width = 0;
			Height = 0;
			Format = 0;
			NumMipLevels = 0;
			NumChannels = 0;
			TextureId = 0;
		}
		TEXTURETYPE Type;
		uint32 Width;
		uint32 Height;
		uint32 Format;
		uint32 Depth;
		uint32 NumMipLevels;
		uint32 NumChannels;
		LPDIRECT3DTEXTURE9 TextureId;
	};
	uint32 CreateTexture(TEXTURETYPE type,uint32 width,uint32 height,uint32 num_channels,uint32 format,uint32 depth,uint32 flags,uint32 mip_levels);
	void DestroyTexture(uint32 id);
	uint32 RemoveTexture(uint32 texture_id);
	uint32 FillPartTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 offsetx,uint32 offsety,uint32 mip_width,uint32 mip_height);
	uint32 FillTexture(uint32 texture_id,uChar *data);
	uint32 FillTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 mip_width,uint32 mip_height);
	uint32 BindTexture(uint32 texture_id,uint32 texture_stage = 0);
	uint32 UnBindTexture(uint32 texture_id,uint32 texture_stage = 0);
	uint32 GenerateMipMapLevels(uint32 texture_id,uint32 hardware = 0);
	void SetTextureFilter(TEXTUREFILTER min_filter,TEXTUREFILTER mag_filter,TEXTUREFILTER mip_filter);
	void SetTextureWrapState(TEXTUREWRAP wrap_s,TEXTUREWRAP wrap_t,TEXTUREWRAP wrap_r);

	//Meshes
	struct GeometryData
	{
		void Init()
		{
			GlobalNumVertex = 0;
			GlobalNumIndex = 0;
			NumVertex = 0;
			Vertex = new Vec3f[1];
			NumIndex = 0;
			Index = new int32[1];
			NumNormal = 0;
			Normal = new Vec3f[1];
			BiNormal = new Vec3f[1];
			Tangent = new Vec3f[1];
			TexCoords = new Vec2f[1];

			GeomertyCount = 0;
			GeomertyVertices = new uint32[1];
			GeomertyIndices = new uint32[1];
			
			NumVertexDeclAttrib = 0;
			VertexFormatShift = 0;
			memset(AttribIndex,0,6*sizeof(uint32));
			pVertextElement = new D3DVERTEXELEMENT9[1];

			NumIndexBuffers = 0;
			pIndexBuffer = new LPDIRECT3DINDEXBUFFER9[1];
		}

		void DeInit()
		{
			SAFE_DELETE_ARRAY(Vertex);
			SAFE_DELETE_ARRAY(Index)
			SAFE_DELETE_ARRAY(Normal);
			SAFE_DELETE_ARRAY(BiNormal);
			SAFE_DELETE_ARRAY(Tangent);
			SAFE_DELETE_ARRAY(TexCoords);
			SAFE_DELETE_ARRAY(GeomertyVertices);
			SAFE_DELETE_ARRAY(GeomertyIndices);
			SAFE_RELEASE(pVertexDecl);
			SAFE_DELETE_ARRAY(pVertextElement);
			SAFE_DELETE_ARRAY(pIndexBuffer);
		}

		uint32 GlobalNumVertex;
		uint32 GlobalNumIndex;

		uint32 NumVertex;
		Vec3f *Vertex;

		uint32 NumNormal;
		Vec3f *Normal;
		Vec3f *BiNormal;
		Vec3f *Tangent;
		Vec2f *TexCoords;
		

		uint32 NumTriangles;
		uint32 NumIndex;		
		INDEXFORMAT IndexFormat;
		int32 *Index;

		uint32 *GeomertyVertices;
		uint32 *GeomertyIndices;
		uint32 GeomertyCount;

		uint32 NumIndexBuffers;
		LPDIRECT3DINDEXBUFFER9 *pIndexBuffer;
		LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;

		uint32 NumVertexDeclAttrib;
		uint32 VertexFormatShift;
		uint32 AttribIndex[6];
		D3DVERTEXELEMENT9 *pVertextElement;
		IDirect3DVertexDeclaration9* pVertexDecl;
	};

	uint32 SetStartNewGeometry(uint32 geometry_id);
	uint32 AddVertexDeclaration(uint32 geometry_id,VERTEXFORMAT format,VERTEXFORMATTYPE type);
	uint32 CreateGeometry();
	void DestroyGeometry(uint32 id);
	uint32 CreateGeometryVertexBuffer(uint32 geometry_id,BUFFERACCESS access,uint32 num_vertex = 0);
	uint32 ReleaseGeometryVertexBuffer(uint32 geometry_id);
	uint32 CreateGeometryIndexBuffer(uint32 geometry_id,BUFFERACCESS access,INDEXFORMAT index_format,uint32 num_index = 0);
	uint32 ReleaseGeometryIndexBuffer(uint32 geometry_id,uint32 buffer_id);
	uint32 AddVerexToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1);
	uint32 AddTexCoordsToGeometry(uint32 geometry_id,uint32 num,Vec2f *data,uint32 object_id = -1);
	uint32 AddNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1);
	uint32 AddBiNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1);
	uint32 AddTangentToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1);
	uint32 AddIndexToGeometry(uint32 geometry_id,uint32 num,int32 *data,uint32 object_id = -1);

	uint32 LoadToVertexBufferGeometry(uint32 geometry_id);
	uint32 LoadToVertexBufferGeometry(uint32 geometry_id,uint32 object_id);
	uint32 LoadToIndexBufferGeometry(uint32 geometry_id);
	uint32 LoadToIndexBufferGeometry(uint32 geometry_id,uint32 object_id,uint32 buffer_id);
	uint32 SetDrawable(uint32 geometry_id);
	uint32 SetIndexBuffer(uint32 geometry_id,uint32 index_buffer_id);
	uint32 DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id);
	uint32 DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id,uint32 object_id);


	//Lights
	struct LightData
	{
		D3DLIGHT9 pLight;
	};

	uint32 CreateLight();
	void DestroyLight(uint32 id);
	uint32 SetLight(uint32 light_id);
	void SetLightEnable(uint32 light_id);
	void SetLightDisable(uint32 light_id);
	void SetLightType(uint32 light_id,LIGHTTYPE Type);
	void SetLightDiffuse(uint32 light_id,Vec4f Diffuse);
	void SetLightSpecular(uint32 light_id,Vec4f Specular);
	void SetLightAmbient(uint32 light_id,Vec4f Ambient);
	void SetLightPosition(uint32 light_id,Vec3f Position);
	void SetLightDirection(uint32 light_id,Vec3f Direction);
	void SetLightAttenuation0(uint32 light_id,Float32 Attenuation0);
	void SetLightAttenuation1(uint32 light_id,Float32 Attenuation1);
	void SetLightAttenuation2(uint32 light_id,Float32 Attenuation2);
	void SetLightCutoff(uint32 light_id,Float32 Cutoff);
	void SetLightExponent(uint32 light_id,Float32 Exponent);

	//Materials
	struct MaterialData
	{
		D3DMATERIAL9 pMaterial;
	};
	uint32 CreateMaterial();
	void DestroyMaterial(uint32 id);
	uint32 SetMaterial(uint32 material_id);
	void SetMaterialAmbient(uint32 material_id,Vec4f Ambient);
	void SetMaterialDiffuse(uint32 material_id,Vec4f Diffuse);
	void SetMaterialSpecular(uint32 material_id,Vec4f Specular);
	void SetMaterialEmissive(uint32 material_id,Vec4f Emissive);
	void SetMaterialPower(uint32 material_id,Float32 Power);

	//Shaders
	struct ShaderData
	{
		LPDIRECT3DVERTEXSHADER9 m_pVertexShader;
		LPD3DXCONSTANTTABLE m_pConstantTableVS;
		LPDIRECT3DPIXELSHADER9 m_pFragmentShader;
		LPD3DXCONSTANTTABLE m_pConstantTableFS;
		LPD3DXBUFFER m_pVSCode;
		LPD3DXBUFFER m_pFSCode;
		DWORD m_dwShaderFlags;
		LPD3DXBUFFER m_pBufferErrors;
		uint32 m_iErrorsStatus;
	};
	struct ShaderParameterData
	{
		uint32 m_pType;
		uint32 m_pShaderId;
		D3DXHANDLE m_pParameterId;
	};
	uint32 CreateShader(Str vertex_shader_text,Str vertex_profile,Str fragment_shader_text,Str fragment_profile);
	void DestroyShader(uint32 id);
	void LoadShader(uint32 shader_id);
	void UnLoadShader(uint32 shader_id);
	void ReleaseShader(uint32 shader_id);
	void BindShader(uint32 shader_id);
	void UnBindShader(uint32 shader_id);
	uint32 AddShaderVertexParameter(uint32 shader_id,Str name_in_shader);
	uint32 AddShaderFragmentParameter(uint32 shader_id,Str name_in_shader);
	void SetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix);
	void SetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix);
	void SetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix);

	void GetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix);
	void GetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix);
	void GetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix);

	void SetShaderParameter1i(uint32 shader_id,uint32 parameter_id,int32 param);
	void SetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void SetShaderParameter1f(uint32 shader_id,uint32 parameter_id,Float32 param);
	void SetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void SetShaderParameter1d(uint32 shader_id,uint32 parameter_id,Double32 param);
	void SetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void SetShaderParameter2i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2);
	void SetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void SetShaderParameter2f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2);
	void SetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void SetShaderParameter2d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2);
	void SetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void SetShaderParameter3i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3);
	void SetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void SetShaderParameter3f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3);
	void SetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void SetShaderParameter3d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3);
	void SetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void SetShaderParameter4i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3,int32 param4);
	void SetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void SetShaderParameter4f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3,Float32 param4);
	void SetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void SetShaderParameter4d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3,Double32 param4);
	void SetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param);

	void GetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void GetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void GetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void GetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void GetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void GetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void GetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void GetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void GetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void GetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param);
	void GetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param);
	void GetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param);
	void SetShaderParameterTexture(uint32 shader_id,uint32 parameter_id,uint32 texture_id);
	Str GetShaderSupportedVertexProfiles();
	Str GetShaderSupportedFragmentProfiles();
	void SetShaderProfile(Str vertex_profile,Str fragment_profile);
	void GetShaderLastError(uint32 shader_id,Str situation);

	//Transforms states
	void SwitchTo2D();
	void SwitchTo3D();
	void PushMatrix();
	void PopMatrix();
	Mat4f GetTransformStateMatrix(TRANSFORMSTATES state);
	void SetTransformStateMatrix(TRANSFORMSTATES state,Mat4f mat);

	//DebugDraw
	void DrawPoint(Vec3f point,Float32 size);
	void DrawLine(Vec3f start,Vec3f end);
	void DrawTriangle(Vec3f v0,Vec3f v1,Vec3f v2,Vec3f n);

	LPDIRECT3D9             m_pRenderContext; 
	LPDIRECT3DDEVICE9       m_pDeviceContext;

protected:
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	ID3DXMatrixStack *m_pMatrixStack;

	ShaderData *m_pShaders;
	ShaderParameterData *m_pShaderParameters;
	LightData *m_pLights;
	MaterialData *m_pMaterials;
	GeometryData *m_pGeometrys;
	TextureData *m_pTextures;

	CGprogram *m_pCgVertexProgram;
    CGprogram *m_pCgFragmentProgram;
	CGparameter *m_pParameters;

private:

	D3DDISPLAYMODE			d3dDisplayMode;
	D3DPRESENT_PARAMETERS	d3dPresentParameters;
	D3DADAPTER_IDENTIFIER9  d3dAdapterIdentifier;
	D3DCAPS9 d3dCaps;
};
