#ifndef __GRAPH_CC__
#define __GRAPH_CC__

#include "traverse.h"


class CConnectedComponent
{
public:
	typedef std::vector<unsigned> component_t;

public:
	template<class G>
	CConnectedComponent(const G& f_g)
	{
		unsigned V = f_g.V(), vv = 0;
		if(!V)
			return;

		CGraphDFS<G> dfs(f_g);
		CCInfo info(*this);

		for(unsigned i = 0; i < V; ++i)
		{
			if(dfs.visited(i))
				continue;

			m_components.push_back( component_t() );
			dfs.traverse(i, cb, &info);

			vv += m_components[info.Component].size();
			if(vv == V)
				break;
			info.Component++;
		}
		ASSERT(vv == V);
	}

	unsigned count() const { return m_components.size(); }
	const component_t& component(unsigned f_i) { return m_components[f_i]; }

private:
	struct CCInfo
	{
		CConnectedComponent& CC;
		unsigned Component;
		CCInfo(CConnectedComponent& f_cc): CC(f_cc), Component(0) {}
	};

	static bool cb(void* f_param, unsigned, unsigned f_w)
	{
		CCInfo& info = *reinterpret_cast<CCInfo*>(f_param);
		info.CC.add(info.Component, f_w);
		return true;
	}
	void add(unsigned f_c, unsigned f_v) { m_components[f_c].push_back(f_v); }

private:
	std::vector<component_t> m_components;
};

#endif // __GRAPH_CC__

