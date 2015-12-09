#include "Convert.h"
#include <string>
#include <sstream>

namespace core
{

	template <typename T>
	std::string to_string(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

	std::string Convert::toString(int n)
	{
		return to_string(n);
	}

	std::string Convert::toString(int n, int leadingZeros)
	{
		std::string result = toString(n);
		int addZeros = leadingZeros - result.size();
		if (addZeros <= 0)
			return result;
		std::string zeros("");
		for (int i = 0; i < addZeros; i++)
		{
			zeros += "0";
		}
		return zeros + result;
	}
}
