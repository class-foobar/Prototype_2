#pragma once
#include "hmain.h"
using namespace GAME;
using namespace std;
namespace DX2D
{
	class physreact;
	struct phcircle
	{
		float* radius;
	};
	struct phrec
	{
		float2* size;
	};
	enum photype
	{
		circle,
		rect,
		none
	};
	class physics;
	class physobj
	{
		friend DX2D::physics;
	protected:
		bool didchecking = false;
		bool wasreleased = false;
	public:
		bool useidentp = false;
		bool affbyborders = true;
		bool* identp = nullptr; //
		bool masterspeed = false;
		float masterspeednumber = 0; // works only if masterspeed == true
		float *rot;
		bool isrotaff = false;
		bool gravaffected = false;
		string nameindex = "";
		double subpixelx = 0;
		double subpixely = 0;
		vector<boost::any> anyvars;
		//int intindex = 0;
		ui rectID = 0; // used for "IDreactionmap"
		map<int, physreact> IDreactionmap;
		bool isron = false;
		map<physobj*, physreact> recmap;
		union
		{
			int2* size = nullptr;
			float* radius;
		};
		photype type = none;
		int2* pos = nullptr;
		float2 speed = { 0,0 };
		bool bounce = false;
		float boumulti = 1;
		bool isvalid()
		{
			return !wasreleased;
		}
		physobj()
		{

		}
		physobj(physobj& ph, bool donotcopypointers/*if true: new physobj will not copy pointers but create his own variables based on values from "ph"*/)
		{
			if (donotcopypointers)
			{
				*this = ph;
				if (ph.type != none)
				{
					if (ph.type == rect)
						size = new int2(*ph.size);
					else if (ph.type == circle)
						radius = new float(*ph.radius);
				}
				pos = new int2(*ph.pos);

			}
			else
			{
				*this = ph;
			}
		}
		physobj(int2* nsize, int2* npos, bool nbounce)
		{
			size = nsize;
			pos = npos;
			bounce = nbounce;
			type = rect;
		}
		physobj(float* nsize, int2* npos, bool nbounce)
		{
			radius = nsize;
			pos = npos;
			bounce = nbounce;
			type = circle;
		}
		void Release()
		{
			if (wasreleased)
				return;
			if (pos != NULL)
			{
				pos = NULL;
				delete pos;
			}
			if (rot != NULL)
			{
				rot = NULL;
				delete rot;
			}
			pos = nullptr;
			rot = nullptr;
			if (type == photype::circle)
			{
				if (radius != NULL)
					delete radius;
				radius = nullptr;
			}
			else if (type == photype::rect)
			{
				if (size != NULL)
					delete size;
				size = nullptr;
			}
			wasreleased = true;
		}
	};
	//program specific, had to be declarated after physobj
	void asthitsh(physobj* obj1, physobj* obj2);
	void asthitproj(physobj* obj1, physobj* obj2);
	class physreact
	{
	protected:

	public:
		bool en = true;
		bool noclip = false;
		bool callfunc = false;
		bool callfuncinstant = false;
		void(*ftocall)(physobj*, physobj*);
	};
	extern 	void(*fformaintocall)(physobj*, physobj*);
	extern physobj* objtc1;
	extern physobj* objtc2;
	extern bool callfunc;
	class physics
	{
		friend class controls;
	public:
		bool roborders = false;
		int2 roborderXY = { 0,0 }; // works only if roborders == true
		vector <string> objvec;
		map <string, physobj*> objmap;
		float tps = 30;
		bool tpswitch = false;
		double speed = 1.0;
		bool isthrpaused = false;
		bool gravity;
	protected:
		clock_t lastf = 1;
		int fpslimit = -1;
		bool ismultithreated = false;
		bool istickinp = false;
	public:
		bool RECTvsRECT(int2* sa, int2* pa, int2* sb, int2* pb)
		{
			int2 bmin = *pb + *sb;
			int2 amin = *pa + *sa;
			bool b1 = (pa->x < bmin.x && amin.x > pb->x);
			bool b2 = (pa->y < bmin.y && amin.y > pb->y);
			if (b1   && b2 )
				return true;
			return false;
		}
		bool RECTvsCIRCLE(int2* sa, int2* pa, float radb, int2* pb)
		{
			int2 amin = *pa + *sa;
			int2 sb(radb * 2, radb * 2);
			if (radb > 2.0f)
			{
				if (!RECTvsRECT(sa, pa, &sb, pb))
				{
					return false;
				}
			}
			else
				if (RECTvsRECT(sa, pa, &sb, pb))
					return true;
			if (Distance<float>(pa->x + (sa->x / 2), pa->y + (sa->y / 2), pb->x, pb->y) < radb/* + (sa->x / 2)*/)
			{
				return true;
			}
			if (Distance<float>(pa->x + (sa->x / 2), pa->y + (sa->y / 2), pb->x, pb->y) < radb /*+ (sa->y / 2)*/)
			{
				return true;
			}
			if (Distance<float>(amin.x, amin.y, pb->x, pb->y) <= radb)
			{
				return true;
			}
			if (Distance<float>(pa->x, pa->y, pb->x, pb->y) <= radb)
			{
				return true;


			}
			if (Distance<float>(pa->x + sa->x, pa->y, pb->x, pb->y) <= radb)
			{
				return true;
			}
			if (Distance<float>(pa->x, pa->y + sa->y, pb->x, pb->y) <= radb)
			{
				return true;
			}
			return false;
		}
		bool CIRCLEvsCIRCLE(float rada, int2* pa, float radb, int2* pb)
		{
			if (Distance<float>(pa->x, pa->y, pb->x, pb->y) <= radb + rada)
				return true;
			return false;
			//float r = rada + radb;
			//r *= r;
			//int2 pa2 = *pa;
			//pa2.x += rada;
			//pa2.y + rada;
			//int2 pb2 = *pb;
			//pb2.x = radb;
			//pb2.y = radb;
			//return r < (pa2.x + pb2.x) ^ 2 + (pa2.y + pb2.y) ^ 2;
		}
	protected:
		void phreactexe(physobj* obj1, physobj* obj2, physreact ph)
		{
			if (!ph.noclip && obj1->bounce == false)
			{
				obj1->speed = { 0 ,0 };
				obj1->masterspeednumber = 0;
			}
			if (ph.callfunc)
			{
				if (ph.callfuncinstant)
				{
					ph.ftocall(obj1, obj2);
				}
				else
				{
					fformaintocall = ph.ftocall;
					objtc1 = obj1;
					objtc2 = obj2;
					callfunc = true;
				}
			}
			return;
		}
		bool check(physobj* obj1, physobj* obj2, bool resolve)
		{
			if (obj1 == obj2)
				return false;
			int i2 = 0;
			physobj* obj = obj1;
			while (roborders)
			{
				if (obj->affbyborders)
				{
					int2 obj1s;
					if (obj->type == rect)
						obj1s = *obj->size;
					else if (obj->type == circle)
						obj1s = { (int)*obj->radius * 2, (int)*obj->radius * 2 };
					if (obj->pos->x > roborderXY.x + obj1s.x)
					{
						obj->pos->x = 0 - obj1s.x;
					}
					if (obj->pos->x < 0 - obj1s.x)
					{
						obj->pos->x = roborderXY.x + obj1s.x;
					}
					if (obj->pos->y > roborderXY.y + obj1s.y)
					{
						obj->pos->y = 0 - obj1s.y;
					}
					if (obj->pos->y < 0 - obj1s.y)
					{
						obj->pos->y = roborderXY.y + obj1s.y;
					}
				}
				//else
				//	DebugBreak();
				if (i2 == 0)
				{
					obj = obj2;
					i2++;
				}
				else
					break;
			}
			if (obj1->type == photype::rect)
			{
				if (obj2->type == photype::rect)
				{
					if (RECTvsRECT(obj1->size, obj1->pos, obj2->size, obj2->pos))
					{
						if (resolve)
						{
							if (obj1->isron)
							{
								if (obj1->recmap.find(obj2) != obj1->recmap.end())
								{
									phreactexe(obj1, obj2, obj1->recmap[obj2]);
								}
								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj1, obj2, obj1->IDreactionmap[obj2->rectID]);
								}
								return true;
							}
							if (obj2->isron)
							{
								if (obj2->recmap.find(obj1) != obj2->recmap.end())
								{
									phreactexe(obj2, obj1, obj2->recmap[obj1]);
								}

								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj2, obj1, obj2->IDreactionmap[obj1->rectID]);
								}
								return true;
							}
							if (!obj1->bounce)
							{
								obj1->speed = { 0 ,0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
							if (!obj2->bounce)
							{
								obj2->speed = { 0, 0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
						}
						return true;
					}
				}
				else if (obj2->type == photype::circle)
				{
					if (RECTvsCIRCLE(obj1->size, obj1->pos, *obj2->radius, obj2->pos))
					{
						if (resolve)
						{
							if (obj1->isron)
							{
								if (obj1->recmap.find(obj2) != obj1->recmap.end())
								{
									phreactexe(obj1, obj2, obj1->recmap[obj2]);
								}
								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj1, obj2, obj1->IDreactionmap[obj2->rectID]);
								}
								return true;
							}
							if (obj2->isron)
							{
								if (obj2->recmap.find(obj1) != obj2->recmap.end())
								{
									phreactexe(obj2, obj1, obj2->recmap[obj1]);
								}

								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj2, obj1, obj2->IDreactionmap[obj1->rectID]);
								}
								return true;
							}
							if (!obj1->bounce)
							{
								obj1->speed = { 0 ,0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
							if (!obj2->bounce)
							{
								obj2->speed = { 0, 0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
						}
						return true;
					}
				}
			}
			else if (obj1->type == photype::circle)
			{
				if (obj2->type == photype::rect)
				{
					if (RECTvsCIRCLE(obj2->size, obj2->pos, *obj1->radius, obj1->pos))
					{
						if (resolve)
						{
							if (obj1->isron)
							{
								if (obj1->recmap.find(obj2) != obj1->recmap.end())
								{
									phreactexe(obj1, obj2, obj1->recmap[obj2]);
								}
								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj1, obj2, obj1->IDreactionmap[obj2->rectID]);
								}
								return true;
							}
							if (obj2->isron)
							{
								if (obj2->recmap.find(obj1) != obj2->recmap.end())
								{
									phreactexe(obj2, obj1, obj2->recmap[obj1]);
								}

								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj2, obj1, obj2->IDreactionmap[obj1->rectID]);
								}
								return true;
							}
							if (!obj1->bounce)
							{
								obj1->speed = { 0 ,0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
							if (!obj2->bounce)
							{
								obj2->speed = { 0, 0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
						}
						return true;
					}
				}
				else if (obj2->type == photype::circle)
				{
					if (CIRCLEvsCIRCLE(*obj1->radius, obj1->pos, *obj2->radius, obj2->pos))
					{

						if (resolve)
						{
							if (obj1->isron)
							{
								if (obj1->recmap.find(obj2) != obj1->recmap.end())
								{
									phreactexe(obj1, obj2, obj1->recmap[obj2]);
								}
								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj1, obj2, obj1->IDreactionmap[obj2->rectID]);
								}
								return true;
							}
							if (obj2->isron)
							{
								if (obj2->recmap.find(obj1) != obj2->recmap.end())
								{
									phreactexe(obj2, obj1, obj2->recmap[obj1]);
								}

								if (obj1->IDreactionmap.find(obj2->rectID) != obj1->IDreactionmap.end())
								{
									phreactexe(obj2, obj1, obj2->IDreactionmap[obj1->rectID]);
								}
								return true;
							}
							if (!obj1->bounce)
							{
								obj1->speed = { 0 ,0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
							if (!obj2->bounce)
							{
								obj2->speed = { 0, 0 };
								obj1->masterspeednumber = 0;
							}
							else
							{
								//TODO: bouncing physics
							}
						}
						return true;
					}
				}
			}
			return false;
		}
	public:
		void pausethread()
		{
			//while (isthrpaused)
			//	;
			if (ismultithreaded())
			{
				isthrpaused = true;
				mttw.notify_one();
				while (istickinp)
					;
			}
		}
		void unpausethread()
		{
			if (ismultithreaded())
			{
				while (!isthrpaused)
					;
				isthrpaused = false;
				while (istickinp)
					;
			}
		}
		float gravitypower = 1.0f;
	protected:
		condition_variable mttw;
		mutex m;
		void ticktoc(double cdf)
		{
			int ni = 0;
			while (ni < objvec.size())
			{

				if (objmap.find(objvec[ni]) == objmap.end())
				{
					objvec.erase(objvec.begin() + ni);
					continue;
				}
				if (objmap[objvec[ni]] == NULL)
				{
					objmap.erase(objvec[ni]);
					objvec.erase(objvec.begin() + ni);
					continue;
				}
				if (objmap[objvec[ni]]->wasreleased == true)
				{
					objmap.erase(objvec[ni]);
					objvec.erase(objvec.begin() + ni);
					continue;
				}
				if (objmap[objvec[ni]]->masterspeed)
				{
					objmap[objvec[ni]]->speed = { objmap[objvec[ni]]->masterspeednumber,objmap[objvec[ni]]->masterspeednumber };
				}
				if (objmap[objvec[ni]]->speed.x != 0)
				{
					if (objmap[objvec[ni]]->isrotaff)
					{
						if (*objmap[objvec[ni]]->rot == 0)
						{
							*objmap[objvec[ni]]->rot = 360;
						}
						double mov = cos(XMConvertToRadians(*objmap[objvec[ni]]->rot)) * -(objmap[objvec[ni]]->speed.x*cdf);
						objmap[objvec[ni]]->subpixelx += mov;
					}
					else
						objmap[objvec[ni]]->subpixelx += objmap[objvec[ni]]->speed.x*cdf;
					while (objmap[objvec[ni]]->subpixelx > 1.0)
					{
						objmap[objvec[ni]]->subpixelx -= 1.0;
						objmap[objvec[ni]]->pos->x += 1;
					}
					while (objmap[objvec[ni]]->subpixelx < -1.0)
					{
						objmap[objvec[ni]]->subpixelx += 1.0;
						objmap[objvec[ni]]->pos->x -= 1;
					}
				}
				if (objmap[objvec[ni]]->speed.y != 0)
				{
					if (objmap[objvec[ni]]->isrotaff)
					{
						if (*objmap[objvec[ni]]->rot == 0)
						{
							*objmap[objvec[ni]]->rot = 360;
						}
						double mov = sin(XMConvertToRadians(*objmap[objvec[ni]]->rot)) * -(objmap[objvec[ni]]->speed.y*cdf);
						objmap[objvec[ni]]->subpixely += mov;
					}
					else
						objmap[objvec[ni]]->subpixely += objmap[objvec[ni]]->speed.y*cdf;
					int foo = objmap[objvec[ni]]->subpixely;
					while (objmap[objvec[ni]]->subpixely > (float)1)
					{
						objmap[objvec[ni]]->subpixely -= 1.0;
						objmap[objvec[ni]]->pos->y += 1;
					}
					while (objmap[objvec[ni]]->subpixely < -1.0f)
					{
						objmap[objvec[ni]]->subpixely += 1.0;
						objmap[objvec[ni]]->pos->y -= 1;
					}
				}
				if (objmap[objvec[ni]]->gravaffected)
				{
					if (!isSupported(objmap[objvec[ni]]->nameindex))
						if (!objmap[objvec[ni]]->masterspeed)
							objmap[objvec[ni]]->speed.y += gravitypower*cdf;
						else
						{
							objmap[objvec[ni]]->masterspeednumber += -sin(180)*gravitypower*cdf;
						}
				}
				ni++;
			}
			if (objvec.size() == 1)
			{
				physobj* obj1 = objmap[objvec[0]];
				if (roborders)
				{
					int sizeX;
					int sizeY;
					if (obj1->type == circle)
					{
						sizeX = sizeY = *obj1->radius;
					}
					else
					{
						sizeX = obj1->size->x;
						sizeY = obj1->size->y;
					}
					if (obj1->pos->x > roborderXY.x + sizeX)
					{
						obj1->pos->x = 0 - sizeX;
					}
					if (obj1->pos->x < 0 - sizeX)
					{
						obj1->pos->x = roborderXY.x + sizeX;
					}
					if (obj1->pos->y > roborderXY.y + sizeY)
					{
						obj1->pos->y = 0 - sizeY;
					}
					if (obj1->pos->y < 0 - sizeY)
					{
						obj1->pos->y = roborderXY.y + sizeY;
					}
				}
			}
			else
			{
				ni = 0;
				while (ni < objvec.size())
				{
					int i = 0;
					while (i < objvec.size())
					{
						if (!objmap[objvec[i]]->didchecking && i != ni)
						{
							check(objmap[objvec[ni]], objmap[objvec[i]], true);
						}
						i++;
					}
					objmap[objvec[ni]]->didchecking = true;
					ni++;
				}
				ni = 0;
				while (ni < objvec.size())
				{
					objmap[objvec[ni]]->didchecking = false;
					ni++;
				}
			}
		}
		double multithreadcdf = 0;
		void multithreadwakeup()
		{
			mttw.notify_one();
		}
		bool isclosing = false;
		bool isthrclosed = false;
		physobj* lastobj = nullptr;
		void multithreadtickwait()
		{
		again:;
			istickinp = false;
			unique_lock<std::mutex> lk(m);
			mttw.wait(lk);
			if (isclosing)
			{
				isthrclosed = true;
				return;
			}
			if (isthrpaused)
			{
				istickinp = false;
				goto again;
			}
			double cdf = multithreadcdf;
			ticktoc(cdf);
			istickinp = false;
			goto again;
		}
	public:
		void resetcdf()
		{
			multithreadcdf = 0;
			lastf = clock();
		}
		bool ismultithreaded()
		{
			return ismultithreated;
		}
		void Release()
		{
			if (ismultithreaded())
				ReleaseTHR();
			int i = 0;
			while (i < objvec.size())
			{
				if (objmap[objvec[i]]->wasreleased)
					objmap[objvec[i]]->Release();
				delete objmap[objvec[i]];
				i++;
			}

		}
		void ReleaseTHR()
		{
			if (isthrclosed)
				return;
			while (istickinp)
				;
			isclosing = true;
			int i = 0;
		didntclose:;
			mttw.notify_one();
			while (i < 10 && !isthrclosed)
			{
				Sleep(1);
				i++;
			}
			if (i >= 10)
				goto didntclose;
			phythr->join();
			ismultithreated = false;
			delete phythr;
			istickinp = false;
			return;
		}
		bool IsThreadIdle()
		{
			if (ismultithreated)
				return true;
			else if (istickinp)
				return false;
			return true;
		}
		thread* phythr = nullptr;
		bool isSupported(string name)
		{
			if (!gravity)
				return true;
			physobj* obj = objmap[name];
			physobj* suptestobj = new physobj(*obj, true);
			if (obj->type == circle)
			{
				*suptestobj->radius += 1.0f;
			}
			else if (obj->type == rect)
			{
				suptestobj->size->x += 1;
			}
			int i = 0;
			bool b = false;
			while (i < objvec.size())
			{
				if (objmap[name] != objmap[objvec[i]])
					if (check(suptestobj, objmap[objvec[i]], false))
					{
						b = true;
						break;
					}
				i++;
			}
			suptestobj->Release();
			delete suptestobj;
			return b;
		}
		void initbmultithread()
		{
			if (!ismultithreated)
			{
				phythr = new thread(&physics::multithreadtickwait, this);
				ismultithreated = true;
			}
		}

		void tick()
		{
			double cdf;
			if (fpslimit != -1)
			{
				if (lastf == -1)
				{
					lastf = clock();
					return;
				}
				cdf = (double)((double)(clock() - lastf) / CLOCKS_PER_SEC);
				if (cdf <= 0)
				{
					return;
				}
				if (cdf < (double)((double)1 / (double)fpslimit))
				{
					return;
				}

				lastf = clock();

			}
			else
			{
				cdf = (double)((double)(clock() - lastf) / CLOCKS_PER_SEC);
			}
			cdf *= speed;
			if (ismultithreated)
			{
				if (!istickinp)
				{
					multithreadcdf = cdf;
					istickinp = true;
					multithreadwakeup();
				}
			}
			else
			{
				lastf = clock();
				ticktoc(cdf);
			}
		}
		void init(pair<bool, int> fpslock, bool initmultithreading)
		{
			if (initmultithreading)
			{
				initbmultithread();
			}
			else
				ismultithreated = false;
			if (fpslock.first)
			{
				fpslimit = fpslock.second;
			}
			else
			{
				fpslimit = -1;
			}
		}
		bool addobj(string name, int2 size, int2 pos, bool bounce, bool gravaffected)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			if (objmap.find(name) != objmap.end())
				return false;
			physobj* newobj = new physobj(new int2(size), new int2(pos), bounce);
			objvec.push_back(name);
			objmap.insert(make_pair(name, newobj));
			newobj->nameindex = name;
			//newobj->intindex = objvec.size() - 1;
			lastobj = newobj;
			return true;
		}
		bool addobj(string name, float radius, int2 pos, bool bounce, bool gravaffected)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			if (objmap.find(name) != objmap.end())
				return false;
			physobj* newobj = new physobj(new float(radius), new int2(pos), bounce);
			objvec.push_back(name);
			objmap.insert(make_pair(name, newobj));
			newobj->nameindex = name;
			lastobj = newobj;
			//newobj->intindex = objvec.size() - 1;
			return true;
		}
		bool addobj(string name, int2* size, int2* pos, bool bounce, bool gravaffected)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			if (objmap.find(name) != objmap.end())
				return false;
			physobj* newobj = new physobj(size, pos, bounce);
			objvec.push_back(name);
			objmap.insert(make_pair(name, newobj));
			newobj->nameindex = name;
			lastobj = newobj;
			//newobj->intindex = objvec.size() - 1;
			return true;
		}
		bool addobj(string name, float radius, int2* pos, bool bounce, bool gravaffected)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			if (objmap.find(name) != objmap.end())
				return false;
			physobj* newobj = new physobj(new float(radius), pos, bounce);
			objvec.push_back(name);
			objmap.insert(make_pair(name, newobj));
			newobj->nameindex = name;
			lastobj = newobj;
			//newobj->intindex = objvec.size() - 1;
			return true;
		}
		physobj* GetLastObj()
		{
			return lastobj;
		}
		bool FindOBJ(string name)
		{
			return(MapFind(objmap, name));
		}
		bool delOBJ(string name)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			if (objmap.find(name) != objmap.end())
			{
				physobj* todelete = objmap[name];
				if (lastobj == todelete)
					lastobj = nullptr;
				objmap.erase(name);
				//objvec.erase(objvec.begin() + todelete->intindex);
				int i = 0;
				todelete->Release();
				delete todelete;
				return true;
			}
			return false;
		}
		bool delOBJ(unsigned int index)
		{
			if (ismultithreated)
			{
				while (istickinp)
				{
					Sleep(0);
				}
			}
			physobj* todelete = objmap[objvec[index]];
			//objvec.erase(objvec.begin() + index);
			if (lastobj == todelete)
				lastobj = nullptr;
			objmap.erase(todelete->nameindex);
			int i = 0;
			todelete->Release();
			delete todelete;
			return true;
		}
		int GetOBJindex(string name)
		{
			int i = 0;
			while (i < objvec.size())
			{
				if (objvec[i] == name)
					return i;
				i++;
			}
			return -1;
		}
	};
}
using namespace DX2D;
namespace GAME
{
	extern map<physobj*, ui> sshps;
	inline void SelectFuncPhys(physobj* p1, physobj* p2)
	{
		physobj* ob;
		if (p1->rectID == mousephysid)
			ob = p2;
		else
			ob = p1;
		sshps.insert(make_pair(ob, ob->rectID));
	}
}