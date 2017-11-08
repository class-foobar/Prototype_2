#include "pathfinding.h"
namespace GAME
{
	namespace NBPF
	{
		path pathsystem::FindRoute(path pr, pathnode* target, map<int2, void*>pp)
		{
			vector<int2> v;
			map<int2, void*> m;
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ 1 * griddensity,0 });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ 1 * griddensity,1 * griddensity });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ -1 * griddensity,0 });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ 0,1 * griddensity });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ -1 * griddensity,-1 * griddensity });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ 1 * griddensity,-1 * griddensity });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ -1 * griddensity,1 * griddensity });
			v.push_back(pr.nodes[pr.nodes.size() - 1].first + int2{ 0,-1 * griddensity });
			int i = 0;
			m = VecToMapnptr<int2>(v);
			i = 0;
			path shortest;
			path shortestlpath;
			vector<path> pas;
			while (i < v.size())
			{
				if (MapFind(nodepmap, v[i]))
				{
					if (Distance(v[i], target->pos) <= griddensity)
					{
						pr.nodes.push_back({ v[i],nodepmap[v[i]] });
						pr.nodes.push_back({ target->pos,target });
						return pr;
					}
					if (nodepmap[v[i]]->type != nodetype::unpassable && nodepmap[v[i]]->type != nodetype::null)
					{
						path prc = pr;
						if (nodepmap[v[i]]->type != nodetype::notpreferred)
							prc.lpath = true;
						prc.nodes.push_back({ v[i],nodepmap[v[i]] });
						pas.push_back(FindRoute(prc, target, m));
						if (!prc.lpath &&shortest.nodes.size() == 0 && prc.nodes.size() != 0)
							shortest = prc;
						else if (prc.lpath &&shortestlpath.nodes.size() == 0 && prc.nodes.size() != 0)
							shortestlpath = prc;
					}
				}
				i++;
			}
			if (pas.size() == 0)
				return{};
			i = 0;
			while (i < pas.size())
			{
				if (pas[i].nodes.size() == 0)
				{
					i++;
					continue;
				}
				if (pas[i].lpath)
				{
					if (shortestlpath.nodes.size() == 0)
						shortestlpath = pas[i];
					if (shortestlpath.nodes.size() > pas[i].nodes.size())
						shortestlpath = pas[i];

				}
				if (shortest.nodes.size() > pas[i].nodes.size())
				{
					shortest = pas[i];
				}
				i++;
			}
			i = 0;
			if (shortest.nodes.size() == 0 && shortestlpath.nodes.size() == 0)
				return{};
			if (shortest.nodes.size() != 0)
				return shortest;
			return shortestlpath;
		}
		void pathsystem::AddGrid(int2 size, int2 pos, nodetype nt = nodetype::passable, int skipX, int skipY)
		{
			int i = 0;
			while (i < size.x)
			{
				i += skipX;
				if (!(i <= size.x))
					break;
				int ii = 0;
				while (ii < size.y)
				{
					ii += skipY;
					if (!(ii <= size.y))
						break;
					if (MapFind(nodepmap, int2(i + pos.x, ii + pos.y)))
					{
						if (nodepmap[int2(i + pos.x, ii + pos.y)] != NULL)
						{
							nodepmap[int2(i + pos.x, ii + pos.y)]->type = nt;
							ii++;
							continue;
						}
					}
					pathnode* pn = new pathnode;
					nodepmap.insert(make_pair(pn->pos = int2(i + pos.x, ii + pos.y), pn));
					nodes.push_back(pn);
					pn->ps = this;
					pn->type = nt;
					ii++;
				}
				i++;
			}
		}
		void pathsystem::AddPoint(int2 pos, nodetype nt = nodetype::passable)
		{
			pathnode* pn = new pathnode;
			pn->pos = pos;
			pn->type = nt;
			pn->ps = this;
			nodepmap.insert(make_pair(pos, pn));
			nodes.push_back(pn);
		}
		path pathsystem::FindPath(int2 tpos, int2 spos, bool createnewnode)
		{
			pathnode* tpn = nullptr;
			pathnode* spn = nullptr;
			bool wtc = false;
			bool wsc = false;
			if (MapFind(nodepmap, tpos))
				tpn = nodepmap[tpos];
			else
			{
				wtc = true;
				tpn = new pathnode;
				tpn->pos = tpos;
				tpn->ps = this;
				tpn->type = nodetype::passable;
			}
			if (MapFind(nodepmap, spos))
				spn = nodepmap[spos];
			else
			{
				wsc = true;
				spn = new pathnode;
				spn->pos = spos;
				spn->ps = this;
				spn->type = nodetype::passable;
			}
			path p;
			int i = 0;
			if (Distance(tpos, spos) < griddensity)
			{
				if (createnewnode)
				{
					if (wtc)
						nodes.push_back(tpn);
					if (wsc)
						nodes.push_back(spn);
					if (wtc)
						nodepmap.insert(make_pair(tpos, tpn));
					if (wsc)
						nodepmap.insert(make_pair(spos, spn));
				}
				else
				{
					if (wtc)
					{
						delete tpn;
						tpn = nullptr;
					}
					if (wsc)
					{
						delete spn;
						spn = nullptr;
					}
				}
				p.nodes.push_back({ spos,spn });
				p.nodes.push_back({ tpos,tpn });
				return p;
			}
			else
			{
				p.nodes.push_back({ spos,spn });
				p = FindRoute(p, tpn, { {spos,nullptr} });
			}
			if (createnewnode)
			{
				if (wtc)
					nodes.push_back(tpn);
				if (wsc)
					nodes.push_back(spn);
				if (wtc)
					nodepmap.insert(make_pair(tpos, tpn));
				if (wsc)
					nodepmap.insert(make_pair(spos, spn));
			}
			else
			{
				if (wtc)
					delete tpn;
				if (wsc)
					delete spn;
			}
		}
		path pathsystem::FindPath(pathnode* tnode, pathnode* snode)
		{
			return FindPath(tnode->pos, snode->pos, true);
		}
	}
	namespace PBPF
	{

	}
}