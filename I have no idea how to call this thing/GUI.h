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
#define WF_NULL					(0 << 0)
#define WF_SHOW					(1 << 0)
#define WF_HIDE					(1 << 1)
#define WF_SCALETO_VH			(1 << 2)
#define WF_SCALETO_V			(1 << 3)
#define WF_SCALETO_H			(1 << 4)
#define WF_PROPORTIONALPOS		(1 << 5)
#define WF_SAMEPOS				(1 << 6)
#define WF_NOVINIT				(1 << 7)
#define AT_NULL					(0 << 0)
#define AT_VINIT				(1 << 0)
#define AT_STYLEPROPPOS			(1 << 1)
#define AT_PROPPOS				(1 << 2)
#define AT_SAMEPOS				(1 << 3)

using namespace std;
namespace GAME
{
	namespace GUI
	{
		namespace deffunc
		{
			extern map<string, void (*)(int2&)> list;
		}
		class UImain;
		class window;
		struct simplewindow;
		class style;
		struct subwindow;
		class core;
		struct simpleshape;
		struct UIresult;
		void loadui(string binloc, core*c);
		void loadGUIdata(string binloc, core* c);
		struct subwindow
		{
			uni2<float> pos;
			uni2<float> sizemp;
			bool sharedid = false;
			unsigned long int defaultflags;
		};
		class window
		{
		private:
		protected:
			bool hidden = false;
		public:
			list<pair<string, string>> activescripts;
			string strname = "NULL"; // optional
			void updatenesting();
			void updatepos(int2 oldpos);
			void(*buttonpressfunc)(int2&) = nullptr;
			void(*rbuttonpressfunc)(int2&) = nullptr;
			void(*mbuttonpressfunc)(int2&) = nullptr;
			map<string, pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>*> colnodes;
			button* bt;
			vector<button*> btts;
			controls con;
			uni2<float> defmultip;
			map<ui, window*> children;
			ui ID;
			int nestingc = 0;
			window* parent;
			core* coreptr;
			ui customdetID = 0;
			bool usecustomdetID = false;
			ui styleid;
			unsigned long int flags;
			uni2<float> proppos;
			uni2<float> defpos;
			uni2<float>size;
			bool* identp = nullptr;
			int2* pos;
			int2 nooffpos;
			vector<pair<int2*,int2>> posvec;
			vector<int2> cufoffsets;
			frame* f = nullptr;
			frame* sf = nullptr;
			vector<bool*> shaperb;
			UIresult initvis(style& s, uni2<float> npos, uni2<float> screenmultip);
			UIresult scale(int2 nsize);
			UIresult show();
			UIresult hide();
			UIresult switchvis();
			vector<void*> otherobjts;
			map<string, pair<string, boost::any>> memory;
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
					auto v = file.GetVarNamesInNode(root+'@');
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
			map<string, string> inputmap;
			map<string,change*> rootnode; // first value is action
			simpleshape sh;
			string message = "NULL";
		};
		class style
		{
		private:
		protected:
		public:
			ui id;
			AZfile file;
			string name;
			map<string, common::RGBA*> colnodemap;
			void(*buttonpressfunc)(int2&) = nullptr;
			void(*rbuttonpressfunc)(int2&) = nullptr;
			void(*mbuttonpressfunc)(int2&) = nullptr;
			uni2<float> size = {1.0f,1.0f};
			//map<string, ui> flags;
			//unsigned long int defaultflags;
			map<string, bool> flags;
			map<string, string> msgproc;
			map<string, bool> defaultflags;
			bool procondefault = false;
			vector<simpleshape> shapes;
			vector<subwindow> subs;
			vector<box> boxes;
			vector<textpiece*> textboxes;
			string flagproc = "";
			string initproc = "";
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
			map<string, vector<boost::any>> args;
			AZfile gameuif;
			AZfile stylef;
			camera* cam;
			string bslink;
			controls* conptr;
			frame* mainframe = nullptr;
			window* root;
			ui scenenum;
			bool isargbmodified = false;
			map<ui, window*> wnds;
			map<string, ui> wndnamemap;
			map<ui, style> styles;
			map<string, sprite> icons;
			map<string, ui> styleids;
			void MouseEvent(WPARAM wParam, LPARAM lParam, UINT msg);
			UIresult AttachTo(window* parent, window* child, unsigned long int flags = AT_NULL);
			//UIresult NewWindow(window*parent, int2 pos, int2 size, ui styleid, unsigned long int flags = 0x00000000L);
			//UIresult NewWindow(window*parent, int2 pos, int2 size, string stylename, unsigned long int flags = 0x00000000L);
			UIresult NewWindow(window*parent, uni2<float> pos, uni2<float> size, ui styleid, unsigned long int flags = 0x00000000L, string strname = "NULL");
			UIresult NewWindow(window*parent, uni2<float> pos, uni2<float> size, string stylename, unsigned long int flags = 0x00000000L, string strname = "NULL");
			UIresult init(controls* con, camera* ncam, frame* mf = nullptr, ui scenen = 0);
			UIresult addstyle(style st);
			UIresult createstyles(AZfile& f, string binloc,bool add=true);
		};
	}
}