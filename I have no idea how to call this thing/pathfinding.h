#pragma once
#include "hmain.h"
namespace GAME
{
	enum class nodetype
	{
		passable = 1,
		unpassable = 2,
		notpreferred = 3,
		null = 0
	};
	namespace NBPF
	{
		class pathnode;
		class path;
		class pathsystem;
		class pathnode
		{
		protected:

		public:
			int2 pos;
			nodetype type;
			pathsystem* ps;
		};
		class path
		{
		public:
			vector<pair<int2, pathnode*>> nodes;
			int2* cpos = nullptr;
			bool lpath = false; // low prioryty path, has  "notpreffered" nodes
			operator bool() const
			{
				return nodes.size() != 0;
			}
		};
		class pathsystem
		{
		protected:
			path FindRoute(path pr, pathnode* target, map<int2, void*>pp);
		public:
			int griddensity = 1;
			vector<pathnode*> nodes;
			map<int2, pathnode*> nodepmap;
			void AddGrid(int2 size, int2 pos, nodetype nt, int skipX = 0, int skipY = 0);
			void AddPoint(int2 pos, nodetype nt);
			path FindPath(int2 tpos, int2 spos, bool createnewnode = false);
			path FindPath(pathnode* tnode, pathnode* snode);
		};
	}
	namespace PBPF
	{

	}
}