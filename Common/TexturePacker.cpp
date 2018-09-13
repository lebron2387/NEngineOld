#include "Image.h"
#include "TexturePacker.h"

CTexturePacker::CTexturePacker()
{
	m_iNewMaxWidth = 0;
	m_iNewMaxHeight = 0;
	m_iNodesCount = 0;
	m_iImagesCount = 0;
	m_iCropToMinSize = 0;
	m_iCropSizesCount = 0;

	m_pImages = new CImage[100];
	m_pNodes = new NodeData[1];
	m_iImagesSortData = new ImageMaxSizeData[1];
	m_pCropSizes = new uint32[1];
}

CTexturePacker::~CTexturePacker()
{
	SAFE_DELETE_ARRAY(m_pNodes);
	SAFE_DELETE_ARRAY(m_iImagesSortData);
	SAFE_DELETE_ARRAY(m_pCropSizes);
	SAFE_DELETE_ARRAY(m_pImages);
}

uint32 CTexturePacker::AddImage(Str filename)
{
	uint32 image_id;
	++m_iImagesCount;
	image_id = m_iImagesCount - 1;
	m_pImages[image_id].LoadImageFromFile(filename);

	m_iImagesSortData = (ImageMaxSizeData*)ReAlloc(m_iImagesSortData,m_iImagesCount * sizeof(ImageMaxSizeData));
	m_iImagesSortData[image_id].image_id = m_iImagesCount-1;
	m_iImagesSortData[image_id].max_size = m_pImages[image_id].GetWidth() >= m_pImages[image_id].GetHeight() ? m_pImages[image_id].GetWidth() : m_pImages[image_id].GetHeight();
	return image_id;
}

uint32 CTexturePacker::AddNode(uint32 x,uint32 y,uint32 width,uint32 height)
{
	++m_iNodesCount;
	m_pNodes = (NodeData*)ReAlloc(m_pNodes,m_iNodesCount * sizeof(NodeData));
	uint32 node_id = m_iNodesCount - 1;
	m_pNodes[node_id].x = x;
	m_pNodes[node_id].y = y;
	m_pNodes[node_id].width = width;
	m_pNodes[node_id].height = height;
	m_pNodes[node_id].image_id = -1;
	return node_id;
}

uint32 CTexturePacker::AddCropSize(uint32 size)
{
	++m_iCropSizesCount;
	m_pCropSizes = (uint32*)ReAlloc(m_pCropSizes,m_iCropSizesCount * sizeof(uint32));
	m_pCropSizes[m_iCropSizesCount-1] = size;
	return m_iCropSizesCount-1;
}

uint32 CTexturePacker::GetNearSize(uint32 original_size)
{
	if(original_size < m_pCropSizes[0])
	{
		return m_pCropSizes[0];
	}

	for(uint32 i=0;i<m_iCropSizesCount;i++)
	{
		if(original_size == m_pCropSizes[i])
		{
			return m_pCropSizes[i];
		}
	}

	for(uint32 i=0;i<m_iCropSizesCount-1;i++)
	{
		if(original_size  > m_pCropSizes[i] && original_size  < m_pCropSizes[i+1])
		{
			return m_pCropSizes[i+1];
		}
	}
	return original_size;
}

Float32 CTexturePacker::GetUCoord(uint32 image_id,Float32 original_u_coord)
{
	if(m_iNewMaxWidth <=0)return 0.0f;
	Float32 start_coord = 0.0f;
	Float32 end_coord = 0.0f;
	for(uint32 i=0;i<m_iNodesCount;i++)
	{
		if(m_pNodes[i].image_id == image_id)
		{
			start_coord = Float32(m_pNodes[i].x+1) / Float32(m_iNewMaxWidth);
			end_coord = Float32(m_pNodes[i].x + m_pNodes[i].width-1) / Float32(m_iNewMaxWidth);

			if(original_u_coord == 0.0f)
			{
				return start_coord;
			}
			else if(original_u_coord >=1.0f)
			{
				return end_coord;
			}
			else
			{
				return (start_coord+((end_coord - start_coord)*original_u_coord));
			}
		}
	}
	return 0.0f;
}

Float32 CTexturePacker::GetVCoord(uint32 image_id,Float32 original_v_coord)
{
	if(m_iNewMaxHeight <=0)return 0.0f;
	Float32 start_coord = 0.0f;
	Float32 end_coord = 0.0f;
	for(uint32 i=0;i<m_iNodesCount;i++)
	{
		if(m_pNodes[i].image_id == image_id)
		{
			start_coord = Float32(m_pNodes[i].y+1) / Float32(m_iNewMaxHeight);
			end_coord = Float32(m_pNodes[i].y + m_pNodes[i].height-1) / Float32(m_iNewMaxHeight);

			if(original_v_coord == 0.0f)
			{
				return start_coord;
			}
			else if(original_v_coord >=1.0f)
			{
				return end_coord;
			}
			else
			{
				return (start_coord+((end_coord - start_coord)*original_v_coord));
			}
		}
	}
	return 0.0f;
}


int32 Comparator( const void * elem1, const void * elem2 )
{
	return ( *(CTexturePacker::ImageMaxSizeData*)elem2 - *(CTexturePacker::ImageMaxSizeData*)elem1 );
}

uint32 CTexturePacker::CreateTexture(CRender *render,uint32 max_width,uint32 max_height,uint32 mip_levels)
{
	uint32 render_texture_id = 0;

	m_iNodesCount = 1;
	m_pNodes[0].x = 0;
	m_pNodes[0].y = 0;
	m_pNodes[0].height = max_height;
	m_pNodes[0].width = max_width;
	m_pNodes[0].image_id = -1;

	qsort (m_iImagesSortData, m_iImagesCount, sizeof(ImageMaxSizeData), Comparator);

	uint32 image_id = 0;
	for(uint32 i=0;i<m_iImagesCount;i++)
	{
		image_id = m_iImagesSortData[i].image_id;
		for(uint32 j=0;j<m_iNodesCount;j++)
		{
			if(m_pNodes[j].image_id == -1)
			{
				if(m_pNodes[j].width == m_pImages[image_id].GetWidth() && m_pNodes[j].height == m_pImages[image_id].GetHeight())
				{
					m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
					break;
				}
				else if(m_pNodes[j].width == m_pImages[image_id].GetWidth() && m_pNodes[j].height > m_pImages[image_id].GetHeight())
				{
					m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
					AddNode(m_pNodes[j].x,m_pNodes[j].y+m_pImages[image_id].GetHeight(),m_pNodes[j].width,m_pNodes[j].height - m_pImages[image_id].GetHeight());
					m_pNodes[j].height = m_pImages[image_id].GetHeight();
					break;
				}
				else if(m_pNodes[j].width > m_pImages[image_id].GetWidth() && m_pNodes[j].height == m_pImages[image_id].GetHeight())
				{
					m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
					AddNode(m_pNodes[j].x+m_pImages[image_id].GetWidth(),m_pNodes[j].y,m_pNodes[j].width - m_pImages[image_id].GetWidth(),m_pImages[image_id].GetHeight());
					m_pNodes[j].width = m_pImages[image_id].GetWidth();
					break;
				}
				else if(m_pNodes[j].width > m_pImages[image_id].GetWidth() && m_pNodes[j].height > m_pImages[image_id].GetHeight())
				{
					if(m_pImages[image_id].GetWidth() > m_pImages[image_id].GetHeight())
					{
						m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
						AddNode(m_pNodes[j].x+m_pImages[image_id].GetWidth(),m_pNodes[j].y,m_pNodes[j].width - m_pImages[image_id].GetWidth(),m_pImages[image_id].GetHeight());
						AddNode(m_pNodes[j].x,m_pNodes[j].y+m_pImages[image_id].GetHeight(),m_pNodes[j].width,m_pNodes[j].height - m_pImages[image_id].GetHeight());
						m_pNodes[j].width = m_pImages[image_id].GetWidth();
						m_pNodes[j].height = m_pImages[image_id].GetHeight();
						break;
					}
					else if(m_pImages[image_id].GetWidth() < m_pImages[image_id].GetHeight())
					{
						m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
						AddNode(m_pNodes[j].x,m_pNodes[j].y+m_pImages[image_id].GetHeight(),m_pImages[image_id].GetWidth(),m_pNodes[j].height - m_pImages[image_id].GetHeight());
						AddNode(m_pNodes[j].x+m_pImages[image_id].GetWidth(),m_pNodes[j].y,m_pNodes[j].width - m_pImages[image_id].GetWidth(),m_pNodes[j].height);
						m_pNodes[j].width = m_pImages[image_id].GetWidth();
						m_pNodes[j].height = m_pImages[image_id].GetHeight();
						break;
					}
					else
					{
						m_pNodes[j].image_id = m_iImagesSortData[i].image_id;
						AddNode(m_pNodes[j].x+m_pImages[image_id].GetWidth(),m_pNodes[j].y,m_pNodes[j].width - m_pImages[image_id].GetWidth(),m_pImages[image_id].GetHeight());
						AddNode(m_pNodes[j].x,m_pNodes[j].y+m_pImages[image_id].GetHeight(),m_pNodes[j].width,m_pNodes[j].height - m_pImages[image_id].GetHeight());
						m_pNodes[j].width = m_pImages[image_id].GetWidth();
						m_pNodes[j].height = m_pImages[image_id].GetHeight();
						break;
					}
				}
			}
		}
	}

	for(uint32 i=0;i<m_iNodesCount;i++)
	{
		if(m_pNodes[i].image_id != -1)
		{
			if(m_pNodes[i].x + m_pNodes[i].width > m_iNewMaxWidth)
			{
				m_iNewMaxWidth = m_pNodes[i].x + m_pNodes[i].width;
			}
			if(m_pNodes[i].y + m_pNodes[i].height > m_iNewMaxHeight)
			{
				m_iNewMaxHeight = m_pNodes[i].y + m_pNodes[i].height;
			}
		}
	}

	CImage *img = new CImage();
	if(m_iCropToMinSize)
	{
		img->CreateEmptyTexture(m_iNewMaxWidth,m_iNewMaxHeight);
	}
	else if(m_iCropSizesCount > 0)
	{
		m_iNewMaxWidth = GetNearSize(m_iNewMaxWidth);
		m_iNewMaxHeight = GetNearSize(m_iNewMaxHeight);
		int32 max_size = m_iNewMaxWidth > m_iNewMaxHeight ? m_iNewMaxWidth : m_iNewMaxHeight;
		m_iNewMaxWidth = max_size;
		m_iNewMaxHeight = max_size;
		img->CreateEmptyTexture(max_size,max_size);
	}
	else
	{
		m_iNewMaxWidth = max_width;
		m_iNewMaxHeight = max_height;
		img->CreateEmptyTexture(max_width,max_height);
	}
	render_texture_id = render->CreateTexture(CRender::TEXTURETYPE::TEXTURE_TYPE_2D,img->GetWidth(),img->GetHeight(),img->GetChannelCount(),img->GetFormat(),img->GetDepth(),0,mip_levels);
	render->FillTexture(render_texture_id,img->GetData());
	SAFE_DELETE(img);

	for(uint32 i=0;i<m_iNodesCount;i++)
	{
		if(m_pNodes[i].image_id != -1)
		{
			render->FillPartTexture(render_texture_id,m_pImages[m_pNodes[i].image_id].GetData(),0,m_pNodes[i].x,m_pNodes[i].y,m_pNodes[i].width,m_pNodes[i].height);
		}
	}	
	//SAFE_DELETE_ARRAY(m_pImages);

	return render_texture_id;
}
