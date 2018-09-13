#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

struct SDMData
{
	Vec3f dir;
	Float32  dist;
};

class SDM3DArray
{
public:
	SDM3DArray() {nx = 0; ny = 0; nz = 0; data = new SDMData[1];}
	SDM3DArray(SDM3DArray &);
	~SDM3DArray() {delete [] data;}

	SDMData & operator()(int32 i, int32 j, int32 k) {return data[i + nx * j + (nx * ny) * k];}

	SDMData & operator()(int32 i, int32 j, int32 k) const {return data[i + nx * j + (nx * ny) * k];}

	void resize(int _nx, int _ny, int _nz) 
	{
		nx = _nx; ny = _ny; nz = _nz;
		delete [] data;
		data = new SDMData[nx * ny * nz];
	}

	private:
		SDMData *data;
		int32 nx, ny, nz;
};

class CSignedDistanceMap
{
public:
	CSignedDistanceMap(){};
	~CSignedDistanceMap(){};

	void ConstructMesh(int32 _nx, int32 _ny, int32 _nz, Float32 extra_frac,Triangle *triangles,uint32 triangles_count,CGeometry *geometry);
	Bool GetPoint(Vec3f &pos, Vec3f &dir, Float32 &dist, Bool accurate_outside_dist);
	Float32 GetSquareDistanceToBBox(Vec3f &pos);
	Bool IntersectSegmentTriangle(Segment seg,Triangle tri,Vec3f pos,Float32 SegP,Bool seg_in_dir_of_tri_normal);
	uint32 NumberTriangleIntersections(Segment seg, Triangle *triangles,uint32 triangles_count);
	__forceinline Float32 GetBoundingRadius() {return boundingRadius;}	

private:
	void PopulateMesh(CGeometry *geometry);
	void PopulateMesh(Triangle *triangles,uint32 triangles_count);

	int32 nx, ny, nz;
	Float32 min_x, min_y, min_z;
	Float32 max_x, max_y, max_z;
	Float32 dx, dy, dz;
	Float32 boundingRadius;

	SDM3DArray data;
};


class CCollision
{
public:
	CCollision(){};
	~CCollision(){};

	__forceinline Bool RigidBodyOverlapSDM(CRigidBody *a,CRigidBody *b)
	{
		Vec3f pos0,pos1;
		Vec3f dir;
		Float32 dist = 0.0f;
		Bool ret = false;
	  
		//Points
		for(uint32 i = 0;i < b->GetGeometry()->GetPointsCount();i++)
		{
			Vec3f orig_pos1 = b->GetGeometry()->GetTransformPoint(i);
			Vec3f pos1 = a->GetIRotation() * (orig_pos1 - a->GetPosition());
	  
			if(a->GetGeometry()->GetSignedDistanceMap()->GetPoint(pos1, dir, dist, false))
			{
				Vec3f normal = a->GetRotation() * dir;
				Vec3f mid_point = orig_pos1 + normal * (0.5f * dist);
	   
				a->AddContact(b,mid_point,normal,dist);
				ret = true;
			}
		}
	  
		//Edges
		for(uint32 j = 0;j < b->GetGeometry()->GetEdgesCount();j++)
		{
			uint32 num_points = b->GetGeometry()->GetEdge(j).num_samples;
			Vec3f pos1_start = b->GetGeometry()->GetTransformPoint(b->GetGeometry()->GetEdge(j).i0);
			Vec3f pos1_end = b->GetGeometry()->GetTransformPoint(b->GetGeometry()->GetEdge(j).i1);
			Vec3f delta = pos1_end - pos1_start;
	  
			Vec3f best_normal;
			Vec3f best_pos;
			Float32 best_depth = -1000.0f;
	      
			for(uint32 i = 0;i < num_points;i++)
			{
				Vec3f orig_pos1 = pos1_start + ( delta*Float32(i) ) / Float32(num_points);
				Vec3f pos1 = a->GetIRotation() * (orig_pos1 - a->GetPosition());
	          
				if(a->GetGeometry()->GetSignedDistanceMap()->GetPoint(pos1, dir, dist, false))
				{
					if(dist > best_depth)
					{
						best_normal = a->GetRotation() * dir;
						best_pos = orig_pos1 + best_normal * (0.5f * dist);
						best_depth = dist;
					}
				}

				if(best_depth >= 0.0f)
				{
					a->AddContact(b,best_pos,best_normal,-best_depth);
					ret =true;
				}
			}
		}
		
		return ret;

	}
	__forceinline Bool AABBOverlap(AABB a,AABB b)
	{
		Vec3f T = a.p - b.p;
		return math_fabs(T.x) <= (a.size.x + b.size.x) && math_fabs(T.y) <= (a.size.y + b.size.y) && math_fabs(T.z) <= (a.size.z + b.size.z);
	}
private:
};