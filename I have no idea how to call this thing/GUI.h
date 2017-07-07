#pragma once
#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "pathfinding.h"
#include "text.hpp"
#include "debugmodule.h"
using namespace std;
namespace GAME
{
	namespace GUI
	{
		class UImain;
		class window;
		struct simplewindow;
		class style;
		struct subwindow;
		struct simpleshape;
		struct UIresult;
		struct subwindow
		{
			int2 pos;
			simpleshape sh;
			uni2<float> sizemp;
			bool sharedid = false;
			unsigned long int defaultflags;
		};
		class window
		{
		private:
		protected:
		public:
			void(*buttonpressfunc)(int2&) = nullptr;
			void(*rbuttonpressfunc)(int2&) = nullptr;
			void(*mbuttonpressfunc)(int2&) = nullptr;
			button* bt;
			map<ui, window*> children;
			ui ID;
			int nestingc = 0;
			window* parent;
			core* coreptr;
			ui customdetID = 0;
			bool usecustomdetID = false;
			unsigned long int flags;
			UIresult scale(int2 nsize);
		};
		struct simpleshape
		{
			ui shape;
			bool gradient;
			common::RGBA icol;
			common::RGBA ocol;
			common::RGBA seccol;
			union
			{
				float radius;
				uni2<float> size;
			};
			uni2<float> pos;
			bool scaletoscreen = false;
		};
		class style
		{
		private:
		protected:
		public:
			void(*buttonpressfunc)(int2&) = nullptr;
			void(*rbuttonpressfunc)(int2&) = nullptr;
			void(*mbuttonpressfunc)(int2&) = nullptr;
			uni2<float> size;
			unsigned long int defaultflags;
			vector<simpleshape> shapes;
			vector<subwindow> subs;
		};
		struct UIresult
		{
			boost::any retval;
			ui code = 0;
			string strret;
			ui specificcode = 0;
		};
		class core
		{
		private:
		protected:
		public:
			window* root;
			map<ui, window*> wnds;
			map<ui, style> styles;
			map<string, ui> styleids;
			UIresult AttachTo(window* parent, window* child);
			UIresult NewWindow(window*parent, int2 pos, int2 size, ui styleid, unsigned long int flags = 0x00000000L);
			UIresult NewWindow(window*parent, int2 pos, int2 size, string stylename, unsigned long int flags = 0x00000000L);
			UIresult init(controls* con, camera* cam, frame* mf = nullptr, ui scenen = 0);
			UIresult addstyle(style st);
			UIresult createstyle(AZfile& f);
		};
	}
}