#pragma once

#include "../Core/Engine.h"

#define TEX_DIM(tex,cmptex,dim,cmpdim) ((tex == cmptex && dim == cmpdim)?(1):(0))
#define MAX_TEXTURES 8
#define MAX_CG_PARAMETERS 100

class CRender
{
public:
	CRender(void);
	virtual ~CRender(void){};

	enum RENDER_NAME
	{
		OPENGL_RENDER = 1,
		DIRECT3D9_RENDER = 2
	};

	enum SHADER_LANGUAGE_TYPE
	{
		SHADER_LANGUAGE_TYPE_DEFAULT = 0,
		SHADER_LANGUAGE_TYPE_CG = 1,
		SHADER_LANGUAGE_TYPE_D3D9_HLSL = 2,
		SHADER_LANGUAGE_TYPE_D3D9_ASM = 3,
		SHADER_LANGUAGE_TYPE_GL_GLSL = 4,
		SHADER_LANGUAGE_TYPE_GL_ARB = 5,
		SHADER_LANGUAGE_TYPE_GL_NV = 6,
	};

	enum FORMAT
	{
		FORMAT_UNKNOWN  = 0,

		// Unsigned formats
		FORMAT_R8       = 1,
		FORMAT_RG8      = 2,
		FORMAT_RGB8     = 3,
		FORMAT_RGBA8    = 4,

		FORMAT_R16      = 5,
		FORMAT_RG16     = 6,
		FORMAT_RGB16    = 7,
		FORMAT_RGBA16   = 8,

		// Signed formats
		FORMAT_R8S      = 9,
		FORMAT_RG8S     = 10,
		FORMAT_RGB8S    = 11,
		FORMAT_RGBA8S   = 12,

		FORMAT_R16S     = 13,
		FORMAT_RG16S    = 14,
		FORMAT_RGB16S   = 15,
		FORMAT_RGBA16S  = 16,

		// Float formats
		FORMAT_R16F     = 17,
		FORMAT_RG16F    = 18,
		FORMAT_RGB16F   = 19,
		FORMAT_RGBA16F  = 20,

		FORMAT_R32F     = 21,
		FORMAT_RG32F    = 22,
		FORMAT_RGB32F   = 23,
		FORMAT_RGBA32F  = 24,

		// Signed integer formats
		FORMAT_R16I     = 25,
		FORMAT_RG16I    = 26,
		FORMAT_RGB16I   = 27,
		FORMAT_RGBA16I  = 28,

		FORMAT_R32I     = 29,
		FORMAT_RG32I    = 30,
		FORMAT_RGB32I   = 31,
		FORMAT_RGBA32I  = 32,

		// Unsigned integer formats
		FORMAT_R16UI    = 33,
		FORMAT_RG16UI   = 34,
		FORMAT_RGB16UI  = 35,
		FORMAT_RGBA16UI = 36,

		FORMAT_R32UI    = 37,
		FORMAT_RG32UI   = 38,
		FORMAT_RGB32UI  = 39,
		FORMAT_RGBA32UI = 40,

		// Packed formats
		FORMAT_RGBE8    = 41,
		FORMAT_RGB9E5   = 42,
		FORMAT_RG11B10F = 43,
		FORMAT_RGB565   = 44,
		FORMAT_RGBA4    = 45,
		FORMAT_RGB10A2  = 46,

		// Depth formats
		FORMAT_D16      = 47,
		FORMAT_D24      = 48,
		FORMAT_D24S8    = 49,
		FORMAT_D32F     = 50,

		// Compressed formats
		FORMAT_DXT1     = 51,
		FORMAT_DXT3     = 52,
		FORMAT_DXT5     = 53,
		FORMAT_ATI1N    = 54,
		FORMAT_ATI2N    = 55,
	};

	enum FILE_TYPE
	{
		FILE_TYPE_BMP = 1,
		FILE_TYPE_JPG = 2,
		FILE_TYPE_TGA = 3,
		FILE_TYPE_PNG = 4,
		FILE_TYPE_DDS = 5,
		FILE_TYPE_PPM = 6,
		FILE_TYPE_DIB = 7,
		FILE_TYPE_HDR = 8,
		FILE_TYPE_PFM = 9,
		FILE_TYPE_XPM = 10,
	};

	enum BLENDING
	{
		ZERO = 0,
		ONE = 1,
		SRC_COLOR = 2,
		ONE_MINUS_SRC_COLOR = 3,
		DST_COLOR = 4,
		ONE_MINUS_DST_COLOR = 5,
		SRC_ALPHA = 6,
		ONE_MINUS_SRC_ALPHA = 7,
		DST_ALPHA = 8,
		ONE_MINUS_DST_ALPHA = 9,
		SRC_ALPHA_SATURATE = 10,
		ADD = 11,
		SUBTRACT = 12,
		REVERSE_SUBTRACT = 13,
		MIN = 14,
		MAX = 15,
	};

	enum DEPTHTEST
	{
		NEVER = 0,
		LESS = 1,
		EQUAL = 2,
		LEQUAL = 3,
		GREATER = 4,
		NOTEQUAL = 5,
		GEQUAL = 6,
		ALWAYS = 7,
	};

	enum STENCILTEST
	{
		KEEP = 0,
		SET_ZERO = 1,
		REPLACE = 2,
		INVERT = 3,
		INCR = 4,
		DECR = 5,
		INCR_SAT = 6,
		DECR_SAT = 7,
	};

	enum TRANSFORMSTATES
	{
		VIEW = 0,
		PROJECTION = 1,
		TEXTURE0 = 2,
		TEXTURE1 = 3,
		TEXTURE2 = 4,
		TEXTURE3 = 5,
		TEXTURE4 = 6,
		TEXTURE5 = 7,
		TEXTURE6 = 8,
		TEXTURE7 = 9,
	};

	enum CULLING
	{
		CULL_BACK = 0,
		CULL_FRONT = 1,
	};

	enum FILLMODE
	{
		SOLID = 0,
		WIREFRAME = 1,
		POINT = 2,
	};

	enum SHADEMODEL
	{
		SMOOTH = 0,
		FLAT = 1,
	};

	enum 
	{
		HW_RENDER = 0,
		SW_RENDER = 1,
	};

	enum BUFFERACCESS 
	{
		STATIC = 0,
		DYNAMIC = 1,
		STREAM = 2,
	};

	enum VERTEXFORMATTYPE
	{
		VERTEX_FORMAT_TYPE_FLOAT = 0,
		VERTEX_FORMAT_TYPE_FLOAT2 = 1,
		VERTEX_FORMAT_TYPE_FLOAT3 = 2,
		VERTEX_FORMAT_TYPE_FLOAT4 = 3,
		//GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE
	};

	enum INDEXFORMAT
	{
		INDEX_FORMAT16 = 0,
		INDEX_FORMAT32 = 1,
	};

	enum VERTEXFORMAT
	{
		VERTEX_FORMAT_POSITION = 0,
		VERTEX_FORMAT_NORMAL = 1,
		VERTEX_FORMAT_BINORMAL = 2,
		VERTEX_FORMAT_TANGENT = 3,
		VERTEX_FORMAT_TEXCOORD = 4,
	};

	enum DRAWPRIMITIVE
	{
		DRAW_PRIMITIVE_TRIANGLE_LIST = 0,
		DRAW_PRIMITIVE_TRIANGLE_STRIP = 1,
		DRAW_PRIMITIVE_LINES = 2,
		DRAW_PRIMITIVE_LINES_STRIP = 3,
		DRAW_PRIMITIVE_POINTS = 4,
	};

	enum TEXTURETYPE
	{
		TEXTURE_TYPE_1D = 0,
		TEXTURE_TYPE_2D = 1,
		TEXTURE_TYPE_3D = 2,
		TEXTURE_TYPE_CUBE = 3,
	};

	enum TEXTUREFILTER
	{
		TEXTURE_FILTER_NONE = 0,
		TEXTURE_FILTER_NEAREST = 1,
		TEXTURE_FILTER_LINEAR = 2,
		TEXTURE_FILTER_ANISOTROPY1 = 3,
		TEXTURE_FILTER_ANISOTROPY2 = 4,
		TEXTURE_FILTER_ANISOTROPY4 = 5,
		TEXTURE_FILTER_ANISOTROPY8 = 6,
		TEXTURE_FILTER_ANISOTROPY16 = 7,
	};

	enum TEXTUREWRAP
	{
		TEXTURE_WRAP_NONE = 0,
		TEXTURE_WRAP_WRAP = 1,
		TEXTURE_WRAP_CLAMP = 2,
		TEXTURE_WRAP_MIRROR = 3,
		TEXTURE_WRAP_BORDER = 4,
	};

	enum TEXTURESTATE
	{
		TEXTURE_STATE_NONE = 0,
		TEXTURE_STATE_MODULATE = 1,
		TEXTURE_STATE_ADD = 3,
		TEXTURE_STATE_ADD_SIGNED = 4,
		TEXTURE_STATE_SUBTRACT = 5,
	};

	enum LIGHTTYPE
	{
		LIGHT_TYPE_POINT = 0,
		LIGHT_TYPE_SPOT = 1,
	};

	virtual Bool Init(int32 screenwidth,int32 screenheight,Bool fullscreen,int32 bpp,Bool vsync,HWND hwnd){return false;}
	virtual void DeInit(Bool fullscreen,HWND hwnd){}
	virtual void RenderStart(){}
	virtual void Render(){}
	virtual void RenderEnd(){}
	virtual void SetProjection(Float32 fov, Float32 aspect,Float32 znear,Float32 zfar){}
	virtual void SetLook(Vec3f pos,Vec3f up,Vec3f view){}
	virtual void SetViewport(int32 x,int32 y,int32 width,int32 height){}
	virtual Str GetVendor(){return "";}
	virtual Str GetVersion(){return "";}
	
	virtual void SetShadeModel(SHADEMODEL data){}
	//virtual void SetAlphaTest() = 0;
	virtual void SetBlending(BLENDING src,BLENDING dst){}
	virtual void SetBlendingOp(BLENDING data){}
	virtual void SetBlendingConstantColor(uint32 r,uint32 g,uint32 b,uint32 a){}
	virtual void SetDepthTest(DEPTHTEST data){}
	//virtual void SetStencilTest() = 0;
	virtual void SetCulling(CULLING data){}
	virtual void SetFillMode(FILLMODE data){}
	virtual void SetColor(Float32 r,Float32 g,Float32 b,Float32 a){}
	virtual void UnSetColor(){};

	virtual void OnOffBlending(Bool state){};
	virtual void OnOffCulling(Bool state){};
	virtual void OnOffDepthTest(Bool state){};
	virtual void OnOffLighting(Bool state){};
	virtual void OnOffScissorsTest(Bool state){};

	virtual void SetScissors(int32 x,int32 y,int32 w,int32 h){};

	__forceinline void SetRenderType(int32 render_type){m_iRenderType=render_type;}
	__forceinline int32 GetRenderType(){return m_iRenderType;}

	//Textures
	struct TextureData;
	virtual uint32 CreateTexture(TEXTURETYPE type,uint32 width,uint32 height,uint32 num_channels,uint32 format,uint32 depth,uint32 flags,uint32 mip_levels){return 0;}
	virtual void DestroyTexture(uint32 id){}
	virtual uint32 RemoveTexture(uint32 texture_id){return 0;}
	virtual uint32 FillPartTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 offsetx,uint32 offsety,uint32 mip_width,uint32 mip_height){return 0;}
	virtual uint32 FillTexture(uint32 texture_id,uChar *data){return 0;}
	virtual uint32 FillTexture(uint32 texture_id,uChar *data,uint32 mip_level,uint32 mip_width,uint32 mip_height){return 0;}
	virtual uint32 BindTexture(uint32 texture_id,uint32 texture_stage = 0){return 0;}
	virtual uint32 UnBindTexture(uint32 texture_id,uint32 texture_stage = 0){return 0;}
	virtual uint32 GenerateMipMapLevels(uint32 texture_id,uint32 hardware = 0){return 0;}
	virtual void SetTextureFilter(TEXTUREFILTER min_filter,TEXTUREFILTER mag_filter,TEXTUREFILTER mip_filter){};
	virtual void SetTextureWrapState(TEXTUREWRAP wrap_s,TEXTUREWRAP wrap_t,TEXTUREWRAP wrap_r){};

	//Geometry
	struct GeometryData;
	virtual uint32 SetStartNewGeometry(uint32 geometry_id){return 0;}
	virtual uint32 AddVertexDeclaration(uint32 geometry_id,VERTEXFORMAT format,VERTEXFORMATTYPE type){return 0;}
	virtual uint32 CreateGeometry(){return 0;}
	virtual void DestroyGeometry(uint32 id){}
	virtual uint32 CreateGeometryVertexBuffer(uint32 geometry_id,BUFFERACCESS access,uint32 num_vertex = 0){return 0;}
	virtual uint32 ReleaseGeometryVertexBuffer(uint32 geometry_id){return 0;}
	virtual uint32 CreateGeometryIndexBuffer(uint32 geometry_id,BUFFERACCESS access,INDEXFORMAT index_format,uint32 num_index = 0){return 0;}
	virtual uint32 ReleaseGeometryIndexBuffer(uint32 geometry_id,uint32 buffer_id){return 0;}
	virtual uint32 AddVerexToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1){return 0;}
	virtual uint32 AddTexCoordsToGeometry(uint32 geometry_id,uint32 num,Vec2f *data,uint32 object_id = -1){return 0;}
	virtual uint32 AddNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1){return 0;}
	virtual uint32 AddBiNormalToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1){return 0;}
	virtual uint32 AddTangentToGeometry(uint32 geometry_id,uint32 num,Vec3f *data,uint32 object_id = -1){return 0;}
	virtual uint32 AddIndexToGeometry(uint32 geometry_id,uint32 num,int32 *data,uint32 object_id = -1){return 0;}
	virtual uint32 LoadToVertexBufferGeometry(uint32 geometry_id){return 0;}
	virtual uint32 LoadToVertexBufferGeometry(uint32 geometry_id,uint32 object_id){return 0;}
	virtual uint32 LoadToIndexBufferGeometry(uint32 geometry_id){return 0;}
	virtual uint32 LoadToIndexBufferGeometry(uint32 geometry_id,uint32 object_id,uint32 buffer_id){return 0;}
	virtual uint32 SetDrawable(uint32 geometry_id){return 0;}
	virtual uint32 SetIndexBuffer(uint32 geometry_id,uint32 index_buffer_id){return 0;}
	virtual uint32 DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id){return 0;}
	virtual uint32 DrawGeometry(DRAWPRIMITIVE primitive_type,uint32 geometry_id,uint32 object_id){return 0;}

	//Lights
	struct LightData;
	virtual uint32 CreateLight(){return 0;}
	virtual void DestroyLight(uint32 id){}
	virtual uint32 SetLight(uint32 light_id){return 0;}
	virtual void SetLightEnable(uint32 light_id){}
	virtual void SetLightDisable(uint32 light_id){}
	virtual void SetLightType(uint32 light_id,LIGHTTYPE Type){}
	virtual void SetLightDiffuse(uint32 light_id,Vec4f Diffuse){}
	virtual void SetLightSpecular(uint32 light_id,Vec4f Specular){}
	virtual void SetLightAmbient(uint32 light_id,Vec4f Ambient){}
	virtual void SetLightPosition(uint32 light_id,Vec3f Position){}
	virtual void SetLightDirection(uint32 light_id,Vec3f Direction){}
	virtual void SetLightAttenuation0(uint32 light_id,Float32 Attenuation0){}
	virtual void SetLightAttenuation1(uint32 light_id,Float32 Attenuation1){}
	virtual void SetLightAttenuation2(uint32 light_id,Float32 Attenuation2){}
	virtual void SetLightCutoff(uint32 light_id,Float32 Cutoff){}
	virtual void SetLightExponent(uint32 light_id,Float32 Exponent){}

	//Materials
	struct MaterialData;
	virtual uint32 CreateMaterial(){return 0;}
	virtual void DestroyMaterial(uint32 id){}
	virtual uint32 SetMaterial(uint32 material_id){return 0;}
	virtual void SetMaterialAmbient(uint32 material_id,Vec4f Ambient){}
	virtual void SetMaterialDiffuse(uint32 material_id,Vec4f Diffuse){}
	virtual void SetMaterialSpecular(uint32 material_id,Vec4f Specular){}
	virtual void SetMaterialEmissive(uint32 material_id,Vec4f Emissive){}
	virtual void SetMaterialPower(uint32 material_id,Float32 Power){}

	//Shaders
	struct ShaderData;
	struct ShaderParameterData;
	virtual uint32 CreateShader(Str vertex_shader_text,Str vertex_profile,Str fragment_shader_text,Str fragment_profile){return 0;}
	virtual void DestroyShader(uint32 id){}
	virtual void LoadShader(uint32 shader_id){}
	virtual void UnLoadShader(uint32 shader_id){}
	virtual void ReleaseShader(uint32 shader_id){}
	virtual void BindShader(uint32 shader_id){}
	virtual void UnBindShader(uint32 shader_id){}
	virtual uint32 AddShaderVertexParameter(uint32 shader_id,Str name_in_shader){return 0;}
	virtual uint32 AddShaderFragmentParameter(uint32 shader_id,Str name_in_shader){return 0;}
	virtual void SetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix){}
	virtual void SetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix){}
	virtual void SetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix){}
	virtual void GetShaderMatrixParameteri(uint32 shader_id,uint32 parameter_id,int32 *matrix){}
	virtual void GetShaderMatrixParameterf(uint32 shader_id,uint32 parameter_id,Float32 *matrix){}
	virtual void GetShaderMatrixParameterd(uint32 shader_id,uint32 parameter_id,Double32 *matrix){}
	virtual void SetShaderParameter1i(uint32 shader_id,uint32 parameter_id,int32 param){}
	virtual void SetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void SetShaderParameter1f(uint32 shader_id,uint32 parameter_id,Float32 param){}
	virtual void SetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void SetShaderParameter1d(uint32 shader_id,uint32 parameter_id,Double32 param){}
	virtual void SetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void SetShaderParameter2i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2){}
	virtual void SetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void SetShaderParameter2f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2){}
	virtual void SetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void SetShaderParameter2d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2){}
	virtual void SetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void SetShaderParameter3i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3){}
	virtual void SetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void SetShaderParameter3f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3){}
	virtual void SetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void SetShaderParameter3d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3){}
	virtual void SetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void SetShaderParameter4i(uint32 shader_id,uint32 parameter_id,int32 param1,int32 param2,int32 param3,int32 param4){}
	virtual void SetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void SetShaderParameter4f(uint32 shader_id,uint32 parameter_id,Float32 param1,Float32 param2,Float32 param3,Float32 param4){}
	virtual void SetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void SetShaderParameter4d(uint32 shader_id,uint32 parameter_id,Double32 param1,Double32 param2,Double32 param3,Double32 param4){}
	virtual void SetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void GetShaderParameter1iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void GetShaderParameter1fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void GetShaderParameter1dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void GetShaderParameter2iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void GetShaderParameter2fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void GetShaderParameter2dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void GetShaderParameter3iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void GetShaderParameter3fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void GetShaderParameter3dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void GetShaderParameter4iv(uint32 shader_id,uint32 parameter_id,int32 *param){}
	virtual void GetShaderParameter4fv(uint32 shader_id,uint32 parameter_id,Float32 *param){}
	virtual void GetShaderParameter4dv(uint32 shader_id,uint32 parameter_id,Double32 *param){}
	virtual void SetShaderParameterTexture(uint32 shader_id,uint32 parameter_id,uint32 texture_id){}
	virtual void GetShaderLastError(uint32 shader_id,Str situation){}
	virtual Str GetShaderSupportedVertexProfiles(){return 0;}
	virtual Str GetShaderSupportedFragmentProfiles(){return 0;}
	virtual void SetShaderProfile(Str vertex_profile,Str fragment_profile){}
	__forceinline void EnableShadersDebugMode(){m_bShadersDebugMode=true;}
	__forceinline void DisableShadersDebugMode(){m_bShadersDebugMode=false;}
	__forceinline void SetShaderLanguage(uint32 shader_language){m_iShaderLanguage=shader_language;}
	__forceinline uint32 GetShaderLanguage(){return m_iShaderLanguage;}

	//Moving 2D
	virtual void Scale2D(Float32 x,Float32 y){};
	virtual void AddScale2D(Float32 x,Float32 y){};
	virtual void Translate2D(Float32 x,Float32 y){};
	virtual void AddTranslate2D(Float32 x,Float32 y){};
	virtual void Rotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void AddRotate2D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void TranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy){};
	virtual void AddTranslateScale2D(Float32 x,Float32 y,Float32 sx,Float32 sy){};
	virtual void TranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void AddTranslateRotate2D(Float32 x,Float32 y,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void ScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void AddScaleRotate2D(Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void TranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void AddTranslateScaleRotate2D(Float32 x,Float32 y,Float32 sx,Float32 sy,Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};

	//Moving 3D
	virtual void Scale3D(Float32 x,Float32 y,Float32 z){};
	virtual void Translate3D(Float32 x,Float32 y,Float32 z){};
	virtual void TranslateScale3D(Float32 x,Float32 y,Float32 z,Float32 sx,Float32 sy,Float32 sz){};
	virtual void Rotate3D(Float32 angle,Float32 axis_x,Float32 axis_y,Float32 axis_z){};
	virtual void SetTransformMatrix(Mat4f mat){}

	//Managment
	virtual void AddRemoveBuffer(uint32 id){};
	virtual void RemoveRemoveBuffer(uint32 id){};

	//Transforms states
	virtual void SwitchTo2D(){};
	virtual void SwitchTo3D(){};
	virtual void PushMatrix(){};
	virtual void PopMatrix(){};
	virtual Mat4f GetTransformStateMatrix(TRANSFORMSTATES state){return 0;}
	virtual void SetTransformStateMatrix(TRANSFORMSTATES state,Mat4f mat){}
	__forceinline uint32 GetRenderName(){return m_iRenderName;}
	__forceinline uint32 GetScreenWidth(){return m_iScreenWidth;}
	__forceinline uint32 GetScreenHeight(){return m_iScreenHeight;}
	//DebugDraw
	virtual void DrawPoint(Vec3f point,Float32 size){};
	virtual void DrawLine(Vec3f start,Vec3f end){};
	virtual void DrawTriangle(Vec3f v0,Vec3f v1,Vec3f v2,Vec3f n){};

	

protected:

	uint32 m_iLightCount;
	uint32 *m_pLightRemove;
	uint32 m_iMaterialsCount;
	uint32 *m_pMaterialRemove;
	uint32 m_iGeometrysCount;
	uint32 *m_pGeometryRemove;
	uint32 m_iTexturesCount;
	uint32 *m_pTextureRemove;
	uint32 m_iRenderType;
	uint32 m_iRenderName;
	uint32 m_iScreenWidth;
	uint32 m_iScreenHeight;
	//Shader variables
	Bool m_bShadersDebugMode;
	uint32 m_iShaderLanguage;
	uint32 m_iShadersCount;
	uint32 m_iCgShadersCount;
	uint32 m_iParametersCount;
	uint32 m_iCgParametersCount;
	CGcontext m_pCgContext;
	CGprofile m_pCgVertexProfile;
    CGprofile m_pCgFragmentProfile;
	uint32 *m_pShaderRemove;
};
