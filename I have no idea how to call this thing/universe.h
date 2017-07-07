#pragma once
#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "ships.h"
#include "physics.h"
#include "text.hpp"
#include "X:\PROJECTS\economy\economy\economy.h"
using namespace std;
using namespace GAME;
using namespace AZfilelib;
using namespace DX2D;
using namespace D2D1;
namespace GAME
{
	class stationmodtempIN;
	class stationmodtempOUT;
}
namespace DX2D
{
	extern main* DXclass;
	//extern GAME::universe* uniclass;
	void loadsavebts(int2& pos);
	//extern map<string, stationmodtempIN>stattempINs;
	//extern map<string, stationmodtempOUT>stattempOUTs;
	//extern map<string, stationmodtempIN>coretemps;
}
namespace GAME
{
	extern	vector<entity> entitylist;
	class universe;
	void loadsector(camera* cam, frame* mf, int2 pos, GAME::universe* uniptr, GAME::economy* ecoptr, GAME::system* sysptr, GAME::space* spptr, DX2D::ship& player);
	class starcol
	{
	public:
		int3 min;
		int3 max;
	};
	class planet
	{
	public:
		node* n;
		ui seed;
		int2* pos;
		float* rot;
		float orbang;
		void* orbobj;
		int mass;
		lli atmospheresize;
		int128 orbrad;
		int128 radius;
		bool isalive;
		ui type;
	};
	enum class stationres
	{
		nullres = 0,
		hydrogen = 1,
		uranium = 2,
		plutonium = 3,
		power = 4,
		thrust = 5,
		oxygen = 6,
		crew = 7
	};
	inline string getstringfromstationres(stationres res)
	{
		switch ((ui)res)
		{
		case (ui)stationres::hydrogen:
			return "hydrogen";
		case (ui)stationres::uranium:
			return "uranium";
		case (ui)stationres::plutonium:
			return "plutonium";
		case (ui)stationres::power:
			return "power";
		case (ui)stationres::thrust:
			return "thrust";
		case (ui)stationres::oxygen:
			return "oxygen";
		case (ui)stationres::crew:
			return "crew";
		}
		return "ERROR";
	}
	inline stationres strtostationres(string str)
	{
		str = tolower(str);
		if (str == "hydrogen")
			return stationres::hydrogen;
		else if (str == "uranium")
			return stationres::uranium;
		else if (str == "plutonium")
			return stationres::plutonium;
		else if (str == "power" || str == "energy")
			return stationres::power;
		else if (str == "thrust")
			return stationres::thrust;
		else if (str == "oxygen")
			return stationres::oxygen;
		else if (str == "crew")
			return stationres::crew;

		return stationres::nullres;
	}
	inline int2 getmodsizefi(int s);
	void loadinoutputs(pair<vector<pair<stationres, float>>&, string>INPUT, pair<vector<pair<stationres, float>>&, string>INPUTd,
		pair<vector<pair<stationres, float>>&, string>INPUTb, pair<vector<pair<stationres, float>>&, string>OUTPUT,
		pair<vector<pair<stationres, float>>&, string>OUTPUTd, pair<vector<pair<stationres, float>>&, string>OUTPUTb, pair<vector<pair<stationres, float>>&, string>STORAGE, AZfile& f, string loc = "");
	//class stationmodIN;
	//class stationmodOUT;
	class sub
	{
	private:
		int lastcrew = -1;
		vector<pair<stationres, float>> INPUTl;
		vector<pair<stationres, float>> OUTPUTl;
		bool hpdstatel = false;
	public:
		vector<pair<stationres, float>> INPUT;
		vector<pair<stationres, float>> OUTPUT;
		vector<pair<stationres, float>> INPUTb; // input bonus per crewman, can be negative;
		vector<pair<stationres, float>> OUTPUTb; // same as above
		vector<pair<stationres, float>> INPUTd;
		vector<pair<stationres, float>> OUTPUTd;
		vector<pair<stationres, float>> storagelimit;
		map<stationres, float> storagelimitm;
		//map<stationres, float> STORAGE;
		float hp = 1;
		float hpd = 0;
		ui mincrew;
		ui maxcrew;
		ui curcrew = 0;
		ui slotsize = -1; // 
		int2 pos;
		int2 size;
		void tick(stationmodOUT* base);
	};

	class stationmodOUTslot
	{
	public:
		int2* pos;
		stationmodOUT* mod = nullptr;
		int slotnum;
		float slotrot = 0;
		int slotsize;
	};
	class stationmodINslot
	{
	public:
		int2* pos;
		stationmodIN* mod = nullptr;
		int slotnum;
		float slotrot = 0;
		int slotsize;
	};
	extern int dbxyz; // if you remove this line the compiler will... well... http://i.imgur.com/snFoWUG.png
	class stationmodtempOUT
	{
		friend class stationmodOUT;
	public:
		string name = "";
		vector<pair<stationres, float>> INPUT;
		vector<pair<stationres, float>> OUTPUT;
		vector<pair<stationres, float>> INPUTb; // input bonus per crewman, can be negative;
		vector<pair<stationres, float>> OUTPUTb; // same as above
		vector<pair<stationres, float>> INPUTd;
		vector<pair<stationres, float>> OUTPUTd;
		vector<pair<stationres, float>> storagelimit;
		vector<pair<int2, int>> slots; // pos, size
		vector<float> slotsrot;
		ui mincrew = 0;
		ui maxcrew = 0;
		string textureloc = "";
		string outsideloc = "NULL";
		int2 pos;
		int2 size;
		vector<pair<int2, int2>> docks;// pos, size
		int2 sizet;
		int2 posin;
		float ang;
		float hp = 0;
		vector<sub> subs;
		stationmodtempOUT()
		{

		}
		stationmodtempOUT(string loc)
		{
			if (loc[loc.size() - 1 != '\\'] && loc[loc.size() - 1] != '/')
				loc += '\\';
			WIN32_FIND_DATA wbs1;
			HANDLE hfind;
			if (((hfind = FindFirstFileA((loc + "core.data").c_str(), &wbs1)) != INVALID_HANDLE_VALUE) && hfind != (HANDLE)ERROR_FILE_NOT_FOUND)
				loc += "core.data";
			else
				loc += "module.data";
			AZfile f = loc;
			loadinoutputs({ INPUT,"INPUT" }, { INPUTd,"INPUTd" }, { INPUTb,"INPUTb" }, { OUTPUT,"OUTPUT" }, { OUTPUTd,"OUTPUTd" }, { OUTPUTb,"OUTPUTb" }, {storagelimit,"STORAGE" }, f);
			int i = 0;
			//maxcrew = f.GetVar<ui>("maxcrew");
			//mincrew = f.GetVar<ui>("mincrew");
			f.GetVarSafe(maxcrew, "maxcrew");
			f.GetVarSafe(mincrew, "mincrew");
			while (f.findstruct("SUB@s" + INTtoSTR(i)))
			{
				string str = "SUB@s" + INTtoSTR(i) + '@';
				int2 spos(f.GetVar<int>(str + "posX"), f.GetVar<int>(str + "posY"));
				int2 ssize(f.GetVar<int>(str + "sizeX"), f.GetVar<int>(str + "sizeY"));
				string storageloc = "SUB@" + f.GetVar<string>(str + "storage");
				string INPUTloc = "SUB@" + f.GetVar<string>(str + "in");
				string INPUTdloc = "SUB@" + f.GetVar<string>(str + "ind");
				string INPUTbloc = "SUB@" + f.GetVar<string>(str + "inb");
				string OUTPUTloc = "SUB@" + f.GetVar<string>(str + "out");
				string OUTPUTdloc = "SUB@" + f.GetVar<string>(str + "outd");
				string OUTPUTbloc = "SUB@" + f.GetVar<string>(str + "outb");
				sub s;
				loadinoutputs({ s.INPUT,INPUTloc }, { s.INPUTd,INPUTdloc }, { s.INPUTb,INPUTbloc }, { s.OUTPUT,OUTPUTloc },
				{ s.OUTPUTd,OUTPUTdloc }, { s.OUTPUTb,OUTPUTbloc }, { s.storagelimit,storageloc }, f);
				s.hp = f.GetVar<float>(str + "hp");
				s.hpd = f.GetVar<float>(str + "hpd");
				s.maxcrew = f.GetVar<ui>(str + "maxcrew");
				s.mincrew = f.GetVar<ui>(str + "mincrew");
				s.pos = spos;
				s.size = ssize;
				subs.push_back(s);
				if (f.GetVar<bool>(str + "issymetricalH"))
				{
					sub _s = s;
					int dist = ssize.x - spos.x;
					_s.pos = ssize + int2{dist, 0};
					subs.push_back(_s);
				}
				if (f.GetVar<bool>(str + "issymetricalV"))
				{
					sub _s = s;
					int dist = ssize.y - spos.y;
					_s.pos = ssize + int2{0, dist};
					subs.push_back(_s);
				}
				i++;
			}
			while (f.findstruct("d" + INTtoSTR(i)))
			{
				string str = "d" + INTtoSTR(i) + '@';
				int2 spos(f.GetVar<int>(str + "posX"), f.GetVar<int>(str + "posY"));
				int2 ssize(f.GetVar<int>(str + "sizeX"), f.GetVar<int>(str + "sizeY"));
				docks.push_back(make_pair(spos, ssize));
				if (f.GetVar<bool>(str + "issymetricalH"))
				{
					int dist = size.x - spos.x;
					docks.push_back(make_pair( spos + int2{ dist, 0 },ssize));
				}
				if (f.GetVar<bool>(str + "issymetricalV"))
				{
					int dist = ssize.y - spos.y;
					docks.push_back(make_pair(spos + int2{ 0, dist }, ssize));

				}
				i++;
			}
			ang = f.GetVar<float>("INangle");
			hp = f.GetVar<float>("hp");
			posin = int2(f.GetVar<int>("INposX"), f.GetVar<int>("INposY"));
			sizet = int2(f.GetVar<int>("tsizeX"), f.GetVar<int>("tsizeY"));
			pos = int2(f.GetVar<int>("posX"), f.GetVar<int>("posY"));
			size = sizet = int2(f.GetVar<int>("sizeX"), f.GetVar<int>("sizeY"));
			name = f.GetVar<string>("name");
			string dirloc = reverseSTR(copyfromchtoend(reverseSTR(loc),'\\',true));
			textureloc = dirloc + "module.png";
			WIN32_FIND_DATA windowsbs;
			if (FindFirstFileA((dirloc + "outside.png").c_str(), &windowsbs))
				outsideloc = dirloc + "outside.png";
			i = 0;
			string sltype = "s";
			int iii = 0;
			while (f.findstruct(sltype + INTtoSTR(i)))
			{
				string str = sltype + INTtoSTR(i);
				int ii = 0;
				vector<pair<int2, int>> toadd;
				vector<float> toaddrot;
				toadd.push_back(pair<int2, int>{ int2{ f.GetVar<int>(str + "posX"),f.GetVar<int>(str + "posY") }, f.GetVar<int>(str + "size") });
				toaddrot.push_back(f.GetVar<float>(str + "angle"));
				bool dv = false;
				bool dh = false;
				int2 msize = getmodsizefi(toadd[0].second);
				if (f.GetVarSafe(dv, str + "doublevertically") && dv)
				{
					msize.x *= 2;
				}
				if (f.GetVarSafe(dh, str + "doublehorizontally") && dh)
				{
					msize.y *= 2;
				}

				int copiesX = 0;
				int copiesY = 0;
				int rowsize = f.GetVar<int>(str + "rowsize");
				if (copiesX = f.GetVar<int>(str + "copiesX"))
				{
					ii = 0;
					while (ii < copiesX)
					{
						int2 npos = toadd[0].first;
						npos.x += msize.x + rowsize;
						toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
						toaddrot.push_back(f.GetVar<float>(str + "angle"));
						ii++;
					}
					ii = 0;
				}
				if (copiesY = f.GetVar<int>(str + "copiesY"))
				{
					ii = 0;
					while (ii < copiesY)
					{
						int2 npos = toadd[0].first;
						npos.y += (msize.y + rowsize)*ii;
						toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
						toaddrot.push_back(f.GetVar<float>(str + "angle"));
						ii++;
					}
					ii = 0;
				}
				vector<int2> tddX;
				vector<int2> tddY;
				if (f.GetVar<bool>(str + "issymetricalH"))
				{
					ii = 0;
					while (ii < toadd.size())
					{
						int dist = msize.x - toadd[ii].first.x;
						int2 npos = { toadd[ii].first };
						npos.x += dist * 2;
						tddX.push_back(npos);
						toaddrot.push_back(f.GetVar<float>(str + "angle") + 180);
						ii++;
					}
				}
				if (f.GetVar<bool>(str + "issymetricalV"))
				{
					ii = 0;
					while (ii < toadd.size())
					{
						int dist = msize.y - toadd[ii].first.y;
						int2 npos = { toadd[ii].first };
						npos.y += dist * 2;
						toaddrot.push_back(f.GetVar<float>(str + "angle") + 180);
						tddY.push_back(npos);
						ii++;
					}
				}
				ii = 0;
				while (ii < tddX.size())
				{
					toadd.push_back(make_pair(tddX[ii], toadd[0].second));
					ii++;
				}
				ii = 0;
				while (ii < tddY.size())
				{
					toadd.push_back(make_pair(tddY[ii], toadd[0].second));
					ii++;
				}
				ii = 0;
				while (ii < toadd.size())
				{
					if (sltype == "s")
					{
						slots.push_back(toadd[ii]);
						slotsrot.push_back(toaddrot[ii]);
					}
					ii++;
				}
				i++;
			}
			i = 0;
		}
	};
	inline int2 getmodsizefi(int s)
	{
		if (s == 0)
			return{ 25,25 };
		else if (s == 1)
			return{ 50,50 };
		else if (s == 2)
			return{ 75,75 };
		else if (s == 3)
			return{ 100,100 };
		else if (s == 4)
			return{ 125,125 };
		return{ -1,-1 };
	}
	class stationmodtempIN
	{
		friend class stationmodIN;
	public:
		bool iscore = false;
		string name;
		vector<pair<stationres, float>> INPUT;
		vector<pair<stationres, float>> OUTPUT;
		vector<pair<stationres, float>> INPUTb; // input bonus per crewman, can be negative;
		vector<pair<stationres, float>> OUTPUTb; // same as above
		vector<pair<stationres, float>> INPUTd;
		vector<pair<stationres, float>> OUTPUTd;
		vector<pair<stationres, float>> storagelimit;
		vector<pair<int2, int>> slots; // pos, size
		vector<float> slotsrot;
		vector<pair<int2, int>> outslots; // pos, size
		vector<float> outslotsrot;
		ui mincrew = 0;
		float hp = 0.0f;
		float hpd = 0.0f;
		ui maxcrew = 0;
		ui slotsize = -1;
		int2 pos;
		int2 size;
		int2 tsize;
		ui longrangesrbonus = 0;
		ui midrangesrbonus = 0;
		ui longrangesrbonuspercw = 0;
		ui midrangesrbonuspercw = 0;
		string textureloc = "";
		string outsideloc = "NULL";
		float ang = 0;
		stationmodtempIN()
		{

		}
		stationmodtempIN(string loc)
		{
			if (loc[loc.size() - 1 != '\\'] && loc[loc.size() - 1] != '/')
				loc += '\\';

			WIN32_FIND_DATA wbs1;
			HANDLE hfind;
			if (((hfind = FindFirstFileA((loc + "core.data").c_str(), &wbs1)) != INVALID_HANDLE_VALUE) && hfind != (HANDLE)ERROR_FILE_NOT_FOUND)
				loc += "core.data";
			else
				loc += "module.data";
			AZfile f = loc;
			loadinoutputs({ INPUT,"INPUT" }, { INPUTd,"INPUTd" }, { INPUTb,"INPUTb" }, { OUTPUT,"OUTPUT" },
			{ OUTPUTd,"OUTPUTd" }, { OUTPUTb,"OUTPUTb" },{ storagelimit,"STORAGE" }, f);
			hp = f.GetVar<float>("hp");
			f.GetVarSafe(hpd, "hpd");
			f.GetVarSafe(slotsize, "size");
			f.GetVarSafe(iscore, "iscore");
			f.GetVarSafe(ang,"INangle");
			pos = { f.GetVar<int>("posX") , f.GetVar<int>("posY") };
			size = { f.GetVar<int>("sizeX") , f.GetVar<int>("sizeY") };
			tsize = { -1,-1 };
			f.GetVarSafe(tsize.x, "tsizeX");
			f.GetVarSafe(tsize.y, "tsizeY");
			if (tsize == int2{ -1, -1 })
				tsize = size;
			hp = f.GetVar<float>("hp");
			pos = int2(f.GetVar<int>("posX"), f.GetVar<int>("posY"));
			name = f.GetVar<string>("name");
			int i = 0;
			string sltype;
			int iii = 0;
			while (iii < 2)
			{
				if (iii == 0)
					sltype = "s";
				else if (iii == 1)
					sltype = "os";
				i = 0;
				while (f.findstruct(sltype + INTtoSTR(i)))
				{
					string str = sltype + INTtoSTR(i) + "@";
					int ii = 0;
					vector<pair<int2, int>> toadd;
					vector<float> toaddrot;
					toadd.push_back(pair<int2, int>{ int2{ f.GetVar<int>(str + "posX"),f.GetVar<int>(str + "posY") }, f.GetVar<ui>(str + "size") });
					toaddrot.push_back(f.GetVar<float>(str + "angle"));

					bool dv = false;
					bool dh = false;
					int2 msize = getmodsizefi(toadd[0].second);
					if(f.GetVarSafe(dv, str + "doublevertically") && dv)
					{
						msize.x *= 2;
					}
					if (f.GetVarSafe(dh, str + "doublehorizontally") && dh)
					{
						msize.y *= 2;
					}
					
					int copiesX = 0;
					int copiesY  = 0;
					int rowsize = f.GetVar<int>(str + "rowsize");
					if (copiesX = f.GetVar<int>(str + "copiesX"))
					{
						ii = 0;
						while (ii < copiesX)
						{
							int2 npos = toadd[0].first;
							npos.x += msize.x + rowsize*(ii + 1);
							toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
							toaddrot.push_back(f.GetVar<float>(str + "angle"));
							ii++;
						}
						ii = 0;
					}
					if (copiesY = f.GetVar<int>(str + "copiesY"))
					{
						ii = 0;
						while (ii < copiesY)
						{
							int2 npos = toadd[0].first;
							npos.y += (msize.y + rowsize)*(ii+1);
							toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
							toaddrot.push_back(f.GetVar<float>(str + "angle"));
							ii++;
						}
						ii = 0;
					}
					vector<int2> tddX;
					vector<int2> tddY;
					if (f.GetVar<bool>(str + "issymetricalH"))
					{
						ii = 0;
						while (ii < toadd.size())
						{
							int dist = tsize.x - toadd[ii].first.x*stationsizemultip;
							int2 npos = { toadd[ii].first };
							npos.x += dist;
							tddX.push_back(npos);
							float nrot = f.GetVar<float>(str + "angle") + 180;
							constraintoscope(nrot, 360.0f, 0.0f);
							toaddrot.push_back(nrot);
							ii++;
						}
					}
					if (f.GetVar<bool>(str + "issymetricalV"))
					{
						ii = 0;
						while (ii < toadd.size())
						{
							int dist = tsize.y - toadd[ii].first.y*stationsizemultip;
							int2 npos = { toadd[ii].first };
							npos.y += dist;
							float nrot = f.GetVar<float>(str + "angle") + 180;
							constraintoscope(nrot, 360.0f, 0.0f);
							toaddrot.push_back(nrot);
							tddY.push_back(npos);
							ii++;
						}
					}
					ii = 0;
					while (ii < tddX.size())
					{
						toadd.push_back(make_pair(tddX[ii], toadd[0].second));
						ii++;
					}
					ii = 0;
					while (ii < tddY.size())
					{
						toadd.push_back(make_pair(tddY[ii], toadd[0].second));
						ii++;
					}
					ii = 0;
					while (ii < toadd.size())
					{
						if (sltype == "s")
						{
							slots.push_back(toadd[ii]);
							slotsrot.push_back(toaddrot[ii]);
						}
						else
						{
							outslots.push_back(toadd[ii]);
							outslotsrot.push_back(toaddrot[ii]);
						}
						ii++;
					}
					i++;
				}
				iii++;
			}
			i = 0;
			string dirloc = reverseSTR(copyfromchtoend(reverseSTR(loc), '\\', true));
			textureloc = dirloc + "module.png";
			WIN32_FIND_DATA windowsbs;
			if (FindFirstFileA((dirloc + "outside.png").c_str(), &windowsbs))
				outsideloc = dirloc + "outside.png";
		}
	};
	extern map<string, stationmodtempOUT*>smOUTtemplates;
	extern map<string, stationmodtempIN*>smINtemplates;
	class stationmodOUT
	{
		friend stationmodtempOUT;
	private:
		int lastcrew = -1;
		vector<pair<stationres, float>> INPUTl;
		vector<pair<stationres, float>> OUTPUTl;
		bool hpdstatel = false;
	public:
		string name = "";
		string textureloc = "";
		string outsideloc = "NULL";
		float INangle = 0.0f;
		int2* pos;
		int2 postm;
		vector<pair<stationres, float>> INPUT;
		vector<pair<stationres, float>> OUTPUT;
		vector<pair<stationres, float>> INPUTb; // input bonus per crewman, can be negative;
		vector<pair<stationres, float>> OUTPUTb; // same as above
		vector<pair<stationres, float>> INPUTd;
		vector<pair<stationres, float>> OUTPUTd;
		vector<pair<stationres, float>> storagelimit;
		map<stationres, float> storagelimitm;
		map<stationres, float> STORAGE;

		ui mincrew;
		ui maxcrew;
		int2 posin;
		int2 sizet;
		int2 size;
		void loadsubmods(AZfile& af, string lk);
		vector<sub> subs;
		float hpd = -1;
		vector<stationmodINslot*>slotsin;
		vector<stationmodOUTslot*>slotsout;
		stationmodOUT()
		{

		}
		stationmodOUT(stationmodtempOUT temp, int2* npos)
		{
			int i = 0;
			INangle = temp.ang;
			INPUT = temp.INPUT;
			INPUTb = temp.INPUTb;
			INPUTd = temp.INPUTd;
			OUTPUT = temp.OUTPUT;
			OUTPUTd = temp.OUTPUTd;
			OUTPUTb = temp.OUTPUTb;
			pos = npos;
			sizet = temp.sizet;
			size = temp.size;
			postm = temp.pos;
			maxcrew = temp.maxcrew;
			mincrew = temp.mincrew;
			subs = temp.subs;
			textureloc = temp.textureloc;
			outsideloc = temp.outsideloc;
			storagelimit = temp.storagelimit;
			sizet = sizet * 2;
			postm = postm * 2;
			size = size * 2;
			storagelimitm = VecToMap(storagelimit);
			while (i < subs.size())
			{
				storagelimitm = SumMaps(storagelimitm, subs[i].storagelimitm);
				i++;
			}
			storagelimit = MaptoVec(storagelimitm);
			i = 0;
		}
		camera* cam = nullptr;
		frame* f = nullptr;
		bool* renderp = nullptr;
		float hp = 1;
		ui curcrew = 0;
		bool* identp = nullptr;
		string stationname = "";
		physobj* pobj;
		bool ison = true;
		void tick();
		void PhysInit(physics& pclass,string s = ""/*do not use*/)
		{
			if (s == "")
			{
				s = stationname + name;
			}
			else
				s += name;
			pclass.addobj(s, new int2 (size), pos, false, false);
			pobj = pclass.GetLastObj();
			int i = 0;
			i = 0;
			while (i < slotsout.size())
			{
				slotsout[i]->mod->PhysInit(pclass, s);
				i++;
			}
			i = 0;
		}
		void RenderInit(camera* ncam, frame* nf, string nstationname,stationmodOUTslot* slot,station* stat,bool iscore=false);
		void  RenderRelease();
	};
	class stationmodIN
	{
		friend stationmodtempIN;
	public:
		string name = "";
		string textureloc = "";
		string outsideloc = "NULL";
		float INangle = 0.0f;
		int2* pos;
		int2 postm;
		vector<pair<stationres, float>> INPUT;
		vector<pair<stationres, float>> OUTPUT;
		vector<pair<stationres, float>> INPUTb; // input bonus per crewman, can be negative;
		vector<pair<stationres, float>> OUTPUTb; // same as above
		vector<pair<stationres, float>> INPUTd;
		vector<pair<stationres, float>> OUTPUTd;
		vector<pair<stationres, float>> storagelimit;
		map<stationres, float> storagelimitm;
		map<stationres, float> STORAGE;

		ui mincrew;
		ui maxcrew;
		ui curcrew = 0;
		int2 posin;
		int2 sizet;
		int2 size;
		vector<stationmodINslot*>slotsin;
		vector<stationmodOUTslot*>slotsout;
		int slotsize;
		float hpmax = 1;
		float hp = 1;
		float hpd = -1;
		stationmodIN()
		{

		}
		stationmodIN(stationmodtempIN temp, int2* npos)
		{
			int i = 0;
			INangle = temp.ang;
			INPUT = temp.INPUT;
			INPUTb = temp.INPUTb;
			INPUTd = temp.INPUTd;
			OUTPUT = temp.OUTPUT;
			OUTPUTd = temp.OUTPUTd;
			OUTPUTb = temp.OUTPUTb;
			storagelimit = temp.storagelimit;
			pos = npos;
			sizet = temp.tsize;
			size = temp.size;
			postm = temp.pos;
			maxcrew = temp.maxcrew;
			mincrew = temp.mincrew;
			textureloc = temp.textureloc;
			outsideloc = temp.outsideloc;
			hp = temp.hp;
			slotsize = temp.slotsize;
			hpmax = temp.hp;
			storagelimitm = VecToMap(temp.storagelimit);
			
			i = 0;
			while (i < temp.slots.size())
			{
				stationmodINslot* sl = new stationmodINslot;
				sl->mod = nullptr;
				sl->pos = new int2( temp.slots[i].first);
				sl->slotnum = i;
				sl->slotsize = temp.slots[i].second;
				sl->slotrot = temp.slotsrot[i];
				slotsin.push_back(sl);
				i++;
			}
			i = 0;
			while (i < temp.outslots.size())
			{
				stationmodOUTslot* sl = new stationmodOUTslot;
				sl->mod = nullptr;
				sl->pos = new int2(temp.outslots[i].first);
				sl->slotnum = i;
				sl->slotsize = temp.outslots[i].second;
				sl->slotrot = temp.outslotsrot[i];
				slotsout.push_back(sl);
				i++;
			}
		}
		void loadsubmods(AZfile& af, string lk);
		camera* cam = nullptr;
		frame* f = nullptr;
		bool* renderp = nullptr;
		bool* identp = nullptr;
		string stationname = "";
		bool ison = true;
		int lastcrew = -1;
		vector<pair<stationres, float>> INPUTl;
		vector<pair<stationres, float>> OUTPUTl;
		bool hpdstatel = false;

		void tick();
		void RenderInit(camera* ncam, frame* nf, string nstationname, stationmodINslot* slot, GAME::station* stat, bool iscore = false);
		void  RenderRelease()
		{
			*identp = false;
			delete renderp;
			renderp = nullptr;
			int i = 0;
			identp = nullptr;
			while (i < slotsin.size())
			{
				slotsin[i]->mod->RenderRelease();
				i++;
			}
			i = 0;
			while (i < slotsout.size())
			{
				slotsout[i]->mod->RenderRelease();
				i++;
			}
		}
	};
	class station
	{
	protected:
		template<typename univar>
		void addtostorage(univar* mod);
		template<>
		void addtostorage(stationmodOUT* mod);
	public:
		string datal;
		vector<stationmodIN*> sINv;
		vector<stationmodOUT*> sOUTv;
		exchangepoint* exp;
		vector<pair<stationres, float>> storagelimit;
		map<stationres, float> storagelimitm;
		map<stationres, float> STORAGE;
		int2* pos;
		float  orbang;
		int randpnum;
		string orbitedbody;
		string name;
		int128 orbrad;
		void* orbobj;
		stationmodIN* core;
		void createmods(AZfile& af, string datapos);
		frame* f = nullptr;
		void tick();
		void RenderInit(frame* mf, camera* cam);
		void RenderEnd();
		//ID2D1Bitmap* bm;
	};
	class star
	{
	public:
		ui seed;
		starcol colrang;
		starcol actualcol;
		char typechar;
		int2* pos;
		float* rot;
		ui hotness;
		ui displayedtemp;
		ui luminosity;
		int probability;
		int128 radius;
		ID2D1Bitmap* bm;
	};
	class system
	{
	public:
		ui seed;
		space* s;
		vector<star*>		 stars;
		map<string, star*>	 starmap;
		vector <planet*>	 planets;
		map<string, planet*> planetmap;
		int randplanetnum;
		int2 *syspos; // middlepoint
		int4 reservedsize; // x and y - top-left corner, z and w - bottom-right corner
		string name;
		vector<station*> statvec;
		map<string, station*>statmap;
		inline int2* getposofobj(string objname)
		{
			if (MapFind(planetmap, objname))
			{
				return planetmap[objname]->pos;
			}
			if (MapFind(starmap, objname))
			{
				return starmap[objname]->pos;
			}
			return nullptr;
		}
	};
	class universe
	{
	private:
		int answer;
		int randomsysnum;
		ui seed;
		void(*initfunc)(universe*);
		map <char, starcol> startypecol;
		string playerlocsys;
		int2 playerlocsector;
		int2* playerloc;
	public:
		map <string, system*> sysmap;
		vector<system*> sysvec;
		string name;
		economy* eco;
		universe(const universe& u)
		{
			*this = u;
		}
		universe(int answer_to_the_ultimate_question, string name, int randomsysnum, vector<int2> randomsysposvec,vector<system*>sysvec
			, map<string, system*> sysmap, void(*initfunc)(universe*), ui seed, map <char, starcol> startypecolour)
		{
			answer = answer_to_the_ultimate_question;
			universe::seed = seed;
			universe::initfunc = initfunc;
			universe::name = name;
			universe::sysmap = sysmap;
			universe::sysvec = sysvec;
			startypecol = startypecolour;

			if(initfunc != nullptr )
				initfunc(this);
		}
	};
	void init(string apploc);
}
/*
ang = f.GetVar<float>("INangle");
hp = f.GetVar<float>("hp");
posin = int2(f.GetVar<int>("INposX"), f.GetVar<int>("INposY"));
sizet = int2(f.GetVar<int>("tsizeX"), f.GetVar<int>("tsizeY"));
pos = int2(f.GetVar<int>("posX"), f.GetVar<int>("posY"));
size = sizet = int2(f.GetVar<int>("sizeX"), f.GetVar<int>("sizeY"));
name = f.GetVar<string>("name");
string dirloc = reverseSTR(copyfromchtoend(reverseSTR(loc),'\\',true));
textureloc = dirloc + "module.png";
WIN32_FIND_DATA windowsbs;
if (FindFirstFileA((dirloc + "outside.png").c_str(), &windowsbs))
outsideloc = dirloc + "outside.png";
i = 0;
string sltype = "s";
int iii = 0;
while (f.findstruct(sltype + INTtoSTR(i)))
{
string str = sltype + INTtoSTR(i);
int ii = 0;
vector<pair<int2, int>> toadd;
toadd.push_back(pair<int2, int>{ int2{ f.GetVar<int>(str + "posX"),f.GetVar<int>(str + "posY") }, f.GetVar<int>(str + "size") });
bool dv = false;
bool dh = false;
int2 msize = getmodsizefi(toadd[0].second);
if (f.GetVarSafe(dv, str + "doublevertically") && dv)
{
msize.x *= 2;
}
if (f.GetVarSafe(dh, str + "doublehorizontally") && dh)
{
msize.y *= 2;
}

int copiesX = 0;
int copiesY = 0;
int rowsize = f.GetVar<int>(str + "rowsize");
if (copiesX = f.GetVar<int>(str + "copiesX"))
{
ii = 0;
while (ii < copiesX)
{
int2 npos = toadd[0].first;
npos.x += msize.x + rowsize;
toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
ii++;
}
ii = 0;
}
if (copiesY = f.GetVar<int>(str + "copiesY"))
{
ii = 0;
while (ii < copiesY)
{
int2 npos = toadd[0].first;
npos.y += (msize.y + rowsize)*ii;
toadd.push_back(pair<int2, int>(npos, toadd[ii].second));
ii++;
}
ii = 0;
}
vector<int2> tddX;
vector<int2> tddY;
if (f.GetVar<bool>(str + "issymetricalH"))
{
ii = 0;
while (ii < toadd.size())
{
int dist = msize.x - toadd[ii].first.x;
int2 npos = { toadd[ii].first };
npos.x += dist * 2;
tddX.push_back(npos);
ii++;
}
}
if (f.GetVar<bool>(str + "issymetricalV"))
{
ii = 0;
while (ii < toadd.size())
{
int dist = msize.y - toadd[ii].first.y;
int2 npos = { toadd[ii].first };
npos.y += dist * 2;
tddY.push_back(npos);
ii++;
}
}
ii = 0;
while (ii < tddX.size())
{
toadd.push_back(make_pair(tddX[ii], toadd[0].second));
ii++;
}
ii = 0;
while (ii < tddY.size())
{
toadd.push_back(make_pair(tddY[ii], toadd[0].second));
ii++;
}
ii = 0;
while (ii < toadd.size())
{
if (sltype == "s")
slots.push_back(toadd[ii]);
ii++;
}
i++;
}
i = 0;
*/