
#include "Joint.h"
#include "RigidBody.h"

CJointDistance::CJointDistance(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 max_distance)
{	
	m_vRigidBody0Position = body0_pos;
	m_vRigidBody1Position = body1_pos;
	m_pRigidBody0 = body0;
	m_pRigidBody1 = body1;
	m_fMaxDistance = max_distance;
}

void CJointDistance::PreStep(Float32 dt)
{
	m_vRelative0 = m_pRigidBody0->GetRotation() * m_vRigidBody0Position;
	m_vRelative1 = m_pRigidBody1->GetRotation() * m_vRigidBody1Position;

	Vec3f world_pos0 = m_pRigidBody0->GetPosition() + m_vRelative0;
	Vec3f world_pos1 = m_pRigidBody1->GetPosition() + m_vRelative1;
	m_vWorldPosition = (world_pos0 + world_pos1) * 0.5f;

	// current location of point 0 relative to point 1
	m_vCurrentRelativePosition = world_pos0 - world_pos1;
}

void CJointDistance::ApplyImpulse(Float32 dt)
{
	Vec3f current_vel0 = m_pRigidBody0->GetVelocity() + Vec3f(m_pRigidBody0->GetAngularVelocity()^ m_vRelative0);
	Vec3f current_vel1 = m_pRigidBody1->GetVelocity() + Vec3f(m_pRigidBody1->GetAngularVelocity()^ m_vRelative1);

	// predict a new location
	Vec3f pred_rel_pos0 = m_vCurrentRelativePosition + (current_vel0 - current_vel1) * dt;

	// if the new position is out of range then clamp it
	Vec3f clamped_rel_pos0 = pred_rel_pos0;
	Float32 clamped_rel_pos0_mag = clamped_rel_pos0.length();
	if (clamped_rel_pos0_mag <= 0.0f)return;
	if (clamped_rel_pos0_mag > m_fMaxDistance)
	{
		clamped_rel_pos0 *= m_fMaxDistance / clamped_rel_pos0_mag;
	}

	// now claculate desired vel based on the current pos, new/clamped pos and dt
	Vec3f desired_rel_vel0 = (clamped_rel_pos0 - m_vCurrentRelativePosition) / dt;

	// Vr is -ve the total velocity change
	Vec3f Vr = (current_vel0 - current_vel1) - desired_rel_vel0;

	Float32 normal_vel = Vr.length();

	if (normal_vel > m_fMaxVelocityMag)
	{
		Vr *= (m_fMaxVelocityMag / normal_vel);
		normal_vel = m_fMaxVelocityMag;
	}
	else if (normal_vel <= 0.001f)
	{
		return;
	}

	Vec3f N = Vr / normal_vel;

	Float32 numerator = -normal_vel;
	Float32 denominator = m_pRigidBody0->GetInvMass() + m_pRigidBody1->GetInvMass() + 
	(N * Vec3f(m_pRigidBody0->GetIWorldInertiaTensor() * (Vec3f(m_vRelative0 ^ N)) ^ m_vRelative0)) + 
	(N * Vec3f(m_pRigidBody1->GetIWorldInertiaTensor() * (Vec3f(m_vRelative1 ^ N)) ^ m_vRelative1));

	if (denominator < 0.0001f)return;

	Float32 normal_impulse = numerator / denominator;

	m_pRigidBody0->AddImpulse(N * normal_impulse, m_vWorldPosition);
	m_pRigidBody1->AddImpulse(N * -normal_impulse, m_vWorldPosition);
}

void CJointDistance::Draw(CRender *render)
{
	render->DrawPoint(m_pRigidBody0->GetTransform()*m_vRigidBody0Position,10);
	render->DrawPoint(m_pRigidBody1->GetTransform()*m_vRigidBody1Position,10);
	render->DrawLine(m_pRigidBody0->GetTransform()*m_vRigidBody0Position,m_pRigidBody1->GetTransform()*m_vRigidBody1Position);
}

CJointBall::CJointBall(CRigidBody * body0, Vec3f body0_pos,CRigidBody * body1, Vec3f body1_pos,Float32 allowed_distance,Float32 timescale)
{
	m_vRigidBody0Position = body0_pos;
	m_vRigidBody1Position = body1_pos;
	m_pRigidBody0 = body0;
	m_pRigidBody1 = body1;
	m_fAllowedDistance = allowed_distance;
	m_fTimescale = timescale;
}

void CJointBall::PreStep(Float32 dt)
{
	m_vRelative0 = m_pRigidBody0->GetRotation() * m_vRigidBody0Position;
	m_vRelative1 = m_pRigidBody1->GetRotation() * m_vRigidBody1Position;
	Vec3f world_pos0 = m_pRigidBody0->GetPosition() + m_vRelative0;
	Vec3f world_pos1 = m_pRigidBody1->GetPosition() + m_vRelative1;
	m_vWorldPosition = (world_pos0 + world_pos1) * 0.5f;

	// add a "correction" based on the deviation of point 0
	Vec3f deviation = world_pos0 - world_pos1;
	Float32 deviation_amount = deviation.length();
	Float32 timescale = m_fTimescale > 0 ? m_fTimescale : -m_fTimescale * dt;
	if (timescale < 0.00001f) timescale = dt;

	if (deviation_amount > m_fAllowedDistance)
	{
		Vec3f deviation_dir = deviation / deviation_amount;
		m_vVelocityRestoringExtra = deviation_dir * ((deviation_amount - m_fAllowedDistance) / timescale);
	}
	else
	{
		m_vVelocityRestoringExtra = Vec3f(0.0f,0.0f,0.0f);
	}
}

void CJointBall::ApplyImpulse(Float32 dt)
{
	Vec3f current_vel0 = m_pRigidBody0->GetVelocity() + Vec3f(m_pRigidBody0->GetAngularVelocity()^ m_vRelative0);
	Vec3f current_vel1 = m_pRigidBody1->GetVelocity() + Vec3f(m_pRigidBody1->GetAngularVelocity()^ m_vRelative1);

	// add a "correction" based on the deviation of point 0
	Vec3f Vr = m_vVelocityRestoringExtra + current_vel0 - current_vel1;

	Float32 normal_vel_sqr = Vr.length2();
	if (normal_vel_sqr <= 0.0001f)return;
	Float32 normal_vel = math_sqrt(normal_vel_sqr);

	// limit things
	if (normal_vel > m_fMaxVelocityMag)
	{
		Vr *= m_fMaxVelocityMag / normal_vel;
		normal_vel = m_fMaxVelocityMag;
	}

	Vec3f N = Vr / normal_vel;

	Float32 numerator = -normal_vel;
	Float32 denominator = m_pRigidBody0->GetInvMass() + m_pRigidBody1->GetInvMass() + 
	(N * Vec3f(m_pRigidBody0->GetIWorldInertiaTensor() * (Vec3f(m_vRelative0 ^ N)) ^ m_vRelative0)) + 
	(N * Vec3f(m_pRigidBody1->GetIWorldInertiaTensor() * (Vec3f(m_vRelative1 ^ N)) ^ m_vRelative1));

	if (denominator < 0.0001f)return;

	Vec3f normal_impulse = N * (numerator / denominator);

	// Rather than getting the body to apply the impulses, calculate the
	// linear and angular components ourselves to avoid recalculating
	// pos - m_position

	//  m_body0->apply_world_impulse(normal_impulse * N, m_world_pos);
	//  m_body1->apply_world_impulse(-normal_impulse * N, m_world_pos);

	if(m_pRigidBody0->GetMass() != FLT_MAX)m_pRigidBody0->AddImpulse(normal_impulse);
	if(m_pRigidBody0->GetMass() != FLT_MAX)m_pRigidBody0->AddAngImpulse(Vec3f(m_vRelative0^ normal_impulse));
	if(m_pRigidBody1->GetMass() != FLT_MAX)m_pRigidBody1->AddImpulse(-normal_impulse);
	if(m_pRigidBody1->GetMass() != FLT_MAX)m_pRigidBody1->AddAngImpulse(Vec3f(m_vRelative1^ -normal_impulse));
}

void CJointBall::Draw(CRender *render)
{
	render->DrawPoint(m_pRigidBody0->GetTransform()*m_vRigidBody0Position,10);
	render->DrawPoint(m_pRigidBody1->GetTransform()*m_vRigidBody1Position,10);
}

CJointHinge::CJointHinge(CRigidBody *body0, CRigidBody *body1,Vec3f hinge_axis,Vec3f hinge_pos_rel0,Float32 hinge_half_width,Float32 sideways_slack,Float32 hinge_fwd_angle,Float32 hinge_bck_angle,Float32 damping)
{
	m_pRigidBody0 = body0;
	m_pRigidBody1 = body1;
	m_vHingeAxis = hinge_axis;
	m_vHingePositionRelative = hinge_pos_rel0;
	m_fDamping = damping;
	m_bUsingLimit = false;

	m_vHingeAxis.normalize();

	Vec3f hinge_pos_rel1 = m_pRigidBody0->GetPosition() + 
	hinge_pos_rel0 - m_pRigidBody1->GetPosition();

	// generate the two positions relative to each body
	Vec3f rel_pos0a = hinge_pos_rel0 + Vec3f(hinge_axis * hinge_half_width);
	Vec3f rel_pos0b = hinge_pos_rel0 - Vec3f(hinge_axis * hinge_half_width);

	Vec3f rel_pos1a = hinge_pos_rel1 + Vec3f(hinge_axis * hinge_half_width);
	Vec3f rel_pos1b = hinge_pos_rel1 - Vec3f(hinge_axis * hinge_half_width);

	Float32 hinge_allowed_distance = 0.005f;
	Float32 hinge_allowed_distance1 = hinge_half_width * sideways_slack;
	Float32 hinge_timescale = -3.0f;

	m_pJointBall = new CJointBall(body0,hinge_pos_rel0,body1,hinge_pos_rel1,hinge_allowed_distance,hinge_timescale);

	m_pJointSidePoint = new CJointDistance*[2];
	m_pJointSidePoint[0] = new CJointDistance(body0,rel_pos0a,body1, rel_pos1a,hinge_allowed_distance1);
	m_pJointSidePoint[1] = new CJointDistance(body0,rel_pos0b,body1,rel_pos1b,hinge_allowed_distance1);

	m_fHingeForwardAngle = hinge_fwd_angle;

	if(hinge_fwd_angle <= m_fMaxHingeAngle)
	{
		// choose a direction that is perpendicular to the hinge
		Vec3f perp_dir(0.0f, 0.0f, 1.0f);
		if ((perp_dir * hinge_axis) > 0.1f)perp_dir = Vec3f(0.0f, 1.0f, 0.0f);
		// now make it perpendicular to the hinge
		Vec3f side_axis = Vec3f(hinge_axis ^ perp_dir);
		perp_dir = Vec3f(side_axis ^ hinge_axis);
		perp_dir.normalize();

		// the length of the "arm"
		// TODO take this as a parameter? what's the effect of changing it?
		Float32 len = 10.0f * hinge_half_width;

		// Choose a position using that dir. this will be the anchor point for
		// body 0. relative to hinge
		Vec3f hinge_rel_anchor_pos0 = perp_dir * len;

		// anchor point for body 1 is chosen to be in the middle of the angle range.
		// relative to hinge
		Float32 angle_to_middle = 0.5f * (hinge_fwd_angle - hinge_bck_angle);
		Mat3f mrotation;
		mrotation.identity();
		mrotation.rotate(-angle_to_middle, hinge_axis);
		Vec3f hinge_rel_anchor_pos1 = mrotation * hinge_rel_anchor_pos0;

		// work out the "string" length
		Float32 hinge_half_angle = 0.5f * (hinge_fwd_angle + hinge_bck_angle);
		Float32 allowed_distance = len * 2 * math_sin_deg(hinge_half_angle * 0.5f);

		Vec3f hinge_pos = body0->GetPosition() + hinge_pos_rel0;
		Vec3f rel_pos0c = hinge_pos + hinge_rel_anchor_pos0 - body0->GetPosition();
		Vec3f rel_pos1c = hinge_pos + hinge_rel_anchor_pos1 - body1->GetPosition();

		m_pJointMaxDistance = new CJointDistance(body0,rel_pos0c,body1,rel_pos1c,allowed_distance);

		m_bUsingLimit = true;
	}
}

CJointHinge::~CJointHinge()
{
	SAFE_DELETE(m_pJointBall);
	SAFE_DELETE(m_pJointSidePoint[0]);
	SAFE_DELETE(m_pJointSidePoint[1]);
	SAFE_DELETE_ARRAY(m_pJointSidePoint);
	SAFE_DELETE(m_pJointMaxDistance);
}

void CJointHinge::PreStep(Float32 dt)
{
	m_pJointBall->PreStep(dt);
	m_pJointSidePoint[0]->PreStep(dt);
	m_pJointSidePoint[1]->PreStep(dt);
	if(m_fHingeForwardAngle <= m_fMaxHingeAngle)
	{
		m_pJointMaxDistance->PreStep(dt);
	}
}

void CJointHinge::ApplyImpulse(Float32 dt)
{
	m_pJointBall->ApplyImpulse(dt);
	m_pJointSidePoint[0]->ApplyImpulse(dt);
	m_pJointSidePoint[1]->ApplyImpulse(dt);
	if(m_fHingeForwardAngle <= m_fMaxHingeAngle)
	{
		m_pJointMaxDistance->ApplyImpulse(dt);
	}
}

void CJointHinge::Draw(CRender *render)
{
	m_pJointBall->Draw(render);
	m_pJointSidePoint[0]->Draw(render);
	m_pJointSidePoint[1]->Draw(render);
	m_pJointMaxDistance->Draw(render);
}