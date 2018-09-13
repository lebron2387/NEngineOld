#pragma once
//***************************************************************************

//Filename	camera.h

//Copyright (C) 2005-2006 Beletskiy Nikita

//***************************************************************************

//***************************************************************************

#include "Core\Engine.h"

//***************************************************************************


//***************************************************************************

class CCamera 
{
public:

	CCamera();
	~CCamera();


	void PositionCamera(Vec3f position,Vec3f view,Vec3f up);
	void SetViewByMouse(float sensivity);
	void StrafeCamera(float speed);
	void MoveCamera(float speed);
	void CheckForMovement(float speed);

	void Look();

	inline void SetPosition(Vec3f position) {Position = position;}
	inline Vec3f GetPosition() {return Position;}

	inline void SetView(Vec3f view) {View = view;}
	inline Vec3f GetView() {return View;}

	inline void SetUp(Vec3f up) {Up = up;}
	inline Vec3f GetUp() {return Up;}


	Vec3f Position;
	Vec3f View;
	Vec3f Up;
};
