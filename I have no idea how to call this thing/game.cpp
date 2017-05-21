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
using namespace GAME;
using namespace DX2D;
using namespace D2D1;
using namespace AZfilelib;
namespace GAME
{
	extern pathsystem mainpaths;
}
namespace debugging
{
	extern debugmain* dbm;
	extern debugging::debugwindow* pdebugdata;
}
namespace DX2D
{
	extern 	string bslink;
	extern main* DXclass;
	void crewman::tick()
	{
		if (hp <= 0)
		{
			//TODO:  RIP
		}
		float roomoxy = r->oxygenam;
		if (type->oxygenneeded != 0)
		{
			if (roomoxy < type->oxygenneeded)
			{
				hp -= 10;
			}
			else
				r->oxygenam -= type->oxygenneeded;
		}
		if (type->damagedbyoxygen)
		{
			float roxper = r->oxygenam / ((r->size.x * r->size.y) / 100);
			hp -= 2.5f * roxper;
		}
		int i = 0;
		double combatm = 1.0f;
		if (debuff != 0.0f)
		{
			combatm -= debuff;
			if (combatm < 0)
				combatm = 0;
		}
		while (i < actions.size())
		{
			switch (actions[i].first)
			{
			case null:
			{
				break;
			}
			case heal:
			{
				crewman* target = boost::any_cast<crewman*>(actions[i].second);
				if (target->hp < target->hpfull)
				{
					float healp = 10.0f;
					healp *= sotherb / 10;
					healp *= sfightb / 10;
					target->hp += healp;
					if (target->hp > target->hpfull)
						target->hp = target->hpfull;
					if (target->hp == target->hpfull)
					{
						actions.erase(actions.begin() + i);
					}
				}
				break;
			}
			case attack:
			{
				crewman* target = boost::any_cast<crewman*>(actions[i].second);
				float damage = wepdamage * combatm;
				target->hp -= damage;
				break;
			}
			case fight:
			{
				int ii = 0;
				//bool bf =boost::any_cast<bool>(actions[i].second);
				while (ii < r->c.size())
				{
					if (r->c[ii]->ft->fac->GetRelations(ft->fac) == facrel::hostile)
					{
						actions.push_back(make_pair(attack, r->c[ii]));
						actions.erase(actions.begin() + i);
						break;
					}
					ii++;
				}
				break;
			}
			case capture:
			{
				break;
			}
			case help:
			{
				crewman* target = boost::any_cast<crewman*>(actions[i].second);
				actions.erase(actions.begin() + i);
				int ii = 0;
				while (ii < target->actions.size())
				{
					actions.push_back(target->actions[ii]);
					ii++;
				}
				break;
			}
			case operate:
			{
				auto slot = boost::any_cast<pair<module*, pair<int2, slottype>>>(actions[i].second);
				if (slot.first->rptr == r)
				{
					if (slot.second.first == *pos)
					{
						int ii = 0;
						actions.erase(actions.begin() + i);
						if (slot.second.second == slottype::control)
						{
							slot.first->ism = true;
						}
					}
				}
				break;
			}
			case repair:
			{
				module* mod = boost::any_cast<module*>(actions[i].second);
				if (mod->hp == mod->maxhp)
					break;
				mod->hp += 50.0f*srepairb;
				if (mod->hp > mod->maxhp)
					mod->hp = mod->maxhp;
				break;
			}
			case move:
			{
				auto pat = boost::any_cast<path>(actions[i].second);
				uni2<long double> normalised = classvariables::normalize(*pos-pat.nodes[0].first);
				normalised = normalised* uni2<long double> {-1, -1};
				posprec = posprec + normalised;
				if (posprec.x >= 1 || posprec.x <= -1)
				{
					pos->x += floor(posprec.x);
					posprec.x -= floor(posprec.x);
				}
				if (posprec.y >= 1 || posprec.y <= -1)
				{
					pos->y += floor(posprec.y);
					posprec.y -= floor(posprec.y);
				}
				break;
			}
			}
			i++;
		}
	}
	inline void insertsdts(vector<sprite>& spritevec, ship s) // insert ship data to sprite vector
	{
		int i = 0;
		while (i < 4)
		{
			vector<eng> engvec;
			bool* bp = nullptr;
			if (i == 0)
			{
				engvec = s.leftroteng;
				bp = s.leftr;
			}
			else if (i == 1)
			{
				engvec = s.rightroteng;
				bp = s.rightr;
			}
			else if (i == 2)
			{
				engvec = s.fronteng;
				bp = s.frontr;
			}
			else if (i == 3)
			{
				engvec = s.backeng;
				bp = s.backr;
			}
			int ii = 0;
			while (ii < engvec.size())
			{
				sprite nsp;
				nsp.render = bp;
				nsp.SetBitmap(engvec[ii].bitmap);
				nsp.SetX(engvec[ii].pos.x);
				nsp.SetY(engvec[ii].pos.y);
				nsp.SetRot(engvec[ii].rot);
				nsp.size = engvec[ii].size;
				spritevec.push_back(nsp);
				ii++;

			}
			i++;
		}
	}
	module::module(module* mod, bool copy, ship& sh, camera* cam, frame* f, int2* npos, int2 size, float* rot)
	{
		*this = *mod;
		if (copy)
		{
			AZfile &file = *filep;
			int i = 0;
			if(!isbaked)
			while (file.findvar("STORAGE@T" + INTtoSTR(i)))
			{
				shipres sr = getshipresfromstr(file.GetVar<string>("STORAGE@T" + INTtoSTR(i)));
				AddToMap(sh.limit, make_pair(sr, (ui)file.GetVar<float>("STORAGE@TA" + INTtoSTR(i))));
				i++;
			}
			room* roo = new room;
			this->rptr = roo;
			roo->oxygenam = size.x * size.y /100;
			roo->pos = *npos;
			roo->size = size;
			sh.rooms.push_back(roo);
			i = 0;
			if (m == modtype::sensor)
			{
				bool mannedibonus  = file.GetVar<bool>("mannedibonus");
				float mannedrbonus = file.GetVar<float>("mannedrbonus");
				if (sh.mannedibonus == false)
					sh.mannedibonus = mannedibonus;
				if (sh.mannedrbonus < mannedrbonus)
					sh.mannedrbonus = mannedrbonus;
				int lr = file.GetVar<int>("lrsenr");
				int mr = file.GetVar<int>("mrsenr");
				if (lr > sh.lrsenr)
					sh.lrsenr = lr;
				if (mr > sh.mrsenr)
					sh.mrsenr = mr;
				slvl = getinfolvlfromstr(file.GetVar<string>("infolvl"));
				slvlm = getinfolvlfromstr(file.GetVar<string>("infolvlmanned"));
				sh.slvl = slvl;
				sh.slvlm = slvlm;
			}
			else if (m == modtype::reactor)
			{
				string rstype = file.GetVar<string>("reactortype");
				if (rstype == "nuclearu")
				{
					rtype = reactortype::nuclearu;
				}
				else if (rstype == "nuclearp")
				{
					rtype = reactortype::nuclearp;

				}
				else if (rstype == "fusion")
				{
					rtype = reactortype::fusion;
				}
			}
			pos = npos;
			sprite sp(pos, rot);
			//sp.SetOffsetXYp(pos, true);
			//sp.SyncPos(pos, true);
			string loc;
			string mpngstr = "module";
			if (isbaked)
			{
				loc = bakedloc;
				mpngstr = mod->name;
			}
			else
				loc = file.link;
			sp.SetBitmapFromFile(STRtoWSTR(reverseSTR((copyfromchtoend(reverseSTR(loc), '\\', true))) + mpngstr + ".png").c_str(), *cam->GetRenderTargetP());
			rb = new bool(true);
			sp.render = rb;
			f->sprites.push_back(sp);
			/*f->sprites[f->sprites.size() - 1].size = { (float)size.x,(float)size.y };*/
		}
	}
	void module::tick(ship& sh)
	{
		vector<pair<shipres, float>>* inp = nullptr;
		vector<pair<shipres, float>>* outp = nullptr;
		int i = 0;
		i = 0;
		if (isd && hasd)
		{
			inp = &ind;
			outp = &out;
		}
		else if (ism && hasm)
		{
			inp = &inm;
			outp = &out;
		}
		else
		{
			inp = &in;
			outp = &out;
		}
		i = 0;
		bool inb = true;
		vector<pair<shipres, float>>& input = *inp;
		vector<pair<shipres, float>>& output = *outp;
		bool outputbreak =true;
		while (i < output.size())
		{
			if (MapFind(sh.limit, output[i].first))
			{
				if (!(sh.limit[output[i].first] < sh.res[output[i].first]))
				{
					outputbreak = false;
				}

			}
			else
				outputbreak = false;
			i++;
		}
		if (outputbreak)
			return;
		i = 0;
		while (i < inp->size())
		{
			if (MapFind(sh.res, input[i].first))
			{
				if (sh.res[input[i].first] < input[i].second)
				{
					inb = false;
					break;
				}
			}
			else
			{
				inb = false;
				break;
			}
			i++;
		}
		i = 0;
		if (inb)
		{
			while (i < output.size())
			{
				AddToMap(sh.res, output[i]);
				i++;
			}
			i = 0;
			while (i < input.size())
			{
				sh.res[input[i].first] -= input[i].second;
				i++;
			}
			i = 0;
		}

	}
	extern 	map<string, species*> specmap;
	species* species::init(AZfile f, bool addtomap)
	{
		string l = "SKILLS@";
		srepair = f.GetVar<float>(l + "repair");
		sweapon = f.GetVar<float>(l + "weapon");
		sfight = f.GetVar<float>(l + "fight");
		spilot = f.GetVar<float>(l + "pilot");
		sother = f.GetVar<float>(l + "other");
		
		maxincrease = f.GetVar<float>(l + "maxincrease");
		basichp = f.GetVar<float>("hp");
		maxhp = f.GetVar<float>("mhp");
		damagedbyoxygen = f.GetVar<bool>("damagedbyoxygen");
		name = f.GetVar<string>("name");
		if (f.findvar("debuff"))
			combatdebuff = f.GetVar<float>("debuff");
		oxygenneeded = f.GetVar<float>("usedoxygen");
		if(addtomap)
			specmap.insert(make_pair(name, this));
		int i = 0;
		 l = reverseSTR(copyfromchtoend(reverseSTR(f.link),'\\',true)) + "topdown";
		WIN32_FIND_DATA data;
		HANDLE h;
		while (((h=FindFirstFileA((l+ INTtoSTR(i) + ".png").c_str(), &data))) != (HANDLE)ERROR_FILE_NOT_FOUND)
		{
			if (h == (HANDLE)INVALID_HANDLE_VALUE || h == (HANDLE)ERROR_FILE_NOT_FOUND)
				break;
			bmvec.push_back(l + INTtoSTR(i) + ".png");
			bmdirmap.insert(make_pair("topdown" + INTtoSTR(i), bmvec[bmvec.size() - 1]));
			i++;
		}
		
		return this;
	}
}
namespace GAME 
{

	namespace selecting 
	{
		physics pcshipclick;
		crewman* scm;
		vector<crewman*> cmv;
		vector<crewman*> cmvlast;
		int lasti = 0;
	} 
}
namespace DX2D
{
	void mousehit(physobj* pobj1, physobj* pobj2)
	{
		physobj* obj = (pobj1->rectID == 666) ? pobj1 : pobj2 ; 
		entity ent = boost::any_cast<entity>(obj->anyvars[0]);
		crewman* cm = boost::any_cast<crewman*>(ent.datav[0]);
		GAME::selecting::cmv.push_back(cm);
	}
}
namespace GAME
{
	ui movetype = 0;
	extern camera* mcam;
	extern 	physics* mpclass;
	extern universe* uniclass;
	extern map<ui, void(*)(int2&)> funcidmap; 
	extern ship* conship;
	map<physobj*, ui> sshps;
	namespace selecting
	{
		physobj* obj  = nullptr;
		vector<physobj*> objs = {};
		int lastobjp = 0;
		int2* mousepos = nullptr;
	}
	int4 camrect = int4{ 0,0,0,0};
	void selectship(int2& pos,entity&ent)		
	{
		ship* s = boost::any_cast<ship*>( ent.datav[0]);
		if (conship == s)
		{
			int2 posc = rotate_point((float)conship->pos->x + ((float)conship->size.width / 2), (float)conship->pos->x +
				((float)conship->size.width / 2), *conship->rot, pos);
			 *s->mousepos = posc;
			 conship->crewphys.tick();
			 if (selecting::cmv.size() == 0 )
			 {
				 return;
			 }
			 else if (selecting::cmv.size() == 1)
			 {
				if((crewman*)( selecting::scm = (selecting::scm == selecting::cmv[0]) ? nullptr : selecting::cmv[0] )!= nullptr)
					conship->dwnd->setvar(selecting::scm->name, "concrewman", true, "STR");
				else 
					conship->dwnd->setvar((string)"none", "concrewman", true, "STR");

			 }
			 else
			 {
				 selecting::cmv.push_back(nullptr);
				 if (selecting::cmv == selecting::cmvlast)
				 {
					VecProceed(selecting::lasti, selecting::cmv);
					selecting::scm = selecting::cmv[selecting::lasti];
					conship->dwnd->setvar(selecting::scm->name, "concrewman", true, "STR");
				 }
				 else
				 {
					 selecting::lasti = 0;
					 selecting::cmvlast = selecting::cmv;
					selecting::scm = selecting::cmv[0];
					conship->dwnd->setvar(selecting::scm->name, "concrewman", true, "STR");
				 }
			 }
		}
		else
		{
			conship = s;
			debugging::pdebugdata->setvar(conship->name, "conship", true, "STR");
			return;
		}
	}
	void shipselecthit(physobj* obj1, physobj* obj2)
	{
		if (FindInVec(selecting::objs, (obj1->rectID == 666) ? obj2 : obj1))
			return;
		selecting::objs.push_back((obj1->rectID == 666) ? obj2 : obj1);
	}
	void select(int2& _pos)
	{
		if (!classvariables::isinside(camrect, _pos))
			return;
		int2 pos = (((uni2<float>)_pos - (uni2<float>)*mcam->GetXYp() + (uni2<float>)mcam->GetOffset())
			*(uni2<float>)mcam->scale).toint2();
		if (selecting::pcshipclick.FindOBJ("mouse"))
			*selecting::mousepos = pos;
		else
			selecting::pcshipclick.addobj("mouse", 1.0f, selecting::mousepos = new int2(pos), false, false);
		physobj* m = selecting::pcshipclick.objmap["mouse"];
		m->rectID = mousephysid;
		selecting::objs = {};
		selecting::pcshipclick.tick();
		int i = 0;
		vector<physobj*> &objs = selecting::objs;
		physobj* obj = nullptr;
		if (objs.size() == 0)
			return;
		if (objs.size() == 1)
			obj = objs[0];
		else
		{
			if (selecting::objs == objs || selecting::obj == objs[0])
			{
				VecProceed(selecting::lastobjp, objs);
			}
			else
				obj = objs[selecting::lastobjp];
		
		}
		if (obj == selecting::obj)
		{
			selecting::obj = nullptr;
			return;
		}
		selecting::obj = obj;
		if (obj == NULL)
			obj = objs[0];
		i = 0;
		objs = {};
		i = 0;
		entity ent = boost::any_cast<entity>(obj->anyvars[0]);
		if(ent.selectf != nullptr)
			ent.selectf(pos,ent);
		//selecting::pcshipclick.delOBJ(m->nameindex);
		return;
	}
	int2 oldcampos = {0,0};
	bool cnp = false;
	void cammovesetref(int2& pos)
	{
		oldcampos = pos;
		cout << "oldpos: " << oldcampos << endl;
	}
	void cammove(int2& pos)
	{
		if (!classvariables::isinside(camrect, pos))
			return;
		if (pos == oldcampos)
			return;
		int2 npos = (pos - oldcampos);
		//npos = (int2{ -1,-1 }*npos);
		npos = npos;
		mcam->updatesecposof = false;
		mcam->SetX(mcam->GetX() + npos.x);
		mcam->SetY(mcam->GetY() + npos.y);
		mcam->updatesecposof = true;
		cout << "campos" << mcam->GetXY() << endl;
		oldcampos = pos;  
	}
	void mbmovestop(int2& pos)
	{
		//cnp = true;
	}
	void smovef(int2& _pos)
	{
		if (!classvariables::isinside(camrect, _pos))
			return;
		//int2 pos = (_pos - *mcam->GetXYp() + mcam->GetOffset())*(int2)mcam->scale;
		int2 pos = (_pos + *mcam->GetXYp()) - (int2)((uni2<float>)((uni2<float>)mcam->GetBitmapSize().touni2<float>()*
			(uni2<float>(1.0f,1.0f)-(uni2<float>)mcam->scale)) / 2).toint2();
		ship& sh = *conship;
		camera* cam = sh.shcam;
		int2 p1 = *sh.pos + int2{ (int)sh.size.width / 2,(int)sh.size.height / 2 };
		int2 p2 = pos;
		if (mcam->scale.x != 1.0f || mcam->scale.y != 1.0f)
		{
			p2.x = mcam->scale.x * p2.x + (mcam->GetBitmapSize().x * (1.0f - mcam->scale.x));
			p2.y = mcam->scale.y * p2.y + (mcam->GetBitmapSize().y * (1.0f - mcam->scale.y));
		}
		if (p1 == p2)
			return;
		bool dold = true;
		if (GetKeyState(VK_SHIFT) < 0)
		{
			movetype = mt_movnext;
			dold = false;
		}
		else
			movetype = mt_mov;
		if (GetKeyState(VK_LCONTROL) < 0)
			movetype = mt_rot;
		if (movetype != mt_rot)
		{
			int4 sr = { (int)sh.pos->x- (int)(float)(sh.size.width*0.5),sh.pos->y - (int)(float)(sh.size.height*0.5),
				sh.pos->x + (int)(float)(sh.size.width*1.5),sh.pos->y + (int)(float)(sh.size.height*1.5) };
			if (classvariables::isinside(sr, pos))
			{
				if (GAME::selecting::scm == nullptr)
					return;
				crewman* cm = GAME::selecting::scm;
				path pa;
				if ((bool)(pa = sh.psystem.FindPath(*cm->pos, pos - *sh.pos, true)))
				{
					cm->actions.push_back(make_pair(caction::move, (boost::any)pa));
				}
				return;
			}
		}
		//float angle = 0;
		//angle = atan2(p1.y - p2.y, p1.x - p2.x);
		//angle = XMConvertToDegrees(angle);
		//angle -= 90;
		//constraintoval(angle, 360.0f);
		int i = 0;
		if (dold)
		{
			while (i < sh.moves.size())
			{
				sh.moves[i].Release(sh.lineframe);
				i++;
			}
			sh.moves.clear();
		}
		i = 0;
		{
			smove m;
			m.trot = new int2(p2);
			if (movetype != mt_movnext)
				sh.moves.clear();
			sh.moves.push_back(m);
		}
		if (movetype == mt_mov || movetype == mt_movnext)
		{
			smove m;
			m.tpos = new int2(p2);
			sh.moves.push_back(m);
		}
	}
}