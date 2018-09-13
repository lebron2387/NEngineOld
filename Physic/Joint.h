#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

class CRigidBody;

class CJoint
{
public:
	CJoint()
	{
		m_fMaxVelocityMag = 30.0f;
		m_fMaxHingeAngle = 180.0f;
	}
	~CJoint(){}
	virtual void PreStep(Float32 dt){}
	virtual void ApplyImpulse(Float32 dt){}
	virtual void Draw(CRender *render){}

	__forceinline void SetMaxVelocityMag(Float32 value){m_fMaxVelocityMag = value;}
	__forceinline Float32 GetMaxVelocityMag(){return m_fMaxVelocityMag;}
	__forceinline void SetMaxHingeAngle(Float32 value){m_fMaxHingeAngle = value;}
	__forceinline Float32 GetMaxHingeAngle(){return m_fMaxHingeAngle;}

	
	__forceinline void SetRigidBody0(CRigidBody *body){m_pRigidBody0 = body;}
	__forceinline CRigidBody *GetRigidBody0(){return m_pRigidBody0;}
	__forceinline void SetRigidBody1(CRigidBody *body){m_pRigidBody1 = body;}
	__forceinline CRigidBody *GetRigidBody1(){return m_pRigidBody1;}

	__forceinline void SetRigidBody0Position(Vec3f value){m_vRigidBody0Position = value;}
	__forceinline Vec3f GetRigidBody0Position(){return m_vRigidBody0Position;}
	__forceinline void SetRigidBody1Position(Vec3f value){m_vRigidBody1Position = value;}
	__forceinline Vec3f GetRigidBody1Position(){return m_vRigidBody1Position;}

	__forceinline void SetRelative0(Vec3f value){m_vRelative0 = value;}
	__forceinline Vec3f GetRelative0(){return m_vRelative0;}
	__forceinline void SetRelative1(Vec3f value){m_vRelative1 = value;}
	__forceinline Vec3f GetRelative1(){return m_vRelative1;}

	__forceinline void SetWorldPosition(Vec3f value){m_vWorldPosition = value;}
	__forceinline Vec3f GetWorldPosition(){return m_vWorldPosition;}

protected:
	Float32 m_fMaxVelocityMag;
	Float32 m_fMaxHingeAngle;

	CRigidBody * m_pRigidBody0;
	CRigidBody * m_pRigidBody1;
	Vec3f m_vRigidBody0Position;
	Vec3f m_vRigidBody1Position;
	// stuff that gets updated
	Vec3f m_vWorldPosition;
	Vec3f m_vRelative0;
	Vec3f m_vRelative1;
};

class CJointDistance : public CJoint
{
public:
	CJointDistance(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 max_distance);
	~CJointDistance(){}
	void PreStep(Float32 dt);
	void ApplyImpulse(Float32 dt);
	void Draw(CRender *render);

	__forceinline void SetMaxDistance(Float32 value){m_fMaxDistance = value;}
	__forceinline Float32 GetMaxDistance(){return m_fMaxDistance;}

	__forceinline void SetCurrentRelativePosition(Vec3f value){m_vCurrentRelativePosition = value;}
	__forceinline Vec3f GetCurrentRelativePosition(){return m_vCurrentRelativePosition;}
private:
	Float32 m_fMaxDistance;
	Vec3f m_vCurrentRelativePosition;
};

class CJointBall : public CJoint
{
public:
	CJointBall(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 allowed_distance,Float32 timescale);
	CJointBall::~CJointBall(){}

	void PreStep(Float32 dt);
	void ApplyImpulse(Float32 dt);
	void Draw(CRender *render);

	__forceinline void SetAllowedDistance(Float32 value){m_fAllowedDistance = value;}
	__forceinline Float32 GetAllowedDistance(){return m_fAllowedDistance;}

	__forceinline void SetTimescale(Float32 value){m_fTimescale = value;}
	__forceinline Float32 GetTimescale(){return m_fTimescale;}

	__forceinline void SetVelocityRestoringExtra(Vec3f value){m_vVelocityRestoringExtra = value;}
	__forceinline Vec3f GetVelocityRestoringExtra(){return m_vVelocityRestoringExtra;}
private:
	Float32 m_fAllowedDistance;
	Float32 m_fTimescale;
	Vec3f m_vVelocityRestoringExtra; // extra vel for restoring deviation
};

class CJointHinge : public CJoint
{
public:
	CJointHinge(CRigidBody *body0, CRigidBody *body1,Vec3f hinge_axis,Vec3f hinge_pos_rel0,Float32 hinge_half_width,Float32 sideways_slack,Float32 hinge_fwd_angle,Float32 hinge_bck_angle,Float32 damping);
	~CJointHinge();

	void PreStep(Float32 dt);
	void ApplyImpulse(Float32 dt);
	void Draw(CRender *render);

	__forceinline void SetDamping(Float32 value){m_fDamping = value;}
	__forceinline Float32 GetDamping(){return m_fDamping;}
	__forceinline void SetHingeForwardAngle(Float32 value){m_fHingeForwardAngle = value;}
	__forceinline Float32 GetHingeForwardAngle(){return m_fHingeForwardAngle;}

	__forceinline void SetUsingLimit(Bool value){m_bUsingLimit = value;}
	__forceinline Bool GetUsingLimit(){return m_bUsingLimit;}

	__forceinline void SetHingeAxis(Vec3f value){m_vHingeAxis = value;}
	__forceinline Vec3f GetHingeAxis(){return m_vHingeAxis;}
	__forceinline void SetHingePositionRelative(Vec3f value){m_vHingePositionRelative = value;}
	__forceinline Vec3f GetHingePositionRelative(){return m_vHingePositionRelative;}
private:
  CJointBall *m_pJointBall;
  CJointDistance **m_pJointSidePoint;
  CJointDistance *m_pJointMaxDistance;

  Vec3f m_vHingeAxis;
  Vec3f m_vHingePositionRelative;
  Float32 m_fDamping;
  Float32 m_fHingeForwardAngle;
  Bool m_bUsingLimit;
};