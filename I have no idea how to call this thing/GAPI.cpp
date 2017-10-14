#include "GUI.h"
extern mutex _py_refmutex;
#define Py_INCREFTS(obj) _py_refmutex.lock(); Py_INCREF(obj); _py_refmutex.unlock()
#define Py_DECREFTS(obj) _py_refmutex.lock(); Py_DECREF(obj); _py_refmutex.unlock()
namespace GAME
{
	extern ship* conship;
	extern GUI::core* UI;
	namespace selecting
	{
		extern physics pcshipclick;
		extern crewman* scm;
		extern vector<crewman*> cmv;
		extern vector<crewman*> cmvlast;
		extern int lasti;
	}
	namespace GUI
	{
		void objectinfownd::create(window* w, entity ent)
		{
			ship* dummy = boost::any_cast<ship*>(ent.datav[0]);
			boost::any undecodedclass = ent.datav[1];
			entitytype type = boost::any_cast<entitytype>(ent.datav[2]);
			if (type == entitytype::ship)
			{
				ship* sh = boost::any_cast<ship*>(undecodedclass);
				info =
				{
					sh->pos,
					sh->name,
					0, // TODO: powerscore determination
					(int)sh->resusagept[shipres::power], // TODO: power usage determination for ships
					"unknown",
					"Ship",
					"Trust me I'm a spaceship pilot",
					"Quo",
					nullptr, // I'm pretty sure I've created a way to get this information TODO: Find it
					int2( 0,0 ) // Same as above
				};
			}
			else if (type == entitytype::statmout || type == entitytype::statmin)
			{
				station* stat = nullptr;
				void* statmod;
				if (type == entitytype::statmout)
					stat = ((stationmodOUT*)(statmod = (void*)boost::any_cast<stationmodOUT*>(undecodedclass)))->statptr;
				else if(type == entitytype::statmin || type == entitytype::statcore)
					stat = ((stationmodIN*)(statmod = (void*)boost::any_cast<stationmodIN*>(undecodedclass)))->statptr;
				info = 
				{
					stat->pos,
					stat->name,
					0, // TODO: powerscore determination
					0, // TODO: power usage determination for stations
					"unknown",
					"Space Station",
					"{Insert a catch-phrase}",
					"Passive-Agressive",
					nullptr, // I'm pretty sure I've created a way to get this information TODO: Actually find it
					int2(0,0) // Same as above
				};
			}
			wnd.w = w;
			wnd.parent = w->parent;
			wnd.name = w->strname;
			wnd.pos = w->pos;
			wnd.textwnds = {};
			vector<string> vs = {"Position: " + info.pos->str(),"Sector: " + info.systempos.str(),"Powerscore: " + to_string(info.powerscore),
				"Status: "+info.status,"Type: " + info.type,"\"" + info.infostr + "\"","Relation: "+ info.relation };
			int i = 0;
			while (i < vs.size())
			{
				UI->NewWindow(wnd.w, { 0.1f,0.05f + 0.90f / vs.size() }, { 0.8f,0.9f / vs.size() }, "TEXTBOX", (WF_SCALETO_V));
				i++;
			}
		}
	}
	namespace Python
	{
		namespace GAPIMOD
		{
			vecbreaker <PyObject*(*)(PyObject*, PyObject*)> vb;
			//----------------------------------------------------------------------------------------------------------------------------
			//MODULE: GAPI
			PYFUNC(ReleaseEntity)
			{
				PyObject* obj;
				PyArg_ParseTuple(args, "O", &obj);
				entity* ptr = (entity*)(void*)PyLong_AsVoidPtr(obj);
				delete ptr;
				Py_RETURN_TRUE;
			}
			PYFUNC(GetPhysObjEntityCopy)
			{
				physobj* ptr;
				PyObject* obj;
				PyArg_ParseTuple(args, "O", &obj);
				ptr = (physobj*)(void*)PyLong_AsVoidPtr(obj);
				int i = 0;
				entity* ent = nullptr;
				while (i < ptr->anyvars.size())
				{
					try
					{
						auto sent = boost::any_cast<entity>(ptr->anyvars[i]);
						ent = new entity(sent);
					}
					catch (...)
					{

					}
					i++;
				}
				return PyLong_FromVoidPtr((void*)ent);
			}
			PYFUNC(EntityGetArg)
			{
				PyObject* obj;
				int n;
				char* ch0;
				PyArg_ParseTuple(args, "O|i|s", &obj, &n,&ch0);
				entity* ent =  (entity*)(void*)PyLong_AsVoidPtr(obj);
				string type = ch0;
				return common::Python::CastPyObj(ent->datav[n], type);
			}
			PYFUNC(ResetInfoWnd)
			{
				Py_RETURN_TRUE;
			}
			PYFUNC(ResetChoiceWnd)
			{
				Py_RETURN_TRUE;
			}
			PYFUNC(SetInfoWnd)
			{
				Py_RETURN_TRUE;
			}
			PYFUNC(SetChoiceWnd)
			{
				Py_RETURN_TRUE;
			}
			PYFUNC(GetSelectedEntity)
			{
				return PyLong_FromVoidPtr((void*)(entity*) new auto (boost::any_cast<entity>(conship->pobj->anyvars[0])));
			}
			PYFUNC(ChangeBoolPtr)
			{
				bool* ptr;
				bool state;
				PyObject* obj;
				PyArg_ParseTuple(args, "O|b", &obj, &state);
				Py_INCREFTS(obj);
				ptr = (bool*)(void*)PyLong_AsVoidPtr(obj);
				Py_DECREFTS(obj);
				*ptr = state;
				Py_RETURN_TRUE;
			}
			PyObject* hError = NULL;
			PyMethodDef arr[__COUNTER__];
			struct PyModuleDef GAPImodule;
			PyMODINIT_FUNC PyInit_GAPI(void)
			{
				int i = 0;
				vector<PyMethodDef> moddefs;
				while (i < vb.names.size())
				{
					PyMethodDef md[] = { { vb.names[i].c_str(),vb.vec[i],METH_VARARGS,"No idea" } };
					moddefs.push_back(md[0]);
					i++;
				}
				PyMethodDef md[] = { { NULL, NULL, 0, NULL } };
				moddefs.push_back(md[0]);
				std::copy(moddefs.begin(), moddefs.end(), arr);
				GAPImodule = {
					PyModuleDef_HEAD_INIT,
					"GAPI",   /* name of module */
					NULL, /* module documentation, may be NULL */
					-1,       /* size of per-interpreter state of the module,
							  or -1 if the module keeps state in global variables. */
					arr/*&moddefsnew->operator[](0)*/
				};
				PyObject *m;
				m = PyModule_Create(&GAPImodule);
				if (m == NULL)
					return NULL;
				hError = PyErr_NewException("GAPI.error", NULL, NULL);
				Py_INCREF(hError);
				PyModule_AddObject(m, "error", hError);
				return m;
			}
		}
	}
}