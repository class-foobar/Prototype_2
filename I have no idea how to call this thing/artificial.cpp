#include "artificial.h"
namespace DX2D
{
	extern string bslink;
}
namespace GAME
{
	namespace AI
	{
		extern map<string, GAME::AI::func> deffunctions;
		string bslink = DX2D::bslink;
		namespace python
		{
			void pyfile::create(string floc)
			{
				loc = floc;
				f = _Py_fopen(floc.c_str(), "r+");
				if ((filename = copytochfromstart(loc, '\\')) == "")
					filename = copytochfromstart(loc, '/');

			}
			void pyfunc::create(string loc, string name)
			{
				f = new pyfile(loc);
				this->name = name;
			}
		}
		void loadAI(enviroment* env,string datafileloc= "NULL")
		{
			int i = 0;
			AZfilelib::AZfile file;
			if (datafileloc == "NULL")
			{
				file.loadfile(bslink + "\\data\\AI\\AI.data");
			}
			else
				file.loadfile(datafileloc);
			string lc = bslink + "\\scripts\\";
			while (file.findvar("MODULES@I" + INTtoSTR(i) + "@name"))
			{
				AImodule* mod = new AImodule;
				int ii = 0;
				string loc = "MODULES@I" + INTtoSTR(i) + "@";
				mod->name = file.GetVar<string>(loc + "name");
				while (file.findstruct(loc + "F" + INTtoSTR(ii)))
				{
					auto st = file.GetStruct(loc + "F" + INTtoSTR(ii));
					string PF = st->getvar<string>("PF");
					string CF = st->getvar<string>("CF");
					IOinterface IOi;
					func* IOf = new func;
					if (PF != "" && PF != "NULL")
					{
						string funcloc = copytochfromstart(PF, '/');
						string funcname = copyfromchtoend(PF, '/');
						python::pyfunc* f = new python::pyfunc(lc+funcloc,funcname);
						IOf->pyfunc = f;
					}
					if (CF != "" && CF != "NULL")
					{
						IOf->cppfunc = deffunctions[CF].cppfunc;
					}
					IOi.f = IOf;
					string INF = st->getvar<string>("INPROCF");
					string OUTF = st->getvar<string>("OUTPROCF");
					if (INF != "" && INF != "NULL")
						IOi.argprocessor->cppfunc = deffunctions[INF].cppfunc;
					if (OUTF != "" && OUTF != "NULL")
						IOi.outputprocessor->cppfunc = deffunctions[OUTF].cppfunc;
					mod->interfaces.push_back(IOi);
					ii++;
				}
				env->modptrvec.push_back(mod);
				env->modulecontainer.insert(make_pair(mod->name, mod));
				i++;
			}
			i = 0;
			while (file.findvar("MAINFRAMES@I" + INTtoSTR(i) + "@name"))
			{
				mainframe* mf = new mainframe;
				string loc = "MAINFRAMES@I" + INTtoSTR(i)  + "@";
				mf->name = file.GetVar<string>(loc + "name");
				int ii = 0;
				while (file.findstruct(loc + "M" + INTtoSTR(ii)))
				{
					auto st = file.GetStruct(loc + "M" + INTtoSTR(ii));
					string mname = st->getvar<string>("name");
					ui id = 0;
					string str;
					if ((str = st->getvar<string>("strid")) != "NULL")
						id = env->inputidmap[str];
					else
						id = st->getvar<ui>("id");
					mf->modlist.push_back(env->GetModule(mname));
					InsertToMapOfVecs(mf->mods, id, env->GetModule(mname));
					ii++;
				}
				env->mfptrvec.push_back(mf);
				env->mframecontainer.insert(make_pair(mf->name, mf));
				i++;
			}

		}
		inline AImodule * enviroment::GetModule(string name) // returns the pointer to original module
		{
			if (modulecontainer.find(name))
				return modulecontainer[name];
			/*if (modulecontainer.find(name))
			return new AImodule(*modulecontainer[name]);*/
			return nullptr;
		}
		inline mainframe * enviroment::GetMainframe(string name)// returns copy of mainframe containing copies of modules
		{
			if (mframecontainer.find(name))
				return new mainframe(*mframecontainer[name]);
			return nullptr;
		}
		inline mainframe::mainframe(const mainframe & src)
		{
			memcpy(this, &src, sizeof(mainframe));
			int i = 0;
			auto oldmodlist = mods;
			map<AImodule*, AImodule*> copytable;
			for (auto it : mods)
			{
				i = 0;
				while (i < it.second.size())
				{
					if (MapFind(copytable, it.second[i]))
						mods[it.first][i] = copytable[it.second[i]];
					else
					{
						auto nmod = new AImodule;
						auto old = it.second[i];
						memcpy(nmod, old, sizeof(AImodule));
						copytable.insert(make_pair(old, nmod));
						mods[it.first][i] = nmod;
					}
					i++;
				}
			}
			i = 0;
			modlist = {};
			for (auto it : copytable)
			{
				modlist.push_back(it.second);
			}
		}
}

}