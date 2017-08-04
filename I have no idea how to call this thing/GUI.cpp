#include "GUI.h"
namespace GAME
{
	extern int4 camrect;
	namespace GUI
	{
		namespace deffunc
		{
			map<string, void(*)(int2&)> list = {};
		}
		void loadui(string binloc, core*c)
		{
			string link = binloc + "\\data\\UI\\gameui.data";
			AZfile file(link);
			string str;
			vector < pair<string, window*>> wndparentv;
			auto nnmap = c->wndnamemap;
			int i = 0;
			while (file.GetVarSafe(str, "W" + INTtoSTR(i)))
			{
				string name = str;
				str = str + "@";
				auto res = ((file.GetVar<bool>(str + "W@PARENTPOSSIZE")?c->NewWindow(c->root, uni2<float>{file.GetVar<float>(str + "W@POSX" ),file.GetVar<float>(str + "W@POSY") },uni2<float>{ file.GetVar<float>(str + "W@SIZEX"), file.GetVar<float>(str + "W@SIZEY")}, file.GetVar<string>(str + "W@STYLE"), WF_NOVINIT | WF_HIDE | WF_SCALETO_V)
				: c->NewWindow(c->root, uni2<float>{ file.GetVar<float>(str + "W@POSX"),file.GetVar<float>(str + "W@POSY") }, uni2<float>{ file.GetVar<float>(str + "W@SIZEX"), file.GetVar<float>(str + "W@SIZEY")}, file.GetVar<string>(str + "W@STYLE"), WF_HIDE | WF_SCALETO_V)));
				if (res.code == UI_OK)
				{
					auto wnd = boost::any_cast<window*>(res.retval[0]);
					wndparentv.push_back(make_pair(file.GetVar<string>(str + "W@PARENT"), wnd));
					nnmap.insert(make_pair(name, boost::any_cast<ui>(res.retval[1])));
					wnd->size = uni2<float>(file.GetVar<float>(str + "W@SIZEX"), file.GetVar<float>(str + "W@SIZEY"));
				}
				else
				{
					//TODO: add error handling
				}
				i++;
			}
			i = 0;
			while (i < wndparentv.size())
			{
				c->AttachTo(c->wnds[nnmap[wndparentv[i].first]],wndparentv[i].second,AT_STYLEPROPPOS | AT_VINIT);
				wndparentv[i].second->show();
				i++;
			}
		}
		ui core::PreAllocateStyle(string name)
		{
			ui id = 0;
			do
			{
				id = rand();
			} while (MapFind( styles,id));
			styles.insert(make_pair(id, style()));
			styleids.insert(make_pair(name, id));
			return id;
		}
		UIresult core::addstyle(style st, string name)
		{
			styles[styleids[name]] = st;
			st.id = styleids[name];
			UIresult ret;
			ret.code = UI_OK;
			return ret;
		}
		UIresult core::AttachTo(window * parent, window * child, unsigned long int flags)
		{
			UIresult ret;
			window* oldparent = child->parent;
			if (parent == oldparent && flags & AT_VINIT)
			{
				child->initvis(styles[child->ID], child->defpos, GAME::camrect.second().touni2<float>());
				ret.code = UI_OK;
				return ret;
			}
			else if (parent == oldparent)
			{
				ret.code = UI_OK;
				return ret;
			}
			if (parent != nullptr)
			{
				child->parent->children.erase(child->ID);
				child->parent = parent;
				parent->children.insert(make_pair(child->ID, child));
				//child->updatepos();
				int2 oldpos = *child->pos;
				if (!(flags & AT_NULL))
				{
					if (flags & AT_VINIT)
					{
						if (flags & AT_PROPPOS)
						{
							uni2<float> psize = oldparent->size;
							auto ppos = oldparent->proppos;
							auto size = child->size;
							auto pos = child->proppos;
							auto wsize = GAME::camrect.second().touni2<float>();
							auto pospp = parent->pos->touni2<float>();
							auto poscp = child->pos->touni2<float>();
							auto multp = pospp / wsize;
							auto multc = poscp / wsize;
							auto realsize = parent->size / wsize;
							*child->pos = (parent->pos->touni2<float>() + (parent->size * multc)).toint2();
						}
						else if (flags & AT_SAMEPOS)
						{
							*child->pos = *child->pos - *parent->pos;
						}
						else if (flags & AT_STYLEPROPPOS)
						{
							uni2<float> psize = oldparent->size;
							auto ppos = oldparent->proppos;
							auto size = child->size;
							auto pos = child->proppos;
							auto wsize = GAME::camrect.second().touni2<float>();
							auto pospp = parent->pos->touni2<float>();
							auto poscp = child->pos->touni2<float>();
							auto multp = pospp / wsize;
							auto multc = child->defpos;
							auto realsize = parent->size / wsize;
							child->nooffpos = (parent->size * multc).toint2();
							*child->pos = (parent->pos->touni2<float>() + (parent->size * multc)).toint2();
						}
						child->initvis(styles[child->ID], child->defpos, GAME::camrect.second().touni2<float>());
					}
					else
					{
						if (flags & AT_PROPPOS)
						{

						}
					}
				}
				child->updatepos(oldpos);
				child->updatenesting();
			}
			else if (flags & AT_VINIT)
			{
				child->initvis(styles[child->ID], child->defpos, GAME::camrect.second().touni2<float>());
			}
			ret.code = UI_OK;
			return ret;
		}
		void GUI::window::updatenesting()
		{
			BOOST_FOREACH(auto ch, children)
			{
				ch.second->nestingc = nestingc + 1;
				ch.second->updatenesting();
			}
		}
		void window::updatepos(int2 oldpos)
		{
			if (children.size() == 0)
			{
				//window* w = parent;
				//vector<ui> ids;
				//int i = 0;
				//while (w != nullptr)
				//{
				//	ids.push_back(w->ID);
				//	w = w->parent;
				//}
				//reverse(ids.begin(), ids.end());
				//vector<int2> offsets;
				//while (i < ids.size())
				//{
				//	coreptr->wnds[ids[i]]->cufoffsets = offsets;
				//	offsets.push_back(coreptr->wnds[ids[i]]->nooffpos);
				//	int ii = 0;
				//	while (ii < posvec.size())
				//	{
				//		*posvec[ii] = *posvec[ii] + *pos + *w->pos;
				//		ii++;
				//	}
				//	i++;
				//}
				//i = 0;
				int ii = 0;
				while (ii < posvec.size())
				{
					*posvec[ii].first = posvec[ii].second + *pos + ((parent != nullptr)?*parent->pos:int2(0,0));
					ii++;
				}
			}
			else
			{
				int i = 0;
				while (i < children.size())
				{
					children[i]->updatepos(oldpos);
					i++;
				}
			}
		}
		UIresult window::initvis(style& s, uni2<float> npos, uni2<float> screenmultip)
		{
			UIresult ret;
			proppos = npos;
			//frame* f = new frame;
			styleid = s.id;
			int i = 0;
			pos = new int2((npos*screenmultip).toint2());
			i = 0;
			if (identp == nullptr)
			{
				identp = new bool(true);
			}
			*identp = true;
			while (i < s.shapes.size())
			{
				brush b;
				bool* rb = new bool(true);
				shaperb.push_back(rb);
				delete b.offset;
				b.offset = pos;
				if (s.shapes[i].hassprite)
				{
					sprite sp = s.shapes[i].ico;
					sp.SetOffsetXYp(pos, true);
					sp.render = rb;
					sp.useidentp = true;
					sp.identp = identp;
					sp.size = (s.shapes[i].size * screenmultip).toint2();
					int2* i2sp;
					sp.SyncPos(i2sp=new int2((s.shapes[i].pos * screenmultip).toint2()), false);
					f->sprites.push_back(sp);
					posvec.push_back(make_pair(i2sp,*i2sp));
				}
				b.useidentp = true;
				b.size = new int2((s.shapes[i].size * screenmultip).toint2());
				b.identp = identp;
				b.pos = new int2((s.shapes[i].pos * screenmultip).toint2());
				posvec.push_back(make_pair(b.pos,*b.pos));
				if (s.shapes[i].shape == 0)
				{
					b.size = new int2((s.shapes[i].size * screenmultip).toint2());
					b.createrectfrompossize = true;
				}
				else
					b.elipse.radiusX = b.elipse.radiusY = s.shapes[i].radius;
				//b.breakonrender = true;
				b.SetType(solidbrush);
				auto mcol = s.shapes[i].col->toColorF();
				(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].col->toColorF(), &b.b.solidbrush.first);
				if(s.shapes[i].seccol != nullptr)
					(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].seccol->toColorF(), &b.b.solidbrush.second);
				else
					(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].col->toColorF(), &b.b.solidbrush.second);
				b.renderp = rb;
				f->brushes.push_back(b);
				i++;
			}
			i = 0;
			con.init();
			con.GetPhysP()->offsetp = pos;
			while (i < s.boxes.size())
			{
				int2* pos = new int2((s.boxes[i].sh.pos * screenmultip).toint2());
				posvec.push_back(make_pair(pos,*pos));
				int2 size = int2((s.boxes[i].sh.size * screenmultip).toint2());
				simpleshape sh = s.boxes[i].sh;
				textclass tc;
				tc.init(); 
				con.addbutton("", pos, size, nullptr, f, coreptr->cam, false, tc);
				button* butt = con.latestcreation;
				butt->renderingoff();
				butt->anyvars.push_back(this);
				butt->anyvars.push_back(i);
				butt->callpfunc = true;
				btts.push_back(butt);
				i++;
			}
			if(s.boxes.size()  > 0)
				bt = btts[0];
			i = 0;
			while (i < s.subs.size())
			{
				window* wnd = new window;
				i++;
			}
			i = 0;
			updatepos({ 0,0 });
			ret.code = UI_OK;
			f->ismactive = true;
			return ret;
		}
		UIresult window::scale(int2 nsize)
		{
			return UIresult();
		}
		UIresult window::show()
		{
			int i = 0;
			hidden = false;
			while (i < shaperb.size())
			{
				*shaperb[i] = true;
				i++;
			}
			UIresult ret;
			ret.code = UI_OK;
			return ret;
		}
		UIresult window::hide()
		{
			int i = 0;
			hidden = true;
			while (i < shaperb.size())
			{
				*shaperb[i] = false;
				i++;
			}
			UIresult ret;
			ret.code = UI_OK;
			return ret;
		}
		UIresult window::switchvis()
		{
			UIresult ret;
			if (hidden)
				ret = show();
			else
				ret = hide();
			return ret;
		}
		UIresult core::NewWindow(window * parent, uni2<float> pos, uni2<float> size, ui styleid, unsigned long int flags)
		{
			UIresult ret;
			style s = styles[styleid];
			int i = 0;
			i = 0;
			window* w = new window;
			w->flags = flags;
			w->coreptr = this;
			do
			{
				w->ID = rand();
			} while (MapFind(wnds, w->ID));
			w->f = new frame;
			mainframe->f["main"].push_back(w->f);
			mainframe->wchiac = "main";
			mainframe->ischfact = true;
			uni2<float> mf;
			int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
			w->defpos = pos;
			if (flags & WF_SCALETO_VH)
			{
				mf.x = ((float)wsize.x)*size.x;
				mf.y = ((float)wsize.y)*size.y;
			}
			else if (flags & WF_SCALETO_V)
			{
				mf.x = ((float)wsize.x)*size.x;
				mf.y = ((float)wsize.y)*size.y;
			}
			else if (flags & WF_SCALETO_H)
			{
				mf.x = ((float)wsize.x)*size.x;
				mf.y = ((float)wsize.y)*size.x;
			}
			if (!(flags & WF_NOVINIT))
			{
				ret = w->initvis(s, pos, mf);
			}
			if (flags & WF_HIDE)
				w->hide();
			else
				w->show();
			ret.retval.push_back(w);
			ret.retval.push_back(w->ID);
			return ret;
		}
		UIresult core::NewWindow(window * parent, uni2<float> pos, uni2<float> size, string stylename, unsigned long int flags)
		{
			return NewWindow(parent, pos, size, styleids[stylename], flags);
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
		UIresult core::addstyle(style st)
		{
			ui id = 0;
			do
			{
				id = rand();
			} while (MapFind(styles, id));
			st.id = id;
			styles.insert(make_pair(id, st));
			UIresult ret;
			ret.code = UI_OK;
			return ret;
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
					files.push_back(binloc + "data\\UI\\styles\\" + fdstr);
				//}
			} while (FindNextFile(hFind, &FindFileData));
			FindClose(hFind);
			int i = 0;
			while (i < files.size())
			{
				if (files[i][files[i].size()-1] == '.')
				{
					i++;
					continue;
				}
				AZfile f(files[i]);
				c->createstyles(f,binloc, true);
				i++;
			}
		}
		UIresult core::createstyles(AZfile& f, string binloc, bool add)
		{
			int i = 0;
			UIresult ret;
			string str = "ICONS@";
			auto iconlocs = f.GetVarNamesInNode("ICONS");
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
					AZstruct* pn = f.GetStruct(str + structs[ii]);
					if (pn->type == "COLNODE")
					{
						common::RGBA* cn = new common::RGBA;
						cn->r = pn->getvar<float>("R")*255.0f;
						cn->b = pn->getvar<float>("B")*255.0f;
						cn->g = pn->getvar<float>("G")*255.0f;
						cn->a = pn->getvar<float>("A");
						cnodemap.insert(make_pair(structs[ii], cn));
					}
					ii++;
				}
				st.colnodemap = cnodemap;
				ii = 0;
				while (f.findstruct( str + "SHAPE" + common::INTtoSTR(ii)+"@SHAPE"))
				{
					AZstruct* sh = f.GetStruct(str + "SHAPE" + common::INTtoSTR(ii) + "@SHAPE");
					simpleshape simsh;
					if (sh->type == "RECT")
					{
						simsh.shape = 0;
						simsh.size = uni2<float>(sh->getvar<float>("SIZEX"), sh->getvar<float>("SIZEY"));
					}
					else
					{
						simsh.shape = 1;
						simsh.radius = sh->getvar<float>("RAD");
					}
					simsh.pos = uni2<float>(sh->getvar<float>("POSX"), sh->getvar<float>("POSY"));
					string str2;
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii) + "@COLNODE"))
					{
						simsh.col = cnodemap[str2];
					}
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii) + "@SECCOLNODE"))
					{
						simsh.seccol = cnodemap[str2];
					}
					if (f.GetVarSafe(str2, str + "SHAPE" + common::INTtoSTR(ii) + "@ICON"))
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
					AZstruct* sh = f.GetStruct(str + "BOX" + common::INTtoSTR(ii) + "@SHAPE");
					simpleshape simsh;
					if (sh->type == "RECT")
					{
						simsh.shape = 0;
						simsh.size = uni2<float>(sh->getvar<float>("SIZEX"), sh->getvar<float>("SIZEY"));
					}
					else
					{
						simsh.shape = 1;
						simsh.radius = sh->getvar<float>("RAD");
					}
					simsh.pos = uni2<float>(sh->getvar<float>("POSX"), sh->getvar<float>("POSY"));
					string str2;
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
					st.boxes.push_back(b);
					ii++;
				}
				ii = 0;
				while (f.findstruct(str + "TEXTBOX" + common::INTtoSTR(ii) + "@SHAPE"))
				{
					AZstruct* sh = f.GetStruct(str + "TEXTBOX" + common::INTtoSTR(ii) + "@SHAPE");
					uni2<float> pos;
					uni2<float> size;
					if (sh->type == "RECT")
					{
						
						size = uni2<float>(sh->getvar<float>("SIZEX"), sh->getvar<float>("SIZEY"));
					}
					else
					{
						DebugBreak();
					}
					pos = uni2<float>(sh->getvar<float>("POSX"), sh->getvar<float>("POSY"));
					textpiece* tp = new textpiece;
					tp->pos = new int2 ((pos*100).toint2());
					tp->size = (size*100).toint2();
					tp->changetext(f.GetVar<string>(str + "TEXTBOX" + common::INTtoSTR(ii) + "@DEFTEXT"));
					st.textboxes.push_back(tp);
					ii++;
				}
				ii = 0;
				while (f.findstruct(str + "WINDOW" + common::INTtoSTR(ii) + "@SHAPE"))
				{
					AZstruct* sh = f.GetStruct(str + "WINDOW" + common::INTtoSTR(ii) + "@SHAPE");
					uni2<float> pos;
					uni2<float> size;
					if (sh->type == "RECT")
					{
						size = uni2<float>(sh->getvar<float>("SIZEX"), sh->getvar<float>("SIZEY"));
					}
					else
					{
						DebugBreak();
					}
					pos = uni2<float>(sh->getvar<float>("POSX"), sh->getvar<float>("POSY"));
					subwindow sw;
					sw.pos = pos;
					sw.sizemp = size;
					sw.sharedid = (f.GetVar<bool>(str + "WINDOW" + common::INTtoSTR(ii) + "@SHAREDID"));
					st.subs.push_back(sw);
					ii++;
				}
				addstyle(st,names[i]);
				i++;
			}
			ret.code = UI_OK;
			return ret;
		}
	}
}