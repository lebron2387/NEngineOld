#include "Mesh.h"

CMesh::CMesh()
{
	m_iNumVertex = 0;
	m_iNumTriangles = 0;
	m_iType = 0;

	Normal = new Vec3f[1];
	BiNormal = new Vec3f[1];
	Tangent = new Vec3f[1];

	
	
	x_file_templates[0].TextID = Str("template");
	x_file_templates[0].TemplateID = X_TEMPLATE;
	x_file_templates[1].TextID = Str("FrameTransformMatrix");
	x_file_templates[1].TemplateID = X_FRAMETRANSFORMMATRIX;
	x_file_templates[2].TextID = Str("Frame");
	x_file_templates[2].TemplateID = X_FRAME;
	x_file_templates[3].TextID = Str("XSkinMeshHeader");
	x_file_templates[3].TemplateID = X_SKINMESHHEADER;
	x_file_templates[4].TextID = Str("MeshTextureCoords");
	x_file_templates[4].TemplateID = X_MESHTEXTURECOORDS;
	x_file_templates[5].TextID = Str("MeshMaterialList");
	x_file_templates[5].TemplateID = X_MESHMATERIALLIST;
	x_file_templates[6].TextID = Str("MeshNormals");
	x_file_templates[6].TemplateID = X_MESHNORMALS;
	x_file_templates[7].TextID = Str("Mesh");
	x_file_templates[7].TemplateID = X_MESH;
	x_file_templates[8].TextID = Str("Material");
	x_file_templates[8].TemplateID = X_MATERIAL;
	x_file_templates[9].TextID = Str("SkinWeights");
	x_file_templates[9].TemplateID = X_SKINWEIGHTS;
	x_file_templates[10].TextID = Str("TextureFilename");
	x_file_templates[10].TemplateID = X_TEXTUREFILENAME;
	x_file_templates[11].TextID = Str("AnimationSet");
	x_file_templates[11].TemplateID = X_ANIMATIONSET;
	x_file_templates[12].TextID = Str("AnimationKey");
	x_file_templates[12].TemplateID = X_ANIMATIONKEY;
	x_file_templates[13].TextID = Str("Animation");
	x_file_templates[13].TemplateID = X_ANIMATION;
	x_file_templates[14].TextID = Str("Header");
	x_file_templates[14].TemplateID = X_HEADER;
}

CMesh::~CMesh()
{
	m_iNumVertex = 0;
	SAFE_DELETE_ARRAY(Vertex);
	SAFE_DELETE_ARRAY(Normal);
	SAFE_DELETE_ARRAY(BiNormal);
	SAFE_DELETE_ARRAY(Tangent);	
	m_iNumTriangles = 0;
	SAFE_DELETE_ARRAY(Index);
	SAFE_DELETE_ARRAY(TexUV);
	m_iType = 0;
	
}

uint32 CMesh::BuildNormals()
{
	Normal = (Vec3f*)ReAlloc(Normal,m_iNumVertex*sizeof(Vec3f));
	
	for(uint32 i=0;i<m_iNumVertex;i++)
	{
		Normal[i] = Vec3f(0.0f,0.0f,0.0f);
	}
	for(uint32 i=0,j=0;i<m_iNumTriangles;i++,j+=3)
	{
		Vec3f v1 = Vertex[Index[j]] - Vertex[Index[j+1]];
		Vec3f v2 = Vertex[Index[j]] - Vertex[Index[j+2]];

		Vec3f n = v1 ^ v2;

		Normal[Index[j]] += n;
		Normal[Index[j+1]] += n;
		Normal[Index[j+2]] += n;	
	}

	for(uint32 i=0;i<m_iNumVertex;i++)
	{
		Normal[i].normalize();
	}

	return 1;
}

uint32 CMesh::BuildTangentSpace()
{
	Normal = (Vec3f*)ReAlloc(Normal,m_iNumVertex*sizeof(Vec3f));	
	Tangent = (Vec3f*)ReAlloc(Tangent,m_iNumVertex*sizeof(Vec3f));
	BiNormal = (Vec3f*)ReAlloc(BiNormal,m_iNumVertex*sizeof(Vec3f));
	
	
	for(uint32 i=0,j=0;i<m_iNumTriangles;i++,j+=3)
	{
		Vec3f v0 = Vertex[Index[j  ]];
		Vec3f v1 = Vertex[Index[j+1]];
		Vec3f v2 = Vertex[Index[j+2]];

		Vec2f tc0 = TexUV[Index[j  ]];
		Vec2f tc1 = TexUV[Index[j+1]];
		Vec2f tc2 = TexUV[Index[j+2]];

		Vec3f normal,tangent,binormal;
		Vec3f e0 = Vec3f(0,tc1.x - tc0.x,tc1.y - tc0.y);
		Vec3f e1 = Vec3f(0,tc2.x - tc0.x,tc2.y - tc0.y);
		for(int32 k = 0; k < 3; k++) 
		{
			e0.x = v1.v[k] - v0.v[k];
			e1.x = v2.v[k] - v0.v[k];
			Vec3f v;
			v = e0^e1;
			if(math_fabs(v.v[0]) > EPSILON) 
			{
				tangent.v[k] = -v.v[1] / v.v[0];
				binormal.v[k] = -v.v[2] / v.v[0];
			}
			else 
			{
				tangent.v[k] = 0;
				binormal.v[k] = 0;
			}
		}
		tangent.normalize();
		binormal.normalize();
		normal = tangent ^ binormal;
		normal.normalize();
		
		BiNormal[Index[j]] = Normal[Index[j]] ^ tangent;
		BiNormal[Index[j]].normalize();
		Tangent[Index[j]] = BiNormal[Index[j]] ^ Normal[Index[j]];
		if(normal * Normal[Index[j]] < 0)BiNormal[Index[j]] = -BiNormal[Index[j]];
		
		BiNormal[Index[j+1]] = Normal[Index[j+1]]^tangent;
		BiNormal[Index[j+1]].normalize();
		Tangent[Index[j+1]] = BiNormal[Index[j+1]] ^ Normal[Index[j+1]];
		if(normal * Normal[Index[j+1]] < 0)BiNormal[Index[j+1]] = -BiNormal[Index[j+1]];

		BiNormal[Index[j+2]] = Normal[Index[j+2]] ^ tangent;
		BiNormal[Index[j+2]].normalize();
		Tangent[Index[j+2]] = BiNormal[Index[j+2]] ^ Normal[Index[j+2]];
		if(normal * Normal[Index[j+2]] < 0) BiNormal[Index[j+2]] = -BiNormal[Index[j+2]];
	}
	return 1;
}

uint32 CMesh::LoadMeshFromFile3DS(Str filename)
{
	if(!m_pFile.Open(filename,Str("rb")))
	{
		//GetEngine()->Error("can`t open (%s) file",filename);
		return 0;
	}
	int i,j;

	uint32 last_pos = 0,pos = 0;
	
	unsigned short chunk_id,ind;
	unsigned int chunk_lenght;
	unsigned int end;
	char name;
	unsigned short count_in_chunk;
	unsigned short face_flags;

	pos += 2 * m_pFile.Read(&chunk_id,2,1);
	if(chunk_id!=LOAD_3DS_CHUNK_MAIN)
	{
	 //ALERT("3ds:can't load %s",name);return false;
		return 0;
	}
	pos += 4 * m_pFile.Read(&end,4,1);

	while (pos < end && last_pos != pos)
	{
		last_pos = pos;
		pos += 2 * m_pFile.Read(&chunk_id,2,1);
		pos += 4 * m_pFile.Read(&chunk_lenght,4,1);

		switch (chunk_id)
        {
			case LOAD_3DS_CHUNK_MAIN: 
			break;    
			case LOAD_3DS_CHUNK_OBJMESH:
			break;
			case LOAD_3DS_CHUNK_OBJBLOCK: 
				i=0;
				do
				{
					pos += m_pFile.Read(&name,1,1);
					//data->Name.Printf((Char*)name);
					i++;
                }
				while(name != '\0' && i<20);
			break;

			case LOAD_3DS_CHUNK_TRIMESH:
			break;
			
			case LOAD_3DS_CHUNK_VERTLIST: 
				pos += sizeof (unsigned short)*m_pFile.Read(&count_in_chunk, sizeof (unsigned short), 1);
				m_iNumVertex = count_in_chunk;
				Vertex = new Vec3f[count_in_chunk];
				
                for (i=0; i<count_in_chunk; i++)
                {
					pos += sizeof (float)*m_pFile.Read(&Vertex[i].x, sizeof(float),1);
                    pos += sizeof (float)*m_pFile.Read(&Vertex[i].z, sizeof(float),1);
					pos += sizeof (float)*m_pFile.Read(&Vertex[i].y, sizeof(float),1);	
					Vertex[i].z *= -1;
				}
				break;
			case LOAD_3DS_CHUNK_FACELIST:
				pos += sizeof (unsigned short)*m_pFile.Read(&count_in_chunk, sizeof (unsigned short), 1);
				m_iNumTriangles = count_in_chunk;
				Index = new int32[count_in_chunk*3];
				
                for (i=0,j=0; i<count_in_chunk; i++,j+=3)
                {
					pos += sizeof (unsigned short)*m_pFile.Read(&ind, sizeof (unsigned short), 1);
					Index[j] = ind;
					pos += sizeof (unsigned short)*m_pFile.Read(&ind, sizeof (unsigned short), 1);
					Index[j+1] = ind;
					pos += sizeof (unsigned short)*m_pFile.Read(&ind, sizeof (unsigned short), 1);
					Index[j+2] = ind;
					pos += sizeof (unsigned short)*m_pFile.Read(&face_flags, sizeof (unsigned short), 1);
				}
                break;
			case LOAD_3DS_CHUNK_MAPLIST:
				pos += sizeof (unsigned short)*m_pFile.Read(&count_in_chunk, sizeof (unsigned short), 1);
				TexUV = new Vec2f[count_in_chunk];
				for (i=0; i<count_in_chunk; i++)
				{
					pos += sizeof (float)*m_pFile.Read(&TexUV[i].v[0], sizeof (float), 1);
                    pos += sizeof (float)*m_pFile.Read(&TexUV[i].v[1], sizeof (float), 1);	
				}
                break;
			default:
				{
					pos += chunk_lenght-6;
					m_pFile.Seek(chunk_lenght-6,File::SEEK_CURRENT);
				}
        } 
	}
	
	m_pFile.Close();
	return 1;
}

uint32 CMesh::LoadMeshFromFileX(Str filename)
{
	x_file_header XHeader;
	if(!m_pFile.Open(filename,"r"))return 0;

	m_pFile.Read((Char*)&XHeader,16,1);
	//Not a .X model file. Aborted...
	if(XHeader.Magic != XOFFILE_FORMAT_MAGIC)return 0;
	//Major version greater than 03. Aborted...
	if(XHeader.Major_Version != XOFFILE_FORMAT_VERSION03)return 0;
	//Minor version greater than 03. Aborted...
	if((XHeader.Minor_Version != XOFFILE_FORMAT_VERSION03) || (XHeader.Minor_Version != XOFFILE_FORMAT_VERSION02))return 0;
	//Not a text format. Aborted...
	if(XHeader.Format != XOFFILE_FORMAT_TEXT)return false;

	while(!m_pFile.IsEOF())
	{
		switch (ProcessBlock()) 
		{
			case X_ERROR:return false;
			case X_COMMENT:break; //nothing to do
			case X_EBRACE:break; //end of a block ?!
			//case X_FRAME:ProcessBone((Frm::Bone*)0); break;
			//case X_MESH:ProcessMesh(); break;
			//case X_ANIMATIONSET:ProcessAnimationSets(); break;
			case X_OBRACE:
			//default: AvoidTemplate(); 
				break;
		}
	}

//if (_LoadSkeletton != 0)
//MapMeshToBones(_LoadSkeletton);

	m_pFile.Close();
	return 1;
}

uint16 CMesh::ProcessBlock()
{
	Str Text;
	Char token = m_pFile.GetChar();
	switch (token) 
	{
		case '\n':
		case ' ':
		case '\t':
			{
				m_pFile.GetChar();
				return X_COMMENT; //spaces are identified as comments
			}
		case '{':
			{
				return X_OBRACE;
			}
		case '}':
			{
				m_pFile.GetChar();
				return X_EBRACE; //We arrived at the end of the block
			}
		case '/':
			{
				m_pFile.GetChar();
				if(m_pFile.GetChar() != '/'){return X_ERROR;} //we processed a block name starting with slash ?!
			}
		case '#':
			{
				for(uint32 i=0;i<255;i++)
				{
					if(m_pFile.GetChar() == '\n')break;
				}
				return X_COMMENT;
			}
		default:
			return GetXFileBlockID();
	};
}

uint16 CMesh::GetXFileBlockID()
{
	uint32 Pos;
	Str str;

	if(m_pFile.IsEOF())return X_COMMENT;

	while(!m_pFile.IsEOF())
	{
		m_pFile.Scan("%s",&str.data);
		for(int i=0; i < 15; i++)
		{
			Pos = str.Find(x_file_templates[i].TextID);
			if (Pos > -1)
			{
				m_pFile.GetChar();//eats the whitespace after the block name.
				return x_file_templates[i].TemplateID;
			}
		}
	}

	return X_UNKNOWN;
}