/**
 * A Maze Solver
 *
 * A maze is traversed using the DFS to find the path
 */
#include <iostream>

#include <vector>
#include <stack>
#include <set>


struct Coord
{
	unsigned x;
	unsigned y;
	Coord(): x(0), y(0) {}
	Coord(unsigned f_x, unsigned f_y): x(f_x), y(f_y) {}
	bool operator<(const Coord& c) const { return (x < c.x || (x == c.x && y < c.y)); }
};


class Maze
{
private:
	// Basic grid
	class Grid
	{
	public:
		Grid(const bool* mat, unsigned cx, unsigned cy):
			m_cells(mat, mat + cx * cy),
			m_cx(cx), m_cy(cy)
		{}
		// Init the grid with predefind points (for a path printing)
		Grid(const std::vector<Coord>& points, unsigned cx, unsigned cy):
			m_cells(cx * cy),
			m_cx(cx), m_cy(cy)
		{
			for(unsigned i = 0, n = points.size(); i < n; ++i)
			{
				const Coord& c = points[i];
				m_cells[c.y * m_cx + c.x] = true;
			}
		}
		bool at(unsigned x, unsigned y) const { return (x < m_cx && y < m_cy && m_cells[y * m_cx + x]); }
		void reset(unsigned x, unsigned y) { m_cells[y * m_cx + x] = false; }

		unsigned width () const { return m_cx; }
		unsigned height() const { return m_cy; }

	protected:
		std::vector<bool> m_cells;
		unsigned m_cx, m_cy;
	};
	// A grid with marked intersections
	class CrossGrid : public Grid
	{
	public:
		CrossGrid(const Grid& g):
			Grid(g),
			m_crosses(m_cx * m_cy)
		{
			for(unsigned i = 0, n; i < m_cy; i++)
			{
				for(unsigned j = 0; j < m_cx; j++)
				{
					if( !at(j, i) )
						continue;
					unsigned n = (unsigned)at(j    , i - 1) +
								 (unsigned)at(j    , i + 1) +
								 (unsigned)at(j - 1, i    ) +
								 (unsigned)at(j + 1, i    );
					if(n > 2)
						m_crosses[i * m_cx + j] = true;
				}
			}
		}
		bool cross(unsigned x, unsigned y) const { return m_crosses[y * m_cx + x]; }

	private:
		std::vector<bool> m_crosses;
	};

public:
	Maze(const bool* f_data, unsigned cx, unsigned cy,
		 unsigned fromX, unsigned fromY,
		 unsigned toX, unsigned toY):
		m_grid(f_data, cx, cy),
		m_from(fromX, fromY),
		m_to(toX, toY)
	{
		CrossGrid g(m_grid);

		/**
		 * Traverse using the DFS.
		 * The last element in the path corresponds to the current position.
		 *
		 * Note, that it's enough to use a stack here, however
		 * a vector allows to keep the non-reversed visit order.
		 */
		m_path.push_back(m_from);
		for(unsigned x = fromX, y = fromY; !(x == toX && y == toY);)
		{
			g.reset(x, y);

			if( g.at(x - 1, y) )
				--x;
			else if( g.at(x + 1, y) )
				++x;
			else if( g.at(x, y - 1) )
				--y;
			else if( g.at(x, y + 1) )
				++y;
			else
			{
				// Unwind until the latest cross
				for(m_path.pop_back(); !m_path.empty(); m_path.pop_back())
				{
					x = m_path.back().x;
					y = m_path.back().y;
					if( g.cross(x, y) )
						break;
				}
				// Stop if nowhere to return
				if(m_path.empty())
					break;
				continue;
			}

			m_path.push_back( Coord(x, y) );
		}
	}

	/*explicit*/ operator bool() const { return !m_path.empty(); }

	void print() const { print(NULL); }
	void printPath() const
	{
		if(m_path.empty())
			std::cout << "Not found" << std::endl;
		else
		{
			Grid pg(m_path, m_grid.width(), m_grid.height());
			print(&pg);
		}
	}

private:
	void print(const Grid* pPathGrid) const
	{
		for(unsigned i = 0, li = m_grid.height() - 1; i <= li; ++i)
		{
			for(unsigned j = 0, lj = m_grid.width() - 1; j <= lj; ++j)
			{
				if( m_grid.at(j, i) )
				{
					if(j == m_from.x && i == m_from.y)
						std::cout << 'S';
					else if(j == m_to.x && i == m_to.y)
						std::cout << 'F';
					else if(pPathGrid && pPathGrid->at(j, i))
						std::cout << 'X';
					else
						std::cout << '+';
				}
				else if(!i || !j || i == li || j == lj)
					std::cout << '#';
				else
					std::cout << ' ';
			}
			std::cout << std::endl;
		}
	}

private:
	const Grid m_grid;
	Coord m_from, m_to;

	std::vector<Coord> m_path;
};

// ====================================
int main(int, const char**)
{
	bool data[] =
	{
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
		0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0,
		0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
		0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0,
		0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
		0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0,
		0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	Maze maze(data, 19, 19, 1, 0, 18, 17);

	std::cout << "Initial:" << std::endl;
	maze.print();
	std::cout << std::endl;

	std::cout << "Path:" << std::endl;
	maze.printPath();

	return 0;
}

