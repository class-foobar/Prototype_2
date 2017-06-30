#include "universe.h"
#include "X:\PROJECTS\economy\economy\economy.h"
#undef _INTtoSTR
//inline string _INTtoSTR(int i)
//{
//	return to_string(i);
//}
//#define INTtoSTR _INTtoSTR
using namespace std;
using namespace DX2D;
using namespace GAME;
using namespace AZfilelib;
namespace DX2D
{
	extern string saveslot;
}
namespace GAME
{
	extern vector<entity> entitylist;
	extern universe* uniclass;
	string apploc;
	string GetLinkToElSTR(string elemloc)
	{
		return apploc + elemloc;
	}
	void init(string napploc)
	{
		apploc = napploc;
		int i = 0;
		using namespace AZfilelib;
		string str = saveslot;
		AZfile unitemp;
		unitemp.loadfile(apploc+"saves\\"+ str +"\\universetemp.unitemp");
		AZfile unistate;
		unistate.loadfile(apploc+"saves\\" + str + "\\universestate.unistate");
		i = 0;
		vector <system*> sysvec;
		map <string, system*> sysmap;
		map <char, starcol> startypcol;
		map<string, currency*> curmap;
		vector<space*> spacevec;
		ui seed = unitemp.GetVar<ui>("seed");
		i = 0;
		if (seed == 0)
			seed = time(NULL);
		vector<string> sysnames;
		while (unitemp.findvar("system" + INTtoSTR(i)))
		{
			sysnames.push_back(unitemp.GetVar<string>("system" + INTtoSTR(i)));
			i++;
		}
		i = 0;
		string type = "OBAFGKM";
		while (i < type.size())
		{
			starcol col;
			string str = "STARGEN";
			str += +'@';
			str += type[i] + (string)"_TYPE" + '@' + "rangemin";
			col.min = int3((ui)unitemp.GetVar<ui>(str + 'R'), (ui)unitemp.GetVar<ui>
				(str + 'G'),
				(ui)unitemp.GetVar<ui>(str + 'B'));
			col.max = int3((ui)unitemp.GetVar<ui>(str + 'R'), (ui)unitemp.GetVar<ui>
				(str + 'G'),
				(ui)unitemp.GetVar<ui>(str + 'B'));
			i++;
		}
		i = 0;
		vector<pair<space*, vector<pair<string, pair<int2, int2>>>>> jumppoints;
		vector<node*> nodesmainvec;
		vector<company*> mcompvec;
		while (i < sysnames.size())
		{
			int ii = 0;
			system* sys = new system;
			sys->name = sysnames[i];
			sys->seed = unitemp.GetVar<ui>(sysnames[i] + "@" + "seed");
			if (sys->seed == 0)
				sys->seed = seed;
			sys->s = new space();
			spacevec.push_back(sys->s);
			space* s = sys->s;
			ii = 0;
			vector<pair<string, pair<int2, int2>>> jps;
			while (unitemp.findstruct(sysnames[i] + "@jump" + INTtoSTR(ii)) )
			{
				jps.push_back(pair<string, pair<int2, int2>>{ sysnames[i] + "@jump" + INTtoSTR(ii) + "@" + "target",
				pair<int2,int2>{ int2{unitemp.GetVar<int>(sysnames[i] + "@jump" + INTtoSTR(ii) + "@posX"), unitemp.GetVar<int>(sysnames[i] + "@jump" + INTtoSTR(ii) + "@posY") },
				int2{unitemp.GetVar<int>(sysnames[i] + "@jump" + INTtoSTR(ii) + "@posXout"), unitemp.GetVar<int>(sysnames[i] + "@jump" + INTtoSTR(ii) + "@posYout") } } });
				ii++;
			}
			jumppoints.push_back({ s,jps });
			vector<string> stars = unitemp.GetStructNamesInNode(sysnames[i] + "@" + "STAR");
			vector<string> planets = unitemp.GetStructNamesInNode(sysnames[i] + "@" + "PLANET");
			vector<string> stations = unitemp.GetStructNamesInNode(sysnames[i] + "@" + "STATION");
			ii = 0;
			map<string, star*> starmap;
			vector <star*> starvec;
			vector<token*>tv;
			//vector<currency*> curvec;
			while (ii < stars.size())
			{
				AZstruct* ss = unitemp.GetStruct(sysnames[i] + "@" + stars[ii]);
				star *s = new star;
				s->pos = new int2( ss->getvar<int>("mapposX"), ss->getvar<int>("mapposY") );
				s->radius = ss->getvar<int128>("radius");
				s->typechar = ss->getvar<char>("typeletter");
				s->hotness = ss->getvar<ui>("hotness");
				s->luminosity = ss->getvar<ui>("luminosity");
				s->luminosity = ss->getvar<int>("probability");
				s->displayedtemp = ss->getvar<ulli>("displayedtemp");
				if (s->seed == 0)
					s->seed = sys->seed;
				starmap.insert(make_pair(stars[ii], s));
				starvec.push_back(s);
				ii++;
			}
			ii = 0;
			map<string, planet*> planetmap;
			vector <planet*> planetvec;
			while (ii < planets.size())
			{
				AZstruct* ss = unitemp.GetStruct(sysnames[i] + "@" + planets[ii]);
				int iii = 0;
				token* t = nullptr;
				currency* pc = nullptr;
				while (unitemp.findvar(sysnames[i] + "@" + planets[ii] + "@cur" + INTtoSTR(iii)))
				{
					if (ss->getvar<string>("cur" + INTtoSTR(iii)) == "NULL")
					{
						iii++;
						continue;
					}
					ulli cura = ss->getvar<ulli>("cura" + INTtoSTR(iii));
					string cname = ss->getvar<string>("cur" + INTtoSTR(iii));
					if (MapFind(curmap, "cur" + INTtoSTR(iii)))
					{
						if (t == nullptr)
						{
							t = CreateToken({ curmap[cname] ,cura });
						}
						else
							curmap[cname]->print(t, cura);
					}
					else
					{
						currency* cur = new currency;
						curmap.insert(make_pair(cname, cur));
						t = CreateToken({ curmap[cname] ,cura });

					}
					iii++;
				}
				iii = 0;
				pc/*mr*/ = curmap[ss->getvar<string>("maincur")];
				token* pt = t;
				planet* plan = new planet;
				ui popi = ss->getvar<ui>("pop");
				float mfoc = ss->getvar<float>("miningfocus");
				float ifoc = ss->getvar<float>("industfocus");
				float ffoc = ss->getvar<float>("farmfocus");
				vector<GAME::pop*> pp; // pEOpLE
				ui mpop = popi*mfoc;
				ui fpop = popi*ffoc;
				ui ipop = popi* ifoc;
				company* c = new company();
				c->name = planets[ii]+" incorporated";
				mcompvec.push_back(c);
				c->isgovcomp = false;
				while (iii < 10)
				{
					pop* p = new pop;
					c->workersvec.push_back(p);
					p->education = work::construction;
					p->t = CreateToken({ pc,2000 });
					pp.push_back(p);
					iii++;
				}
				iii = 0;
				while (iii < popi)
				{
					pop* p = new pop;
					if (iii > mpop + fpop)
						p->education = work::industrialprod;
					else if (iii > mpop)
						p->education = work::foodprod;
					else
						p->education = work::mining;
					p->t = CreateToken({ pc,1000 });
					pp.push_back(p);
					iii++;
				}
				//plan->pos = new int2({ ss->getvar<int>("mapposX"), ss->getvar<int>("mapposY") });
#undef std
#undef min
#undef max
				int128 r = plan->orbrad = ss->getvar<int128>("orbradius");
				float *fp = plan->rot = new float(ss->getvar<float>("rot"));
				float f = *fp;
				float cxf = cos(f);
				int128 i128x = (int)cxf;
				i128x *= r;
				i128x /= 100;
				float syf = sin(f);
				int128 i128y = (int)syf;
				i128y *= r;
				i128y /= 100;
				plan->pos = new int2((int)i128x, (int)i128y);
				plan->orbrad = ss->getvar<int128>("orbradius");
				plan->atmospheresize = ss->getvar<lli>("atmospheresize");
				plan->type = ss->getvar<ui>("type");
				plan->mass = ss->getvar<int>("mass");
				//long double surface;
				//long double rad = (double)plan->radius;
				//surface = 4 * M_PIl * powl(rad, 2);
				//surface /= 1000000;
				//surface = roundl(surface);
				int128 sur = plan->radius*plan->radius;
				sur *= 4;
				sur = sur* (int) (M_PIl*100);
				sur /= 100;
#ifdef _DEBUG
				ulli i64sur1 = (ulli)sur;
#endif // _DEBUG

				iii = 0;
				float landc = (float)ss->getvar<float>("landcoverage");
				sur *= (int)(landc * 100);
				sur /= 100;
				sur /= 1000000;
#ifdef _DEBUG
				ulli i64sur2 = (ulli)sur;
#endif // _DEBUG
				vector<land*> lvec;
				land* lconst = new land;
				lconst->lvl = 1;
				lconst->maxpop = 6;
				lconst->t = pt;
				lconst->payment = 4000 / GetCurrencyValue(pc);
				lconst->u = usage::construction;
				lvec.push_back(lconst);
				lconst = new land(*lconst);
				lvec.push_back(lconst);
				int tiles = (int)sur;
				int totaltiles = (int)sur;
				tiles -= popi / 5;
				int mtiles = tiles*mfoc;
				int ftiles = tiles*ffoc;
				int itiles = tiles* ifoc;
				float cv = GetCurrencyValue(pc);
				while (iii < tiles)
				{
					land* l = new land;
					if (iii > mtiles + ftiles)
						l->u = usage::industrialprod;
					else if (iii > mtiles)
						l->u = usage::foodprod;
					else
						l->u = usage::mine;
					l->maxpop = 5;
					l->payment = 2000 / cv;
					lvec.push_back(l);
					iii++;
				}
				iii = 0;
				while (iii < popi / 5)
				{
					land* l = new land;
					l->u = usage::home;
					l->maxpop = 6;
					l->payment = 1000 / GetCurrencyValue(pc);
					iii++;
				}
				iii = 0;
				exchangepoint* ex = new exchangepoint(plan->pos, "bsexpoint");
				map<string, exchangepoint*> fuckcpp;
				fuckcpp.insert(pair<string, exchangepoint*>("bsexpoint", ex));
				plan->n = new node(ss->getvar<float>("taxrate") * 100, pt, s, planets[ii], pp, totaltiles, lvec, vector<exchangepoint*>{ ex },fuckcpp, plan->pos);
				c->HQnode = plan->n;
				if (plan->seed == 0)
					plan->seed = sys->seed;
				planetmap.insert(make_pair(planets[ii], plan));
				planetvec.push_back(plan);
				s->nodvec.push_back(plan->n);
				s->nodmap.insert(make_pair(planets[ii], plan->n));
				nodesmainvec.push_back(plan->n);
				plan->n->officialcur = pc;
				ii++;
			}
			ii = 0;
			while (ii < stations.size())
			{
				station* stat = new station;
				stat->pos = new int2(0, 0);
				string str = sysnames[i] + '@' + stations[i] + '@';
				string exof = unitemp.GetVar<string>(str+"exchangepointof");
				int exi = unitemp.GetVar<int>(str+"exchangepointi");
				stat->orbitedbody = unitemp.GetVar<string>(str+"orbitedbody");
				stat->orbrad = unitemp.GetVar<int128>(str+"orbrad");
				stat->orbang = unitemp.GetVar<float>(str+"orbang");
				string datal = unitemp.GetVar<string>(str+"stationdatalink");
				stat->datal = datal;
				if (datal[datal.size() - 1] != '@')
					datal += '@';
				stat->createmods(unitemp, datal);
				int iii = 0;
				while (unitemp.findvar(str + "STORAGE@type" + INTtoSTR(iii)))
				{
					stationres res =  strtostationres(unitemp.GetVar<string>(str + "STORAGE@type" + INTtoSTR(iii)));
					AddToMap(stat->STORAGE, { res,unitemp.GetVar<float>(str + "STORAGE@amount" + INTtoSTR(iii)) });
					iii++;
				}
				iii = 0;
				if (exof != "NULL")
				{
					while (planetmap[exof]->n->expoints.size() <= exi)
						planetmap[exof]->n->expoints.push_back(new exchangepoint(new int2(0,0),"expoint" +  INTtoSTR((int)planetmap[exof]->n->expoints.size()-1),planetmap[exof]->n));
					//delete planetmap[exof]->n->expoints[exi]->pos;
					//planetmap[exof]->n->expoints[exi]->pos = (stat->pos);
					planetmap[exof]->n->expoints[exi]->exchangepointname = stat->name;
				}
				sys->statmap.insert(make_pair(stat->name, stat));
				sys->statvec.push_back(stat);
				ii++;
			}
			ii = 0;
			sys->planetmap = planetmap;
			sys->starmap = starmap;
			sys->planets = planetvec;
			sys->stars = starvec;
			sys->syspos = new int2(unitemp.GetVar<int>(sysnames[i] + "@" + "XPOS"), unitemp.GetVar<int>(sysnames[i] + "@" + "YPOS"));
			sys->randplanetnum = unitemp.GetVar<ui>(sysnames[i] + "@" + "randplanetnum");
			sysvec.push_back(sys);
			sys->s = s;
			s->spacename = sys->name;
			sysmap.insert(make_pair(sysnames[i], sys));
			i++;
		}
		int answer = unitemp.GetVar<int>("answertotheultimatequestion");
		string uniname = unitemp.GetVar<string>("uniname");
		map<usage, vector<productionIO>> pIO;
		vector <productionIO> vIO;
		{
			productionIO IO;
			IO.in.push_back(make_pair(cargotype::commonmetalore, 1));
			IO.out.push_back(make_pair(cargotype::everydayitems, 5));
			vIO.push_back(IO);
		}
		{
			productionIO IO;
			IO.in.push_back(make_pair(cargotype::raremetalore, 1));
			IO.out.push_back(make_pair(cargotype::everydayitems, 15));
			vIO.push_back(IO);
		}
		{
			productionIO IO;
			IO.in.push_back(make_pair(cargotype::commonmetalore, 4));
			IO.in.push_back(make_pair(cargotype::raremetalore, 2));
			IO.out.push_back(make_pair(cargotype::buildingmaterials, 1));
			vIO.push_back(IO);
		}
		pIO.insert(make_pair(usage::industrialprod, vIO));
		vIO.clear();
		{
			productionIO IO;
			IO.in.push_back(make_pair(cargotype::raremetalore, 2));
			IO.in.push_back(make_pair(cargotype::luxymetalore, 2));
			IO.out.push_back(make_pair(cargotype::luxyitems, 2));
			vIO.push_back(IO);
		}
		{
			productionIO IO;
			IO.in.push_back(make_pair(cargotype::commonmetalore, 3));
			IO.in.push_back(make_pair(cargotype::raremetalore, 2));
			IO.in.push_back(make_pair(cargotype::luxymetalore, 2));
			IO.out.push_back(make_pair(cargotype::luxyitems, 3));
			vIO.push_back(IO);
		}
		pIO.insert(make_pair(usage::luxyprod, vIO));
		vIO.clear();
		{
			productionIO IO;
			IO.reqIN = false;
			IO.out.push_back(make_pair(cargotype::food, 2));
			vIO.push_back(IO);
		}
		pIO.insert(make_pair(usage::foodprod, vIO));
		vIO.clear();
		{
			productionIO IO;
			IO.reqIN = false;
			IO.out.push_back(make_pair(cargotype::luxyfood, 1));
			vIO.push_back(IO);
		}
		pIO.insert(make_pair(usage::luxyfoodprod, vIO));
		i = 0;
		while (i < jumppoints.size())
		{
			space* s = jumppoints[i].first;
			int ii = 0;
			map<string, space*> bsmap;
			while (ii < spacevec.size())
			{
				bsmap.insert(make_pair(spacevec[i]->spacename, spacevec[i]));
				ii++;
			}
			ii = 0;
			while (ii < jumppoints[i].second.size())
			{
				space* target = bsmap[jumppoints[i].second[ii].first];
				pair<int2, int2> i2i2 = jumppoints[i].second[ii].second; //r2d2's retarded brother
				pair<int2*, int2*> i2pi2p = { new int2(i2i2.first),new int2(i2i2.second) };
				s->jumppoints.insert(make_pair(target,i2pi2p));
				ii++;
			}
			i++;
		}
		ui randainum = unitemp.GetVar<ui>("randomtraders");
		i = 0;
		vector<trader*> traders;
		while (i < randainum)
		{
			ui phui = rand() % spacevec.size();
			if (phui == spacevec.size())
				phui--;
			space* s = spacevec[phui];
			int2* pos = new int2((int)(rand() % 600) - 300, (int)(rand() % 600) - 300);
			auto cvp = MapSecondtoVec(curmap);
			int cvpe = rand() % curmap.size();
			if (cvpe == curmap.size())
				cvpe--;
			currency* cur = cvp[cvpe];
			token* t = CreateToken({ cur,rand() % 5000 + 100 / GetCurrencyValue(cur) });
			trader* shinzo = new trader();
			shinzo->maxcargo = rand() % 30 + 2;
			shinzo->comp = nullptr;
			shinzo->tradername = "shinzo #" + INTtoSTR(i);
			shinzo->pos = pos;
			shinzo->s = s;
			traders.push_back(shinzo);
			i++;
		}
		ui rsysnum = unitemp.GetVar<ui>("randomsysnum");
		uniclass = new universe(answer, uniname, rsysnum, {},sysvec,sysmap,nullptr,seed,startypcol);
		uniclass->eco = new economy(nodesmainvec, spacevec, traders, MapSecondtoVec(curmap), pIO, mcompvec);
		i = 0;
		while (i < randainum)
		{
			traders[i]->economyptr = uniclass->eco;
			i++;
		}
		i = 0;
		while (i < nodesmainvec.size())
		{
			nodesmainvec[i]->ecoptr = uniclass->eco;
			i++;
		}
		i = 0;
		i = 0;
		uniclass->eco->daypassed = true;
		//uniclass->eco->tick();
	}
	void loadinoutputs(pair<vector<pair<stationres, float>>&, string>INPUT, pair<vector<pair<stationres, float>>&, string>INPUTd,
		pair<vector<pair<stationres, float>>&, string>INPUTb, pair<vector<pair<stationres, float>>&, string>OUTPUT,
		pair<vector<pair<stationres, float>>&, string>OUTPUTd, pair<vector<pair<stationres, float>>&, string>OUTPUTb, pair<vector<pair<stationres, float>>&, string>STORAGE, AZfile& f, string loc)
	{
		int i = 0;

		vector<pair<vector<pair<stationres, float>>&, string>> links = {
			INPUT,INPUTd,INPUTb,OUTPUT,OUTPUTb,OUTPUTd,STORAGE
		};
		while (i < links.size())
		{
			links[i].second = loc + links[i].second;
			i++;
		}
		i = 0;
		while (i < links.size())
		{
			if (f.findvar(links[i].second))
			{
				int ii = 0;
				while (f.findvar(links[i].second + "type" + INTtoSTR(ii)))
				{
					links[i].first.push_back(pair<stationres, float>(strtostationres(f.GetVar<string>(links[i].second + "type" + INTtoSTR(ii))),
						f.GetVar<float>(links[i].second + "amount" + INTtoSTR(ii))));
					ii++;
				}
			}
			i++;
		}
	}
	void loadsector(camera* cam, frame* mf, int2 pos, GAME::universe* uniptr, GAME::economy* ecoptr, GAME::system* sysptr, GAME::space* spptr, DX2D::ship& player)
	{
		frame* primaryf    = new frame; // stations, ships and stuff
		frame* secondaryf  = new frame; // planets, stars, moons
		frame* backgroundf = new frame; // stars
		mf->f[mf->wchiac].push_back(backgroundf);
		mf->f[mf->wchiac].push_back(secondaryf);
		mf->f[mf->wchiac].push_back(primaryf);
		primaryf->wchiac = "primarysubframes";
		secondaryf->wchiac = "secondarysubframes";
		backgroundf->wchiac = "backgroundsubframes";
		primaryf->ismactive = true;
		secondaryf->ismactive = true;
		backgroundf->ismactive = true;
		primaryf->ischfact = true;
		secondaryf->ischfact = true;
		backgroundf->ischfact = true;
		int i = 0;
		while (i < sysptr->statvec.size())
		{
			int128 r = sysptr->statvec[i]->orbrad;
			float cxf = cos(sysptr->statvec[i]->orbang);
			int128 i128x = (int)cxf;
			i128x *= r;
			i128x /= 100;
			float syf = sin(sysptr->statvec[i]->orbang);
			int128 i128y = (int)syf;
			i128y *= r;
			i128y /= 100;
			uni2<float> fpos = { (float)(int)i128x, (float)(int)i128y };
			fpos = fpos / 299792;
			int2* ppp = sysptr->getposofobj(sysptr->statvec[i]->orbitedbody);
 			fpos = fpos + (uni2<float>)*sysptr->getposofobj(sysptr->statvec[i]->orbitedbody);
			int2 spos = fpos.toint2low();
			if (spos == pos)
			{
				int2 actualpos = (uni2<float>(fpos - spos.touni2<float>()) * sectorsize).toint2();
				*sysptr->statvec[i]->pos = actualpos /*= { 100 ,100}*/;
				sysptr->statvec[i]->RenderInit(primaryf,cam);
				*player.pos = /*-*/(actualpos +int2 {-100, -100});
				debugging::debugwindow* dw = new debugging::debugwindow;
				dw->dvarrect = int4{ 0,0,300,240 };
				dw->subwindowsrect = int4(0, 240, 300, 300);
				dw->wsize = { 300,340 };
				//dw->addvar(actualpos, "INT2", "station " + sysptr->statvec[i]->name);
				//dw->init();
				//debugging::dbm->addsw(dw);
			}
			i++;
		}
	}
}
#undef INTtoSTR