#pragma once
#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "text.hpp"
#include "X:\PROJECTS\economy\economy\economy.h"
namespace GAME
{
	//extern GUI::core* UI;
}
using namespace GAME;
using namespace D2D1;
using namespace std;
inline std::string _RandomString(int len, ui seed = 0/*won't be used if 0*/)
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
#define RandomString _RandomString
namespace DX2D
{
	extern ID2D1DeviceContext *hwndRT;
	extern ID2D1Factory1* pD2DFactory;
	extern main* DXclass;
	class controls;
	class button;
	class button
	{
		friend class controls;
	private:

	protected:
		bool isenabled = true;
		controls* c = nullptr;
		bool* render = nullptr;
		string text;
		ui ID;
		string name;
		bool textready = false;
		string textnh;
		bool callonrelease = false;
		bool* idenp = nullptr;
	public:
		vector<boost::any> anyvars;
		bool isrendered()
		{
			return *render;
		}
		void renderingoff()
		{
			*render = false;
		}
		void renderingon()
		{
			*render = true;
		}
		void renderingswitch()
		{
			*render = !*render;
		}
		etime lt;
		int targetstate = 0;
		//bool notonmmove = false;
		bool hassubbtns = false; // if true: the int2& value passed to buttonpressfunc will be pointer to this
		void(*buttonpressfunc)(int2&) = nullptr;
		void(*rbuttonpressfunc)(int2&) = nullptr;
		void(*mbuttonpressfunc)(int2&) = nullptr;
		bool callpyscript = false;
		bool backgroundbutton = false; // if true button will only be pressed if no other button is 
		bool callpfunc = true;
		bool callpyonanymmove = false;
		int2* pos = nullptr;
		int2 size;
		//vector<vector<sprite>> s; 
		//vector<vector<brush>> b;
		//sprite disabledsprite;
		//brush disabledbrush;
		//int idlesn = 0;
		//int presssn = 1;
		//int states = 0;
		//int idlebn = 0;
		//int presbsn = 1;
		//int stateb = 0;
		map<int,frame*> f; // you
		vector<frame*> fvec;
		map<int, string> fnamemap;
		int idlen = 0;
		int mousehoverstate = 0;
		int pressn = 1;
		int state = 0;
		bool isdefbutton = false;
		string disabledstrid = "";
		float secpertick = 0.1f;
		frame* mainframe;
		camera* cam;
		button()
		{

		}
		button(string nname, string ntext, controls* nc, frame* nmainframe,map<int,frame*> nf,map<int, string> nfnm,
			void(*nbuttonpressfunc)(int2&), int nidlen, int npressn,int nmousehoverstate, string disabledname, ui nID, bool ncallonrelease , camera* ncam)
		{
			cam = ncam;
			name = name;
			text = ntext;
			c = nc;
			f = nf;
			fnamemap = nfnm;
			buttonpressfunc = nbuttonpressfunc;
			idlen = nidlen;
			pressn = npressn;
			disabledstrid = disabledname;
			ID = nID;
			callpfunc = nbuttonpressfunc != nullptr;
			callonrelease = ncallonrelease;
			mousehoverstate = nmousehoverstate;
			state = idlen;
			targetstate = idlen;
			fvec = MapSecondtoVec(nf);
		}
		~button()
		{

		}
		void Release(bool deleteframe, bool deletepos)
		{
			if (deletepos)
				delete pos;
			if(idenp != nullptr)
				*idenp = false;
			if (render != nullptr)
				*render = false;
			if (deleteframe)
			{
				int i = 0;
				int ii = 0;
				while (ii < f.size())
				{
					if (FindInVec(mainframe->f[mainframe->wchiac], f[ii], i))
						mainframe->f[mainframe->wchiac].erase(mainframe->f[mainframe->wchiac].begin() + i);
					delete f[ii];
				}
			}
			

		}
	};
	extern vector<button*> btnspr;
	extern controls* currentlycheckingc;
	void GetAcB(physobj* obj1, physobj* obj2);
	class controls
	{
		friend class button;
		friend void GetAcB(physobj* obj1, physobj* obj2);
	private:
		physics pclass;
		int2* pobjpos;

	protected:
		vector<frame*> defaultbutton_fv;
		frame* defaultbutton_disabled;
		int defaultbutton_idlei = 0;
		int defaultbutton_msi = 1;
		int defaultbutton_pressi = 2;
		float defaultbutton_secpt = 0.2f;
		map<string, void*> textnames;
		FILE* _callpyloc;
		string _pyscriptname;
		int _testi = 0;
		void callpyscript()
		{
			try
			{
				while (_testi == 51)
					break;
				PyRun_SimpleFile(_callpyloc, _pyscriptname.c_str());
				_testi++;
			}
			catch (...)
			{
				cout << "\b";
				cout << "Critical Error has occured..." << endl << ">";
			}
		}
	public:
		inline physics* GetPhysP()
		{
			return &pclass;
		}
		void(*lmousemovefunc)(int2&) = nullptr;
		void(*rmousemovefunc)(int2&) = nullptr;
		void(*mmousemovefunc)(int2&) = nullptr;
		void(*lbupfunc)(int2&) = nullptr;
		void(*rbupfunc)(int2&) = nullptr;
		void(*mbupfunc)(int2&) = nullptr;
		void(*lbdownfunc)(int2&) = nullptr;
		void(*rbdownfunc)(int2&) = nullptr;
		void(*mbdownfunc)(int2&) = nullptr;
		bool lbuttondown = false;
		bool rbuttondown = false;
		bool mbuttondown = false;
		eclock controlclock;
		textclass tc;
		button* latestcreation = nullptr;
		vector<button*> buttons;
		bool wasinit = false;
		map < ui,button*> buttonsIDmap;
		int2 lastmousepos = {0,0};
		map <string, button*> buttonSTRmap;
		void callpy(button* bt, UINT msg, int2 pos);
		void clearbuttons(textclass& tc)
		{
			int i = 0;
			buttonsIDmap.clear();
			buttonSTRmap.clear();
			while (i < buttons.size())
			{
				buttons[i]->Release(!buttons[i]->isdefbutton, true);
				tc.deltext(buttons[i]->cam, buttons[i]->textnh, false);
				i++;
			}
			
			buttons.clear();
		}
		bool delbutton(string name, bool delpos = true, bool delframe = false )
		{
			int i = 0;
			while (i < buttons.size())
			{
				if (buttons[i]->name == name)
				{
					buttons[i]->Release(delframe, delpos);
					buttons.erase(buttons.begin() + i);
					return true;
				}
				i++;
			}
			return false;
		}
		bool delbutton(button* btn,bool delpos = true, bool delframe = false)
		{
			int i = 0;
			while (i < buttons.size())
			{
				if (buttons[i] == btn )
				{
					buttons[i]->Release(delframe, delpos);
					buttons.erase(buttons.begin() + i);
					return true;
				}
				i++;
			}
			return false;
		}
		bool addbutton(string buttontext,int2* pos, int2 size, vector<frame*> nf, void(*buttonpressfunc)(int2&),int idlei, int pressedi,
			int mousehoverstate,frame* disabledf,bool callonrelease,frame* main/*main is NOT target frame, button will be rendered to new frames inserted to main.f*/,
			camera* cam,textclass& tc,bool stepinmain = false, ui ID = 0/* will be randomised if 0*/,
			string name = ""/*will be randomised if ""*/, int2 basesize = { 0,0 }, textpiece* tp = nullptr /*will be created if nullptr*/, 
			float fontsize = 10.0f, string fontname = "Times New Roman", D2D1_COLOR_F col = { 0.0f,0.0f,0.0f,1.0f })
		{
			if (fontname == "")
				fontname = "Times New Roman";
			if (fontsize == 0.0f)
				fontsize = 10.0f;
			int i = 0;
			uni2<float> mp;
			bool* identp = new bool(true);
			bool* renderp = new bool(true);
			if (basesize.x == 0)
			{
				float mpx;
				int2 six;
				if (nf[0]->brushes.size() > 0)
				{
					six = *nf[0]->brushes[0].size;
				}
				else if (nf[0]->sprites.size() > 0)
				{
					six = nf[0]->sprites[0].GetSize();
				}
				mpx = (float)size.x / (float)six.x;
				mp.x = mpx;
			}
			else
			{
				mp.x = size.x / basesize.x;
			}
			if (basesize.y == 0)
			{
				float mpy;
				int2 siy;
				if (nf[0]->brushes.size() > 0)
				{
					siy = *nf[0]->brushes[0].size;
				}
				else if (nf[0]->sprites.size() > 0)
				{
					siy = nf[0]->sprites[0].GetSize();
				}
				mpy = (float)size.y / (float)siy.y;
				mp.y = mpy;
			}
			else
			{
				mp.y = size.y / basesize.y;
			}
			while (i < nf.size())
			{
				int ii = 0;
				while (ii < nf[i]->sprites.size())
				{
					int2 sz = nf[i]->sprites[ii].GetSize();
					nf[i]->sprites[ii].size.height = sz.y * mp.y;
					nf[i]->sprites[ii].size.width = sz.x * mp.x;
					nf[i]->sprites[ii].offsetbysize();
					nf[i]->sprites[ii].SetXY(/**nf[i]->sprites[ii].GetXYp() +*/ *pos);
					nf[i]->sprites[ii].render = renderp;
					nf[i]->sprites[ii].identp = identp;
					nf[i]->sprites[ii].useidentp = true;
					ii++;
				}
				ii = 0;
				while (ii < nf[i]->brushes.size())
				{
					int2 sz = *nf[i]->brushes[ii].size;
					nf[i]->brushes[ii].size->x = sz.x * mp.x;
					nf[i]->brushes[ii].size->y = sz.y * mp.y;
					nf[i]->brushes[ii].createrectfrompossize = true;
					*nf[i]->brushes[ii].pos = /**nf[i]->brushes[ii].pos+*/*pos;
					nf[i]->brushes[ii].renderp = renderp;
					nf[i]->brushes[ii].identp = identp;
					nf[i]->brushes[ii].useidentp = true;
					ii++;
				}
				i++;
			}
			i = 0;
			if (stepinmain)
			{
				frame* nma = new frame();
				if (main->wchiac == "")
				{
					main->wchiac = "button_" + name;
				}
				main->f[main->wchiac].push_back(nma);
				main->ischfact = true;
				main = nma;
			}
			main->ischfact = true;
			string tnstr = "";
			if (tp == nullptr)
			{
				do
				{
					tnstr = RandomString(3) + "button";
				} while (MapFind(textnames,tnstr));
				textnames.insert(make_pair(tnstr, nullptr));
				tp = tc.addtext(cam, fontname, pos, size, fontsize,tnstr , buttontext,col);
				tp->format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			}
			else
			{
				InsertToMapOfVecs(tc.TMAP, make_pair(cam, tp));
			}
			if (buttonSTRmap.find(name) != buttonSTRmap.end())
				return false;
			if (name == "" || buttonSTRmap.find(name) != buttonSTRmap.end())
			{
				do
				{
					i++;
					name = (string)((INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)));
					if (i > 254 * 254 * 254)
						return false;
				} while (buttonSTRmap.find(name) != buttonSTRmap.end());
			}
			i = 0;
			if (ID == 0 || buttonsIDmap.find(ID) != buttonsIDmap.end())
			{
				do
				{
					i++;
					ID = rand();
					if (i > 1000000)
						return false;
				} while (buttonsIDmap.find(ID) != buttonsIDmap.end());
			}
			i = 0;
			map<int, frame*> sfmp;
			map<int, string> ifmp;
			while (i < nf.size())
			{
				string fname = (string)((INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)));
				fname += INTtoSTR(i) + name;
				main->f.insert(make_pair(fname, vector<frame*>{ nf[i] }));
				sfmp.insert(make_pair(/*ID/2 +*/ i, nf[i] ));
				ifmp.insert(make_pair(/*ID/2 + */i, fname));
				i++;
			}
			string fname = (string)((INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)) + (INTtoSTR(rand() % 254)));
			fname += INTtoSTR(i) + name + "disable";
			
			i = 0;
			bool* bp = new bool(true);
			while (i < nf.size())
			{
				int ii = 0;
				while (ii < nf[i]->brushes.size())
				{
					nf[i]->brushes[ii].renderp = bp;
					ii++;
				}
				ii = 0;
				while (ii < nf[i]->sprites.size())
				{
					nf[i]->sprites[ii].render = bp;
					ii++;
				}
				i++;
			}
			i = 0;
			button* butt = new button(name,buttontext,this,main,sfmp,ifmp,buttonpressfunc,idlei,pressedi, mousehoverstate, fname, ID,callonrelease,cam);
			main->wchiac = ifmp[idlei];
			butt->textnh = tnstr;
			if (butt == nullptr)
			{
				delete bp;
				return false;
			}
			butt->mainframe = main;
			butt->render = bp;
			butt->c = this;
			buttonsIDmap.insert(make_pair(ID, butt));
			buttonSTRmap.insert(make_pair(name, butt));
			buttons.push_back(butt);
			pclass.addobj(name, new int2(size), pos, false, false);
			pclass.objmap[name]->rectID = 666;
			physreact pr;
			pr.noclip = true;
			pclass.objmap[name]->IDreactionmap.insert(make_pair(777, pr));
			pclass.objmap[name]->isron = true;
			latestcreation = butt;
			butt->pos = pos;
			butt->size = size;
			return true;
		}
		bool addbutton(button* butt)
		{
			//main->wchiac = ifmp[idlei];
			//butt->textnh = tnstr;
			/*if (butt == nullptr)
			{
				delete bp;
				return false;
			}*/
			//butt->mainframe = main;
			//butt->render = bp;
			int i = 0;
			ui ID = 0;
			if (ID == 0 || buttonsIDmap.find(ID) != buttonsIDmap.end())
			{
				do
				{
					i++;
					ID = rand();
					if (i > 1000000)
						return false;
				} while (buttonsIDmap.find(ID) != buttonsIDmap.end());
			}
			butt->c = this;
			string name = to_string(ID) + "b";
			buttonsIDmap.insert(make_pair(butt->ID, butt));
			buttonSTRmap.insert(make_pair(name, butt));
			buttons.push_back(butt);
			pclass.addobj(name, new int2(butt->size), butt->pos, false, false);
			pclass.objmap[name]->rectID = 666;
			physreact pr;
			pr.noclip = true;
			pclass.objmap[name]->IDreactionmap.insert(make_pair(777, pr));
			pclass.objmap[name]->isron = true; 
			latestcreation = butt;
			//butt->pos = pos;
			//butt->size = size;
			return true;
		}
		inline bool addbutton(string buttontext, int2* pos, int2 size, void(*buttonpressfunc)(int2&), frame* main, camera* cam, bool callonrelease,
			textclass &tcp, float fontsize = 10.0f, string fontname = "Times New Roman", D2D1_COLOR_F col = {0.0f,0.0f,0.0f,1.0f})
		{
			if (fontname == "")
				fontname = "Times New Roman";
			if (fontsize == 0)
				fontsize = 10.0f;
			bool b = addbutton(buttontext, pos, size, copyvecofptrs<frame>(defaultbutton_fv, &frame::getcopy), buttonpressfunc, defaultbutton_idlei, defaultbutton_pressi, defaultbutton_msi,
				defaultbutton_disabled, callonrelease, main, cam, tcp, true, 0U, "", { 0,0 },nullptr,fontsize, fontname,col);
			buttons[buttons.size() - 1]->isdefbutton = true;
			return b;
		}
		inline void MouseMove(button* b, int2 pos)
		{
			if (b->state == b->idlen)
			{
				b->state = b->mousehoverstate;
				b->targetstate = b->mousehoverstate;
			}
		}
		inline void MouseLMB(button* b, int2 pos)
		{
			lbuttondown = true;
			b->targetstate = b->pressn;
			if (!b->callonrelease)
			{
				if(b->callpfunc && b->buttonpressfunc != nullptr)
					if (!b->hassubbtns)
						b->buttonpressfunc(pos);
					else
					{
						uintptr_t v = reinterpret_cast<uintptr_t>( b);
						ui x = 0, y = 0;
						x = (ui)((v & 0xFFFFFFFF00000000LL) >> 32);
						y = (ui)(v & 0xFFFFFFFFLL);
						int xi, yi;
						//if (x <= INT_MAX)
						//	xi = static_cast<int>(x);
						//else if (x >= INT_MIN)
						//	xi = static_cast<int>(x - INT_MIN) + INT_MIN;
						//if (y <= INT_MAX)
						//	yi = static_cast<int>(y);
						//else if (y >= INT_MIN)
							//yi = static_cast<int>(y - INT_MIN) + INT_MIN;
						xi = x - INT_MAX;
						yi = y - INT_MAX;
						b->buttonpressfunc(int2(xi, yi));
					}
			}
		}
		inline void MouseLMBup(button* b, int2 pos)
		{
			lbuttondown = false;
			b->targetstate = b->mousehoverstate;
			if (b->callonrelease)
			{
				if (b->callpfunc)
					b->buttonpressfunc(pos);
			}
		}
		inline void MouseMMB(button* b, int2 pos)
		{
			mbuttondown = true;
			if (b->mbuttonpressfunc == nullptr)
				return;
			b->targetstate = b->pressn;
			if (!b->callonrelease)
			{
				if (b->callpfunc && b->buttonpressfunc != nullptr)
					b->mbuttonpressfunc(pos);
			}
		}
		inline void MouseMMBup(button* b, int2 pos)
		{
			mbuttondown = false;
			if (b->mbuttonpressfunc == nullptr)
				return;
			b->targetstate = b->mousehoverstate;
			if (b->callonrelease)
			{
				if (b->callpfunc)
					b->mbuttonpressfunc(pos);
			}
		}
		inline void MouseRMB(button* b, int2 pos)
		{
			rbuttondown = true;
			if (b->rbuttonpressfunc == nullptr)
				return;
			b->targetstate = b->pressn;
			if (!b->callonrelease)
			{
				if (b->callpfunc && b->buttonpressfunc != nullptr)
					b->rbuttonpressfunc(pos);
			}
		}
		inline void MouseRMBup(button* b, int2 pos)
		{
			rbuttondown = false;
			if (b->rbuttonpressfunc == nullptr)
				return;
			b->targetstate = b->mousehoverstate;
			if (b->callonrelease)
			{
				if (b->callpfunc)
					b->rbuttonpressfunc(pos);
			}
		}
		void MouseEvent(WPARAM wParam, LPARAM lParam, UINT msg, int2* forcedpos = nullptr);
		void tick()
		{
			if (!wasinit)
				return;
			int i = 0;
			controlclock.tick();
			etime tm = controlclock.gettime();
			while (i < buttons.size())
			{
				if (buttons[i]->isenabled && buttons[i]->mainframe != nullptr)
				{
					buttons[i]->mainframe->wchiac = buttons[i]->fnamemap[buttons[i]->state];
					if (buttons[i]->state != buttons[i]->targetstate)
						if ((buttons[i]->lt - tm).GetMS() <= buttons[i]->secpertick * 1000)
						{
							VecProceed(buttons[i]->state, buttons[i]->fvec);
							buttons[i]->lt = tm;
						}
				}
				else
					buttons[i]->mainframe->wchiac = buttons[i]->disabledstrid;
				i++;
			}
		}
		controls()
		{
		}
		void init()
		{
			tc.init();
			wasinit = true;
			pobjpos = new int2(0, 0);
			pclass.addobj("mouse", new int2(1, 1), pobjpos, false, false);
			pclass.objmap["mouse"]->rectID = 777;
			physreact pr;
			pr.callfuncinstant = true;
			pr.callfunc = true;
			pr.ftocall = &GetAcB;
			pclass.objmap["mouse"]->IDreactionmap.insert(make_pair(666, pr));
			pclass.objmap["mouse"]->isron = true;
			vector<frame*> fv;
			defaultbutton_idlei = 0;
			defaultbutton_msi = 1;
			defaultbutton_pressi = 2;
			defaultbutton_secpt = 0.5f;
			{
				frame* f = new frame;
				brush b(true, new int2{ 0,0 }, new int2{ 100,25 }, new float(0.0f));
				b.SetType(brushtypes::solidbrush);
				hwndRT->CreateSolidColorBrush(ColorF(ColorF::Gray), &b.b.solidbrush.first);
				hwndRT->CreateSolidColorBrush(ColorF(ColorF::Gray), &b.b.solidbrush.second);
				f->brushes.push_back(b);
				fv.push_back(f);
			}
			{
				frame* f = new frame;
				{
					brush b(true, new int2{ 1,1 }, new int2{ 99,24 }, new float(0.0f));
					b.SetType(brushtypes::solidbrush);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::LightGray, 0.0f), &b.b.solidbrush.first);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::Gray), &b.b.solidbrush.second);
					f->brushes.push_back(b);
				}
				{
					brush b(true, new int2{ 2,2 }, new int2{ 98,23 }, new float(0.0f));
					b.SetType(brushtypes::solidbrush);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::LightGray), &b.b.solidbrush.first);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::Gray), &b.b.solidbrush.second);
					f->brushes.push_back(b);
				}
				fv.push_back(f);
			}
			{
				frame* f = new frame;
				{
					brush b(true, new int2{ 0,0 }, new int2{ 100,25 }, new float(0.0f));
					b.SetType(brushtypes::solidbrush);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::LightGray, 1.0f), &b.b.solidbrush.first);
					hwndRT->CreateSolidColorBrush(ColorF(ColorF::LightGray), &b.b.solidbrush.second);
					f->brushes.push_back(b);
				}
				fv.push_back(f);
			}
			defaultbutton_fv = fv;
		}
		~controls()
		{
			if(wasinit)
				delete pobjpos;

		}
	};
}
#undef RandomString