
#include "Physic.h"

//Collision

CCollision::CCollision(uint32 type,Vec4f sizes)
{
	m_iType = type;
	m_vSizes = sizes;
	m_iTrianglesCount = 0;
	m_pTriangles = new Triangle[1];
	m_pTrianglesTransform = new Triangle[1];
	if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX)
	{
		m_iTrianglesCount = 12;
		m_pTriangles = (Triangle*)ReAlloc(m_pTriangles,m_iTrianglesCount*sizeof(Triangle));
		m_pTrianglesTransform = (Triangle*)ReAlloc(m_pTrianglesTransform,m_iTrianglesCount*sizeof(Triangle));
		Float32 dx = sizes.x * 0.5f;
		Float32 dy = sizes.y * 0.5f;
		Float32 dz = sizes.z * 0.5f;

		enum {TFL = 0, TBL = 1, TBR = 2, TFR = 3, BFL = 4, BBL = 5, BBR = 6, BFR = 7};
		Vec3f points[8];
		points[TFL] = Vec3f( dx,  dy,  dz);
		points[TBL] = Vec3f(-dx,  dy,  dz);
		points[TBR] = Vec3f(-dx, -dy,  dz);
		points[TFR] = Vec3f( dx, -dy,  dz);

		points[BFL] = Vec3f( dx,  dy, -dz);
		points[BBL] = Vec3f(-dx,  dy, -dz);
		points[BBR] = Vec3f(-dx, -dy, -dz);
		points[BFR] = Vec3f( dx, -dy, -dz);

		// top
		m_pTriangles[0] = Triangle(points[TFL], points[TBL], points[TBR]);
		m_pTriangles[1] = Triangle(points[TFL], points[TBR], points[TFR]);
		// bottom
		m_pTriangles[2] = Triangle(points[BFL], points[BBR], points[BBL]);
		m_pTriangles[3] = Triangle(points[BFL], points[BFR], points[BBR]);
		// left
		m_pTriangles[4] = Triangle(points[BFL], points[BBL], points[TBL]);
		m_pTriangles[5] = Triangle(points[BFL], points[TBL], points[TFL]);
		// right
		m_pTriangles[6] = Triangle(points[BFR], points[TFR], points[TBR]);
		m_pTriangles[7] = Triangle(points[BFR], points[TBR], points[BBR]);
		// front
		m_pTriangles[8] = Triangle(points[TFL], points[BFR], points[BFL]);
		m_pTriangles[9] = Triangle(points[TFL], points[TFR], points[BFR]);
		// back
		m_pTriangles[10] = Triangle(points[TBL], points[BBR], points[TBR]);
		m_pTriangles[11] = Triangle(points[TBL], points[BBL], points[BBR]);

		m_pAABB.min = Vec3f(-dx, -dy, -dz);
		m_pAABB.max = Vec3f(dx, dy, dz);

		RebuildAABB();
	}
	else if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
	{
		RebuildAABB();
	}
}

CCollision::~CCollision(void)
{
	SAFE_DELETE_ARRAY(m_pTriangles);
}

void CCollision::UpdateCollisionData(Mat4f transform)
{
	m_mTransform = transform;
	if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)return;
	for(uint32 i=0;i<m_iTrianglesCount;i++)
	{
		for(uint32 j=0;j<3;j++)
		{
			m_pTrianglesTransform[i].v[j] = transform * m_pTriangles[i].v[j];
		}
		m_pTrianglesTransform[i].normal = transform * m_pTriangles[i].normal;
		m_pTrianglesTransform[i].Update();
	}
	RebuildAABB();
}

void CCollision::DrawCollisionData(CRender *render)
{
	for(uint32 i=0;i<m_iTrianglesCount;i++)
	{
		render->DrawTriangle(m_pTriangles[i].v[0],m_pTriangles[i].v[1],m_pTriangles[i].v[2]);
	}
}

void CCollision::DrawAABB(CRender *render)
{
	Vec3f max = m_pAABB.max;
	Vec3f min = m_pAABB.min;

	render->DrawPoint(max,10);
	render->DrawPoint(min,5);
}

void CCollision::RebuildAABB()
{
	if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX)
	{
		
		m_pAABB.p = Vec3f(m_mTransform.mat[12],m_mTransform.mat[13],m_mTransform.mat[14]);

		m_pAABB.max = m_pAABB.p;
		m_pAABB.min = m_pAABB.p;

		for(uint32 i=0;i<12;i++)
		{
			for(uint32 j=0;j<3;j++)
			{
				if(m_pTrianglesTransform[i].v[j].x < m_pAABB.min.x)m_pAABB.min.x = m_pTrianglesTransform[i].v[j].x;
				if(m_pTrianglesTransform[i].v[j].y < m_pAABB.min.y)m_pAABB.min.y = m_pTrianglesTransform[i].v[j].y;
				if(m_pTrianglesTransform[i].v[j].z < m_pAABB.min.z)m_pAABB.min.z = m_pTrianglesTransform[i].v[j].z;

				if(m_pTrianglesTransform[i].v[j].x > m_pAABB.max.x)m_pAABB.max.x = m_pTrianglesTransform[i].v[j].x;
				if(m_pTrianglesTransform[i].v[j].y > m_pAABB.max.y)m_pAABB.max.y = m_pTrianglesTransform[i].v[j].y;
				if(m_pTrianglesTransform[i].v[j].z > m_pAABB.max.z)m_pAABB.max.z = m_pTrianglesTransform[i].v[j].z;
				
			}
		}
		

		m_pAABB.size = Vec3f((m_pAABB.max.x-m_pAABB.min.x)*0.5f,(m_pAABB.max.y-m_pAABB.min.y)*0.5f,(m_pAABB.max.z-m_pAABB.min.z)*0.5f);

	}
	else if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
	{
		m_pAABB.p = Vec3f(m_mTransform.mat[12],m_mTransform.mat[13],m_mTransform.mat[14]);
	}
}

Bool CCollision::AABBOverlap(AABB a,AABB b)
{
	Vec3f T = a.p - b.p;
	return fabs(T.x) <= (a.size.x + b.size.x) &&
          fabs(T.y) <= (a.size.y + b.size.y) &&
          fabs(T.z) <= (a.size.z + b.size.z);
}

Bool CCollision::RigidBodyOverlap(CRigidBody *a,CRigidBody *b)
{
	Bool ret = false;
	if(a->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE &&
		b->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
	{
		Float32 a_r = a->GetSizes().x * 0.5f;
		Float32 b_r = b->GetSizes().x * 0.5f;
		Vec3f p = a->GetPosition() - b->GetPosition();
		if(p.length() <= a_r + b_r)
		{
			Vec3f normal = p;
			normal.normalize();
			a->AddContact(b,a->GetPosition() - (normal*a_r),normal,math_fabs((a_r + b_r) - p.length()));
			ret = true;
		}
	}
	else if(a->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE &&
			b->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_PLANE)
		{
			Float32 a_r = a->GetSizes().x * 0.5f;
			Vec3f normal = Vec3f(b->GetSizes().x,b->GetSizes().y,b->GetSizes().z);
			normal.normalize();
			Float32 d = DistancePointPlane(Vec3f(0,0,0),normal,a->GetPosition());
			if(d - a_r <= 0)
			{
				a->AddContact(b,a->GetPosition() - (normal*a_r),normal,math_fabs(d - a_r));
				ret = true;
			}
	}
	else if(a->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX &&
			b->GetType() == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX)
	{

		TriangleOverlap(a,b);
		ret = true;
	}
	return ret;
}

Bool CCollision::TriangleSegmentOverlap(Triangle t1,Vec3f s1,Vec3f s2,Float32 &depth,Vec3f &p)
{
	return false;	
}

Bool CCollision::TriangleOverlap(CRigidBody *a,CRigidBody *b)
{
	int next[3] = { 1, 2, 0 };
	for(uint32 i=0;i<12;i++)//a
	{
		for(uint32 j=0;j<12;j++)//b
		{		
			Triangle t;
			Triangle ct;

			Triangle t2 = Triangle( a->GetCollision()->GetTriangleTransform(i).v[0],
									a->GetCollision()->GetTriangleTransform(i).v[1],
									a->GetCollision()->GetTriangleTransform(i).v[2]);

			Triangle t1 = Triangle( b->GetCollision()->GetTriangleTransform(j).v[0],
									b->GetCollision()->GetTriangleTransform(j).v[1],
									b->GetCollision()->GetTriangleTransform(j).v[2]);
			for(int ii=0;ii<3;ii++)
			{
				t.v[ii] = t1.v[ii];
				ct.v[ii] = t2.v[ii];
				t.c[ii] = t1.c[ii];
				ct.c[ii] = t2.c[ii];
			}
			t.normal = t1.normal;
			ct.normal = t2.normal;
			t.plane = t1.plane;
			ct.plane = t2.plane;
			
			float dist0[3];
			int collide0 = 0;
			
			float dist1[3];
			int collide1 = 0;
			
			// intersection of planes
			for(int m = 0; m < 3; m++) 
			{
				dist0[m] = Vec4f(ct.v[m],1) * t.plane;
				
				if(dist0[m] > 0.0) collide0++;
				else if(dist0[m] < 0.0) collide0--;
			}
			if(collide0 == 3 || collide0 == -3) continue;
			
			for(int m = 0; m < 3; m++) 
			{
				dist1[m] = Vec4f(t.v[m],1) * ct.plane;

				if(dist1[m] > 0.0)
					collide1++;
				else if(dist1[m] < 0.0)
					collide1--;
			}
			if(collide1 == 3 || collide1 == -3) continue;
			
			for(int m = 0; m < 3; m++) 
			{
				if(dist0[m] >= 0.0) continue;		// point is above of the plane
				if(dist0[next[m]] < 0.0) continue;	// edge doesn`t intersection with plane
				
				Vec3f p0 = ct.v[m];
				Vec3f edge = ct.v[next[m]] - ct.v[m];
				Vec3f p1 = p0 - edge * dist0[m] / (dist0[next[m]] - dist0[m]);	// intersect edge with plane
				
				// is this point in the triangle ??
				if((p1 - t.v[0]) * t.c[0] < 0.0) continue;
				if((p1 - t.v[1]) * t.c[1] < 0.0) continue;
				if((p1 - t.v[2]) * t.c[2] < 0.0) continue;
				
				// add intersection point
				//vec3 n = t->plane - ct->plane;
				//n.normalize();
				a->AddContact(b,p1,t.plane,0.0);
				
				// add vertex of the traingle
				if((p0 - t.v[0]) * t.c[0] < 0.0) continue;
				if((p0 - t.v[1]) * t.c[1] < 0.0) continue;
				if((p0 - t.v[2]) * t.c[2] < 0.0) continue;
				
				a->AddContact(b,p0,t.plane,-dist0[m]);
			}
			
			for(int m = 0; m < 3; m++) 
			{
				if(dist1[m] >= 0.0) continue;		// point is above of the triangle plane
				if(dist1[next[m]] < 0.0) continue;	// edge doesn`t intersection with traingle plane
				
				Vec3f p0 = t.v[m];
				Vec3f edge = t.v[next[m]] - t.v[m];
				Vec3f p1 = p0 - edge * dist1[m] / (dist1[next[m]] - dist1[m]);	// intersect edge with plane
				
				// is this point in the triangle ??
				if((p1 - ct.v[0]) * ct.c[0] < 0.0) continue;
				if((p1 - ct.v[1]) * ct.c[1] < 0.0) continue;
				if((p1 - ct.v[2]) * ct.c[2] < 0.0) continue;
				
				// add intersection point
				a->AddContact(b,p1,t.plane,0.0f);
				
				// add vertex of the traingle
				if((p0 - ct.v[0]) * ct.c[0] < 0.0) continue;
				if((p0 - ct.v[1]) * ct.c[1] < 0.0) continue;
				if((p0 - ct.v[2]) * ct.c[2] < 0.0) continue;
				
				a->AddContact(b,p0,t.plane,dist1[m]);
			}

		}
	}
	
	return false;
}

Float32 CCollision::DistancePointPlane(Vec3f plane_point,Vec3f normal,Vec3f point)
{
    return (normal.x * point.x + normal.y * point.y + normal.z * point.z - normal.x * plane_point.x - normal.y * plane_point.y - normal.z * plane_point.z);
}

//Physic
CPhysic::CPhysic(void)
{
	m_fTime = 0.0f;
	m_fTimeStep = 1.0f / 60.0f;
	m_fGravitation = -9.8f;
	m_fVelocityMax = 20.0f;
	m_fVelocityThreshold = 0.1f * 0.1f;
	m_fAngularVelocityThreshold = (2.0f * DEG2RAD) * (2.0f * DEG2RAD);
	m_fTimeToFrost = 1.0f / 5.0f;
	m_fPenetrationSpeed = 1.0f / 5.0f;
	m_iNumFirstIterations = 1;
	m_iNumSecondIterations = 15;
	m_iRigidBodysCount = 0;
	m_pRigidBodys = new CRigidBody*[1];
}

CPhysic::~CPhysic(void)
{
	for(uint32 i=0;i<m_iRigidBodysCount;i++)
	{
		SAFE_DELETE(m_pRigidBodys[i]);
	}
	SAFE_DELETE_ARRAY(m_pRigidBodys);
}

void CPhysic::DrawContacts(CRender *render)
{
	for(uint32 i=0;i<m_iRigidBodysCount;i++)
	{
		for(uint32 j=0;j<m_pRigidBodys[i]->GetContactsNum();j++)
		{
			render->DrawPoint(m_pRigidBodys[i]->GetCountact(j).point,10);
			render->DrawLine(m_pRigidBodys[i]->GetCountact(j).point,Vec3f(m_pRigidBodys[i]->GetCountact(j).point+m_pRigidBodys[i]->GetCountact(j).normal*(m_pRigidBodys[i]->GetCountact(j).depth+1)*10),10);
		}
	}
}

uint32 CPhysic::AddRigidBody(CRigidBody::RIGID_BODY_TYPE type,Float32 *sizes,uint32 contacts_num)
{
	m_iRigidBodysCount++;
	uint32 rigidbody_id = m_iRigidBodysCount-1;
	m_pRigidBodys = (CRigidBody**)ReAlloc(m_pRigidBodys,m_iRigidBodysCount*sizeof(CRigidBody));
	m_pRigidBodys[rigidbody_id] = new CRigidBody(type,sizes,contacts_num);
	return rigidbody_id;
}

void CPhysic::SeparateRigidBody(CRigidBody *a,CRigidBody *b)
{
	if(a->GetImmovable() && b->GetImmovable())return;
};

void CPhysic::Update(Float32 step)
{
	m_fTime += step;
	
	while(m_fTime > m_fTimeStep)
	{
		
		m_fTime -= m_fTimeStep;

		for(uint32 i = 0;i < m_iRigidBodysCount;i++) 
		{			
			m_pRigidBodys[i]->SetForce(Vec3f(0,0,0));
			m_pRigidBodys[i]->SetTorque(Vec3f(0,0,0));
			
			if(m_pRigidBodys[i]->GetFrozen() == 0) 
			{
				m_pRigidBodys[i]->FindContacts(m_pRigidBodys,m_iRigidBodysCount,m_fVelocityMax,m_fTimeStep);
			}
			
			m_pRigidBodys[i]->SetFrozen(0);
		}

		
		
		for(uint32 i = 0;i<m_iNumFirstIterations;i++) 
		{
			int32 done = 1;
			for(uint32 j = 0;j < m_iRigidBodysCount;j++) 
			{
				if(m_pRigidBodys[j]->ContactsResponse(m_fPenetrationSpeed,m_fTimeStep) == 0) done = 0;
			}
			/*for(uint32 j = 0;j < num_joints;j++) 
			{
				joints[j]->response(time_step);
			}
			*/
			if(done) break;
		}
		
		for(uint32 i = 0;i < m_iRigidBodysCount;i++) 
		{	
			if(m_pRigidBodys[i]->GetContactsNum() > 0 /*&& rb->num_joints == 0*/ &&
				m_pRigidBodys[i]->GetVelocity() * m_pRigidBodys[i]->GetVelocity() < m_fVelocityThreshold &&
				m_pRigidBodys[i]->GetAngularVelocity() * m_pRigidBodys[i]->GetAngularVelocity() < m_fAngularVelocityThreshold) 
			{
				
				m_pRigidBodys[i]->SetFrozenTime(m_pRigidBodys[i]->GetFrozenTime() + m_fTimeStep);
				
				if(m_pRigidBodys[i]->GetFrozenTime() > m_fTimeToFrost) 
				{
					if(m_pRigidBodys[i]->GetFrozenNumObjects() == m_pRigidBodys[i]->GetCollidedRigidBodysNum()) 
					{
						m_pRigidBodys[i]->SetFrozen(1);
						m_pRigidBodys[i]->SetVelocity(Vec3f(0,0,0));
						m_pRigidBodys[i]->SetAngularVelocity(Vec3f(0,0,0));
					}
					else 
					{
						m_pRigidBodys[i]->SetFrozen(0);
						m_pRigidBodys[i]->SetFrozenTime(0.0f);
						m_pRigidBodys[i]->FindContacts(m_pRigidBodys,m_iRigidBodysCount,m_fVelocityMax,m_fTimeStep);
					}
				}
				m_pRigidBodys[i]->SetFrozenNumObjects(m_pRigidBodys[i]->GetCollidedRigidBodysNum());
				
			}
			else
			{
				m_pRigidBodys[i]->SetFrozen(0);
				m_pRigidBodys[i]->SetFrozenTime(0.0f);
				m_pRigidBodys[i]->FindContacts(m_pRigidBodys,m_iRigidBodysCount,m_fVelocityMax,m_fTimeStep);
			}
			
			m_pRigidBodys[i]->CalcForces(m_fGravitation,m_fTimeStep);
			m_pRigidBodys[i]->IntegrateVelocity(m_fVelocityMax,m_fTimeStep);
		}
		
		for(uint32 i = 0;i < m_iNumSecondIterations;i++) 
		{
			int done = 1;
			for(uint32 j = 0;j < m_iRigidBodysCount;j++) 
			{
				if(m_pRigidBodys[j]->ContactsResponse(m_fPenetrationSpeed,m_fTimeStep,true) == 0) done = 0;
			}
			/*for(uint32 j = 0;j < num_joints; j++) 
			{
				joints[j]->response(time_step);
			}
			*/
			if(done) break;
		}
		
		for(uint32 i = 0;i < m_iRigidBodysCount;i++) 
		{
			if(m_pRigidBodys[i]->GetFrozen() == 0) m_pRigidBodys[i]->IntegratePosition(m_fTimeStep);
		}
	}
	
	// new simulate - new objects
	for(uint32 i = 0;i < m_iRigidBodysCount;i++) 
	{
		m_pRigidBodys[i]->SetSimulated(0);
		m_pRigidBodys[i]->SetImmovable(0);
	}


	
	//num_rigidbodies = 0;
	//num_joints = 0;
}

//RigidBody
CRigidBody::CRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,uint32 contacts_num)
{
	m_vSizes = sizes;
	m_iType = type;
	m_iMaxContactsNum = contacts_num;
	m_iContactsCount = 0;
	m_iCollidedRigidBodysCount = 0;
	m_pContacts = new Contact[contacts_num];
	m_pCollision = new CCollision(type,sizes);
	m_vPosition = Vec3f(0.0f,0.0f,0.0f);
	m_vVelocity = Vec3f(0.0f,0.0f,0.0f);
	m_vAngularVelocity = Vec3f(0.0f,0.0f,0.0f);
	m_vAngularMomentum = Vec3f(0.0f,0.0f,0.0f);
	m_vForce = Vec3f(0.0f,0.0f,0.0f);
	m_vTorque = Vec3f(0.0f,0.0f,0.0f);
	
	m_mOrienation.identity();
	m_mIBodyInertiaTensor.identity();
	m_mIWorldInertiaTensor.identity();
	m_mTransform.identity();
	m_mITransform.identity();
	
	m_FrozenNumObjects = 0;
	m_fMass = 1.0f;
	m_fIMass = 1.0f / m_fMass;
	m_fRestitution = 0.0f;
	m_fFriction = 0.0f;
	m_fFrozenTime = 0.0f;
	
	m_bFrozen = false;
	m_bSimulated = true;
	m_bImmovable = false;
}

CRigidBody::~CRigidBody(void)
{
	SAFE_DELETE_ARRAY(m_pContacts);
	SAFE_DELETE(m_pCollision);
}

void CRigidBody::AddContact(CRigidBody *b,Vec3f point,Vec3f normal,Float32 depth)
{
	if(m_iContactsCount == m_iMaxContactsNum)
	{
		m_iContactsCount--;
	}

	m_iContactsCount++;
	m_pContacts[m_iContactsCount-1].rb = b;
	m_pContacts[m_iContactsCount-1].normal = normal;
	m_pContacts[m_iContactsCount-1].point = point;
	m_pContacts[m_iContactsCount-1].depth = depth;

	if(m_iCollidedRigidBodysCount == 0)
	{
		m_iCollidedRigidBodysCount++;
	}
	else
	{
		for(uint32 i = 0;i < m_iContactsCount;i++) 
		{
			if(m_pContacts[i].rb != b)
			{
				m_iCollidedRigidBodysCount++;
				break;
			}
		}
	}
}

void CRigidBody::SetMass(Float32 value)
{
	m_fMass = value;
	m_fIMass = 1.0f / m_fMass;
	if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX) 
	{
		Mat3f inertiaTensor;
		inertiaTensor[0] = 1.0f / 12.0f * m_fMass * (m_vSizes[1] * m_vSizes[1] + m_vSizes[2] * m_vSizes[2]);
		inertiaTensor[4] = 1.0f / 12.0f * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[2] * m_vSizes[2]);
		inertiaTensor[8] = 1.0f / 12.0f * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[1] * m_vSizes[1]);
		m_mIBodyInertiaTensor = inertiaTensor.inverse();
	}
	else if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE) 
	{
		Mat3f inertiaTensor;
		inertiaTensor[0] = 2.0f / 5.0f * m_fMass * (m_vSizes[0]*0.5f) * (m_vSizes[0]*0.5f);
		inertiaTensor[4] = 2.0f / 5.0f * m_fMass * (m_vSizes[0]*0.5f) * (m_vSizes[0]*0.5f);
		inertiaTensor[8] = 2.0f / 5.0f * m_fMass * (m_vSizes[0]*0.5f) * (m_vSizes[0]*0.5f);
		m_mIBodyInertiaTensor = inertiaTensor.inverse();
	}
	else if(m_iType == CRigidBody::RIGID_BODY_TYPE::RIGID_BODY_TYPE_CYLINDER) 
	{
		Mat3f inertiaTensor;
		inertiaTensor[0] = 1.0f / 12.0f * m_fMass * m_vSizes[1] * m_vSizes[1];
		inertiaTensor[4] = 1.0f / 12.0f * m_fMass * m_vSizes[1] * m_vSizes[1];
		inertiaTensor[8] = 1.0f / 2.0f  * m_fMass * m_vSizes[0] * m_vSizes[0];
		m_mIBodyInertiaTensor = inertiaTensor.inverse();
	}
}

void CRigidBody::SetStatic(Vec3f pos)
{
	Mat4f m;
	m.translate(pos);
	m_vPosition = m * Vec3f(0,0,0);
	m_mOrienation = Mat3f(m);
	
	m_mTransform = m;
	m_mITransform = m_mTransform.inverse();
	
	m_vVelocity			= Vec3f(0,0,0);
	m_vAngularVelocity	= Vec3f(0,0,0);
	m_vAngularMomentum	= Vec3f(0,0,0);
	
	m_mIWorldInertiaTensor.identity();
	
	m_bFrozen = 0;
	m_fFrozenTime = 0.0f;
	m_bImmovable = 1;
}

void CRigidBody::AddImpulse(Vec3f point,Vec3f impulse)
{
	m_vVelocity += impulse * m_fIMass;
	m_vAngularMomentum += Vec3f(point - m_vPosition)^impulse;
	m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
}

void CRigidBody::CalcForces(Float32 gravitation,Float32 step)
{
	m_vForce = Vec3f(0.0f,0.0f,0.0f);
	m_vTorque = Vec3f(0.0f,0.0f,0.0f);	
	
	if(!m_bFrozen) m_vForce.y += gravitation * m_fMass;

	m_vForce -= m_vVelocity * 0.1f;
}

void CRigidBody::FindContacts(CRigidBody **rigid_bodys,uint32 rigid_bodys_num,Float32 max_velocity,Float32 step)
{
	Vec3f old_velicity;
	Vec3f old_angularMomentum;
	Vec3f old_angularVelocity;
	Vec3f old_pos;
	Mat3f old_orientation;
	Mat3f old_iWorldInertiaTensor;
	
	old_velicity = m_vVelocity;
	old_angularMomentum = m_vAngularMomentum;
	old_angularVelocity = m_vAngularVelocity;
	old_pos = m_vPosition;
	old_orientation = m_mOrienation;
	old_iWorldInertiaTensor = m_mIWorldInertiaTensor;
	
	IntegrateVelocity(max_velocity,step);
	IntegratePosition(step);
	
	// find conact points
	CalculateCollisions(rigid_bodys,rigid_bodys_num);
	
	m_vVelocity = old_velicity;
	m_vAngularMomentum = old_angularMomentum;
	m_vAngularVelocity = old_angularVelocity;
	m_vPosition = old_pos;
	m_mOrienation = old_orientation;
	m_mIWorldInertiaTensor = old_iWorldInertiaTensor;
}

void CRigidBody::IntegrateVelocity(Float32 max_velocity,Float32 step)
{
	m_vVelocity += m_vForce * step * m_fIMass;
	
	Float32 velosity = m_vVelocity.length();
	if(velosity > max_velocity) m_vVelocity *= max_velocity / velosity;
	
	m_vAngularMomentum += m_vTorque * step;
	m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
}

void CRigidBody::IntegratePosition(Float32 step)
{
	m_vPosition += m_vVelocity * step;
	
	Mat3f m;
	m[0] = 0.0;						m[3] = -m_vAngularVelocity[2];	m[6] = m_vAngularVelocity[1];
	m[1] = m_vAngularVelocity[2];	m[4] = 0.0;						m[7] = -m_vAngularVelocity[0];
	m[2] = -m_vAngularVelocity[1];	m[5] = m_vAngularVelocity[0];	m[8] = 0.0;

	m_mOrienation += (m * m_mOrienation) * step;
	m_mOrienation.orthonormalize();
	
	m_mIWorldInertiaTensor = m_mOrienation * m_mIBodyInertiaTensor * m_mOrienation.transpose();
	
	m_mTransform = Mat4f(m_mOrienation);
	m_mTransform[12] = m_vPosition.x;
	m_mTransform[13] = m_vPosition.y;
	m_mTransform[14] = m_vPosition.z;
	m_mITransform = m_mTransform.inverse();

	m_pCollision->UpdateCollisionData(m_mTransform);
}

int32 CRigidBody::ContactsResponse(Float32 penetration_speed,Float32 step,int32 zero_restitution )
{
	int32 done = 1;
	
	for(uint32 i = 0;i < m_iContactsCount;i++) 
	{	



		/*
		if(!m_pContacts[i].rb->GetImmovable())
		{	// rigidbody - rigidbody contact			
			Vec3f r0 = m_pContacts[i].point - m_vPosition;
			Vec3f r1 = m_pContacts[i].point - m_pContacts[i].rb->m_vPosition;
			
			Vec3f vel = (Vec3f(m_vAngularVelocity^r0) + m_vVelocity) - 
				(Vec3f(m_pContacts[i].rb->m_vAngularVelocity^r1) + m_pContacts[i].rb->m_vVelocity);
			
			Float32 normal_vel = m_pContacts[i].normal * vel;
			if(normal_vel > -EPSILON) continue;
			
			Float32 impulse_numerator;
			if(!zero_restitution) impulse_numerator = -(1.0f + m_fRestitution) * normal_vel;
			else impulse_numerator = -normal_vel + m_pContacts[i].depth * penetration_speed / step;
			
			if(impulse_numerator < EPSILON) continue;
			
			Vec3f tangent = -(vel - m_pContacts[i].normal * normal_vel);
			
			done = 0;
			
			Float32 impulse_denominator = 1.0f * m_fIMass + 1.0f * m_pContacts[i].rb->m_fIMass
				+ m_pContacts[i].normal * Vec3f(Vec3f(m_mIWorldInertiaTensor * Vec3f(r0^m_pContacts[i].normal))^r0)
				+ m_pContacts[i].normal * Vec3f(Vec3f(m_pContacts[i].rb->m_mIWorldInertiaTensor * Vec3f(r1^m_pContacts[i].normal))^r1);
			
			Vec3f impulse = m_pContacts[i].normal * (impulse_numerator / impulse_denominator) * (1+m_pContacts[i].depth);
			
			if(m_bFrozen == 0 && m_bImmovable == 0) 
			{
				m_vVelocity += impulse * m_fIMass;
				m_vAngularMomentum += Vec3f(r0^impulse);
				m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
			}
			if(m_pContacts[i].rb->m_bFrozen == 0 && m_pContacts[i].rb->m_bImmovable == 0) 
			{
				m_pContacts[i].rb->m_vVelocity -= impulse * m_pContacts[i].rb->m_fIMass;
				m_pContacts[i].rb->m_vAngularMomentum -= Vec3f(r1^impulse);
				m_pContacts[i].rb->m_vAngularVelocity = m_pContacts[i].rb->m_mIWorldInertiaTensor * m_pContacts[i].rb->m_vAngularMomentum;
			}
			
			// friction
			if(tangent.normalize() < EPSILON) continue;
			
			vel = (Vec3f(m_vAngularVelocity^r0) + m_vVelocity) - (Vec3f(m_pContacts[i].rb->m_vAngularVelocity^r1) + m_pContacts[i].rb->m_vVelocity);
			
			Float32 tangent_vel = tangent * vel;
			if(tangent_vel > -EPSILON) continue;
			
			Float32 friction_numerator = -tangent_vel * m_fFriction;
			Float32 friction_denominator = 1.0f * m_fIMass + 1.0f * m_pContacts[i].rb->m_fIMass +
				tangent * Vec3f(Vec3f(m_mIWorldInertiaTensor * Vec3f(r0^tangent))^r0) +
				tangent * Vec3f(Vec3f(m_pContacts[i].rb->m_mIWorldInertiaTensor * Vec3f(r1^tangent))^r1);
			
			impulse = tangent * friction_numerator / friction_denominator;
			
			if(!m_bFrozen) 
			{
				m_vVelocity += impulse * m_fIMass;
				m_vAngularMomentum += Vec3f(r0^impulse);
				m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
			}
			
			if(!m_pContacts[i].rb->m_bFrozen) 
			{
				m_pContacts[i].rb->m_vVelocity -= impulse * m_pContacts[i].rb->m_fIMass;
				m_pContacts[i].rb->m_vAngularMomentum -= Vec3f(r1^impulse);
				m_pContacts[i].rb->m_vAngularVelocity = m_pContacts[i].rb->m_mIWorldInertiaTensor * m_pContacts[i].rb->m_vAngularMomentum;
			}
		}
		else
		{	// rigidbody - scene contact
			
			if(m_bFrozen) continue;
			
			Vec3f r = m_pContacts[i].point - m_vPosition;
			
			Vec3f vel = Vec3f(m_vAngularVelocity^r) + m_vVelocity;

			Float32 normal_vel = m_pContacts[i].normal * vel;
			if(normal_vel > -EPSILON) continue;
			
			Float32 impulse_numerator;
			if(!zero_restitution) impulse_numerator = -(1.0f + m_fRestitution) * normal_vel;
			else impulse_numerator = -normal_vel + m_pContacts[i].depth * penetration_speed / step;
			
			if(impulse_numerator < EPSILON) continue;
			
			Vec3f tangent = -(vel - m_pContacts[i].normal * normal_vel);
			
			done = 0;
			
			Float32 impulse_denominator = 1.0f * m_fIMass +
				m_pContacts[i].normal * Vec3f(Vec3f(m_mIWorldInertiaTensor * Vec3f(r^m_pContacts[i].normal))^r);
			
			Vec3f impulse = m_pContacts[i].normal * (impulse_numerator / impulse_denominator)  * (1+m_pContacts[i].depth);
			
			m_vVelocity += impulse * m_fIMass;
			m_vAngularMomentum += Vec3f(r^impulse);
			m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
			
			// friction
			if(tangent.normalize() < EPSILON) continue;
			
			vel = Vec3f(m_vAngularVelocity^r) + m_vVelocity;

			Float32 tangent_vel = tangent * vel;
			if(tangent_vel > -EPSILON) continue;
			
			Float32 friction_numerator = -tangent_vel * m_fFriction;
			Float32 friction_denominator = 1.0f * m_fIMass + tangent * Vec3f(Vec3f(m_mIWorldInertiaTensor * Vec3f(r^tangent))^r);
			
			impulse = tangent * friction_numerator / friction_denominator;
			
			m_vVelocity += impulse * m_fIMass;
			m_vAngularMomentum += Vec3f(r^impulse);
			m_vAngularVelocity = m_mIWorldInertiaTensor * m_vAngularMomentum;
		}
		*/
	}
	return done;
}

void CRigidBody::CalculateCollisions(CRigidBody **rigid_bodys,uint32 rigid_bodys_num)
{
	m_iContactsCount = 0;
	for(uint32 i=0;i<rigid_bodys_num;i++)
	{
		if(this != rigid_bodys[i])
		{
			//if(m_pCollision->AABBOverlap(GetCollision()->GetAABB(),rigid_bodys[i]->GetCollision()->GetAABB()))
			//{
				m_pCollision->RigidBodyOverlap(this,rigid_bodys[i]);
			//}
		}
	}
}