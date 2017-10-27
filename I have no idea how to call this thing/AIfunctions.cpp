#include "artificial.h"
#define FUNC(x) void* x (void* data); helper x## _helper(&h,x,#x); void* x (void* data)
#define FT(x)  helper x## _helper(&h,x,#x)
class helper
{
public:
	map<string, GAME::AI::func> *fmap = nullptr;
	map<string, GAME::AI::func> sf;
	helper(map<string, GAME::AI::func>*nfm)
	{
		fmap = nfm;
		if (sf.size() > 0)
		{
			for (auto it : sf)
			{
				fmap->insert(it);
			}
		}
	}
	helper(helper*h, void* (*f)(void*), string name)
	{
		GAME::AI::func fcon(f);
		if (fmap == nullptr)
			sf.insert(make_pair(name, f));
		else 
			fmap->insert(make_pair(name, fcon));
	}
	
};
namespace GAME
{
	namespace AI
	{
		map<string, GAME::AI::func> deffunctions;
		namespace functions
		{
			helper h(&deffunctions);
			FUNC(test)
			{
				return nullptr;
			}
		}
	}
}
#undef FUNC
#undef FT