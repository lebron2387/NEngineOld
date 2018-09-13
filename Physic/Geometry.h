#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

class CRigidBody;
class CSignedDistanceMap;

struct AABB
{
	Vec3f p;
	Vec3f size;
	Vec3f min;
	Vec3f max;
};

struct Segment
{
	Segment(Vec3f _start,Vec3f _dir)
	{
		start = _start;
		dir = _dir;
	}
	Vec3f start;
	Vec3f dir; 
};

struct Edge
{
	Edge() {}
	Edge(int32 _i0, int32 _i1, int32 _num_samples)
	{
		i0 = _i0;
		i1 = _i1;
		num_samples = (_num_samples > 2) ? _num_samples : 2;
	}
	int32 i0, i1, num_samples;
};

struct Triangle
{
	Triangle(){}
	Triangle(Vec3f v0,Vec3f v1,Vec3f v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
	Vec3f v[3];
};

class CGeometry
{
public:
	CGeometry(RIGID_BODY_TYPE type,Vec4f sizes);
	~CGeometry();

	void Update(Mat4f transform);
	void Draw(CRender *render);
	void DrawAABB(CRender *render);

	__forceinline Triangle GetTriangle(uint32 id){return m_pTriangles[id];}
	__forceinline Triangle GetTransformTriangle(uint32 id){return m_pTransformTriangles[id];}
	__forceinline uint32 GetPointsCount(){return m_iPointsCount;}
	__forceinline uint32 GetEdgesCount(){return m_iEdgesCount;}
	__forceinline uint32 GetTrianglesCount(){return m_iTrianglesCount;}
	__forceinline Vec3f GetPoint(uint32 id){return m_pPoints[id];}
	__forceinline Vec3f GetTransformPoint(uint32 id){return m_pTransformPoints[id];}
	__forceinline Edge GetEdge(uint32 id){return m_pEdges[id];}

	__forceinline CSignedDistanceMap *GetSignedDistanceMap(){return m_pSDM;}

	Bool GetMeshInfoSDM(Vec3f &pos, Bool &inside, Vec3f &vector_to_surface);
	void RebuildAABB();
	AABB GetAABB(){return m_pAABB;}

private:
	CSignedDistanceMap *m_pSDM;
	uint32 m_iType;
	Vec3f m_vSizes;
	AABB m_pAABB;
	Triangle *m_pTriangles;
	Triangle *m_pTransformTriangles;
	Edge *m_pEdges;
	Vec3f *m_pPoints;
	Vec3f *m_pTransformPoints;
	uint32 m_iPointsCount;
	uint32 m_iEdgesCount;
	uint32 m_iTrianglesCount;
	Mat4f m_mTransform;
};