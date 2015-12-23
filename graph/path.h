#ifndef __GRAPH_PATH__
#define __GRAPH_PATH__

#include "traverse.h"


template<class G>
class CGraphPath : public CGraphBFS<G>
{
public:
	// Find a path excluding the passed edge
	CGraphPath(const G& f_g, const edge_t& f_e):
		CGraphBFS<G>(f_g),
		m_exclude(f_e)
	{}
	virtual ~CGraphPath() {}

protected:
	virtual void push_if(fringe_bfs_t& f_s, unsigned f_v, unsigned f_w)
	{
		// Exclude the edge
		if((m_exclude.first  == f_v && m_exclude.second == f_w) ||
		   (m_exclude.second == f_v && m_exclude.first  == f_w))
			return;
		CGraphBFS<G>::push_if(f_s, f_v, f_w);
	}

private:
	const edge_t m_exclude;
};

#endif // __GRAPH_PATH__

