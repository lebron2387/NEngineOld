#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

#define MAX_CONTACTS 8

class CRigidBody;

struct AABB
{
	Vec3f p;
	Vec3f size;
	Vec3f min;
	Vec3f max;
};

struct Contact 
{
	CRigidBody *rb;
	Vec3f point;
	Vec3f normal;
	Float32 depth;
};

struct Triangle
{
	Triangle(){}
	Triangle(Vec3f v0,Vec3f v1,Vec3f v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
		normal = Vec3f(v[1]-v[0])^Vec3f(v[2]-v[0]);
		normal.normalize();
		plane = Vec4f(normal,-v[0] * normal);
		c[0] = Vec3f(plane.v)^(v[0] - v[2]);
		c[1] = Vec3f(plane.v)^(v[1] - v[0]);
		c[2] = Vec3f(plane.v)^(v[2] - v[1]);
	}

	void Update()
	{
		plane = Vec4f(normal,-v[0] * normal);
		c[0] = Vec3f(plane.v)^(v[0] - v[2]);
		c[1] = Vec3f(plane.v)^(v[1] - v[0]);
		c[2] = Vec3f(plane.v)^(v[2] - v[1]);
	}
	Vec3f v[3];
	Vec3f c[3];
	Vec3f normal;
	Vec4f plane;
};

class CCollision
{
public:
	CCollision(uint32 type,Vec4f sizes);
	~CCollision(void);
	
	Bool AABBOverlap(AABB a,AABB b);
	Bool RigidBodyOverlap(CRigidBody *a,CRigidBody *b);

	void RebuildAABB();
	void UpdateCollisionData(Mat4f transform);
	void DrawCollisionData(CRender *render);
	void DrawAABB(CRender *render);
	
	Triangle GetTriangle(uint32 id){return m_pTriangles[id];}
	Triangle GetTriangleTransform(uint32 id){return m_pTrianglesTransform[id];}
	
	Float32 DistancePointPlane(Vec3f plane_point,Vec3f normal,Vec3f point);

	Bool TriangleOverlap(CRigidBody *a,CRigidBody *b);
	Bool TriangleSegmentOverlap(Triangle t1,Vec3f s1,Vec3f s2,Float32 &depth,Vec3f &p);
	
	AABB GetAABB(){return m_pAABB;}
private:
	uint32 m_iType;
	Vec3f m_vSizes;
	Mat4f m_mTransform;
	uint32 m_iTrianglesCount;
	Triangle *m_pTriangles;
	Triangle *m_pTrianglesTransform;
	AABB m_pAABB;
};

class CRigidBody
{
public:
	enum RIGID_BODY_TYPE
	{
		RIGID_BODY_TYPE_NONE = 0,
		RIGID_BODY_TYPE_SPHERE = 1,
		RIGID_BODY_TYPE_BOX = 2,
		RIGID_BODY_TYPE_CYLINDER = 3,
		RIGID_BODY_TYPE_PLANE = 4,
	};

	CRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,uint32 contacts_num = MAX_CONTACTS);
	~CRigidBody(void);

	void SetStatic(Vec3f pos);
	void AddContact(CRigidBody *b,Vec3f point,Vec3f normal,Float32 depth);
	void AddImpulse(Vec3f point,Vec3f impulse);
	void CalcForces(Float32 gravitation,Float32 step);
	void FindContacts(CRigidBody **rigid_bodys,uint32 rigid_bodys_num,Float32 max_velocity,Float32 step);
	void IntegrateVelocity(Float32 max_velocity,Float32 step);
	void IntegratePosition(Float32 step);
	int32 ContactsResponse(Float32 penetration_speed,Float32 step,int32 zero_restitution = 0);
	void CalculateCollisions(CRigidBody **rigid_bodys,uint32 rigid_bodys_num);

	inline void SetPosition(Vec3f value){m_vPosition = value;}
	inline Vec3f GetPosition(){return m_vPosition;}
	inline void SetVelocity(Vec3f value){m_vVelocity = value;}
	inline Vec3f GetVelocity(){return m_vVelocity;}
	inline void SetAngularVelocity(Vec3f value){m_vAngularVelocity = value;}
	inline Vec3f GetAngularVelocity(){return m_vAngularVelocity;}
	inline void SetAngularMomentum(Vec3f value){m_vAngularMomentum = value;}
	inline Vec3f GetAngularMomentum(){return m_vAngularMomentum;}
	inline void SetForce(Vec3f value){m_vForce = value;}
	inline Vec3f GetForce(){return m_vForce;}
	inline void SetTorque(Vec3f value){m_vTorque = value;}
	inline Vec3f GetTorque(){return m_vTorque;}

	inline void SetOrienation(Mat3f value){m_mOrienation = value;}
	inline Mat3f GetOrienation(){return m_mOrienation;}
	inline void SetIBodyInertiaTensor(Mat3f value){m_mIBodyInertiaTensor = value;}
	inline Mat3f GetIBodyInertiaTensor(){return m_mIBodyInertiaTensor;}
	inline void SetIWorldInertiaTensor(Mat3f value){m_mIWorldInertiaTensor = value;}
	inline Mat3f GetIWorldInertiaTensor(){return m_mIWorldInertiaTensor;}
	inline void SetTransform(Mat4f value){m_mTransform = value;}
	inline Mat4f GetTransform(){return m_mTransform;}
	inline void SetITransform(Mat4f value){m_mITransform = value;}
	inline Mat4f GetITransform(){return m_mITransform;}

	inline void SetFrozen(Bool value){m_bFrozen = value;}
	inline Bool GetFrozen(){return m_bFrozen;}
	inline void SetSimulated(Bool value){m_bSimulated = value;}
	inline Bool GetSimulated(){return m_bSimulated;}
	inline void SetImmovable(Bool value){m_bImmovable = value;}
	inline Bool GetImmovable(){return m_bImmovable;}

	void SetMass(Float32 value);
	inline Float32 GetMass(){return m_fMass;}
	inline Float32 GetIMass(){return m_fIMass;}
	inline void SetRestitution(Float32 value){m_fRestitution = value;}
	inline Float32 GetRestitution(){return m_fRestitution;}
	inline void SetFriction(Float32 value){m_fFriction = value;}
	inline Float32 GetFriction(){return m_fFriction;}
	inline void SetFrozenTime(Float32 value){m_fFrozenTime = value;}
	inline Float32 GetFrozenTime(){return m_fFrozenTime;}

	inline void SetFrozenNumObjects(int32 value){m_FrozenNumObjects = value;}
	inline int32 GetFrozenNumObjects(){return m_FrozenNumObjects;}
	inline void SetType(uint32 value){m_iType = value;}
	inline uint32 GetType(){return m_iType;}

	inline void SetSizes(Vec4f values){m_vSizes = values;}
	inline Vec4f GetSizes(){return m_vSizes;}
	inline void SetMaxContactsNum(uint32 value){m_iMaxContactsNum = value;}
	inline uint32 GetMaxContactsNum(uint32 value){return m_iMaxContactsNum;}
	inline void SetContactsNum(uint32 value){m_iContactsCount = value;}
	inline uint32 GetContactsNum(){return m_iContactsCount;}
	inline void SetCollidedRigidBodysNum(uint32 value){m_iCollidedRigidBodysCount = value;}
	inline uint32 GetCollidedRigidBodysNum(){return m_iCollidedRigidBodysCount;}
	Contact GetCountact(uint32 id){return m_pContacts[id];}
	CCollision *GetCollision(){return m_pCollision;}

private:

	uint32 m_iMaxContactsNum;
	uint32 m_iContactsCount;
	uint32 m_iCollidedRigidBodysCount;
	Contact *m_pContacts;
	CCollision *m_pCollision;

	Vec4f m_vSizes;

	Vec3f m_vPosition;
	Vec3f m_vVelocity;
	Vec3f m_vAngularVelocity;
	Vec3f m_vAngularMomentum;
	Vec3f m_vForce;
	Vec3f m_vTorque;
	
	Mat3f m_mOrienation;
	Mat3f m_mIBodyInertiaTensor;
	Mat3f m_mIWorldInertiaTensor;
	Mat4f m_mTransform;
	Mat4f m_mITransform;
	
	int32 m_FrozenNumObjects;
	Float32 m_fMass;
	Float32 m_fIMass;
	Float32 m_fRestitution;
	Float32 m_fFriction;
	Float32 m_fFrozenTime;
	
	Bool m_bFrozen;
	Bool m_bSimulated;
	Bool m_bImmovable;

	uint32 m_iType;
};

class CPhysic
{
public:
	CPhysic(void);
	~CPhysic(void);
	void SeparateRigidBody(CRigidBody *a,CRigidBody *b);
	void Update(Float32 step);
	uint32 AddRigidBody(CRigidBody::RIGID_BODY_TYPE type,Float32 *sizes,uint32 contacts_num = MAX_CONTACTS);
	void DrawContacts(CRender *render);

	inline void SetTime(Float32 value){m_fTime = value;}
	inline Float32 GetTime(){return m_fTime;}
	inline void SetTimeStep(Float32 value){m_fTimeStep = value;}
	inline Float32 GetTimeStep(){return m_fTimeStep;}
	inline void SetGravitation(Float32 value){m_fGravitation = value;}
	inline Float32 GetGravitation(){return m_fGravitation;}
	inline void SetVelocityMax(Float32 value){m_fVelocityMax = value;}
	inline Float32 GetVelocityMax(){return m_fVelocityMax;}
	inline void SetVelocityThreshold(Float32 value){m_fVelocityThreshold = value;}
	inline Float32 GetVelocityThreshold(){return m_fVelocityThreshold;}
	inline void SetAngularVelocityThreshold(Float32 value){m_fAngularVelocityThreshold = value;}
	inline Float32 GetAngularVelocityThreshold(){return m_fAngularVelocityThreshold;}
	inline void SetTimeToFrost(Float32 value){m_fTimeToFrost = value;}
	inline Float32 GetTimeToFrost(){return m_fTimeToFrost;}
	inline void SetPenetrationSpeed(Float32 value){m_fPenetrationSpeed = value;}
	inline Float32 GetPenetrationSpeed(){return m_fPenetrationSpeed;}

	inline void SetNumFirstIterations(uint32 value){m_iNumFirstIterations = value;}
	inline uint32 GetNumFirstIterations(){return m_iNumFirstIterations;}
	inline void SetNumSecondIterations(uint32 value){m_iNumSecondIterations = value;}
	inline uint32 GetNumSecondIterations(){return m_iNumSecondIterations;}

	inline CRigidBody *GetRigidBody(uint32 id){return m_pRigidBodys[id];}
	inline void SetRigidBodysCount(uint32 value){m_iRigidBodysCount = value;}
	inline uint32 GetRigidBodysCount(){return m_iRigidBodysCount;}
private:
	Float32 m_fTime;
	Float32 m_fTimeStep;
	Float32 m_fGravitation;
	Float32 m_fVelocityMax;
	Float32 m_fVelocityThreshold;
	Float32 m_fAngularVelocityThreshold;
	Float32 m_fTimeToFrost;
	Float32 m_fPenetrationSpeed;
	uint32 m_iNumFirstIterations;
	uint32 m_iNumSecondIterations;

	uint32 m_iRigidBodysCount;
	CRigidBody **m_pRigidBodys;
};

