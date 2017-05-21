#pragma once
#include "hmain.h"
#define defdbbsizex 150
#define defdbbsizey 60
namespace debugging
{
	class debugwindow;
	class debugvar;
	class debugmain;
	class debugvar
	{
	public:
		string type;
		boost::any val;
		string name;
		HWND hw;
		ui fontsize = 15;
	};

	class debugwindow
	{
	public:
		HWND dbhwnd;
		string name;
		debugmain* dm;
		int2 wsize = { 800,700 };
		vector<debugwindow*> subwindows;
		int4 subwindowsrect = { 0,0,800,350 };
		vector<int2>subposslots;
		vector<debugvar*> vars;
		map<string, debugvar*> vmap;
		int4 dvarrect = { 0,350,800,700 };
		HWND hw = NULL;
		vector<int2>posslots;
		void setvar(boost::any val, string name, bool createifdoesnotexist = false, string type = "INT", ui fontsize = 15);
		bool addvar(boost::any val, string type, string name,ui fontsize = 15);
		void init();
		void placevars();
		void placesubs();
		void update();
		debugwindow* find(HWND hnd)
		{
			if (hw == hnd)
				return this;
			int i = 0;
			while (i < subwindows.size())
			{
				debugwindow* dbp;
				if ((dbp = subwindows[i]->find(hnd)) != nullptr)
				{
					return dbp;
				}
				i++;
			}
			return nullptr;
		}
	};
	class debugmain
	{
	private:
	protected:
		bool wasinit = false;
	public:
		int4 subwindowsrect = { 0,0,800,500 };
		thread* th = nullptr;
		int2 wsize = { 800,500 };
		vector<int2>posslots;
		HWND hwindow;
		int lastsubslottaken = 0;
		vector<debugwindow*> subwindows;
		bool endme = false;
		void init();
		void startmt();
		void addsw(debugwindow* sw, bool push = true);
		void dmain();
	};
}