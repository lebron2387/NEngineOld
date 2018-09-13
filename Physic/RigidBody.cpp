
#include "Physic.h"
#include "Geometry.h"

CRigidBody::CRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,Float32 mass,uint32 max_contacts)
{
	m_iType = type;
	m_iContactsCount = 0;
	m_iMaxContactsNum = max_contacts;
	m_pContacts = new Contact[m_iMaxContactsNum];
	m_pGeometry = new CGeometry(type,sizes);
	m_vPosition = Vec3f(0.0f,0.0f,0.0f);
	m_mRotation.identity();
	m_mIRotation.identity();
	m_vVelocity = Vec3f(0.0f,0.0f,0.0f);
	m_vAngularVelocity = Vec3f(0.0f,0.0f,0.0f);
	m_vAngularMomentum = Vec3f(0.0f,0.0f,0.0f);
	m_vForce = Vec3f(0.0f,0.0f,0.0f);
	m_vTorque = Vec3f(0.0f,0.0f,0.0f);
	m_fFriction = 0.0f;

	m_vSizes = sizes;
	SetMass(mass);
}

CRigidBody::~CRigidBody()
{
	SAFE_DELETE_ARRAY(m_pContacts);
	SAFE_DELETE(m_pGeometry);
}

void CRigidBody::AddContact(CRigidBody *b,Vec3f point,Vec3f normal,Float32 depth)
{
	for(uint32 i=0;i<m_iContactsCount;i++)
	{
		if(m_pContacts[i].position == point)
		{
			return;
		}
	}
	if(m_iContactsCount == m_iMaxContactsNum)m_iContactsCount--;
	m_iContactsCount++;
	uint32 contact_id = m_iContactsCount - 1;
	m_pContacts[contact_id].body = b;
	m_pContacts[contact_id].normal = normal;
	m_pContacts[contact_id].position = point;
	m_pContacts[contact_id].separation = depth;
}

void CRigidBody::SetMass(Float32 value)
{
	m_fMass = value;

	if (m_fMass < FLT_MAX)
	{
		m_fInvMass = 1.0f / m_fMass;

		if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_SPHERE)
		{
			SetIBodyInertiaTensor(
			(1.0f / 5.0f) * m_fMass * (m_vSizes[1] * m_vSizes[1] + m_vSizes[2] * m_vSizes[2]),
			(1.0f / 5.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[2] * m_vSizes[2]),
			(1.0f / 5.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[1] * m_vSizes[1]));
		}
		else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_BOX) 
		{
			SetIBodyInertiaTensor(
			(1.0f / 3.0f) * m_fMass * (m_vSizes[1] * m_vSizes[1] + m_vSizes[2] * m_vSizes[2]),
			(1.0f / 3.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[2] * m_vSizes[2]),
			(1.0f / 3.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[1] * m_vSizes[1]));
		}
		else if(m_iType == RIGID_BODY_TYPE::RIGID_BODY_TYPE_CYLINDER) 
		{
			SetIBodyInertiaTensor(
			(1.0f / 2.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0]),
			(1.0f / 2.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[1] * m_vSizes[1]),
			(1.0f / 2.0f) * m_fMass * (m_vSizes[0] * m_vSizes[0] + m_vSizes[1] * m_vSizes[1]));
		}
	}
	else
	{
		m_fInvMass = 0.0f;
		for(uint32 i=0;i<9;i++)
		{
			m_mIBodyInertiaTensor.mat[i] = 1.0f/FLT_MAX;
		}
		m_mIWorldInertiaTensor.identity();
	}
}

void CRigidBody::UpdateContacts()
{}

void CRigidBody::DrawContacts(CRender *render)
{
	for(uint32 i=0;i<m_iContactsCount;i++)
	{
		render->DrawPoint(m_pContacts[i].position,10);
		render->DrawLine(m_pContacts[i].position,Vec3f(m_pContacts[i].position+m_pContacts[i].normal*(m_pContacts[i].separation+1)*10));
	}
}

void CRigidBody::IntegratePosition(Float32 dt)
{
	m_vPosition += m_vVelocity * dt;

	Mat3f m;
	m[0] = 0.0; 					m[3] = -m_vAngularVelocity[2];	m[6] = m_vAngularVelocity[1];
	m[1] = m_vAngularVelocity[2];	m[4] = 0.0; 					m[7] = -m_vAngularVelocity[0];
	m[2] = -m_vAngularVelocity[1];	m[5] = m_vAngularVelocity[0];	m[8] = 0.0;

	m_mRotation += (m * m_mRotation) * dt;
	m_mRotation.orthonormalize();

	m_mIRotation = m_mRotation.transpose();
	
	m_mIWorldInertiaTensor = m_mRotation * m_mIBodyInertiaTensor * m_mIRotation;
	
	m_mTransform = Mat4f(m_mRotation);
	m_mTransform[12] = m_vPosition.x;
	m_mTransform[13] = m_vPosition.y;
	m_mTransform[14] = m_vPosition.z;

	m_pGeometry->Update(m_mTransform);

	m_vForce = Vec3f(0.0f, 0.0f, 0.0f);
	m_vTorque = Vec3f(0.0f, 0.0f, 0.0f);
}

void CRigidBody::IntegrateVelocity(Vec3f gravity,Float32 dumping,Float32 dt)
{
	m_vVelocity += (gravity + m_vForce * m_fInvMass) * dt;
	m_vVelocity *= dumping;

	m_vAngularMomentum += m_vTorque * dt;
	m_vAngularVelocity += m_mIWorldInertiaTensor * m_vAngularMomentum;
	m_vAngularVelocity *= dumping;
}

void CRigidBody::PreStep(Float32 dt)
{
	Float32 k_allowedPenetration = 0.0f;
	Float32 k_biasFactor = 0.2f;

	for(uint32 i=0;i<m_iContactsCount;i++)
	{
		Vec3f r1 = m_pContacts[i].position - m_vPosition;
		Vec3f r2 = m_pContacts[i].position - m_pContacts[i].body->m_vPosition;

		// Precompute normal mass, tangent mass, and bias.
		Float32 rn1 = r1 * m_pContacts[i].normal;
		Float32 rn2 = r2 * m_pContacts[i].normal;
		Float32 kNormal = m_fInvMass + m_pContacts[i].body->m_fInvMass;
		//kNormal += invI * (float(r1 * r1) - rn1 * rn1) + m_pContacts[i].body->invI * (float(r2 * r2) - rn2 * rn2);
		m_pContacts[i].massNormal = 1.0f / kNormal;

		Vec3f tangent = m_pContacts[i].normal;
		Float32 rt1 = Float32(r1 * tangent);
		Float32 rt2 = Float32(r2 * tangent);
		Float32 kTangent = m_fInvMass + m_pContacts[i].body->m_fInvMass;
		//kTangent += invI * (float(r1 * r1) - rt1 * rt1) + m_pContacts[i].body->invI * (float(r2 * r2) - rt2 * rt2);
		m_pContacts[i].massTangent = 1.0f /  kTangent;
		Float32 penetration = m_pContacts[i].separation + k_allowedPenetration;
		m_pContacts[i].bias = -k_biasFactor * dt * (0.0f > penetration ? penetration : 0.0f);
	}
}

void CRigidBody::ApplyImpulse()
{
	for(uint32 i=0;i<m_iContactsCount;i++)
	{
		m_pContacts[i].r1 = m_pContacts[i].position - m_vPosition;
		m_pContacts[i].r2 = m_pContacts[i].position - m_pContacts[i].body->m_vPosition;

		// Relative velocity at contact
		Vec3f dv = m_pContacts[i].body->m_vVelocity + (m_pContacts[i].body->m_vAngularVelocity ^ m_pContacts[i].r2) - m_vVelocity - (m_vAngularVelocity ^ m_pContacts[i].r1);

		Float32 vn = (dv * m_pContacts[i].normal);

		Float32 dPn = m_pContacts[i].massNormal * (-vn + m_pContacts[i].bias);
	
		dPn = (dPn > 0.0f ? dPn : 0.0f);

		// Apply contact impulse
		Vec3f Pn = m_pContacts[i].normal * dPn;

		m_vVelocity -= Pn * m_fInvMass;
		m_vAngularVelocity -= m_mIWorldInertiaTensor * (m_pContacts[i].r1 ^ Pn);

		m_pContacts[i].body->m_vVelocity += Pn * m_pContacts[i].body->m_fInvMass;
		m_pContacts[i].body->m_vAngularVelocity += m_pContacts[i].body->m_mIWorldInertiaTensor * (m_pContacts[i].r2 ^ Pn);
		
		// Relative velocity at contact
		dv = m_pContacts[i].body->m_vVelocity + (m_pContacts[i].body->m_vAngularVelocity ^ m_pContacts[i].r2) - m_vVelocity - (m_vAngularVelocity ^ m_pContacts[i].r1);

		Vec3f tangent = -dv;
		tangent.normalize();
		
		Float32 vt = (dv * tangent);
		Float32 dPt = m_pContacts[i].massTangent * (-vt);

		Float32 maxPt = m_fFriction * dPn;
		dPt = math_clamp(dPt, -maxPt, maxPt);

		// Apply contact impulse
		Vec3f Pt = tangent * dPt;

		m_vVelocity -= Pt * m_fInvMass;
		m_vAngularVelocity -= m_mIWorldInertiaTensor * (m_pContacts[i].r1 ^ Pt);

		m_pContacts[i].body->m_vVelocity += Pt * m_pContacts[i].body->m_fInvMass;
		m_pContacts[i].body->m_vAngularVelocity += m_pContacts[i].body->m_mIWorldInertiaTensor * (m_pContacts[i].r2 ^ Pt);		
	}
}