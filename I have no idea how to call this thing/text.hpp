#pragma once
#include "hmain.h"
#include "sprite.h"
#include "camera.h"
#include "brush.h"
namespace DX2D
{
	std::wstring STRtoWSTR(const std::string& s);
	std::string WSTRtoSTR(std::wstring wstr);
	class textpiece
	{
		friend class textclass;
	protected:
		std::wstring wtext;
		DWRITE_FONT_WEIGHT weight;
		DWRITE_FONT_STYLE style;
		bool wasreleased = false;
		DWRITE_FONT_STRETCH stretch;
		const WCHAR* TEXT;
	public:
		int2 *pos;
		int2 size = { 0,0 };
		ID2D1SolidColorBrush* brush;
		float fontsize = 10;
		IDWriteTextFormat* format;
		string name;
		void Render(ID2D1BitmapRenderTarget* RT);
		void changetext(string str)
		{
			wtext = STRtoWSTR(str);
			TEXT = wtext.c_str();
		}
		const WCHAR* GetTextWP()
		{
			return TEXT;
		}
		std::wstring GetTextWSTR()
		{
			return wtext;

		}
		std::string GetTextSTR()
		{
			return WSTRtoSTR(wtext);
		}
		void Release()
		{
			if (!wasreleased)
			{
				delete pos;
				brush->Release();
				format->Release();
				wasreleased = true;
			}
		}
		void Release(bool delpos)
		{
			if (!wasreleased)
			{
				if (delpos)
					delete pos;
				brush->Release();
				format->Release();
				wasreleased = true;
			}
		}
		textpiece()
		{

		}
	};
	class textclass
	{
	protected:
		IDWriteFactory1* idwfac;
	public:
		D2D_COLOR_F defcol;
		std::map<camera*, std::vector <textpiece*>> TMAP;
		textpiece* addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name, string text);
		textpiece* addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name, string text, D2D1_COLOR_F col);
		textpiece* addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name,
			string text, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, D2D_COLOR_F col);
		bool deltext(camera* cam, int index, bool delpos);
		bool deltext(camera* cam, textpiece* tp, bool delpos);
		bool deltext(camera* cam, string name, bool delpos);
		void RenderCam(camera* cam);
		bool init();
		void Release(bool deletetextpieces, bool deletetextpiecespos)
		{
			if (deletetextpieces)
			{
				vector<vector<textpiece*>> tpvv = MapSecondtoVec(TMAP);
				int i = 0;
				while (i < tpvv.size())
				{
					int ii = 0;
					while (ii < tpvv[i].size())
					{
						tpvv[i][ii]->Release(deletetextpiecespos);
						delete tpvv[i][ii];
						ii++;
					}
				}
			}
			idwfac->Release();
			return;
		}
		void clear()
		{
			auto tpvv = MaptoVec(TMAP);
			int i = 0;
			while (i < tpvv.size())
			{
				int ii = 0;
				while (ii < tpvv[i].second.size())
				{
					deltext( tpvv[i].first,tpvv[i].second[ii],false);
					ii++;
				}
				i++;
			}
		}
		textclass()
		{
			init();
		}
		~textclass()
		{
		}
	};
}