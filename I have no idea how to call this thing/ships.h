#pragma once
#include "hmain.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "camera.h"
#include "frame.h"
#include "debugmodule.h"
#include "pathfinding.h"
//#include "universe.h"
#include "eco.h"
class starcol;
class planet;
//inline string getstringfromstationres(stationres res);
//inline stationres strtostationres(string str);
//void loadinoutputs(pair<vector<pair<stationres, float>>&, string>INPUT, pair<vector<pair<stationres, float>>&, string>INPUTd,
//	pair<vector<pair<stationres, float>>&, string>INPUTb, pair<vector<pair<stationres, float>>&, string>OUTPUT,
//	pair<vector<pair<stationres, float>>&, string>OUTPUTd, pair<vector<pair<stationres, float>>&, string>OUTPUTb, pair<vector<pair<stationres, float>>&, string>STORAGE, AZfile& f, string loc = "");
//class stationmodIN;
//class stationmodOUT;
//class stationmodtempOUT;
//class stationmodtempIN;
using namespace GAME;
using namespace std;
using namespace AZfilelib;
inline string _INTtoSTR(int i)
{
	return to_string(i);
}
#define INTtoSTR _INTtoSTR
namespace debugging
{
	extern debugmain* dbm;
	extern debugging::debugwindow* pdebugdata;
}
namespace GAME
{
	class entity;
	void smovef(int2& _pos);
	void cammove(int2& pos);
	void cammovesetref(int2& pos);
	void select(int2& _pos);
	void mbmovestop(int2& pos);
	void shipselecthit(physobj* obj1, physobj* obj2);
	void selectship(int2& pos,entity& ent);
	namespace selecting
	{

	}
	class entity
	{
	public:
		void(*selectf)(int2&,entity&);
		vector<boost::any> datav;
		map<int, boost::any> datam;
	};
	extern 	ui movetype;
}
namespace DX2D
{
	extern int2* powerbarsize;
	extern ui asteroidlimit;
	extern bool callfunc;
	extern double power;
	extern double maxpower;
	extern 	vector<camera*>*shipcamvec;
	extern int projdamage;
	extern int rof; // rate of fire in projectiles per 1000ms
	extern double asr; // asteroid spawn cooldown
	extern int shieldpoints;
	extern bool isdth; //
	class eng
	{
	public:
		int2 pos;
		float rot;
		ID2D1Bitmap* bitmap;
		D2D1_SIZE_F size;
		HRESULT SetBitmapFromFile(LPCWSTR szFileName, ID2D1RenderTarget* camRT)
		{
			HRESULT hr = 1;
			// Create a decoder
			IWICBitmapDecoder *pDecoder = NULL;
			IWICImagingFactory2*  fac;
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory2,
				(LPVOID*)&fac
			);
			hr = fac->CreateDecoderFromFilename(
				szFileName,                      // Image to be decoded
				NULL,                            // Do not prefer a particular vendor
				GENERIC_READ,                    // Desired read access to the file
				WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
				&pDecoder                        // Pointer to the decoder
			);

			// Retrieve the first frame of the image from the decoder
			IWICBitmapFrameDecode *pFrame = NULL;

			if (SUCCEEDED(hr))
			{

				hr = pDecoder->GetFrame(0, &pFrame);
				IWICFormatConverter* pConverter;
				hr = fac->CreateFormatConverter(&pConverter);
				hr = pConverter->Initialize(
					pFrame,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
				ID2D1RenderTarget* RT = camRT;
				hr = RT->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					&bitmap
				);
			}
			else
			{
				DebugBreak();
			}
			size = bitmap->GetSize();

			return hr;
		}
	};
	enum class shipres
	{
		nullres = 0,
		hydrogen = 1,
		uranium = 2,
		plutonium = 3,
		power = 4,
		thrust = 5,
		oxygen = 6
	};
	enum class modtype
	{
		cockpit = 4,
		oxygen = 5,
		fixedcannon = 2,
		turret = 7,
		engine = 3,
		sensor = 6,
		reactor = 1
	};
	enum class slottype
	{
		control=0,
		repair=1,
		use=2,
		none
	};
	enum class sensorlvl
	{
		type,
		roomlayout,
		roomtype,
		lifeforms,
		crew,
		damage,
		full,
		null
	};
	enum class reactortype
	{
		nuclearp,
		nuclearu,
		fusion,
		null
	};
	inline shipres getshipresfromstr(string str)
	{
		str = tolower(str);
		if (str == "hydrogen")
			return shipres::hydrogen;
		else if (str == "uranium")
			return shipres::uranium;
		else if (str == "plutonium")
			return shipres::plutonium;
		else if (str == "power" || str == "energy" )
			return shipres::power;
		else if (str == "thrust")
			return shipres::thrust;
		else if (str == "oxygen")
			return shipres::oxygen;
		
		return shipres::nullres;
	}
	inline string getstringfromshipres(shipres res)
	{
		switch ((ui)res)
		{
		case (ui)shipres::hydrogen:
			return "hydrogen";
		case (ui)shipres::uranium:
			return "uranium";
		case (ui)shipres::plutonium:
			return "plutonium";
		case (ui)shipres::power:
			return "power";
		case (ui)shipres::thrust:
			return "thrust";
		case (ui)shipres::oxygen:
			return "oxygen";
		}
		return "ERROR";
	}
	inline sensorlvl getinfolvlfromstr(string str)
	{
		str = tolower(str);
		if (str == "type")
			return sensorlvl::type;
		else if (str == "roomlayout")
			return sensorlvl::roomlayout;
		else if (str == "roomtype")
			return sensorlvl::roomtype;
		else if (str == "lifeforms")
			return sensorlvl::lifeforms;
		else if (str == "crew")
			return sensorlvl::crew;
		else if (str == "damage")
			return sensorlvl::damage;
		else if (str == "full")
			return sensorlvl::full;
	}
	class ship;
	class crewman;
	class room
	{
	public:
		int2 pos;
		int2 size;
		float oxygenam;
		vector<crewman*> c; 
	};
	class factoken;
	class faction;
	class factions;
	enum class  facrel
	{
		neutral=0,
		allied=2,
		friendly=1,
		hostile=-2,
		negative=-1
	};
	class faction
	{
		friend class factoken;
		friend class factions;
	protected:
		void Release();
	public:
		void Delete();
		ui ID = 0;
		string name;
		factions* fmain;
		vector<factoken*>tokens;
		map<faction*, facrel>relations;
		factoken* CreateToken();
		void DelteToken(ui ftID);
		facrel GetRelations(faction* fac)
		{
			if (!MapFind(relations, fac))
			{
				if (MapFind(fac->relations, this))
				{
					relations.insert(make_pair(fac, fac->relations[this]));
					return fac->relations[this];
				}
				else
				{
					fac->relations.insert(make_pair(this, facrel::neutral));
					relations.insert(make_pair(fac, facrel::neutral));
					return facrel::neutral;
				}
			}
			else
				return relations[fac];
		}
	};
	class factoken
	{
		friend class factions;
		friend class faction;
	protected:
		void Release();
	public:
		ui facID = 0;
		ui tokID = 0;
		faction* fac;
		void Delete();
	};
	class factions
	{
	public:
		vector<faction*> facv;
		map< ui, faction*> facuim;
		//inline faction* CreateFaction();
		faction* CreateFaction(map<faction*, facrel> rel = {});
		void DeleteFaction(ui facID);
		faction* GetFaction(string name);
	};
	class species
	{
	private:

	protected:

	public:
		factoken* ft;
		string name;
		float srepair;
		float sweapon;
		float sfight;
		float spilot;
		float sother;
		float maxincrease;
		bool damagedbyoxygen;
		float oxygenneeded;
		float basichp;
		float maxhp;
		float combatdebuff = 0.0f;
		vector<string> bmvec;
		map<string, string> bmdirmap;
		species()
		{

		}
		species* init(AZfile f, bool addtomap);
	};
	enum /*class*/ caction
	{
		heal = 1,
		attack = 2,
		capture =3, // not used for now
		help=4,
		operate=5,
		repair=6,
		move=7,
		fight =8,
		null=0

	};
	class crewman
	{
	private:

	protected:
		uni2<long double>posprec;

	public:
		factoken* ft;
		vector<pair<caction, boost::any>>actions;
		string name;
		float srepairb;
		float sweaponb;
		float fightmultip = 1.0f;
		float sfightb;
		float spilotb;
		float sotherb;
		float debuff;
		float wepdamage = 1.0f;
		vector<crewman*>debuffed;
		float hp;
		float hpfull;
		int2* pos;
		species* type;
		frame* f;
		string bmdir;
		room* r;
		string modname;
		int slotnum;
		int wtdws = 0; // num of image being used for topdown view
		void tick();
	};
	class module
	{
	private:

	protected:
	public:
		vector<crewman*> repaircrew;
		room* rptr = nullptr;
		bool isbaked = false;;
		string bakedloc = "";
		AZfile *filep;
		float maxhp = 1.0f;
		bool canbedestroyed = false;
		float maxhpdest = 0.0f;
		modtype m;
		float hp = 1.0f;
		reactortype rtype = reactortype::null;
		sensorlvl slvl = sensorlvl::null;
		sensorlvl slvlm = sensorlvl::null;
		string name;
		int size;
		bool* rb = nullptr;
		int2* pos = nullptr;
		vector<pair<shipres, float>> in;
		vector<pair<shipres, float>> out;
		vector<pair<shipres, float>> ind;
		vector<pair<shipres, float>> outd;
		vector<pair<shipres, float>> inm;
		vector<pair<shipres, float>> outm;
		vector<pair<int2, pair<slottype,float>>> slots;
		vector<boost::any> args;
		bool hasd = false;
		bool hasm = false;
		bool isd = false;
		bool ism = false;
		module()
		{

		}
		module(module* mod, bool copy, ship& sh, camera* cam, frame* f, int2* npos, int2 size,float* rot);
		module(string dir)
		{
			if (dir[dir.size() - 1] != '\\')
				dir += '\\';
			filep = new AZfile( dir + "module.data");
			*this = module(*filep);
		}
		module(AZfile nfile /*will be ignored if filep != nullptr*/) 
		{
			filep = new AZfile(nfile.link);
			AZfile&file = *filep;
			int i = 0;
			while (file.findvar("INPUT@type" + INTtoSTR(i)))
			{
				in.push_back(make_pair(getshipresfromstr(file.GetVar<string>("INPUT@type" + INTtoSTR(i))), file.GetVar<float>("INPUT@amount" + INTtoSTR(i))));
				i++;
			}

			i = 0;
			while (file.findvar("INPUTm@type" + INTtoSTR(i)))
			{
				inm.push_back(make_pair(getshipresfromstr(file.GetVar<string>("INPUTm@type" + INTtoSTR(i))), file.GetVar<float>("INPUTm@amount" + INTtoSTR(i))));
				i++;
			}
			i = 0;
			while (file.findvar("INPUTd@type" + INTtoSTR(i)))
			{
				ind.push_back(make_pair(getshipresfromstr(file.GetVar<string>("INPUTd@type" + INTtoSTR(i))), file.GetVar<float>("INPUTd@amount" + INTtoSTR(i))));
				i++;
			}


			i = 0;
			while (file.findvar("OUTPUT@type" + INTtoSTR(i)))
			{
				out.push_back(make_pair(getshipresfromstr(file.GetVar<string>("OUTPUT@type" + INTtoSTR(i))), file.GetVar<float>("OUTPUT@amount" + INTtoSTR(i))));
				i++;
			}

			i = 0;
			while (file.findvar("OUTPUTm@type" + INTtoSTR(i)))
			{
				outm.push_back(make_pair(getshipresfromstr(file.GetVar<string>("OUTPUTm@type" + INTtoSTR(i))), file.GetVar<float>("OUTPUTm@amount" + INTtoSTR(i))));
				i++;
			}

			i = 0;
			while (file.findvar("OUTPUTd@type" + INTtoSTR(i)))
			{
				outd.push_back(make_pair(getshipresfromstr(file.GetVar<string>("OUTPUTd@type" + INTtoSTR(i))), file.GetVar<float>("OUTPUTd@amount" + INTtoSTR(i))));
				i++;
			}
			i = 0;
			while (file.findvar("SLOT" + INTtoSTR(i)))
			{
				AZstruct* ss = file.GetStruct("SLOT" + INTtoSTR(i));
				slots.push_back(make_pair(int2{ ss->getvar<int>("X"),ss->getvar<int>("Y") }, make_pair( slottype(ss->getvar<ui>("ID")) ,ss->getvar<float>("angle"))));
				i++;
			}
			m = (modtype)file.GetVar<ui>("type");
			hp = file.GetVar<float>("hitpoints");
			maxhp = hp;
			name = file.GetVar<string>("dirname");
			size = file.GetVar<int>("size");
			if (file.findvar("hascriticalstate"))
			{
				canbedestroyed = file.GetVar<bool>("hascriticalstate");
				if (canbedestroyed)
				{
					maxhpdest = file.GetVar<float>("maxdamage");
				}
			}
			else
				canbedestroyed = false;
		}
		void tick(ship& sh);
	};
	class shiptemp
	{
	public:
		map<string, int2> modpos;
		map<string, int2> modsize;
		map<string, modtype> mtype;
		map<string, module*> bakedmods;
		int2 size;
		map<string, string> bakedmloc;
		vector<pair<int2, int2>> cargoareas;
		vector<pair<int2, int2>> rooms;
		vector<pair<shipres, ui>> shipresmstorage;
		vector<pair<string,pair<int2,pair<ui,float>>>>   slots;
		vector<pair<string,pair<int2,int2>>> outlines;
		vector<pair<int2, int2>> doors;
		int3 doorcol;
		shiptemp()
		{
			
		}
		shiptemp(AZfile file)
		{
			string dir = reverseSTR(reverseSTR(copyfromchtoend(file.link, '\\', true)));
			int i = 0;
			while (file.findvar("OUTLINE" + INTtoSTR(i)))
			{
				outlines.push_back(make_pair(reverseSTR((copyfromchtoend(reverseSTR(dir), '\\', true))) + file.GetVar<string>("OUTLINE" + INTtoSTR(i)),
					make_pair(int2(file.GetVar<int>("OUTLINE" + INTtoSTR(i) + "X"), file.GetVar<int>("OUTLINE" + INTtoSTR(i) + "Y")),
						int2(file.GetVar<int>("OUTLINE" + INTtoSTR(i) + "SX"), file.GetVar<int>("OUTLINE" + INTtoSTR(i) + "SY")))));
				i++;
			}
			i = 0;
			string cockloc/*...*/ = dir + file.GetVar<string>("COCKPIT");
			int2 cockpos(file.GetVar<int>("COCKPITX"), file.GetVar<int>("COCKPITY"));
			int2 cocksize/*large*/(file.GetVar<int>("COCKPITSX"), file.GetVar<int>("COCKPITSY"));
			module* m = new module();
			m->pos = new int2(cockpos);
			m->hasd = false;
			m->hasm = false;
			m->hp = file.GetVar<float>("COCKPITHP");
			m->maxhp = file.GetVar<float>("COCKPITHP");
			m->size = 1;
			modpos.insert(make_pair("cockpit", cockpos));
			modpos.insert(make_pair("cockpit", cocksize));
			m->isbaked = true;
			m->bakedloc = file.link;
			m->name = "cockpit";
			bakedmods.insert(make_pair("cockpit", m));
			bakedmloc.insert(make_pair("cockpit", cockloc));
			size = { file.GetVar<int>("SIZEX"),file.GetVar<int>("SIZEY") };
			vector<string> modn;
			i = 0;
			while (file.findvar("MODULES@mod" + INTtoSTR(i)))
			{
				modn.push_back(file.GetVar<string>("MODULES@mod" + INTtoSTR(i)));
				i++;
			}
			i = 0;
			while (i < modn.size())
			{
				modpos.insert(make_pair(modn[i], int2(file.GetVar<int>("MODULES@" + modn[i] + "@X"), file.GetVar<int>("MODULES@" + modn[i] + "@Y"))));
				modsize.insert(make_pair(modn[i], int2(file.GetVar<int>("MODULES@" + modn[i] + "@SX"), file.GetVar<int>("MODULES@" + modn[i] + "@SY"))));
				mtype.insert(make_pair(modn[i], (modtype)file.GetVar<ui>("MODULES@" + modn[i] + "@ID")));

				i++;
			}
			i = 0;
			int2 shipsize = { file.GetVar<int>("SIZEX"),file.GetVar<int>("SIZEY") };
			while (file.findvar("MODULES@D" + INTtoSTR(i)))
			{
				int2 pos1 = int2{ file.GetVar<int>("MODULES@D" + INTtoSTR(i) + "@X1"),file.GetVar<int>("MODULES@D" + INTtoSTR(i) + "@Y1") };
				int2 pos2 = int2{ file.GetVar<int>("MODULES@D" + INTtoSTR(i) + "@X2"),file.GetVar<int>("MODULES@D" + INTtoSTR(i) + "@Y2") };
				doors.push_back({ pos1,pos2 });
				if (file.GetVar<bool>("MODULES@D" + INTtoSTR(i) + "@ISSYMETRICAL"))
				{
					int2 pos1s, pos2s;
					pos1s.y = pos1.y;
					pos2s.y = pos2.y;
					pos1s.x = shipsize.x - pos1.x;
					pos2s.x = shipsize.x - pos2.x;
					doors.push_back({ pos1s,pos2s });
				}
				i++;
			}
			i = 0;
			while (file.findvar("SLOT" + INTtoSTR(i)))
			{
				string sname = "SLOT" + INTtoSTR(i);
				slots.push_back(make_pair(file.GetVar<string>(sname + "@MODULENAME"),
					make_pair(int2(file.GetVar<int>(sname + "@X"), file.GetVar<int>(sname + "@Y")),
						make_pair( file.GetVar<ui>(sname + "@ID"),file.GetVar<float>(sname + "@angle")))));
				i++;
			}
			doorcol = int3(file.GetVar<int>("DOOR_R"), file.GetVar<int>("DOOR_G"), file.GetVar<int>("DOOR_B"));
			i = 0;
			while (file.findvar("ROOMS@R" + INTtoSTR(i)))
			{
				string lk = "ROOMS@R" + INTtoSTR(i) + "@";
				int2 rp = 
				{
					file.GetVar<int>(lk + "X"),
					file.GetVar<int>(lk+"Y")
				};
				int2 rs
				(
					file.GetVar<int>(lk + "SX"),
					file.GetVar<int>(lk + "SY")
				);
				rooms.push_back(make_pair(rp,rs));

				if (file.GetVar<bool>(lk + "ISSYMETRICAL"))
				{
					int2 pos1s, pos2s;
					pos1s.y = rp.y;
					pos2s.y = rp.y;
					pos1s.x = shipsize.x - rp.x;
					pos2s.x = shipsize.x - rp.x;
					rooms.push_back({ pos1s,pos2s });
				}

				i++;
			}
			i = 0;
			while (file.findvar("CARGOROOMS@R" + INTtoSTR(i)))
			{
				string lk = "CARGOROOMS@R" + INTtoSTR(i) + "@";
				int2 rp =
				{
					file.GetVar<int>(lk + "X"),
					file.GetVar<int>(lk + "Y")
				};
				int2 rs
				(
					file.GetVar<int>(lk + "SX"),
					file.GetVar<int>(lk + "SY")
				);
				cargoareas.push_back(make_pair(rp, rs));

				if (file.GetVar<bool>(lk + "ISSYMETRICAL"))
				{
					int2 pos1s, pos2s;
					pos1s.y = rp.y;
					pos2s.y = rp.y;
					pos1s.x = shipsize.x - rp.x;
					pos2s.x = shipsize.x - rp.x;
					cargoareas.push_back({ pos1s,pos2s });
				}

				i++;
			}
			i = 0;
			while (file.findvar("STORAGE@T" + INTtoSTR(i)))
			{
				shipres sr = getshipresfromstr(file.GetVar<string>("STORAGE@T" + INTtoSTR(i)));
				int am = file.GetVar<float>("STORAGE@TA" + INTtoSTR(i));
				shipresmstorage.push_back(make_pair(sr, (ui)am));
				i++;
			}
			i = 0;
		}
	};
	class smove
	{
	private:

	protected:

	public:
		bool* b = nullptr;
		int2* tpos = nullptr;
		int2* trot = nullptr;
		bool delrotpos = true;
		line *l = nullptr;
		void Release(frame* f)
		{
			int i = 0;
			if(b != nullptr )
				while (i < f->lines.size())
				{
					if (f->lines[i].renderp == b)
					{
						f->lines.erase(f->lines.begin() + i);
					}
					i++;
				}
			i = 0;
			if (delrotpos)
			{
				delete trot;
				delete tpos;
				delete b;
			}
		}
	};
	void mousehit(physobj* pobj1, physobj* pobj2);
	class ship
	{
	protected:
	public:
		GAME::system* sys = nullptr;
		GAME::universe* uniclass = nullptr;
		debugging::debugwindow *dwnd = nullptr;
		debugging::debugwindow *resdwnd = nullptr;
		int2* mousepos = new int2(0, 0);
		physobj* pobj;
		pathsystem psystem;
		physics crewphys;
		factoken* ft;
		vector<smove> moves;
		int2* pos;
		int2* universepos = nullptr;
		float* rot;
		//int2 size;
		int lrsenr = 0; // long range sensor range
		int mrsenr = 0; // medium range sensor range
		float mannedrbonus = 1.0f;
		float agility = 10.0f; // degrees per tick
		bool mannedibonus = false;
		vector<pair<bool*,pair<int2, int2>>> doors;
		vector<pair<string, pair<int2, pair<ui,float>>>>   slots; // name, pos, size
		vector<pair<string, pair<int2, int2>>> outlines;
		vector<room*> rooms;
		map<shipres, ui> limit;
		map<shipres, float> res = {};
		vector<pair<string,module*>> modvec;
		map<string, module*> modmap;
		sensorlvl slvl = sensorlvl::null;
		sensorlvl slvlm = sensorlvl::null;
		ID2D1Bitmap* mainship;
		map<string, crewman*> crewmap;
		D2D1_SIZE_F size;
		string name;
		camera* shcam = nullptr;
		bool* frontr = nullptr;
		bool* backr = nullptr;
		bool* rightr = nullptr;
		bool* leftr = nullptr;
		frame* fr = nullptr;
		bool* topb = nullptr;
		vector<eng> leftroteng;
		uni2<long double> posprec = { 0.0l,0.0l };
		vector<eng> rightroteng;
		vector<eng> fronteng;
		vector<eng> backeng;
		vector<int2*> outlinepos;
		void tick()
		{
			dwnd->setvar(*pos, "shippos", true, "INT2");
			if (this == nullptr)
				return;
			int i = 0;
			frame* f = fr;
			while (i < modvec.size())
			{
				modvec[i].second->tick(*this);
				i++;
			}
			auto cmv = MaptoVec(crewmap);
			i = 0;
			while (i < cmv.size())
			{
				cmv[i].second->tick();
				//dwnd->subwindows[1]->setvar((int2)*cmv[i].second->pos, cmv[i].second->name + "'s pos", true, "INT2");
				i++;
			}
			i = 0;
			if (dwnd != nullptr)
			{
				int bullshit = moves.size();
				dwnd->setvar((int2&)*pos, "shippos", true, "INT2");
				dwnd->setvar((float&)*rot, "shiprot", true, "FLOAT");
				dwnd->setvar((int&)bullshit, "movcount", true, "INT");
				dwnd->setvar((string&)name, "shipname", true, "STR");
			}
			if (resdwnd != nullptr)
			{
#if monitorshipresdebug == 1
				auto resv = MaptoVec(res);
				i = 0;
				while (i < resv.size())
				{
					resdwnd->setvar(resv[i].second, getstringfromshipres(resv[i].first), true, "FLOAT", 10);
					i++;
				}
				i = 0;
#endif
			}

			i = 0;
			vector<shipres> rtypesvec = MapFirsttoVec(res);
			while (i < rtypesvec.size())
			{
				if (MapFind(limit, rtypesvec[i]) )
				{
					if (res[rtypesvec[i]] > limit[rtypesvec[i]])
					{
						res[rtypesvec[i]] = limit[rtypesvec[i]];
					}
				}
				i++;
			}
			if (!MapFind(res, shipres::thrust) || moves.size() == 0)
				return;
			float thrust = res[shipres::thrust];
			res[shipres::thrust] = 0.0f;
			i = 0;
			while (i < rooms.size())
			{
				float maxoxy = rooms[i]->size.x * rooms[i]->size.y / 100;

				if (rooms[i]->oxygenam < maxoxy)
				{
					maxoxy /= 10;
					float neededoxy = maxoxy - rooms[i]->oxygenam;
					if (MapTGT(res, shipres::oxygen, neededoxy))
					{
						rooms[i]->oxygenam += neededoxy;
						res[shipres::oxygen] -= neededoxy;
					}
				}
				i++;
			}
			i = 0;
			if (moves.size() != 0)
			{
				if (moves[0].trot != nullptr)
				{
					constraintoval(*rot, 360.0f);
					//constraintoval(*moves[0].trot, 360.0f);
					float srot = *rot;
					int2 p1 = *pos + int2{ (int)size.width / 2,(int)size.height / 2 };
					int2 p2 = *moves[0].trot;
					float trot = atan2(p1.y - p2.y, p1.x - p2.x);
					trot = XMConvertToDegrees(trot);
					trot -= 90;
					constraintoval(trot, 360.0f);
					if (trot == *rot)
					{
						moves[0].Release(f);
						moves.erase(moves.begin());
						goto donewithmoving;
					}
					float mp = 1;
					//if (srot == 0)
					//	srot = 360;
					float rotdif = trot-srot;
					if (abs(rotdif) != rotdif)
						mp = -1;
					//float rotdif = srot-trot;
					if (agility >= abs(rotdif))
					{
						moves[0].Release(f);
						moves.erase(moves.begin());
						*rot = trot;
						goto donewithmoving;
					}
					*rot += mp*agility;
				}
				i = 0;
				while (i < moves.size())
				{
					if (moves[i].tpos == nullptr)
					{
						i++;
						continue;
					}
					int2 tar = *moves[i].tpos;
					int2 cpos = { 0,0 };
					if (i == 0)
						cpos = *pos + int2{ (int)this->size.width / 2,(int)this->size.height / 2 };
					else
					{
						int ii = 1;
						while (i - ii > -1)
							if (moves[i - ii].tpos == nullptr)
								ii++;
							else
								break;
						if (i - ii == -1)
							cpos = *pos + int2{ (int)this->size.width / 2,(int)this->size.height / 2 };
						else 
							cpos = *moves[i - ii].tpos;
					}
					if (moves[i].b == nullptr)
					{
						moves[i].b = new bool(true);
						line l;
						l.renderp = moves[i].b;
						ID2D1SolidColorBrush* col = NULL;
						ID2D1BitmapRenderTarget* RT = *linecam->GetRenderTargetP();
						RT->CreateSolidColorBrush(ColorF(1, 1, 1), &col);
						l.color = col;
						l.p1 = { 0,0 };
						l.p2 = { 0,0 };
						lineframe->lines.push_back(l);
						moves[i].l = &lineframe->lines[lineframe->lines.size() - 1];
					}
					if (moves[i].l != nullptr)
					{
						moves[i].l->p1 = cpos;
						moves[i].l->p2 = tar;
						if (i == 0)
						{
							//moves[0].l->p1 = *pos + int2{ (int)this->size.width / 2,(int)this->size.height / 2 };
							//cout << "p1 = " << "{ " << cpos.x << ", " << cpos.y << " }" << endl
							//	<< "p2 = " << "{ " << tar.x << ", " << tar.y << " }" << endl;
						}
					}
					i++;
				}
				i = 0;
				if (moves[0].tpos != nullptr)
				{
					int2 tar = *moves[0].tpos;
					int2 cpos = *pos + int2{ (int)this->size.width / 2,(int)this->size.height / 2 };
					long double movdist = thrust / 10;
					bool endjustafter = false;
					if (tar == cpos)
					{
						endjustafter = true;
					}
					if (Distance<long double>((long double)cpos.x, (long double)cpos.y, (long double)tar.x, (long double)tar.y) <= movdist)
					{
						moves[0].Release(lineframe);
						moves.erase(moves.begin());
						//*pos = tar;
						goto donewithmoving;
					}
					uni2<long double> normalised = classvariables::normalize(cpos - tar);
					normalised = normalised* movdist;
					normalised = normalised* uni2<long double> {-1, -1};
					//*pos = int2{  (int)floorl( normalised.x),(int)floorl(normalised.y) };
					posprec = posprec + normalised;
					if (posprec.x >= 1 || posprec.x <= -1)
					{
						pos->x += floor(posprec.x);
						posprec.x -= floor(posprec.x);
					}
					if (posprec.y >= 1|| posprec.y <= -1)
					{
						pos->y += floor(posprec.y);
						posprec.y -= floor(posprec.y);
					}
					if (endjustafter)
					{
						moves[0].Release(lineframe);
						moves.erase(moves.begin());
						//*pos = tar;
						goto donewithmoving;
					}
					goto donewithmoving;
				}
			}
		donewithmoving:;
		}
		HRESULT SetBitmapFromFile(LPCWSTR szFileName, ID2D1RenderTarget* camRT)
		{
			HRESULT hr = 1;
			// Create a decoder
			IWICBitmapDecoder *pDecoder = NULL;
			IWICImagingFactory2*  fac;
			CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory2,
				(LPVOID*)&fac
			);
			hr = fac->CreateDecoderFromFilename(
				szFileName,                      // Image to be decoded
				NULL,                            // Do not prefer a particular vendor
				GENERIC_READ,                    // Desired read access to the file
				WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
				&pDecoder                        // Pointer to the decoder
			);

			// Retrieve the first frame of the image from the decoder
			IWICBitmapFrameDecode *pFrame = NULL;

			if (SUCCEEDED(hr))
			{

				hr = pDecoder->GetFrame(0, &pFrame);
				IWICFormatConverter* pConverter;
				hr = fac->CreateFormatConverter(&pConverter);
				hr = pConverter->Initialize(
					pFrame,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
				ID2D1RenderTarget* RT = camRT;
				hr = RT->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					&mainship
				);
			}
			else
			{
				DebugBreak();
			}
			size = mainship->GetSize();

			return hr;
		}
		void AddMod(module* temp, int2* pos, int2 size, camera* cam, frame* f, string slot)
		{
			module* mod = new module(temp, true, *this, cam, f, pos,size,new float(0));
			int i = 0;
			modmap[slot] = mod;
			modvec = MaptoVec(modmap);
		}
		void Release()
		{
			delete frontr;
			
			delete backr;
			delete rightr;
			delete leftr;
		}
		ship()
		{
			frontr = new bool(false);
			backr = new bool(false);
			rightr = new bool(false);
			leftr = new bool(false);
		}
		camera* linecam = nullptr;
		frame* lineframe= nullptr;
		ship(const ship&s) = default;
		ship(shiptemp st, camera* cam, frame* f, frame* nlineframe, camera* nlinecam, int2* unipos, GAME::system* csys,universe* uclass)
		{
			uniclass = uclass;
			linecam = nlinecam;
			sys = csys;
			universepos = unipos;
			lineframe = nlineframe;
			pos = cam->GetXYp();
			shcam = cam;
			frontr = new bool(false);
			backr = new bool(false);
			rightr = new bool(false);
			fr = f;
			leftr = new bool(false);
			rot = new float(0);
			cam->SyncRot(rot, true);
			cam->customrotpoint = st.size;
			size = st.size;
			crewphys.addobj("mouse", 2.0f, mousepos, false, false);
			physobj* cobj = crewphys.GetLastObj();
			physreact pr;
			pr.callfuncinstant = true;
			pr.callfunc = true;
			pr.ftocall = mousehit;
			cobj->IDreactionmap.insert(make_pair(666, pr));
			cobj->isron = true;
			ID2D1BitmapRenderTarget* RT = *cam->GetRenderTargetP();
			int i = 0;
			while (i < st.rooms.size())
			{
				room* roo = new room;
				roo->pos = st.rooms[i].first;
				roo->size = st.rooms[i].second;
				roo->oxygenam = roo->size.x * roo->size.y / 100;
				psystem.AddGrid(roo->size, roo->pos, nodetype::passable ,2,2);
				i++;
			}
			i = 0;
			while (i < st.outlines.size())
			{
				int2* opos = new int2(0, 0);
				sprite  s(opos,new float(0));
				outlinepos.push_back(opos);
				s.SetBitmapFromFile(STRtoWSTR(st.outlines[i].first).c_str(), RT);
				s.SetOffsetXYp(new int2(st.outlines[i].second.first), true);
				s.size = st.outlines[i].second.second;
				//s.SetXY(st.outlines[i].second.first);
				//s.offsetbysize();
				f->sprites.push_back(s);
				i++;
			}
			i = 0;
			frame* fl = new frame;
			while (i < st.doors.size())
			{
				bool* b = new bool(true);
				doors.push_back(make_pair(b, st.doors[i]));
				ID2D1SolidColorBrush* col;
				RT->CreateSolidColorBrush(ColorF(st.doorcol.x / 255, st.doorcol.y / 255, (st.doorcol.z / 255)), &col);
				line l;
				l.color = col;
				l.p1 = st.doors[i].first;
				l.p2 = st.doors[i].second;
				int2 size = (l.p2.x - l.p1.x > l.p2.y - l.p1.y) ? int2{l.p2.x - l.p1.x, 1} : int2{1, l.p2.y - l.p1.y};
				psystem.AddGrid(size, l.p1, nodetype::passable);
				fl->lines.push_back(l);
				i++;
			}
			i = 0;
			slots = st.slots;
			outlines = st.outlines;
			//vector<pair<string,int2>> mposv = MaptoVec(st.modpos);
			//vector<pair<string, int2>> msizev = MaptoVec(st.modsize); 
			//vector<pair<string, modtype>> mtypev = MaptoVec(st.mtype);
			vector<module*> bm = MapSecondtoVec(st.bakedmods);
			vector<string> bstr = MapFirsttoVec(st.bakedmloc);
			while (i < st.bakedmods.size())
			{
				modvec.push_back(make_pair(bstr[i], new module(bm[i], true, *this, cam, f, new int2(*bm[i]->pos), st.modsize[bstr[i]], new float (0))));
				modmap.insert(modvec[modvec.size() - 1]);
				room* roo = new room;
				roo->pos = int2(*bm[i]->pos);
				roo->size = st.modsize[bstr[i]];
				rooms.push_back(roo);
				roo->oxygenam = roo->size.x * roo->size.y / 100;
				i++;
			}
			while (i < slots.size())
			{
				if (MapFind(modmap, slots[i].first))
				{
					modmap[slots[i].first]->slots.push_back(make_pair(slots[i].second.first, make_pair((slottype)slots[i].second.second.first,slots[i].second.second.second)));
				}
				i++;
			}
			i = 0;
			//cam->usecustomrenderrotpoint = true;
			//cam->customrendrotpoint = st.size / int2{ 2, 2 };
			if (!f->ischfact)
			{
				f->wchiac = "lines";
				f->f.insert(make_pair("lines", vector<frame*>{ fl }));
			}
			else
			{
				f->f[f->wchiac].push_back(fl);
			}
			f->ischfact = true;
			f->ismactive = true;
			i = 0;
		}
	};
	extern vector <ship*> shipsvec;
	extern map<string, shiptemp> shiptemps;
	inline void insertsdts(vector<sprite>& spritevec, ship s);
#undef INTtoSTR
}