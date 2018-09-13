
#include "Physic.h"
#include "RigidBody.h"
#include "Collision.h"

float distance_sqr_to_ellips(Vec3f rkPoint,Vec3f & rkEllipsoid,Vec3f & rkClosest)
{
  // goes wrong when the dimensions are < 1 or so... due to numerical accuracy I think.
  // so scale everything up...
  float scale = 10.0f;
    
  Vec3f afExtent = rkEllipsoid * scale;
  rkPoint *= scale;

  float fA2 = afExtent[0]*afExtent[0];
  float fB2 = afExtent[1]*afExtent[1];
  float fC2 = afExtent[2]*afExtent[2];
  float fU2 = rkPoint[0]*rkPoint[0];
  float fV2 = rkPoint[1]*rkPoint[1];
  float fW2 = rkPoint[2]*rkPoint[2];
  float fA2U2 = fA2*fU2, fB2V2 = fB2*fV2, fC2W2 = fC2*fW2;
  
  // initial guess
  float fURatio = rkPoint[0]/afExtent[0];
  float fVRatio = rkPoint[1]/afExtent[1];
  float fWRatio = rkPoint[2]/afExtent[2];
  float fT;
  if ( fURatio*fURatio+fVRatio*fVRatio+fWRatio*fWRatio < (float)1.0 )
  {
    fT = (float)0.0f;
  }
  else
  {
    float fMax = afExtent[0];
    if ( afExtent[1] > fMax )
      fMax = afExtent[1];
    if ( afExtent[2] > fMax )
      fMax = afExtent[2];
    
	fT = fMax*rkPoint.length();
  }
  
  // Newton's method
  const int iMaxIteration = 64;
  float fP, fQ, fR;
  for (int i = 0; i < iMaxIteration; i++)
  {
    fP = fT+fA2;
    fQ = fT+fB2;
    fR = fT+fC2;
    float fP2 = fP*fP;
    float fQ2 = fQ*fQ;
    float fR2 = fR*fR;
    float fS = fP2*fQ2*fR2-fA2U2*fQ2*fR2-fB2V2*fP2*fR2-fC2W2*fP2*fQ2;
    if ( fabs(fS) < 1e-16 )
      break;
    
    float fPQ = fP*fQ, fPR = fP*fR, fQR = fQ*fR, fPQR = fP*fQ*fR;
    float fDS = ((float)2.0)*(fPQR*(fQR+fPR+fPQ)-fA2U2*fQR*(fQ+fR)-
      fB2V2*fPR*(fP+fR)-fC2W2*fPQ*(fP+fQ));
    fT -= fS/fDS;
  }
  
  rkClosest[0] = fA2*rkPoint[0]/fP;
  rkClosest[1] = fB2*rkPoint[1]/fQ;
  rkClosest[2] = fC2*rkPoint[2]/fR;
  Vec3f kDiff = rkClosest - rkPoint;
  rkClosest /= scale;
  kDiff /= scale;
  return kDiff.length2();
}

CGeometry::CGeometry(RIGID_BODY_TYPE type,Vec4f sizes)
{
	m_pSDM = new CSignedDistanceMap();
	m_iType = type;
	m_vSizes = sizes;
	m_iTrianglesCount = 0;
	m_iPointsCount = 0;
	m_iEdgesCount = 0;
	m_pTriangles = new Triangle[1];
	m_pTransformTriangles = new Triangle[1];
	m_pEdges = new Edge[1];
	m_pPoints = new Vec3f[1];
	m_pTransformPoints = new Vec3f[1];

	int32 mesh_num = 0;
	Float32 extra_frac = 0.0f;

	if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
	{
		mesh_num = 10;
		extra_frac = 1.1f;
		int32 slice=0, stack=0,stacks=10,slices=10;
		for(stack = 0 ; stack < stacks-1 ; stack++)
		{
			for(slice = 0 ; slice < slices ; slice++)
			{
				Float32 long0 = ((0.0f + slice) / (slices - 1.0f)) * 360.0f;
				Float32 long1 = ((0.0f + slice) / (slices - 1.0f)) * 360.0f;
				Float32 long2 = ((1.0f + slice) / (slices - 1.0f)) * 360.0f;
				Float32 long3 = ((1.0f + slice) / (slices - 1.0f)) * 360.0f;
	      
				Float32 lat0 = -90.0f + ((0.0f + stack) / (stacks - 1.0f)) * 180.0f;

				Float32 lat1 = -90.0f + ((1.0f + stack) / (stacks - 1.0f)) * 180.0f;
				Float32 rp0 = math_cos_deg(lat0);
				Float32 rp1 = math_cos_deg(lat1);
				Float32 rp2 = rp0;
				Float32 rp3 = rp1;

				Float32 sin_lat0 = math_sin_deg(lat0);
				Float32 sin_lat1 = math_sin_deg(lat1);
				Float32 sin_lat2 = sin_lat0;
				Float32 sin_lat3 = sin_lat1;

				Vec3f pos[4] = 
				{
					Vec3f(rp0 * math_cos_deg(long0), rp0 * math_sin_deg(long0), sin_lat0),
					Vec3f(rp1 * math_cos_deg(long1), rp1 * math_sin_deg(long1), sin_lat1),
					Vec3f(rp2 * math_cos_deg(long2), rp2 * math_sin_deg(long2), sin_lat2),
					Vec3f(rp3 * math_cos_deg(long3), rp3 * math_sin_deg(long3), sin_lat3) 
				};

				int j;
				for(j = 0 ; j < 4 ; ++j)
				{	
					pos[j][0] *= m_vSizes.x;
					pos[j][1] *= m_vSizes.y;
					pos[j][2] *= m_vSizes.z;
				}	

				if(stack == 0)
				{
					if(slice == 0)
					{
						m_iPointsCount++;
						m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
						m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
						m_pPoints[m_iPointsCount-1] = pos[0];
					}

					m_iTrianglesCount++;
					m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTransformTriangles = (Triangle*)ReAlloc(m_pTransformTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTriangles[m_iTrianglesCount-1] = Triangle(pos[2], pos[3], pos[1]);
				}
				else if (stack == stacks - 2)
				{
					m_iPointsCount++;
					m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
					m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
					m_pPoints[m_iPointsCount-1] = pos[0];

					if(slice == 0)
					{
						m_iPointsCount++;
						m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
						m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
						m_pPoints[m_iPointsCount-1] = pos[1];
					}

					m_iTrianglesCount++;
					m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTransformTriangles = (Triangle*)ReAlloc(m_pTransformTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTriangles[m_iTrianglesCount-1] = Triangle(pos[0], pos[2], pos[1]);
				}
				else
				{
					m_iPointsCount++;
					m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
					m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
					m_pPoints[m_iPointsCount-1] = pos[0];

					m_iTrianglesCount+=2;
					m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTransformTriangles = (Triangle*)ReAlloc(m_pTransformTriangles,m_iTrianglesCount*sizeof(Triangle));
					m_pTriangles[m_iTrianglesCount-2] = Triangle(pos[0], pos[2], pos[1]);
					m_pTriangles[m_iTrianglesCount-1] = Triangle(pos[2], pos[3], pos[1]);
				}
			}
		}
	}
	else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX)
	{
		mesh_num = 16;
		extra_frac = 1.1f;

		Float32 dx = sizes.x * 0.5f;
		Float32 dy = sizes.y * 0.5f;
		Float32 dz = sizes.z * 0.5f;

		int32 num_edge_samples = 6;

		m_iPointsCount = 8;
		m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
		m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));

		enum {TFL = 0, TBL = 1, TBR = 2, TFR = 3, BFL = 4, BBL = 5, BBR = 6, BFR = 7};
		m_pPoints[TFL] = Vec3f( dx,  dy,  dz);
		m_pPoints[TBL] = Vec3f(-dx,  dy,  dz);
		m_pPoints[TBR] = Vec3f(-dx, -dy,  dz);
		m_pPoints[TFR] = Vec3f( dx, -dy,  dz);

		m_pPoints[BFL] = Vec3f( dx,  dy, -dz);
		m_pPoints[BBL] = Vec3f(-dx,  dy, -dz);
		m_pPoints[BBR] = Vec3f(-dx, -dy, -dz);
		m_pPoints[BFR] = Vec3f( dx, -dy, -dz);

		m_iEdgesCount = 12;
		m_pEdges = (Edge*)ReAlloc(m_pEdges,m_iEdgesCount*sizeof(Edge));

		m_pEdges[0] = Edge(TFL, TBL, num_edge_samples);
		m_pEdges[1] = Edge(TBL, TBR, num_edge_samples);
		m_pEdges[2] = Edge(TBR, TFR, num_edge_samples);
		m_pEdges[3] = Edge(TFR, TFL, num_edge_samples);

		m_pEdges[4] = Edge(BFL, BBL, num_edge_samples);
		m_pEdges[5] = Edge(BBL, BBR, num_edge_samples);
		m_pEdges[6] = Edge(BBR, BFR, num_edge_samples);
		m_pEdges[7] = Edge(BFR, BFL, num_edge_samples);

		m_pEdges[8] = Edge(TFL, BFL, num_edge_samples);
		m_pEdges[9] = Edge(TBL, BBL, num_edge_samples);
		m_pEdges[10] = Edge(TBR, BBR, num_edge_samples);
		m_pEdges[11] = Edge(TFR, BFR, num_edge_samples);

		m_iTrianglesCount = 12;
		m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
		m_pTransformTriangles = (Triangle*)ReAlloc(m_pTransformTriangles,m_iTrianglesCount*sizeof(Triangle));

		// top
		m_pTriangles[0] = Triangle(m_pPoints[TFL], m_pPoints[TBL], m_pPoints[TBR]);
		m_pTriangles[1] = Triangle(m_pPoints[TFL], m_pPoints[TBR], m_pPoints[TFR]);
		// bottom
		m_pTriangles[2] = Triangle(m_pPoints[BFL], m_pPoints[BBR], m_pPoints[BBL]);
		m_pTriangles[3] = Triangle(m_pPoints[BFL], m_pPoints[BFR], m_pPoints[BBR]);
		// left
		m_pTriangles[4] = Triangle(m_pPoints[BFL], m_pPoints[BBL], m_pPoints[TBL]);
		m_pTriangles[5] = Triangle(m_pPoints[BFL], m_pPoints[TBL], m_pPoints[TFL]);
		// right
		m_pTriangles[6] = Triangle(m_pPoints[BFR], m_pPoints[TFR], m_pPoints[TBR]);
		m_pTriangles[7] = Triangle(m_pPoints[BFR], m_pPoints[TBR], m_pPoints[BBR]);
		// front
		m_pTriangles[8] = Triangle(m_pPoints[TFL], m_pPoints[BFR], m_pPoints[BFL]);
		m_pTriangles[9] = Triangle(m_pPoints[TFL], m_pPoints[TFR], m_pPoints[BFR]);
		// back
		m_pTriangles[10] = Triangle(m_pPoints[TBL], m_pPoints[BBR], m_pPoints[TBR]);
		m_pTriangles[11] = Triangle(m_pPoints[TBL], m_pPoints[BBL], m_pPoints[BBR]);
	}
	else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_CYLINDER)
	{
		mesh_num = 16;
		extra_frac = 1.1f;
		Float32 radius = m_vSizes.x;
		Float32 length = m_vSizes.y;

		uint32 stacks = 20;
		uint32 slices = 20;

		if(stacks < 2)stacks = 2;
		if(slices < 3)slices = 3;

		for(uint32 i = 0;i < slices;i++)
		{
			for(uint32 j = 0;j < stacks;j++)
			{
				Float32 x0 = -0.5f * length + length * (j + 0.0f) / stacks;
				Float32 x1 = -0.5f * length + length * (j + 1.0f) / stacks;

				Float32 theta0 = 360.0f * (i + 0.0f) / slices;

				Float32 y0 = radius * math_cos_deg(theta0);
				Float32 z0 = radius * math_sin_deg(theta0);

				m_iPointsCount++;
				m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
				m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
				m_pPoints[m_iPointsCount-1] = Vec3f(x0,y0,z0);

				if(j == (stacks - 1))
				{
					m_iPointsCount++;
					m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
					m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
					m_pPoints[m_iPointsCount-1] = Vec3f(x1,y0,z0);
				}
			}
		}

		m_iPointsCount+=2;
		m_pPoints = (Vec3f*)ReAlloc(m_pPoints,m_iPointsCount*sizeof(Vec3f));
		m_pTransformPoints = (Vec3f*)ReAlloc(m_pTransformPoints,m_iPointsCount*sizeof(Vec3f));
		m_pPoints[m_iPointsCount-2] = Vec3f(0.5f * length, 0, 0);
		m_pPoints[m_iPointsCount-1] = Vec3f(-0.5f * length, 0, 0);

		for(uint32 i = 0;i < slices;i++)
		{
			Float32 x0 = -0.5f * length;
			Float32 x1 = 0.5f * length;
			Float32 theta0 = 360.0f * (i + 0.0f) / slices;
			Float32 theta1 = 360.0f * (i + 1.0f) / slices;
			Float32 y0 = radius * math_cos_deg(theta0);
			Float32 y1 = radius * math_cos_deg(theta1);
			Float32 z0 = radius * math_sin_deg(theta0);
			Float32 z1 = radius * math_sin_deg(theta1);

			m_iTrianglesCount+=4;
			m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
			m_pTransformTriangles = (Triangle*)ReAlloc(m_pTransformTriangles,m_iTrianglesCount*sizeof(Triangle));

			m_pTriangles[m_iTrianglesCount-4] = Triangle(Vec3f(x0, y0, z0), Vec3f(x1, y1, z1), Vec3f(x1, y0, z0));
			m_pTriangles[m_iTrianglesCount-3] = Triangle(Vec3f(x0, y0, z0), Vec3f(x0, y1, z1), Vec3f(x1, y1, z1));
			m_pTriangles[m_iTrianglesCount-2] = Triangle(Vec3f(x0,  0,  0), Vec3f(x0, y1, z1), Vec3f(x0, y0, z0));
			m_pTriangles[m_iTrianglesCount-1] = Triangle(Vec3f(x1,  0,  0), Vec3f(x1, y0, z0), Vec3f(x1, y1, z1));
		}
	}

	RebuildAABB();

	m_pSDM->ConstructMesh(mesh_num,mesh_num,mesh_num,extra_frac,m_pTriangles,m_iTrianglesCount,this);
}

CGeometry::~CGeometry()
{
	SAFE_DELETE(m_pSDM);
	SAFE_DELETE_ARRAY(m_pTriangles);
	SAFE_DELETE_ARRAY(m_pTransformTriangles);
	SAFE_DELETE_ARRAY(m_pEdges);
	SAFE_DELETE_ARRAY(m_pPoints);
	SAFE_DELETE_ARRAY(m_pTransformPoints);
	
}

void CGeometry::Update(Mat4f transform)
{
	m_mTransform = transform;

	for(uint32 i=0;i<m_iTrianglesCount;i++)
	{
		for(uint32 j=0;j<3;j++)
		{
			m_pTransformTriangles[i].v[j] = transform * m_pTriangles[i].v[j];
		}
	}

	for(uint32 i = 0;i < m_iPointsCount;i++)
	{
		m_pTransformPoints[i] = transform * m_pPoints[i];
	}

	RebuildAABB();
}

void CGeometry::DrawAABB(CRender *render)
{
	Vec3f p = m_pAABB.p - m_pAABB.size;
	render->DrawLine(p,p + Vec3f(m_pAABB.size.x*2,0,0));
	render->DrawLine(p,p + Vec3f(0,m_pAABB.size.y*2,0));
	render->DrawLine(p,p + Vec3f(0,0,m_pAABB.size.z*2));
	p = m_pAABB.p - Vec3f(-m_pAABB.size.x,m_pAABB.size.y,m_pAABB.size.z);
	render->DrawLine(p,p + Vec3f(0,m_pAABB.size.y*2,0));

	p = m_pAABB.p + m_pAABB.size;
	render->DrawLine(p,p - Vec3f(m_pAABB.size.x*2,0,0));
	render->DrawLine(p,p - Vec3f(0,m_pAABB.size.y*2,0));
	render->DrawLine(p,p - Vec3f(0,0,m_pAABB.size.z*2));
	p = m_pAABB.p - Vec3f(m_pAABB.size.x,m_pAABB.size.y,-m_pAABB.size.z);
	render->DrawLine(p,p + Vec3f(0,m_pAABB.size.y*2,0));

	p = m_pAABB.p + Vec3f(m_pAABB.size.x,-m_pAABB.size.y,m_pAABB.size.z);
	render->DrawLine(p,p - Vec3f(m_pAABB.size.x*2,0,0));
	render->DrawLine(p,p - Vec3f(0,0,m_pAABB.size.z*2));

	p = m_pAABB.p - Vec3f(m_pAABB.size.x,-m_pAABB.size.y,m_pAABB.size.z);
	render->DrawLine(p,p + Vec3f(m_pAABB.size.x*2,0,0));
	render->DrawLine(p,p + Vec3f(0,0,m_pAABB.size.z*2));
}

void CGeometry::Draw(CRender *render)
{
	for(uint32 i=0;i<m_iTrianglesCount;i++)
	{
		Vec3f n = Vec3f(m_pTransformTriangles[i].v[1] - m_pTransformTriangles[i].v[0]) ^ Vec3f(m_pTransformTriangles[i].v[2] - m_pTransformTriangles[i].v[0]);
		n.normalize();
		render->DrawTriangle(m_pTriangles[i].v[0],m_pTriangles[i].v[1],m_pTriangles[i].v[2],n);
		/*
		n = Vec3f(m_pTriangles[i].v[1] - m_pTriangles[i].v[0]) ^ Vec3f(m_pTriangles[i].v[2] - m_pTriangles[i].v[0]);
		n.normalize();
		render->DrawLine(m_pTriangles[i].v[0],m_pTriangles[i].v[0] + n * 5,1);
		*/
	}
}

void CGeometry::RebuildAABB()
{
	m_pAABB.p = Vec3f(m_mTransform.mat[12],m_mTransform.mat[13],m_mTransform.mat[14]);

	m_pAABB.max = m_pAABB.p;
	m_pAABB.min = m_pAABB.p;

	for(uint32 i=0;i<m_iTrianglesCount;i++)
	{
		for(uint32 j=0;j<3;j++)
		{
			if(m_pTransformTriangles[i].v[j].x < m_pAABB.min.x)m_pAABB.min.x = m_pTransformTriangles[i].v[j].x;
			if(m_pTransformTriangles[i].v[j].y < m_pAABB.min.y)m_pAABB.min.y = m_pTransformTriangles[i].v[j].y;
			if(m_pTransformTriangles[i].v[j].z < m_pAABB.min.z)m_pAABB.min.z = m_pTransformTriangles[i].v[j].z;

			if(m_pTransformTriangles[i].v[j].x > m_pAABB.max.x)m_pAABB.max.x = m_pTransformTriangles[i].v[j].x;
			if(m_pTransformTriangles[i].v[j].y > m_pAABB.max.y)m_pAABB.max.y = m_pTransformTriangles[i].v[j].y;
			if(m_pTransformTriangles[i].v[j].z > m_pAABB.max.z)m_pAABB.max.z = m_pTransformTriangles[i].v[j].z;
		}
	}

	m_pAABB.size = Vec3f((m_pAABB.max.x-m_pAABB.min.x)*0.5f,(m_pAABB.max.y-m_pAABB.min.y)*0.5f,(m_pAABB.max.z-m_pAABB.min.z)*0.5f);
}

Bool CGeometry::GetMeshInfoSDM(Vec3f &pos, Bool &inside, Vec3f &vector_to_surface)
{
	if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX)
	{
		Vec3f delta = Vec3f(m_vSizes.x * 0.5f, m_vSizes.y * 0.5f, m_vSizes.z * 0.5f);

		Vec3f face_point;
		Float32 dists[3];
		inside = true;

		for(int32 i = 0 ; i < 3 ; i++)
		{
			if(pos[i] > delta[i])
			{
				face_point[i] = delta[i];
				inside = false;
			}
			else if(pos[i] < -delta[i])
			{
				face_point[i] = -delta[i];
				inside = false;
			}
			else
			{	
				if(pos[i] > 0.0f)
					face_point[i] = delta[i];
				else
					face_point[i] = -delta[i];
			}
			dists[i] = math_fabs(face_point[i] - pos[i]);
		}

		int32 best_i = 0;
		if(dists[1] < dists[best_i])best_i = 1;
		if(dists[2] < dists[best_i])best_i = 2;

		for(int32 i = 0 ; i < 3 ; i++)
		{
			if(i != best_i)
			{
				face_point[i] = pos[i];
				if(face_point[i] > delta[i])
					face_point[i] = delta[i];
				else if(face_point[i] < -delta[i])
					face_point[i] = -delta[i];
			}
		}
		vector_to_surface = face_point - pos;
		return true;
	}
	else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
	{

		Vec3f extends = Vec3f(m_vSizes.x, m_vSizes.x, m_vSizes.x);
		Vec3f closest;

		Float32 dist = distance_sqr_to_ellips(pos, extends, closest);
		vector_to_surface = closest - pos;
		Float32 v = (pos[0] / m_vSizes.x) * (pos[0] / m_vSizes.x) + 
					(pos[1] / m_vSizes.y) * (pos[1] / m_vSizes.y) + 
					(pos[2] / m_vSizes.z) * (pos[2] / m_vSizes.z);
		if (v > 1.0f)
		inside = false;
		else 
		inside = true;
		return true;
	}
	else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_CYLINDER)
	{
		// the +ve end of the cylinder
		Float32 dx = m_vSizes[1] * 0.5f;

		Float32 dx1 = dx - m_vSizes[0];

		Float32 r = (Float32)hypot(Float32(pos[1]), Float32(pos[2]));

		if ( (r > m_vSizes[0]) || (pos[0] > dx) || (pos[0] < -dx) )
		{
			inside = false;
		}
		else
		{
			inside = true;
		}

		Vec3f surface_pos;

		if (r == 0.0f)
		{
			// nasty case
			if (m_vSizes[0] < dx)
			{
				surface_pos = Vec3f(0, m_vSizes[0], 0);
			}
			else
			{
				if (pos[0] > 0)
				surface_pos = Vec3f(dx, 0, 0);
				else
				surface_pos = Vec3f(-dx, 0, 0);
			}
		}
		else if ( (pos[0] > dx1 + r) || (pos[0] < -(dx1 + r)) )
		{
			// inside the cone that results in the nearest surface point being on the +ve/-ve end
			surface_pos = pos;
			if (r > m_vSizes[0])
			{
				surface_pos[1] *= m_vSizes[0] / r;
				surface_pos[2] *= m_vSizes[0] / r;
			}
			if (pos[0] > 0)
			{
				surface_pos[0] = dx;
			}
			else if (pos [0] < 0)
			{
				surface_pos[0] = -dx;
			}
		}
		else
		{
			// nearest point is on the curved cylinder surface.
			surface_pos = pos;
			surface_pos[1] *= m_vSizes[0] / r;
			surface_pos[2] *= m_vSizes[0] / r;
			if (pos[0] > dx)
			{
				surface_pos[0] = dx;
			}
			else if (pos[0] < -dx)
			{
				surface_pos[0] = -dx;
			}
		}

		vector_to_surface = surface_pos - pos;

		return true;
	}
	return false;
}