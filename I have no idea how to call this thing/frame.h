#pragma once
#include "hmain.h"
#include "brush.h"
#include "sprite.h"
using namespace std;
using namespace D2D1;
namespace DX2D
{
	struct pixel
	{
		int X;
		int Y;
		ID2D1SolidColorBrush* color;
	};
	struct line
	{
		int2 p1;
		int2 p2;
		ID2D1SolidColorBrush* color;
		bool* renderp = nullptr;
	};
	struct frame
	{
		bool ismactive = true; // is main frame active
		vector <sprite> sprites;
		vector <pixel> freepixels;
		vector <brush> brushes;
		vector<line> lines;
		map <string, vector<frame*>> f;
		bool ischfact = false; // is children frame active
		string wchiac; // which children frame vector is active
		frame* getcopy()
		{
			frame* fr = new frame(*this);
			int i = 0;
			while (i < fr->sprites.size())
			{
				fr->sprites[i].copypointers();
				i++;
			}
			i = 0;
			while (i < fr->brushes.size())
			{
				fr->brushes[i].copypointers();
				i++;
			}
			i = 0;
			while (i < fr->lines.size())
			{
				if(fr->lines[i].renderp != nullptr)
					fr->lines[i].renderp = new bool(*fr->lines[i].renderp);
				i++;
			}
			i = 0;
			auto fvec = MaptoVec(f);
			while (i < fvec.size())
			{
				int ii = 0;
				while (ii < fvec[i].second.size())
				{
					fvec[i].second[ii] = fvec[i].second[ii]->getcopy();
					ii++;
				}
			}
			i = 0;
			return fr;
		}
	};
}