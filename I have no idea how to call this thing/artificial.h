#pragma once
#include "hmain.h"
#define pyobj PyObject* 
namespace GAME
{
	inline void executevatemp()
	{

	}
	template <typename unifunc, typename univar, typename... uniarg>
	inline void executevatemp(unifunc& f, univar& ref, uniarg... args)
	{
		int dummy[] = { 0, ((void)f(ref,std::forward<uniarg>(args)), 0) ... };
	}
	template<typename ... uniarg>
	inline void* tempfunc(uniarg...args)
	{
		return nullptr;
	}
	namespace AI
	{
		class enviroment;
		class AImodule;
		class mainframe;
		class IOinterface;
		class func;
		void loadAI(enviroment* env, string datafileloc);
		namespace python
		{
			class pyfile
			{
				friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
			private:
			protected:
			public:
				FILE* f;
				string filename;
				string loc;
				void create(string floc);
				pyfile() = default;
				pyfile(string floc)
				{
					create(floc);
				}
			};
			class pyfunc
			{
				friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
			private:
			protected:
			public:
				pyfile* f;
				string name;
				template <typename... uniargs>
				void* call(uniargs... args);
				void create(string loc, string name);
				template<typename... uniarg>
				void* operator()(uniarg... args)
				{
					return call(args...);
				}
				pyfunc() = default;
				pyfunc(string loc,string name)
				{
					create(loc, name);
				}
			};
			template<typename ...uniargs>
			inline void * pyfunc::call(uniargs ...args)
			{
				PyObject* modstr = PyUnicode_FromStringAndSize(f->filename.c_str(), f->filename.size());
				PyObject* mod = PyImport_Import(modstr);
				PyObject* func = PyObject_GetAttrString(mod, name.c_str());
				pyobj* obj = PyTuple_New(sizeof...(args));
				pair<pyobj*, int*>pir(obj, new int(0));
				auto f = [](pair<pyobj*, int*> &pir, auto arg)
				{
					PyTuple_SetItem(pir.first, *pir.second++, arg);
				};
				executevatemp(f, pir, args...);
				obj = pir.first;
				delete pir.second;
				return PyObject_CallObject(func, obj);
			}
		}
		class func
		{
			friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
		private:
		protected:
		public:
			python::pyfunc* pyfunc = nullptr ;
			void* (*cppfunc)(void* data) = nullptr;
			template<typename... uniarg>
			void* call(uniarg... args);
			template<typename... uniarg>
			void* operator()(uniarg... args)
			{
				return call(args...);
			}
			func() = default;
			func(void* (*f)(void* data))
			{
				cppfunc = f;
			}
			func(python::pyfunc* pyf, void* (*f)(void* data) = nullptr) 
			{
				cppfunc = f;
				pyfunc = pyf;
			}
		};
		class enviroment
		{
			friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
		private:
		protected:
			common::safemap <string, mainframe*> mframecontainer;
			common::safemap<string, AImodule*> modulecontainer;
			vector<AImodule*> modptrvec;
			vector<mainframe*> mfptrvec;
		public:
			map<string, ui> inputidmap;
			AImodule* GetModule(string name);
			mainframe* GetMainframe(string name);
		};
		class mainframe
		{
			friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
		private:
		protected:
			deque<AImodule*> modlist;
		public:
			string name = "";
			common::safemap<string, void*> memory;
			map<ui, vector<AImodule*>> mods;
			template<typename... uniarg>
			map<ui, vector<void*>> processdata(uniarg... args);
			template<typename... uniarg>
			vector<void*> processdata(ui id, uniarg...args);
			mainframe() = default;
			mainframe(const mainframe&src);
		};
		class IOinterface
		{
			friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
		private:
		protected:
			func* argprocessor = nullptr;
			func* outputprocessor = nullptr;
			func* f = nullptr;
		public:
			template<typename... uniarg>
			void* process(uniarg... args);
		};
		class AImodule
		{
			friend void GAME::AI::loadAI(enviroment* env, string datafileloc);
		private:
		protected:
			vector<IOinterface> interfaces;
			func* outputprocessor = nullptr;
		public:
			common::safemap<string, void*> memory;
			string name = "";
			template<typename... uniarg>
			void* process(uniarg... args);

		};
		template<typename ...uniarg>
		inline map<ui, vector<void*>> mainframe::processdata(uniarg ...args)
		{
			map<ui, vector<void*>> ret;
			int i = 0;
			for (auto it : mods)
			{
				vector<void*> v;
				i = 0;
				while (i < it.second.size())
				{
					v.push_back(it.second[i]->process(args...));
					i++;
				}
				ret.insert(make_pair(it.first, v));
			}
			return ret;
		}
		template<typename ...uniarg>
		inline vector<void*> mainframe::processdata(ui id, uniarg ...args)
		{
			vector<void*> ret;
			vector<AImodule*> v = mods[id];
			int i = 0;
			while (i < v.size())
			{
				ret.push_back(v[i]->process(args...));
				i++;
			}
			return ret;
		}
		template<typename ...uniarg>
		inline void * func::call(uniarg ...args)
		{
			void* ret = nullptr;
			void* ret0 = nullptr;
			void* ret1 = nullptr;
			if (pyfunc)
			{
				ret0 = pyfunc(args);
			}
			if (cppfunc)
			{
				vector<void*> v;
				auto f = [](vector<void*> v, auto arg)
				{
					if (is_pointer(arg))
						v.push_back(arg);
					else
						v.push_back(&arg);
				};
				executevatemp(f, v, args...);
				ret1 = cppfunc(v);
			}
			if (ret0 &&  ret1)
				ret = new pair<void*, void*>(ret0, ret1);
			else
				ret = (ret0) ? ret0 : ret1;
			return ret;
		}
		template<typename ...uniarg>
		inline void * IOinterface::process(uniarg ...args)
		{
			int i = 0;
			void* ret = nullptr; 
			if (argprocessor)
			{
				void* arg = argprocessor(args);
				ret = func(arg);
			}
			else
			{
				ret = func(args...);
			}
			if (outputprocessor)
				return outputprocessor(ret);
			return ret;
		}
		template<typename ...uniarg>
		inline void * AImodule::process(uniarg ...args)
		{
			void* ret = nullptr;
			int i = 0;
			vector<void*> v;
			while (i < interfaces.size())
			{
				v.push_back(interfaces[i].process(args...));
				i++;
			}
			if (outputprocessor)
				return outputprocessor(v);
			return ret = new vector<void*>(v);
		}
}
}