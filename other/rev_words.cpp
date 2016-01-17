/**
 * Reverse word occurrence in a string
 */
#include <iostream>
#include <sstream>


int main(int, char**)
{
	const std::string istr = "Reverse words in a string.\tFind it easy? Now do it in-place (This is the real challenge)\n";
	const std::string delimiters = " .,!?()\n\t";

	std::cout << "Input: \"" << istr << '"' << std::endl;

	// std::istringstream
	{
		std::string ostr;

		/**
		 * The EOF bit is set only on attempt to read beyond the end of file.
		 * Therefore an istream must be tested BETWEEN streaming and using the value.
		 */
		std::istringstream iss(istr);
		std::string s;
		while(iss >> s)
		{
			//if(ostr.empty()) ostr = s; else
			ostr = s + " " + ostr;
		}

		std::cout << "std::istringstream: \"" << ostr << '\"' << std::endl;
	}
	// std::find_first_of
	{
		std::string ostr;

		size_t pos = istr.find_first_not_of(delimiters);
		while(pos != std::string::npos)
		{
			size_t cur = pos;
			pos = istr.find_first_of(delimiters, pos);

			std::string s = istr.substr(cur, pos - cur);
			//if(ostr.empty()) ostr = s; else
			ostr = s + std::string(" ") + ostr;

			if(pos != std::string::npos)
				pos = istr.find_first_not_of(delimiters, pos + 1);
		}

		std::cout << "std::find_first_of: \"" << ostr << '"' << std::endl;
	}
	// In-place
	{
		std::string ostr = istr;
		size_t len = ostr.length();

		// Reverse each word
		for(size_t pos = ostr.find_first_not_of(delimiters);
			pos != std::string::npos; 
			pos = ostr.find_first_not_of(delimiters, pos))
		{
			size_t cur = pos;
			pos = ostr.find_first_of(delimiters, pos);
			if(pos == std::string::npos)
				pos = len;

			for(size_t i = cur, j = pos - 1, n = (pos - cur) / 2; n; i++, j--, n--)
				std::swap(ostr[i], ostr[j]);
		}
		
		// Reverse the strings with reversed words
		for(size_t i = 0, j = len - 1, mid = len / 2; i < mid; i++, j--)
			std::swap(ostr[i], ostr[j]);

		std::cout << "In-place: \"" << ostr << '"' << std::endl;
	}

	return 0;
}

