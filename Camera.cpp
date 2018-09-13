//***************************************************************************

//Filename	Camera.cpp

//Copyright (C) 2005-2006 Beletskiy Nikita

//***************************************************************************

#include "camera.h"

//***************************************************************************

CCamera::CCamera()
{
	Position = Vec3f(0,0,0);		
	View = Vec3f(0,0,0);		 
	Up = Vec3f(0,0,0);
};

//***************************************************************************

CCamera::~CCamera()
{};

//***************************************************************************

void CCamera::PositionCamera(Vec3f position,Vec3f view,Vec3f up)
{
	Position = position;
	View = Position + view;
	Up = up;
};

//***************************************************************************

Vec3f MatRotateAroundPoint(Vec3f Point,Vec3f Center,float angle,float x,float y,float z)
{
	Vec3f NewView;
	Vec3f View = Point - Center;	
	
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	NewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* View.x;
	NewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* View.y;
	NewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* View.z;

	NewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* View.x;
	NewView.y += (cosTheta + (1 - cosTheta) * y * y)		* View.y;
	NewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* View.z;

	NewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* View.x;
	NewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* View.y;
	NewView.z += (cosTheta + (1 - cosTheta) * z * z)		* View.z;

	Point = Center + NewView;

	return  Point;
};

//***************************************************************************

void CCamera::SetViewByMouse(float sensivity)
{
	POINT mousePos;
	
	int middleX = 200;				
	int middleY = 200;
	
	static float currentRotX = 0.0f;

	GetCursorPos(&mousePos);	
	
	if(mousePos.x == middleX &&
	   mousePos.y == middleY)return;

    float angleY = 0.0f;							
	float angleZ = 0.0f;

	SetCursorPos(middleX, middleY);
	
	angleY = (float)(((middleX - mousePos.x) ) / 1000.0f) * sensivity;		
	angleZ = (float)(((middleY - mousePos.y) ) / 1000.0f) * sensivity;

	currentRotX -= angleZ;  

	if(currentRotX > 1.5f)
		currentRotX = 1.5f;
	else if(currentRotX < -1.5f)
		currentRotX = -1.5f;
	else
	{
		Vec3f Axis = Vec3f(View - Position) ^ Up;
		Axis.normalize();
		View = MatRotateAroundPoint(View,Position,angleZ, Axis.x, Axis.y, Axis.z);
		View = MatRotateAroundPoint(View,Position,angleY, 0, 1, 0);
	}
};

//***************************************************************************


//***************************************************************************

void CCamera::StrafeCamera(float speed)
{
	Vec3f Strafe = Vec3f(Vec3f(View - Position) ^ Up);
	Strafe.normalize();

	Position.x += Strafe.x * speed;
	Position.z += Strafe.z * speed;

	View.x += Strafe.x * speed;
	View.z += Strafe.z * speed;
};

//***************************************************************************

void CCamera::MoveCamera(float speed)
{
	Vec3f vector = Vec3f(View - Position);
	vector.normalize();

	Position.x += vector.x * speed;
	Position.z += vector.z * speed;
	Position.y += vector.y * speed;

	View.x += vector.x * speed;
	View.z += vector.z * speed;
	View.y += vector.y * speed;
};

//***************************************************************************

void CCamera::CheckForMovement(float speed)
{
	if(GetKeyState('W')&0x80)MoveCamera(speed);
	if(GetKeyState('S')&0x80)MoveCamera(-speed);
	if(GetKeyState('A')&0x80)StrafeCamera(-speed);
	if(GetKeyState('D')&0x80)StrafeCamera(speed);
};

//***************************************************************************

void CCamera::Look()
{
};

//***************************************************************************
