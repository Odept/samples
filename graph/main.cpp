#include <iostream>

#include "graph.h"
#include "cc.h"
#include "loop.h"
#include "euler.h"


int main(int, char**)
{
	unsigned v[][2] =
	{
		//{0,1}, {0,2}, {0,5}, {0,6}, {1,2}, {2,3}, {4,5}, {4,6}, {4,3}, {4,2}
		{0,1}, {0,2}, {0,3}, {1,3}, {2,5}, {3,6}, {4,7}, {4,8}, {5,6}, {5,8}, {5,9}, {6,7}, {6,9}
		//{0,1}, {0,2}, {0,3}, {1,3}, {1,4}, {2,5}, {2,9}, {3,6}, {4,7}, {4,8}, {5,8}, {5,9}, {6,7}, {6,9}, {7,8}
		//{1,7}, {7,6}, {3,8}, {8,5}, {5,2}, {2,4}, {4,5}, {4,8}
	};

	// Construct a graph
	unsigned v_max = 0;
	for(unsigned i = 0; i < sizeof(v) / sizeof(*v); i++)
	{
		if(v[i][0] > v_max)
			v_max = v[i][0];
		if(v[i][1] > v_max)
			v_max = v[i][1];
	}
	CEGraph g(v_max + 1);

	// Add edges
	for(unsigned i = 0; i < sizeof(v) / sizeof(*v); i++)
		g.insert(v[i][0], v[i][1]);
	std::cout << g.V() << " vertices, " << g.E() << " edges" << std::endl << g;

	// Connectivity
	bool bConnected;
	for(CConnectedComponent cc(g);;)
	{
		unsigned n = cc.count();

		if( (bConnected = (n == 1)) )
			std::cout << "The graph is connected" << std::endl;
		else
		{
			// Print components
			std::cout << n << " connected components:" << std::endl;
			for(unsigned i = 0; i < n; i++)
			{
				const CConnectedComponent::component_t& c = cc.component(i);
				std::cout << "  #" << i << ':';
				for(unsigned j = 0, nj = c.size(); j < nj; j++)
					std::cout << ' ' << c[j];
				std::cout << std::endl;
			}
		}
		break;
	}

	// Loops
	for(CGraphLoop L(g);;)
	{
		if(unsigned n = L.count())
		{
			// Print loops
			std::cout << n << " basic loops:" << std::endl;
			for(unsigned i = 0; i < n; i++)
			{
				const CGraphLoop::loop_t& l = L.loop(i);
				std::cout << "  #" << i << ':';
				for(unsigned j = 0, nj = l.size(); j < nj; j++)
					std::cout << ' ' << l[j];
				std::cout << std::endl;
			}
		}
		else
			std::cout << "The graph has no loops (tree)" << std::endl;
		break;
	}

	// Euler
	for(;bConnected;)
	{
		CGraphEuler euler(g, 0);

		// Print
		unsigned n = euler.length();

		if(euler.hasPath())
			std::cout << "Euler path(" << n << "):";
		else if(euler.hasCycle())
			std::cout << "Euler cycle(" << n << "):";
		else
		{
			std::cout << "No Euler path or cycle" << std::endl;
			break;
		}

		for(unsigned i = 0; i < n; i++)
			std::cout << ' ' << euler[i];
		std::cout << " (" << euler.iterations() << " iteration"
				  << (euler.iterations() == 1 ? "" : "s") << ")" << std::endl;
		break;
	}

	return 0;
}

