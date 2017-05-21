#include "ships.h"
using namespace std;
using namespace GAME;
using namespace DX2D;
namespace DX2D
{
	void faction::Release()
	{
		int i = 0;
		while (i < tokens.size())
		{
			tokens[i]->Release();
			delete tokens[i];
			i++;
		}
	}
	void faction::Delete()
	{
		int i = 0;
		fmain->facuim.erase(ID);
		while (i < fmain->facv.size())
		{
			if (fmain->facv[i]->ID == ID)
				fmain->facv.erase(fmain->facv.begin() + i);
			i++;
		}

		this->Release();
		delete this;
	}
	factoken* faction::CreateToken()
	{
		factoken* f = new factoken;
		f->fac = this;
		f->facID = ID;
		ui nID = tokens.size() - 1;
		bool b = false;
		do
		{
			nID++;
			b = false;
			int i = 0;
			while (i < tokens.size())
			{
				if (tokens[i]->facID == nID)
				{
					b = true;
				}
				i++;
			}
		} while (b);
		f->tokID = nID;
		tokens.push_back(f);
		return f;
	}
	void faction::DelteToken(ui ftID)
	{
		int i = 0;
		while (i < tokens.size())
		{
			if (tokens[i]->tokID = ftID)
			{
				tokens[i]->Release();
				delete tokens[i];
				tokens.erase(tokens.begin() + i);
				break;
			}
			i++;
		}
	}
	void factoken::Release()
	{

	}
	void factoken::Delete()
	{
		int i = 0;
		while (i < fac->tokens.size())
		{
			if (fac->tokens[i]->tokID == tokID)
			{
				fac->tokens.erase(fac->tokens.begin() + i);
				break;
			}
			i++;
		}
		this->Release();
		delete this;
	}
	faction* factions::CreateFaction(map<faction*, facrel> rel)
	{
		faction* f = new faction;
		f->relations = rel;
		int i = 0;
		bool b;
		ui ID = facv.size();
		while (MapFind(facuim, ID))
			ID++;
		f->ID = ID;
		facv.push_back(f);
		facuim.insert(make_pair(ID, f));
		return f;
	}
	//inline faction* factions::CreateFaction()
	//{
	//	return CreateFaction({});
	//}
	void factions::DeleteFaction(ui facID)
	{
		faction* f = facuim[facID];
		f->Delete();
		facuim.erase(facID);
		int i = 0;
		while (i < facv.size())
		{
			if (facv[i]->ID == facID)
			{
				facv.erase(facv.begin() + facID);
				break;
			}
			i++;
		}
		delete f;
	}
	faction* factions::GetFaction(string name) 
	{
		int i = 0;
		while (i < facv.size())
		{
			if (facv[i]->name == name)
				return facv[i];
			i++;
		}
		return nullptr;
	}
}