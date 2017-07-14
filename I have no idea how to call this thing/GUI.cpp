#include "GUI.h"
namespace GAME
{
	namespace GUI
	{
		ui core::PreAllocateStyle(string name)
		{
			ui id = 0;
			do
			{
				id = rand();
			} while (MapFind( styles,id));
			styles.insert(make_pair(id, style()));
			styleids.insert(make_pair(name, id));
		}
		UIresult core::addstyle(style st, string name)
		{
			styles[styleids[name]] = st;
			UIresult ret;
			ret.code = UI_OK;
			return ret;
		}
		UIresult core::init(controls * con, camera * ncam, frame * mf, ui scenen)
		{
			conptr = con;
			mainframe = mf;
			scenenum = scenen;
			UIresult result;
			result.code = UI_OK;
			cam = ncam;
			return result;
		}
		void loadGUIdata(string binloc, core * c)
		{
			AZfile f;
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;
			string sPath = binloc + "data\\UI\\styles\\*";
			hFind = FindFirstFile(sPath.data(), &FindFileData);
			vector<string> files;
			do
			{
				//if (FindFileData.dwFileAttributes == 16)
				//{
					string fdstr = FindFileData.cFileName;
					files.push_back("data\\UI\\styles\\" + fdstr);
				//}
			} while (FindNextFile(hFind, &FindFileData));
			FindClose(hFind);
			int i = 0;
			while (i < files.size())
			{
				AZfile f(files[i]);
				c->createstyles(f,binloc, true);
				i++;
			}
		}
		UIresult core::createstyles(AZfile& f, string binloc, bool add)
		{
			int i = 0;
			string str = "ICONS@";
			auto iconlocs = f.GetVarNamesInNode(str);
			while (i < iconlocs.size())
			{
				string s = f.GetVar<string>(str + iconlocs[i]);
				string loc = common::findoneoffiles(
				{ binloc + "data\\UI\\icon\\" + s,binloc + "data\\UI\\icon\\" + s + ".png", binloc + "data\\UI\\icon\\" + s + ".ico", binloc +
					"data\\UI\\icon\\" + s + ".bmp", s, s + ".ico", s + ".png", s + ".bmp" });
				if (loc != "NULL"&&loc != "")
				{
					sprite sp;
					sp.SetBitmapFromFile(STRtoWSTR(loc).c_str(), *cam->GetRenderTargetP());
					icons.insert(make_pair(iconlocs[i], sp));
				}
				i++;
			}
			i = 0;
			str = "STYLES@S";
			string s;
			vector<string> names;
			while (f.GetVarSafe(s, str + common::INTtoSTR(i++)))
				names.push_back(s);
			i = 0;
			vector<pair<ui, string>> styleuinames;
			while (i < names.size())
			{
				styleuinames.push_back(make_pair(PreAllocateStyle(names[i]),names[i]));
				i++;
			}
			i = 0;
			str = "STYLES@";
			while (i < names.size())
			{
				style st;
				st.name = names[i];
				str = "STYLES@" + names[i] + "@";
				int ii = 0;
				auto structs = f.GetStructNamesInNode(str);
				map<string, common::RGBA*> cnodemap;
				while (ii < structs.size())
				{
					AZstruct* pn = f.GetStruct(str + structs[i]);
					if (pn->type == "COLNODE")
					{
						common::RGBA* cn = new common::RGBA;
						cn->r = pn->getvar<float>("R")*255.0f;
						cn->b = pn->getvar<float>("B")*255.0f;
						cn->g = pn->getvar<float>("G")*255.0f;
						cn->a = pn->getvar<float>("A");
						cnodemap.insert(make_pair(structs[i], cn));
					}
					ii++;
				}
				st.colnodemap = cnodemap;
				ii = 0;
				while (f.findstruct( str + "SHAPE" + common::INTtoSTR(ii)+"@SHAPE"))
				{
					AZstruct* sh = f.GetStruct(str + "SHAPE" + common::INTtoSTR(ii++) + "@SHAPE");
					simpleshape simsh;
					if (sh->type == "RECT")
					{
						simsh.shape = 0;
						simsh.size = int2(sh->getvar<int>("SIZEX"), sh->getvar<int>("SIZEY"));
					}
					else
					{
						simsh.shape = 1;
						simsh.radius = sh->getvar<float>("RAD");
					}
					simsh.pos = int2(sh->getvar<int>("POSX"), sh->getvar<int>("POSY"));
					string str2;
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@COLNODE"))
					{
						simsh.col = cnodemap[str];
					}
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@SECCOLNODE"))
					{
						simsh.seccol = cnodemap[str];
					}
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@ICON"))
					{
						simsh.ico = icons[str];
						simsh.hassprite = true;
					}
					simsh.id = ii;
					st.shapes.push_back(simsh);
					ii++;
				}
				ii = 0;
				st.file = f;
				while (f.findstruct(str + "BOX" + common::INTtoSTR(ii) + "@SHAPE"))
				{
					AZstruct* sh = f.GetStruct(str + "BOX" + common::INTtoSTR(ii++) + "@SHAPE");
					simpleshape simsh;
					if (sh->type == "RECT")
					{
						simsh.shape = 0;
						simsh.size = int2(sh->getvar<int>("SIZEX"), sh->getvar<int>("SIZEY"));
					}
					else
					{
						simsh.shape = 1;
						simsh.radius = sh->getvar<float>("RAD");
					}
					simsh.pos = int2(sh->getvar<int>("POSX"), sh->getvar<int>("POSY"));
					string str2;
					//if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@COLNODE"))
					//{
					//	simsh.col = cnodemap[str];
					//}
					//if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@SECCOLNODE"))
					//{
					//	simsh.seccol = cnodemap[str];
					//}
					//if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii++) + "@ICON"))
					//{
					//	simsh.ico = icons[str];
					//	simsh.hassprite = true;
					//}
					simsh.id = ii;
					box b;
					b.sh = simsh;
					vector<string>inputs = {"LBUTTONUP","RBUTTONUP","LBUTTONDOWN","RBUTTONDOWN","MBUTTONUP","MBUTTONDOWN","MOUSEMOVE"};
					int iii = 0;
					str2 = str + "BOX" + common::INTtoSTR(ii) + "@";
					while (iii < inputs.size())
					{
						b.rootnode.insert(make_pair(inputs[iii], new box::change));
						string str3;
						int iiii = 0;
						string rootstr = "";
						string getstr = "";
						while (f.GetVarSafe(str3,getstr = (rootstr = str2 + inputs[iii] + "@") + "SUB" + common::INTtoSTR(iiii)))
						{
							b.rootnode[inputs[iii]]->save(f, rootstr + str3,inputs[iii]);
							iiii++;
						}
						iii++;
					}
					ii++;
				}
				i++;
			}
		}
	}
}