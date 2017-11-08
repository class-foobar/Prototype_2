#include "GUI.h"
#define corruptioncheck 0
mutex _py_refmutex;
#define Py_INCREFTS(obj) _py_refmutex.lock(); Py_INCREF(obj); _py_refmutex.unlock()
#define Py_DECREFTS(obj) _py_refmutex.lock(); Py_DECREF(obj); _py_refmutex.unlock()
#define PY_INCREFTS Py_INCREFTS
#define PY_DECREFTS Py_DECREFTS
namespace GAME
{
	extern mutex detachUIthmutex;
	extern int4 camrect;
	extern GUI::core* UI;
	extern vector<pair<thread*, bool*>>scriptthreads;
	extern deque<pycall*> calls;
	extern mutex scriptthreadmutex;
	namespace GUI
	{
		mutex GUIthmutex;
		thread* curth;
		map<thread*, bool*> scmap;
		namespace Python
		{
			namespace GUIMOD
			{
				vecbreaker <PyObject*(*)(PyObject*, PyObject*)> vb;
				//----------------------------------------------------------------------------------------------------------------------------
				//MODULE: GUI 
				PyObject* hError = NULL;
				PYFUNC(NewWindow)
				{
					ui flags, parentid;
					uni2<float> pos, size;
					char* chstylename = NULL, *chthisname = NULL;
					auto argn = PyTuple_Size(args);
					string thisname = "NULL";
					if (argn == 7)
					{
						PyArg_ParseTuple(args, "I|s|f|f|f|f|I", &parentid, &chstylename, &pos.x, &pos.y, &size.x, &size.y, &flags);
						thisname = "NULL";
					}
					else if(argn == 8)
					{
						PyArg_ParseTuple(args, "I|s|f|f|f|f|I|s", &parentid, &chstylename, &pos.x, &pos.y, &size.x, &size.y, &flags, &chthisname);
						if (chthisname != NULL)
							thisname = chthisname;
					}
					else if (argn == 1)
					{
						auto tuple = PyTuple_GetItem(args, 0);
						PY_INCREFTS(tuple);
						PyArg_ParseTuple(tuple, "I|s|f|f|f|f|I|s", &parentid, &chstylename, &pos.x, &pos.y, &size.x, &size.y, &flags, &chthisname);
						PY_DECREFTS(tuple);
						if (chthisname != NULL)
							thisname = chthisname;
					}
					else
					{
						return NULL;
					}
					ui flagscopy = WF_SCALETO_V;
					string stylename = chstylename;
					auto res = UI->NewWindow(UI->wnds[parentid], pos, size, stylename, (ui)flags);
					auto wnd = boost::any_cast<window*>(res.retval[0]);
					wnd->strname = thisname;
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					return PyLong_FromUnsignedLong(wnd->ID);
				}
				PYFUNC(HideWindow)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					UI->wnds[ID]->hide();
					return PyBool_FromLong(0);
				}
				PYFUNC(ShowWindow)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					UI->wnds[ID]->show();
					return PyBool_FromLong(0);
				}
				PYFUNC(SwitchWinVis)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					UI->wnds[ID]->switchvis();
					return PyBool_FromLong(0);
				}
				PYFUNC(GetX)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromLong(UI->wnds[ID]->pos->x);
				}
				PYFUNC(GetY)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromLong(UI->wnds[ID]->pos->y);

				}
				PYFUNC(SetX)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					UI->wnds[ID]->pos->x = n;
					return PyBool_FromLong(0);
				}
				PYFUNC(SetY)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					UI->wnds[ID]->pos->y = n;
					return PyBool_FromLong(0);
				}
				PYFUNC(SetXu)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					//int2 old = *UI->wnds[ID]->pos;
					UI->wnds[ID]->pos->x = n;
					UI->wnds[ID]->updatepos();
					return PyBool_FromLong(0);

				}
				PYFUNC(SetYu)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					//int2 old = *UI->wnds[ID]->pos;
					UI->wnds[ID]->pos->y = n;
					UI->wnds[ID]->updatepos();
					return PyBool_FromLong(0);
				}
				PYFUNC(SetSX)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					UI->wnds[ID]->size.x = n;
					return PyBool_FromLong(0);
				}
				PYFUNC(SetSY)
				{
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i", &ID, &n);
					UI->wnds[ID]->size.y = n;
					return PyBool_FromLong(0);
				}
				PYFUNC(GetSX)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromLong(UI->wnds[ID]->size.x);
				}
				PYFUNC(GetSY)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromLong(UI->wnds[ID]->size.y);
				}
				PYFUNC(SetCol)
				{
					ui ID;
					string node, nnode;
					char *ch0 = NULL, *ch1 = NULL;
					bool switcharound = false;
					PyArg_ParseTuple(args, "I|s|s|b", &ID, &ch0, &ch1, &switcharound);
					node = ch0;
					nnode = ch1;
					style s = UI->styles[UI->wnds[ID]->styleid];
					auto oldptr = *UI->wnds[ID]->colnodes[node];
					auto ptr = *UI->wnds[ID]->colnodes[node] = *UI->wnds[ID]->colnodes[nnode];
					if (switcharound == true)
						*UI->wnds[ID]->colnodes[nnode] = oldptr;
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					return PyBool_FromLong(0);
				}
				PYFUNC(GetCol)
				{
					string name;
					char* ch0 = NULL;
					ui ID;
					PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					name = ch0;
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					return PyLong_FromVoidPtr(UI->wnds[ID]->colnodes[name]);
				}
				PYFUNC(AddColPtr)
				{
					ui ID;
					ulli ptr0, ptr1;
					string name;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|K|s", &ID, &ptr0, &ch0);
					name = ch0;
					auto c0 = static_cast<pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>*>((void*)ptr0);
					UI->wnds[ID]->colnodes.insert(make_pair(name, new pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>(*c0)));
					return PyLong_FromLong(0);
				}
				PYFUNC(CreateColFromRGB)
				{
					ulli ptr0, ptr1;
					PyArg_ParseTuple(args, "K|K", &ptr0, &ptr1);
					common::RGBA* col0 = static_cast<common::RGBA*>((void*)ptr0);
					common::RGBA* col1 = nullptr;
					if (ptr1 != 0)
						col1 = static_cast<common::RGBA*>((void*)ptr1);
					camera* cam = UI->cam;
					ID2D1SolidColorBrush* b0;
					ID2D1SolidColorBrush* b1 = nullptr;
					(*cam->GetRenderTargetP())->CreateSolidColorBrush(col0->toColorF(), &b0);
					if (col1 != nullptr)
						(*cam->GetRenderTargetP())->CreateSolidColorBrush(col1->toColorF(), &b1);
					pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>* pir = new pair<ID2D1SolidColorBrush*, ID2D1SolidColorBrush*>(b0, b1);
					return PyLong_FromVoidPtr(pir);
				}
				PYFUNC(AddCol)
				{
					ui ID;
					float r, g, b, a;
					char* ch0 = NULL;
					string name;
					PyArg_ParseTuple(args, "I|f|f|f|f|s", &ID, &r, &g, &b, &a, &ch0);
					name = ch0;
					common::RGBA col(r, g, b, a);
					//auto ptr = malloc(sizeof common::RGBA);
					void* ptr = new common::RGBA(col);
					if (ID != 0)
						UI->wnds[ID]->otherobjts.push_back(ptr);
					return PyLong_FromVoidPtr(ptr);
				}
				PYFUNC(UpdatePos)
				{
					ui ID;
					_UpdatePosMutex.lock();
					PyArg_ParseTuple(args, "I", &ID);
					UI->wnds[ID]->updatepos();
					_UpdatePosMutex.unlock();
					return PyLong_FromLong(0);
				}
				PYFUNC(GetArg)
				{
					_GetArgMutex.lock();
					string strid;
					char* stridch = NULL;
					int it;
					auto argn = PyTuple_Size(args);
					if (argn == 3)
					{
						char* ch0;
						PyArg_ParseTuple(args, "s|i|s", &stridch, &it,&ch0);
						strid = stridch;
						string type = ch0;
						_GetArgMutex.unlock();
						return common::Python::CastPyObj(UI->args[strid][it], type);
					}
					PyArg_ParseTuple(args, "s|i", &stridch, &it);
					strid = stridch;
					//cout << strid << " has reached GetArg(" << to_string(it) << ")\n";
					if (!MapFind(UI->args.getref(), strid))
					{
						PyErr_SetString(hError, "GetArg error: Arguments were deleted before the call");
						_GetArgMutex.unlock();
						return NULL;
					}
					if (UI->args[strid].size() == 0)
					{
						PyErr_SetString(hError, "GetArg error: No arguments found");
						_GetArgMutex.unlock();
						return NULL;
					}
					vector<boost::any> bargs = {};
				retry:;
					try
					{
						bargs = UI->args[strid];
					}
					catch (...)
					{
						goto retry;
					}
					_GetArgMutex.unlock();
					switch (it)
					{
					case 0:
					{
						return PyLong_FromUnsignedLong(boost::any_cast<ui>(bargs[0]));
						break;
					}
					case 1:
					{
						return PyLong_FromLong(boost::any_cast<int>(bargs[1]));
						break;
					}
					case 2:
					{
						return PyLong_FromUnsignedLong(boost::any_cast<ui>(bargs[2]));
						break;
					}
					case 3:
					{
						string str = boost::any_cast<string>(bargs[3]);
						return PyUnicode_FromStringAndSize(str.c_str(), str.size());
						break;
					}
					case 4:
					{
						string str = boost::any_cast<string>(bargs[4]);
						return PyUnicode_FromStringAndSize(str.c_str(), str.size());
						break;
					}
					case 5:
					{
						return PyLong_FromVoidPtr(boost::any_cast<void*>(bargs[5]));
						break;
					}
					case 6:
					{
						return PyLong_FromLong(boost::any_cast<int>(bargs[6]));
						break;
					}
					case 7:
					{
						return PyLong_FromLong(boost::any_cast<int>(bargs[7]));
						break;
					}
					}
				}
				PYFUNC(Exit)
				{
					string strid;
					char*ch = NULL;
					PyArg_ParseTuple(args, "s", &ch);
					Py_RETURN_TRUE;
					strid = ch;
#if PYWRITESCRIPTDATA == true
						cout << strid << " has exited" << endl;
#endif
					//while (UI->isargbmodified)
					//	Sleep(0);
					//UI->isargbmodified = true;
					UI->argmodmutex->lock();

					if (MapFind(UI->args.getref(), strid))
					{
						auto bargs = UI->args[strid];
						if (bargs.size() > 10)
						{
							UI->argmodmutex->unlock();
							Py_RETURN_TRUE;
						}
						auto fargs = boost::any_cast<vector<wchar_t*>>(bargs[(bargs.size() < 10)?1:8]);
						int i = 0;
						while (i < fargs.size())
						{
							delete[] fargs[i];
							i++;
						}
						if (bargs.size() > 10)
						{
							*boost::any_cast<bool*>(bargs[13]) = true;
						}
						UI->idmapmutex.lock();
						UI->processtridmap.erase(getpid());
						UI->idmapmutex.unlock();
						UI->args.erase(strid);
					}
					else
					{
						//PyErr_SetString(hError, "Exit error: Exit has already been called or wrong strid has been passed");
					}
					//UI->isargbmodified = false;
					UI->argmodmutex->unlock();
					Py_RETURN_TRUE;

#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					Py_RETURN_TRUE;
					return PyLong_FromLong(0);
				}
				PYFUNC(MouseMoveDetB)
				{
					ulli ptr;
					bool b;
					PyArg_ParseTuple(args, "K|b", &ptr, &b);
					auto bcptr = static_cast<pair<button*, controls*>*>((void*)ptr);
					bcptr->first->callpyonanymmove = b;
					Py_RETURN_TRUE;
				}
				PYFUNC(Memory_Set)
				{
					_Memory_SetMutex.lock();
					string type, name, dummytype, dummyname;
					ui ID;
					char* ch0 = NULL, *ch1 = NULL;
					bool b = false;
					auto size = PyTuple_Size(args);
					PyObject* obj;
					if (size == 3)
					{
						ch1 = "PYOBJ";
						PyArg_ParseTuple(args, "I|s|O", &ID, &ch0, &obj);
					}
					else if (size == 4)
						PyArg_ParseTuple(args, "I|s|s|O", &ID, &ch0, &ch1, &obj);
					else
						return NULL;
					if (string(ch0).find("subwindow") != string::npos)
						pass;
					Py_INCREFTS(obj);
					if (ID == 0)
						ID = 1;
					name = ch0;
					type = ch1;
					if (istypestr(name))
					{
						string str = type;
						type = name;
						name = str;
					}
					int pid = 0;
					window* wnd = UI->wnds[ID];
					if (!MapFind(wnd->memory, pid))
					{
						wnd->memory[pid];
					}
					if (type == "PYOBJ")
					{
						Py_INCREFTS(obj);
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) obj)));
					}
					else if (type == "STR")
					{
						string val = PyBytes_AsString(PyUnicode_AsUTF8String(obj));
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) val)));
					}
					else if (type == "INT")
					{
						int val = PyLong_AsLong(obj);
						//PyArg_ParseTuple(args, string("I|s|s|" + 'i').c_str(), &ID, &dummyname, &dummytype, &val);
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) val)));
					}
					else if (type == "UINT")
					{
						ui val = PyLong_AsUnsignedLong(obj);
						//PyArg_ParseTuple(args, string("I|s|s|" + 'i').c_str(), &ID, &dummyname, &dummytype, &val);
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) val)));
					}
					else if (type == "BOOL")
					{
						bool val;
						//PyArg_ParseTuple(args, string("I|s|s|" + 'p').c_str(), &ID, &ch0, &ch1, &val);
						val = PyObject_IsTrue(obj);
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) val)));
					}
					else if (type == "FLOAT")
					{
						float val = PyFloat_AsDouble(obj);
						//PyArg_ParseTuple(args, string("I|s|s|" + 'f').c_str(), &ID, &ch0, &ch1, &val);
						MapSet(wnd->memory[pid], make_pair(name, make_pair(type, (boost::any) val)));
					}
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					//Py_DECREF(obj);
					Py_DECREFTS(obj);
					_Memory_SetMutex.unlock();
					Py_RETURN_TRUE;

				}
				PYFUNC(Memory_GetType)
				{
					string type, name;
					ui ID;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					if (ID == 0)
						ID = 1;
					name = ch0;
					window* wnd = UI->wnds[ID];
					type = wnd->memory[0][name].first;
					return PyUnicode_FromStringAndSize(type.c_str(), type.size());
				}
				PYFUNC(Memory_Get)
				{
					string type, name;
					ui ID;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					if (ID == 0)
						ID = 1;
					name = ch0;
					window* wnd = UI->wnds[ID];
					auto var = wnd->memory[0][name].second;
					type = wnd->memory[0][name].first;
					if (string(ch0).find("subwindow") != string::npos)
						pass;
					if (type == "PYOBJ")
					{
						auto val = boost::any_cast<PyObject*>(var);
						Py_INCREFTS(val);
						return val;
					}
					else if (type == "STR")
					{
						auto val = boost::any_cast<string>(var);
						//cout << val << endl;
						return PyUnicode_FromStringAndSize(val.c_str(), val.size());
					}
					else if (type == "INT")
					{
						//cout << boost::any_cast<int>(var) << endl;
						return PyLong_FromLong(boost::any_cast<int>(var));
					}
					else if (type == "UINT")
					{
						//cout << boost::any_cast<int>(var) << endl;
						return PyLong_FromLong(boost::any_cast<ui>(var));
					}
					else if (type == "BOOL")
					{
						//cout << boost::any_cast<bool>(var) << endl;
						return PyBool_FromLong((long)boost::any_cast<bool>(var));
					}
					else if (type == "FLOAT")
					{
						//cout << boost::any_cast<float>(var) << endl;
						return PyFloat_FromDouble(boost::any_cast<float>(var));
					}
					return PyLong_FromLong(0);
				}
				PYFUNC(Memory_GetSafe)
				{
					string type, name;
					ui ID;
					char* ch0 = NULL;
					PyObject* obj;
					PyArg_ParseTuple(args, "I|s|O", &ID, &ch0,&obj);
					Py_INCREFTS(obj);
					if (ID == 0)
						ID = 1;
					name = ch0;
					window* wnd = UI->wnds[ID];
					if (!MapFind(wnd->memory[0], name))
						return obj;
					Py_DECREFTS(obj);
					auto var = wnd->memory[0][name].second;
					type = wnd->memory[0][name].first;
					if (type == "PYOBJ")
					{
						auto val = boost::any_cast<PyObject*>(var);
						return val;
					}
					else if (type == "STR")
					{
						auto val = boost::any_cast<string>(var);
						//cout << val << endl;
						return PyUnicode_FromStringAndSize(val.c_str(), val.size());
					}
					else if (type == "INT")
					{
						//cout << boost::any_cast<int>(var) << endl;
						return PyLong_FromLong(boost::any_cast<int>(var));
					}
					else if (type == "UINT")
					{
						//cout << boost::any_cast<int>(var) << endl;
						return PyLong_FromLong(boost::any_cast<ui>(var));
					}
					else if (type == "BOOL")
					{
						//cout << boost::any_cast<bool>(var) << endl;
						return PyBool_FromLong((long)boost::any_cast<bool>(var));
					}
					else if (type == "FLOAT")
					{
						//cout << boost::any_cast<float>(var) << endl;
						return PyFloat_FromDouble(boost::any_cast<float>(var));
					}
					return PyLong_FromLong(0);

				}
				PYFUNC(Memory_Find)
				{
					string name;
					ui ID = 1;
					char* ch0 = NULL;
					if(PyTuple_Size(args) == 1)
						PyArg_ParseTuple(args, "s", &ch0);
					else
						PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					if (ID == 0)
						ID = 1;
					name = ch0;
					//	if (name == "LBUTTONUP")
				//			DebugBreak();
					return PyBool_FromLong((long)MapFind(UI->wnds[ID]->memory[0], name));
				}
				PYFUNC(GetRealPosX)
				{
					int i;
					ui ID;
					PyArg_ParseTuple(args, "I|i", &ID, &i);
					window* wnd = UI->wnds[ID];
					return PyLong_FromLong(wnd->btts[i]->pos->x);
				}
				PYFUNC(GetRealPosY)
				{
					int i;
					ui ID;
					PyArg_ParseTuple(args, "I|i", &ID, &i);
					window* wnd = UI->wnds[ID];
					return PyLong_FromLong(wnd->btts[i]->pos->y);
				}
				PYFUNC(GetRealSizeX)
				{
					int i;
					ui ID;
					auto size = PyTuple_Size(args);
					if (size == 2)
					{
						PyArg_ParseTuple(args, "I|i", &ID, &i);
						window* wnd = UI->wnds[ID];
						return PyLong_FromLong(wnd->btts[i]->size.x);
					}
					int ii;
					PyArg_ParseTuple(args, "I|i|i", &ID, &i,&ii);
					window* wnd = UI->wnds[ID];
					return PyLong_FromLong((wnd->shpb[i].brushes.size()>i)?wnd->shpb[i].brushes[ii]->size->x:wnd->shpb[i].sprites[ii]->size.width);
				}
				PYFUNC(GetRealSizeY)
				{
					int i;
					ui ID;
					auto size = PyTuple_Size(args);
					if (size == 2)
					{
						PyArg_ParseTuple(args, "I|i", &ID, &i);
						window* wnd = UI->wnds[ID];
						return PyLong_FromLong(wnd->btts[i]->size.y);
					}
					int ii;
					PyArg_ParseTuple(args, "I|i|i", &ID, &i, &ii);
					window* wnd = UI->wnds[ID];
					return PyLong_FromLong((wnd->shpb[i].brushes.size()>ii) ? wnd->shpb[i].brushes[ii]->size->y : wnd->shpb[i].sprites[ii]->size.height);
				}
				PYFUNC(GetRealSize)
				{
					int i, ii;
					ui ID;
					auto argn = PyTuple_Size(args);
					int2 size;
					if (argn == 2)
					{
						PyArg_ParseTuple(args, "I|i", &ID, &i);
						window* wnd = UI->wnds[ID];
						size = wnd->btts[i]->size;
					}
					else
					{
						PyArg_ParseTuple(args, "I|i|i", &ID, &i, &ii);
						window* wnd = UI->wnds[ID];
						size = int2(((wnd->shpb[i].brushes.size() > i) ? *wnd->shpb[i].brushes[ii]->size : wnd->shpb[i].sprites[ii]->size));
					}
					pyobj mod = PyImport_ImportModule("GTYPES");
					PY_INCREFTS(mod);
					pyobj _class = PyObject_GetAttrString(mod, "int2");
					PY_INCREFTS(_class);
					pyobj ret = PyInstanceMethod_New(_class);
					PY_INCREFTS(ret);
					PyObject_SetAttrString(ret, "x", PyLong_FromLong(size.x));
					PyObject_SetAttrString(ret, "y", PyLong_FromLong(size.y));
					PY_DECREFTS(_class);
					PY_DECREFTS(mod);
					return ret;

				}
				PYFUNC(GetRealSizeXY)
				{
					return GetRealSize(self, args);
				}
				PYFUNC(SetShapeSize)
				{
					int2 size;
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i|i|i", &ID, &n, &size.x, &size.y);
					window* wnd = UI->wnds[ID];
					auto s = wnd->shpb[n].sprites;
					auto b = wnd->shpb[n].brushes;
					int i = 0;
					while (i < s.size())
					{
						s[i]->size = size;
						i++;
					}
					i = 0;
					while (i < b.size())
					{
						*b[i]->size  = size;
						i++;
					}
					i = 0;
					Py_RETURN_TRUE;
				}
				PYFUNC(SetShapePos)
				{
					int2 pos;
					ui ID;
					int n;
					PyArg_ParseTuple(args, "I|i|i|i", &ID, &n, &pos.x,&pos.y);
					window* wnd = UI->wnds[ID];
					auto s = wnd->shpb[n].pospairs;
					int i = 0;
					while (i < s.size())
					{
						*s[i].first = s[i].second + pos;
						i++;
					}
					Py_RETURN_TRUE;
				}
				PYFUNC(IsInside)
				{
					int4 rect;
					int2 point;
					PyArg_ParseTuple(args, "i|i|i|i|i|i", &point.x, &point.y, &rect.x, &rect.y, &rect.z, &rect.w);
					return PyBool_FromLong((long)classvariables::isinside(rect, point));
				}
				PYFUNC(CursorX)
				{
					POINT  p;
					GetCursorPos(&p);
					return PyLong_FromLong(p.x);
				}
				PYFUNC(CursorY)
				{
					POINT  p;
					GetCursorPos(&p);
					return PyLong_FromLong(p.y);
				}
				PYFUNC(File_GetVar)
				{
					string filename, varname, type;
					char* ch0 = NULL; char* ch1 = NULL; char* ch2 = NULL;
					PyArg_ParseTuple(args, "s|s|s", &ch0, &ch1, &ch2);
					filename = ch0;
					varname = ch1;
					type = ch2;
					AZfile *file = nullptr;
					if (filename == "gameui")
						file = &UI->gameuif;
					else if (filename == "styles")
						file = &UI->stylef;
					auto var = *file->GetFromNodes(varname);
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					if (type == "STR")
					{
						auto val = boost::any_cast<string>(var);
						return PyUnicode_FromStringAndSize(val.c_str(), val.size());
					}
					else if (type == "INT")
					{
						return PyLong_FromLong(boost::any_cast<int>(var));
					}
					else if (type == "BOOL")
					{
						return PyBool_FromLong((long)boost::any_cast<bool>(var));
					}
					else if (type == "FLOAT")
					{
						return PyFloat_FromDouble(boost::any_cast<float>(var));
					}
				}
				PYFUNC(File_Find)
				{
					string filename, varname;
					char* ch0 = NULL, *ch1 = NULL;
					PyArg_ParseTuple(args, "s|s", &ch0, &ch1);
					filename = ch0;
					varname = ch1;
					AZfile *file = nullptr;
					if (filename == "gameui")
						file = &UI->gameuif;
					else if (filename == "styles")
						file = &UI->stylef;
					return PyBool_FromLong((long)file->findvar(varname));
				}
				PYFUNC(AddSpriteToShape)
				{
					string spritename;
					ui ID;
					ui flags;
					int i;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s|i|I", &ID, &ch0, &i,&flags);
					int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
					spritename = ch0;
					window* wnd = UI->wnds[ID];
					style s = UI->styles[wnd->styleid];
					sprite sp = UI->icons[spritename];
					uni2<float> mf;
					uni2<float> pmf;
					if (flags & WF_SCALETO_VH || flags == NULL)
					{
						mf.x = ((float)wsize.x)*wnd->size.x;
						mf.y = ((float)wsize.y)*wnd->size.y;
						pmf.x = ((float)wsize.x)*wnd->defpos.x;
						pmf.y = ((float)wsize.y)*wnd->defpos.y;
					}
					else if (flags & WF_SCALETO_V)
					{
						mf.x = ((float)wsize.y)*wnd->size.x;
						mf.y = ((float)wsize.y)*wnd->size.y;
						pmf.x = ((float)wsize.y)*wnd->defpos.x;
						pmf.y = ((float)wsize.y)*wnd->defpos.y;
					}
					else if (flags & WF_SCALETO_H)
					{
						mf.x = ((float)wsize.x)*wnd->size.x;
						mf.y = ((float)wsize.x)*wnd->size.y;
						pmf.x = ((float)wsize.x)*wnd->defpos.x;
						pmf.y = ((float)wsize.x)*wnd->defpos.y;
					}
					uni2<float> rmf = mf;
					if (s.shapes[i].lockh)
						rmf.x = mf.x;
					if (s.shapes[i].lockv)
						rmf.y = mf.y;
					sp.SetOffsetXYp(wnd->pos, true);
					//bool* rb = new bool(true);
					sp.render = wnd->shaperb[i];
					sp.useidentp = true;
					sp.identp = wnd->identp;
					uni2<float> copy;
					sp.size = (copy = s.shapes[i].size * rmf).toint2();
					int2* i2sp;
					sp.SyncPos(i2sp = new int2(/*pmf.toint2()+*/(s.shapes[i].pos * rmf).toint2()), false);
					//*i2sp = *i2sp + (wnd->defpos*).toint2();
					wnd->sf->sprites.push_back(sp);
					wnd->posvec.push_back(make_pair(i2sp, *i2sp));
#if corruptioncheck == 1
					auto _heap = malloc(sizeof(int));
					free(_heap);
#endif
					return PyLong_FromLong(0);
				}
				PYFUNC(GetName)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);
					window* wnd = UI->wnds[ID];
					return PyUnicode_FromStringAndSize(wnd->strname.c_str(), wnd->strname.size());
				}
				PYFUNC(DestroyWnd)
				{
					ui ID;
					PyArg_ParseTuple(args, "I", &ID);

					Py_RETURN_TRUE;
				}
				PYFUNC(DestroyWndAndExit)
				{
					return Exit(self, args);;
				}
				PYFUNC(MinimizeWnd)
				{
					Py_RETURN_TRUE;
				}
				PYFUNC(WaitFor)
				{
					ui ID;
					string comm, strid;
					char* ch0 = NULL, *ch1 = NULL;
					PyArg_ParseTuple(args, "I|s|s", &ID,&ch0,&ch1);
					comm = ch0;
					strid = ch1;
					auto wnd = UI->wnds[ID];
					map<string,void*> signals;
					int i = 0;
					while (i < comm.size())
					{
						string str = "";
						while (!isblank(comm[i]))
							if (!(i + 1 < comm.size()))
							{
								str += comm[i];
								break;
							}
							else
							{
								str += comm[i];
								i++;
							}
						signals.insert(make_pair(str,nullptr));
						i++;
					}
					i = 0;
					auto bargs = wnd->coreptr->args[strid];
					string scripttype = boost::any_cast<string>(bargs[9]);
					mutex m;
					unique_lock<std::mutex> lk(m);
					condition_variable *mttw = new condition_variable;
					wnd->waitmapmutex.lock();
					wnd->waitmap.insert(make_pair(scripttype, make_pair(signals,(void*)mttw)));
					wnd->waitmapmutex.unlock();
					thread* th = boost::any_cast<thread*>(bargs[12]);
					scriptthreadmutex.lock();
					*boost::any_cast<bool*>(bargs[13]) = true;
					bool* rb = nullptr;
					if (curth == th)
					{
						curth = new thread(pythreadqueue, rb = new bool(false));
						scriptthreads.push_back(make_pair(curth, rb));
						scmap.insert(make_pair(curth, rb));
						curth->detach();
					}
					scriptthreadmutex.unlock();
					//detachUIthmutex.lock();
					
					//detachUIthmutex.unlock();
					GUIthmutex.unlock();
					mttw->wait(lk);
					wnd->waitmapmutex.lock();
					GUIthmutex.lock();
					delete mttw;
					string *val = static_cast<string*>(wnd->waitmap[scripttype].second);
					string _val = *val;
					delete val;
					wnd->waitmap.erase(scripttype);
					wnd->waitmapmutex.unlock();
					return PyUnicode_FromStringAndSize(_val.c_str(), _val.size());
				}
				PYFUNC(Detach)
				{
					ui ID = 0;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					string strid = ch0;
					auto wnd = UI->wnds[ID];
					auto bargs = wnd->coreptr->args[strid];
					thread* th = boost::any_cast<thread*>(bargs[12]);
					scriptthreadmutex.lock();
					*boost::any_cast<bool*>(bargs[13]) = true;
					bool* rb = nullptr;
					if (curth == th)
					{
						curth = new thread(pythreadqueue, rb = new bool(false));
						scriptthreads.push_back(make_pair(curth, rb));
						scmap.insert(make_pair(curth, rb));
						curth->detach();
					}
					scriptthreadmutex.unlock();
					Py_RETURN_TRUE;
				}
				PYFUNC(GetSTRID)
				{
					_GetSTRIDMutex.lock();
					string strid = UI->processtridmap[getpid()];
					//cout << strid << " has reached GetSTRID\n";
					_GetSTRIDMutex.unlock();
					int i = 0;
					string s = "";
					while (!IsNumberCH(strid[i]))
						s += strid[i++];
					if (s == "basewindow_msg_move.py")
						pass;
					return PyUnicode_FromStringAndSize (strid.c_str(), strid.size());
				}
				PYFUNC(GetFLoc)
				{
					_GetFLocMutex.lock();
					string strid;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "s", &ch0);
					strid = ch0;
					auto loc = boost::any_cast<string>(UI->args[strid][(UI->args[strid].size() < 10)? 2:14]);
					_GetFLocMutex.unlock();
					return PyUnicode_FromStringAndSize(loc.c_str(), loc.size());
				}
				PYFUNC(Once)
				{
					UI->oncemutex.lock();
					ui ID = 0;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s", &ID, &ch0);
					string strid = ch0;
					if (!MapFind(UI->oncescmap.getref(), ID))
						UI->oncescmap.insert(make_pair(ID, map<string,string>()));
					auto filename = boost::any_cast<string>(UI->args[strid][9]);
					UI->oncescmap[ID].insert(make_pair(filename, strid));
					UI->oncemutex.unlock();
					Py_RETURN_TRUE;
				}
				PYFUNC(KeyState)
				{
					char* ch0 = NULL, *ch1 = NULL;
					PyArg_ParseTuple(args, "s", &ch0);
					string code = ch0;
					map<string, ui>inputs = { { "LBUTTON",VK_LBUTTON },{ "RBUTTON",VK_RBUTTON },{ "MBUTTON",VK_MBUTTON } };
					auto codei = VK_RETURN;
					if (MapFind(inputs, code))
						codei = inputs[code];
					else if (code[0] == '0' && code[1] == 'x')
					{
						codei = stol(code, 0, 16);
					}
					else
					{
						codei = (int)(char)code[0];
					}
					if(GetKeyState(codei) & 0x8000)
						Py_RETURN_TRUE;
					Py_RETURN_FALSE;
				}
				PYFUNC(GetParent)
				{
					ui ID = 0;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromUnsignedLong(UI->wnds[ID]->parent->ID);
				}
				PYFUNC(GetWndByName)
				{
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "s", &ch0);
					if (!MapFind(UI->wndnamemap, (string)ch0))
						return PyLong_FromUnsignedLong(0);
					return PyLong_FromUnsignedLong(UI->wndnamemap.at((string)ch0));
				}
				PYFUNC(GetFlags)
				{
					ui ID = 0;
					PyArg_ParseTuple(args, "I", &ID);
					return PyLong_FromUnsignedLong(UI->wnds[ID]->flags);
				}
				PYFUNC(ProjectWindow)
				{
					ui wndid, target;
					uni2<float> proppos, propsize;
					int shapen;
					PyArg_ParseTuple(args, "I|I|f|f|f|f|i", &target, &wndid,&proppos.x,&proppos.y,&propsize.x,&propsize.y,&shapen);
					int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
					_ProjectWindowMutex.lock();
					window* wnd = UI->wnds[wndid];
					window* projwnd = new window;
					window* twnd = UI->wnds[target];
					memcpy(projwnd, wnd, sizeof(window));
					projwnd->resize(twnd->defpos,propsize,true);
					int4 rect(*wnd->pos,wnd->realsize + *wnd->pos);
					projwnd->f->capturerect = rect;
					projwnd->f->renderaftercapture = false;
					projwnd->f->capture = true;
					while (projwnd->f->capture)
						Sleep(0);
					projwnd->f->ismactive = false;
					projwnd->f->ischfact = false;
					sprite sp = UI->wnds[target]->f->capturetarget;
					_ProjectWindowMutex.unlock();
					delete projwnd;

					style s = UI->styles[twnd->styleid];
					uni2<float> mf;
					uni2<float> pmf;
					ui flags = twnd->flags;
					if (flags & WF_SCALETO_VH || flags == NULL)
					{
						mf.x = ((float)wsize.x)*twnd->size.x;
						mf.y = ((float)wsize.y)*twnd->size.y;
						pmf.x = ((float)wsize.x)*twnd->defpos.x;
						pmf.y = ((float)wsize.y)*twnd->defpos.y;
					}
					else if (flags & WF_SCALETO_V)
					{
						mf.x = ((float)wsize.y)*twnd->size.x;
						mf.y = ((float)wsize.y)*twnd->size.y;
						pmf.x = ((float)wsize.y)*twnd->defpos.x;
						pmf.y = ((float)wsize.y)*twnd->defpos.y;
					}
					else if (flags & WF_SCALETO_H)
					{
						mf.x = ((float)wsize.x)*twnd->size.x;
						mf.y = ((float)wsize.x)*twnd->size.y;
						pmf.x = ((float)wsize.x)*twnd->defpos.x;
						pmf.y = ((float)wsize.x)*twnd->defpos.y;
					}
					uni2<float> rmf = mf;
					if (s.shapes[shapen].lockh)
						rmf.x = mf.x;
					if (s.shapes[shapen].lockv)
						rmf.y = mf.y;
					sp.SetOffsetXYp(twnd->pos, true);
					//bool* rb = new bool(true);
					sp.render = twnd->shaperb[shapen];
					sp.useidentp = true;
					sp.identp = twnd->identp;
					uni2<float> copy;
					sp.size = (copy = s.shapes[shapen].size * rmf).toint2();
					int2* i2sp;
					sp.SyncPos(i2sp = new int2(/*pmf.toint2()+*/(s.shapes[shapen].pos * rmf).toint2()), false);
					//*i2sp = *i2sp + (twnd->defpos*).toint2();
					sp.useidentp = true;
					auto ret = PyLong_FromVoidPtr(sp.render = sp.identp = new bool(true));
					twnd->sf->sprites.push_back(sp);
					twnd->posvec.push_back(make_pair(i2sp, *i2sp));
					return ret;
				}
				PYFUNC(GetTextureFromWindow)
				{
					ui wndid, target;
					uni2<float> proppos, propsize;
					int shapen;
					PyArg_ParseTuple(args, "I|I|f|f|f|f|i", &target, &wndid, &proppos.x, &proppos.y, &propsize.x, &propsize.y, &shapen);
					vector<window*> wnds;
					window* wnd = UI->wnds[wndid];
					sprite *spr = new sprite;
					vector<bool*> waitvec;
					std::function<void(window*,vector<window*>&, sprite*,vector<bool*>)> frf = [&](window* wnd, vector<window*>&_wnds,sprite *sp,vector<bool*>&waitvec)
					{
						int i = 0;
						int4 rect(*wnd->pos,*wnd->pos+wnd->realsize);
						wnd->f->capture = true;
						waitvec.push_back(&wnd->f->capture);
						wnd->f->capturerect = rect;
						wnd->f->renderaftercapture = false;
						wnd->f->capturetarget = sp;
						while (i < wnd->children.size())
						{
							frf(wnd->children[i],_wnds,sp,waitvec);
							i++;
						}
						_wnds.push_back(wnd);
					};
					frf(wnd,wnds,spr,waitvec);
					window* twnd = UI->wnds[target];
					int i = 0;
					while (i < waitvec.size())
					{
						while (*waitvec[i])
							Sleep(0);
						i++;
					}
					i = 0;
					//while (i < wnds.size())
					//{
					//	sprite sp = *spr;
					//	twnd->sf[shapen].sprites.push_back(sp);
					//}
					//twnd->sf[shapen].sprites.push_back(*spr);
					//delete spr;
					return PyLong_FromVoidPtr(spr);
				}
				PYFUNC(ProjectTexture)
				{
					ui target;
					ulli txint;
					uni2<float> proppos, propsize;
					int shapen;
					PyArg_ParseTuple(args, "I|K|f|f|f|f|i", &target, &txint, &proppos.x, &proppos.y, &propsize.x, &propsize.y, &shapen);
					sprite* sp = (sprite*)(void*)txint;
					window* twnd = UI->wnds[target];
					int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
					style s = UI->styles[twnd->styleid];
					uni2<float> mf;
					uni2<float> pmf;
					int4 rect(*twnd->pos, twnd->realsize + *twnd->pos);
					ui flags = twnd->flags;
					if (flags & WF_SCALETO_VH || flags == NULL)
					{
						mf.x = ((float)wsize.x)*twnd->size.x;
						mf.y = ((float)wsize.y)*twnd->size.y;
						pmf.x = ((float)wsize.x)*twnd->defpos.x;
						pmf.y = ((float)wsize.y)*twnd->defpos.y;
					}
					else if (flags & WF_SCALETO_V)
					{
						mf.x = ((float)wsize.y)*twnd->size.x;
						mf.y = ((float)wsize.y)*twnd->size.y;
						pmf.x = ((float)wsize.y)*twnd->defpos.x;
						pmf.y = ((float)wsize.y)*twnd->defpos.y;
					}
					else if (flags & WF_SCALETO_H)
					{
						mf.x = ((float)wsize.x)*twnd->size.x;
						mf.y = ((float)wsize.x)*twnd->size.y;
						pmf.x = ((float)wsize.x)*twnd->defpos.x;
						pmf.y = ((float)wsize.x)*twnd->defpos.y;
					}
					uni2<float> rmf = mf;
					if (s.shapes[shapen].lockh)
						rmf.x = mf.x;
					if (s.shapes[shapen].lockv)
						rmf.y = mf.y;
					sp->SetOffsetXYp(twnd->pos, true);
					//bool* rb = new bool(true);
					sp->render = twnd->shaperb[shapen];
					sp->useidentp = true;
					sp->identp = twnd->identp;
					uni2<float> copy;
					sp->size = (copy = s.shapes[shapen].size * rmf).toint2();
					int2* i2sp;
					sp->SyncPos(i2sp = new int2(/*pmf.toint2()+*/(s.shapes[shapen].pos * rmf).toint2()), false);
					//*i2sp = *i2sp + (twnd->defpos*).toint2();
					sp->useidentp = true;
					auto ret = PyLong_FromVoidPtr(sp->render = sp->identp = new bool(true));
					twnd->sf->sprites.push_back(sp);
					twnd->posvec.push_back(make_pair(i2sp, *i2sp));
					return ret;
				}
				PYFUNC(ChangeBoolPtr)
				{
					bool* ptr;
					bool state;
					PyObject* obj;
					PyArg_ParseTuple(args, "O|b", &obj,&state );
					Py_INCREFTS(obj);
					ptr = (bool*)(void*)PyLong_AsVoidPtr(obj);
					Py_DECREFTS(obj);
					*ptr = state;
					Py_RETURN_TRUE;
				}
				PYFUNC(AttachTo)
				{
					ui pid, chid, flags;
					PyArg_ParseTuple(args, "I|I|I", &pid,&chid,&flags);
					UI->AttachTo(UI->wnds[pid], UI->wnds[chid], flags);
					Py_RETURN_TRUE;
				}
				PYFUNC(SetAttachToF)
				{
					ui ID = 0;
					char* ch0 = NULL;
					PyArg_ParseTuple(args, "I|s", &ID,&ch0);
					string fname = ch0;
					window* wnd = UI->wnds[ID];
					wnd->handleattachto = fname;
					Py_RETURN_TRUE;
				}
				PYFUNC(IsDestroyed)
				{
					Py_RETURN_FALSE;
				}
				PYFUNC(SendMsg)
				{
					ui ID;
					ui lpid, wpid;
					LPARAM lParam;
					WPARAM wParam;
					window* wnd;
					ui msg;
					PyArg_ParseTuple(args, "I|I|I|I",&ID,&wpid,&lpid,&msg);
					lParam = lpid;
					wParam = wpid;
					wnd = UI->wnds[ID];
					wnd->con.MouseEvent(wParam, lParam, msg);
					Py_RETURN_TRUE;
				}
				PYFUNC(GetWndID)
				{
					pyobj obj;
					PyArg_ParseTuple(args, "O", &obj);
					window* wnd = (window*)(void*)PyLong_AsVoidPtr(obj);
					return PyLong_FromUnsignedLongLong(wnd->ID);
				}
				static  PyObject* test(PyObject *self, PyObject *args)
				{
					return NULL;
				}
				PyMethodDef arr[__COUNTER__];
				struct PyModuleDef GUImodule;
				PyMODINIT_FUNC PyInit_GUI(void)
				{
					int i = 0;
					vector<PyMethodDef> moddefs;
					while (i < vb.names.size())
					{
						PyMethodDef md[] = { { vb.names[i].c_str(),vb.vec[i],METH_VARARGS,"If I'd tell ya I'd have to kill ya" } };
						moddefs.push_back(md[0]);
						i++;
					}
					PyMethodDef md[] = { { NULL, NULL, 0, NULL } };
					moddefs.push_back(md[0]);
					std::copy(moddefs.begin(), moddefs.end(), arr);
					GUImodule = {
						PyModuleDef_HEAD_INIT,
						"GUI",   /* name of module */
						NULL, /* module documentation, may be NULL */
						-1,       /* size of per-interpreter state of the module,
								  or -1 if the module keeps state in global variables. */
						arr/*&moddefsnew->operator[](0)*/
					};
					PyObject *m;
					m = PyModule_Create(&GUImodule);
					if (m == NULL)
						return NULL;
					hError = PyErr_NewException("GUI.error", NULL, NULL);
					Py_INCREF(hError);
					PyModule_AddObject(m, "error", hError);
					return m;
				}
			}
			namespace AZFLIBMOD
			{
				vecbreaker <PyObject*(*)(PyObject*, PyObject*)> vb;
				//----------------------------------------------------------------------------------------------------------------------------
				//MODULE: AZflib
				PyObject* hError = NULL;
				PYFUNC(Open)
				{
					_OpenMutex.lock();
					char *ch0, *ch1;
					string strid, name;
					PyArg_ParseTuple(args, "s", &ch0);
					//strid = ch0;
					name = ch0;
					WIN32_FIND_DATAA fdata;
					HANDLE hFind = (HANDLE)FindFirstFileA(name.c_str(), &fdata);
					if ((LONG)hFind == ERROR_FILE_NOT_FOUND)
					{
						FindClose(hFind);
						return PyLong_FromLong(0);
					}
					auto ptr = new AZfile(name);
					_OpenMutex.unlock();
					return PyLong_FromVoidPtr(ptr);
				}
				PYFUNC(FindFile)
				{
					char* ch0;
					PyArg_ParseTuple(args, "s", &ch0);
					string name = ch0;
					WIN32_FIND_DATAA fdata;
					HANDLE hFind = (HANDLE)FindFirstFileA(name.c_str(), &fdata);
					if ((LONG)hFind == ERROR_FILE_NOT_FOUND)
					{
						FindClose(hFind);
						Py_RETURN_FALSE;
					}
					Py_RETURN_TRUE;
				}
				PYFUNC(Release)
				{
					_ReleaseMutex.lock();
					ulli ptr;
					PyArg_ParseTuple(args, "K", &ptr);
					auto file = static_cast<AZfile*>((void*)ptr);
					delete file;
					_ReleaseMutex.unlock();
					Py_RETURN_TRUE;
				}
				PYFUNC(GetVar)
				{
					ulli ptr;
					char *ch0 = NULL;
					char *ch1;
					string type, name;
					if (PyTuple_Size(args) == 3)
					{
						PyArg_ParseTuple(args, "K|s|s", &ptr, &ch0, &ch1);
						type = ch0;
						name = ch1;
						type = STRtoUP(type);
					}
					else
					{
						PyArg_ParseTuple(args, "K|s", &ptr, &ch1);
						name = ch1;
					}
					auto file = static_cast<AZfile*>((void*)ptr);
					if (ch0 == NULL)
						type = file->GetType(name);
					return common::Python::CastPyObj(file->GetVarAny(name), type);
				}
				PYFUNC(SetVar)
				{
					_SetVarMutex.lock();
					ulli ptr;
					char *ch0, *ch1;
					string type, name;
					PyObject* val;
					PyArg_ParseTuple(args, "K|s|s|O", &ptr, &ch0, &ch1, &val);
					Py_INCREFTS(val);
					name = ch1;
					type = ch0;
					auto file = static_cast<AZfile*>((void*)ptr);
					file->SetVar(type, new boost::any(common::Python::AnyFromPyObj(val, type)), name);
					Py_DECREFTS(val);
					_SetVarMutex.unlock();
					Py_RETURN_TRUE;
				}
				PYFUNC(SaveFile)
				{
					_SaveFileMutex.lock();
					ulli ptr;
					char *ch0;
					PyArg_ParseTuple(args, "K|s", &ptr, &ch0);
					string dir = ch0;
					auto file = static_cast<AZfile*>((void*)ptr);
					file->SaveToFile(dir);
					_SaveFileMutex.unlock();
					Py_RETURN_TRUE;
				}
				PYFUNC(FindVar)
				{
					ulli ptr;
					char *ch0;
					string name;
					PyArg_ParseTuple(args, "K|s", &ptr, &ch0);
					name = ch0;
					auto file = static_cast<AZfile*>((void*)ptr);
					if (file == nullptr || name == "NULL")
						Py_RETURN_FALSE;
					if (name.find("NULL") != string::npos)
						Py_RETURN_FALSE;
					bool b = file->findvar(name);
					if (name.find("call") != string::npos)
						pass;
					return PyBool_FromLong(b);
				}
				//static PyMethodDef AZMethods[] =
				//{
				//	PYMETH(Open),
				//	PYMETH(Release),
				//	PYMETH(GetVar),
				//	PYMETH(SetVar),
				//	PYMETH(SaveFile),
				//	PYMETH(FindVar),
				//	{ NULL, NULL, 0, NULL }
				//};
				//static struct PyModuleDef AZflibmodule = {
				//	PyModuleDef_HEAD_INIT,
				//	"AZflib",   /* name of module */
				//	NULL, /* module documentation, may be NULL */
				//	-1,       /* size of per-interpreter state of the module,
				//			  or -1 if the module keeps state in global variables. */
				//	AZMethods
				//};
				//PyMODINIT_FUNC PyInit_AZflib(void)
				//{
				//	PyObject *m;
				//	m = PyModule_Create(&AZflibmodule);
				//	if (m == NULL)
				//		return NULL;
				//	return m;
				//}
				PyMethodDef arr[__COUNTER__];
				struct PyModuleDef AZflibmodule;
				PyMODINIT_FUNC PyInit_AZflib(void)
				{
					PyObject *m;
					int i = 0;
					vector<PyMethodDef> moddefs;
					while (i < vb.names.size())
					{
						PyMethodDef md[] = { { vb.names[i].c_str(),vb.vec[i],METH_VARARGS,"If I'd tell ya I'd have to kill ya" } };
						moddefs.push_back(md[0]);
						i++;
					}
					PyMethodDef md[] = { { NULL, NULL, 0, NULL } };
					moddefs.push_back(md[0]);
					std::copy(moddefs.begin(), moddefs.end(), arr);
					AZflibmodule = {
						PyModuleDef_HEAD_INIT,
						"AZflib",   /* name of module */
						NULL, /* module documentation, may be NULL */
						-1,       /* size of per-interpreter state of the module,
								  or -1 if the module keeps state in global variables. */
						arr
					};
					m = PyModule_Create(&AZflibmodule);
					if (m == NULL)
						return NULL;
					hError = PyErr_NewException("GUI.error", NULL, NULL);
					Py_INCREF(hError);
					PyModule_AddObject(m, "error", hError);
					return m;
				}
			}
			namespace GAPIMOD
			{
				extern struct PyModuleDef GAPImodule;;
			}
		}
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
			vector<ui> flagvec;
			auto nnmap = c->wndnamemap;
			int i = 0;
			c->gameuif = file;
			while (file.GetVarSafe(str, "W" + INTtoSTR(i)))
			{
				string name = str;
				str = str + "@";
				ui wflags = 0;
				auto res = ((file.GetVar<bool>(str + "W@PARENTPOSSIZE")?c->NewWindow(c->root, uni2<float>{file.GetVar<float>(str + "W@POSX" ),file.GetVar<float>(str + "W@POSY") },uni2<float>{ file.GetVar<float>(str + "W@SIZEX"), file.GetVar<float>(str + "W@SIZEY")}, file.GetVar<string>(str + "W@STYLE"), 
					wflags = (WF_NOVINIT | WF_HIDE | WF_SCALETO_V | ((file.GetVar<bool>(str + "W@POSBASERIGHT")) ? WF_POSBASERIGHT : WF_POSBASELEFT)) ,name)
				: c->NewWindow(c->root, uni2<float>{ file.GetVar<float>(str + "W@POSX"),file.GetVar<float>(str + "W@POSY") }, uni2<float>{ file.GetVar<float>(str + "W@SIZEX"), file.GetVar<float>(str + "W@SIZEY")}, file.GetVar<string>(str + "W@STYLE"),
					wflags = (WF_HIDE | WF_SCALETO_V | ((file.GetVar<bool>(str + "W@POSBASERIGHT")) ? WF_POSBASERIGHT : WF_POSBASELEFT)),name)));
				flagvec.push_back(wflags);
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
				if (flagvec[i] & WF_NOVINIT)
					flagvec[i] &= ~(WF_NOVINIT);
				c->AttachTo(c->wnds[nnmap[wndparentv[i].first]],wndparentv[i].second,AT_STYLEPROPPOS | AT_VINIT, /*WF_HIDE | WF_SCALETO_V*/flagvec[i]);
				wndparentv[i].second->show();
				i++;
			}
		}
		void pythreadqueue(bool* b)
		{
		restart:;
			scriptthreadmutex.lock();
			scriptthreadmutex.unlock();
			int i = 0;
			thread* thisth = curth;
			int ii = 0;
			UI->_mttw.wait(UI->_ulm);
			while (calls.size() > 0)
			{
				UI->argmodmutex->lock();
				auto call = calls.front();
				UI->argmodmutex->unlock();
				if (call->isbeingprocessed)
				{
					UI->argmodmutex->lock();
					calls.pop_front();
					UI->argmodmutex->unlock();
					continue;
				}
				GUIthmutex.lock();
				call->isbeingprocessed = true;
				call->conptr->callpyscript(call->strid,call->scriptloc,call->scriptname,(void*)call);
				if (curth != thisth)
				{
					i = 0;
					//*call->cancontinue = true;
					//delete call;
					while (i < scriptthreads.size())
					{
						if (scriptthreads[i].first == curth)
						{
							*scriptthreads[i].second = true;
							break;
						}
						i++;
					}
					GUIthmutex.unlock();
					return;
				}
				UI->argmodmutex->lock();
				//*call->cancontinue = true;
				//delete call;
				calls.pop_front();
				UI->argmodmutex->unlock();
				GUIthmutex.unlock();
			}
			goto restart;
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
		bool core::MouseEvent(WPARAM wParam, LPARAM lParam, UINT msg)
		{
			int i = 0;
			bool b = false;
			BOOST_FOREACH(auto& it, wnds)
			{
				if (it.second != nullptr)
				{
					it.second->con.MouseEvent(wParam, lParam, msg);
					if (it.second->con.waspycalled)
						b = true;
					it.second->con.waspycalled = false;
				}
			}
			return b;
		}
		UIresult core::AttachTo(window * parent, window * child, unsigned long int flags, unsigned long int visflags)
		{
			if (parent != nullptr)
				if (parent->isnull)
					parent == nullptr;
			UIresult ret;
			int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
			uni2<float> mf;
			mf.x = ((float)wsize.x)*child->size.x;
			mf.y = ((float)wsize.y)*child->size.y;
			window* oldparent = child->parent;
			bool deloldparent = false;
			if (!oldparent)
			{
				deloldparent = true;
				oldparent = new window;
				oldparent->size = uni2<float>(1, 1);
			}
			if (parent == oldparent && flags & AT_VINIT)
			{
				child->initvis(styles[child->styleid], child->defpos, parent->realsize,visflags);
				ret.code = UI_OK;
				return ret;
			}
			else if (parent == oldparent && ! flags & AT_FORCE)
			{
				ret.code = UI_OK;
				return ret;
			}
			if (parent != nullptr)
			{	
				if (child->pos == nullptr)
					child->pos = new int2(0, 0);
				if(child->parent != nullptr )
					child->parent->children.erase(child->ID);
				child->parent = parent;
				parent->children.insert(make_pair(child->ID, child));
				if (!(flags & AT_NULL))
				{
					if (flags & AT_VINIT)
					{
						uni2<float> surf = parent->size;
						uni2<float> pos = child->proppos;
						if (flags & AT_PROPPOS)
						{
							if (child->pos == 0)
								pos = uni2<float>(0, 0);
							else 
								pos = ((child->parent == nullptr)? GAME::camrect.second().touni2<float>() : oldparent->size) / child->pos->touni2<float>();
							
						}
						else if (flags & AT_SAMEPOS)
						{
							surf = GAME::camrect.second().touni2<float>();
						}
						else if (flags & AT_STYLEPROPPOS)
						{
							pos = child->defpos;
						}
						if (!(visflags & WF_POSBASELEFT) && !(visflags & WF_POSBASERIGHT))
						{
							if (parent->flags & WF_POSBASERIGHT)
								visflags |= WF_POSBASERIGHT;
							else
								visflags |= WF_POSBASELEFT;
						}
						child->initvis(styles[child->styleid], pos,surf, visflags);
					}
				}
				if (deloldparent)
					delete oldparent;
				child->updatenesting();
				//child->updatepos();
	/*			if (!(flags & AT_NULL))
				{
					if (flags & AT_VINIT)
					{
						if (flags & AT_PROPPOS)
						{
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
						child->initvis(styles[child->ID], child->defpos, GAME::camrect.second().touni2<float>()*child->size, visflags);
					}
					else
					{
						if (flags & AT_PROPPOS)
						{

						}
					}
				}
				child->updatepos();*/
			}
			else if (flags & AT_VINIT)
			{
				child->initvis(styles[child->styleid], child->defpos, GAME::camrect.second().touni2<float>(),visflags);
			}
			ret.code = UI_OK;
			string attachfunc;
			if(parent != nullptr)
				if ((attachfunc = /*UI->styles[parent->styleid].attachtof*/parent->handleattachto) != "")
				{
					pycall* call = new pycall();
					call->createstrid(UI);
					call->c = UI;
					call->scriptloc = UI->bslink + "scripts\\" + attachfunc;
					call->args.push_back((void*)parent);
					call->args.push_back((void*)child);
					call->cancontinue = false;
					call->args.push_back(flags);
					call->args.push_back(visflags);
					GAME::UI->argmodmutex->lock();
					calls.push_back(call);
					
					GAME::UI->argmodmutex->unlock();
				}
			if ((attachfunc = /*UI->styles[child->styleid].attachtof*/child->handleattachto) != "")
			{
				pycall* call = new pycall();
				call->createstrid(UI);
				call->c = UI;
				call->scriptloc = UI->bslink + "scripts\\" + attachfunc;
				call->args.push_back((void*)child);
				call->args.push_back((void*)parent);
				call->cancontinue = false;
				call->args.push_back(flags);
				call->args.push_back(visflags);
				GAME::UI->argmodmutex->lock();
				calls.push_back(call);
				GAME::UI->argmodmutex->unlock();

			}

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
		void window::updatepos()
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
					*posvec[ii].first = posvec[ii].second + *pos + ((parent != nullptr)?((parent->flags & WF_POSBASELEFT)?*parent->pos:*parent->pos-parent->realsize):int2(0,0));
					ii++;
				}
			}
			else
			{
				int i = 0;
				while (i < children.size())
				{
					children[i]->updatepos();
					i++;
				}
			}
		}
		UIresult window::initvis(style& s, uni2<float> npos, uni2<float> screenmultip, ulli flags )
		{
			UIresult ret;
			proppos = npos;
			//int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
			uni2<float>wsize = screenmultip;
			int i = 0;
			pos = new int2((npos*wsize).toint2());
			//if (flags & WF_POSBASERIGHT)
			//{
			//	pos->x = wsize.x - pos->x;
			//}
			//if (flags & WF_SCALETO_VH || flags == NULL)
			//{
			//	pos = new int2((npos*wsize.touni2<float>()).toint2());
			//}
			//else if (flags & WF_SCALETO_V)
			//{
			//	pos = new int2((npos*wsize.x).toint2());
			//}
			//else if (flags & WF_SCALETO_H)
			//{
			//	pos = new int2((npos*wsize.y).toint2());
			//}
			defmultip = screenmultip;
			i = 0;
			if (identp == nullptr)
			{
				identp = new bool(true);
			}
			*identp = true;
			ui start = f->brushes.size();
			{
				uni2<float> mf;
				if (flags & WF_SCALETO_VH || flags == NULL)
				{
					mf.x = ((float)wsize.x)*size.x;
					mf.y = ((float)wsize.y)*size.y;
				}
				else if (flags & WF_SCALETO_V)
				{
					mf.x = ((float)wsize.y)*size.x;
					mf.y = ((float)wsize.y)*size.y;
				}
				else if (flags & WF_SCALETO_H)
				{
					mf.x = ((float)wsize.x)*size.x;
					mf.y = ((float)wsize.x)*size.y;
				}
				realsize = mf.toint2();
			}
			while (i < s.shapes.size())
			{
				bool* rb = new bool(true);
				window::subshape subsh;
				subsh.sf = sf;
				if (s.shapes[i].hassprite)
				{
					sprite sp = s.shapes[i].ico;
					//sp.SetOffsetXYp(pos, false);
					sp.render = rb;
					sp.useidentp = true;
					sp.identp = identp;
					int2* i2sp;
					if (/*s.shapes[i].lockh || s.shapes[i].lockv*/true)
					{
						uni2<float> mf;
						if (flags & WF_SCALETO_VH || flags == NULL)
						{
							mf.x = ((float)wsize.x)*size.x;
							mf.y = ((float)wsize.y)*size.y;
						}
						else if (flags & WF_SCALETO_V)
						{
							mf.x = ((float)wsize.y)*size.x;
							mf.y = ((float)wsize.y)*size.y;
						}
						else if (flags & WF_SCALETO_H)
						{
							mf.x = ((float)wsize.x)*size.x;
							mf.y = ((float)wsize.x)*size.y;
						}
						uni2<float> rmfp, rmfs;
						if (s.shapes[i].lockh)
							rmfs.x = ((float)wsize.x)*s.shapes[i].size.x;
						else
							rmfs.x = s.shapes[i].size.x * mf.x;
						rmfp.x = s.shapes[i].pos.x * mf.x;
						if (s.shapes[i].lockv)
							rmfs.y = ((float)wsize.y)*s.shapes[i].size.y;
						else
							rmfs.y = s.shapes[i].size.y * mf.y;
						rmfp.y = s.shapes[i].pos.y * mf.y;
						if (flags & WF_POSBASERIGHT)
							rmfp.x += screenmultip.x - rmfs.x;
							//i2sp->x += screenmultip.x-rmfs.x;
						sp.size =  int2((rmfs).toint2());
						i2sp = new int2((rmfp).toint2());
						sp.SyncPos(i2sp, false);
						//*i2sp = *i2sp + (defpos*rmf).toint2();
					}
					else
					{
						sp.size = (s.shapes[i].size * screenmultip).toint2();
						sp.SyncPos(i2sp = new int2((s.shapes[i].pos * screenmultip).toint2()), false);
					}
					sf->sprites.push_back(sp);
					posvec.push_back(make_pair(i2sp,*i2sp));
					subsh.pospairs.push_back(make_pair(i2sp, *i2sp));
					subsh.sprites.push_back(&sf->sprites[sf->sprites.size() - 1]);

				}
				if (s.shapes[i].col != NULL || s.shapes[i].seccol != NULL)
				{
					brush b;
					shaperb.push_back(rb);
					delete b.offset;
					b.offset = pos;
					b.useidentp = true;
					b.identp = identp;
					if (/*s.shapes[i].lockh || s.shapes[i].lockv*/true)
					{
						uni2<float> mf;
						if (flags & WF_SCALETO_VH || flags == NULL)
						{
							mf.x = ((float)wsize.x)*size.x;
							mf.y = ((float)wsize.y)*size.y;
						}
						else if (flags & WF_SCALETO_V)
						{
							mf.x = ((float)wsize.y)*size.x;
							mf.y = ((float)wsize.y)*size.y;
						}
						else if (flags & WF_SCALETO_H)
						{
							mf.x = ((float)wsize.x)*size.x;
							mf.y = ((float)wsize.x)*size.y;
						}
						uni2<float> rmfp, rmfs;
						if (s.shapes[i].lockh)
							rmfs.x = ((float)wsize.x)*s.shapes[i].size.x;
						else
							rmfs.x = s.shapes[i].size.x * mf.x;
						rmfp.x = s.shapes[i].pos.x * mf.x;
						if (s.shapes[i].lockv)
							rmfs.y = ((float)wsize.y)*s.shapes[i].size.y;
						else
							rmfs.y = s.shapes[i].size.y * mf.y;
						rmfp.y = s.shapes[i].pos.y * mf.y;
						if (flags & WF_POSBASERIGHT)
							rmfp.x += screenmultip.x - rmfs.x;
							//b.pos->x -= rmfs.x;
						b.size = new int2(( rmfs).toint2());
						b.pos = new int2((rmfp).toint2());
						//*b.pos = *b.pos + (defpos*wsize.touni2<float>()).toint2();
					}
					else
					{
						b.size = new int2((s.shapes[i].size * screenmultip).toint2());
						b.pos = new int2((s.shapes[i].pos * screenmultip).toint2());
					}
					posvec.push_back(make_pair(b.pos, *b.pos));
					if (s.shapes[i].shape == 0)
					{
						//b.size = new int2((s.shapes[i].size * screenmultip).toint2());
						b.createrectfrompossize = true;
					}
					else
						b.elipse.radiusX = b.elipse.radiusY = s.shapes[i].radius;
					//b.breakonrender = true;
					b.SetType(solidbrush);
					auto mcol = s.shapes[i].col->toColorF();
					if (s.shapes[i].col->in)
						(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].col->toColorF(), &b.b.solidbrush->first);
					else
						b.b.solidbrush->first = nullptr;
					if ((s.shapes[i].seccol != nullptr) ? s.shapes[i].seccol->out : s.shapes[i].col->out)
					{
						if (s.shapes[i].seccol != nullptr)
							(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].seccol->toColorF(), &b.b.solidbrush->second);
						else
							(*coreptr->cam->GetRenderTargetP())->CreateSolidColorBrush(s.shapes[i].col->toColorF(), &b.b.solidbrush->second);
					}
					else
						b.b.solidbrush->second = nullptr;
					b.renderp = rb;
					f->brushes.push_back(b);
					subsh.brushes.push_back(&f->brushes[f->brushes.size() - 1]);
				}
				i++;
				subsh.bt = bt;
				shpb.push_back(subsh);
			}
			i = start;
			{
				int ii = 0;
				while (i < f->brushes.size())
				{
					//auto pir = make_pair(findkeybyvalue(s.colnodemap, s.shapes[i].col), f->brushes.size()-1);
					auto pir = make_pair(findkeybyvalue(s.colnodemap, s.shapes[ii].col), f->brushes[i].b.solidbrush);
					colnodes.insert(pir);
					ii++;
					i++;
				}
			}
			i = 0;
			con.init();
			con.GetPhysP()->offsetp = pos;
			while (i < s.boxes.size())
			{
				uni2<float> mf;
				if (flags & WF_SCALETO_VH || flags == NULL)
				{
					mf.x = ((float)wsize.x)*size.x;
					mf.y = ((float)wsize.y)*size.y;
				}
				else if (flags & WF_SCALETO_V)
				{
					mf.x = ((float)wsize.y)*size.x;
					mf.y = ((float)wsize.y)*size.y;
				}
				else if (flags & WF_SCALETO_H)
				{
					mf.x = ((float)wsize.x)*size.x;
					mf.y = ((float)wsize.x)*size.y;
				}
				//uni2<float> rmf = mf;
				//if (s.boxes[i].sh.lockh)
				//	rmf.x = mf.x;
				//if (s.boxes[i].sh.lockv)
				//	rmf.y = mf.y;
				uni2<float> rmfp, rmfs;
				if (s.boxes[i].sh.lockh)
					rmfs.x = ((float)wsize.x)*s.boxes[i].sh.size.x;
				else
					rmfs.x = s.boxes[i].sh.size.x * mf.x;
				rmfp.x = s.boxes[i].sh.pos.x * mf.x;
				if (s.boxes[i].sh.lockv)
					rmfs.y = ((float)wsize.y)*s.shapes[i].size.y;
				else
					rmfs.y = s.boxes[i].sh.size.y * mf.y;
				rmfp.y = s.boxes[i].sh.pos.y * mf.y;
				if (flags & WF_POSBASERIGHT)
					rmfp.x += screenmultip.x - rmfs.x;
					//pos->x += screenmultip.x - rmfs.x;
				int2* pos = new int2((rmfp).toint2());
				//*pos = *pos + (defpos*wsize.touni2<float>()).toint2();
				posvec.push_back(make_pair(pos,*pos));
				int2 bsize = rmfs.toint2();
				simpleshape sh = s.boxes[i].sh;
				sh.pos = *pos;
				sh.size = bsize;
				textclass tc;
				tc.init(); 
				//if (flags & WF_POSBASERIGHT)
				//	size = int2(size.y, size.x);
				con.addbutton("", pos, bsize, nullptr, f, coreptr->cam, false, tc);
				button* butt = con.latestcreation;
				butt->renderingoff();
				butt->anyvars.push_back(this);
				butt->anyvars.push_back(i);
				butt->anyvars.push_back(s.boxes[i].inputmap);
				butt->anyvars.push_back(UI->bslink);
				butt->callpfunc = true;
				butt->callpyscript = true;
				btts.push_back(butt);
				//con.addbutton(butt);
				i++;
			}
			if(s.boxes.size()  > 0)
				bt = btts[0];
		//	con.buttons = btts;
			i = 0;
			while (i < s.subs.size())
			{
				window* wnd = new window;
				i++;
			}
			i = 0;
			updatepos();
			ret.code = UI_OK;
			f->ismactive = true;
			vector<boost::any> v;
			/*		window* wnd = boost::any_cast<window*>(bt->anyvars[0]);
			auto m = boost::any_cast<>bt->anyvars[2]*/;
			v.push_back(ID);
			map<string, ui>inputs = { { "LBUTTONUP",WM_LBUTTONUP },{ "RBUTTONUP",WM_RBUTTONUP },{ "LBUTTONDOWN",WM_LBUTTONDOWN },{ "RBUTTONDOWN",WM_RBUTTONDOWN },
			{ "MBUTTONUP" ,WM_MBUTTONUP },{ "MBUTTONDOWN",WM_MBUTTONDOWN },{ "MOUSEMOVE",WM_MOUSEMOVE } };
			//vector<ui> inpotsuint = { };
			i = 0;
			string inputstr;
		feachdone:;
			string msgf = inputstr;
			string strid = s.flagproc + to_string(ID);
			wstring wstr0 = STRtoWSTR(strid);
			wchar_t * ch0 = new wchar_t[wstr0.size() + 1];
			wcsncpy(ch0, wstr0.c_str(), wstr0.size() + 1);
			wstring wstr1 = STRtoWSTR(coreptr->bslink);
			wchar_t * ch1 = new wchar_t[wstr1.size() + 1];
			wcsncpy(ch1, wstr1.c_str(), wstr1.size() + 1);
			wchar_t* _args[] = { ch0, ch1 };
			int argc = sizeof(_args) / sizeof(_args[0]);
			vector<wchar_t*> w_tv = { ch0,ch1 };
			v.push_back(w_tv);
			//while (UI->isargbmodified)
			//	Sleep(0);
			//UI->isargbmodified = true;
			UI->argmodmutex->lock();
			v.push_back(coreptr->bslink);
			UI->args.insert(make_pair(strid, v));
			UI->argmodmutex->unlock();
			UI->idmapmutex.lock();
			if (MapFind(UI->processtridmap, getpid()))
				UI->processtridmap.erase(getpid());
			UI->processtridmap.insert(make_pair(getpid(), strid));
			UI->idmapmutex.unlock();
			//UI->isargbmodified = false;

			//PySys_SetArgv(argc, _args);
			int ii = 0;
			if (s.flagproc != "")
			{
				string loc = coreptr->bslink + "scripts\\" + s.flagproc;
				FILE* file = _Py_fopen(loc.c_str(), "r+");
				auto pyret = PyRun_AnyFileEx(file, s.flagproc.c_str(), true);
				UI->idmapmutex.lock();
				UI->processtridmap.erase(getpid());
				UI->idmapmutex.unlock();
				auto fuckvs = UI;
				UI->argmodmutex->lock();
				auto bargs = UI->args[strid];
				auto fargs = boost::any_cast<vector<wchar_t*>>(bargs[(bargs.size() < 10) ? 1 : 8]);
				int i = 0;
				while (i < fargs.size())
				{
					delete[] fargs[i];
					i++;
				}
				if (bargs.size() > 10)
				{
					*boost::any_cast<bool*>(bargs[13]) = true;
				}
				UI->argmodmutex->unlock();
			}
			UI->argmodmutex->lock();
			UI->args.erase(strid);
			UI->argmodmutex->unlock();
			return ret;
		}
		UIresult window::scale(int2 nsize)
		{
			return UIresult();
		}
		UIresult GAME::GUI::window::resize(uni2<float> npos, uni2<float> screenmultip, bool copypointers)
		{
			UIresult ret;
			ret.code = UI_OK;
			int i = 0;
			otherobjts = {};
			nooffpos = {};
			cufoffsets = {};
			if (copypointers)
			{
				
				initvis(coreptr->styles[styleid], npos, screenmultip, flags);
				i = 0;
				posvec = {};
				defshapepossize = {};
				btts = {};
				shaperb = {};
				for each (auto p in children)
				{
					coreptr->AttachTo(this, p.second, AT_VINIT | AT_FORCE | AT_PROPPOS);
					colnodes = {};
				}
				
				return ret;
			}
			while (i < posvec.size())
			{
				delete posvec[i].first;
				i++;
			}
			i = 0;
			posvec = {};
			defshapepossize = {};
			while (i < btts.size())
			{
				delete btts[i];
				i++;
			}
			btts = {};
			i = 0;
			for each (auto var in colnodes)
			{
				delete var.second;
			}
			colnodes = {};
			i = 0;
			while (i < shaperb.size())
			{
				delete shaperb[i];
				i++;
			}
			shaperb = {};
			delete pos;
			return ret;
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
		UIresult core::NewWindow(window * parent, uni2<float> pos, uni2<float> size, ui styleid, unsigned long int flags, string strname)
		{
			UIresult ret;
			style s = styles[styleid];
			int i = 0;
			i = 0;
			window* w = new window;
			w->styleid = styleid;
			w->strname = strname;
			w->flags = flags;
			w->coreptr = this;
			w->handleattachto = s.attachtof;
			if (s.name == "SCROLLABLE" || s.name == "scrollable")
				pass;
			do
			{
				w->ID = rand();
			} while (MapFind(wnds, w->ID));
			wnds.insert(make_pair(w->ID,w));
			w->f = new frame;
			mainframe->f["main"].push_back(w->f);
			mainframe->wchiac = "main";
			mainframe->ischfact = true;
			w->f->ismactive = true;
			w->f->ischfact = true;
			frame* f2 = new frame;
			w->f->f.insert(make_pair("spriteframe", vector<frame*>{f2}));
			w->f->wchiac = "spriteframe";
			w->sf = f2;
			f2->ismactive = true;
			uni2<float> mf;
			int2 wsize = { GAME::camrect.z - GAME::camrect.x, GAME::camrect.w - GAME::camrect.y };
			w->defpos = pos;
			mf.x = ((float)wsize.x)*size.x;
			mf.y = ((float)wsize.y)*size.y;
			w->size = size;
			MapSet(UI->wndnamemap, make_pair(strname, w->ID));
			if (!(flags & WF_NOVINIT))
			{
				if (parent == nullptr)
					ret = w->initvis(s, pos, wsize.touni2<float>(), flags);
				else
					ret = AttachTo(parent, w, AT_VINIT | AT_FORCE | AT_PROPPOS, flags);
			}
			if (flags & WF_HIDE)
				w->hide();
			else
				w->show();
			ret.retval.push_back(w);
			ret.retval.push_back(w->ID);
			string strid = to_string(w->ID);
			return ret;
		}
		UIresult core::NewWindow(window * parent, uni2<float> pos, uni2<float> size, string stylename, unsigned long int flags,string strname)
		{
			return NewWindow(parent, pos, size, styleids[stylename], flags,strname);
		}
		UIresult core::init(controls * con, camera * ncam, frame * mf, ui scenen, string link)
		{
			int i = 0;
			bslink = link;
			PyImport_AppendInittab("GUI", Python::GUIMOD::PyInit_GUI);
			PyImport_AppendInittab("AZflib", Python::AZFLIBMOD::PyInit_AZflib);
			PyImport_AppendInittab("GAPI", GUI::Python::GAPIMOD::PyInit_GAPI);
			//PyEval_InitThreads();
			Py_Initialize();
			wstring wstr0 = STRtoWSTR((bslink + "scripts\\"));
			wchar_t * ch0 = new wchar_t[wstr0.size() + 1];
			wcsncpy(ch0, wstr0.c_str(), wstr0.size() + 1);
			wchar_t* _args[] = { ch0 };
			PySys_SetArgv(1, _args);
			PyImport_ImportModule("GUI");
			PyImport_ImportModule("AZflib");
			PyImport_ImportModule("GAPI");
			conptr = con;
			mainframe = mf;
			scenenum = scenen;
			UIresult result;
			result.code = UI_OK;
			cam = ncam;
			scriptthreadmutex.lock();
			bool* rb = nullptr;				
			mutex m;
			UI->_ulm = unique_lock<std::mutex>(m);
			curth = new thread(pythreadqueue,rb = new bool(false));
			scriptthreads.push_back(make_pair(curth, rb));
			scriptthreadmutex.unlock();
			curth->detach();
			window* rootptr;
			wnds.insert(make_pair(1, rootptr = new window));
			rootptr->isnull = true;
			wnds.insert(make_pair(0, nullptr));
			Sleep(10);
			return result;
		}
		void core::reinit()
		{
			Py_FinalizeEx();
			PyImport_AppendInittab("GUI", Python::GUIMOD::PyInit_GUI);
			PyImport_AppendInittab("AZflib", Python::AZFLIBMOD::PyInit_AZflib);
			Py_Initialize();
			PyImport_ImportModule("GUI");
			PyImport_ImportModule("AZflib");
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
			c->bslink = binloc;
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
			stylef = f;
			auto iconlocs = f.GetVarNamesInNode("ICONS@");
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
						cn->in = pn->getvar<bool>("IN");
						cn->out = pn->getvar<bool>("OUT");
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
					simsh.lockh = sh->getvar<bool>("LOCKH");
					simsh.lockv = sh->getvar<bool>("LOCKV");
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
						simsh.ico = icons[str2];
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
					b.message = f.GetVar<string>(str + "BOX" + common::INTtoSTR(ii) + "@MSG");
					while (iii < inputs.size())
					{
						string str10000;
						if (f.GetVarSafe(str10000, (str2 + inputs[iii] + "@") + "MSG"))
							b.inputmap.insert(make_pair(inputs[iii], str10000));
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
					string deftext = "";
					if (f.findvar(str + "@TEXTBOX" + common::INTtoSTR(ii) + "@DEFTEXT"))
						deftext = f.GetVar<string>(str + "TEXTBOX" + common::INTtoSTR(ii) + "@DEFTEXT");
					tp->changetext(deftext);
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
				try
				{
					map<string, bool> flags;
					auto v = f.GetVarNamesInNode(str + "FLAGS@");
					ii = 0;
					while (ii < v.size())
					{
						flags.insert(make_pair(v[ii], f.GetVar<bool>(str + "FLAGS@" + v[ii])));
						ii++;
					}
					map<string, bool> falseflags;
					st.defaultflags = flags;
					BOOST_FOREACH(auto it, flags)
					{
						falseflags.insert(make_pair(it.first, false));
					}
				}
				catch (...)
				{

				}
				try
				{
					ii = 0;
					auto v = f.GetVarNamesInNode(str + "SCRIPTF@MSG@");
					while (ii < v.size())
					{
						st.msgproc.insert(make_pair(v[ii], f.GetVar<string>(str + "SCRIPTF@MSG@" + v[ii])));
						ii++;
					}
				}
				catch(...){}
				ii = 0;
				f.GetVarSafe(st.flagproc, str + "SCRIPTF@FLAGPROC");
				f.GetVarSafe(st.initproc, str + "SCRIPTF@INITPROC");
				f.GetVarSafe(st.attachtof, str + "SCRIPTF@ATTACHTO");
				f.GetVarSafe(st.procondefault, str + "DEFUSEFPROC");
				addstyle(st,names[i]);
				i++;
			}
			ret.code = UI_OK;
			return ret;
		}
	}
}