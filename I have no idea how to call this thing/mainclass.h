#pragma once
#include "hmain.h"
#include "camera.h"
#include "sprite.h"
#include "physics.h"
#include "text.hpp"
#include "ships.h"
#include "universe.h"
#include "button.h"
#include "text.hpp"
//using namespace std;
//using namespace GAME;
//using namespace AZfilelib;
//using namespace DX2D;
//using namespace D2D1;
//extern bool wasstarted;
//extern bool pausemainth;
//extern bool ismainthpaused;
//extern bool isfullscreened;
//extern HWND hwnd;
//extern const float SCREEN_DEPTH;
//extern mainwins style;
//extern HWND hstart;
//extern main* DXclass = nullptr;
//extern map<string, shiptemp> shiptemps;
//extern bool isppaused = false;
//extern bool isfscreentoud = false;
//extern bool iscurrentlyfscreened = false;
//extern int score = 0;
//extern bool callfunc = false;
//extern int powupcreated = 0;
//extern int difficulty = 0;
//extern frame* backgroundframe;
//extern frame* pf = nullptr;
//extern vector<frame*> ssfvec; // shield storage vec;
//extern int projinfly = 0;
//extern int curprojnum = 0;
//extern vector <clock_t> projectilefiredate;
//zvector<pair<int, string>>projectilevec;
//vector <ship> ships;
//vector<camera*>*shipcamvec;
//vector <bool*> projrenbp;
////
//void initships();
////engine-specific ( mostly ) 
//struct physreactonset;
//ID2D1HwndRenderTarget *hwndRT = nullptr;
//ID2D1Factory* pD2DFactory = nullptr;
//
//void(*fformaintocall)(physobj*, physobj*);
//physobj* objtc1;
//physobj* objtc2;
//extern const float SCREEN_NEAR;namespace DX2D
//{
//
//	class main
//	{
//		friend inline void DX2D::Frame();
//	private:
//		ID2D1RenderTarget *RTp;
//		void Render(camera* cam)
//		{
//			/*ID2D1RenderTarget **//*RTp = *cam->GetRenderTargetP();*/
//			ID2D1BitmapRenderTarget *RT = *cam->GetRenderTargetP();
//			//RTp->CreateCompatibleRenderTarget(&RT);
//			RT->BeginDraw();
//			RT->Clear();
//			D2D_MATRIX_3X2_F oldtransform;
//			RT->GetTransform(&oldtransform);
//			D2D1_POINT_2F center = { RT->GetSize().width,RT->GetSize().height };
//			//D2D1::Matrix3x2F rot = D2D1::Matrix3x2F::Rotation( cam->GetRot(), center);
//			//D2D1::Matrix3x2F trans = Matrix3x2F::Translation(cam->GetX(), cam->GetY());		
//			RT->SetTransform(Matrix3x2F::Translation(cam->GetX(), cam->GetY())*D2D1::Matrix3x2F::Rotation(cam->GetRot(), center));
//			int s = 0;
//			int i = 0;
//			int ii = 0;
//			int iii = 0;
//			while (s < cam->scenes.size())
//			{
//				renderframevec(cam->scenes[s].objectsvec, cam, RT);
//				s++;
//			}
//			textc.RenderCam(cam);
//			HRESULT hr = RT->EndDraw();
//			RT->GetBitmap(&cam->bm);
//			RT->SetTransform(oldtransform);
//			//RT->Release();
//			//RTp->Release();
//			int fooshit = -1;
//		}
//	public:
//		map<string, module*> modtemps;
//		map<string, AZfile> modules;
//		controls con;
//		textclass textc;
//		string apploc = "C:\\";
//		wstring GetLinkToEl(string elemloc)
//		{
//			string str = apploc + elemloc;
//			return STRtoWSTR(str);
//		}
//		string GetLinkToElSTR(string elemloc)
//		{
//			return apploc + elemloc;
//		}
//		physics pclass;
//		ID2D1HwndRenderTarget* RenderTarget = nullptr;
//		camera* maincam;
//		int GetpoX(float perc)
//		{
//			RECT r;
//			GetWindowRect(hwnd, &r);
//			return (r.right - r.left) * (perc / 100);
//		}
//		int GetpoY(float perc)
//		{
//			RECT r;
//			GetWindowRect(hwnd, &r);
//			return (r.bottom - r.top) * (perc / 100);
//		}
//		int2 GetWindowSize()
//		{
//			RECT r;
//			GetWindowRect(hwnd, &r);
//			return{ (r.left - r.right),(r.bottom - r.top) };
//		}
//		vector <camera*> cams;
//		void renderframevec(vector<frame*>f, camera* cam, ID2D1BitmapRenderTarget *RT)
//		{
//			int i = 0;
//			D2D_MATRIX_3X2_F oldtransform;
//			RT->GetTransform(&oldtransform);
//			D2D_SIZE_F SZ = RT->GetSize();
//			//if (cam->usescale)
//			//{
//			//	RT->SetTransform(Matrix3x2F::Translation(cam->GetX(), cam->GetY())*D2D1::Matrix3x2F::Rotation(cam->GetRot(), { SZ.width / 2, SZ.height / 2 })*
//			//		Matrix3x2F::Scale({ cam->scale.x, cam->scale.y
//			//	}, cam->scalepoint));
//			//}
//			//else
//			RT->SetTransform(Matrix3x2F::Rotation(cam->GetRot(), { SZ.width / 2,SZ.height / 2 }));
//			while (i < f.size())
//			{
//				if (f[i]->ismactive)
//				{
//					//render
//					int ii = 0;
//					while (ii < f[i]->lines.size())
//					{
//						if (f[i]->lines[ii].renderp != nullptr)
//							if (!*f[i]->lines[ii].renderp)
//							{
//								ii++;
//								continue;
//							}
//						RT->DrawLine(f[i]->lines[ii].p1, f[i]->lines[ii].p2, f[i]->lines[ii].color);
//						ii++;
//					}
//					ii = 0;
//					while (ii < f[i]->sprites.size())
//					{
//						if (f[i]->sprites[ii].render != nullptr)
//							if (!*f[i]->sprites[ii].render)
//							{
//								ii++;
//								continue;
//							}
//						if (f[i]->sprites[ii].useidentp)
//						{
//							if (f[i]->sprites[ii].identp == nullptr)
//							{
//								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
//								continue;
//							}
//							else if (*f[i]->sprites[ii].identp == NULL)
//							{
//								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
//								continue;
//							}
//							else if (f[i]->sprites[ii].identp == false)
//							{
//								f[i]->sprites.erase(f[i]->sprites.begin() + ii);
//								continue;
//							}
//						}
//						D2D_RECT_F loc;
//						loc.top = (f[i]->sprites[ii].GetY() + f[i]->sprites[ii].GetOffSetp()->y);
//						loc.left = (f[i]->sprites[ii].GetX() + f[i]->sprites[ii].GetOffSetp()->x);
//						loc.bottom = (f[i]->sprites[ii].GetY() + f[i]->sprites[ii].size.height + f[i]->sprites[ii].GetOffSetp()->y)/**cam->scale.y*/;
//						loc.right = (f[i]->sprites[ii].GetX() + f[i]->sprites[ii].size.width + f[i]->sprites[ii].GetOffSetp()->x)/**cam->scale.x*/;
//						D2D_MATRIX_3X2_F oldtransform2;
//						RT->GetTransform(&oldtransform2);
//						D2D_POINT_2F p = { (f[i]->sprites[ii].GetX() + f[i]->sprites[ii].GetOffSetp()->x + (f[i]->sprites[ii].size.width / 2))/**cam->scale.x*/,
//							(f[i]->sprites[ii].GetY() + f[i]->sprites[ii].GetOffSetp()->y + (f[i]->sprites[ii].size.height / 2))/**cam->scale.y*/ };
//						RT->SetTransform(Matrix3x2F::Rotation(f[i]->sprites[ii].GetRot(), p));
//						RT->DrawBitmap(f[i]->sprites[ii].GetBitmap(), loc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
//						RT->SetTransform(oldtransform2);
//						ii++;
//					}
//					ii = 0;
//					while (ii < f[i]->freepixels.size())
//					{
//						const D2D1_ELLIPSE d2el = { CD2DPointF(f[i]->freepixels[ii].X, f[i]->freepixels[ii].Y) , 0, 0 };
//						//d2el.point = CD2DPointF(f[i]->freepixels[ii].X, f[i]->freepixels[ii].Y);
//						//d2el.radiusX = 1;
//						//d2el.radiusY = 1;
//						RT->DrawEllipse(&d2el, f[i]->freepixels[ii].color);
//						ii++;
//					}
//					ii = 0;
//					while (ii < f[i]->brushes.size())
//					{
//						if (f[i]->brushes[ii].useidentp)
//						{
//							if (f[i]->brushes[ii].identp == nullptr)
//							{
//								if (f[i]->brushes[ii].renderp == nullptr)
//									f[i]->brushes[ii].renderp = new bool(false);
//								else
//									*f[i]->brushes[ii].renderp = (false);
//								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
//								continue;
//							}
//							else if (*f[i]->brushes[ii].identp == false)
//							{
//								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
//								continue;
//							}
//							else if (*f[i]->brushes[ii].identp == NULL)
//							{
//								//f[i]->brushes.erase(f[i]->brushes.begin() + ii);
//								continue;
//							}
//						}
//						if (f[i]->brushes[ii].renderp != nullptr)
//						{
//							if (!*f[i]->brushes[ii].renderp)
//							{
//								ii++;
//								continue;
//							}
//						}
//						if (f[i]->brushes[ii].createrectfrompossize)
//						{
//							f[i]->brushes[ii].rect.left = f[i]->brushes[ii].pos->x;
//							f[i]->brushes[ii].rect.right = f[i]->brushes[ii].pos->x + f[i]->brushes[ii].size->x;
//							f[i]->brushes[ii].rect.top = f[i]->brushes[ii].pos->y;
//							f[i]->brushes[ii].rect.bottom = f[i]->brushes[ii].pos->y + f[i]->brushes[ii].size->y;
//						}
//						switch (f[i]->brushes[ii].brushtype)
//						{
//						case brushtypes::bitmapbrush:
//						{
//							if (f[i]->brushes[ii].rot != nullptr && *f[i]->brushes[ii].rot != 0)
//							{
//								RT->SetTransform(Matrix3x2F::Rotation(*f[i]->brushes[ii].rot));
//								RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.bitmapbrush);
//								RT->SetTransform(oldtransform);
//							}
//							else
//								RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.bitmapbrush);
//							break;
//						}
//						case brushtypes::lineargradient:
//						{
//							RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.lineargradient);
//							break;
//						}
//						case brushtypes::solidbrush:
//						{
//							RT->FillRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.solidbrush.first);
//							RT->DrawRectangle(f[i]->brushes[ii].rect, f[i]->brushes[ii].b.solidbrush.second);
//							break;
//						}
//						case brushtypes::radialgradient:
//						{
//							RT->FillEllipse(f[i]->brushes[ii].elipse, f[i]->brushes[ii].b.radialgradient.first);
//							RT->DrawEllipse(f[i]->brushes[ii].elipse, f[i]->brushes[ii].b.radialgradient.second);
//							break;
//						}
//						}
//						ii++;
//					}
//				}
//				if (f[i]->ischfact)
//				{
//					renderframevec(f[i]->f[f[i]->wchiac], cam, RT);
//				}
//				i++;
//			}
//			RT->SetTransform(oldtransform);
//		}
//		void init(mainwins style)
//		{
//			int i = 0;
//#if defined(DEBUG) || defined(_DEBUG)
//			D2D1_FACTORY_OPTIONS options;
//			options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
//
//			HRESULT hr = D2D1CreateFactory(
//				D2D1_FACTORY_TYPE_SINGLE_THREADED,
//				options,
//				&pD2DFactory
//			);
//#else
//			HRESULT hr = D2D1CreateFactory(
//				D2D1_FACTORY_TYPE_SINGLE_THREADED,
//				&pD2DFactory
//			);
//#endif
//			RECT rc;
//			GetClientRect(hwnd, &rc);
//
//			// Create a Direct2D render target	
//			hr = pD2DFactory->CreateHwndRenderTarget(
//				D2D1::RenderTargetProperties(),
//				D2D1::HwndRenderTargetProperties(
//					hwnd,
//					D2D1::SizeU(
//						rc.right - rc.left,
//						rc.bottom - rc.top)
//				),
//				&RenderTarget
//			);
//			D2D1_RENDER_TARGET_PROPERTIES rtpnc = D2D1::RenderTargetProperties(
//				D2D1_RENDER_TARGET_TYPE_DEFAULT,
//				D2D1::PixelFormat(
//					DXGI_FORMAT_UNKNOWN,
//					D2D1_ALPHA_MODE_IGNORE),
//				0,
//				0,
//				D2D1_RENDER_TARGET_USAGE_NONE,
//				D2D1_FEATURE_LEVEL_DEFAULT
//			);
//			const D2D1_RENDER_TARGET_PROPERTIES rtp = rtpnc;
//			hwndRT = RenderTarget;
//			maincam = new camera;
//			if (!SUCCEEDED(hr))
//			{
//				DebugBreak();
//			}
//			if (!SUCCEEDED(hr))
//			{
//				DebugBreak();
//			}
//			con.init();
//			apploc = style.programloc;
//			maincam->reconstruct();
//			int2 acs = { clientsize.right - clientsize.left,clientsize.bottom - clientsize.left };
//			scene mainscene;
//			frame* mf = new frame;
//			mf->ismactive = false;
//			mf->ischfact = true;
//			mf->wchiac = "mainframe";
//			frame * f = new frame;
//			GAME::init(apploc);
//			mf->f.insert(make_pair("mainframe", vector<frame*>({ f })));
//			mainscene.objectsmap.insert(make_pair("mainframe", mf));
//			mainscene.objectsvec.push_back(mf);
//			//camera* scam = new camera;
//			//scam->scenes.push_back(mainscene);
//			//cams.push_back(scam);
//			shipcamvec = &cams;
//			maincam->usescale = true;
//			maincam->SetScalePoint(maincam->GetBitmapSize() / 2);
//			maincam->scenes.push_back(mainscene);
//			GAME::funcidmap.insert(make_pair(666, initgame));
//			GAME::funcidmap.insert(make_pair(105, move));
//			bslink = apploc;
//			AZfile menu;
//			menu.loadfile(GetLinkToElSTR("bin\\data\\menu.file"));
//			vector<string>buttons;
//			i = 0;
//			if (menu.findvar("MAINMENU@BUTTON" + INTtoSTR(i)))
//			{
//				buttons.push_back(menu.GetVar<string>("MAINMENU@BUTTON" + INTtoSTR(i)));
//				i++;
//			}
//			i = 0;
//			while (i < buttons.size())
//			{
//				int2 bpos;
//				bpos.x = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + +"@POSX"));
//				bpos.y = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@POSY"));
//				int2 bsize;
//				bsize.x = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@SIZEX"));
//				bsize.y = GetpoX(menu.GetVar<float>("MAINMENU@" + buttons[i] + "@SIZEY"));
//				ui ID = menu.GetVar<ui>("MAINMENU@" + buttons[i] + "@ID");
//				string bname = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@NAME");
//				string btext = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@TEXT");
//				string fontname = menu.GetVar<string>("MAINMENU@" + buttons[i] + "@FONTNAME");
//				float fontsize = menu.GetVar<float>("MAINMENU@" + buttons[i] + "@FONTSIZE");
//				bool novis = menu.GetVar<bool>("MAINMENU@" + buttons[i] + "@NOVISUALS");
//				if (!novis)
//					con.addbutton(btext, new int2(bpos), bsize, GAME::funcidmap[ID], mf, maincam, false, textc, fontsize, fontname);
//				else
//					con.addbutton(btext, new int2(bpos), bsize, { new frame }, GAME::funcidmap[ID], 0, 0, 0,
//						new frame, false, mf, maincam, textc, true, 0, "", { 0,0 }, nullptr, fontsize, fontname);
//				i++;
//			}
//		}
//		void Release()
//		{
//			ID2D1BitmapRenderTarget *RT;
//			RT = *maincam->GetRenderTargetP();
//			RT->Release();
//			int i = 0;
//			while (i < cams.size())
//			{
//				RT = *cams[i]->GetRenderTargetP();
//				if (RT != nullptr)
//					RT->Release();
//				i++;
//			}
//			if (RTp != nullptr)
//				RTp->Release();
//			if (pD2DFactory != nullptr)
//				pD2DFactory->Release();
//			pclass.Release();
//		}
//		~main()
//		{
//
//		}
//	};
//}