#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "GUI.h"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "pathfinding.h"
#include "text.hpp"
#include "debugmodule.h"
using namespace AZfilelib;
using namespace DirectX;
using namespace D2D1;
using namespace std;
using namespace GAME;
extern bool wasstarted;
extern bool pausemainth;
extern bool ismainthpaused;
extern bool isfullscreened;
extern HWND hwnd;
extern const float SCREEN_DEPTH;
extern mainwins style;
extern HWND hstart;
extern const float SCREEN_NEAR;
//template<typename v1, typename v2>
//inline bool mapfind(std::map<v1,v2>&m,v1 &key)
//{
//	if (m->find(*key) == m->end())
//		return false;
//	return true;
//}
RECT clientsize;
namespace debugging
{
	namespace sharedv
	{
		extern	camera** maincam;
	}
	extern int2** debugpos;
	debugmain* dbm = nullptr;
	debugging::debugwindow* pdebugdata = nullptr;
	debugwindow*  maincamdebg = nullptr;
}
namespace GAME
{
	extern vector<thread*>scriptthreads;
	vector<entity> entitylist;
	extern int4 camrect;
	extern universe* uniclass;
	map<ui, void(*)(int2&)> funcidmap;
	ship* conship;
	physics* mpclass;
	camera* mcam;
	camera* pcam = nullptr;
	extern 	int2 oldcampos;
	extern 	int4 camrect;
	extern ui movetype;
	namespace selecting
	{
		extern	physics pcshipclick;
	}
	GUI::core* UI;
}
namespace DX2D
{
	map<string, species*> specmap;
	//inline LPCWSTR STRtoLPCW(std::string &str)
	//{
	//	wstring wstr = STRtoWSTR(str);
	//	LPCWSTR lpcw = wstr.c_str();
	//	return lpcw;
	//}
	map<string, stationmodtempIN>stattempINs;
	map<string, stationmodtempOUT>stattempOUTs;
	map<string, stationmodtempIN>coretemps;
	map<string, shiptemp> shiptemps;
	bool isppaused = false;
	bool isfscreentoud = false;
	bool iscurrentlyfscreened = false;
	int score = 0;
	bool callfunc = false;
	int powupcreated = 0;
	int difficulty = 0;
	frame* backgroundframe;
	frame* pf = nullptr;
	vector<frame*> ssfvec; // shield storage vec;
	int projinfly = 0;
	int curprojnum = 0;
	vector <clock_t> projectilefiredate;
	vector<pair<int, string>>projectilevec;
	vector <ship*> shipsvec;
	vector<camera*>*shipcamvec;
	vector <bool*> projrenbp;
	//
	void initships();
	//engine-specific ( mostly ) 
	struct physreactonset;
	ID2D1DeviceContext *hwndRT = nullptr;
	ID2D1Factory1* pD2DFactory = nullptr;
	ID3D11Device1 *Direct3DDevice;
	ID3D11DeviceContext1 *Direct3DContext;
	ID2D1Device *Direct2DDevice;
	IDXGISwapChain1 *DXGISwapChain;
	ID2D1Bitmap1 *Direct2DBackBuffer;
	void(*fformaintocall)(physobj*, physobj*);
	physobj* objtc1;
	physobj* objtc2;

	class main;
	main* DXclass = nullptr;
	class scene;

	//class renderobj
	//{
	//public:
	//	int currentframe;



	//	map <string, vector<frame*> > f;
	//};
	class scene;
	string bslink;
	void initgame(int2& bs);
	//void move(int2& to);
	void subbutton(int2& pos);
	class main
	{
		friend inline void DX2D::Frame();
	private:
		ID2D1RenderTarget *RTp;
		void Render(camera* cam)
		{

			/*ID2D1RenderTarget **//*RTp = *cam->GetRenderTargetP();*/
			ID2D1BitmapRenderTarget *RT = *cam->GetRenderTargetP();
			//RTp->CreateCompatibleRenderTarget(&RT);
			RT->BeginDraw();
			RT->Clear();
			if (cam == DXclass->maincam)
			{
				debugging::maincamdebg->setvar(*cam->GetXYp(), "maincam pos", true, "INT2");
				//RT->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
			}
			int2 oldp;
			D2D_MATRIX_3X2_F trans;
			if (!cam->ignoreXYforrendering)
			{
				trans = Matrix3x2F::Translation(-cam->GetX(), -cam->GetY());
			}
			else
				trans = Matrix3x2F::Translation(0, 0);
			D2D_MATRIX_3X2_F oldtransform;
			RT->GetTransform(&oldtransform);
			D2D1_POINT_2F center = { RT->GetSize().width,RT->GetSize().height };	
			RT->SetTransform(trans*D2D1::Matrix3x2F::Rotation(cam->GetRot(), center));
			D2D_MATRIX_3X2_F newtransform;
			RT->GetTransform(&newtransform);
			int s = 0;
			int i = 0;
			int ii = 0;
			int iii = 0;
			while (s < cam->scenes.size())
			{
				renderframevec(cam->scenes[s].objectsvec, cam, RT);
				s++;
			}
			textc.RenderCam(cam);
			HRESULT hr = RT->EndDraw();
			if (cam->bm != NULL)
			{
				cam->bm->Release();
				cam->bm = NULL;
			}
			RT->GetBitmap(&cam->bm);
			RT->SetTransform(oldtransform);
			//RT->Release();
			//RTp->Release();
			int fooshit = -1;
			//cam->SetX(cam->GetX() - cam->GetOffsetX());
			//cam->SetY(cam->GetY() - cam->GetOffsetY());
		}
	public:
		debugging::debugmain dbmain;
		
		factions facclass;
		map<string, module*> modtemps;
		map<string, AZfile> modules;
		controls con;
		textclass textc;
		string apploc = "C:\\";
		wstring GetLinkToEl(string elemloc)
		{
			string str = apploc + elemloc;
			return STRtoWSTR(str);
		}
		string GetLinkToElSTR(string elemloc)
		{
			return apploc + elemloc;
		}
		physics pclass;
		ID2D1DeviceContext* RenderTarget = nullptr;
		camera* maincam;
		int GetpoX(float perc)
		{
			RECT r;
			GetWindowRect(hwnd, &r);
			return (r.right - r.left) * (perc/100);
		}
		int GetpoY(float perc)
		{
			RECT r;
			GetWindowRect(hwnd, &r);
			return (r.bottom - r.top) * (perc/100);
		}
		debugging::debugwindow* programdata = nullptr;
		int2 GetWindowSize()
		{
			RECT r;
			GetWindowRect(hwnd, &r);
			return{ (r.left - r.right),(r.bottom - r.top) };
		}
		vector <camera*> cams;
		void renderframevec(vector<frame*>f, camera* cam, ID2D1BitmapRenderTarget *RT)
		{
			int i = 0;
			D2D_MATRIX_3X2_F oldtransform;
			D2D_MATRIX_3X2_F trans;
			if (cam->ignoreXYforrendering)
			{
				trans = Matrix3x2F::Translation(0, 0);
			}
			else 
				trans = Matrix3x2F::Translation(-cam->GetX(), -cam->GetY());
			RT->GetTransform(&oldtransform);
			RT->SetTransform(trans);
			D2D_SIZE_F SZ = RT->GetSize();
			while (i < f.size())
			{
				if (f[i]->ismactive)
				{
					//render
					int ii = 0;
					while (ii < f[i]->lines.size())
					{
						if(f[i]->lines[ii].renderp != nullptr)
							if (!*f[i]->lines[ii].renderp)
							{
								ii++;
								continue;
							}
						RT->DrawLine(f[i]->lines[ii].p1, f[i]->lines[ii].p2, f[i]->lines[ii].color);
						ii++;
					}
					ii = 0;
					while (ii < f[i]->sprites.size())
					{
						if (f[i]->sprites[ii].render != nullptr)
							if (!*f[i]->sprites[ii].render)
							{
								ii++;
								continue;
							}
						if (f[i]->sprites[ii].useidentp)
						{
							if (f[i]->sprites[ii].identp == nullptr)
							{
								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
								continue;
							}
							else if (*f[i]->sprites[ii].identp == NULL)
							{
								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
								continue;
							}
							else if (f[i]->sprites[ii].identp == false)
							{
								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
								continue;
							}
						}
						if (f[i]->sprites[ii].forceoverridepos)
						{
							if (f[i]->sprites[ii].overridepos != nullptr)
							{
								f[i]->sprites[ii].SyncPos(new int2(*f[i]->sprites[ii].overridepos),false);
								f[i]->sprites[ii].forceoverridepos = false;
							}
						}
						D2D_RECT_F loc;
						loc.top = (f[i]->sprites[ii].GetY() + f[i]->sprites[ii].GetOffSetp()->y);
						loc.left = (f[i]->sprites[ii].GetX() + f[i]->sprites[ii].GetOffSetp()->x+0);
						loc.bottom = (f[i]->sprites[ii].GetY() + f[i]->sprites[ii].size.height + f[i]->sprites[ii].GetOffSetp()->y)/**cam->scale.y*/;
						loc.right = (f[i]->sprites[ii].GetX() + f[i]->sprites[ii].size.width + f[i]->sprites[ii].GetOffSetp()->x)/**cam->scale.x*/;
						if (f[i]->sprites[ii].breakonrender)
							DebugBreak();
						if (f[i]->sprites[ii].savelastloc)
						{
							if (f[i]->sprites[ii].lastlochaststarted)
							{
								if (f[i]->sprites[ii].breakonlocchange)
								{
									if ((int4)loc != (int4)f[i]->sprites[ii].lastloc)
									{
										DebugBreak();
									}
								}
								f[i]->sprites[ii].lastloc = loc;
							}
							else
							{
								f[i]->sprites[ii].lastloc = loc;
							}
						}
						D2D_MATRIX_3X2_F oldtransform2;
						RT->GetTransform(&oldtransform2);
						D2D_POINT_2F p;
						if (!cam->usecustomrenderrotpoint)
						{
							int2 rotpoint;
							if (!f[i]->sprites[ii].usecustompoint)
							{
								rotpoint = int2((f[i]->sprites[ii].size.width / 2), (f[i]->sprites[ii].size.height / 2));
							}
							else
							{
								rotpoint = f[i]->sprites[ii].customrpoint;
								//f[i]->sprites[ii].Rotate(0.5);
							}
							
							p = { (-cam->GetX())+(f[i]->sprites[ii].GetX() + f[i]->sprites[ii].GetOffSetp()->x + rotpoint.x)/**cam->scale.x*/,
							(-cam->GetY())+(f[i]->sprites[ii].GetY() + f[i]->sprites[ii].GetOffSetp()->y + rotpoint.y)/**cam->scale.y*/ };
							RT->SetTransform(trans*/*Matrix3x2F::Translation(cam->GetX(), cam->GetY())**/Matrix3x2F::Rotation(f[i]->sprites[ii].GetRot(), p));
						}
						//else
						//{
						//	RT->SetTransform(trans*Matrix3x2F::Rotation(f[i]->sprites[ii].GetRot(), cam->customrotpoint));
						//}
						RT->DrawBitmap(f[i]->sprites[ii].GetBitmap(), loc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
						RT->SetTransform(oldtransform2);
						ii++;
					}
					ii = 0;
					while (ii < f[i]->freepixels.size())
					{
						const D2D1_ELLIPSE d2el = { CD2DPointF(f[i]->freepixels[ii].X, f[i]->freepixels[ii].Y) , 0, 0 };
						//d2el.point = CD2DPointF(f[i]->freepixels[ii].X, f[i]->freepixels[ii].Y);
						//d2el.radiusX = 1;
						//d2el.radiusY = 1;
						RT->DrawEllipse(&d2el, f[i]->freepixels[ii].color);
						ii++;
					}
					ii = 0;
					while (ii < f[i]->brushes.size())
					{
						if (f[i]->brushes[ii].useidentp)
						{
							if (f[i]->brushes[ii].identp == nullptr)
							{
								if (f[i]->brushes[ii].renderp == nullptr)
									f[i]->brushes[ii].renderp = new bool(false);
								else 
									*f[i]->brushes[ii].renderp = (false);
								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
								continue;
							}
							else if (*f[i]->brushes[ii].identp == false)
							{
								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
								continue;
							}
							else if (*f[i]->brushes[ii].identp == NULL)
							{
								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
								continue;
							}
						}
						if (f[i]->brushes[ii].renderp != nullptr)
						{
							if (!*f[i]->brushes[ii].renderp)
							{
								ii++;
								continue;
							}
						}
						if (f[i]->brushes[ii].breakonrender)
							DebugBreak();
						if (f[i]->brushes[ii].createrectfrompossize)
						{
							f[i]->brushes[ii].rect.left = f[i]->brushes[ii].pos->x;
							f[i]->brushes[ii].rect.right = f[i]->brushes[ii].pos->x + f[i]->brushes[ii].size->x;
							f[i]->brushes[ii].rect.top = f[i]->brushes[ii].pos->y;
							f[i]->brushes[ii].rect.bottom = f[i]->brushes[ii].pos->y + f[i]->brushes[ii].size->y;
						}
						switch (f[i]->brushes[ii].brushtype)
						{
						case brushtypes::bitmapbrush:
						{
							if (f[i]->brushes[ii].rot != nullptr && *f[i]->brushes[ii].rot != 0)
							{
								if (!cam->usecustomrenderrotpoint)
									RT->SetTransform(Matrix3x2F::Translation(cam->GetX(),cam->GetY())*Matrix3x2F::Rotation(*f[i]->brushes[ii].rot));
								RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.bitmapbrush);
								if (!cam->usecustomrenderrotpoint)
									RT->SetTransform(oldtransform);
							}
							else
								RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.bitmapbrush);
							break;
						}
						case brushtypes::lineargradient:
						{
							RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.lineargradient);
							break;
						}
						case brushtypes::solidbrush:
						{
							RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.solidbrush.first);
							RT->DrawRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.solidbrush.second);
							break;
						}
						case brushtypes::radialgradient:
						{
							RT->FillEllipse(f[i]->brushes[ii].elipse, f[i]->brushes[ii].b.radialgradient.first);
							RT->DrawEllipse(f[i]->brushes[ii].elipse, f[i]->brushes[ii].b.radialgradient.second);
							break;
						}
						}
						ii++;
					}
				}
				if (f[i]->ischfact)
				{
					renderframevec(f[i]->f[f[i]->wchiac], cam, RT);
				}
				i++;
			}
			RT->SetTransform(oldtransform);
		}
		IDXGIFactory2 *dxgiFactory;
		IDXGIAdapter *dxgiAdapter;
		IDXGIDevice *dxgiDevice;
		ID3D11Device *device;
		ID3D11DeviceContext *context;
		IDXGISurface *dxgiBackBuffer;
		void init(mainwins style)
		{
			int i = 0;		
			RECT rc;
			GetClientRect(hwnd, &rc);
			debugging::dbm = &dbmain;
			//
			HRESULT hr = NULL;
#if defined(DEBUG) || defined(_DEBUG)
			D2D1_FACTORY_OPTIONS options;
			ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
			options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void **>(&pD2DFactory));
#else
			D2D1_FACTORY_OPTIONS options;
			ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, reinterpret_cast<void **>(&pD2DFactory));
#endif
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};
			UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
			D3D_FEATURE_LEVEL returnedFeatureLevel;
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
				&device, &returnedFeatureLevel, &context);
			device->QueryInterface(__uuidof(ID3D11Device1), (void **)&Direct3DDevice);
			context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&Direct3DContext);
			Direct3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&dxgiDevice);
			pD2DFactory->CreateDevice(dxgiDevice, &Direct2DDevice);
			Direct2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &hwndRT);
			dxgiDevice->GetAdapter(&dxgiAdapter);
			dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = rc.right-rc.left;
			swapChainDesc.Height = rc.bottom-rc.top;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.Stereo = false;
			//swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;
			hr = dxgiFactory->CreateSwapChainForHwnd(Direct3DDevice, hwnd, &swapChainDesc, nullptr, nullptr, &DXGISwapChain);
			DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
			FLOAT dpiX, dpiY;
			pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
			D2D1_BITMAP_PROPERTIES1 bitmapProperties =
				D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

			hr = hwndRT->CreateBitmapFromDxgiSurface(dxgiBackBuffer, &bitmapProperties, &Direct2DBackBuffer);
			hwndRT->SetTarget(Direct2DBackBuffer);
			maincam = new camera;
			DXclass->RenderTarget = hwndRT;
			if (!SUCCEEDED(hr))
			{
				DebugBreak();
			}
			if (!SUCCEEDED(hr))
			{
				DebugBreak();
			}
			con.init();
			apploc = style.programloc;
			maincam->reconstruct();
			int2 acs = { clientsize.right - clientsize.left,clientsize.bottom - clientsize.left };
			scene mainscene;
			frame* mf = new frame;
			mf->ismactive = true;
			mf->ischfact = true;
			mf->wchiac = "mainframe";
			frame * f = new frame;
			mf->f.insert(make_pair("mainframe", vector<frame*>({ f })));
			mainscene.objectsmap.insert(make_pair("mainframe", mf));
			mainscene.objectsvec.push_back(mf);
			shipcamvec = &cams;
			maincam->usescale = true;
			maincam->SetScalePoint(  maincam->GetBitmapSize() / 2);
			maincam->scenes.push_back(mainscene);
			GAME::funcidmap.insert(make_pair(666,initgame));
			GAME::funcidmap.insert(make_pair(123, subbutton));
			GAME::funcidmap.insert(make_pair(777, DX2D::loadsavebts));
			GAME::funcidmap.insert(make_pair(105, smovef));
			GAME::funcidmap.insert(make_pair(106, GAME::select));
			GAME::funcidmap.insert(make_pair(107, cammove));
			GAME::mpclass = &pclass;
			bslink = apploc;
			AZfile menu;
			menu.loadfile(GetLinkToElSTR("bin\\data\\menu.file"));
			vector<string>buttons;
			i = 0;
			if (menu.findvar("MAINMENU@BUTTON" + INTtoSTR(i)))
			{
				buttons.push_back(menu.GetVar<string>("MAINMENU@BUTTON" + INTtoSTR(i)));
				i++;
			}
			i = 0;
			while (i < buttons.size())
			{
				int2 bpos;
				bpos.x = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + +"@POSX"));
				bpos.y = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@POSY"));
				int2 bsize;
				bsize.x = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@SIZEX"));
				bsize.y = GetpoY(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@SIZEY"));				
				ui ID = menu.GetVar<ui>("MAINMENU@" + buttons[i] + "@ID");
				string bname = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@NAME");
				string btext = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@TEXT");
				string fontname = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@FONTNAME");
				float fontsize = menu.GetVar<float>("MAINMENU@" + buttons[i] + "@FONTSIZE");
				bool novis = menu.GetVar<bool>("MAINMENU@" + buttons[i] + "@NOVISUALS");
				if (!novis)
					con.addbutton(btext, new int2(bpos), bsize, GAME::funcidmap[ID], mf, maincam, false, textc, fontsize, fontname);
				else
					con.addbutton(btext, new int2(bpos), bsize, { new frame }, GAME::funcidmap[ID], 0, 0, 0, 
						new frame, false, mf, maincam, textc, true, 0, "", { 0,0 }, nullptr, fontsize, fontname);
				string start = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@START");
				if (start != "NULL")
				{
					button* butt = DXclass->con.latestcreation;
					butt->anyvars.push_back(menu);
					butt->anyvars.push_back(start);
					butt->hassubbtns = true;
				}
				i++;
			}
			programdata = new debugging::debugwindow;
			debugging::debugwindow* camdata = new debugging::debugwindow;
			debugging::pdebugdata = programdata;
			programdata->name = "programdata";
			programdata->dvarrect = int4{ 0,0,300,300 };
			programdata->wsize = { 400,500 };
			camdata->dvarrect = int4{ 0,0,300,300 };
			camdata->wsize = { 300,300 };
			programdata->addvar((string&)apploc, "STR", "Program's directory", 8);
			camdata->addvar((int2&)*maincam->GetXYp(), "INT2", "maincam pos");
			mcam = DXclass->maincam;
			camdata->name = "camdata";
			dbmain.subwindows.push_back(programdata);
			dbmain.subwindows.push_back(camdata);
			debugging::maincamdebg = camdata;
			dbmain.startmt();
			if (dbmain.console == nullptr)
				dbmain.console = new debugging::debugconsole;
			dbmain.console->init(true);
			dbmain.console->start();
		}
		void Release()
		{
			ID2D1BitmapRenderTarget *RT;
			RT = *maincam->GetRenderTargetP();
			RT->Release();
			int i = 0;
			while (i < cams.size())
			{
				RT = *cams[i]->GetRenderTargetP();
				if (RT != nullptr)
					RT->Release();
				i++;
			}
			if (RTp != nullptr)
				RTp->Release();
			if (pD2DFactory != nullptr)
				pD2DFactory->Release();
			pclass.Release();
		}
		~main()
		{

		}
	};
	void initships()
	{
		//ship ms;
		//ms.SetBitmapFromFile(DXclass->GetLinkToEl("bin\\img\\bm.png").c_str(), *DXclass->cams[0]->GetRenderTargetP());
		//eng beng;
		//beng.pos = { 0,4 };
		//beng.rot = 270;
		//beng.SetBitmapFromFile(DXclass->GetLinkToEl("bin\\img\\mainengine.png").c_str(), *DXclass->cams[0]->GetRenderTargetP());
		//ms.backeng.push_back(beng);
		//beng.pos = { 0,18 };
		//ms.backeng.push_back(beng);
		//beng.rot = 90;
		//beng.pos = { 26,4 };
		//ms.fronteng.push_back(beng);
		//beng.pos = { 26,18 };
		//ms.fronteng.push_back(beng);
		//eng meng;
		//meng.SetBitmapFromFile(DXclass->GetLinkToEl("bin\\img\\manvengine.png").c_str(), *DXclass->cams[0]->GetRenderTargetP());
		//meng.pos = { 5,2 };
		//meng.rot = 180;
		//ms.leftroteng.push_back(meng);
		//meng.pos = { 21,2 };
		//ms.rightroteng.push_back(meng);
		//meng.pos = { 21,26 };
		//meng.rot = 360;
		//ms.leftroteng.push_back(meng);
		//meng.pos = { 5,26 };
		//ms.rightroteng.push_back(meng);
		//shiptemps.insert(make_pair("testship", ms));
	}
	//program specific, had to be declarated after physobj
	clock_t t;
	void init(mainwins style)
	{
		t = etime::second;
		DXclass = new main;
		DXclass->init(style);
	}
	void Release()
	{
		DXclass->Release();
	}
	bool bbg = false;
	ID2D1SolidColorBrush* placeholderscb = NULL;
	void Frame()
	{
		int i = 0;
		//if (DXclass->maincam->scale.x > 2.0f)
		//	return;
		if (!isppaused)
		{
			DXclass->pclass.tick();
			double ti = (double)((double)clock() / (double)CLOCKS_PER_SEC) - (double)((double)t / (double)CLOCKS_PER_SEC);
			if (ti >= (double)0.1)
			{
				while (i < shipsvec.size())
				{
					shipsvec[i]->tick();
					//cout << "ship " << i << " pos is " << "X=" << ships[i].pos->x << " Y=" << ships[i].pos->y << endl;
					i++;
				}
				i = 0;
				if (uniclass != nullptr)
				{
					while (i < uniclass->sysvec.size())
					{
						int ii = 0;
						while (ii < uniclass->sysvec[i]->statvec.size())
						{
							station* stat = uniclass->sysvec[i]->statvec[ii];
							stat->tick();
							ii++;
						}
						i++;
					}
				}
				t = clock();
			}
		}
		i = 0;
		DXclass->RenderTarget->BeginDraw();
		DXclass->RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		int2 offsetmc = { 0,0 };
		if (DXclass->maincam->usescale && DXclass->maincam->scale.x > 1 && DXclass->maincam->scale.y > 1)
		{
			int2 os = DXclass->maincam->GetBitmapSize();
			offsetmc = int2((DXclass->maincam->scale.x * os.x)/2, (DXclass->maincam->scale.y * os.y) / 2);
			DXclass->maincam->SetOffset(offsetmc);
		}
		DXclass->Render(DXclass->maincam);
		HRESULT hr;
		RECT r;
		GetWindowRect(hwnd, &r);
		D2D1_SIZE_F sze = DXclass->maincam->bm->GetSize();
		D2D1_RECT_F d2r = { 0 /*+DXclass->maincam->GetX()*/,0/*+DXclass->maincam->GetY()*/,sze.width /*+ DXclass->maincam->GetX()*/,sze.height /*+ DXclass->maincam->GetY()*/ };
		ID2D1Bitmap *bm1 = NULL;
		ID2D1BitmapRenderTarget* BRT = *DXclass->maincam->GetRenderTargetP();
		bool wasBRTenddrawc = true;
		/*	bm->Release();*/
		i = 0;
		DXclass->con.tick();
		camera* lcam = nullptr;
		while (i < DXclass->cams.size())
		{
			//DXclass->cams[i]->SetOffset(DXclass->cams[i]->GetOffset()+DXclass->maincam->GetOffset());
			if (lcam == DXclass->cams[i])
			{
				DXclass->cams.erase(DXclass->cams.begin() + i);
				continue;
			}
			else 
				DXclass->Render(DXclass->cams[i]);
			if (DXclass->cams[i]->isrendonscreen)
			{
				ID2D1Bitmap *bm;
				ID2D1BitmapRenderTarget* BRTc = *DXclass->cams[i]->GetRenderTargetP();
				BRTc->GetBitmap(&bm);

				D2D1_SIZE_F sz = DXclass->cams[i]->bm->GetSize();
				DXclass->cams[i]->loc = { (float)DXclass->cams[i]->GetX(),(float)DXclass->cams[i]->GetY(),
					((float)DXclass->cams[i]->GetX()) + ((float)sz.width,(float)DXclass->cams[i]->GetY()) + (float)sz.height };
				BRT->BeginDraw();
				wasBRTenddrawc = false;
				if (DXclass->cams[i]->isonlyrectrendered)
				{
					D2D_RECT_F rf;
					ID2D1Bitmap *bmcopy = NULL;
					const D2D1_RECT_U cru = DXclass->cams[i]->renderRECT;
					D2D1_POINT_2U POINT;
					POINT.x = 0;
					POINT.y = 0;
					//D2D1_ALPHA_MODE_PREMULTIPLIED
					D2D1_BITMAP_PROPERTIES dbp;
					dbp.pixelFormat = bm->GetPixelFormat();
					bm->GetDpi(&dbp.dpiX, &dbp.dpiY);
					//dbp.pixelFormat = PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
					HRESULT HR = BRT->CreateBitmap({ cru.bottom - cru.top,cru.right - cru.left }, dbp, &bmcopy);
					HR = bmcopy->CopyFromBitmap(&POINT, bm, &cru);
					rf = { (float)cru.left,(float)cru.top,(float)cru.right,(float)cru.bottom };
					//if (!DXclass->cams[i]->ignoreXYforrendering)
					//{
						rf.left += DXclass->cams[i]->GetX();
						rf.right += DXclass->cams[i]->GetX();
						rf.top += DXclass->cams[i]->GetY();
						rf.bottom += DXclass->cams[i]->GetY();
					//}
					rf.left += -DXclass->maincam->GetX() /*+ offsetmc.x + DXclass->maincam->GetOffsetX()*/;
					rf.right += -DXclass->maincam->GetX()/* + offsetmc.x + DXclass->maincam->GetOffsetX()*/;
					rf.top += -DXclass->maincam->GetY()/* + offsetmc.y + DXclass->maincam->GetOffsetY()*/;
					rf.bottom += -DXclass->maincam->GetY()/* + offsetmc.y + DXclass->maincam->GetOffsetY()*/;
					D2D1_SIZE_F _sz = bmcopy->GetSize();
					Matrix3x2F ot;
					D2D1_POINT_2F p2f;
					BRT->GetTransform(&ot);
					if (DXclass->cams[i]->usecustomrotpoint)
					{
						p2f = DXclass->cams[i]->customrotpoint;
						p2f.x += DXclass->cams[i]->GetX();
						p2f.y += DXclass->cams[i]->GetY();
						p2f.x += -DXclass->maincam->GetX()/* + DXclass->maincam->GetOffsetX()*/;
						p2f.y += -DXclass->maincam->GetY()/* + DXclass->maincam->GetOffsetY()*/;
					}
					else
						p2f = { rf.left + (_sz.width / 2) + DXclass->cams[i]->rotpointoffset.x,rf.top + (_sz.height / 2) + DXclass->cams[i]->rotpointoffset.y };
					BRT->SetTransform(/*Matrix3x2F::Translation(DXclass->cams[i]->GetX(), DXclass->cams[i]->GetY())**/ Matrix3x2F::Rotation((DXclass->cams[i]->GetRot()), p2f));
					BRT->DrawBitmap(bmcopy, rf, 1.0, defaultinterpolationmode );

					//if (placeholderscb == NULL)
					//{
					//	BRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LawnGreen, 1.0f), &placeholderscb);
					//}
					//BRT->DrawEllipse(D2D1_ELLIPSE({ p2f,2.0f,2.0f }), placeholderscb);
					BRT->SetTransform(ot);
					bmcopy->Release();
				}
				else
				{
					BRT->DrawBitmap(bm, d2r, 1.0, defaultinterpolationmode);
				}
				wasBRTenddrawc = true;
				BRT->EndDraw();
				bm->Release();
			}
			lcam = DXclass->cams[i];
			i++;
		}
		if(wasBRTenddrawc == false)
			if(DXclass->cams.size() != 0)
				BRT->EndDraw();
		bm1 = DXclass->maincam->bm;
		if (DXclass->maincam->usescale && DXclass->maincam->scale.x != 1 && DXclass->maincam->scale.y != 1)
		{
			bool down = !(DXclass->maincam->scale.x > 1 && DXclass->maincam->scale.y > 1);
			if (!down)
			{
#if catchleak == 1
				if (_CrtDumpMemoryLeaks())
					__debugbreak();
#endif // !catchleak == 1
				goto scaleend;

			}
			D2D1_SIZE_F oldsize = bm1->GetSize();
			int Xdif = (down) ? oldsize.width - oldsize.width*DXclass->maincam->scale.x : 0;
			int Ydif = (down) ? oldsize.height - oldsize.height*DXclass->maincam->scale.y : 0;
			int Xrc = (down) ? oldsize.width - (Xdif / 2) : oldsize.width + (oldsize.width*DXclass->maincam->scale.x) / 2;
			int Yrc = (down) ? oldsize.height - (Ydif / 2) : oldsize.height + (oldsize.height*DXclass->maincam->scale.y) / 2;
			int Xlc = (down) ? Xdif / 2 : 0;
			int Ylc = (down) ? Ydif / 2 : 0;
			D2D_RECT_F rf;
			ID2D1Bitmap *bmcopy = NULL;
			D2D1_RECT_F oldRECT = (down) ? D2D1_RECT_F{ 0, 0, oldsize.width, oldsize.height } : D2D1_RECT_F{ 0, 0, (float) /*oldsize.width*/Xrc,(float)Yrc/*oldsize.height*/ };
			//oldRECT = D2D1_RECT_F{ 0, 0, oldsize.width, oldsize.height };
			int2 addoffset = DXclass->maincam->GetOffset();
			const D2D1_RECT_U cru = /*(down) ?*/ D2D1_RECT_U{ (ui)Xlc /*+ DXclass->maincam->GetX()*/,(ui)Ylc /*+ DXclass->maincam->GetY()*/,(ui)Xrc /*+ DXclass->maincam->GetX()*/,(ui)Yrc /*+ DXclass->maincam->GetY()*/ }
			/*: D2D1_RECT_U{ (ui)oldRECT.left,(ui)oldRECT.top,(ui)oldRECT.right,(ui)oldRECT.bottom }*/;
			D2D1_POINT_2U POINT;
			POINT.x = 0;
			POINT.y = 0;
			//D2D1_ALPHA_MODE_PREMULTIPLIED
			D2D1_BITMAP_PROPERTIES dbp;
			dbp.pixelFormat = bm1->GetPixelFormat();
			bm1->GetDpi(&dbp.dpiX, &dbp.dpiY);
			//dbp.pixelFormat = PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
			HRESULT HR = BRT->CreateBitmap({ cru.right - cru.left,cru.bottom - cru.top }, dbp, &bmcopy);
			HR = bmcopy->CopyFromBitmap(&POINT, bm1, &cru);
			rf = { (float)cru.left,(float)cru.top,(float)cru.right,(float)cru.bottom };
			rf.left += DXclass->maincam->GetX();
			rf.right += DXclass->maincam->GetX();
			rf.top += DXclass->maincam->GetY();
			rf.bottom += DXclass->maincam->GetY();
			D2D1_SIZE_F _sz = bmcopy->GetSize();
			Matrix3x2F ot;
			D2D1_POINT_2F p2f;
			BRT->GetTransform(&ot);
			//addoffset = { Xrc / 2,Yrc / 2 };
			DXclass->maincam->SetOffset(addoffset);
			addoffset = { 0,0 };
			if (DXclass->maincam->usecustomrotpoint)
			{
				p2f = (D2D1_POINT_2F)((uni2<float>) DXclass->maincam->customrotpoint + uni2<float>((float)addoffset.x, (float)addoffset.y));
			}
			else
				p2f = { rf.left / 2 + DXclass->maincam->rotpointoffset.x,rf.top / 2 + DXclass->maincam->rotpointoffset.y };
			DXclass->RenderTarget->SetTransform(Matrix3x2F::Translation(addoffset.x, addoffset.y)*
				Matrix3x2F::Rotation((DXclass->maincam->GetRot()), p2f));

			DXclass->RenderTarget->DrawBitmap(bmcopy, oldRECT, 1.0, advancedinterpolatonmode);
			DXclass->RenderTarget->SetTransform(ot);
			bmcopy->Release();
			//}
			//else if ((DXclass->maincam->scale.x > 1.0f && DXclass->maincam->scale.y > 1.0f))
			//{
			//	D2D1_SIZE_F oldsize = bm1->GetSize();

			//}
		}
		else
			scaleend:DXclass->RenderTarget->DrawBitmap(bm1, d2r, 1.0, advancedinterpolatonmode);
		//bm1->Release();
		hr = DXclass->RenderTarget->EndDraw();
		displayHRerrors(hr, hwnd, __LINE__ - 1, false, "ID2D1DeviceContext->EndDraw has failed");

		DXGI_PRESENT_PARAMETERS parameters = { 0 };
		parameters.DirtyRectsCount = 0;
		parameters.pDirtyRects = nullptr;
		parameters.pScrollRect = nullptr;
		parameters.pScrollOffset = nullptr;

		hr = DXGISwapChain->Present1(1, 0, &parameters);
		displayHRerrors(hr, hwnd, __LINE__ - 1, false, "DXGISwapChain->Present1 has failed");

	}
	bool wp = false;
	bool sp = false;
	bool irq = false;
	bool ire = false;
	bool irqed = false;
	bool ireed = false;
	bool isfac = false;
	bool isbac = false;
	bool isfpinr = false; // is it first projectile?
	bool isprojtim = false;
	string saveslot = "defsave";
	bool wasinitgamel = false;
	void initgame(int2& bs)
	{
		if (wasinitgamel)
			return;
		wasinitgamel = true;
		DXclass->con.clearbuttons(DXclass->textc);
		DXclass->textc.clear();
		if (bslink[bslink.length() - 1] != '\\' && bslink[bslink.length() - 1] != '/')
		{
			bslink += "\\";
		}
		bslink += "bin\\";
		camera* maincam = DXclass->maincam;
		debugging::sharedv::maincam = &maincam;
		frame* mf = maincam->scenes[0].objectsvec[0];
		int2 a= maincam->GetBitmapSize();
		camrect = int4{ 0,0,a.x,a.y };
		AZfile menu;
		menu.loadfile(bslink + "data\\menu.file");
		vector<string>buttons;
		/*{
			brush br(true,int2(300,300),int2(50,50),0.0f);
			ID2D1SolidColorBrush* scb1 = NULL;
			ID2D1SolidColorBrush* scb2 = NULL;
			ID2D1BitmapRenderTarget* brt = *maincam->GetRenderTargetP();
			brt->CreateSolidColorBrush({ 1.0f,1.0f,1.0f,1.0f }, &scb1);
			brt->CreateSolidColorBrush({ 1.0f,1.0f,1.0f,1.0f }, &scb2);
			br.brushtype = brushtypes::solidbrush;
			br.b.solidbrush =  pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>(scb1,scb2);
			mf->brushes.push_back(br);
		}*/
		int i = 0;
		DXclass->con.mmousemovefunc = cammove;
		DXclass->con.mbdownfunc = cammovesetref;
		//mcam->secposvec.push_back(&oldcampos);
		DXclass->con.mbupfunc = mbmovestop;
		while (menu.findvar("GAME@BUTTON" + INTtoSTR(i)))
		{
			buttons.push_back(menu.GetVar<string>("GAME@BUTTON" + INTtoSTR(i)));
			i++;
		}
		i = 0;
		while (i < buttons.size())
		{
			int2 bpos;
			bpos.x = DXclass->GetpoX(menu.GetVar<float>("GAME@" + buttons[i] + +"@POSX"));
			bpos.y = DXclass->GetpoY(menu.GetVar<float>("GAME@" + buttons[i] + "@POSY"));
			int2 bsize;
			bsize.x = DXclass->GetpoX(menu.GetVar<float>("GAME@" + buttons[i] + "@SIZEX"));
			bsize.y = DXclass->GetpoY(menu.GetVar<float>("GAME@" + buttons[i] + "@SIZEY"));
			ui ID = menu.GetVar<ui>("GAME@" + buttons[i] + "@ID");
			string bname = menu.GetVar<string>("GAME@" + buttons[i] + "@NAME");
			string btext = menu.GetVar<string>("GAME@" + buttons[i] + "@TEXT");
			string fontname = menu.GetVar<string>("GAME@" + buttons[i] + "@FONTNAME");
			float fontsize = menu.GetVar<float>("GAME@" + buttons[i] + "@FONTSIZE");
			bool novis = menu.GetVar<bool>("GAME@" + buttons[i] + "@NOVISUALS");
			if (!novis)
				DXclass->con.addbutton(btext, new int2(bpos), bsize, GAME::funcidmap[ID], mf, maincam, false, DXclass->textc, fontsize, fontname);
			else
				DXclass->con.addbutton(btext, new int2(bpos), bsize, { new frame }, GAME::funcidmap[ID], 0, 0, 0,
					new frame, false, mf, maincam, DXclass->textc, true, 0, bname, { 0,0 }, nullptr, fontsize, fontname);
			if (buttons[i] == "moveb")
			{
				DXclass->con.buttons[DXclass->con.buttons.size()-1]->rbuttonpressfunc = GAME::select;
			}
			i++;
		}
		mcam = DXclass->maincam;
		mpclass = &DXclass->pclass;
		oldcampos = *DXclass->maincam->GetXYp();
		movetype = mt_mov;
		i = 0;
		vector<string> subfolders;
		const char* PATH = ".";
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		string sPath = bslink + "data\\modules\\*";
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					subfolders.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		sPath = bslink + "data\\ships\\*";
		vector <string> shipnames;
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr  = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					shipnames.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		vector<string>smodsin;
		vector<string>smodsout;
		vector<string>scores;
		FindClose(hFind);
		sPath = bslink + "data\\station\\cores\\*";
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					scores.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		sPath = bslink + "data\\station\\modules\\out\\*";
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					smodsout.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		sPath = bslink + "data\\station\\modules\\in\\*";
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					smodsin.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		i = 0;
		while (i < scores.size())
		{
			coretemps.insert(make_pair(scores[i], stationmodtempIN((string)bslink + "data\\station\\cores\\" + scores[i])));
			i++;
		}
		i = 0;
		while (i < smodsout.size())
		{
			stattempOUTs.insert(make_pair(smodsout[i], stationmodtempOUT((string)bslink + "data\\station\\modules\\out\\" + smodsout[i])));
			i++;
		}
		i = 0;
		while (i < smodsin.size())
		{
			stattempINs.insert(make_pair(smodsin[i], stationmodtempIN((string)bslink + "data\\station\\modules\\in\\" + smodsin[i])));
			i++;
		}
		FindClose(hFind);
		i = 0;
		while (i < subfolders.size())
		{
			DXclass->modules.insert(make_pair(subfolders[i], AZfile(bslink + "data\\modules\\" + subfolders[i] + "\\module.data")));
			DXclass->modtemps.insert(make_pair(subfolders[i], new module(bslink + "data\\modules\\" + subfolders[i])));
			i++;
		}
		i = 0;
		while (i < shipnames.size())
		{
			shiptemp st(bslink + "data\\ships\\"+ shipnames[i] + '\\' + shipnames[i] + ".ship");
			shiptemps.insert(make_pair(shipnames[i], st));
			i++;
		}
		i = 0;
		sPath = bslink + "data\\species\\crew\\*";
		vector <string> speciesv;
		hFind = FindFirstFile(sPath.data(), &FindFileData);
		do
		{
			if (FindFileData.dwFileAttributes == 16)
			{
				string fdstr  = FindFileData.cFileName;
				if (fdstr != "." && fdstr != "..")
					speciesv.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
		while (i < speciesv.size())
		{
			DX2D::species* s = new DX2D::species;
			string lnk = bslink + "data\\species\\crew\\" + speciesv[i] + "\\species.data";
			AZfile sf(lnk);
			s->init(lnk, true);

			i++;
		}
		i = 0;
		AZfile unitemp(bslink + "saves\\" + saveslot  +"\\universetemp.unitemp");
		i = 0;
		vector<string>ships;
		while (unitemp.findvar("PLAYER@SHIP" + INTtoSTR(i)))
		{
			ships.push_back(unitemp.GetVar<string>("PLAYER@SHIP" + INTtoSTR(i)));
			i++;
		}
		i = 0;
		string curship = unitemp.GetVar<string>("PLAYER@curship");
		while (i < ships.size())
		{
			if (ships[i] == curship)
				break;
			i++;
		}
		GAME::init(bslink);
		string shipn = "PLYER@" + ships[i] + '@';
		i = 0;
		string shiptype = unitemp.GetVar<string>("PLAYER@" + curship + "@type");
		AZfile playership(bslink + "data\\ships\\" + shipn + '\\' + shipn + ".ship");
		frame* f = new frame;
		pcam = new camera(new int2(0, 0), new float(0));
		pcam->isrendonscreen = true;
		camera*c = new camera(new int2(0, 0), new float(0));
		c->ignoreXYforrendering = true;
		ship*playerp = new ship (shiptemps[shiptype], c, f,mf,maincam,new int2(100,100),uniclass->sysmap[unitemp.GetVar<string>("PLAYER@cursys")],uniclass);
		ship &player = *playerp;
		scene s;
		s.objectsvec.push_back(f);
		s.objectsmap.insert(make_pair("mainshipf", f));
		c->scenes.push_back(s);
		shipcamvec->push_back(c);
		c->scalepoint = { player.size.width/2,player.size.height / 2 };
		player.pos = c->GetXYp();
		*player.universepos = int2(unitemp.GetVar<int>("PLAYER@curposX"), unitemp.GetVar<int>("PLAYER@curposY"));
		DXclass->cams.push_back(GAME::pcam);
		DXclass->cams.push_back(c);
		//mf->f[mf->wchiac].push_back(f);
		//mf->ischfact = true;
		f->ismactive = true;
		vector<string> mods;
		string csd = "PLAYER@" + curship + "@MODULES@";
		while (unitemp.findvar(csd + "modslot" + INTtoSTR(i)))
		{
			module* mtemp = DXclass->modtemps[unitemp.GetVar<string>(csd + "modtype" + INTtoSTR(i))];
			string mtype = unitemp.GetVar<string>(csd + "modslot" + INTtoSTR(i));
			int2 posdebugbullshit = shiptemps[shiptype].modpos[mtype];
			player.AddMod(mtemp, new int2(shiptemps[shiptype].modpos[mtype]), shiptemps[shiptype].modsize[mtype],c, f, mtype);
			i++;
		}
		i = 0;
		vector<string> facnames;
		vector<vector<pair<string, int>>> relmapv;
		csd = "FACTIONS@F";
		while (unitemp.findvar(csd + INTtoSTR(i) +"@name"))
		{
			vector<pair<string, int>> relmap;
			string name = unitemp.GetVar<string>(csd + INTtoSTR(i)+"@name");
			int ii = 0;
			while (unitemp.findvar(csd + INTtoSTR(i) + "@rel" + INTtoSTR(ii) + "n"))
			{
				string n = unitemp.GetVar<string>(csd + INTtoSTR(i) + "@rel" + INTtoSTR(ii) + "n");
				int rel = (int)unitemp.GetVar<int>(csd + INTtoSTR(i) + "@rel" + INTtoSTR(ii) + "r");
				relmap.push_back(make_pair(n, rel));
				ii++;
			}
			facnames.push_back(name);
			relmapv.push_back(relmap);
			i++;
		}
		i = 0;
		map<string,faction*> facmap;
		while (i < facnames.size())
		{
			facmap.insert(make_pair(facnames[i],DXclass->facclass.CreateFaction()));

			i++;
		}
		i = 0;
		while (i < relmapv.size())
		{
			map<faction* ,facrel> facrelmap;
			int ii = 0;
			while (ii < relmapv[i].size())
			{
				facrelmap.insert(make_pair(facmap[relmapv[i][ii].first],(facrel)relmapv[i][ii].second));
				ii++;
			}
			facmap[facnames[i]/*relmapv[i][ii].first*/]->relations = facrelmap;

			i++;
		}
		i = 0;
		vector<pair<string,faction*>> facvec = MaptoVec(facmap);
		while (i < facvec.size())
		{
			facvec[i].second->name = facvec[i].first;
			i++;
		}
		i = 0;
		csd = "PLAYER@" + curship + "@CREW@";
		frame* cf = new frame;
		faction* playerfac = DXclass->facclass.GetFaction(unitemp.GetVar<string>("PLAYER@facname"));
		f->wchiac = "sub";
		InsertToMapOfVecs(f->f, { f->wchiac,cf });
		while (unitemp.findvar(csd + "CM" + INTtoSTR(i) + "@type"))
		{
			crewman* cm = new crewman;
			string dr = csd + "CM" + INTtoSTR(i) + '@';
			species* sp = specmap[unitemp.GetVar<string>(dr+ "type")];
			cm->bmdir = sp->bmvec[unitemp.GetVar<int>(dr + "bmi")];
			cm->wtdws = unitemp.GetVar<int>(dr + "bmi");
			cm->hp = unitemp.GetVar<float>(dr + "hp");
			cm->hpfull = unitemp.GetVar<float>(dr + "mhp");
			cm->sfightb = unitemp.GetVar<float>(dr + "fight");
			cm->spilotb = unitemp.GetVar<float>(dr + "pilot");
			cm->sotherb = unitemp.GetVar<float>(dr + "other");
			cm->sweaponb = unitemp.GetVar<float>(dr + "weapon");
			cm->srepairb = unitemp.GetVar<float>(dr + "repair");
			cm->debuff = sp->combatdebuff;
			cm->name = unitemp.GetVar<string>(dr + "name");
			cm->type = sp;
			sprite cs;
			cs.SetBitmapFromFile(STRtoWSTR(cm->bmdir).c_str(),*c->GetRenderTargetP());
			cm->modname = unitemp.GetVar<string>(dr + "modslot");
			cm->pos = new int2(*player.modmap[cm->modname]->pos);
			cm->slotnum = unitemp.GetVar<int>(dr + "slot");
			*cm->pos = *cm->pos + player.modmap[cm->modname]->slots[cm->slotnum].first;
			cm->r = player.modmap[cm->modname]->rptr;
			cs.SyncPos(cm->pos, true);
			cm->ft = playerfac->CreateToken();
			int ii = i;
			while (!player.crewphys.addobj("crewman" + INTtoSTR(ii++),new int2(3,3),cm->pos,false,false));
			player.crewphys.GetLastObj()->rectID = 666;
			entity ent;
			ent.selectf = nullptr;
			ent.pos = cm->pos;
			ent.entname = "crewman";
			ent.datav.push_back(cm);
			entitylist.push_back(ent);
			player.crewphys.GetLastObj()->anyvars.push_back(ent);
			player.fr->sprites.push_back(cs);
			player.crewmap.insert(make_pair(cm->name, cm));
			i++;
		}
		i = 0;
		csd = "PLAYER@" + curship + "@CARGO@RES@";
		while (unitemp.findvar(csd + "T" + INTtoSTR(i)))
		{
			shipres rest = getshipresfromstr(unitemp.GetVar<string>(csd + "T" + INTtoSTR(i)));
			float fam = (unitemp.GetVar<float>(csd + "TA" + INTtoSTR(i)));
			player.res.insert(make_pair(rest,fam));
			i++;
		}
		i = 0;
		player.ft = playerfac->CreateToken();
		::shipsvec.push_back(playerp);
		i = 0;
		conship = nullptr;
		conship = ::shipsvec[::shipsvec.size()-1];
		while(!DXclass->pclass.addobj("playership", new int2(conship->size), conship->pos, false, false));
		while (!selecting::pcshipclick.addobj("playership", new int2(conship->size), conship->pos, false, false));
		physreact rct;
		rct.ftocall = shipselecthit;
		rct.callfunc = true;
		rct.callfuncinstant = true;
		selecting::pcshipclick.objmap["playership"]->IDreactionmap.insert(make_pair(mousephysid,rct));
		conship->pobj = DXclass->pclass.GetLastObj();
		selecting::pcshipclick.objmap["playership"]->isron = true;
		entity mainshipen;
		ship* conshipbullshit = conship;
		conship->name = curship;
		mainshipen.selectf = GAME::selectship;
		mainshipen.datav.push_back(conship);
		conship->pobj->anyvars.push_back(mainshipen);
		selecting::pcshipclick.objmap["playership"]->anyvars.push_back(mainshipen);
		mainshipen.pos = conship->pos;
		mainshipen.entname = "Ship";
		entitylist.push_back(mainshipen);
		c->isrendonscreen = true;
		c->isonlyrectrendered = true;
		c->renderRECT.left = 0;
		c->renderRECT.top = 0;
		int highest = GetHighestof2(player.size.width, player.size.height);
		c->renderRECT.right = highest;
		c->renderRECT.bottom = highest;
		//c->renderRECT.right = player.size.width;
		//c->renderRECT.bottom = player.size.height;
		c->usecustomrotpoint = true;
		c->customrotpoint = { player.size.width / 2,player.size.height / 2 };
		debugging::debugwindow* shipdbd = new debugging::debugwindow;
		shipdbd->dvarrect = int4{ 0,0,300,240 };
		shipdbd->subwindowsrect = int4(0, 240, 300, 300);
		shipdbd->wsize = { 300,340 };
		shipdbd->addvar((int2&)*c->GetXYp(), "INT2", "shippos");
		shipdbd->addvar((float&)*c->GetRotP(), "FLOAT", "shiprot");
		shipdbd->addvar((string&)conship->name, "STR", "conship");
		shipdbd->name = "ship data";
		debugging::debugwindow* shipresdbd = new debugging::debugwindow;
		debugging::debugwindow* shipcrew = new debugging::debugwindow;
		shipcrew->name = "crew";
		shipcrew->wsize = int2(defdbbsizex*3, defdbbsizey*5);
		shipcrew->dvarrect = int4(0,0,defdbbsizex * 3, defdbbsizey * 5);
		shipresdbd->name = "shipres";
		shipdbd->subwindows.push_back(shipresdbd);
		shipdbd->subwindows.push_back(shipcrew);
		DXclass->dbmain.addsw(shipdbd);
		shipresdbd->dvarrect = int4{ 0,0,600,320 };
		shipresdbd->wsize = { 600,320 };
		conship->dwnd = shipdbd;
		pcam = maincam;
		conship->resdwnd = shipresdbd;
		loadsector(maincam,mf,*player.universepos,uniclass,uniclass->eco,player.sys,player.sys->s,player);
		maincam->SetXY(*player.pos + int2(-100,-100));
		GAME::UI = new GUI::core;
		f = new frame;
		//mf->f[mf->wchiac].push_back(f);
		camera* uicam = new camera;
		uicam->loc = maincam->loc;
		DXclass->cams.push_back(uicam);
		f->ismactive = true;
		uicam->reconstruct();
		scene uis;
		uis.objectsmap.insert(make_pair("ui", f));
		uis.objectsvec.push_back(f);
		uicam->scenes.push_back(uis);
		uicam->isrendonscreen = true;
		GAME::UI->init(&DXclass->con, uicam, f);
		GAME::GUI::loadGUIdata(bslink, GAME::UI);
		GAME::GUI::loadui(bslink, GAME::UI);
	}
	bool isdbbshown = false;
	bool isscrdwnon = false; // if true - scrolling will call scrollfunc and will not scale
	void(*scrollfunc)(int2&,double&) = nullptr; //mouse pos, delta
	camera* loadcam = nullptr;
	controls loadcon;
	void scrolldownload(int2& pos, double& delta)
	{
		loadcam->SetY(loadcam->GetY() - delta / 5000);
	}
	vector<pair<int4, string>> saves;
	void loadpressfunc(int2& pos)
	{
		int i = 0;
		while (i < saves.size())
		{
			if (classvariables::isinside(saves[i].first, pos))
				break;
			i++;
		}
		saveslot = saves[i].second;
		initgame(pos);
	}
	void loadsavebts(int2& pos)
	{
		loadcon.init();
		static int savesinaline = 5;
		int2 wsz = DXclass->GetWindowSize();
		int4 rsz = { wsz / 6,wsz - wsz / 6 };
		int2 buttonsize = (rsz.second()-rsz.first()) / savesinaline;
		buttonsize.y /= 3;
		isscrdwnon = true;
		scrollfunc = scrolldownload;
		if (loadcam != nullptr)
		{
			int i = 0;
			while (i < DXclass->cams.size())
			{
				if (DXclass->cams[i] == loadcam)
					DXclass->cams.erase(DXclass->cams.begin() + i);
				i++;
			}
			loadcam->deleteatdest = true;
			delete loadcam;
		}
		loadcam = new camera;
		DXclass->cams.push_back(loadcam);
		int i = 0;
		vector<string> savesstr;
		WIN32_FIND_DATA fuckwinapi;
		HANDLE hFind;
		string sPath = bslink + "saves\\*";
		hFind = FindFirstFile(sPath.data(), &fuckwinapi);
		do
		{
			if (fuckwinapi.dwFileAttributes == 16)
			{
				string fdstr = fuckwinapi.cFileName;
				if (fdstr != "." && fdstr != "..")
					if(fdstr != "defsave")
						savesstr.push_back(fdstr);
			}
		} while (FindNextFile(hFind, &fuckwinapi));
		FindClose(hFind);
		i = 0;
		int ii = 0;
		int2 prev = rsz.first();
		loadcam->isrendonscreen = true;
		loadcam->renderRECT = rsz;
		while (i < savesstr.size())
		{
			int2 pos = prev;
			if (ii >= savesinaline)
			{
				ii = 0;
				pos.x = 0;
				pos.y += buttonsize.y;
			}
			else
			{
				prev.x += buttonsize.x;
				ii++;
			}
			prev = pos;
			frame* f = new frame;
			scene sc;
			sc.objectsmap.insert(make_pair("#nolivesmatter", f));
			sc.objectsvec.push_back(f);
			loadcam->scenes.push_back(sc);
			AZfile data(bslink + "saves\\" + savesstr[i]);
			string text = savesstr[i] + "\n" + "date: " + data.GetVar<string>("date") + " time: " + data.GetVar<string>("time") + "\n" + "current ship: " + data.GetVar<string>("shiptype") 
				+ "(" + data.GetVar<string>("ship") + ") shipcount: " + INTtoSTR(data.GetVar<int>("ships")) +"\nlocation:" + data.GetVar<string>("plocation");
			loadcon.addbutton(text, new int2(pos), buttonsize, loadpressfunc,f,loadcam,true,loadcon.tc );
			saves.push_back(make_pair(int4(pos, buttonsize + pos), savesstr[i]));
			i++;
		}
		i = 0;
	}
	LRESULT EmulateLEwinproc(HWND hwndwin, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (callfunc)
		{
			fformaintocall(objtc1, objtc2);
			callfunc = false;
		}
		if (DXclass != nullptr)
			if (DXclass->pclass.isthrpaused)
			{
				DXclass->pclass.isthrpaused = false;
			}

		int i = 0;
		int ii = 0;
		int iii = 0;
		switch (msg)
		{
		case WM_CREATE:
		{
			break;
		}
		case WM_TIMER:
		{
			if (!isppaused)
			{
				switch (wParam)
				{
				case fullscreentimeouttimer:
				{
					KillTimer(hwnd, fullscreentimeouttimer);
					isfscreentoud = false;
					break;
				}
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_F11:
			{
				
				ShowWindow(DXclass->dbmain.hwindow, (isdbbshown = !isdbbshown) ? SW_SHOW:SW_HIDE);
				break;
			}
			case VK_PAUSE:
			case VK_P:
			{
				if (!isppaused)
				{
					int2 acs = { clientsize.right - clientsize.left,clientsize.bottom - clientsize.left };
					//DXclass->pclass.pausethread();
					DXclass->textc.addtext(DXclass->maincam, "Consolas",
						new int2(acs.x / 2 - acs.x / 6, acs.y / 2 - acs.y / 6), { acs.x / 6,acs.y / 6 }, 50, "pause", "PAUSED");
					isppaused = true;
				}
				else
				{
					DXclass->pclass.resetcdf();
					//DXclass->pclass.unpausethread();
					DXclass->textc.deltext(DXclass->maincam, "pause", true);
					isppaused = false;
				}
				break;
			}
			}
		}
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MOUSEMOVE:
		{
			if (DXclass == nullptr)
				break;
			if (GetKeyState(VK_SHIFT))
				movetype = mt_movnext;
			else
				movetype = mt_mov;
			DXclass->con.MouseEvent(wParam, lParam, msg);
			int i = 0;
			if(UI != nullptr)
				UI->MouseEvent(wParam, lParam, msg);
			if (DXclass->programdata != nullptr)
			{
				int2 pos(GET_X_LPARAM((lParam)), GET_Y_LPARAM((lParam)));
				DXclass->programdata->setvar(int2(GET_X_LPARAM((lParam)), GET_Y_LPARAM((lParam))), "mousepos", true, "INT2");
				uni2<float> posf2 = (uni2<float>)pos - (uni2<float>)*DXclass->maincam->GetXYp();
				posf2 =  posf2+ (uni2<float>)DXclass->maincam->GetOffset();
				posf2 = posf2* (uni2<float>)DXclass->maincam->scale;
				DXclass->programdata->setvar((int2)posf2.toint2(),"mouseposrelative",true,"INT2");
			}
			i = 0;
			while (i < scriptthreads.size())
			{
				try
				{
					scriptthreads[i]->join();
				}
				catch (...)
				{

				}
				delete scriptthreads[i];
				scriptthreads.erase(scriptthreads.begin()+i);
				i++;
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			double deltawh = -GET_WHEEL_DELTA_WPARAM(wParam);
			if (isscrdwnon)
			{
				if (scrollfunc != nullptr)
					scrollfunc(boost::any_cast<int2>(DXclass->programdata->vmap["mouseposrelative"]->val), deltawh);
				break;
			}
			bool b1 = false;
			bool b = false;
			if (DXclass->maincam->scale.x > 1.0f && DXclass->maincam->scale.x + deltawh / 5000 < 1.0f)
			{
				DXclass->maincam->scale.x = 1.0f;
				b1 = true;
				goto scaleset;
			}
			if (DXclass->maincam->scale.x < 1.0f && DXclass->maincam->scale.x + deltawh / 5000 > 1.0f)
			{
				DXclass->maincam->scale.x = 1.0f;
				goto scaleset;
			}
			DXclass->maincam->scale.x += deltawh / 5000;
			if (DXclass->maincam->scale.x < 0.3f)
			{
				DXclass->maincam->scale.x = 0.3f;
			}
			else if (DXclass->maincam->scale.x > 3.0f)
			{
				DXclass->maincam->scale.x = 3.0f;
				b = true;
			}
			DXclass->maincam->scale.y = DXclass->maincam->scale.x;
		scaleset:;
			if ((DXclass->maincam->scale.x >= 1.0f || b1) && !b)
			{
				HRESULT hr = NULL;
				ID2D1BitmapRenderTarget** BRT = DXclass->maincam->GetRenderTargetP();
				ID2D1Bitmap* BMTRL = NULL;
				(*BRT)->GetBitmap(&BMTRL);
				BMTRL->Release();
				if (DXclass->maincam->bm != nullptr)
					DXclass->maincam->bm->Release();
				DXclass->maincam->bm = NULL;
				(*BRT)->Release();
				//*BRT = NULL;
				//ID2D1BitmapRenderTarget* BRTP = NULL;
				D2D1_SIZE_F drf = { clientsize.right,clientsize.bottom };
				drf.width *= DXclass->maincam->scale.x;
				drf.height *= DXclass->maincam->scale.y;
				//hwndRT->Resize({ (ui)drf.width,(ui)drf.height });
				int2 i2 = ((((uni2<float>)DXclass->con.lastmousepos*DXclass->maincam->scale - ((uni2<float>)drf)/2)) / 10).toint2();
				DXclass->maincam->SetXY(*DXclass->maincam->GetXYp()+i2);
				DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
				swapChainDesc.Width = drf.width;
				swapChainDesc.Height = drf.height;
				swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				swapChainDesc.Stereo = false;
				//swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 2;
				swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				swapChainDesc.Flags = 0;
				DXGISwapChain->Release();
				DXclass->dxgiBackBuffer->Release();
				hr = DXclass->dxgiFactory->CreateSwapChainForHwnd(Direct3DDevice, hwnd, &swapChainDesc, nullptr, nullptr, &DXGISwapChain);
				DXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&DXclass->dxgiBackBuffer));
				FLOAT dpiX, dpiY;
				pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);
				D2D1_BITMAP_PROPERTIES1 bitmapProperties =
					D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
						D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
				Direct2DBackBuffer->Release();
				hr = hwndRT->CreateBitmapFromDxgiSurface(DXclass->dxgiBackBuffer, &bitmapProperties, &Direct2DBackBuffer);
				hwndRT->SetTarget(Direct2DBackBuffer);
				if (DXclass->maincam->scale > DXclass->maincam->oldscale)
				{
					DXclass->maincam->SetOffset(((int2)drf / 2) * 1);
				}
				else
				{
					DXclass->maincam->SetOffset(-(((int2)drf / 2) * 1));
				}
				DXclass->maincam->oldscale = DXclass->maincam->scale;
				hwndRT->CreateCompatibleRenderTarget(drf, BRT);
				int bulllshitbreakpoint = 0;
			}
			else
			{
				D2D1_SIZE_F drf = { clientsize.right,clientsize.bottom };
				drf.width *= DXclass->maincam->scale.x;
				drf.height *= DXclass->maincam->scale.y;
				int2 i2 = int2(((uni2<float>)DXclass->con.lastmousepos*DXclass->maincam->scale - ((uni2<float>)drf) / 2).toint2()) / 10;
				DXclass->maincam->SetXY(*DXclass->maincam->GetXYp()-i2);
			}
				//DXclass->maincam->SetXY(*DXclass->maincam->GetXYp() + int2(((uni2<float>)DXclass->con.lastmousepos - ((uni2<float>) int2{ clientsize.right, clientsize.bottom }/2)
				//*DXclass->maincam->scale)*(uni2<float>) int2 { clientsize.right, clientsize.bottom }*DXclass->maincam->scale) / 10);
			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case ID_STARTBUTTON:
			{
				DX2D::init(style);
				wasstarted = true;
				DestroyWindow(hstart);
				break;
			}
			}
			break;
		}
		//case WM_CAPTURECHANGED:
		//case WM_KILLFOCUS:
		//{
		//	if (wasstarted && isfullscreened)
		//	{
		//		exitFullscreen(hwnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0);
		//		if (!isfscreentoud)
		//		{
		//			SetTimer(hwnd, fullscreentimeouttimer, fullscreentimeout, NULL);
		//			isfscreentoud = true;
		//		}
		//	}
		//	break;
		//}
		case WM_SYSKEYDOWN:
		{
			if (wParam == VK_RETURN)
			{
				if ((HIWORD(lParam) & KF_ALTDOWN))
				{
					if (iscurrentlyfscreened)
					{
						exitFullscreen(hwnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 30, 30);
						iscurrentlyfscreened = false;
					}
					else
					{
						HDC windowHDC = GetDC(hwnd);
						enterFullscreen(hwnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), GetDeviceCaps(windowHDC, BITSPIXEL), GetDeviceCaps(windowHDC, VREFRESH));
						ReleaseDC(hwnd, windowHDC);
						iscurrentlyfscreened = true;
					}
				}
				else
					return DefWindowProc(hwndwin, msg, wParam, lParam);
			}
			else
				return DefWindowProc(hwndwin, msg, wParam, lParam);
			break;
		}
		/*case WM_ACTIVATEAPP:
		{
		if (iscurrentlyfscreened)
		{
		exitFullscreen(hwnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 30, 30);
		iscurrentlyfscreened = false;
		}
		else
		{
		HDC windowHDC = GetDC(hwnd);
		enterFullscreen(hwnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), GetDeviceCaps(windowHDC, BITSPIXEL), GetDeviceCaps(windowHDC, VREFRESH));
		ReleaseDC(hwnd, windowHDC);
		iscurrentlyfscreened = true;
		}
		break;
		}*/
		case WM_SETFOCUS:
		{
			if (GetFocus() == hwnd)
				if (wasstarted && isfullscreened && iscurrentlyfscreened && !isfscreentoud)
				{
					HDC windowHDC = GetDC(hwnd);
					enterFullscreen(hwnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), GetDeviceCaps(windowHDC, BITSPIXEL), GetDeviceCaps(windowHDC, VREFRESH));
					ReleaseDC(hwnd, windowHDC);
				}
			break;
		}
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_MINIMIZE:
			{
				DXclass->pclass.pausethread();
				ShowWindow(hwnd, SW_HIDE);
				break;
			}
			case SC_MAXIMIZE:
			{
				DXclass->pclass.unpausethread();
				ShowWindow(hwnd, SW_SHOW);
				break;
			}
			case SC_CLOSE:
			{
				if (DXclass->pclass.ismultithreaded())
					DXclass->pclass.ReleaseTHR();
				wasstarted = false;
				DestroyWindow(hwnd);
				PostQuitMessage(0);
				return WM_QUIT;
				break;
			}
			}
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0L;
		default:
			return DefWindowProc(hwndwin, msg, wParam, lParam);
		}
	endofmp:;
	}

	void subbutton(int2& pos)
	{
		uintptr_t v = ((ulli)(((lli)pos.x) + INT_MAX)) << 32 | (ui)(((lli)pos.y) + INT_MAX);
		void* ptr = reinterpret_cast<void*>(v);
		button* btn = static_cast<button*>(ptr);
		AZfile menu = boost::any_cast<AZfile>(btn->anyvars[0]);
		string sublink = boost::any_cast<string>(btn->anyvars[1]);
		DXclass->con.delbutton(btn);
		int i = 0;
		vector<string> buttons;
		while (menu.findvar("MAINMENU@" + sublink + '@' + "BUTTON" + INTtoSTR(i)))
		{
			buttons.push_back(menu.GetVar<string>("MAINMENU@" + sublink + '@' + "BUTTON" + INTtoSTR(i)));
			i++;
		}
		i = 0;
		frame* mf = new frame;
		DXclass->maincam->scenes[0].objectsvec.push_back(mf);
		DXclass->maincam->scenes[0].objectsmap.insert(make_pair("blahblah", mf));
		while (i < buttons.size())
		{
			int2 bpos;
			bpos.x = DXclass->GetpoX(menu.GetVar<float>("MAINMENU@" + sublink + "@" + buttons[i] + +"@POSX"));
			bpos.y = DXclass->GetpoY(menu.GetVar<float>("MAINMENU@" + sublink + "@" + buttons[i] + "@POSY"));
			int2 bsize;
			bsize.x = DXclass->GetpoX(menu.GetVar<float>("MAINMENU@" + sublink + "@" + buttons[i] + "@SIZEX"));
			bsize.y = DXclass->GetpoY(menu.GetVar<float>("MAINMENU@" + sublink + "@" + buttons[i] + "@SIZEY"));
			ui ID = menu.GetVar<ui>("MAINMENU@" + sublink + "@" + buttons[i] + "@ID");
			string bname = menu.GetVar<string>("MAINMENU@" + sublink + "@" + buttons[i] + "@NAME");
			string btext = menu.GetVar<string>("MAINMENU@" + sublink + "@" + buttons[i] + "@TEXT");
			string fontname = menu.GetVar<string>("MAINMENU@" + sublink + "@" + buttons[i] + "@FONTNAME");
			float fontsize = menu.GetVar<float>("MAINMENU@" + sublink + "@" + buttons[i] + "@FONTSIZE");
			bool novis = menu.GetVar<bool>("MAINMENU@" + sublink + "@" + buttons[i] + "@NOVISUALS");
			if (!novis)
				DXclass->con.addbutton(btext, new int2(bpos), bsize, GAME::funcidmap[ID], mf, DXclass->maincam, false, DXclass->textc, fontsize, fontname);
			else
				DXclass->con.addbutton(btext, new int2(bpos), bsize, { new frame }, GAME::funcidmap[ID], 0, 0, 0,
					new frame, false, mf, DXclass->maincam, DXclass->textc, true, 0, bname, { 0,0 }, nullptr, fontsize, fontname);
			string start = menu.GetVar<string>("MAINMENU@" + sublink + "@" + buttons[i] + "@START");
			if (start != "NULL")
			{
				button* butt = DXclass->con.latestcreation;
				butt->anyvars.push_back(menu);
				butt->anyvars.push_back(start);
			}
			i++;
		}
		i = 0;
	}
}