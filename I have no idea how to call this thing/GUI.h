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
#define UI_NULL 0
#define UI_OK 0
#define UI_EXCEPTIONTHROWN 1
#define UI_WRONGARG 2
#define UI_UNKNOWN 3
#define UI_UNKNWONERROR 4
#define UI_NULLPTR 5
using namespace std;
namespace GAME
{
	namespace GUI
	{
		namespace deffunc
		{

			map<string, void (*)(int2&)> list = {};
		}
		class UImain;
		class window;
		struct simplewindow;
		class style;
		struct subwindow;
		class core;
		struct simpleshape;
		struct UIresult;
		void loadGUIdata(string binloc, core* c);
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
			common::RGBA* col =nullptr;
			common::RGBA* seccol = nullptr;
			bool hassprite = false;
			sprite ico;
			union /*shapesize*/
			{
				float radius = 0.0f;
				uni2<float> size;
				//shapesize()
				//{
				//	radius = 0.0f;
				//}
			};
			uni2<float> pos;
			bool scaletoscreen = false;
			ui id;
			simpleshape()
			{
				radius = 0.0f;
			}
		};
		struct box
		{
			struct change;
			struct change
			{
				map<string, change*> nodes;
				vector<pair<string, string>> changes; // name-newval
				void save(AZfile &file, string root, string act)
				{
					auto v = file.GetVarNamesInNode(root);
					int i = 0;
					while (i < v.size())
					{
						if (v[i][0] == 'S' && v[i][1] == 'U' && v[i][2] == 'B' && IsNumberCH(v[i][3]))
						{
							string nroot;
							string str = file.GetVar<string>(nroot = root + "@" + v[i]);
							change* ch;
							nodes.insert(make_pair(str,ch = new change));
							ch->save(file, nroot, act);

						}
						else
						{
							string name = v[i];
							string newval = file.GetVar<string>(root + "@" + name);
							changes.push_back(make_pair(name, newval));
						}
						i++;
					}

				}
				
			};
			map<string,change*> rootnode; // first value is action
			simpleshape sh;
			string message = "NULL";
		};
		class style
		{
		private:
		protected:
		public:
			AZfile file;
			string name;
			map<string, common::RGBA*> colnodemap;
			void(*buttonpressfunc)(int2&) = nullptr;
			void(*rbuttonpressfunc)(int2&) = nullptr;
			void(*mbuttonpressfunc)(int2&) = nullptr;
			uni2<float> size;
			unsigned long int defaultflags;
			vector<simpleshape> shapes;
			vector<subwindow> subs;
			vector<box> boxes;
		};
		struct UIresult
		{
			vector<boost::any> retval;
			ui code = 0;
			string strret;
			ui specificcode = 0;
		};
		class core
		{
		private:
		protected:
			ui PreAllocateStyle(string name);
			UIresult addstyle(style st, string name);
		public:
			camera* cam;
			controls* conptr;
			frame* mainframe = nullptr;
			window* root;
			ui scenenum;
			map<ui, window*> wnds;
			map<ui, style> styles;
			map<string, sprite> icons;
			map<string, ui> styleids;
			UIresult AttachTo(window* parent, window* child);
			UIresult NewWindow(window*parent, int2 pos, int2 size, ui styleid, unsigned long int flags = 0x00000000L);
			UIresult NewWindow(window*parent, int2 pos, int2 size, string stylename, unsigned long int flags = 0x00000000L);
			UIresult init(controls* con, camera* ncam, frame* mf = nullptr, ui scenen = 0);
			UIresult addstyle(style st);
			UIresult createstyles(AZfile& f, string binloc,bool add=true);
		};
	}
}