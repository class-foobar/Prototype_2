#include "universe.h"
#include "hmain.h"
#include "X:\PROJECTS\economy\economy\economy.h"
namespace DX2D
{
	extern map<string, stationmodtempIN>stattempINs;
	extern map<string, stationmodtempOUT>stattempOUTs;
	extern map<string, stationmodtempIN>coretemps;
}
using namespace DX2D;
namespace GAME
{
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
	void stationmodOUT::RenderInit (camera* ncam, frame* nf, string nstationname, stationmodOUTslot* slot)
	{
		stationname = "";
		stationname = nstationname;
		cam = ncam;
		f = nf;
		sprite tx;
		tx.SetBitmapFromFile(STRtoWSTR(textureloc).c_str(), *cam->GetRenderTargetP());
		tx.size = (D2D_SIZE_F)(sizet*stationsizemultip);
		tx.SetOffsetXYp(new int2(int2{ 0,0 }-postm), true);
		tx.SyncPos(pos, false);
		tx.forceoverridepos = true;
		tx.overridepos = pos;
		tx.SetRot(slot->slotrot);
		renderp = new bool(true);
		identp = new bool(true);
		tx.render = renderp;
		tx.identp = identp;
		tx.useidentp = true;
		tx.savelastloc = true;
		tx.breakonlocchange = true;
		nf->sprites.push_back(tx);
		random_shuffle(nf->sprites.begin(), nf->sprites.end());
		auto sptr = nf->sprites[nf->sprites.size() - 1].GetXYpp();
		int i = 0;
		while (i < slotsin.size())
		{
			if (slotsin[i]->mod != nullptr)
				slotsin[i]->mod->RenderInit(ncam, nf, nstationname, slotsin[i]);
			i++;
		}
		i = 0;
		while (i < slotsout.size())
		{
			if (slotsout[i]->mod != nullptr)
				slotsout[i]->mod->RenderInit(ncam, nf, nstationname, slotsout[i]);
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