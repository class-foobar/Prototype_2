#pragma once
#include "hmain.h"
namespace GAME
{
	template<typename ... uniarg>
	inline void* tempfunc(uniarg...&args)
	{
		return nullptr;
	}
	namespace AI
	{
		enum generalenum
		{
			all = 0,
			none = 0
		};
		struct function;
		namespace python
		{
			struct pyfunc
			{
				function* translator = nullptr;
				pyfile* f;
				string name;
				template<typename ...uniarg>
				PyObject* call(uniarg... &args);
				template<typename ...uniarg>
				void* operator()(uniarg...&);
			};
			struct pyfile
			{
				void setf(string,bool=false);
				void close();
				pyfile(string);
				pyfile();
				FILE* f;
				template<typename ...uniarg>
				void* operator()(uniarg...&);
			};
			
		}
		class environment
		{
		private:
		protected:
			deque<mainframe*> mfvec;
			map<ui, mainframe*> mfmap;
		public:
			mainframe* createmainframe();
			mainframe* get(ui ID);
			AImodule* createmodule(mainframe* = nullptr);
		};
		struct function
		{
			python::pyfunc* pyf = nullptr;
			template<typename ... uniarg>
			void* (*f)(uniarg...&) = tempfunc;
			template<typename ... uniarg>
			void* operator()(uniarg...&args)
			{
				return (pyf == nullptr) ? f(args) : pyf(args);
			}
			template<typename ... uniarg>
			pair<void*, void*> both(uniarg...&args)
			{
				return make_pair((pyf == nullptr) ? nullptr : pyf(args), (f == nullptr) ? nullptr : f());
			}
			function();
			template<typename ... uniarg>
			function(python::pyfunc* pyf, void* (*f)(uniarg...&));
			template<typename ... uniarg>
			function( void* (*f)(uniarg...&));
			template<typename ... uniarg>
			function(python::pyfunc* pyf);

		};
		class IOinterface
		{
		private:
		protected:
			ui ID;
			vector <function*> incalls;
			vector <function*> outalls;
			vector <function*> unicalls;
		public:
			template<typename ... uniarg>
			void in(uniarg...&args);
			void* out();
		};
		class mainframe
		{
		private:
		protected:
			deque<mainframe*> mfvec;
			map<ui, mainframe*> mfmap;
			map<string, ui> stridmap;
		public:
			AImodule* get(ui ID);
			AImodule* get(string ID);
			template<typename ...uniarg>
			inline bool putin(ui modid,ui ID, uniarg...&args);
			inline void* putout(ui modid,ui ID);
			template<typename ...uniarg>
			bool putin(ui ID, uniarg...&args);
			void* putout(ui ID);
			function* tick = nullptr;
			bool both = false;
			template<typename ... uniarg>
			void* operator()(uniarg...&args)
			{
				return (!both)?tick(args):new pair(tick->both(args));
			}
		};
		class AImodule
		{
		private:
		protected:
		public:
			map<ui, IOinterface*> interfaces;
			template<typename ...uniarg>
			bool putin(ui ID, uniarg...&args);
			void* putout(ui ID);
		};
	}
}