#include "Convert.h"

namespace core
{
	std::string Convert::toString(int n)
	{
		return std::to_string(n);
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
