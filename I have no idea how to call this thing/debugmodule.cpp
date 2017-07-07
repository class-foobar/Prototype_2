#include "hmain.h"
#include "debugmodule.h"
#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "text.hpp"
using namespace std;
namespace GAME
{
	extern vector<entity> entitylist;
}
namespace debugging
{
	int2** debugpos = nullptr;
	namespace sharedv
	{
		camera** maincam;
	}
	namespace debugfunctions
	{
		map<string, void(*)(vector<boost::any >, void*)> defaultm =
		{
			{ "help",&help },
			{ "getentities",&getentities },
			{ "campos",&gcampos},
			{ "setdbpos",&setdbpos},
			{ "setdbx",&setdbx },
			{ "setdby",&setdby },
			{ "setcampos",&setcampos },
			{ "setcamx",&setcamx },
			{ "setcamy",&setcamy },
			{ "getdbpos",&getdbpos},
			{ "getdbhex",&getdbhex},
			{ "mdbx",&mdbx },
			{ "mdby",&mdby }
		};
		map<string, vector<string>> customdef = {};
		vector<string> getconfuncdata(string name)
		{
			string i = typeid(int).name();
			string s = typeid(string).name();
			string f = typeid(float).name();
			map<string, vector<string>> def =
			{
				{ "help",{ "!" + s } },
				{ "getentities",{} },
			};
			auto tot = _SumMaps(def, customdef);
			if (MapFind(tot, name))
				return tot[name];
			else
				return { "error" };
		}
		int areconargscorrect(vector<boost::any> &args, string name)
		{
			int i = 0;
			vector<string> data = getconfuncdata(name);
			if (data.size() == 0)
				return CON_OK;
			if (data == vector<string>{"error"})
				return CON_NULL;
			int ii = data.size();
			while (i < data.size())
			{
				if (data[i][0] == '!')
					ii--;
				i++;
			}		
			if (args.size() < ii)
				return CON_SYNERROR;
			i = 0;
			string _i = typeid(int).name();
			string _s = typeid(string).name();
			string _f = typeid(float).name();
			while (i < data.size())
			{
				bool isoptional = false;
				if (data[i][0] == '!')
				{
					i++;
					continue;
				}
				string str = data[i];
				data[i].erase(0);
				try
				{
					if (data[i] == _i)
					{
						boost::any_cast<int> (args[i]);
					}
					else if (data[i] == _f)
					{
						boost::any_cast<float>(args[i]);
					}
					else
					{
						boost::any_cast<string>(args[i]);
					}
				}
				catch (boost::bad_any_cast)
				{
					if (!isoptional)
						return CON_EXCTHORWN;
				}
				i++;
			}
			return CON_OK;
		}
		map<string, string> extendedcondef =
		{
			{ "help", "Displays either list of all functions or information about specific function\nArguments:\n(optional)[string]function name" },
			{ "getentities", "Get list of all entities\nArguments:\n(none)" }
		};
		CONSOLEFUNCTION(mdbx)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = "Position has been changed";
				result->ID = CON_OK;
				(*debugpos)->x += boost::any_cast<int>(args[0]);
			}
		}
		CONSOLEFUNCTION(mdby)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = "Position has been changed";
				result->ID = CON_OK;
				(*debugpos)->y += boost::any_cast<int>(args[0]);
			}
		}
		CONSOLEFUNCTION(getdbhex)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				stringstream ss;
				ss << *debugpos;
				result->message = ss.str();
				result->ID = CON_OK;
			}
		}
		CONSOLEFUNCTION(getdbpos)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = (*debugpos)->str();
				result->ID = CON_OK;
			}
		}
		CONSOLEFUNCTION(setdbpos)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = "Position has been changed";
				result->ID = CON_OK;
				(*debugpos)->x = boost::any_cast<int>(args[0]);
				(*debugpos)->y = boost::any_cast<int>(args[1]);
			}
		}
		CONSOLEFUNCTION(setdbx)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = "Position has been changed";
				result->ID = CON_OK;
				(*debugpos)->x = boost::any_cast<int>(args[0]);
			}
		}
		CONSOLEFUNCTION(setdby)
		{
			PREPCFUNC;
			if (debugpos == nullptr || *debugpos == nullptr)
			{
				result->ID = CON_NULL;
				result->message = "No variable attached";
			}
			else
			{
				result->message = "Position has been changed";
				result->ID = CON_OK;
				(*debugpos)->y = boost::any_cast<int>(args[0]);
			}
		}
		CONSOLEFUNCTION(setcampos)
		{
			PREPCFUNC;
			result->message = "Position has been changed";
			result->ID = CON_OK;		
			(*sharedv::maincam)->SetX(boost::any_cast<int>(args[0]));
			(*sharedv::maincam)->SetY(boost::any_cast<int>(args[1]));
		}
		CONSOLEFUNCTION(setcamx)
		{
			PREPCFUNC;
			result->message = "Position has been changed";
			result->ID = CON_OK;
			(*sharedv::maincam)->SetX(boost::any_cast<int>(args[0]));
		}
		CONSOLEFUNCTION(setcamy)
		{
			PREPCFUNC;
			result->message = "Position has been changed";
			result->ID = CON_OK;
			(*sharedv::maincam)->SetY( boost::any_cast<int>(args[0]));
		}
		//void gcampos(vector<boost::any>args, void*resptr)
		CONSOLEFUNCTION(gcampos)
		{
			auto result = static_cast<debugconsole::result*>(resptr);
			result->ID = CON_OK;
			result->message = (*(*sharedv::maincam)->GetXYp()).str();
			return;
		}
		//void help(vector<boost::any>args, void*resptr)
		CONSOLEFUNCTION(help)
		{
			auto result = static_cast<debugconsole::result*>(resptr);
			string str = "";
			if (args.size() == 0)
			{
				vector<string> data = MapFirsttoVec(defaultm);
				int i = 0;
				while (i < data.size())
				{
					if(data[i] != "help")
						str += data[i] + '\n';
					i++;
				}
				result->ID = CON_OK;
			}
			else
			{
				string name = tolower(boost::any_cast<string>(args[0]));
				if (MapFind(extendedcondef, name))
				{
					str = extendedcondef[name];
					result->ID = CON_OK;
				}
				else
				{
					str = "Function exists, but no definition found";
					result->ID = CON_NULL;
				}
			}
			result->message = str;
		}
		//void getentities(vector<boost::any>args, void*resptr)
		CONSOLEFUNCTION(getentities)
		{
			auto result = static_cast<debugconsole::result*>(resptr);
			string str;
			int i = 0;
			while (i < GAME::entitylist.size())
			{
				str += "name: " + GAME::entitylist[i].entname + ", pos: ";
				if (GAME::entitylist[i].pos == nullptr)
				{
					str += "NULL\n";
				}
				else
				{
					str += GAME::entitylist[i].pos->str() + "\n";
				}
				i++;
			}
			result->message = str;
			result->ID = CON_OK;
		}
	}
	extern debugmain *dbm;
	string getstrfromtnv(string type, string name, boost::any val)
	{

		string tnv = "";
		tnv += name + ": ";
		stringstream f;
		bool usestr = false;
		string str = "";
		if (type == "INT" || type == "INT32")
		{
			f << boost::any_cast<int&>(val) << '\n';
		}
		else if (type == "UINT" || type == "UINT32")
		{
			f << boost::any_cast<ui&>(val) << '\n';
		}
		else if (type == "INT64")
		{
			f << boost::any_cast<lli&>(val) << '\n';
		}
		else if (type == "UINT64")
		{
			f << boost::any_cast<ulli&>(val) << '\n';
		}
		else if (type == "INT128")
		{
			f << boost::any_cast<mp::int128_t&>(val) << '\n';
		}
		else if (type == "UINT128")
		{
			f << boost::any_cast<mp::uint128_t&>(val) << '\n';
		}
		else if (type == "CHAR")
		{
			f << boost::any_cast<char&>(val) << '\n';
		}
		else if (type == "WCHAR")
		{
			f << boost::any_cast<wchar_t&>(val) << '\n';
		}
		else if (type == "BOOL")
		{
			f << boost::any_cast<bool&>(val) << '\n';
		}
		else if (type == "FLOAT")
		{
			f << boost::any_cast<float&>(val) << '\n';
		}
		else if (type == "DOUBLE")
		{
			f << boost::any_cast<double&>(val) << '\n';
		}
		else if (type == "STR")
		{
			usestr = true;
			string str0 = (string) "\"" + boost::any_cast<string&>(val) + (string) "\"" + "\n";
			str += str0;
		}
		else if (type == "WSTR")
		{
			usestr = true;
			const std::wstring ws = boost::any_cast<wstring&>(val);
			const std::locale locale("");
			typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
			const converter_type& converter = std::use_facet<converter_type>(locale);
			std::vector<char> to(ws.length() * converter.max_length());
			std::mbstate_t state;
			const wchar_t* from_next;
			char* to_next;
			const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
			const std::string s(&to[0], to_next);
			str += s;
		}
		else if (type == "INT2")
		{
			usestr = true;
			int2 i2 = boost::any_cast<int2&>(val);
			str += "(" + INTtoSTR(i2.x) + ", " + INTtoSTR(i2.y) + ")";
		}
		else if (type == "INT4")
		{
			usestr = true;
			int4 i4 = boost::any_cast<int4&>(val);
			str += "(" + INTtoSTR(i4.x) + ", " + INTtoSTR(i4.y) + ", " + INTtoSTR(i4.z) + ", " + INTtoSTR(i4.w) + ")";
		}
		else if (type == "FLOAT2" || type == "UNI2 FLOAT")
		{
			usestr = true;
			uni2<float> u2 = boost::any_cast<uni2<float>&>(val);
			str += "(" + to_string( u2.x) + ", " + to_string(u2.y) + ")";
		}
		else if (type == "DOUBLE2" || type == "UNI2 DOUBLE")
		{
			usestr = true;
			uni2<double> u2 = boost::any_cast<uni2<double>&>(val);
			str + "(" + to_string(u2.x) + ", " + to_string(u2.y) + ")";
		}
		string fstr = "";
		if (usestr)
			fstr = str;
		else 
			f >> fstr;
		if (fstr[fstr.length() - 1] == '\n')
			fstr.erase(fstr.begin() + fstr.length() - 1);
		tnv += fstr;
		return tnv;
	}
	void debugwindow::update()
	{
		int i = 0;
		while (i < subwindows.size())
		{
			subwindows[i]->update();
			i++;
		}
		i = 0;
		while (i < vars.size())
		{
			SetWindowText(vars[i]->hw, getstrfromtnv(vars[i]->type, vars[i]->name, vars[i]->val).c_str());
			ShowWindow(vars[i]->hw, SW_SHOW);
			i++;
		}
	}
	void debugmain::addsw(debugwindow* sw, bool push )
	{
		if (push)
		{
			subwindows.push_back(sw);
		}
		sw->dm = this;
		int i = 0;
		i = lastsubslottaken;
		while (i < subwindows.size())
		{
			subwindows[i]->dm = this;
			subwindows[i]->init();

			subwindows[i]->hw = CreateWindowExA(0, "BUTTON", subwindows[i]->name.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				posslots[i].x, posslots[i].y, /*(subwindowsrect.z - subwindowsrect.x) / subwindows.size()*/defdbbsizex,
				defdbbsizey/*(subwindowsrect.w - subwindowsrect.y) / subwindows.size()*/, hwindow, (HMENU)i, GetModuleHandle(0), 0);
			i++;
		}
		sw->init();

	}
	LRESULT CALLBACK msgproc(HWND hwndwin, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		bool ismain = (dbm->hwindow == hwndwin);
		switch (msg)
		{
		case WM_CREATE:
		{
			SetTimer(hwndwin, 666, 1000, NULL);
			break;
		}
		case WM_TIMER:
		{
			if (ismain)
			{
				int i = 0;
				while (i < dbm->subwindows.size())
				{
					dbm->subwindows[i]->update();
					i++;
				}
			}
			break;
		}
		case WM_COMMAND:
		{
			int ilw = LOWORD(wParam);
			if (ismain)
			{
				ShowWindow(dbm->subwindows[ilw]->hw,SW_SHOW);
			}
			else
			{
				int i = 0;
				debugwindow* db = nullptr;
				while (i < dbm->subwindows.size())
				{
					if ((db = dbm->subwindows[i]->find(hwndwin)) != nullptr)
						break;
					i++;
				}
				if (db != nullptr)
				{
					DestroyWindow(db->subwindows[ilw]->hw);
					db->subwindows[ilw]->init();
					bool b = ShowWindow(db->subwindows[ilw]->hw, SW_SHOW);
				}
			}
			break;
		}
		case WM_CLOSE:
		case WM_DESTROY:
		{
			if (ismain)
			{
				ShowWindow(dbm->hwindow, SW_HIDE);
			}
			else
			{
				int i = 0;
				debugwindow* db = nullptr;
				while (i < dbm->subwindows.size())
				{
					if ((db = dbm->subwindows[i]->find(hwndwin)) != nullptr)
						break;
					i++;
				}
				if (db != nullptr)
				{
					ShowWindow(db->dbhwnd, SW_HIDE);
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwndwin, msg, wParam, lParam);
		}
	}
	void debugwindow::placesubs()
	{
		int i = 0;
		subposslots = {};
		int2 prev = { subwindowsrect.x, subwindowsrect.y };
		while (i < subwindows.size())
		{
			subposslots.push_back(prev);
			//if (prev + int2{ (int)(subwindowsrect.z - subwindowsrect.x) / (int)subwindows.size(),(int)(subwindowsrect.w - subwindowsrect.y) / (int)subwindows.size() } >
			//	int2{ (int)subwindowsrect.z,(int)subwindowsrect.w })
			//{
			//	prev.x = subwindowsrect.x;
			//	prev.y += (subwindowsrect.w - subwindowsrect.y) / subwindows.size();
			//}
			if (int2(prev + int2{ defdbbsizex,defdbbsizey }).x > int2{ (int)subwindowsrect.z,(int)subwindowsrect.w }.x)
			{
				prev.x = subwindowsrect.x;
				prev.y += defdbbsizey/*(subwindowsrect.w - subwindowsrect.y) / subwindows.size()*/;
			}
			else
				prev.x += defdbbsizex;
			i++;
		}
		i = 0;
		while (i < subwindows.size())
		{
			subwindows[i]->dm = dm;
			subwindows[i]->init();

			subwindows[i]->hw = CreateWindowExA(0, "BUTTON", subwindows[i]->name.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				subposslots[i].x, subposslots[i].y, defdbbsizex, defdbbsizey,hw, (HMENU)i, GetModuleHandle(0), 0);
			i++;
		}
		i = 0;
	}
	void debugwindow::placevars()
	{

		int i = 0;
		while (i < vars.size())
		{
			if (vars[i]->hw != NULL)
				DestroyWindow(vars[i]->hw);
			i++;
		}
		i = 0;
		while (i < vars.size())
		{
			if (posslots.size() == i)
			{
				this->init();
			}
			string &type = vars[i]->type;
			boost::any &val = vars[i]->val;
			string tnv = getstrfromtnv(type, vars[i]->name, val);
			int2 size = { (int)(dvarrect.z - dvarrect.x) / (int)vars.size(), (int)(dvarrect.w - dvarrect.y) / (int)vars.size() };
			vars[i]->hw = CreateWindowExA(0, "STATIC", tnv.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				posslots[i].x, posslots[i].y, defdbbsizex, defdbbsizey, hw,
				(HMENU)i, GetModuleHandle(0), 0);
			HFONT hFont = CreateFont(vars[i]->fontsize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
			SendMessage(vars[i]->hw, WM_SETFONT, WPARAM(hFont), TRUE);
			i++;
		}
	}
	bool debugwindow::addvar(boost::any val, string type, string name, ui fontsize )
	{
		debugvar* dv = new debugvar;
		dv->val = val;
		dv->name = name;
		dv->type = type;
		dv->fontsize = fontsize;
		vars.push_back(dv);
		vmap.insert(make_pair(name, dv));
		return true;
	}
	void debugwindow::setvar(boost::any val, string name,bool createifdoesnotexist ,string type, ui fontsize )
	{
		if (MapFind(vmap, name))
		{
			vmap[name]->val = val;
			if(vmap[name]->hw == NULL)
				placevars();
		}
		else if(createifdoesnotexist)
		{
			addvar(val,type,name,fontsize);
			placevars();

		}
	}
	void debugwindow::init()
	{
		//dbhwnd = dm->hwindow;
		hw = CreateWindowExA(0, "dbc", name.c_str(),
			WS_OVERLAPPEDWINDOW, rand()%200, rand()%200,
			wsize.x, wsize.y, 0, 0, GetModuleHandle(0), 0);
		ShowWindow(hw, SW_HIDE);
		int i = 0;
		while (i < subwindows.size())
		{
			subwindows[i]->dm = dm;
			subwindows[i]->init();
			i++;
		}
		i = 0;
		int2 prev = { dvarrect.x,dvarrect.y };
		posslots = {};
		while (prev.y + defdbbsizey <= dvarrect.w)
		{
			posslots.push_back(prev);
			if (int2(prev + int2{ defdbbsizex,defdbbsizey }).x > int2{ (int)dvarrect.z,(int)dvarrect.w }.x)
			{
				prev.x = dvarrect.x;
				prev.y += defdbbsizey;
			}
			else 
				prev.x += defdbbsizex;
			i++;
		}
		placevars();
		placesubs();
	}
	void debugmain::init()
	{
		if (wasinit)
			return;
		else
			wasinit = true;
		if(console == nullptr )
			console = new debugconsole;
		const char* const myclass = "dbc";
		WNDCLASSEXA wndclass = { sizeof(WNDCLASSEX),CS_DBLCLKS, NULL,
			0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
			LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
			0, myclass, LoadIcon(0,IDI_APPLICATION) };
		wndclass.lpfnWndProc = msgproc;
		if (RegisterClassEx(&wndclass))
		{
			hwindow = CreateWindowExA(0, myclass, "Debug",
				WS_OVERLAPPEDWINDOW, 0, 0,
				800, 500, 0, 0, GetModuleHandle(0), 0);
			ShowWindow(hwindow, SW_HIDE);
		}
		else
			DebugBreak();
		int i = 0;
		int2 prev = { subwindowsrect.x, subwindowsrect.y };
		while (prev.y + defdbbsizey <= subwindowsrect.w)
		{
			posslots.push_back(prev);
			if (int2(prev + int2{ defdbbsizex,defdbbsizey }).x > int2{ (int)subwindowsrect.z,(int)subwindowsrect.w }.x)
			{
				prev.x = subwindowsrect.x;
				prev.y += defdbbsizey/*(subwindowsrect.w - subwindowsrect.y) / subwindows.size()*/;
			}
			else
				prev.x += defdbbsizex;

			i++;
		}

		i = 0;
		while (i < subwindows.size())
		{
			subwindows[i]->dm = this;
			subwindows[i]->init();
			subwindows[i]->hw = CreateWindowExA(0, "BUTTON", subwindows[i]->name.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
				posslots[i].x, posslots[i].y, /*(subwindowsrect.z - subwindowsrect.x) / subwindows.size()*/defdbbsizex,
				defdbbsizey/*(subwindowsrect.w - subwindowsrect.y) / subwindows.size()*/, hwindow, (HMENU)i, GetModuleHandle(0), 0);
			i++;
		}
		lastsubslottaken = i;
		i = 0;
	}
	void debugmain::dmain()
	{
		init();
		int i = 0;
		while (i < subwindows.size())
		{
			addsw(subwindows[i], false);
			i++;
		}
		i = 0;
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) && !endme)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		delete th;
		return;
	}
	void debugmain::startmt()
	{
		th = new thread(&debugmain::dmain, this);
	}
	void debugconsole::thread_loop()
	{
	lock:;
		unique_lock<std::mutex> lk(m);
		mttw.wait(lk);
	nolock:;
		while (!active&&!killthread);
		if (killthread)
		{
			return;
		}
		while (active)
		{
			string message = read();
			if (imtoolazytodoitproperly::isblankstr(message))
				continue;
			auto result = processfunction(message);
			if (result.ID == CON_EXIT)
			{
				write(result.message);
				write("exiting...");
				Sleep(2);
				isthalive = false;
				active = false;
				return;
			}
			if (result.ID > 0)
			{
				write("An error has occured:");
				write(result.message);
			}
			else
			{
				write(result.message);
			}
		}
		if (killthread)
		{
			return;
		}
	}

	debugconsole::result debugconsole::processfunction(string line)
	{
		vector<boost::any> args = {};
		string fname = "";
		result res;
		int i = 0;
		res.ID = CON_NULL;
		while (!imtoolazytodoitproperly::isblankch(line[i]) && i < line.size())
			fname += line[i++];
		if ((i < line.size()))
		{
			vector <string>strarg;
			string str = "";
			do
			{
				str = "";
				while (imtoolazytodoitproperly::isblankch(line[i]) && i < line.size())
					i++;
				while (!imtoolazytodoitproperly::isblankch(line[i]) && i < line.size())
					str += line[i++];
				if (str != "")
					strarg.push_back(str);

			} while (str != "");
			i = 0;
			while (i < strarg.size())
			{
				if (IsNumberSTR(strarg[0], true))
				{
					int ii = 0;
					bool b = false;
					while (ii < str.size())
					{
						if (str[i] == '.')
						{
							b = true;
							break;
						}
						ii++;
					}
					if (b)
					{
						args.push_back(stof(strarg[i]));
					}
					else
					{
						args.push_back(stoi(strarg[i]));
					}
				}
				else
				{
					args.push_back(strarg[i]);
				}
				i++;
			}

		}
		fname = tolower(fname);
		if (MapFind(funcmap, fname))
		{
			if (checkargs)
			{
				if (debugfunctions::areconargscorrect(args, fname) < 1)
				{
					(funcmap[fname](args, (void*)&res));
				}
				else
				{
					res.ID = CON_SYNERROR;
					res.message = "Error, wrong arguments\nThe correct arguments are:\n";
					int i = 0;
					vector<string> data = debugfunctions::getconfuncdata(fname);
					while (i < data.size())
					{
						string str = data[i];
						if (str[0] == '!')
						{
							str.erase(0);
							str = "(optional)" + str;
						}
						res.message += str;
						i++;
					}
				}
			}
			else 
				(funcmap[fname](args, (void*)&res));
		}
		else
		{
			res.ID = 5;
			res.message = "function not found";
		}
		return res;
	}

	void debugconsole::hide()
	{
		//TODO hide/show
	}

	void debugconsole::show()
	{

	}
	void debugconsole::switchwis()
	{
		if (isvis)
			hide();
		else
			show();
		isvis = !isvis;
	}

	void debugconsole::init(bool usedefwincon, map<string, void(*)(vector<boost::any >,void*)>deffuncmap)
	{
		th = new thread(&debugconsole::thread_loop, this);
		winconmode = usedefwincon;
		isthalive = true;
		active = false;
		killthread = false;
		funcmap = _SumMaps(deffuncmap, debugfunctions::defaultm);
	}

	void debugconsole::start()
	{
		active = true;
		mttw.notify_one();
	}

	void debugconsole::end(bool force)
	{
		if (!isthalive)
			return;
		active = false;
		if (force)
			th->join();
		killthread = true;
	}

	void debugconsole::release(bool suicide)
	{
		end(true);
		delete th;
		if (suicide)
			delete this;
	}

	void debugconsole::execute(string line)
	{
		processfunction(line);
	}

}