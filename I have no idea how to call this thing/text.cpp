#include "text.hpp"
using namespace std;
namespace DX2D
{
	extern 	ID2D1DeviceContext *hwndRT;
	std::wstring STRtoWSTR(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	std::string WSTRtoSTR(wstring wstr)
	{
		return string((const char*)&wstr[0], sizeof(wchar_t) / sizeof(char)*wstr.size());
	}
	bool textclass::deltext(camera* cam, int index, bool delpos)
	{
		if (TMAP.find(cam) == TMAP.end())
			return false;
		if (TMAP[cam].size() - 1 < index)
			return false;
		TMAP[cam][index]->Release(delpos);
		TMAP[cam].erase(TMAP[cam].begin() + index);
		if (TMAP[cam].size() == 0)
		{
			TMAP.erase(cam);
		}
		return true;
	}
	bool textclass::deltext(camera* cam, textpiece* tp, bool delpos)
	{
		if (TMAP.find(cam) == TMAP.end())
			return false;
		int i = 0;
		bool foundit = false;
		while (i < TMAP[cam].size())
		{
			if (TMAP[cam][i] == tp)
			{
				foundit = true;
				tp->Release(delpos);
				TMAP[cam].erase(TMAP[cam].begin() + i);
				if (TMAP[cam].size() == 0)
				{
					TMAP.erase(cam);
				}
				break;
			}
			i++;
		}
		return foundit;
	}
	bool textclass::deltext(camera* cam, string name, bool delpos)
	{
		if (TMAP.find(cam) == TMAP.end())
			return false;
		int i = 0;
		bool foundit = false;
		while (i < TMAP[cam].size())
		{
			if (TMAP[cam][i]->name == name)
			{
				foundit = true;
				TMAP[cam][i]->Release(delpos);
				TMAP[cam].erase(TMAP[cam].begin() + i);
				if (TMAP[cam].size() == 0)
				{
					TMAP.erase(cam);
				}
				break;
			}
			i++;
		}
		return foundit;
	}
	void textpiece::Render(ID2D1BitmapRenderTarget* RT)
	{
		RT->DrawText(
			TEXT,
			wtext.size() - 1,
			format,
			D2D1::RectF(pos->x, pos->y, size.x + pos->x, size.y + pos->y),
			brush
		);
	}
	void textclass::RenderCam(camera* cam)
	{
		int i = 0;
		while (i < TMAP[cam].size())
		{
			if (cam == NULL)
				break;
			if (TMAP[cam][i] == NULL)
			{
				i++;
				continue;
			}
			if (TMAP[cam][i]->wasreleased)
			{
				TMAP[cam].erase(TMAP[cam].begin() + i);
			}
			else 
				if(TMAP[cam][i]->wtext != L"")
					TMAP[cam][i]->Render(*cam->GetRenderTargetP());
			i++;
		}
	}
	textpiece* textclass::addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name,
		string text, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, D2D_COLOR_F col)
	{
		textpiece* tp = new textpiece;
		tp->fontsize = fontsize;
		tp->wtext = STRtoWSTR(text);
		tp->TEXT = tp->wtext.c_str();
		tp->weight = weight;
		ID2D1BitmapRenderTarget* RT = *cam->GetRenderTargetP();
		RT->CreateSolidColorBrush(col, &tp->brush);
		tp->style = style;
		tp->stretch = stretch;
		tp->size = size;
		tp->name = name;
		tp->pos = pos;
		idwfac->CreateTextFormat(
			STRtoWSTR(fontname).c_str(),
			NULL,
			weight,
			style,
			stretch,
			fontsize,
			L"", //locale
			&tp->format
		);
		if (TMAP.find(cam) == TMAP.end())
		{
			TMAP.insert(make_pair(cam, vector<textpiece*>{ tp }));
		}
		else
		{
			TMAP[cam].push_back(tp);
		}
		return tp;
	}
	textpiece* textclass::addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name, string text)
	{
		return addtext(cam, fontname, pos, size, fontsize, name, text, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, defcol);
	}
	textpiece* textclass::addtext(camera* cam, string fontname, int2* pos, int2 size, float fontsize, string name, string text, D2D1_COLOR_F col)
	{
		return addtext(cam, fontname, pos, size, fontsize, name, text, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, col);
	}
	bool textclass::init()
	{
		HRESULT hr;
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(idwfac),
			reinterpret_cast<IUnknown **>(&idwfac)
		);
		defcol.a = 1.0f;
		if (FAILED(hr))
			return false;
		return true;
	}
}