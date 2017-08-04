#pragma once
#include "hmain.h"
using namespace std;
using namespace D2D1;
namespace DX2D
{
	union brushunion
	{
		ID2D1LinearGradientBrush*		 						  lineargradient;
		pair<ID2D1RadialGradientBrush*, ID2D1RadialGradientBrush*>radialgradient; // first will fill rect, second will be  drawn around.
		pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>		  solidbrush; // first will fill rect, second will be  drawn around.
		ID2D1BitmapBrush*				 						  bitmapbrush;
		void*							 						  nullplaceholder;
		brushunion()
		{
			nullplaceholder = nullptr;
		}
	};
	enum brushtypes
	{
		lineargradient, radialgradient, solidbrush, bitmapbrush, nullbrush
	};
	struct brush
	{
		brushtypes  brushtype;
		brushunion  b;
		D2D_RECT_F  rect;
		D2D1_ELLIPSE elipse;
		bool createrectfrompossize = false;
		int2* pos;
		int2* size;
		int2* offset;
		float* rot;
		bool useidentp = false;
		bool* identp = nullptr;
		bool* renderp = nullptr;
		bool breakonrender = false;
		void copypointers()
		{
			if (identp != nullptr)
			{
				identp = new bool(*identp);
			}
			if (renderp != nullptr)
			{
				renderp = new bool(*renderp);
			}
			pos = new int2(*pos);
			size = new int2(*size);
			offset = new int2(*offset);
			rot = new float(*rot);
		}
		void SetType(brushtypes bt)
		{
			brushtype = bt;
			switch (bt)
			{
			case lineargradient:
			{
				b.lineargradient = NULL;
				break;
			}
			case radialgradient:
			{
				b.radialgradient.first = NULL;
				b.radialgradient.second = NULL;
				break;
			}
			case solidbrush:
			{
				b.solidbrush.first = NULL;
				b.solidbrush.second = NULL;
				break;
			}
			case bitmapbrush:
			{
				b.bitmapbrush = NULL;
				break;
			}
			case nullbrush:
			{
				b.nullplaceholder = nullptr;
				break;
			}
			}
		}
		void Release()
		{
			if (rot != nullptr)
				delete rot;
			if (pos != nullptr)
				delete pos;
			if (size != nullptr)
				delete size;
		}
		brush()
		{
			brushtype = nullbrush;
			pos = nullptr;
			size = nullptr;
			rot = nullptr;
			offset = new int2(0, 0);
		}
		brush(bool updaterectfrompossize, int2 npos, int2 nsize, float nrot)
		{
			createrectfrompossize = updaterectfrompossize;
			pos = new int2(npos);
			size = new int2(nsize);
			offset = new int2(0, 0);
			rot = new float(nrot);
			rect.top = pos->y;
			rect.bottom = pos->y + size->y;
			rect.left = pos->x;
			rect.right = pos->x + size->x;
		}
		brush(bool updaterectfrompossize, int2* npos, int2* nsize, float* nrot)
		{
			createrectfrompossize = updaterectfrompossize;
			pos = npos;
			size = nsize;
			offset = new int2(0, 0);
			rot = nrot;
			rect.top = pos->y;
			rect.bottom = pos->y + size->y;
			rect.left = pos->x;
			rect.right = pos->x + size->x;
		}
		brush(D2D_RECT_F nrect, float nrot)
		{
			rot = new float(nrot);
			size = nullptr;
			pos = nullptr;
			offset = new int2(0, 0);
			rect = nrect;
		}
		brush(D2D1_ELLIPSE nelipse, float nrot)
		{
			rot = new float(nrot);
			size = nullptr;
			pos = nullptr;
			offset = new int2(0, 0);
			elipse = nelipse;
		}
	};
}