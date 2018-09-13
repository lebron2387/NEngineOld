#include "../Core/Engine.h"
#include "../Render/Render.h"

class CImage;

class CTexturePacker
{
public:
	CTexturePacker();
	~CTexturePacker();

	struct ImageMaxSizeData
	{
		int32 image_id;
		uint32 max_size;
		__forceinline const int32 operator-(const ImageMaxSizeData &v) const { return max_size - v.max_size; }
	};

	struct NodeData
	{
		NodeData()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			image_id = -1;
		};
		uint32 x;
		uint32 y;
		uint32 width;
		uint32 height;
		int32 image_id;
	};

	uint32 CreateTexture(CRender *render,uint32 max_width,uint32 max_height,uint32 mip_levels);
	uint32 AddImage(Str filename);
	uint32 AddCropSize(uint32 size);
	Float32 GetUCoord(uint32 image_id,Float32 original_u_coord);
	Float32 GetVCoord(uint32 image_id,Float32 original_v_coord);
	
	__forceinline int32 GetImageWidth(uint32 image_id){return m_pImages[image_id].GetWidth();}
	__forceinline int32 GetImageHeight(uint32 image_id){return m_pImages[image_id].GetHeight();}

	__forceinline void EnableCropToMinSize(){m_iCropToMinSize = 1;}
	__forceinline void DisableCropToMinSize(){m_iCropToMinSize = 0;}

private:

	uint32 GetNearSize(uint32 original_size);
	uint32 AddNode(uint32 x,uint32 y,uint32 width,uint32 height);

	uint32 m_iNewMaxWidth;
	uint32 m_iNewMaxHeight;

	CImage *m_pImages;
	ImageMaxSizeData *m_iImagesSortData;
	uint32 m_iImagesCount;

	uint32 m_iNodesCount;
	NodeData *m_pNodes;

	uint32 m_iCropToMinSize;
	uint32 m_iCropSizesCount;
	uint32 *m_pCropSizes;
protected:
};