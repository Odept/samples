/**
 * Multiply two big numbers that are represented as strings
 */
#include <iostream>
#include <vector>


static std::string mul(const std::string& f_x, const std::string& f_y)
{
	int lx = f_x.length();
	int ly = f_y.length();

	std::vector<int> z(lx * ly);
	// Carry for adding a result of each xy-iteration
	int cz = 0;
	// Start position for adding a result of each xy-iteration
	int k;

	for(int i = lx - 1, p = 0; i >= 0; i--, p++)
	{
		// Carry for x[i]*y multiplication
		int c = 0;

		k = p;
		for(int j = ly - 1; j >= 0; j--, k++)
		{
			c = (f_x[i] - '0') * (f_y[j] - '0') + c;
			cz = z[k] + c % 10 + cz;

			z[k] = cz % 10;

			c /= 10;
			cz /= 10;
		}

		// Add the carry if present
		cz = z[k] + c + cz;
		if(cz)
		{
			z[k] = cz % 10;
			cz /= 10;
		}
		else
			--k;
	}
	if(cz)
		z[++k] = cz;

	// Inverse and convert to string
	std::string r;
	for(int i = k; i >= 0; i--)
		r.append(1, z[i] + '0');

	return r;
}

// ====================================
int main(int, char**)
{
	std::string x, y;

	// Input
	std::cout << "First  number: "; std::cin >> x;
	std::cout << "Second number: "; std::cin >> y;
	//x = "1234";
	//y = "98765";

	// Mul
	std::string z = mul(x, y);

	// Print
	std::cout << x << " + " << y << " = " << z << std::endl;

	return 0;
}

