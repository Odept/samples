#ifndef __GRAPH_EULER__
#define __GRAPH_EULER__

#include "traverse.h"


class CGraphEuler
{
private:
	typedef std::vector<unsigned> path_t;

	enum PathType { PathNone, PathSimple, PathCycle };

public:
	template<class G>
	CGraphEuler(const G& f_g, unsigned f_v):
		m_iterations(0),
		m_type(PathNone)
	{
		if(f_v >= f_g.V())
			return;

		// Check for a valid path/cycle first
		m_type = calcPathType(f_g, f_v);
		if(m_type == PathNone)
			return;

		// Make a copy of the passed graph because
		// the method removes edges as they are processed.
		G g(f_g);

		/**
		 * 1. Take any simple path;
		 * 2. One by one pop visited vertices;
		 * 3. If a vertex has linked nodes left, then goto (1)
		 */
		path_t lpath;
		for(unsigned v = f_v, n; g.begin(v) != g.end(v) || !lpath.empty(); m_iterations++)
		{
			path_simple(g, v, lpath);
			for(n = lpath.size(); n;)
			{
				v = lpath[--n];
				if(g.degree(v))
					break;
				m_path.push_back(v);
			}
			lpath.resize(n);
		}
	}
	
	bool hasPath() const { return (m_type == PathSimple); }
	bool hasCycle() const { return (m_type == PathCycle); }

	unsigned length()					const { return m_path.size(); }
	unsigned operator[](unsigned f_i)	const { return m_path[f_i]; }
	unsigned iterations()				const { return m_iterations; }

private:
	/**
	 * Use BFS to check if an Euler path or cycle exists.
	 * BFS calls a callback each time it visits an unprocessed node.
	 */
	struct PathInfo
	{
		const CGraph& G;
		unsigned Odds;
		PathInfo(const CGraph& f_g): G(f_g), Odds(0) {}
	};

	template<class G>
	PathType calcPathType(const G& f_g, unsigned f_v) const
	{
		if( !f_g.degree(f_v) )
			return PathNone;

		CGraphBFS<G> bfs(f_g);
		PathInfo info(f_g);

		bfs.traverse(f_v, cbTraverse, &info);
		switch(info.Odds)
		{
			case 0:		return PathCycle;
			case 2:		return PathSimple;
			default:	return PathNone;
		}
	}
	static bool cbTraverse(void* f_param, unsigned, unsigned f_w)
	{
		PathInfo& info = *reinterpret_cast<PathInfo*>(f_param);

		if(info.G.degree(f_w) & 1)
		{
			info.Odds++;
			if(info.Odds > 2)
				return false;
		}
		return true;
	}

	// Walk along any simple path and remove edges that are used for that
	template<typename G>
	void path_simple(G& f_g, unsigned f_v, path_t& f_path) const
	{
		unsigned v = f_v;
		for(typename G::AdjIterator it = f_g.begin(v); it != f_g.end(v); it = f_g.begin(v))
		{
			f_path.push_back(v);
			v = *it;
			f_g.remove(it);
		}
		f_path.push_back(v);
	}

private:
	path_t m_path;
	unsigned m_iterations;

	PathType m_type;
};

#endif // __GRAPH_EULER__

