#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "text.hpp"
#include "GUI.h"
using namespace GAME;
using namespace D2D1;
using namespace std;
using namespace GAME::GUI;
namespace GAME
{
	extern core* UI;
	vector<pair<thread*,bool*>>scriptthreads;
	mutex scriptthreadmutex;
	deque<pycall*> calls;
	namespace GUI
	{
		extern thread* curth;
	}
}
namespace DX2D
{
	vector<button*> btnspr;
	controls* currentlycheckingc;
	void GetAcB(physobj* obj1, physobj* obj2)
	{
		physobj* obj = nullptr;
		if (obj1->nameindex != "mouse")
			obj = obj1;
		else if (obj2->nameindex != "mouse")
			obj = obj2;
		if (obj != nullptr)
			btnspr.push_back(currentlycheckingc->buttonSTRmap[obj->nameindex]);
	}
	void controls::callreinit()
	{
		UI->reinit();
	}
	void controls::callpyscript(string strid, string loc, string fname, void* callptr)
	{
		pycall* call = static_cast<pycall*>  (callptr);
		FILE* _f = NULL;
		_f = _Py_fopen(loc.c_str(), "r");
		if (loc == "")
			loc = fname;
		UI->argmodmutex->lock();
		UI->idmapmutex.lock();
		UI->processtridmap.insert(make_pair(getpid(), strid));
		UI->idmapmutex.unlock();
		UI->argmodmutex->unlock();
		//try
		//{
		if (fname == "")
		{
			fname = reverseSTR(copytochfromstart(reverseSTR(loc), '/'));
			if(fname == loc)
				fname = reverseSTR(copytochfromstart(reverseSTR(loc), '\\'));
		}
#ifndef nopydebugoutput
		string str = loc + ", " + " strid: " + strid + "\n";
		OutputDebugStringA(str.c_str());
#endif // !nopydebugoutput
		if (fname.find("\\") != string::npos || fname.find("/") != string::npos)
		{
			string fnameold = fname;
			fname = reverseSTR(copytochfromstart(reverseSTR(fname), '/'));
			if (fname == fnameold)
				fname = reverseSTR(copytochfromstart(reverseSTR(fname), '\\'));
		}
		if (!UI->args.find(strid))
			UI->args.insert(make_pair(strid, call->args));
		auto res = PyRun_AnyFileEx(_f, fname.c_str(), true);
		//PyGILState_Release(gstate);
		UI->idmapmutex.lock();
		UI->processtridmap.erase(getpid());
		UI->idmapmutex.unlock();
		auto fuckvs = UI;
		UI->argmodmutex->lock();
		int i = 0;
		auto bargs = UI->args[strid];
		if (bargs.size() != 0 && bargs.size() > 10)
		{
			try
			{
				auto fargs = boost::any_cast<vector<wchar_t*>>(bargs[(bargs.size() < 10) ? 1 : 8]);
				while (i < fargs.size())
				{
					delete[] fargs[i];
					i++;
				}
			}
			catch (...) {} // at least you tried
		}
		if (bargs.size() > 10)
		{
			try
			{
				*boost::any_cast<bool*>(bargs[13]) = true;
			}
			catch (...) {}
		}
		bool isfirstargwindow = true;
		if(UI->args.size() > 0 && MapFind(UI->args.getref(), strid))
		{ 
			try
			{
				auto bs = UI->args[strid][0];
				if (UI->args[strid].size() != 0)
					boost::any_cast<ui>(UI->args[strid][0]);
				else
					isfirstargwindow = false;
			}
			catch (...)
			{
				isfirstargwindow = false;
			}
			if (isfirstargwindow)
				if(UI->args[strid].size() != 0)
					if (MapFind(UI->oncescmap.getref()[boost::any_cast<ui>(UI->args[strid][0])], fname))
					{
						UI->oncemutex.lock();
						UI->oncescmap.getref()[boost::any_cast<ui>(UI->args[strid][0])].erase(fname);
						UI->oncemutex.unlock();
					}
			if (MapFind(UI->args.getref(), strid))
				UI->args.erase(strid);
		}
		UI->argmodmutex->unlock();
		delete call;
		/*GAME::scriptthreadmutex.lock();
		int i = 0;
		GAME::scriptthreadmutex.unlock();*/
		//}
		//catch (std::exception& e)
		//{
		//	cout << "\b";
		//	cout << "Critical Error has occured..." << endl << ">";
		//}
	}

	void controls::callpy(button* bt, UINT msg, int2 pos)
	{
		string strid;
		auto wnd = boost::any_cast<window*>(bt->anyvars[0]);
		auto m = boost::any_cast<map<string, string>>(bt->anyvars[2]);
		map<string,ui>inputs = { {"LBUTTONUP",WM_LBUTTONUP},{"RBUTTONUP",WM_RBUTTONUP },{"LBUTTONDOWN",WM_LBUTTONDOWN },{"RBUTTONDOWN",WM_RBUTTONDOWN },
		{"MBUTTONUP" ,WM_MBUTTONUP },{"MBUTTONDOWN",WM_MBUTTONDOWN},{"MOUSEMOVE",WM_MOUSEMOVE } };
		int i = 0;
		string inputstr;
		BOOST_FOREACH(auto it, inputs)
		{
			if (it.second == msg)
			{
				inputstr = it.first;
				goto feachdone;
			}
		}
	feachdone:;
		int bn = boost::any_cast<int>(bt->anyvars[1]);
		string scriptname = UI->styles[wnd->styleid].msgproc[UI->styles[wnd->styleid].boxes[bn].message];
		if (MapFind(wnd->waitmap, scriptname))
		{
			wnd->waitmapmutex.lock();

			auto mttw = static_cast<condition_variable*>(wnd->waitmap[scriptname].second);
			if (MapFind(wnd->waitmap[scriptname].first, inputstr))
			{
				wnd->waitmap[scriptname].second = (void*)new string(inputstr);
				mttw->notify_one();
			}
			wnd->waitmapmutex.unlock();
			return;
		}
		try
		{
			auto ONMAP = UI->oncescmap.getref();
			//UI->oncemutex.lock();
			if (MapFind(ONMAP, wnd->ID))
				if (MapFind(ONMAP[wnd->ID], scriptname))
				{
					return;
				}
		}
		catch (...)
		{

		}
		//UI->oncemutex.unlock();
		vector<boost::any> v;
		v.push_back(wnd->ID);
		v.push_back(bt->anyvars[1]);
		v.push_back(msg);
		UI->args.lock();
		do
		{
			strid = to_string(rand());
		} while (MapFind(UI->args.getref(),strid));
		strid = scriptname + strid;
		UI->args.unlock();
		string msgf = inputstr;
		v.push_back(msgf);
		if (scriptname != "")
		{
			v.push_back(bt->anyvars[3]);
			auto pairptr = new pair<button*, controls*>(bt, this);
			v.push_back((void*)pairptr);
			v.push_back(pos.x);
			v.push_back(pos.y); //8(7)
			string subloc = boost::any_cast<string>(bt->anyvars[3]);

			wstring wstr0 = STRtoWSTR(strid);
			wchar_t * ch0 = new wchar_t[wstr0.size() + 1];
			wcsncpy(ch0, wstr0.c_str(), wstr0.size() + 1);
			wstring wstr1 = STRtoWSTR(subloc);
			wchar_t * ch1 = new wchar_t[wstr1.size() + 1];
			wcsncpy(ch1, wstr1.c_str(), wstr1.size() + 1);
			wchar_t* _args[] = { ch0, ch1};
			int argc = sizeof(_args) / sizeof(_args[0]);
			vector<wchar_t*> w_tv = { ch0,ch1 };
			v.push_back(w_tv); // 9(8)
		/*	while (UI->isargbmodified)
				Sleep(0);
			UI->isargbmodified = true;*/
			UI->argmodmutex->lock();
			v.push_back(scriptname); // 10(9)
			v.push_back(vector<boost::any>()); // 11(10)
			v.push_back(map<string,boost::any>()); // 12(11)
			v.push_back((thread*)curth); // 13(12)
			bool * bp;
			v.push_back(bp = new bool (false)); // 14(13) isjoinable or isdetached
			v.push_back(subloc); // 15(14) accessible from GUI.GetFLoc()
			UI->args.insert(make_pair(strid, v));
			//PySys_SetArgv(argc-1, _args);
			UI->argmodmutex->unlock();
			UI->idmapmutex.lock();
			//UI->processtridmap.insert(make_pair(getpid(), strid));
			UI->idmapmutex.unlock();
			//UI->isargbmodified = false;
			int ii = 0;
			string loc = subloc + "scripts\\" + scriptname;
			//_strid = strid;
			////while (_callpyloc != "")
			////	Sleep(0);
			//_callpyloc = loc;
			////try
			////{
			//	_pyscriptname = scriptname;
			//	thread* thptr = nullptr;
			//	UI->argmodmutex->lock();
			//	scriptthreads.push_back(make_pair(boost::any_cast<thread*>(UI->args[strid][12] = thptr = new thread (&controls::callpyscript,this, strid, loc, scriptname)), bp));
			//UI->argmodmutex->unlock();
			pycall call;
			call.cancontinue = bp;
			call.args = v;
			call.c = UI;
			call.scriptloc = loc;
			call.scriptname = scriptname;
			call.strid = strid;
			call.wnd = wnd;
			call.conptr = this;
			UI->argmodmutex->lock();
			GAME::calls.push_back(new pycall(call));
			UI->argmodmutex->unlock();
			if (calls.size() == 1)
				UI->_mttw.notify_one();
			//callpyscript();
#if PYWRITESCRIPTDATA == true
				cout << scriptname << " has started (ID: " << strid << ")" << endl;
#endif
	/*		}
			catch (...)
			{*/
				//cout << "\b";
				//cout << "Critical Error has occured..." << endl << ">";
			//}
			//if (MapFind(UI->args, strid))
			//	DebugBreak();
			loc = "";
		}

	}
	void controls::MouseEvent(WPARAM wParam, LPARAM lParam, UINT msg, int2* forcedpos)
	{
		if (!wasinit)
			return;
		int2 pos = (forcedpos == nullptr) ? int2(GET_X_LPARAM((lParam)), GET_Y_LPARAM((lParam))) : *forcedpos;
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			lastmousepos = pos;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (lbdownfunc != nullptr)
				lbdownfunc(pos);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (rbdownfunc != nullptr)
				rbdownfunc(pos);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			if (mbdownfunc != nullptr)
				mbdownfunc(pos);
			break;
		}
		case WM_LBUTTONUP:
		{
			if (lbupfunc != nullptr)
				lbdownfunc(pos);
			break;
		}
		case WM_RBUTTONUP:
		{
			if (rbupfunc != nullptr)
				rbupfunc(pos);
			break;
		}
		case WM_MBUTTONUP:
		{
			if (mbupfunc != nullptr)
				mbupfunc(pos);
			break;
		}
		}
		if (buttons.size() == 0)
			return;
		*pobjpos = pos;
		while (currentlycheckingc != nullptr)
			Sleep(0);
		currentlycheckingc = this;
		pclass.tick();
		vector<button*> bac = btnspr;
		btnspr.clear();
		currentlycheckingc = nullptr;
		int i = 0;
		map<int2*, void*> mp; //should be deleted once the positioning gets fixed, for fuck sake how does this keep happening?
		while (i < buttons.size())
		{
			if (buttons[i] == NULL)
			{
				buttons.erase(buttons.begin() + i);
				continue;
			}
			if (buttons[i]->targetstate != buttons[i]->idlen && buttons[i]->targetstate == buttons[i]->state)
			{
				if (bac.size() == 0)
				{
					buttons[i]->targetstate = buttons[i]->idlen;
					buttons[i]->state = buttons[i]->idlen;
				}
				else if (!FindInVec(bac, buttons[i]))
				{
					buttons[i]->targetstate = buttons[i]->idlen;
					buttons[i]->state = buttons[i]->idlen;
				}
			}
			GUI::core* UIptr = GAME::UI;
			if (buttons[i]->callpyscript)
				if (buttons[i]->callpyonanymmove)
				{
					callpy(buttons[i], msg, pos);
					waspycalled = true;
				}
				else
				{
					/*if (!MapFind(mp, &buttons[i]->size))
					{
						int y = buttons[i]->size.x;
						buttons[i]->size.x = buttons[i]->size.y;
						buttons[i]->size.y = y;
						mp.insert(make_pair(&buttons[i]->size, (void*)nullptr));
					}*/
					int4 rec(*buttons[i]->pos, *buttons[i]->pos + buttons[i]->size);
					if (classvariables::isinside(rec, pos))
					{
						callpy(buttons[i], msg, pos);
						waspycalled = true;
					}
				}
			i++;
		}
		i = 0;
		while (i < bac.size())
		{
			if (bac[i] == NULL)
				bac.erase(bac.begin() + i);
			i++;
		}
		if (bac.size() == 0)
			return;
		i = 0;
		while (i < bac.size())
		{
			if (bac.size() != 1 && bac[i]->backgroundbutton || (bac[i]->backgroundbutton && waspycalled))
			{
				i++;
				continue;
			}
			i++;
		}
		i = 0;
		switch (msg)
		{
		case WM_MOUSEMOVE:
		{
			if (rmousemovefunc != nullptr)
				if (rbuttondown)
					rmousemovefunc(pos);
			if (lmousemovefunc != nullptr)
				if (lbuttondown)
					lmousemovefunc(pos);
			if (mmousemovefunc != nullptr)
				if (mbuttondown)
					mmousemovefunc(pos);
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseMove(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_MBUTTONDOWN:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseMMB(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseMMBup(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseRMB(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseRMBup(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseLMB(bac[i], pos);
				i++;
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			while (i < bac.size())
			{
				if ((bac.size() != 1 && bac[i]->backgroundbutton) || (bac[i]->backgroundbutton && waspycalled))
				{
					i++;
					continue;
				}
				MouseLMBup(bac[i], pos);
				i++;
			}
			break;
		}
		}
	}

}