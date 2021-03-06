#pragma once
#define _AFXDLL
//#undef INTtoSTR
#ifdef _DEBUG
#undef _DEBUG
#include "X:\PROJECTS\I have no idea how to call this thing\packages\python.3.6.2\tools\include\Python.h"
#define _DEBUG
#else
#include "X:\PROJECTS\I have no idea how to call this thing\packages\python.3.6.2\tools\include\Python.h"
#endif
#include <afxwin.h>
#include <algorithm>
#include <AM_uber.h>
#include <dwrite_1.h>
#include <boost\any.hpp>
#include <boost\algorithm\\clamp.hpp>
#include <afxrendertarget.h>
#include <boost\foreach.hpp>
#include <boost\foreach_fwd.hpp>
#include <boost\multiprecision\cpp_int.hpp>
#include <condition_variable>
#include <conio.h>
#include <cstring>
#include <cstdint>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <direct.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include <stdarg.h>
#include <directxmath.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <math.h>  
#include <mutex>
#include <objidl.h>
#include <random>
#include <sstream>
#include <stdio.h>
#include <string>
#include <thread>
#include <uberlibmods.h>
#include <vector>
#include <windows.h>
#include <windowsx.h>
#include <Wincodec.h>
#include <d2d1effecthelpers.h>
#include <Mmsystem.h>
#include <excpt.h>
#include <deque>
#include <signal.h>
#include <tchar.h>
#define DETECTMEMORYLEAKS 0
#ifdef _DEBUG
#if DETECTMEMORYLEAKS == 1
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAPALLOC
#endif
#endif
#include <stdlib.h>
#ifdef _DEBUG
#if DETECTMEMORYLEAKS == 1
#include <crtdbg.h>
#endif
#endif
#include "X:\PROJECTS\AZfilelib\AZfilelib\AZfilelib.h"
using namespace DirectX;
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3dcompiler.lib")
#ifdef _DEBUG
#pragma comment(lib, "X:\\PROJECTS\\AZfilelib\\x64\\Debug\\AZfilelib\\AZfilelib.lib")
#else 
#pragma comment(lib, "X:\\PROJECTS\\AZfilelib\\x64\\Release\\AZfilelib\\AZfilelib.lib")
#endif // DEBUG
#ifdef _DEBUG
#pragma comment(lib, "X:\\PROJECTS\\AM_uberlib\\x64\\Debug\\AM_uberlib.lib")
#else 
#pragma comment(lib, "X:\\PROJECTS\\AM_uberlib\\x64\\Release\\AM_uberlib.lib")
#endif // DEBUG
#define forever while(true)
inline void SignalHandler(int signal)
{
	printf("Signal %d", signal);
	throw "!Access Violation!";
}

inline std::string INTtoSTR(int i)
{
	return std::to_string(i);
}
namespace classvariables
{
	template<typename univar>
	struct uni2;
	struct int2;
	struct int3;
	struct int4;
	struct int2
	{
		int x;
		int y;
		operator XMFLOAT2 ()const
		{
			return XMFLOAT2((float)x, (float)y);
		}
		operator std::vector<int>() const
		{
			return{ x, y };
		}
		friend std::ostream& operator<< (std::ostream& os, int2& i2);
		operator D2D_POINT_2F () const
		{
			return{(float)x,(float)y};
		}
		operator D2D_SIZE_F () const
		{
			return{ (float)x,(float)y };
		}
		//template <typename univar>
		//operator uni2 () const
		//{
		//	return{ (univar)x,(univar)y };
		//}
		template<typename univar>
		uni2<univar> touni2()
		{
			return uni2<univar>{ (univar)x,(univar)y };
		}
		std::vector<int> vec2()
		{
			return{ x,y };
		}
		std::string str()
		{
			return "{ " + to_string(x) + ", " + to_string(y) + " }";
		}
		int2 operator/(int n)
		{
			return{ x / n,y / n };
		}
		int2 operator*(int n)
		{
			return{ x*n,y*n };
		}
		int2 operator-()const
		{
			int2 nv = { -x,-y };
			return nv;
		}
		int2 operator-(int2 &n)
		{
			//this->x -= n.x;
			//this->y -= n.y;
			return{ x - n.x,y - n.y };
		}
		int2 operator+(int2 &n)
		{
			//this->x += n.x;
			//this->y += n.y;
			return{ x + n.x,y + n.y };
		}
		int2 operator+=(int2 &n)
		{
			//this->x += n.x;
			//this->y += n.y;
			return (*this = { x + n.x,y + n.y });
		}
		int2 operator*(int2 &n)
		{
			//this->x *= n.x;
			//this->y *= n.y;
			return{ x*n.x,y*n.y };
		}
		bool operator<(const int2 &n)
		{
			if (x < n.x && y < n.y)
				return true;
			return false;
		}
		bool operator>(const int2 &n)
		{
			if (x > n.x && y > n.y)
				return true;
			return false;
		}
		int2 operator/(int2 n)
		{
			//this->x /= n.x;
			//this->y /= n.y;
			return{ x / n.x,y / n.y };
		}
		int2 operator^(int2 n)
		{
			return{ (int)powl(x, n.x),(int)powl(y, n.y) };
		}
		int2 operator%(int2 n)
		{
			return{ x%n.x,y%n.y };
		}
		bool operator==(int2 n)
		{
			if (x == n.x && y == n.y)
				return true;
			return false;
		}
		bool operator!=(int2 n)
		{
			if (x != n.x || y != n.y)
			{
				return true;
			}
			return false;
		}
		inline int2(int nx, int ny)
		{
			this->x = nx;
			this->y = ny;
		}
		inline int2(D2D_SIZE_F n)
		{
			x = n.width;
			y = n.height;
		}
		inline int2(XMFLOAT2 n)
		{
			x =(int) n.x;
			y = (int) n.y;
		}
		//template <typename univar>
		//inline int2(uni2<univar> u2)
		//{
		//	x = u2.x;
		//	y = u2.y;
		//}
		int2()
		{

		}
	};
	inline std::ostream& operator<< (std::ostream& os, int2& i2)
	{
		return (os << (std::string)("(" + INTtoSTR(i2.x) + ", " + INTtoSTR(i2.y) + ")"));
	}
	inline bool operator<(const int2 &n1, const int2 &n2)
	{
		if (n1.x < n2.x && n1.y < n2.y)
			return true;
		return false;
	}
	inline pair<double, double> normalize(int2 f)
	{
		double len = sqrt(f.x * f.x + f.y * f.y);
		if (len != 0)
		{
			return{ (double)f.x / (double)len,(double)f.y / (double)len };
		}
		return pair<double, double>(0, 0);
	}
	struct int3
	{
		int x;
		int y;
		int z;
		int3 operator-()const
		{
			int3 nv = { -x,-y,-z };
			return nv;
		}
		int3 operator-(int3 n)
		{
			return int3(x - n.x, y - n.y, z - n.z);
		}
		int3 operator+(int3 n)
		{
			return int3(x + n.x, y + n.y, z + n.z);
		}
		int3 operator*(int3 n)
		{
			return int3(x * n.x, y * n.y, z * n.z);
		}
		int3 operator/(int3 n)
		{
			return int3(x / n.x, y / n.y, z / n.z);
		}
		int3 operator^(int3 n)
		{
			return int3(powl(x, n.x), powl(y, n.y), powl(z, n.z));
		}
		bool operator==(int3 n)
		{
			if (x == n.x && y == n.y && z == n.z)
				return true;
			return false;
		}
		bool operator!=(int3 n)
		{
			if (x != n.x || y != n.y || z != n.z)
			{
				return true;
			}
			return false;
		}
		inline int3(int nx, int ny, int nz)
		{
			x = nx;
			y = ny;
			z = nz;
		}
		int3()
		{

		}
	};
	struct int4
	{
		int x;
		int y;
		int z;
		int w;
		int2 first()
		{
			return int2(x,y);
		}
		int2 second()
		{
			return int2(z, w);
		}
		operator D2D1_RECT_U() const
		{
			return{(ui) x,(ui)y,(ui)z,(ui)w };
		}
		int4 operator-()const
		{
			int4 nv = { -x,-y,-z,-w };
			return nv;
		}
		int4 operator-(int4 n)
		{
			return int4(x - n.x, y - n.y, z - n.z, w - n.w);
		}
		int4 operator+(int4 n)
		{
			return int4(x + n.x, y + n.y, z + n.z, w + n.w);
		}
		int4 operator*(int4 n)
		{
			return int4(x * n.x, y * n.y, z * n.z, w * n.w);
		}
		int4 operator/(int4 n)
		{
			return int4(x / n.x, y / n.y, z / n.z, w / n.w);
		}
		int4 operator^(int4 n)
		{
			return int4(powl(x, n.x), powl(y, n.y), powl(z, n.z), powl(w, n.w));
		}
		bool operator==(int4 n)
		{
			if (x == n.x && y == n.y && z == n.z && w == n.w)
				return true;
			return false;
		}
		bool operator!=(int4 n)
		{
			if (x != n.x || y != n.y || z != n.z || w != n.w)
			{
				return true;
			}
			return false;
		}
		operator D2D_RECT_F() const
		{
			return D2D_RECT_F{(float)x,(float)y,(float)z,(float)w};
		}
		int4 operator=(D2D_RECT_F rct)
		{
			return{ (int)rct.right, (int)rct.top, (int)rct.left, (int)rct.bottom };
		}
		inline int4(int nx, int ny, int nz, int nw)
		{
			x = nx;
			y = ny;
			z = nz;
			w = nw;
		}
		inline int4(int2 n0, int2 n1)
		{
			x = n0.x;
			y = n0.y;
			z = n1.x;
			w = n1.y;
		}
		inline int4(D2D_RECT_F r)
		{
			x = r.left;
			y = r.top;
			z = r.right;
			w = r.bottom;
		}
		int4()
		{

		}
	};
	inline bool isinside(int4 rect, int2 point)
	{
		if (point.x < rect.x)
			return false;
		if (point.y < rect.y)
			return false;
		if (point.x > rect.z)
			return false;
		if (point.y > rect.w)
			return false;
		return true;
	}
	template <typename univar>
	struct uni2
	{
		univar x;
		univar y;
		operator D2D_POINT_2F () const
		{
			return{ (float)x,(float)y };
		}
		operator D2D_SIZE_F () const
		{
			return{ (float)x,(float)y };
		}
		//operator int2 () const
		//{
		//	return{ (int)x,(int)y };
		//}
		uni2(int2 n)
		{
			x = n.x;
			y = n.y;
		}
		uni2(const D2D_POINT_2F& p )
		{
			x = p.x;
			y = p.y;
		}
		int2 toint2()
		{
			return{ (int)x,(int)y };
		}
		int2 toint2low()
		{
			return{ (int)floor(x),(int)floor(y) };
		}
		int2 toint2high()
		{
			return{ (int)ceil(x),(int)ceil (y) };
		}
		operator std::vector<univar>() const
		{
			return{ x, y };
		}
		operator XMFLOAT2 ()const
		{
			return XMFLOAT2( x,y );
		}
		uni2<univar>& operator=(const pair<univar, univar>& v)
		{
			return uni2<univar>{ v.first,v.second };
		}
		uni2<univar>& operator=(const uni2<univar>&) = default;
		std::vector<univar> vec2()
		{
			return{ x,y };
		}
		uni2 operator-()const
		{
			uni2 nv = { -x,-y };
			return nv;
		}
		uni2 operator/(univar n)
		{
			return{ x / n,y / n };
		}
		uni2 operator*(univar n)
		{
			return{ x*n,y*n };
		}
		uni2 operator-(uni2 n)
		{
			//this->x -= n.x;
			//this->y -= n.y;
			return{ x - n.x,y - n.y };
		}
		uni2 operator+(uni2 n)
		{
			//this->x += n.x;
			//this->y += n.y;
			return{ x + n.x,y + n.y };
		}
		uni2 operator*(uni2 n)
		{
			//this->x *= n.x;
			//this->y *= n.y;
			return{ x*n.x,y*n.y };
		}
		bool operator<(uni2 n)
		{
			if (x < n.x && y < n.y)
				return true;
			return false;
		}
		bool operator>(uni2 n)
		{
			if (x > n.x && y > n.y)
				return true;
			return false;
		}
		uni2 operator/(uni2 n)
		{
			//this->x /= n.x;
			//this->y /= n.y;
			return{ x / n.x,y / n.y };
		}
		uni2 operator^(uni2 n)
		{
			return{ (univar)powl(x, n.x),(univar)powl(y, n.y) };
		}
		uni2 operator%(uni2 n)
		{
			return{ x%n.x,y%n.y };
		}
		bool operator==(uni2 n)
		{
			if (x == n.x && y == n.y)
				return true;
			return false;
		}
		bool operator!=(uni2 n)
		{
			if (x != n.x || y != n.y)
			{
				return true;
			}
			return false;
		}
		inline uni2(XMFLOAT2 n)
		{
			x = (univar)n.x;
			y = (univar)n.y;
		}
		//inline uni2(int2 n)
		//{
		//	x = (univar)n.x;
		//	y = (univar)n.y;
		//}
		inline uni2(pair<univar, univar> v)
		{
			*this = this->operator=( v);
		}
		inline uni2(univar nx, univar ny)
		{
			this->x = nx;
			this->y = ny;
		}
		uni2()
		{

		}
	};
	
}
#define stype					shadertypes
#define float2					XMFLOAT2
#define float3					XMFLOAT3
#define float4					XMFLOAT4 
#define float3x3				XMFLOAT3X3
#define float4x3				XMFLOAT4X3
#define float4x4				XMFLOAT4X4
#define int2					classvariables::int2
#define int3					classvariables::int3
#define int4					classvariables::int4
#define uni2					classvariables::uni2
#define finline					__forceinline
#define int64					signed long long int
#define int128					boost::multiprecision::int128_t
#define int256 					boost::multiprecision::int256_t
#define int512 					boost::multiprecision::int512_t
#define int1024					boost::multiprecision::int1024_t
#define uint64					unsigned long long int
#define uint128					boost::multiprecision::uint128_t
#define uint256		 		    boost::multiprecision::uint256_t
#define uint512					boost::multiprecision::uint512_t
#define uint1024				boost::multiprecision::uint1024_t
#define finline					__forceinline
#define LE_DIR_FORWARD			1
#define LE_DIR_BACKWARD			-1
#define LE_DIR_RIGHT			90
#define LE_DIR_LEFT				-90
#define LE_DIR_UP				91
#define	LE_DIR_DOWN				-89
#define	LE_ROT_PITCH			6
#define LE_ROT_YAW				7
#define LE_ROT_ROLL				8
#define CHK_w					119
#define CHK_s					115
#define CHK_a					97
#define CHK_d					100
#define CHK_c					99
#define CHK_x					120
#define VK_W					0x57
#define VK_A					0x41
#define VK_D					0x44
#define VK_S					0x53
#define VK_Q					0x51
#define VK_E					0x45
#define VK_P					0x50
#define fullscreentimeouttimer  3242424
#define fullscreentimeout		100
#define ID_STARTBUTTON			6554
#define M_PIl				    3.141592653589793238462643383279502884L
#define mousephysid				1
#define shipphysid				123
#define mt_rot					1
#define mt_mov					2
#define mt_movnext				3
#define monitorshipresdebug		1
#define catchleaks				0
#define sectorsize				299792
#define defaultinterpolationmode D2D1_BITMAP_INTERPOLATION_MODE_LINEAR  //D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
#define advancedinterpolatonmode D2D1_INTERPOLATION_MODE::D2D1_INTERPOLATION_MODE_ANISOTROPIC
#define PYFUNCD(name) static  PyObject* name(PyObject *self, PyObject *args)
#define PYFUNC(name) mutex _ ## name ## Mutex; static PyObject* name(PyObject *self, PyObject *args); vecbreaker<PyObject*(*)(PyObject*,PyObject*)> name ## generic_vbreaker(vb,name,#name,__COUNTER__); PYFUNCD(name)
#define BS(name) void name (string s); vecbreaker<void(*)(string s)> name ## vbreaker = vb + &name
#define PYMETH(name) {#name,name,METH_VARARGS, "If I'd tell you I'd have to kill you"}
#define PYWRITESCRIPTDATA false
template<typename univar>
class vecbreaker
{
public:
	vector<univar> vec;
	vector<string> names;
	vecbreaker& operator+(univar v1)
	{
		vec.push_back(v1);
		return *this;
	}
	vecbreaker()
	{

	}
	vecbreaker(vecbreaker &vbr,univar uv,string str...)
	{
		vbr.vec.push_back(uv);
		vbr.names.push_back(str);
	}
};

#define stationsizemultip 2.0f
namespace common
{
	struct RGBA
	{
		int r;
		int g;
		int b;
		float a;
		bool in = true;
		bool out = true;
		RGBA()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 1.0f;
		}
		RGBA(int nr, int ng, int nb,float na = 1.0f)
		{
			r = nr;
			g = ng;
			b = nb;
			a = na;
		}
		//RGBA(RGBA& n) = default;
		D2D1::ColorF toColorF()
		{
			this;
			return D2D1::ColorF(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f, a);
		}
	};
	inline string findoneoffiles(vector<string>locations)
	{
		int i = 0;
		string ret = "NULL";
		while (i < locations.size() && ret == "NULL")
		{
			WIN32_FIND_DATA FindFileData;
			if ((FindFirstFileA(locations[i].c_str(), &FindFileData)!= INVALID_HANDLE_VALUE))
			{
				ret = locations[i];
			}
			i++;
		}
		return ret;
	}
	inline std::string INTtoSTR(int i)
	{
		return std::to_string(i);
	}
	namespace Python
	{
		inline PyObject* VecToPyobj(vector<ui> v)
		{
			int i = 0;
			PyObject* ret = PyList_New(0);
			while (i < v.size())
			{
				PyList_Append(ret, PyLong_FromUnsignedLong(v[i]));
				i++;
			}
			return ret;
		}
		inline PyObject* VecToPyobj(vector<int> v)
		{
			int i = 0;
			PyObject* ret = PyList_New(0);
			while (i < v.size())
			{
				PyList_Append(ret, PyLong_FromLong(v[i]));
				i++;
			}
			return ret;
		}
		inline PyObject* VecToPyobj(vector<ulli> v)
		{
			int i = 0;
			PyObject* ret = PyList_New(0);
			while (i < v.size())
			{
				PyList_Append(ret, PyLong_FromUnsignedLongLong(v[i]));
				i++;
			}
			return ret;
		}
		inline PyObject* VecToPyobj(vector<string> v)
		{
			int i = 0;
			PyObject* ret = PyList_New(0);
			while (i < v.size())
			{
				PyList_Append(ret, PyUnicode_FromString(v[i].c_str()));
				i++;
			}
			return ret;
		}
		inline PyObject* ToPyObj(int var)
		{
			return PyLong_FromLong(var);
		}
		inline PyObject* ToPyObj(string var)
		{
			return PyUnicode_FromStringAndSize(var.c_str(), var.size());
		}
		inline PyObject* ToPyObj(ui var)
		{
			return PyLong_FromUnsignedLong(var);
		}
		inline PyObject* ToPyObj(ulli var)
		{
			return PyLong_FromUnsignedLongLong(var);
		}
		inline PyObject* ToPyObj(void* var)
		{
			return PyLong_FromVoidPtr(var);
		}
		inline PyObject* ToPyObj(bool var)
		{
			return PyBool_FromLong(var);
		}
		inline PyObject* ToPyObj(wstring var)
		{
			return PyUnicode_FromWideChar(var.c_str(), var.size());
		}
		inline PyObject* ToPyObj(lli var)
		{
			return PyLong_FromLongLong(var);
		}
		inline PyObject* ToPyObj(double var)
		{
			return PyLong_FromDouble(var);
		}
		inline PyObject* ToPyObj(float var)
		{
			return PyFloat_FromDouble(var);
		}
		inline boost::any AnyFromPyObj(PyObject* val,string type)
		{
			if (type == "INT" || type == "INT32")
			{
				return (boost::any)(PyLong_AsLong(val));
			}
			else if (type == "UINT" || type == "UINT32")
			{
				return (boost::any)(PyLong_AsUnsignedLong(val));
			}
			else if (type == "INT64")
			{
				return (boost::any)(PyLong_AsLongLong(val));
			}
			else if (type == "UINT64")
			{
				return (boost::any)(PyLong_AsUnsignedLongLong(val));
			}
			//else if (type == "INT128")
			//{
			//	return ToPyObj( boost::any_cast<mp::int128_t>(val) );
			//}
			//else if (type == "UINT128")
			//{
			//	return ToPyObj( boost::any_cast<mp::uint128_t>(val) );
			//}
		/*	else if (type == "CHAR")
			{
				return (boost::any)(PyUnico(val));
			}
			else if (type == "WCHAR")
			{
				return (boost::any)(PyLong_AsLong(val));
			}*/
			else if (type == "BOOL")
			{
				return (boost::any)(bool)(PyObject_IsTrue(val));
			}
			else if (type == "FLOAT")
			{
				return (boost::any)(float)(PyFloat_AsDouble(val));
			}
			else if (type == "DOUBLE")
			{
				return (boost::any)(PyFloat_AsDouble(val));
			}
			else if (type == "STR")
			{
				return (boost::any)(string)(_PyUnicode_AsString(val));
			}
			/*else if (type == "WSTR")
			{
				const std::wstring ws = boost::any_cast<wstring>(val);
				const std::locale locale("");
				typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
				const converter_type& converter = std::use_facet<converter_type>(locale);
				std::vector<char> to(ws.length() * converter.max_length());
				std::mbstate_t state;
				const wchar_t* from_next;
				char* to_next;
				const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
				const std::string s(&to[0], to_next);
				return ToPyObj(s);
			}*/
		}
		inline PyObject* CastPyObj(boost::any val, string type)
		{
			if (type == "INT" || type == "INT32")
			{
				return ToPyObj(boost::any_cast<int>(val));
			}
			else if (type == "UINT" || type == "UINT32")
			{
				return ToPyObj(boost::any_cast<ui>(val) );
			}
			else if (type == "INT64")
			{
				return ToPyObj(boost::any_cast<lli>(val) );
			}
			else if (type == "UINT64")
			{
				return ToPyObj(boost::any_cast<ulli>(val) );
			}
			//else if (type == "INT128")
			//{
			//	return ToPyObj( boost::any_cast<mp::int128_t>(val) );
			//}
			//else if (type == "UINT128")
			//{
			//	return ToPyObj( boost::any_cast<mp::uint128_t>(val) );
			//}
			else if (type == "CHAR")
			{
				return ToPyObj( boost::any_cast<char>(val) );
			}
			else if (type == "WCHAR")
			{
				return ToPyObj( boost::any_cast<wchar_t>(val) );
			}
			else if (type == "BOOL")
			{
				return ToPyObj( boost::any_cast<bool>(val) );
			}
			else if (type == "FLOAT")
			{
				return ToPyObj( boost::any_cast<float>(val) );
			}
			else if (type == "DOUBLE")
			{
				return ToPyObj( boost::any_cast<double>(val) );
			}
			else if (type == "STR")
			{
				string str = (string) boost::any_cast<string>(val);
				return ToPyObj( str );
			}
			else if (type == "WSTR")
			{
				const std::wstring ws = boost::any_cast<wstring>(val);
				const std::locale locale("");
				typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
				const converter_type& converter = std::use_facet<converter_type>(locale);
				std::vector<char> to(ws.length() * converter.max_length());
				std::mbstate_t state;
				const wchar_t* from_next;
				char* to_next;
				const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
				const std::string s(&to[0], to_next);
				return ToPyObj( s );
			}
		}
	}
}
inline bool operator>(float2 f0, float2 f1)
{
	return(f0.x > f1.x && f0.y > f1.y);
}
//inline D2D1_POINT_2F operator+(const D2D1_POINT_2F &n0, const D2D1_POINT_2F &n1)
//{
//	return D2D1_POINT_2F{ n0.x + n1.x,n0.y + n1.y };
//}
inline bool enterFullscreen(HWND hwnd, int fullscreenWidth, int fullscreenHeight, int colourBits, int refreshRate) {
	DEVMODE fullscreenSettings;
	bool isChangeSuccessful;
	RECT windowBoundary;

	EnumDisplaySettings(NULL, 0, &fullscreenSettings);
	fullscreenSettings.dmPelsWidth = fullscreenWidth;
	fullscreenSettings.dmPelsHeight = fullscreenHeight;
	fullscreenSettings.dmBitsPerPel = colourBits;
	fullscreenSettings.dmDisplayFrequency = refreshRate;
	fullscreenSettings.dmFields = DM_PELSWIDTH |
		DM_PELSHEIGHT |
		DM_BITSPERPEL |
		DM_DISPLAYFREQUENCY;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, fullscreenWidth, fullscreenHeight, SWP_SHOWWINDOW);
	isChangeSuccessful = ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
	ShowWindow(hwnd, SW_MAXIMIZE);

	return isChangeSuccessful;
}
inline bool exitFullscreen(HWND hwnd, int windowX, int windowY, int windowedWidth, int windowedHeight, int windowedPaddingX, int windowedPaddingY) {
	bool isChangeSuccessful;

	SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_LEFT);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	isChangeSuccessful = ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL;
	SetWindowPos(hwnd, HWND_NOTOPMOST, windowX, windowY, windowedWidth + windowedPaddingX, windowedHeight + windowedPaddingY, SWP_SHOWWINDOW);
	ShowWindow(hwnd, SW_RESTORE);

	return isChangeSuccessful;
}
inline int2 rotate_point(float cx, float cy, float angle, int2 p)
{
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	p.x -= cx;
	p.y -= cy;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cx;
	p.y = ynew + cy;
	return p;
}
inline std::string tolower(string str)
{
	int i = 0;
	while (i < str.size())
	{
		str[i] = tolower(str[i]);
		i++;
	}
	return str;
}
inline std::string reverseSTR(std::string str)
{
	std::string rstr;
	int i = 0;
	int len = str.length() - 1;
	while (i < len)
	{
		//if (len - i < 0)
		//{
		//	break;
		//}
		rstr += str[len - i];
		i++;
	}
	rstr += str[0];
	return rstr;
}
inline std::string copyfromchtoend(std::string str, char ch, bool includech) // copies everything AFTER str[i] == ch
{
	std::string rstr;
	int i = 0;
	while (i < str.length())
	{
		if (str[i] == ch)
		{
			if (!includech)
				i++;
			goto noerror;
		}
		i++;
	}
	return "error";
noerror:; // YET
	while (i < str.length())
	{
		rstr += str[i];
		i++;
	}
	return rstr;
}
template <typename univar>
univar Distance(univar dX0, univar dY0, univar dX1, univar dY1)
{
	return sqrt(((double)dX1 - (double)dX0)*((double)dX1 - (double)dX0) + ((double)dY1 - (double)dY0)*((double)dY1 - (double)dY0));
}
inline double Distance(int2 d0, int2 d1)
{
	return Distance<double>(d0.x, d0.y, d1.x, d1.y);
}
//template <typename univar>
//inline void constraintoval(univar &var, univar val) // constrains variable's max value
//{
//	while (var > val)
//	{
//		var -= val;
//	}
//	while (var < 0)
//	{
//		var += val;
//	}
//}
//template <typename univar>
//inline void constraintoscope(univar &var, univar maxval, univar minval)// constrains variable's maximal and minimal value
//{
//	while (var > maxval)
//	{
//		var -= maxval + minval;
//	}
//	while (var < minval)
//	{
//		var += maxval + minval;
//	}
//}
//std::wstring STRtoWSTR(const std::string& s);
//inline LPCWSTR STRtoLPCW(std::string &str);
//inline std::string _INTtoSTR(int i)
//{
//	return std::to_string(i);
//}
//#define _INTtoSTR INTtoSTR

struct mainwins
{
	int pozX = 700;
	int pozY = 0;
	DWORD dwstyle = WS_OVERLAPPEDWINDOW;
	UINT wcstyle = CS_DBLCLKS;
	LPCSTR name = "maintest";
	std::string programloc = "";
	bool fullscreen = false;
	bool vsync = false;
	std::vector<std::string> args;
	int argc = 1;
};
namespace GAME
{
	void move(int2& pos);
	class station;
	class star;
	class sub;
	class system;
	class universe;
	class stationmodIN;
	class stationmodOUT;
}
namespace DX2D
{
	extern ID2D1DeviceContext *hwndRT;
	class main;
	void init(mainwins style);
	void Release();
	void Frame(bool ismth = false,condition_variable* _mttw = nullptr, unique_lock<mutex>* _lk = nullptr);
	LRESULT EmulateLEwinproc(HWND hwndwin, UINT msg, WPARAM wParam, LPARAM lParam);
}
using namespace std;
inline string OnOffSTR(bool b)
{
	if (b)
		return "ON";
	else
		return "OFF";
}
inline float percentageof(ui part, ui of)
{
	float one = of / 100;
	return part / one * 100;
}
inline int2 percentageofresi(int2 res, uni2<float> perc) 
{
	uni2<float> rvf = perc;
	rvf.x *= res.x;
	rvf.y *= res.y;
	return int2(rvf.x, rvf.y);
}
inline int2 percentageofresi(int2 res, float xperc, float yperc)
{
	return percentageofresi(res, { xperc,yperc });
}
inline uni2<float> percentageofresf(int2 res, uni2<float> perc)
{
	return uni2<float>(res.x, res.y)* perc;
}
inline void displayHRerrors(HRESULT hr, HWND hwnd, int line = -1, bool fstop = false, string errormessage = "", string name = "error", vector<string>adinfonames = {}, ...)
{
	if (SUCCEEDED(hr))
		return;
	va_list vl;
	int i = 0;
	string adstr = "";
	if (adinfonames.size() > 0)
	{
		va_start(vl, adinfonames);
		while (i < adinfonames.size())
		{
			adstr += adinfonames[i] + " " + va_arg(vl, string) + ", ";
			i++;
		}
		adstr.erase(adstr.length() - 1);
		adstr.erase(adstr.length() - 1);
		va_end(vl);
	}
	string text = "An error has occurred";
	if (line != -1)
		text += " at line " + to_string(line);
	if (errormessage != "")
		text += ": " + errormessage;
	if (adstr != "")
		text += "\nAdditional info:\n" + adstr;
	text += "\nAttemnpt to continue?";
	int res = MessageBox(hwnd,text.c_str(),name.c_str(), MB_YESNO| MB_ICONERROR);
	if (res == IDNO)
		DebugBreak();
	if (fstop)
		DebugBreak();
	i = 0;
}
#define poresi percentageofresi
#define poresf percentageofresf
template <typename univar>
inline void constraintoval(univar &var, univar val) // constrains variable's max value
{
	while (var >= val)
	{
		var -= val;
	}
	while (var < 0)
	{
		var += val;
	}
}
template <typename univar0,typename univar1>
inline univar0 findkeybyvalue(map<univar0, univar1> m, univar1 val)
{
	//pair<univar0, univar1> it;
	/*for (auto it = m.begin(); it != m.end(); ++it)
		if (it->second == univar1)
			return it->first;*/
	//const int prevToFind = 10;
	//auto findResult = std::find_if(std::begin(val), std::end(val), [&](const std::pair<int, struct_t*> &pair)
	//{
	//	return pair.second->prev == prevToFind;
	//});
	//struct_t *foundValue = nullptr
	//	if (findResult != std::end(val))
	//	{
	//		foundKey = findResult->first;
	//		foundValue = findResult->second;

	//		// Now do something with the key or value!
	//	}
	BOOST_FOREACH(auto it, m)
	{
		if (it.second == val)
			return it.first;
	}
}
template <typename univar>
inline vector<univar*> copyvecofptrs(vector<univar*> univec)
{
	int i = 0;
	while (i < univec.size())
	{
		univec[i] = new univar(*univec[i]);
		i++;
	}
	return univec;
}
template <typename univar>
inline vector<univar*> copyvecofptrs(vector<univar*> univec,univar*(univar::*copyfunc)(void))
{
	int i = 0;
	while (i < univec.size())
	{
		univec[i] = /*univec[i]->copyfunc();*/ (univar*)(((*univec[i]).*copyfunc)());
		i++;
	}
	return univec;
}
template <typename univar>
inline void constraintoscope(univar &var, univar maxval, univar minval)// constrains variable's maximal and minimal value
{
	while (var > maxval)
	{
		var -= maxval + minval;
	}
	while (var < minval)
	{
		var += maxval + minval;
	}
}
template <typename univar,typename univar2>
inline void constraintoscope(univar &var, univar2 maxval, univar2 minval)// constrains variable's maximal and minimal value
{
	while (var > maxval)
	{
		var -= maxval + minval;
	}
	while (var < minval)
	{
		var += maxval + minval;
	}
}
template <typename univar>
inline univar dwgbv(univar val, univar decreaseby, univar valtodecrease)//decrease without going below value
{
	if (valtodecrease - decreaseby < val)
		return val;
	else
		return valtodecrease - decreaseby;
}
template <typename univar>
inline univar iwgav(univar val, univar increaseby, univar valtoincrease)//increase without going above value
{
	if (valtoincrease - increaseby > val)
		return val;
	else
		return valtoincrease - increaseby;
}
template<typename univar, typename univar2>
bool MapFind(map<univar, univar2> m, univar key)
{
	if (m.find(key) == m.end())
		return false;
	return true;
}
template<typename univar, typename univar2>
bool InsertToMapOfVecs(map<univar, vector<univar2>> &m, pair<univar, univar2> p)
{
	if (MapFind(m, p.first))
	{
		m[p.first].push_back(p.second);
	}
	else
	{
		m.insert(make_pair(p.first, vector<univar2>{p.second}));
	}
	return true;
}
template<typename univar, typename univar2>
bool MapTGT(map<univar, univar2> m, univar key, univar2 val)
{
	if (!MapFind(m, key))
	{
		return false;
	}
	if (m[key] < val)
		return true;
	return false;
}
template<typename univar, typename univar2>
bool MapTLT(map<univar, univar2> m, univar key, univar2 val)
{
	if (!MapFind(m, key))
	{
		return false;
	}
	if (m[key] > val)
		return true;
	return false;
}
template<typename univar, typename univar2>
bool MapTEQ(map<univar, univar2> m, univar key, univar2 val)
{
	if (!MapFind(m, key))
	{
		return false;
	}
	if (m[key] == val)
		return true;
	return false;
}
template<typename univar, typename univar2>
pair<univar, univar2>& operator+=(pair<univar, univar2>&p, pair<univar, univar2>&p2)
{
	p.first += p2.first;
	p.second += p2.second;
	return p;
}
template<typename univar, typename univar2>
void MapSet(map<univar, univar2> &m, pair<univar, univar2> val)
{
	if (MapFind(m, val.first))
		m[val.first] = val.second;
	else
		m.insert(val);
	return;
}
template<typename univar, typename univar2>
void AddToMap(map<univar, univar2> &m, pair<univar, univar2> add)
{
	if (MapFind(m, add.first))
		m[add.first] += add.second;
	else
		m.insert(add);
	return;
}
template<typename univar,typename univar2>
map<univar, univar2>SumMaps(map<univar, univar2> m0, map<univar, univar2> m1)
{
	for (auto it = m1.begin(); it != m1.end(); ++it) m0[it->first] += it->second;
	return m0;

}
template<typename univar, typename univar2>
map<univar, univar2>_SumMaps(map<univar, univar2> m0, map<univar, univar2> m1) // slower, use when normal SumMaps throws an error
{
	pair<univar, univar2> p;
	BOOST_FOREACH(p, m1)
	{
		m0.insert(p);
	}
	return m0;

}
template<typename univar, typename univar2>
vector<univar>MapFirsttoVec(map <univar, univar2> m)
{
	pair<univar, univar2> me;
	vector<univar> v;
	BOOST_FOREACH(me, m) {
		v.push_back(me.first);
	}
	return v;
}
template<typename univar, typename univar2>
vector<univar2>MapSecondtoVec(map <univar, univar2> m)
{
	pair<univar, univar2> me;
	vector<univar2> v;
	BOOST_FOREACH(me, m) {
		v.push_back(me.second);
	}
	return v;
}
template<typename univar, typename univar2>
map<univar, univar2>VecToMap(vector<pair<univar, univar2>>v)
{
	int i = 0;
	map<univar, univar2> m;
	while (i < v.size())
	{
		m.insert(v[i]);
		i++;
	}
	return m;
}
template<typename univar>
map<univar, void*>VecToMapnptr(vector<univar>v)
{
	int i = 0;
	map<univar, void*>  m;
	while (i < v.size())
	{
		m.insert(make_pair(v[i], nullptr));
		i++;
	}
	return m;
}
template<typename univar, typename univar2>
vector<pair<univar, univar2>>MaptoVec(map <univar, univar2> m)
{
	pair<univar, univar2> me;
	vector<pair<univar, univar2>> v;
	BOOST_FOREACH(me, m) {
		v.push_back(me);
	}
	return v;
}
template<typename univar>
bool FindInVec(vector<univar> v, univar key)
{
	int i = 0;
	while (i < v.size())
	{
		if (v[i] == key)
			return true;
		i++;
	}
	return false;
}
template<typename univar>
inline univar GetHighestof2(univar e1, univar e2)
{
	if (e1 > e2)
		return e1;
	return e2;
}
template<typename univar>
bool FindInVec(vector<univar> v, univar key, int &ii)
{
	int i = 0;
	while (i < v.size())
	{
		if (v[i] == key)
		{
			ii = i;
			return true;
		}
		i++;
	}
	return false;
}
template <typename univar>
inline void VecProceed(int& i, vector<univar> vec) 
{
	i++;
	if (i >= vec.size())
	{
		constraintoval(i, (int)vec.size());
		return;
	}
}
template <typename univar>
inline void VecProceedBy(int& i, int by, vector<univar> vec)
{
	i += by;
	if ( i >= vec.size())
	{
		constraintoval(i, (int)vec.size() - 1);
		return;
	}
}
template <typename univar>
inline vector<univar> SumVecs(vector<univar> v0, vector<univar> v1)
{
	int i = 0;
	vector<univar> v;
	while (i < v0.size())
	{
		v.push_back(v0);
		i++;
	}
	i = 0;
	while (i < v1.size())
	{
		v.push_back(v1);
		i++;

	}
}
inline std::string RandomString(int len, ui seed = 0/*won't be used if 0*/)
{
	if (seed != 0)
		srand(seed);
	int i = 0;
	string retval = "";
	while (i < len)
	{
		retval += (string)(INTtoSTR(rand() % 254));
		i++;
	}
	return retval;
}
//inline bool isblankch(char ch)
//{
//	if (ch == NULL || ch == ' ' || ch == '\t' || ch == '\n')
//		return true;
//	return false;
//}
//inline bool isblankstr(string str)
//{
//	int i = 0;
//	if (str == "")
//		return true;
//	while (i < str.size())
//	{
//		if (!isblankch(str[i]))
//			return false;
//	}
//	return true;
//}