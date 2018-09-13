#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

#include "RigidBody.h"
#include "Joint.h"
#include "Geometry.h"
#include "Collision.h"

class CPhysic
{
public:
	CPhysic();
	~CPhysic();

	uint32 AddRigidBody(RIGID_BODY_TYPE type,Vec4f sizes,Float32 mass);
	uint32 AddJointDistance(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 max_distance);
	uint32 AddJointBall(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 allowed_distance,Float32 timescale = 1);
	uint32 AddJointHinge(CRigidBody *body0, CRigidBody *body1,Vec3f hinge_axis,Vec3f hinge_pos_rel0,Float32 hinge_half_width,Float32 sideways_slack,Float32 hinge_fwd_angle,Float32 hinge_bck_angle,Float32 damping);
	
	void Step(Float32 dt,CRender *render);
	void BroadPhase();
	void NarrowPhase(CRender *render);

	__forceinline CRigidBody *GetRigidBody(uint32 i){return m_pBodies[i];}
	__forceinline uint32 GetRigidBodysCount(){return m_iRigidBodiesCount;}
	__forceinline CJoint *GetJoint(uint32 i){return m_pJoints[i];}
	__forceinline uint32 GetJointsCount(){return m_iJointsCount;}

	__forceinline void SetDumping(Float32 value){m_fDumping = value;}
	__forceinline Float32 GetDumping(){return m_fDumping;}
	__forceinline void SetGravity(Vec3f value){m_vGravity = value;}
	__forceinline Vec3f GetGravity(){return m_vGravity;}
	__forceinline void SetIterations(uint32 value){m_iIterations = value;}
	__forceinline uint32 GetIterations(){return m_iIterations;}
	__forceinline void SetBroadPhasePairs(uint32 value){m_iBroadPhasePairs = value;}
	__forceinline uint32 GetBroadPhasePairs(){return m_iBroadPhasePairs;}
	__forceinline void SetNarrowPhasePairs(uint32 value){m_iNarrowPhasePairs = value;}
	__forceinline uint32 GetNarrowPhasePairs(){return m_iNarrowPhasePairs;}
	__forceinline void SetContactsCount(uint32 value){m_iContactsCount = value;}
	__forceinline uint32 GetContactsCount(){return m_iContactsCount;}

	__forceinline void SetSolverTime(uint32 value){m_iSolverTime = value;}
	__forceinline uint32 GetSolverTime(){return m_iSolverTime;}
	__forceinline void SetBroadPhaseTime(uint32 value){m_iBroadPhaseTime = value;}
	__forceinline uint32 GetBroadPhaseTime(){return m_iBroadPhaseTime;}
	__forceinline void SetNarrowPhaseTime(uint32 value){m_iNarrowPhaseTime = value;}
	__forceinline uint32 GetNarrowPhaseTime(){return m_iNarrowPhaseTime;}
	__forceinline void SetTotalTime(uint32 value){m_iTotalTime = value;}
	__forceinline uint32 GetTotalTime(){return m_iTotalTime;}

private:
	CCollision *m_pCollision;
	CRigidBody **m_pBodies;
	CJoint **m_pJoints;
	uint32 m_iPairsCount;
	uint32 m_iJointsCount;
	uint32 m_iRigidBodiesCount;
	
	Float32 m_fDumping;
	Vec3f m_vGravity;
	uint32 m_iIterations;
	uint32 m_iBroadPhasePairs;
	uint32 m_iNarrowPhasePairs;
	uint32 m_iContactsCount;

	uint32 m_iSolverTime;
	uint32 m_iBroadPhaseTime;
	uint32 m_iNarrowPhaseTime;
	uint32 m_iTotalTime;
};