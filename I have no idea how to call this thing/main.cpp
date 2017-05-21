#pragma once
//#include "hmain.h"
//#include "universe.h"
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
using namespace AZfilelib;
namespace GAME
{
	string version = "null";
	ui build = 0;
	universe* uniclass = nullptr;
}
inline std::string __INTtoSTR(int i)
{
	return std::to_string(i);
}
#undef INTtoSTR
#define INTtoSTR __INTtoSTR
long long int _STRtoINT(string STR)
{
	long long int i;
	i = stoll(STR);
	return i;
}
#define STRtoINT _STRtoINT
HWND hwnd;
extern HWND hlocationtbox;
bool pausemainth = false;
bool ismainthpaused = false;
extern RECT clientsize;
const float SCREEN_DEPTH = 1000.0f;
HWND hstart;
const float SCREEN_NEAR = 0.1f;
bool wasstarted = false;
//extern DX::DXSETTINGS settings;
mainwins style;
bool isfullscreened = false;
LRESULT CALLBACK MainWinProc(HWND hwndwin, UINT msg, WPARAM wParam, LPARAM lParam);
int main(int argc, char *argv[])
{
	bool ispoped = true;
	bool isfullscreendefault = false;
	style.programloc = argv[0];
	style.programloc = reverseSTR(copyfromchtoend(reverseSTR(style.programloc), '\\', true));
	int i = 0;
	AZfile vfile(style.programloc+"bin\\version.file");
	GAME::version = vfile.GetVar<string>("version");
	GAME::build = vfile.GetVar<ui>("build");
	string lbdate = vfile.GetVar<string>("date");
	string lbtime = vfile.GetVar<string>("time");
	if (lbdate != __DATE__ || lbtime != __TIME__)
	{
		vfile.SetVar("STR", vfile.GetFromNodes("date"), __DATE__);
		vfile.SetVar("STR", vfile.GetFromNodes("time"), __TIME__);
		vfile.SetVar("UINT", vfile.GetFromNodes("build"), to_string((int)++GAME::build));
		int ii = 0;
		ui vui[4];
		while (i < 4)
		{
			string strbuf;
			while (ii < version.length() && version[ii] != '.')
			{
				strbuf += version[ii];
				ii++;
			}
			ii++;
			vui[i] = STRtoINT(strbuf);
			i++;
		}
		version = "";
		vui[3]++;
		if (vui[3] >= 100)
		{
			vui[3] = 0;
			vui[2]++;
		}
		if (vui[2] >= 10)
		{
			vui[2] = 0;
			vui[1]++;
		}
		if (vui[1] >= 10)
		{
			vui[1] = 0;
			vui[0]++;
		}
		i = 0;
		while (i < 4)
		{
			version += INTtoSTR((int)vui[i]) + ".";
			i++;
		}
		version.erase(version.length() - 1);
		vfile.SetVar("STR", vfile.GetFromNodes("version"), version);
		vfile.SaveToFile(style.programloc + "bin\\version.file");
	}
	i = 0;
	while (i < argc)
	{
		string s = argv[i];
		style.args.push_back(s);
		i++;
	}
	style.argc = argc;
	i = 0;
	const char* const myclass = "myclass";
	WNDCLASSEXA wndclass = { sizeof(WNDCLASSEX), style.wcstyle, NULL,
		0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
		LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
		0, myclass, LoadIcon(0,IDI_APPLICATION) };
	wndclass.lpfnWndProc = MainWinProc;
	style.pozX = 0;
	style.pozY = 0;
	if (RegisterClassEx(&wndclass))
	{
		hwnd = CreateWindowExA(0, myclass, style.name,
			style.dwstyle, style.pozX, style.pozY,
			1920, 1080, 0, 0, GetModuleHandle(0), 0);
	}
	HDC windowHDC = GetDC(hwnd);
	RECT r;
	GetWindowRect(hwnd, &r);
	clientsize = r;
	if (ispoped)
	{
		ShowWindow(hwnd, SW_SHOW);
	}
	else
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	if (isfullscreendefault)
	{
		enterFullscreen(hwnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), GetDeviceCaps(windowHDC, BITSPIXEL), GetDeviceCaps(windowHDC, VREFRESH));
		isfullscreened = true;
	}
	ReleaseDC(hwnd, windowHDC);
	MSG msg;
	string vs = version;
	ui bu = build;

	do
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (wasstarted)
		{
			DX2D::Frame();
			if (pausemainth)
			{
				ismainthpaused = true;
				while (pausemainth)
					;
				ismainthpaused = false;
			}
		}
		else
		{
			DX2D::init(style);
			wasstarted = true;
		}
	} while (msg.message != WM_QUIT);
	DX2D::Release();
	DebugSetProcessKillOnExit(true);
	return 0;
}
LRESULT CALLBACK MainWinProc(HWND hwndwin, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DX2D::EmulateLEwinproc(hwndwin, msg, wParam, lParam);
}
#undef INTtoSTR
#undef STRtoINT