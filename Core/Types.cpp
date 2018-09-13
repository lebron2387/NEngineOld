#include "Types.h"

Str::Str()
{
	length = 0;
	capacity = 0;
	data = NULL;
}

Str::Str(const char *s)
{
	length = 0;
	capacity = 0;
	data = NULL;
	Append(s);
}

Str::Str(const Str &s)
{
	length = 0;
	capacity = 0;
	data = NULL;
	Append(s);
}

Str::Str(char c)
{
	length = 0;
	capacity = 0;
	data = NULL;
	Printf("%c",c);
}

Str::Str(int32 i)
{
	length = 0;
	capacity = 0;
	data = NULL;
	Printf("%d",i);
}

Str::Str(Float32 f)
{
	length = 0;
	capacity = 0;
	data = NULL;
	Printf("%g",f);
}

Str::~Str() 
{
	delete [] data;
}

/*
 */

int32 Str::operator==(const char *s) const 
{
	if(data == NULL) return 0;
	return !strcmp(data,s);
}

int32 Str::operator==(const Str &s) const 
{
	if(data == NULL) return 0;
	return !strcmp(data,s.data);
}

int32 Str::operator!=(const char *s) const 
{
	return !(*this == s);
}

int32 Str::operator!=(const Str &s) const 
{
	return !(*this == s);
}

/*
 */
int32 Str::operator<(const char *s) const 
{
	return strcmp(data,s) < 0;
}

int32 Str::operator<(const Str &s) const 
{
	return strcmp(data,s.data) < 0;
}

int32 Str::operator>(const char *s) const 
{
	return strcmp(data,s) > 0;
}

int32 Str::operator>(const Str &s) const 
{
	return strcmp(data,s.data) > 0;
}

/*
 */
Str &Str::operator=(const char *s) 
{
	length = 0;
	Append(s);
	return *this;
}

Str &Str::operator=(const Str &s) 
{
	length = 0;
	Append(s);
	return *this;
}

/*
 */
Str Str::operator+(char c) const 
{
	Str ret = *this;
	ret.Append(c);
	return ret;
}

Str Str::operator+(const char *s) const 
{
	Str ret = *this;
	ret.Append(s);
	return ret;
}

Str Str::operator+(const Str &s) const 
{
	Str ret = *this;
	ret.Append(s);
	return ret;
}

Str &Str::operator+=(char c) 
{
	Append(c);
	return *this;
}

Str &Str::operator+=(const char *s) 
{
	Append(s);
	return *this;
}

Str &Str::operator+=(const Str &s) 
{
	Append(s);
	return *this;
}

/*
 */
void Str::Clear() 
{
	length = 0;
	if(data) data[length] = '\0';
}

/*
 */
void Str::Append(char c) 
{
	Append(&c,1);
}

void Str::Append(const char *s) 
{
	if(s == NULL) return;
	Append(s,(int)strlen(s));
}

void Str::Append(const Str &s) 
{
	Append(s.data,s.length);
}

void Str::Append(const char *s,uint32 s_length) 
{
	uint32 new_length = length + s_length;
	if(data == NULL) 
	{
		capacity = new_length * 2 + 1;
		data = new char[capacity];
		memcpy(data,s,s_length);
	}
	else if(new_length >= capacity) 
	{
		capacity = new_length * 2 + 1;
		char *new_data = new char[capacity];
		memcpy(new_data,data,length);
		memcpy(new_data + length,s,s_length);
		delete [] data;
		data = new_data;
	}
	else 
	{
		memcpy(data + length,s,s_length);
	}
	length = new_length;
	data[length] = '\0';
}

/*
 */
void Str::Printf(const char *format,...) 
{
	char buf[4096];
	va_list	va;								
	va_start(va,format);
	vsprintf(buf, format, va);
	va_end(va);
	Append(buf);
}

int32 Str::Find(Str str,uint32 start)
{
	if(start < Size())
	{
		for(uint32 i = start;i<Size();i++)
		{
			if(SubStr(i,int32(i+str.Size())) == str)
			{
				return i;
			}
		}
	}
	return -1;
}

Str Str::GetExpandPart(Str delimiter,uint32 ind)
{
	uint32 count = 0;
	int32 f = 0;
	int32 f_start = 0;
	while(1)
	{
		f = Find(delimiter,f_start);
		if(f < 0)
		{
			break;
		}
		else
		{
			if(ind == 0)
			{
				return SubStr(0,f);
			}
			if(count == ind)
			{
				return SubStr(f_start+1,f);
			}
			count++;
			f_start = f+1;
		}
	}
	if(ind == count)
	{
		return SubStr(f_start == 0 ? 0 : f_start+1);
	}
	return Str("");
}

uint32 Str::GetExpandCount(Str delimiter)
{
	uint32 count = 0;
	int32 f = 0;
	int32 f_start = 0;
	while(1)
	{
		f = Find(delimiter,f_start);
		if(f < 0)
		{
			break;
		}
		else
		{
			count++;
			f_start = f+1;
		}
	}

	count++;
	return count;
}