#pragma once


#include "../Core/Engine.h"


class CMesh
{
public:
	CMesh();
	~CMesh();

	enum
	{
		FILE_TYPE_3DS = 1,
		FILE_TYPE_OBJ = 2,
		FILE_TYPE_ASE = 3,
		FILE_TYPE_DAE = 4,
		FILE_TYPE_COLLADA = 5,
		FILE_TYPE_X = 6,
	};

	enum MESH_DATA_TYPE
	{
		MESH_DATA_TYPE_SINGLE_TRIANGLE = 1,
		MESH_DATA_TYPE_STRIPED_TRIANGLE = 2
	};
	

	//Load
	uint32 LoadMeshFromFile3DS(Str filename);
	uint32 LoadMeshFromFileOBJ(Str filename);
	uint32 LoadMeshFromFileASE(Str filename);
	uint32 LoadMeshFromFileDAE(Str filename);
	uint32 LoadMeshFromFileCollada(Str filename);
	uint32 LoadMeshFromFileX(Str filename);
	

	//Save
	uint32 SaveMeshInFile3DS(Str filename);
	uint32 SaveMeshInFileOBJ(Str filename);
	uint32 SaveMeshInFileASE(Str filename);
	uint32 SaveMeshInFileDAE(Str filename);
	uint32 SaveMeshInFileCollada(Str filename);
	uint32 SaveMeshInFileX(Str filename);

	//other
	uint32 BuildNormals();
	uint32 BuildTangentSpace();
	
	//Convert
	uint32 ConvertTo3DS();
	uint32 ConvertToOBJ();
	uint32 ConvertToASE();
	uint32 ConvertToDAE();
	uint32 ConvertToStripTriangles();
	uint32 ConvertToSingleTriangles();

	//Get\Set Data
	__forceinline uint32 GetNumVertex(){return m_iNumVertex;}
	__forceinline uint32 GetNumTriangles(){return m_iNumTriangles;}
	__forceinline uint32 GetType(){return m_iType;}
	__forceinline uint32 GetIndex(uint32 id){return Index[id];}
	__forceinline Vec3f GetVertex(uint32 id){return Vertex[id];}
	__forceinline Vec3f GetNormal(uint32 id){return Normal[id];}
	__forceinline Vec3f GetBiNormal(uint32 id){return BiNormal[id];}
	__forceinline Vec3f GetTangent(uint32 id){return Tangent[id];}
	__forceinline Vec2f GetTexUV(uint32 id){return TexUV[id];}
	
	
private:
	uint16 GetXFileBlockID();
	uint16 ProcessBlock();
	enum 
	{
		LOAD_3DS_CHUNK_MAIN = 0x4d4d,
		LOAD_3DS_CHUNK_OBJMESH = 0x3d3d,
		LOAD_3DS_CHUNK_OBJBLOCK = 0x4000,
		LOAD_3DS_CHUNK_TRIMESH = 0x4100,
		LOAD_3DS_CHUNK_VERTLIST = 0x4110,
		LOAD_3DS_CHUNK_FACELIST = 0x4120,
		LOAD_3DS_CHUNK_MAPLIST = 0x4140,
		LOAD_3DS_CHUNK_SMOOTHLIST = 0x4150
	};

	struct x_file_header 
	{
		unsigned long Magic;
		unsigned short Major_Version;
		unsigned short Minor_Version;
		unsigned long Format;
		unsigned long Float_Size;
	};
	enum
	{
		X_NAME = 1,
		X_STRING = 2,
		X_INTEGER = 3,
		X_GUID = 5,
		X_INTEGER_LIST = 6,
		X_FLOAT_LIST = 7,
		X_OBRACE = 10,
		X_EBRACE = 11,
		X_OPAREN = 12,
		X_CPAREN = 13,
		X_OBRACKET = 14,
		X_CBRACKET = 15,
		X_OANGLE = 16,
		X_CANGLE = 17,
		X_DOT = 18,
		X_COMMA = 19,
		X_SEMICOLON = 20,
		X_TOKEN_TEMPLATE = 31,
		X_WORD = 40,
		X_DWORD = 41,
		X_FLOAT = 42,
		X_DOUBLE = 43,
		X_CHAR = 44,
		X_UCHAR = 45,
		X_SWORD = 46,
		X_SDWORD = 47,
		X_VOID = 48,
		X_LPSTR = 49,
		X_UNICODE = 50,
		X_CSTRING = 51,
		X_ARRAY = 52,
		//Personal tokens
		X_ERROR = -1,
		X_TEMPLATE = 60,
		X_HEADER = 61,
		X_FRAME = 62,
		X_FRAMETRANSFORMMATRIX = 63,
		X_MESH = 64,
		X_MESHTEXTURECOORDS = 65,
		X_MESHMATERIALLIST = 66,
		X_MATERIAL = 67,
		X_SKINMESHHEADER = 68,
		X_SKINWEIGHTS = 69,
		X_TEXTUREFILENAME = 70,
		X_MESHNORMALS = 71,
		X_ANIMATIONSET = 72,
		X_ANIMATION = 73,
		X_ANIMATIONKEY = 74,
		X_COMMENT = 254,
		X_UNKNOWN = 255 //unknown block
	};

	struct xfile_template_id
	{
		Str TextID;
		uint16 TemplateID;
	};

	xfile_template_id x_file_templates[15];

public:
	File m_pFile;
	uint32 m_iNumVertex;
	Vec3f *Vertex;
	Vec3f *Normal;
	Vec3f *BiNormal;
	Vec3f *Tangent;
	uint32 m_iNumTriangles;
	int32 *Index;
	Vec2f *TexUV;
	uint32 m_iType;
};

#define XOFFILE_FORMAT_MAGIC \
    ((long)'x' + ((long)'o' << 8) + ((long)'f' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_VERSION03 \
    ((long)'0' + ((long)'3' << 8))

#define XOFFILE_FORMAT_VERSION02 \
    ((long)'0' + ((long)'3' << 8))

#define XOFFILE_FORMAT_BINARY \
    ((long)'b' + ((long)'i' << 8) + ((long)'n' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_TEXT   \
    ((long)'t' + ((long)'x' << 8) + ((long)'t' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_COMPRESSED \
    ((long)'c' + ((long)'m' << 8) + ((long)'p' << 16) + ((long)' ' << 24))

#define XOFFILE_FORMAT_FLOAT_BITS_32 \
    ((long)'0' + ((long)'0' << 8) + ((long)'3' << 16) + ((long)'2' << 24))

#define XOFFILE_FORMAT_FLOAT_BITS_64 \
    ((long)'0' + ((long)'0' << 8) + ((long)'6' << 16) + ((long)'4' << 24))
