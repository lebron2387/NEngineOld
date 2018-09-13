#pragma once

#include "../Core/Engine.h"
#include "../Render/Render.h"

class CShader
{
public:
	CShader(void);
	~CShader(void);

	uint32 CreateShaderFromFile(Str filename,Str vertex_shader_profile,Str fragment_shader_profile,CRender *render);
};
