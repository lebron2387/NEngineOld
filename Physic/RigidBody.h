#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"



#define MAX_BODY_CONTACTS 1024

enum RIGID_BODY_TYPE
{
	RIGID_BODY_TYPE_NONE = 0,
	RIGID_BODY_TYPE_SPHERE = 1,
	RIGID_BODY_TYPE_BOX = 2,
	RIGID_BODY_TYPE_CYLINDER = 3,
	//RIGID_BODY_TYPE_CAPSULE = 4,
	//RIGID_BODY_TYPE_PLANE = 5,
	//RIGID_BODY_TYPE_MESH = 6,

};

class CGeometry;
class CRigidBody;

struct Contact
{
	Contact()
	{
		Pn = 0.0f;
		Pt = 0.0f;
		Pnb = 0.0f; 
	}

	CRigidBody *body;
	Vec3f position;
	Vec3f normal;
	Vec3f r1, r2;
	Float32 separation;
	Float32 Pn;	// accumulated normal impulse
	Float32 Pt;	// accumulated tangent impulse
	Float32 Pnb;	// accumulated normal impulse for position bias
	Float32 massNormal, massTangent;
	Float32 bias;
};

class CRigidBody
{
public:
	CRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,Float32 mass = FLT_MAX,uint32 max_contacts = MAX_BODY_CONTACTS);
	~CRigidBody();

	void DrawContacts(CRender *render);
	void AddContact(CRigidBody *b,Vec3f point,Vec3f normal,Float32 depth);
	void UpdateContacts();
	void PreStep(Float32 dt);
	void ApplyImpulse();
	void IntegratePosition(Float32 dt);
	void IntegrateVelocity(Vec3f gravity,Float32 dumping,Float32 dt);

	__forceinline void AddForce(Vec3f force) {m_vForce += force;}
	__forceinline void AddForce(Vec3f force, Vec3f pos) {  m_vForce += force; m_vTorque += (pos - m_vPosition ^ force); }
	__forceinline void AddTorque(Vec3f torque) {m_vTorque += torque;}
	__forceinline void AddTorque(Vec3f torque, Vec3f pos) { m_vTorque += torque; m_vForce += (pos - m_vPosition ^ torque); }

	__forceinline void AddImpulse(Vec3f impulse) { m_vVelocity += impulse * m_fInvMass; }
	__forceinline void AddImpulse(Vec3f impulse, Vec3f pos) {  m_vVelocity += impulse * m_fInvMass; m_vAngularVelocity += m_mIWorldInertiaTensor * (pos - m_vPosition ^ impulse);}
	__forceinline void AddAngImpulse(Vec3f ang_impulse){ m_vAngularVelocity += m_mIWorldInertiaTensor * ang_impulse; }
	

	
	__forceinline void SetPosition(Vec3f value){m_vPosition = value;}
	__forceinline Vec3f GetPosition(){return m_vPosition;}
	__forceinline void SetVelocity(Vec3f value){m_vVelocity = value;}
	__forceinline Vec3f GetVelocity(){return m_vVelocity;}
	__forceinline void SetAngularVelocity(Vec3f value){m_vAngularVelocity = value;}
	__forceinline Vec3f GetAngularVelocity(){return m_vAngularVelocity;}
	__forceinline void SetAngularMomentum(Vec3f value){m_vAngularMomentum = value;}
	__forceinline Vec3f GetAngularMomentum(){return m_vAngularMomentum;}
	__forceinline void SetForce(Vec3f value){m_vForce = value;}
	__forceinline Vec3f GetForce(){return m_vForce;}
	__forceinline void SetTorque(Vec3f value){m_vTorque = value;}
	__forceinline Vec3f GetTorque(){return m_vTorque;}

	__forceinline void SetRotation(Mat3f value){m_mRotation = value;}
	__forceinline Mat3f GetRotation(){return m_mRotation;}
	__forceinline void SetIRotation(Mat3f value){m_mIRotation = value;}
	__forceinline Mat3f GetIRotation(){return m_mIRotation;}
	__forceinline void SetIBodyInertiaTensor(Float32 Ix,Float32 Iy,Float32 Iz)
	{
		Mat3f inertiaTensor;
		inertiaTensor[0] = Ix;
		inertiaTensor[4] = Iy;
		inertiaTensor[8] = Iz;
		m_mIBodyInertiaTensor = inertiaTensor.inverse();
	}
	__forceinline Mat3f GetIBodyInertiaTensor(){return m_mIBodyInertiaTensor;}
	__forceinline void SetIWorldInertiaTensor(Mat3f value){m_mIWorldInertiaTensor = value;}
	__forceinline Mat3f GetIWorldInertiaTensor(){return m_mIWorldInertiaTensor;}
	__forceinline void SetTransform(Mat4f value){m_mTransform = value;}
	__forceinline Mat4f GetTransform(){return m_mTransform;}

	void SetSizes(Vec4f sizes);
	__forceinline Vec4f GetSizes(){return m_vSizes;}
	__forceinline void SetFriction(Float32 value){m_fFriction = value;}
	__forceinline Float32 GetFriction(){return m_fFriction;}
	void SetMass(Float32 value);
	__forceinline Float32 GetMass(){return m_fMass;}
	__forceinline Float32 GetInvMass(){return m_fInvMass;}

	__forceinline void SetType(uint32 value){m_iType = value;}
	__forceinline uint32 GetType(){return m_iType;}

	__forceinline Contact GetContact(uint32 id){return m_pContacts[id];}
	__forceinline void SetContactNum(uint32 value){m_iContactsCount = value;}
	__forceinline uint32 GetContactNum(){return m_iContactsCount;}
	__forceinline void SetMaxContactNum(uint32 value){m_iMaxContactsNum = value;}
	__forceinline uint32 GetMaxContactNum(){return m_iMaxContactsNum;}

	__forceinline CGeometry *GetGeometry(){return m_pGeometry;}

private:
	CGeometry *m_pGeometry;
	Contact *m_pContacts;
	uint32 m_iContactsCount;
	uint32 m_iMaxContactsNum;

	Vec3f m_vPosition;
	Vec3f m_vVelocity;
	Vec3f m_vAngularVelocity;
	Vec3f m_vAngularMomentum;
	Vec3f m_vForce;
	Vec3f m_vTorque;
	Vec4f m_vSizes;
	Mat3f m_mRotation;
	Mat3f m_mIRotation;
	Mat3f m_mIBodyInertiaTensor;
	Mat3f m_mIWorldInertiaTensor;
	Mat4f m_mTransform;

	Float32 m_fFriction;
	Float32 m_fMass;
	Float32 m_fInvMass;
	uint32 m_iType;
};
