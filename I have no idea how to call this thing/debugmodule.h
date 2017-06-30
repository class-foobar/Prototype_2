#pragma once
#include "hmain.h"
#define defdbbsizex 150
#define defdbbsizey 60
#define CON_NULL -1
#define CON_OK 0
#define CON_EXCTHORWN 1
#define CON_SYNERROR 2
#define CON_INTERNALERROR 3
#define CON_SYSTEMERROR 4
#define CON_FNOTFOUND 5
#define CON_EXIT 6
#define CON_FUNCDEFERROR 7
#define CONSOLEFUNCTION(name) void name(vector<boost::any>args, void*resptr)
#define PREPCFUNC auto result = static_cast<debugconsole::result*>(resptr)
using namespace std;
namespace debugging
{
	namespace sharedv
	{
	}
	namespace imtoolazytodoitproperly
	{
		inline bool isblankch(char ch)
		{
			if (ch == NULL || ch == ' ' || ch == '\t' || ch == '\n')
				return true;
			return false;
		}
		inline bool isblankstr(string str)
		{
			int i = 0;
			if (str == "")
				return true;
			while (i < str.size())
			{
				if (!isblankch(str[i]))
					return false;
			}
			return true;
		}
	}
	class debugwindow;
	class debugvar;
	class debugmain;
	class debugconsole;
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
		debugconsole* console;
		void init();
		void startmt();
		void addsw(debugwindow* sw, bool push = true);
		void dmain();
	};
	class debugconsole
	{
	private:
		
	public:
		bool checkargs = true;
		class result
		{
		private:
		protected:
		public:
			string message = "";
			ui ID = -1;
		};
	protected:
		map<string, void(*)(vector<boost::any>, void*)>funcmap;
		bool active = false;
		bool isthalive = false;
		bool killthread = false;
		void thread_loop();
		result processfunction(string line);
		bool isvis = true;
		thread* th;
		mutex m;
		condition_variable mttw;
		bool winconmode = false;
		inline void write(string text,bool forceenter = true)
		{
			if (text.size() == 0)
				return;
			if (imtoolazytodoitproperly::isblankstr(text))
				return;
			if (text[text.size() - 1] != '\n' && forceenter)
				text += '\n';
			if (winconmode)
			{
				printf(text.c_str());
			}
			else
			{
				//TODO: BUILD IN CONSOLE
			}
		}
		inline string read()
		{
			if (winconmode)
			{
				string str;
				cout << '>';
				getline(cin, str);
				return str;
			}
			else
			{

			}
		}
	public:
		inline bool isvisible()
		{
			return isvis;
		}
		inline void addfunc(string name, void(*func)(vector<boost::any>, void*))
		{
			funcmap.insert(make_pair(name, func));
		}
		inline void addfunc(pair<string, void(*)(vector<boost::any >, void*)>func)
		{
			funcmap.insert(func);
		}
		inline bool hasfunc(string name)
		{
			if (MapFind(funcmap, name))
				return true;
			return false;
		}
		void hide();
		void show();
		void switchwis();
		void init(bool usedefwincon = true, map<string, void(*)(vector<boost::any >, void*)>deffuncmap = {});
		void start();
		void end(bool force=true);
		void release(bool suicide=false);
		void execute(string line);
	};
	namespace debugfunctions
	{
	/*	void help(vector<boost::any>args, void*resptr);
		void getentities(vector<boost::any>args, void*resptr);*/
		//void gcampos(vector<boost::any>args, void*resptr);
		CONSOLEFUNCTION(help);
		CONSOLEFUNCTION(gcampos);
		CONSOLEFUNCTION(getentities);
		CONSOLEFUNCTION(setdbpos);
		CONSOLEFUNCTION(setdbx);
		CONSOLEFUNCTION(setdby);
		CONSOLEFUNCTION(setcampos);
		CONSOLEFUNCTION(setcamy);
		CONSOLEFUNCTION(setcamx);
		CONSOLEFUNCTION(getdbpos);
		CONSOLEFUNCTION(getdbhex);
	}
}