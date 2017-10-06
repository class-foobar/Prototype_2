#include "GUI.h"
namespace GAME
{
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

			}
			PYFUNC(ResetChoiceWnd)
			{

			}
			PYFUNC(GetSelectedEntity)
			{

			}
			PYFUNC(SetInfoWnd)
			{

			}
			PYFUNC(SetChoiceWnd)
			{

			}
			PYFUNC(GetSelectedEntity)
			{

			}
			PYFUNC(ChangeBoolPtr)
			{

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