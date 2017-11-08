#include "universe.h"
//#include "hmain.h"
#include "ships.h"
#include "X:\PROJECTS\economy\economy\economy.h"
namespace debugging
{
	extern int2** debugpos;
}
namespace DX2D
{
	extern map<string, stationmodtempIN>stattempINs;
	extern map<string, stationmodtempOUT>stattempOUTs;
	extern map<string, stationmodtempIN>coretemps;
}
using namespace DX2D;
namespace GAME
{
	extern vector<entity> entitylist;
	template<typename univar>
	void station::addtostorage(univar* mod)
	{
		STORAGE = SumMaps(STORAGE, mod->STORAGE);
		storagelimitm = SumMaps(storagelimitm, mod->storagelimitm);
		int i = 0;
		while (i < mod->slotsin.size())
		{
			if (mod->slotsin[i]->mod != nullptr)
				addtostorage(mod->slotsin[i]->mod);
			i++;
		}
	}
	template<>
	void station::addtostorage(stationmodOUT* mod)
	{
		STORAGE = SumMaps(STORAGE, mod->STORAGE);
		storagelimitm = SumMaps(storagelimitm, mod->storagelimitm);
		int i = 0;
		while (i < mod->slotsin.size())
		{
			if (mod->slotsin[i]->mod != nullptr)
				addtostorage(mod->slotsin[i]->mod);
			i++;
		}
	}
	void station::createmods(AZfile& af, string datapos)
	{
		if (datapos[datapos.size() - 1] != '@')
			datapos += '@';
		name = af.GetVar<string>(datapos + "name");
		core = new stationmodIN(coretemps[af.GetVar<string>(datapos + "core")], new int2(0, 0));
		core->loadsubmods(af, datapos);
		addtostorage(core);
		int i = 0;
		while (i < core->slotsin.size())
		{
			if (core->slotsin[i]->mod != nullptr)
				addtostorage(core->slotsin[i]->mod);
			i++;
		}
		i = 0;
		while (i < core->slotsout.size())
		{
			if (core->slotsout[i]->mod != nullptr)
				addtostorage(core->slotsout[i]->mod);
			i++;
		}
		i = 0;
		storagelimit = MaptoVec(storagelimitm);
	}
	void station::tick()
	{
		int i = 0;
		while (i < sINv.size())
		{
			sINv[i]->tick();
			i++;
		}
		i = 0;
		while (i < sOUTv.size())
		{
			sOUTv[i]->tick();
			i++;
		}
	}
	void stationmodOUT::PhysInit(physics& pclass,int2 possoffset, string s)
	{
		if (dummyship == nullptr)
		{
			dummyship = new ship();
			dummyship->makedummy();
		}
		if (s == "")
		{
			s = stationname + name;
		}
		else
			s += name;
		pclass.addobj(s, new int2(size),new int2(*pos+possoffset), false, false);
		pobj = pclass.GetLastObj();
		physreact ph;
		ph.callfunc = true;
		ph.ftocall = shipselecthit;
		ph.callfuncinstant = true;
		pobj->isron = true;
		entity ent;
		ent.selectf = GAME::selectship;
		ent.datav.push_back(dummyship);
		dummyship->pobj = pobj;
		pobj->IDreactionmap.insert(make_pair(mousephysid, ph));
		ent.pos = pos;
		ent.entname = "station module";
		ent.datav.push_back(this);
		ent.datav.push_back(entitytype::statmout);
		entitylist.push_back(ent);
		pobj->anyvars.push_back(ent);
		pobj->anyvars.push_back(this);
		int i = 0;
		i = 0;
		while (i < slotsout.size())
		{
			slotsout[i]->mod->PhysInit(pclass, possoffset,s);
			i++;
		}
		i = 0;
	}
	void station::RenderInit(frame* mf, camera* cam)
	{
		int i = 0;
		if (f == nullptr)
			f = new frame;
		if (mf->wchiac == "")
		{
			mf->wchiac = "station" + RandomString(3);
		}
		mf->ischfact = true;
		mf->f[mf->wchiac].push_back(f);
		core->pos = pos;
		f->ismactive = true;
		f->ischfact = true;
		auto fakeslot = new stationmodINslot;
		fakeslot->slotrot = 0.0f;
		fakeslot->pos = new int2(0, 0);
		while (i < core->slotsout.size())
		{
			*core->slotsout[i]->pos = *core->slotsout[i]->pos * stationsizemultip;
			core->slotsout[i]->pos->y -= core->slotsout[i]->mod->size.y/2;
			i++;
		}
		i = 0;
		while (i < core->slotsin.size())
		{
			*core->slotsin[i]->pos = *core->slotsin[i]->pos * stationsizemultip;
			//core->slotsout[i]->pos->y -= core->slotsout[i]->mod->size.y / 2;
			i++;
		}
		core->RenderInit(cam, f, name, fakeslot, this, true);
		//debugging::debugpos = /*pos;*/ f->sprites[f->sprites.size() - 1].GetXYp();
		i = 0;
		entity ent;
		ent.pos = pos;
		ent.entname = name;
		ent.datav.push_back(this);
		entitylist.push_back(ent);
		delete fakeslot->pos;
		delete fakeslot;
		return;
	}
	void station::RenderEnd()
	{
		int i = 0;
		while (i < core->slotsin.size())
		{
			core->slotsin[i]->mod->RenderRelease();
			i++;
		}
		i = 0;
		while (i < core->slotsout.size())
		{
			core->slotsout[i]->mod->RenderRelease();
			i++;
		}

	}
	//map<string, stationmodtempOUT*>smOUTtemplates;
	//map<string, stationmodtempIN*>smINtemplates;
	void stationmodIN::loadsubmods(AZfile& af, string lk)
	{
		int i = 0;
		if (lk[lk.size() - 1] != '@')
			lk += '@';
		while (i < slotsin.size())
		{
			string str = lk + "IS" + INTtoSTR(i) + "@core";
			int ii = i;
			string type;
			if(af.GetVarSafe<string>(type,str))
			{
				if (slotsin[ii]->mod == nullptr && slotsin[ii]->slotsize == stattempINs[type].slotsize);
				{
					slotsin[ii]->mod = new stationmodIN(stattempINs[af.GetVar<string>(lk + "IS" + INTtoSTR(i) + "@core")], slotsin[ii]->pos);
					slotsin[ii]->mod->loadsubmods(af, lk + "IS" + INTtoSTR(i) + '@');
					//break;
				}
				ii++;
			}
			i++;
		}
		i = 0;
		while (i < slotsout.size())
		{
			int ii = i;
			if (lk[lk.size() - 1] != '@')
				lk += '@';
			string str = lk + "OS" + INTtoSTR(i) + "@core";
			string type;
			if (af.GetVarSafe<string>(type, str))
			{
				if (slotsout[ii]->mod == nullptr && slotsout[ii]->slotsize == stattempINs[type].slotsize);
				{
					slotsout[ii]->mod = new stationmodOUT(stattempOUTs[af.GetVar<string>(lk + "OS" + INTtoSTR(i) + "@core")], slotsout[ii]->pos);
					slotsout[ii]->mod->loadsubmods(af, lk + "OS" + INTtoSTR(i) + '@');
					//break;
				}
				ii++;
			}
			i++;
		}
	}
	void stationmodOUT::loadsubmods(AZfile& af, string lk)
	{
		int i = 0;
		if (lk[lk.size() - 1] != '@')
			lk += '@';
		while (i < slotsin.size())
		{
			string str = lk + "IS" + INTtoSTR(i) + "@core";
			int ii = i;
			string type;
			if (af.GetVarSafe<string>(type, str))
			{
				if (slotsin[ii]->mod == nullptr && slotsin[ii]->slotsize == stattempINs[type].slotsize);
				{
					slotsin[ii]->mod = new stationmodIN(stattempINs[af.GetVar<string>(lk + "IS" + INTtoSTR(i) + "@core")], slotsin[ii]->pos);
					slotsin[ii]->mod->loadsubmods(af, lk + "IS" + INTtoSTR(i) + '@');
					break;
				}
				ii++;
			}
			i++;
		}
		i = 0;
		while (i < slotsout.size())
		{
			int ii = i;
			if (lk[lk.size() - 1] != '@')
				lk += '@';
			string str = lk + "OS" + INTtoSTR(i) + "@core";
			string type;
			if (af.GetVarSafe<string>(type, str))
			{
				if (slotsout[ii]->mod == nullptr && slotsout[ii]->slotsize == stattempINs[type].slotsize);
				{
					slotsout[ii]->mod = new stationmodOUT(stattempOUTs[af.GetVar<string>(lk + "OS" + INTtoSTR(i) + "@core")], slotsout[ii]->pos);
					slotsout[ii]->mod->loadsubmods(af, lk + "OS" + INTtoSTR(i) + '@');
					break;
				}
				ii++;
			}
			i++;
		}
	}
	void sub::tick(stationmodOUT *base)
	{
		int i = 0;
		ui crewb = (curcrew > maxcrew) ? maxcrew - mincrew : curcrew - mincrew;
		vector<pair<stationres, float>>* INPUTp;
		vector<pair<stationres, float>>* OUTPUTp;
		map<stationres, float>& STORAGE = base->STORAGE;
		i = 0;
		if (crewb != lastcrew || hpd != hpdstatel)
		{
			if (curcrew < mincrew)
				return;
			if (hpd != -1)
			{
				if (hp <= hpd)
				{
					if (INPUTd.size() > 0)
						INPUTl = INPUTd;
					if (OUTPUTd.size() > 0)
						OUTPUTl = OUTPUTd;
				}
			}
			else
			{
				INPUTl = INPUT;
				INPUTl = INPUT;
			}
			auto INPm = VecToMap(INPUTl);
			auto OUTm = VecToMap(OUTPUTl);
			i = 0;
			while (i < INPUTb.size())
			{
				pair<stationres, float> pir = INPUTb[i];
				pir.second *= crewb;
				AddToMap(INPm, pir);
				i++;
			}
			i = 0;
			while (i < OUTPUTb.size())
			{
				pair<stationres, float> pir = OUTPUTb[i];
				pir.second *= crewb;
				AddToMap(OUTm, pir);
				i++;
			}
			INPUTl = MaptoVec(INPm);
			OUTPUTl = MaptoVec(OUTm);
		}
		//else
		//{
		//	INPUTp = &INPUTl;
		//	OUTPUTp = &OUTPUTl;
		//}
		//vector<pair<stationres, float>> _INPUT = *INPUTp;
		//vector<pair<stationres, float>> _OUTPUT = *OUTPUTp;
		i = 0;
		while (i < INPUTl.size())
		{
			if (MapFind(STORAGE, INPUTl[i].first))
			{
				if (STORAGE[INPUTl[i].first] < INPUT[i].second)
					return;
			}
			else
				return;
			i++;
		}
		bool isallfull = true;
		i = 0;
		while (i < OUTPUTl.size())
		{
			if (MapFind(STORAGE, INPUTl[i].first))
			{
				if (STORAGE[INPUTl[i].first] != storagelimitm[INPUTl[i].first])
					isallfull = false;
			}
			i++;
		}
		i = 0;

		if (isallfull)
			return;
		while (i < OUTPUTl.size())
		{
			AddToMap(STORAGE, OUTPUTl[i]);
			if (STORAGE[OUTPUT[i].first] > storagelimitm[OUTPUT[i].first])
				STORAGE[OUTPUT[i].first] = storagelimitm[OUTPUT[i].first];
			i++;
		}
		i = 0;
		while (i < INPUTl.size())
		{
			AddToMap(STORAGE, make_pair(INPUTl[i].first, -INPUTl[i].second));
			i++;
		}
		i = 0;
		i = 0;
	}
	void stationmodOUT::tick()
	{
		int i = 0;
		while (i < subs.size())
		{
			subs[i].tick(this);
			i++;
		}
		i = 0;
		{
			ui crewb = (curcrew > maxcrew) ? maxcrew - mincrew : curcrew - mincrew;
			vector<pair<stationres, float>>* INPUTp;
			vector<pair<stationres, float>>* OUTPUTp;
			while (i < slotsin.size())
			{
				if (slotsin[i]->mod != nullptr)
					slotsin[i]->mod->tick();
				i++;
			}
			while (i < slotsout.size())
			{
				if (slotsout[i]->mod != nullptr)
					slotsout[i]->mod->tick();
				i++;
			}
			i = 0;
			if (crewb != lastcrew || hpd != hpdstatel)
			{
				if (curcrew < mincrew)
					return;
				if (hpd != -1)
				{
					if (hp <= hpd)
					{
						if (INPUTd.size() > 0)
							INPUTl = INPUTd;
						if (OUTPUTd.size() > 0)
							OUTPUTl = OUTPUTd;
					}
				}
				else
				{
					INPUTl = INPUT;
					INPUTl = INPUT;
				}
				auto INPm = VecToMap(INPUTl);
				auto OUTm = VecToMap(OUTPUTl);
				i = 0;
				while (i < INPUTb.size())
				{
					pair<stationres, float> pir = INPUTb[i];
					pir.second *= crewb;
					AddToMap(INPm, pir);
					i++;
				}
				i = 0;
				while (i < OUTPUTb.size())
				{
					pair<stationres, float> pir = OUTPUTb[i];
					pir.second *= crewb;
					AddToMap(OUTm, pir);
					i++;
				}
				INPUTl = MaptoVec(INPm);
				OUTPUTl = MaptoVec(OUTm);
			}
			//else
			//{
			//	INPUTp = &INPUTl;
			//	OUTPUTp = &OUTPUTl;
			//}
			//vector<pair<stationres, float>> _INPUT = *INPUTp;
			//vector<pair<stationres, float>> _OUTPUT = *OUTPUTp;
			i = 0;
			while (i < INPUTl.size())
			{
				if (MapFind(STORAGE, INPUTl[i].first))
				{
					if (STORAGE[INPUTl[i].first] < INPUT[i].second)
						return;
				}
				else
					return;
				i++;
			}
			bool isallfull = true;
			i = 0;
			while (i < OUTPUTl.size())
			{
				if (MapFind(STORAGE, INPUTl[i].first))
				{
					if (STORAGE[INPUTl[i].first] != storagelimitm[INPUTl[i].first])
						isallfull = false;
				}
				i++;
			}
			i = 0;

			if (isallfull)
				return;
			while (i < OUTPUTl.size())
			{
				AddToMap(STORAGE, OUTPUTl[i]);
				if (STORAGE[OUTPUT[i].first] > storagelimitm[OUTPUT[i].first])
					STORAGE[OUTPUT[i].first] = storagelimitm[OUTPUT[i].first];
				i++;
			}
			i = 0;
			while (i < INPUTl.size())
			{
				AddToMap(STORAGE, make_pair(INPUTl[i].first, -INPUTl[i].second));
				i++;
			}
			i = 0;
			i = 0;
		}
	}
	void stationmodOUT::RenderInit (camera* ncam, frame* nf, string nstationname, stationmodOUTslot* slot, station* stat, bool iscore)
	{
		statptr = stat;
		stationname = "";
		stationname = nstationname;
		cam = ncam;
		f = nf;
		pos = slot->pos;
		sprite tx;
		tx.usecustompoint = true;
		tx.customrpoint = { 0,sizet.y / 2 };
		tx.SetBitmapFromFile(STRtoWSTR(textureloc).c_str(), *cam->GetRenderTargetP());
		tx.size = (D2D_SIZE_F)(sizet/**stationsizemultip*/);
		if (!iscore)
			tx.SetOffsetXYp(stat->pos, true);
		tx.SyncPos(pos, false);
		tx.forceoverridepos = true;
		tx.overridepos = pos;
		slot->slotrot += 270;
		constraintoscope(slot->slotrot, 360.0f, 0.0f);
		tx.SetRot(slot->slotrot);
		//tx.breakonrender = true;
		renderp = new bool(true);
		identp = new bool(true);
		tx.render = renderp;
		tx.identp = identp;
		tx.useidentp = true;
		nf->sprites.push_back(tx);
		debugging::debugpos = nf->sprites[nf->sprites.size()-1].GetXYpp();
		//cout << pos->str() << endl;
		//random_shuffle(nf->sprites.begin(), nf->sprites.end());
		auto sptr = nf->sprites[nf->sprites.size() - 1].GetXYpp();
		int i = 0;
		while (i < slotsin.size())
		{
			if (slotsin[i]->mod != nullptr)
				slotsin[i]->mod->RenderInit(ncam, nf, nstationname, slotsin[i],stat);
			i++;
		}
		i = 0;
		while (i < slotsout.size())
		{
			if (slotsout[i]->mod != nullptr)
				slotsout[i]->mod->RenderInit(ncam, nf, nstationname, slotsout[i],stat);
			i++;
		}
		return;
	}
	void stationmodIN::RenderInit(camera* ncam, frame* nf, string nstationname, stationmodINslot* slot, GAME::station* stat, bool iscore )
	{
		statptr = stat;
		stationname = "";
		stationname = nstationname;
		cam = ncam;
		f = nf;
		sprite tx;
		tx.SetBitmapFromFile(STRtoWSTR(textureloc).c_str(), *cam->GetRenderTargetP());
		tx.size = (D2D_SIZE_F)(sizet*stationsizemultip);
		if(!iscore)
			tx.SetOffsetXYp(stat->pos, true);
		tx.SyncPos(pos, false);
		constraintoscope(slot->slotrot, 360.0f, 0.0f);
		tx.SetRot(slot->slotrot);
		renderp = new bool(true);
		identp = new bool(true);
		tx.render = renderp;
		tx.identp = identp;
		tx.useidentp = true;
		//tx.breakonrender = true;
		nf->sprites.push_back(tx);
		int i = 0;
		while (i < slotsin.size())
		{
			if (slotsin[i]->mod != nullptr)
				slotsin[i]->mod->RenderInit(ncam, nf, nstationname, slotsin[i], stat);
			i++;
		}
		i = 0;
		while (i < slotsout.size())
		{
			if (slotsout[i]->mod != nullptr)
				slotsout[i]->mod->RenderInit(ncam, nf, nstationname, slotsout[i], stat);
			i++;
		}
		return;
	}
	void stationmodOUT::RenderRelease()
	{
		*identp = false;
		delete renderp;
		renderp = nullptr;
		int i = 0;
		identp = nullptr;
		while (i < slotsout.size())
		{
			if (slotsout[i]->mod != nullptr)
				slotsout[i]->mod->RenderRelease();
			i++;
		}
		i = 0;

		while (i < slotsout.size())
		{
			if (slotsin[i]->mod != nullptr)
				slotsin[i]->mod->RenderRelease();
			i++;
		}
	}
	void stationmodIN::tick()
	{
		ui crewb = (curcrew > maxcrew) ? maxcrew - mincrew : curcrew - mincrew;
		vector<pair<stationres, float>>* INPUTp;
		vector<pair<stationres, float>>* OUTPUTp;
		int i = 0;
		while (i < slotsin.size())
		{
			if (slotsin[i]->mod != nullptr)
				slotsin[i]->mod->tick();
			i++;
		}
		while (i < slotsout.size())
		{
			if (slotsout[i]->mod != nullptr)
				slotsout[i]->mod->tick();
			i++;
		}
		i = 0;
		if (crewb != lastcrew || hpd != hpdstatel)
		{
			if (curcrew < mincrew)
				return;
			if (hpd != -1)
			{
				if (hp <= hpd)
				{
					if (INPUTd.size() > 0)
						INPUTl = INPUTd;
					if (OUTPUTd.size() > 0)
						OUTPUTl = OUTPUTd;
				}
			}
			else
			{
				INPUTl = INPUT;
				INPUTl = INPUT;
			}
			auto INPm = VecToMap(INPUTl);
			auto OUTm = VecToMap(OUTPUTl);
			i = 0;
			while (i < INPUTb.size())
			{
				pair<stationres, float> pir = INPUTb[i];
				pir.second *= crewb;
				AddToMap(INPm, pir);
				i++;
			}
			i = 0;
			while (i < OUTPUTb.size())
			{
				pair<stationres, float> pir = OUTPUTb[i];
				pir.second *= crewb;
				AddToMap(OUTm, pir);
				i++;
			}
			INPUTl = MaptoVec(INPm);
			OUTPUTl = MaptoVec(OUTm);
		}
		//else
		//{
		//	INPUTp = &INPUTl;
		//	OUTPUTp = &OUTPUTl;
		//}
		//vector<pair<stationres, float>> _INPUT = *INPUTp;
		//vector<pair<stationres, float>> _OUTPUT = *OUTPUTp;
		i = 0;
		while (i < INPUTl.size())
		{
			if (MapFind(STORAGE, INPUTl[i].first))
			{
				if (STORAGE[INPUTl[i].first] < INPUT[i].second)
					return;
			}
			else
				return;
			i++;
		}
		bool isallfull = true;
		i = 0;
		while (i < OUTPUTl.size())
		{
			if (MapFind(STORAGE, INPUTl[i].first))
			{
				if (STORAGE[INPUTl[i].first] == storagelimitm[INPUTl[i].first])
					isallfull = false;
			}
			i++;
		}
		i = 0;

		if (isallfull)
			return;
		while (i < OUTPUTl.size())
		{
			AddToMap(STORAGE, OUTPUTl[i]);
			if (STORAGE[OUTPUT[i].first] > storagelimitm[OUTPUT[i].first])
				STORAGE[OUTPUT[i].first] = storagelimitm[OUTPUT[i].first];
			i++;
		}
		i = 0;
		while (i < INPUTl.size())
		{
			AddToMap(STORAGE, make_pair(INPUTl[i].first, -INPUTl[i].second));
			i++;
		}
		i = 0;
		i = 0;
	}
}