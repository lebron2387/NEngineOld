
#include "Physic.h"

CPhysic::CPhysic()
{
	m_pBodies = new CRigidBody*[1];
	m_pJoints = new CJoint*[1];
	m_pCollision = new CCollision();

	m_iRigidBodiesCount = 0;
	m_iJointsCount = 0;
	m_iIterations = 10;
	m_iBroadPhasePairs = 0;
	m_iNarrowPhasePairs = 0;
	m_iContactsCount = 0;
	m_vGravity = Vec3f(0.0f, -9.8f,0.0f);

	m_fDumping = 0.9999f;

	m_iSolverTime = 0;
	m_iBroadPhaseTime = 0;
	m_iNarrowPhaseTime = 0;
	m_iTotalTime = 0;
};

CPhysic::~CPhysic()
{
	for(uint32 i=0;i<m_iRigidBodiesCount;i++)
	{
		SAFE_DELETE(m_pBodies[i]);
	}
	SAFE_DELETE_ARRAY(m_pBodies);
	for(uint32 i=0;i<m_iJointsCount;i++)
	{
		SAFE_DELETE(m_pJoints[i]);
	}
	SAFE_DELETE_ARRAY(m_pJoints);
	SAFE_DELETE(m_pCollision);
};

uint32 CPhysic::AddRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,Float32 mass)
{
	m_iRigidBodiesCount++;
	uint32 rigidbody_id = m_iRigidBodiesCount-1;
	m_pBodies = (CRigidBody**)ReAlloc(m_pBodies,m_iRigidBodiesCount*sizeof(CRigidBody));
	m_pBodies[rigidbody_id] = new CRigidBody(type,sizes,mass);
	return rigidbody_id;
}

uint32 CPhysic::AddJointDistance(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 max_distance)
{
	m_iJointsCount++;
	uint32 rigidbody_id = m_iJointsCount-1;
	m_pJoints = (CJoint**)ReAlloc(m_pJoints,m_iJointsCount*sizeof(CJoint));
	m_pJoints[rigidbody_id] = new CJointDistance(body0,body0_pos,body1,body1_pos,max_distance);
	return rigidbody_id;
}

uint32 CPhysic::AddJointBall(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 allowed_distance,Float32 timescale)
{
	m_iJointsCount++;
	uint32 rigidbody_id = m_iJointsCount-1;
	m_pJoints = (CJoint**)ReAlloc(m_pJoints,m_iJointsCount*sizeof(CJoint));
	m_pJoints[rigidbody_id] = new CJointBall(body0,body0_pos,body1,body1_pos,allowed_distance,timescale);
	return rigidbody_id;
}

uint32 CPhysic::AddJointHinge(CRigidBody *body0, CRigidBody *body1,Vec3f hinge_axis,Vec3f hinge_pos_rel0,Float32 hinge_half_width,Float32 sideways_slack,Float32 hinge_fwd_angle,Float32 hinge_bck_angle,Float32 damping)
{
	m_iJointsCount++;
	uint32 rigidbody_id = m_iJointsCount-1;
	m_pJoints = (CJoint**)ReAlloc(m_pJoints,m_iJointsCount*sizeof(CJoint));
	m_pJoints[rigidbody_id] = new CJointHinge(body0,body1,hinge_axis,hinge_pos_rel0,hinge_half_width,sideways_slack,hinge_fwd_angle,hinge_bck_angle,damping);
	return rigidbody_id;
}

int rigidbody_cmp(const void *a,const void *b) 
{
	CRigidBody *r0 = *(CRigidBody**)a;
	CRigidBody *r1 = *(CRigidBody**)b;
	
	//if(r0->GetPosition().v[0] <  r1->GetPosition().v[0] || r0->GetPosition().v[1] <  r1->GetPosition().v[1] || r0->GetPosition().v[2] <  r1->GetPosition().v[2]) return 1;
	//if(r0->GetPosition().v[0] > r1->GetPosition().v[0] || r0->GetPosition().v[1] > r1->GetPosition().v[1] || r0->GetPosition().v[2] > r1->GetPosition().v[2]) return -1;
	if(r0->GetPosition().v[0] <  r1->GetPosition().v[0])return 1;
	if(r0->GetPosition().v[0] >  r1->GetPosition().v[0])return -1;

	return 0;
}

void CPhysic::BroadPhase()
{
	uint32 start = CTimer::Time();
	//qsort(m_pBodies,m_iRigidBodiesCount,sizeof(CRigidBody*),rigidbody_cmp);
	m_iBroadPhaseTime+=CTimer::Time()-start;
	m_iTotalTime+=m_iBroadPhaseTime;
}

void CPhysic::NarrowPhase(CRender *render)
{
	for(uint32 i = 0; i < m_iRigidBodiesCount; ++i)
	{
		m_pBodies[i]->SetContactNum(0);
	}

	m_iBroadPhasePairs = 0;
	m_iNarrowPhasePairs = 0;
	m_iContactsCount = 0;

	for(uint32 i = 0; i < m_iRigidBodiesCount; ++i)
	{
		for(uint32 j = 0; j < m_iRigidBodiesCount; ++j)
		{
			if(m_pBodies[i] != m_pBodies[j]){
/*			if( m_pBodies[j]->GetGeometry()->GetAABB().min.x > m_pBodies[i]->GetGeometry()->GetAABB().max.x &&
				m_pBodies[j]->GetGeometry()->GetAABB().min.y > m_pBodies[i]->GetGeometry()->GetAABB().max.y &&
				m_pBodies[j]->GetGeometry()->GetAABB().min.z > m_pBodies[i]->GetGeometry()->GetAABB().max.z
				)
			{
				break;
			}
*/
				
			++m_iBroadPhasePairs;
			//render->DrawLine(m_pBodies[i]->GetPosition(),m_pBodies[j]->GetPosition(),10);

			uint32 start = CTimer::Time();
			Bool aabb_overlaped = m_pCollision->AABBOverlap(m_pBodies[i]->GetGeometry()->GetAABB(),m_pBodies[j]->GetGeometry()->GetAABB());
			m_iBroadPhaseTime+=CTimer::Time()-start;
			m_iTotalTime+=m_iBroadPhaseTime;

			if(aabb_overlaped)
			{
				++m_iNarrowPhasePairs;
				uint32 start = CTimer::Time();
				m_pCollision->RigidBodyOverlapSDM(m_pBodies[i],m_pBodies[j]);
				m_iContactsCount += m_pBodies[i]->GetContactNum();
				m_iNarrowPhaseTime+=CTimer::Time()-start;
				m_iTotalTime+=m_iNarrowPhaseTime;
			}
			}
		}
	}
}

void CPhysic::Step(Float32 dt,CRender *render)
{
	m_iSolverTime = 0;
	m_iBroadPhaseTime = 0;
	m_iNarrowPhaseTime = 0;
	m_iTotalTime = 0;

	float inv_dt = dt > 0.0f ? 1.0f / dt : 0.0f;
	// Determine overlapping bodies and update contact point4s.
	BroadPhase();
	NarrowPhase(render);
	// Integrate forces.
	uint32 start = CTimer::Time();
	for(uint32 i = 0; i < m_iRigidBodiesCount; ++i)
	{
		if(m_pBodies[i]->GetInvMass() == 0.0f)continue;
		m_pBodies[i]->IntegrateVelocity(m_vGravity,m_fDumping,dt);
	}
	// Perform pre-steps.
	for(uint32 i = 0; i < m_iRigidBodiesCount; ++i)
	{
		m_pBodies[i]->PreStep(inv_dt);
	}
	for(uint32 i = 0; i < m_iJointsCount; ++i)
	{
		m_pJoints[i]->PreStep(inv_dt);
	}
	// Perform iterations
	for(uint32 i = 0; i < m_iIterations; ++i)
	{
		for(uint32 j = 0; j < m_iRigidBodiesCount; ++j)
		{
			m_pBodies[j]->ApplyImpulse();	
		}
		for(uint32 j = 0; j < m_iJointsCount; ++j)
		{
			m_pJoints[j]->ApplyImpulse(dt);	
		}
	}
	// Integrate Velocities
	for(uint32 i = 0; i < m_iRigidBodiesCount; ++i)
	{
		m_pBodies[i]->IntegratePosition(dt);
	}
	m_iSolverTime+=CTimer::Time()-start;
	m_iTotalTime+=m_iSolverTime;
}