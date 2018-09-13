#pragma once

#include "Math.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "incdef.h"

struct Vec2i;
struct Vec2d;
struct Vec2f;
struct Vec3i;
struct Vec3d;
struct Vec3f;
struct Vec4i;
struct Vec4d;
struct Vec4f;
struct Mat3i;
struct Mat3d;
struct Mat3f;
struct Mat4i;
struct Mat4d;
struct Mat4f;
struct Quati;
struct Quatd;
struct Quatf;

class Array1i;
class Array2i;
class Array3i;
class Array1f;
class Array2f;
class Array3f;

class Array1i
{
public:
	Array1i()
	{
		m_iFirstDimensionSize = 0;
	}
	Array1i(int32 m)
	{
		m_iFirstDimensionSize = m;
		data = new int32[m];
	}
	~Array1i(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		data = new int32[m];
	}
	__forceinline int32 &operator[](int32 index){return data[index];}
	__forceinline int32 operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	int32 *data;
};

class Array1f
{
public:
	Array1f()
	{
		m_iFirstDimensionSize = 0;
	}
	Array1f(int32 m)
	{
		m_iFirstDimensionSize = m;
		data = new Float32[m];
	}
	~Array1f(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		data = new Float32[m];
	}
	__forceinline Float32 &operator[](int32 index){return data[index];}
	__forceinline Float32 operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	Float32 *data;
};

class Array2i
{
public:
	Array2i()
	{
		m_iFirstDimensionSize = 0;
		m_iSecondDimensionSize = 0;
	}
	Array2i(int32 m,int32 n)
	{
		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		data = new Array1i[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n);
	}
	~Array2i(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m,int32 n)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		data = new Array1i[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n);
	}
	__forceinline Array1i &operator[](int32 index){return data[index];}
	__forceinline Array1i operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
	__forceinline int32 GetSizeSecondDimension(){return m_iSecondDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	int32 m_iSecondDimensionSize;
	Array1i *data;
};

class Array2f
{
public:
	Array2f()
	{
		m_iFirstDimensionSize = 0;
		m_iSecondDimensionSize = 0;
	}
	Array2f(int32 m,int32 n)
	{
		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		data = new Array1f[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n);
	}
	~Array2f(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m,int32 n)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		data = new Array1f[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n);
	}
	__forceinline Array1f &operator[](int32 index){return data[index];}
	__forceinline Array1f operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
	__forceinline int32 GetSizeSecondDimension(){return m_iSecondDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	int32 m_iSecondDimensionSize;
	Array1f *data;
};

class Array3i
{
private:
	Array3i()
	{
		m_iFirstDimensionSize = 0;
		m_iSecondDimensionSize = 0;
		m_iThirdDimensionSize = 0;
	}
public:
	Array3i(int32 m,int32 n,int32 k)
	{
		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		m_iThirdDimensionSize = k;
		data = new Array2i[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n,k);
	}
	~Array3i(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m,int32 n,int32 k)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		m_iThirdDimensionSize = k;
		data = new Array2i[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n,k);
	}
	__forceinline Array2i &operator[](int32 index){return data[index];}
	__forceinline Array2i operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
	__forceinline int32 GetSizeSecondDimension(){return m_iSecondDimensionSize;}
	__forceinline int32 GetSizeThirdDimension(){return m_iThirdDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	int32 m_iSecondDimensionSize;
	int32 m_iThirdDimensionSize;
	Array2i *data;
};

class Array3f
{
public:
	Array3f()
	{
		m_iFirstDimensionSize = 0;
		m_iSecondDimensionSize = 0;
		m_iThirdDimensionSize = 0;
	}
	Array3f(int32 m,int32 n,int32 k)
	{
		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		m_iThirdDimensionSize = k;
		data = new Array2f[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n,k);
	}
	~Array3f(){SAFE_DELETE_ARRAY(data);}
	__forceinline void ReInit(int32 m,int32 n,int32 k)
	{
		if(m_iFirstDimensionSize != 0)SAFE_DELETE_ARRAY(data);

		m_iFirstDimensionSize = m;
		m_iSecondDimensionSize = n;
		m_iThirdDimensionSize = k;
		data = new Array2f[m];
		for(int32 i=0;i<m;i++)data[i].ReInit(n,k);
	}
	__forceinline Array2f &operator[](int32 index){return data[index];}
	__forceinline Array2f operator[] (int32 index) const {return data[index];}
	__forceinline int32 GetSizeFirstDimension(){return m_iFirstDimensionSize;}
	__forceinline int32 GetSizeSecondDimension(){return m_iSecondDimensionSize;}
	__forceinline int32 GetSizeThirdDimension(){return m_iThirdDimensionSize;}
private:
	int32 m_iFirstDimensionSize;
	int32 m_iSecondDimensionSize;
	int32 m_iThirdDimensionSize;
	Array2f *data;
};

class Str
{
public:

	Str();
	~Str();
	Str(const Char *s);
	Str(const Str &s);
	explicit Str(Char c);
	explicit Str(int32 i);
	explicit Str(Float32 f);
	
	
	int32 operator==(const Char *s) const;
	int32 operator==(const Str &s) const;
	int32 operator!=(const Char *s) const;
	int32 operator!=(const Str &s) const;
	int32 operator<(const Char *s) const;
	int32 operator<(const Str &s) const;
	int32 operator>(const Char *s) const;
	int32 operator>(const Str &s) const;

	Str &operator=(const Char *s);
	Str &operator=(const Str &s);
	Str operator+(Char c) const;
	Str operator+(const Char *s) const;
	Str operator+(const Str &s) const;
	Str &operator+=(Char c);
	Str &operator+=(const Char *s);
	Str &operator+=(const Str &s);
	
	__forceinline operator const Char*() { return data; }
	__forceinline operator const Char*() const { return data; }
	__forceinline Char &operator[](int32 index) { return data[index]; }
	__forceinline Char operator[](int32 index) const { return data[index]; }
	__forceinline const Char *GetData() { return data; }
	__forceinline const Char *GetData() const { return data; }
	__forceinline Bool Empty(){return (length == 0)? true : false;}
	
	__forceinline uint32 Size() const { return length; }
	__forceinline Str SubStr(uint32 start,int32 size = -1)
	{
		Str ret;
		for(uint32 i=start;i<(size == -1 ? length : size);i++)
		{
			ret += data[i];
		}
		return ret;
	}
	__forceinline Str InsertStr(Str str,uint32 pos)
	{
		return SubStr(0,pos)+str+SubStr(pos);	
	}
	__forceinline Str RemoveStr(uint32 pos,uint32 size = 0)
	{
		Str r1 = SubStr(0,pos);
		Str r2 = SubStr(pos+size);
		return size == 0 ? SubStr(0,pos) : SubStr(0,pos) + SubStr(pos+size);	
	}

	Str GetExpandPart(Str delimiter,uint32 ind);
	uint32 GetExpandCount(Str delimiter);

	void Clear();
	void Append(Char c);
	void Append(const Char *s);
	void Append(const Str &s);
	void Printf(const Char *format,...);

	
	int32 Find(Str str,uint32 start = 0);
	
private:
	
	void Append(const Char *s,uint32 s_length);
	
	uint32 length;
	uint32 capacity;
public:
	Char *data;
};

class File
{
	public:
		File(){};
		~File(){};

		enum SEEK
		{
			SEEK_START = SEEK_SET,
			SEEK_CURRENT = SEEK_CUR,
			SEEK_EOF = SEEK_END,
		};

		__forceinline Bool Open(Str filename,Str open_mode){m_pFile=fopen(filename,open_mode); if(m_pFile==NULL){return false;}return true;}
		__forceinline uint32 Read(void * ptr,uint32 size,uint32 count){return fread(ptr,size,count,m_pFile);}
		__forceinline uint32 Write(void * ptr,uint32 size,uint32 count){return fwrite(ptr,size,count,m_pFile);}
		__forceinline int32 IsEOF(){return feof(m_pFile);}
		__forceinline int32 Flush(){return fflush(m_pFile);}
		__forceinline int32 GetPosition(uint32 *pos){return fgetpos(m_pFile,(fpos_t*)pos);}
		__forceinline int32 SetPosition(uint32 *pos){return fsetpos(m_pFile,(fpos_t*)pos);}
		__forceinline int32 GetChar(){return fgetc(m_pFile);}
		__forceinline int32 PutChar(Str ch){return fputc((int32)ch.data,m_pFile);}
		__forceinline int32 Seek(uint32 offset,int32 origin){return fseek(m_pFile,offset,origin);}
		__forceinline int32 Scan(const char *format,...)
		{
			va_list	va;								
			va_start(va,format);
			int32 ret = fscanf(m_pFile,format,va);
			va_end(va);
			return ret;
		}
		__forceinline FILE *GetFile(){return m_pFile;}
		__forceinline void Close(){fclose(m_pFile);}
	private:
		FILE *m_pFile;
};

struct Vec2i
{
	__forceinline Vec2i() : x(0), y(0) { }
	__forceinline Vec2i(int32 x,int32 y) : x(x), y(y) { }
	__forceinline Vec2i(const int32 *v) : x(v[0]), y(v[1]) { }
	__forceinline Vec2i(const Vec2i &v) : x(v.x), y(v.y) { }
	
	__forceinline Bool operator==(const Vec2i &v) { return (math_abs(x - v.x) < EPSILON && math_abs(y - v.y) < EPSILON); }
	__forceinline Bool operator!=(const Vec2i &v) { return !(*this == v); }
	
	__forceinline const Vec2i operator*(int32 f) const { return Vec2i(x * f,y * f); }
	__forceinline const Vec2i operator/(int32 f) const { return Vec2i(x / f,y / f); }
	__forceinline const Vec2i operator+(const Vec2i &v) const { return Vec2i(x + v.x,y + v.y); }
	__forceinline const Vec2i operator-() const { return Vec2i(-x,-y); }
	__forceinline const Vec2i operator-(const Vec2i &v) const { return Vec2i(x - v.x,y - v.y); }
	
	__forceinline Vec2i &operator*=(int32 f) { return *this = *this * f; }
	__forceinline Vec2i &operator/=(int32 f) { return *this = *this / f; }
	__forceinline Vec2i &operator+=(const Vec2i &v) { return *this = *this + v; }
	__forceinline Vec2i &operator-=(const Vec2i &v) { return *this = *this - v; }
	
	__forceinline int32 operator*(const Vec2i &v) const { return x * v.x + y * v.y; }
	
	__forceinline operator int32*() { return (int32*)&x; }
	__forceinline operator const int32*() const { return (int32*)&x; }
	
	__forceinline int32 &operator[](int32 i) { return ((int32*)&x)[i]; }
	__forceinline const int32 operator[](int32 i) const { return ((int32*)&x)[i]; }
	
	__forceinline int32 length() const { return (int32)math_sqrt(Float32(x * x + y * y)); }
	__forceinline int32 normalize() 
	{
		int32 inv,length = (int32)math_sqrt(Float32(x * x + y * y));
		inv = 1 / length;
		x *= inv;
		y *= inv;
		return length;
	}
	
	union
	{
		struct
		{
			int32 x,y;
		};
		int32 v[2];
	};
};


struct Vec2f
{
	__forceinline Vec2f() : x(0), y(0) { }
	__forceinline Vec2f(Float32 x,Float32 y) : x(x), y(y) { }
	__forceinline Vec2f(const Float32 *v) : x(v[0]), y(v[1]) { }
	__forceinline Vec2f(const Vec2f &v) : x(v.x), y(v.y) { }
	
	__forceinline Bool operator==(const Vec2f &v) { return (math_fabs(x - v.x) < EPSILON && math_fabs(y - v.y) < EPSILON); }
	__forceinline Bool operator!=(const Vec2f &v) { return !(*this == v); }
	
	__forceinline const Vec2f operator*(Float32 f) const { return Vec2f(x * f,y * f); }
	__forceinline const Vec2f operator/(Float32 f) const { return Vec2f(x / f,y / f); }
	__forceinline const Vec2f operator+(const Vec2f &v) const { return Vec2f(x + v.x,y + v.y); }
	__forceinline const Vec2f operator-() const { return Vec2f(-x,-y); }
	__forceinline const Vec2f operator-(const Vec2f &v) const { return Vec2f(x - v.x,y - v.y); }
	
	__forceinline Vec2f &operator*=(Float32 f) { return *this = *this * f; }
	__forceinline Vec2f &operator/=(Float32 f) { return *this = *this / f; }
	__forceinline Vec2f &operator+=(const Vec2f &v) { return *this = *this + v; }
	__forceinline Vec2f &operator-=(const Vec2f &v) { return *this = *this - v; }
	
	__forceinline Float32 operator*(const Vec2f &v) const { return x * v.x + y * v.y; }
	
	__forceinline operator Float32*() { return (Float32*)&x; }
	__forceinline operator const Float32*() const { return (Float32*)&x; }
	
	__forceinline Float32 &operator[](int32 i) { return ((Float32*)&x)[i]; }
	__forceinline const Float32 operator[](int32 i) const { return ((Float32*)&x)[i]; }
	
	__forceinline Float32 length() const { return (Float32)math_sqrt(x * x + y * y); }
	__forceinline Float32 normalize() 
	{
		Float32 inv,length = (Float32)math_sqrt(x * x + y * y);
		inv = 1.0f / length;
		x *= inv;
		y *= inv;
		return length;
	}
	
	union
	{
		struct
		{
			Float32 x,y;
		};
		Float32 v[2];
	};
};

struct Vec3i 
{	
	__forceinline Vec3i() : x(0), y(0), z(0) { }
	__forceinline Vec3i(int32 x,int32 y,int32 z) : x(x), y(y), z(z) { }
	__forceinline Vec3i(const int32 *v) : x(v[0]), y(v[1]), z(v[2]) { }
	__forceinline Vec3i(const Vec3i &v) : x(v.x), y(v.y), z(v.z) { }
	//__forceinline Vec3i(const Vec4i &v) : x(v.x), y(v.y), z(v.z) { };
	
	__forceinline Bool operator==(const Vec3i &v) { return (math_abs(x - v.x) < EPSILON && math_abs(y - v.y) < EPSILON && math_abs(z - v.z) < EPSILON); }
	__forceinline Bool operator!=(const Vec3i &v) { return !(*this == v); }
	
	__forceinline const Vec3i operator*(int32 f) const { return Vec3i(x * f,y * f,z * f); }
	__forceinline const Vec3i operator/(int32 f) const { return Vec3i(x / f,y / f,z / f); }
	__forceinline const Vec3i operator+(const Vec3i &v) const { return Vec3i(x + v.x,y + v.y,z + v.z); }
	__forceinline const Vec3i operator-() const { return Vec3i(-x,-y,-z); }
	__forceinline const Vec3i operator-(const Vec3i &v) const { return Vec3i(x - v.x,y - v.y,z - v.z); }

	__forceinline const Vec3i operator^(const Vec3i &v) const 
	{ return Vec3i((y*v.z)-(z*v.y),(z*v.x)-(x*v.z),(x*v.y)-(y*v.x));}

	__forceinline Vec3i &operator*=(int32 f) { return *this = *this * f; }
	__forceinline Vec3i &operator/=(int32 f) { return *this = *this / f; }
	__forceinline Vec3i &operator+=(const Vec3i &v) { return *this = *this + v; }
	__forceinline Vec3i &operator-=(const Vec3i &v) { return *this = *this - v; }
	
	__forceinline int32 operator*(const Vec3i &v) const { return x * v.x + y * v.y + z * v.z; }
	
	__forceinline operator int32*() { return (int32*)&x; }
	__forceinline operator const int32*() const { return (int32*)&x; }
	
	__forceinline int32 &operator[](int32 i) { return ((int32*)&x)[i]; }
	__forceinline const int32 operator[](int32 i) const { return ((int32*)&x)[i]; }
	
	__forceinline int32 length() const { return (int32)math_sqrt(Float32(x * x + y * y + z * z)); }
	__forceinline int32 length2() const { return (int32)(x * x + y * y + z * z); }
	/*
	__forceinline Vec3i normalize()
	{
		int32 inv,length = (int32)math_sqrt(Float32(x * x + y * y + z * z));
		inv = 1 / length;
		x *= inv;
		y *= inv;
		z *= inv;
		return Vec3i(x,y,z);
	}
	*/
	union {
		struct {
			int32 x,y,z;
		};
		int32 v[3];
	};
};

struct Vec3f 
{	
	__forceinline Vec3f() : x(0), y(0), z(0) { }
	__forceinline Vec3f(Float32 x,Float32 y,Float32 z) : x(x), y(y), z(z) { }
	__forceinline Vec3f(const Float32 *v) : x(v[0]), y(v[1]), z(v[2]) { }
	__forceinline Vec3f(const Vec3f &v) : x(v.x), y(v.y), z(v.z) { }
	__forceinline Vec3f(const Vec4f &v);
	
	__forceinline Bool operator==(const Vec3f &v) { return (math_fabs(x - v.x) < EPSILON && math_fabs(y - v.y) < EPSILON && math_fabs(z - v.z) < EPSILON); }
	__forceinline Bool operator!=(const Vec3f &v) { return !(*this == v); }
	
	__forceinline const Vec3f operator*(Float32 f) const { return Vec3f(x * f,y * f,z * f); }
	__forceinline const Vec3f operator/(Float32 f) const { return Vec3f(x / f,y / f,z / f); }
	__forceinline const Vec3f operator+(const Vec3f &v) const { return Vec3f(x + v.x,y + v.y,z + v.z); }
	__forceinline const Vec3f operator-() const { return Vec3f(-x,-y,-z); }
	__forceinline const Vec3f operator-(const Vec3f &v) const { return Vec3f(x - v.x,y - v.y,z - v.z); }

	__forceinline const Vec3f operator^(const Vec3f &v) const 
	{ return Vec3f((y*v.z)-(z*v.y),(z*v.x)-(x*v.z),(x*v.y)-(y*v.x));}

	__forceinline Vec3f &operator*=(Float32 f) { return *this = *this * f; }
	__forceinline Vec3f &operator/=(Float32 f) { return *this = *this / f; }
	__forceinline Vec3f &operator+=(const Vec3f &v) { return *this = *this + v; }
	__forceinline Vec3f &operator-=(const Vec3f &v) { return *this = *this - v; }
	
	__forceinline Float32 operator*(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }
	
	__forceinline operator Float32*() { return (Float32*)&x; }
	__forceinline operator const Float32*() const { return (Float32*)&x; }
	
	__forceinline Float32 &operator[](int32 i) { return ((Float32*)&x)[i]; }
	__forceinline const Float32 operator[](int32 i) const { return ((Float32*)&x)[i]; }
	
	__forceinline Float32 length() const { return (Float32)math_sqrt(x * x + y * y + z * z); }
	__forceinline Float32 length2() const { return (Float32)(x * x + y * y + z * z); }

	__forceinline Vec3f tangent()
	{
		Float32 xx = math_fabs(x);
		Float32 yy = math_fabs(y);
		Float32 zz = math_fabs(z);
		Vec3f axis = Vec3f(0,0,0);
		if(xx <= yy && xx<= zz)
		{
			axis = Vec3f(1,0,0);
		}
		else if(zz <= xx && zz<= yy)
		{
			axis = Vec3f(0,0,1);
		}
		else
		{
			axis = Vec3f(0,1,0);
		}
		return axis;
	}
	
	__forceinline Float32 normalize()
	{
		Float32 inv,length = (Float32)math_sqrt(x * x + y * y + z * z);
		if(length < EPSILON) return 0.0;
		inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
		return length;
	}
	
	union {
		struct {
			Float32 x,y,z;
		};
		Float32 v[3];
	};
};

struct Vec4f 
{
	__forceinline Vec4f() : x(0), y(0), z(0), w(1) { }
	__forceinline Vec4f(Float32 x,Float32 y,Float32 z,Float32 w) : x(x), y(y), z(z), w(w) { }
	__forceinline Vec4f(const Float32 *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
	__forceinline Vec4f(const Vec3f &v) : x(v.x), y(v.y), z(v.z), w(1) { }
	__forceinline Vec4f(const Vec3f &v,Float32 w) : x(v.x), y(v.y), z(v.z), w(w) { }
	__forceinline Vec4f(const Vec4f &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	
	__forceinline Bool operator==(const Vec4f &v) { return (math_fabs(x - v.x) < EPSILON && math_fabs(y - v.y) < EPSILON && math_fabs(z - v.z) < EPSILON && math_fabs(w - v.w) < EPSILON); }
	__forceinline Bool operator!=(const Vec4f &v) { return !(*this == v); }
	
	__forceinline const Vec4f operator*(Float32 f) const { return Vec4f(x * f,y * f,z * f,w * f); }
	__forceinline const Vec4f operator/(Float32 f) const { return Vec4f(x / f,y / f,z / f,w / f); }
	__forceinline const Vec4f operator+(const Vec4f &v) const { return Vec4f(x + v.x,y + v.y,z + v.z,w + v.w); }
	__forceinline const Vec4f operator-() const { return Vec4f(-x,-y,-z,-w); }
	__forceinline const Vec4f operator-(const Vec4f &v) const { return Vec4f(x - v.x,y - v.y,z - v.z,z - v.w); }
	
	__forceinline Vec4f &operator*=(Float32 f) { return *this = *this * f; }
	__forceinline Vec4f &operator/=(Float32 f) { return *this = *this / f; }
	__forceinline Vec4f &operator+=(const Vec4f &v) { return *this = *this + v; }
	__forceinline Vec4f &operator-=(const Vec4f &v) { return *this = *this - v; }
	
	__forceinline Float32 operator*(const Vec4f &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	
	__forceinline operator Float32*() { return (Float32*)&x; }
	__forceinline operator const Float32*() const { return (Float32*)&x; }
	
	__forceinline Float32 &operator[](int32 i) { return ((Float32*)&x)[i]; }
	__forceinline const Float32 operator[](int32 i) const { return ((Float32*)&x)[i]; }

	__forceinline Vec4f normalize()
	{
		Float32 inv,length = (Float32)math_sqrt(x * x + y * y + z * z + w * w);
		inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
		return Vec4f(x,y,z,w);
	}
	
	union {
		struct {
			Float32 x,y,z,w;
		};
		Float32 v[4];
	};
};

struct Vec4i 
{
	__forceinline Vec4i() : x(0), y(0), z(0), w(1) { }
	__forceinline Vec4i(int32 x,int32 y,int32 z,int32 w) : x(x), y(y), z(z), w(w) { }
	__forceinline Vec4i(const int32 *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
	__forceinline Vec4i(const Vec3i &v) : x(v.x), y(v.y), z(v.z), w(1) { }
	__forceinline Vec4i(const Vec3i &v,uint32 w) : x(v.x), y(v.y), z(v.z), w(w) { }
	__forceinline Vec4i(const Vec4i &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	
	__forceinline Bool operator==(const Vec4i &v) { return (math_abs(x - v.x) < EPSILON && math_abs(y - v.y) < EPSILON && math_abs(z - v.z) < EPSILON && math_abs(w - v.w) < EPSILON); }
	__forceinline Bool operator!=(const Vec4i &v) { return !(*this == v); }
	
	__forceinline const Vec4i operator*(int32 f) const { return Vec4i(x * f,y * f,z * f,w * f); }
	__forceinline const Vec4i operator/(int32 f) const { return Vec4i(x / f,y / f,z / f,w / f); }
	__forceinline const Vec4i operator+(const Vec4i &v) const { return Vec4i(x + v.x,y + v.y,z + v.z,w + v.w); }
	__forceinline const Vec4i operator-() const { return Vec4i(-x,-y,-z,-w); }
	__forceinline const Vec4i operator-(const Vec4i &v) const { return Vec4i(x - v.x,y - v.y,z - v.z,z - v.w); }
	
	__forceinline Vec4i &operator*=(int32 f) { return *this = *this * f; }
	__forceinline Vec4i &operator/=(int32 f) { return *this = *this / f; }
	__forceinline Vec4i &operator+=(const Vec4i &v) { return *this = *this + v; }
	__forceinline Vec4i &operator-=(const Vec4i &v) { return *this = *this - v; }
	
	__forceinline int32 operator*(const Vec4i &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	
	__forceinline operator int32*() { return (int32*)&x; }
	__forceinline operator const int32*() const { return (int32*)&x; }
	
	__forceinline int32 &operator[](int32 i) { return ((int32*)&x)[i]; }
	__forceinline const int32 operator[](int32 i) const { return ((int32*)&x)[i]; }
/*
	__forceinline Vec4i normalize()
	{
		Float32 inv,length = math_sqrt(x * x + y * y + z * z + w * w);
		inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
		w *= inv;
		return Vec4i(x,y,z,w);
	}
	*/
	union {
		struct {
			int32 x,y,z,w;
		};
		int32 v[4];
	};
};

__forceinline Vec3f::Vec3f(const Vec4f &v) 
{
	x = v.x;
	y = v.y;
	z = v.z;
}

struct Mat3f 
{
	
	Mat3f()
	{
		mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
	}
	Mat3f(const Float32 *m)
	{
		mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
		mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
		mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
	}
	Mat3f(const Mat3f &m) 
	{
		mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
		mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
		mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
	}
	Mat3f(const Mat4f &m);
	
	Vec3f operator*(const Vec3f &v) const
	{
		Vec3f ret;
		ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
		ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
		ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
		return ret;
	}
	Vec4f operator*(const Vec4f &v) const
	{
		Vec4f ret;
		ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
		ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
		ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
		ret[3] = v[3];
		return ret;
	}
	Mat3f operator*(Float32 f) const {
		Mat3f ret;
		ret[0] = mat[0] * f; ret[3] = mat[3] * f; ret[6] = mat[6] * f;
		ret[1] = mat[1] * f; ret[4] = mat[4] * f; ret[7] = mat[7] * f;
		ret[2] = mat[2] * f; ret[5] = mat[5] * f; ret[8] = mat[8] * f;
		return ret;
	}
	Mat3f operator*(const Mat3f &m) const
	{
		Mat3f ret;
		ret[0] = mat[0] * m[0] + mat[3] * m[1] + mat[6] * m[2];
		ret[1] = mat[1] * m[0] + mat[4] * m[1] + mat[7] * m[2];
		ret[2] = mat[2] * m[0] + mat[5] * m[1] + mat[8] * m[2];
		ret[3] = mat[0] * m[3] + mat[3] * m[4] + mat[6] * m[5];
		ret[4] = mat[1] * m[3] + mat[4] * m[4] + mat[7] * m[5];
		ret[5] = mat[2] * m[3] + mat[5] * m[4] + mat[8] * m[5];
		ret[6] = mat[0] * m[6] + mat[3] * m[7] + mat[6] * m[8];
		ret[7] = mat[1] * m[6] + mat[4] * m[7] + mat[7] * m[8];
		ret[8] = mat[2] * m[6] + mat[5] * m[7] + mat[8] * m[8];
		return ret;
	}
	Mat3f operator+(const Mat3f &m) const 
	{
		Mat3f ret;
		ret[0] = mat[0] + m[0]; ret[3] = mat[3] + m[3]; ret[6] = mat[6] + m[6];
		ret[1] = mat[1] + m[1]; ret[4] = mat[4] + m[4]; ret[7] = mat[7] + m[7];
		ret[2] = mat[2] + m[2]; ret[5] = mat[5] + m[5]; ret[8] = mat[8] + m[8];
		return ret;
	}
	Mat3f operator-(const Mat3f &m) const
	{
		Mat3f ret;
		ret[0] = mat[0] - m[0]; ret[3] = mat[3] - m[3]; ret[6] = mat[6] - m[6];
		ret[1] = mat[1] - m[1]; ret[4] = mat[4] - m[4]; ret[7] = mat[7] - m[7];
		ret[2] = mat[2] - m[2]; ret[5] = mat[5] - m[5]; ret[8] = mat[8] - m[8];
		return ret;
	}
	
	Mat3f &operator*=(Float32 f) { return *this = *this * f; }
	Mat3f &operator*=(const Mat3f &m) { return *this = *this * m; }
	Mat3f &operator+=(const Mat3f &m) { return *this = *this + m; }
	Mat3f &operator-=(const Mat3f &m) { return *this = *this - m; }
	
	operator Float32*() { return mat; }
	operator const Float32*() const { return mat; }
	
	Float32 &operator[](int32 i) { return mat[i]; }
	const Float32 operator[](int32 i) const { return mat[i]; }
	
	Mat3f transpose() const 
	{
		Mat3f ret;
		ret[0] = mat[0]; ret[3] = mat[1]; ret[6] = mat[2];
		ret[1] = mat[3]; ret[4] = mat[4]; ret[7] = mat[5];
		ret[2] = mat[6]; ret[5] = mat[7]; ret[8] = mat[8];
		return ret;
	}
	Float32 det() const 
	{
		Float32 det;
		det = mat[0] * mat[4] * mat[8];
		det += mat[3] * mat[7] * mat[2];
		det += mat[6] * mat[1] * mat[5];
		det -= mat[6] * mat[4] * mat[2];
		det -= mat[3] * mat[1] * mat[8];
		det -= mat[0] * mat[7] * mat[5];
		return det;
	}
	Mat3f inverse() const 
	{
		Mat3f ret;
		Float32 idet = 1.0f / det();
		ret[0] =  (mat[4] * mat[8] - mat[7] * mat[5]) * idet;
		ret[1] = -(mat[1] * mat[8] - mat[7] * mat[2]) * idet;
		ret[2] =  (mat[1] * mat[5] - mat[4] * mat[2]) * idet;
		ret[3] = -(mat[3] * mat[8] - mat[6] * mat[5]) * idet;
		ret[4] =  (mat[0] * mat[8] - mat[6] * mat[2]) * idet;
		ret[5] = -(mat[0] * mat[5] - mat[3] * mat[2]) * idet;
		ret[6] =  (mat[3] * mat[7] - mat[6] * mat[4]) * idet;
		ret[7] = -(mat[0] * mat[7] - mat[6] * mat[1]) * idet;
		ret[8] =  (mat[0] * mat[4] - mat[3] * mat[1]) * idet;
		return ret;
	}
	
	void zero() 
	{
		mat[0] = 0.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = 0.0; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 0.0;
	}
	void identity() 
	{
		mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
	}
	void rotate(Float32 angle,const Vec3f &axis) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		Vec3f v = axis;
		v.normalize();
		Float32 xy = v.x * v.y;
		Float32 yz = v.y * v.z;
		Float32 zx = v.z * v.x;
		Float32 xs = v.x * s;
		Float32 ys = v.y * s;
		Float32 zs = v.z * s;
		mat[0] = (1.0f - c) * v.x * v.x + c; mat[3] = (1.0f - c) * xy - zs; mat[6] = (1.0f - c) * zx + ys;
		mat[1] = (1.0f - c) * xy + zs; mat[4] = (1.0f - c) * v.y * v.y + c; mat[7] = mat[6] = (1.0f - c) * yz - xs;
		mat[2] = (1.0f - c) * zx - ys; mat[5] = (1.0f - c) * yz + xs; mat[8] = (1.0f - c) * v.z * v.z + c;
	}
	void rotate(Float32 angle,Float32 x,Float32 y,Float32 z) {rotate(angle,Vec3f(x,y,z));}
	void rotate_x(Float32 angle)
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = c; mat[7] = -s;
		mat[2] = 0.0; mat[5] = s; mat[8] = c;
	}
	void rotate_y(Float32 angle) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = c; mat[3] = 0.0; mat[6] = s;
		mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
		mat[2] = -s; mat[5] = 0.0; mat[8] = c;
	}
	void rotate_z(Float32 angle)
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = c; mat[3] = -s; mat[6] = 0.0;
		mat[1] = s; mat[4] = c; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
	}
	void scale(const Vec3f &v) 
	{
		mat[0] = v.x; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = v.y; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = v.z;
	}
	void scale(Float32 x,Float32 y,Float32 z) {scale(Vec3f(x,y,z));}
	void orthonormalize() 
	{
		Vec3f x(mat[0],mat[1],mat[2]);
		Vec3f y(mat[3],mat[4],mat[5]);
		Vec3f z;
		x.normalize();
		z = x^y;
		z.normalize();
		y = z^x;
		y.normalize();
		mat[0] = x.x; mat[3] = y.x; mat[6] = z.x;
		mat[1] = x.y; mat[4] = y.y; mat[7] = z.y;
		mat[2] = x.z; mat[5] = y.z; mat[8] = z.z;
	}
	
	Float32 mat[9];
};

struct Mat4f 
{	
	Mat4f() 
	{
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	Mat4f(const Mat3f &m)
	{
		mat[0] = m[0]; mat[4] = m[3]; mat[8] = m[6]; mat[12] = 0.0;
		mat[1] = m[1]; mat[5] = m[4]; mat[9] = m[7]; mat[13] = 0.0;
		mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	Mat4f(const Float32 *m) 
	{
		mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
		mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
		mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
		mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
	}
	Mat4f(const Mat4f &m)
	{
		mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
		mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
		mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
		mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
	}
	
	Vec3f operator*(const Vec3f &v) const 
	{
		Vec3f ret;
		ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12];
		ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13];
		ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14];
		return ret;
	}
	Vec4f operator*(const Vec4f &v) const
	{
		Vec4f ret;
		ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12] * v[3];
		ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13] * v[3];
		ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14] * v[3];
		ret[3] = mat[3] * v[0] + mat[7] * v[1] + mat[11] * v[2] + mat[15] * v[3];
		return ret;
	}
	Mat4f operator*(Float32 f) const 
	{
		Mat4f ret;
		ret[0] = mat[0] * f; ret[4] = mat[4] * f; ret[8] = mat[8] * f; ret[12] = mat[12] * f;
		ret[1] = mat[1] * f; ret[5] = mat[5] * f; ret[9] = mat[9] * f; ret[13] = mat[13] * f;
		ret[2] = mat[2] * f; ret[6] = mat[6] * f; ret[10] = mat[10] * f; ret[14] = mat[14] * f;
		ret[3] = mat[3] * f; ret[7] = mat[7] * f; ret[11] = mat[11] * f; ret[15] = mat[15] * f;
		return ret;
	}
	Mat4f operator*(const Mat4f &m) const 
	{
		Mat4f ret;
		ret[0] = mat[0] * m[0] + mat[4] * m[1] + mat[8] * m[2] + mat[12] * m[3];
		ret[1] = mat[1] * m[0] + mat[5] * m[1] + mat[9] * m[2] + mat[13] * m[3];
		ret[2] = mat[2] * m[0] + mat[6] * m[1] + mat[10] * m[2] + mat[14] * m[3];
		ret[3] = mat[3] * m[0] + mat[7] * m[1] + mat[11] * m[2] + mat[15] * m[3];
		ret[4] = mat[0] * m[4] + mat[4] * m[5] + mat[8] * m[6] + mat[12] * m[7];
		ret[5] = mat[1] * m[4] + mat[5] * m[5] + mat[9] * m[6] + mat[13] * m[7];
		ret[6] = mat[2] * m[4] + mat[6] * m[5] + mat[10] * m[6] + mat[14] * m[7];
		ret[7] = mat[3] * m[4] + mat[7] * m[5] + mat[11] * m[6] + mat[15] * m[7];
		ret[8] = mat[0] * m[8] + mat[4] * m[9] + mat[8] * m[10] + mat[12] * m[11];
		ret[9] = mat[1] * m[8] + mat[5] * m[9] + mat[9] * m[10] + mat[13] * m[11];
		ret[10] = mat[2] * m[8] + mat[6] * m[9] + mat[10] * m[10] + mat[14] * m[11];
		ret[11] = mat[3] * m[8] + mat[7] * m[9] + mat[11] * m[10] + mat[15] * m[11];
		ret[12] = mat[0] * m[12] + mat[4] * m[13] + mat[8] * m[14] + mat[12] * m[15];
		ret[13] = mat[1] * m[12] + mat[5] * m[13] + mat[9] * m[14] + mat[13] * m[15];
	 	ret[14] = mat[2] * m[12] + mat[6] * m[13] + mat[10] * m[14] + mat[14] * m[15];
		ret[15] = mat[3] * m[12] + mat[7] * m[13] + mat[11] * m[14] + mat[15] * m[15];
		return ret;
	}
	Mat4f operator+(const Mat4f &m) const 
	{
		Mat4f ret;
		ret[0] = mat[0] + m[0]; ret[4] = mat[4] + m[4]; ret[8] = mat[8] + m[8]; ret[12] = mat[12] + m[12];
		ret[1] = mat[1] + m[1]; ret[5] = mat[5] + m[5]; ret[9] = mat[9] + m[9]; ret[13] = mat[13] + m[13];
		ret[2] = mat[2] + m[2]; ret[6] = mat[6] + m[6]; ret[10] = mat[10] + m[10]; ret[14] = mat[14] + m[14];
		ret[3] = mat[3] + m[3]; ret[7] = mat[7] + m[7]; ret[11] = mat[11] + m[11]; ret[15] = mat[15] + m[15];
		return ret;
	}
	Mat4f operator-(const Mat4f &m) const
	{
		Mat4f ret;
		ret[0] = mat[0] - m[0]; ret[4] = mat[4] - m[4]; ret[8] = mat[8] - m[8]; ret[12] = mat[12] - m[12];
		ret[1] = mat[1] - m[1]; ret[5] = mat[5] - m[5]; ret[9] = mat[9] - m[9]; ret[13] = mat[13] - m[13];
		ret[2] = mat[2] - m[2]; ret[6] = mat[6] - m[6]; ret[10] = mat[10] - m[10]; ret[14] = mat[14] - m[14];
		ret[3] = mat[3] - m[3]; ret[7] = mat[7] - m[7]; ret[11] = mat[11] - m[11]; ret[15] = mat[15] - m[15];
		return ret;
	}
	
	Mat4f &operator*=(Float32 f) { return *this = *this * f; }
	Mat4f &operator*=(const Mat4f &m) { return *this = *this * m; }
	Mat4f &operator+=(const Mat4f &m) { return *this = *this + m; }
	Mat4f &operator-=(const Mat4f &m) { return *this = *this - m; }
	
	operator Float32*() { return mat; }
	operator const Float32*() const { return mat; }
	
	Float32 &operator[](int32 i) { return mat[i]; }
	const Float32 operator[](int32 i) const { return mat[i]; }
	
	Mat4f rotation() const 
	{
		Mat4f ret;
		ret[0] = mat[0]; ret[4] = mat[4]; ret[8] = mat[8]; ret[12] = 0;
		ret[1] = mat[1]; ret[5] = mat[5]; ret[9] = mat[9]; ret[13] = 0;
		ret[2] = mat[2]; ret[6] = mat[6]; ret[10] = mat[10]; ret[14] = 0;
		ret[3] = 0; ret[7] = 0; ret[11] = 0; ret[15] = 1;
		return ret;
	}
	Mat4f transpose() const
	{
		Mat4f ret;
		ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[3];
		ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[7];
		ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[11];
		ret[3] = mat[12]; ret[7] = mat[13]; ret[11] = mat[14]; ret[15] = mat[15];
		return ret;
	}
	Mat4f transpose_rotation() const 
	{
		Mat4f ret;
		ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[12];
		ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[13];
		ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[14];
		ret[3] = mat[3]; ret[7] = mat[7]; ret[14] = mat[14]; ret[15] = mat[15];
		return ret;
	}
	
	Float32 det() const 
	{
		Float32 det;
		det = mat[0] * mat[5] * mat[10];
		det += mat[4] * mat[9] * mat[2];
		det += mat[8] * mat[1] * mat[6];
		det -= mat[8] * mat[5] * mat[2];
		det -= mat[4] * mat[1] * mat[10];
		det -= mat[0] * mat[9] * mat[6];
		return det;
	}
	
	Mat4f inverse() const 
	{
		Mat4f ret;
		Float32 idet = 1.0f / det();
		ret[0] =  (mat[5] * mat[10] - mat[9] * mat[6]) * idet;
		ret[1] = -(mat[1] * mat[10] - mat[9] * mat[2]) * idet;
		ret[2] =  (mat[1] * mat[6] - mat[5] * mat[2]) * idet;
		ret[3] = 0.0;
		ret[4] = -(mat[4] * mat[10] - mat[8] * mat[6]) * idet;
		ret[5] =  (mat[0] * mat[10] - mat[8] * mat[2]) * idet;
		ret[6] = -(mat[0] * mat[6] - mat[4] * mat[2]) * idet;
		ret[7] = 0.0;
		ret[8] =  (mat[4] * mat[9] - mat[8] * mat[5]) * idet;
		ret[9] = -(mat[0] * mat[9] - mat[8] * mat[1]) * idet;
		ret[10] =  (mat[0] * mat[5] - mat[4] * mat[1]) * idet;
		ret[11] = 0.0;
		ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
		ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
		ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
		ret[15] = 1.0;
		return ret;
	}
	
	void zero() 
	{
		mat[0] = 0.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 0.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 0.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 0.0;
	}
	void identity()
	{
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate(Float32 angle,const Vec3f &axis) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		Vec3f v = axis;
		v.normalize();
		Float32 xy = v.x * v.y;
		Float32 yz = v.y * v.z;
		Float32 zx = v.z * v.x;
		Float32 xs = v.x * s;
		Float32 ys = v.y * s;
		Float32 zs = v.z * s;
		mat[0] = (1.0f - c) * v.x * v.x + c; mat[4] = (1.0f - c) * xy - zs; mat[8] = (1.0f - c) * zx + ys; mat[12] = 0.0;
		mat[1] = (1.0f - c) * xy + zs; mat[5] = (1.0f - c) * v.y * v.y + c; mat[9] = mat[6] = (1.0f - c) * yz - xs; mat[13] = 0.0;
		mat[2] = (1.0f - c) * zx - ys; mat[6] = (1.0f - c) * yz + xs; mat[10] = (1.0f - c) * v.z * v.z + c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate(Float32 angle,Float32 x,Float32 y,Float32 z) {rotate(angle,Vec3f(x,y,z));}
	void rotate_x(Float32 angle) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = c; mat[9] = -s; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = s; mat[10] = c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate_y(Float32 angle) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = c; mat[4] = 0.0; mat[8] = s; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = -s; mat[6] = 0.0; mat[10] = c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate_z(Float32 angle) 
	{
		Float32 rad = angle * DEG2RAD;
		Float32 c = cos(rad);
		Float32 s = sin(rad);
		mat[0] = c; mat[4] = -s; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = s; mat[5] = c; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void scale(const Vec3f &v) 
	{
		mat[0] = v.x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = v.y; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = v.z; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void scale(Float32 x,Float32 y,Float32 z) {scale(Vec3f(x,y,z));}
	void translate(const Vec3f &v)
	{
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = v.x;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = v.y;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = v.z;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void translate(Float32 x,Float32 y,Float32 z) {translate(Vec3f(x,y,z));}
	void reflect(const Vec4f &plane) 
	{
		Float32 x = plane.x;
		Float32 y = plane.y;
		Float32 z = plane.z;
		Float32 x2 = x * 2.0f;
		Float32 y2 = y * 2.0f;
		Float32 z2 = z * 2.0f;
		mat[0] = 1.0f - x * x2; mat[4] = -y * x2; mat[8] = -z * x2; mat[12] = -plane.w * x2;
		mat[1] = -x * y2; mat[5] = 1.0f - y * y2; mat[9] = -z * y2; mat[13] = -plane.w * y2;
		mat[2] = -x * z2; mat[6] = -y * z2; mat[10] = 1.0f - z * z2; mat[14] = -plane.w * z2;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void reflect(Float32 x,Float32 y,Float32 z,Float32 w) {reflect(Vec4f(x,y,z,w));}
	
	void perspective(Float32 fov,Float32 aspect,Float32 znear,Float32 zfar) 
	{
		
		Float32 y = tan(fov * PI / 360.0f);
		Float32 x = y * aspect;
		mat[0] = 1.0f / x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0f / y; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = -1.0; mat[15] = 0.0;

	}
	void look_at(const Vec3f &eye,const Vec3f &dir,const Vec3f &up)
	{
		Vec3f x,y,z;
		Mat4f m0,m1;
		z = eye - dir;
		z.normalize();
		x = up^z;
		x.normalize();
		y = z^x;
		y.normalize();
		m0[0] = x.x; m0[4] = x.y; m0[8] = x.z; m0[12] = 0.0;
		m0[1] = y.x; m0[5] = y.y; m0[9] = y.z; m0[13] = 0.0;
		m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = 0.0;
		m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
		m1.translate(-eye);
		*this = m0 * m1;
	}
	void look_at(const Float32 *eye,const Float32 *dir,const Float32 *up){look_at(Vec3f(eye),Vec3f(dir),Vec3f(up));}
	
	Float32 mat[16];
};

__forceinline Mat3f::Mat3f(const Mat4f &m)
 {
	mat[0] = m[0]; mat[3] = m[4]; mat[6] = m[8];
	mat[1] = m[1]; mat[4] = m[5]; mat[7] = m[9];
	mat[2] = m[2]; mat[5] = m[6]; mat[8] = m[10];
}

struct Quatf
{
	
	Quatf() : x(0), y(0), z(0), w(1) { }
	Quatf(const Vec3f &dir,Float32 angle) {set(dir,angle);}
	Quatf(Float32 x,Float32 y,Float32 z,Float32 angle) {set(x,y,z,angle);}
	
	operator Float32*() { return (Float32*)&x; }
	operator const Float32*() const { return (Float32*)&x; }
	
	Float32 &operator[](int32 i) { return ((Float32*)&x)[i]; }
	const Float32 operator[](int32 i) const { return ((Float32*)&x)[i]; }
	
	Quatf operator*(const Quatf &q) const 
	{
		Quatf ret;
		ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
		ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
		ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
		ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return ret;
	}
	
	void set(const Vec3f &dir,Float32 angle) 
	{
		Float32 length = dir.length();
		if(length != 0.0) {
			length = 1.0f / length;
			Float32 sinangle = sin(angle * DEG2RAD / 2.0f);
			x = dir[0] * length * sinangle;
			y = dir[1] * length * sinangle;
			z = dir[2] * length * sinangle;
			w = cos(angle * DEG2RAD / 2.0f);
		} else {
			x = y = z = 0.0;
			w = 1.0;
		}
	}
	void set(Float32 x,Float32 y,Float32 z,Float32 angle) {set(Vec3f(x,y,z),angle);}
	
	void slerp(const Quatf &q0,const Quatf &q1,Float32 t) 
	{
		Float32 k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
		Quatf q;
		if(cosomega < 0.0) 
		{
			cosomega = -cosomega;
			q.x = -q1.x;
			q.y = -q1.y;
			q.z = -q1.z;
			q.w = -q1.w;
		}
		else
		{
			q.x = q1.x;
			q.y = q1.y;
			q.z = q1.z;
			q.w = q1.w;
		}
		if(1.0 - cosomega > 1e-6) 
		{
			Float32 omega = acos(cosomega);
			Float32 sinomega = sin(omega);
			k0 = sin((1.0f - t) * omega) / sinomega;
			k1 = sin(t * omega) / sinomega;
		} 
		else 
		{
			k0 = 1.0f - t;
			k1 = t;
		}
		x = q0.x * k0 + q.x * k1;
		y = q0.y * k0 + q.y * k1;
		z = q0.z * k0 + q.z * k1;
		w = q0.w * k0 + q.w * k1;
	}
	
	Mat3f to_matrix() const 
	{
		Mat3f ret;
		Float32 x2 = x + x;
		Float32 y2 = y + y;
		Float32 z2 = z + z;
		Float32 xx = x * x2;
		Float32 yy = y * y2;
		Float32 zz = z * z2;
		Float32 xy = x * y2;
		Float32 yz = y * z2;
		Float32 xz = z * x2;
		Float32 wx = w * x2;
		Float32 wy = w * y2;
		Float32 wz = w * z2;
		ret[0] = 1.0f - (yy + zz); ret[3] = xy - wz; ret[6] = xz + wy;
		ret[1] = xy + wz; ret[4] = 1.0f - (xx + zz); ret[7] = yz - wx;
		ret[2] = xz - wy; ret[5] = yz + wx; ret[8] = 1.0f - (xx + yy);
		return ret;
	}
	
	union 
	{
		struct 
		{
			Float32 x,y,z,w;
		};
		Float32 q[4];
	};
};