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
	void controls::callpyscript(string strid, string loc, string fname )
	{
		//auto localname = _pyscriptname;
		//string loccopy = _callpyloc;
		//string strid = _strid;
		FILE* _f = NULL;
		_f = _Py_fopen(loc.c_str(), "r");
		/*_pyscriptname = "";
		_callpyloc = "";
		_strid = "";*/
		UI->argmodmutex->lock();
		UI->idmapmutex.lock();
		UI->processtridmap.insert(make_pair(getpid(), strid));
		UI->idmapmutex.unlock();
		UI->argmodmutex->unlock();
		//try
		//{
		if (fname == "basewindow_msg_move.py")
			_testi++;
		PyGILState_STATE gstate;
		//gstate = PyGILState_Ensure();
		auto res = PyRun_AnyFileEx(_f, fname.c_str(), true);
		//PyGILState_Release(gstate);
		UI->idmapmutex.lock();
		UI->processtridmap.erase(getpid());
		UI->idmapmutex.unlock();
		auto fuckvs = UI;
		UI->argmodmutex->lock();
		_realtesti++;
		auto bargs = UI->args[strid];
		auto fargs = boost::any_cast<vector<wchar_t*>>(bargs[(bargs.size() < 10) ? 1 : 8]);
		int i = 0;
		while (i < fargs.size())
		{
			delete[] fargs[i];
			i++;
		}
		if (bargs.size() > 10)
		{
			*boost::any_cast<bool*>(bargs[13]) = true;
		}
		UI->args.erase(strid);
		UI->argmodmutex->unlock();
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
		vector<boost::any> v;
		v.push_back(wnd->ID);
		v.push_back(bt->anyvars[1]);
		v.push_back(msg);
		do
		{
			strid = to_string(rand());
		} while (MapFind(UI->args,strid));
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
			v.push_back((thread*)nullptr); // 13(12) placeholder
			bool * bp;
			v.push_back(bp = new bool (false)); // 14(13) isjoinable or isdetached
			v.push_back(subloc); // 15(14) accessible from GUI.GetFLoc()
			UI->args.insert(make_pair(strid, v));
			//PySys_SetArgv(argc-1, _args);
			UI->argmodmutex->unlock();
			UI->idmapmutex.lock();
			UI->processtridmap.insert(make_pair(getpid(), strid));
			UI->idmapmutex.unlock();
			//UI->isargbmodified = false;
			int ii = 0;
			string loc = subloc + "scripts\\" + scriptname;
			_strid = strid;
			//while (_callpyloc != "")
			//	Sleep(0);
			_callpyloc = loc;
			//try
			//{
				_pyscriptname = scriptname;
				thread* thptr = nullptr;
				UI->argmodmutex->lock();
				scriptthreads.push_back(make_pair(boost::any_cast<thread*>(UI->args[strid][12] = thptr = new thread (&controls::callpyscript,this, strid, loc, scriptname)), bp));
				UI->argmodmutex->unlock();
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