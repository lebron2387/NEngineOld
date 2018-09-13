
#include "Shader.h"

CShader::CShader(void)
{
}

CShader::~CShader(void)
{
}

uint32 CShader::CreateShaderFromFile(Str filename,Str vertex_shader_profile,Str fragment_shader_profile,CRender *render)
{
	Str v_src,f_src;
	FILE *file = fopen(filename,"rb");
	if(file == NULL) 
	{
		return 0;
	}
		
	fseek(file,0,SEEK_END);
	int32 size = ftell(file);
	Char *data = new char[size + 1];
	data[size] = '\0';
	fseek(file,0,SEEK_SET);
	fread(data,1,size,file);
	fclose(file);
		
	int32 vshader_end_iterator = 0;
	int32 shader_format_separator_pos = 0;
	// skip comments
	char *s = data;
	char *d = data;
	while(*s) 
	{
		if(*s == '/' && *(s + 1) == '/') 
		{
			while(*s && *s != '\n') s++;
			while(*s && *s == '\n') s++;
			*d++ = '\n';
		}
		else if(*s == '/' && *(s + 1) == '*') 
		{
			while(*s && (*s != '*' || *(s + 1) != '/')) s++;
			s += 2;
			while(*s && *s == '\n') s++;
			*d++ = '\n';
		}
		else *d++ = *s++;
	}
	*d = '\0';
	
	// find shaders
	s = data;
	while(*s) 
	{
		if(*s == '<') 
		{
			char *name = s;
			while(*s)
			{
				if(strchr("> \t\n\r",*s))break;
				s++;
			}
			if(*s == '>')
			{		// it`s shader
				*name++ = '\0';
				*s++ = '\0';
				
				while(*s && strchr(" \t\n\r",*s)) s++;

				shader_format_separator_pos = Str(name).Find(Str("="));

				if(!strcmp(Str(name).SubStr(0,shader_format_separator_pos).GetData(),"vertex"))
				{
					v_src = Str(s);
					if(shader_format_separator_pos > 0)vertex_shader_profile = Str(name).SubStr(shader_format_separator_pos+1);
				}
				if(!strcmp(Str(name).SubStr(0,shader_format_separator_pos).GetData(),"fragment"))
				{
					f_src = Str(s);
					if(shader_format_separator_pos > 0)fragment_shader_profile = Str(name).SubStr(shader_format_separator_pos+1);
					v_src = v_src.SubStr(0,vshader_end_iterator);
				}
			}
		}
		s++;
		vshader_end_iterator++;
	}
	SAFE_DELETE_ARRAY(data);

	return render->CreateShader(v_src,vertex_shader_profile,f_src,fragment_shader_profile);
}
