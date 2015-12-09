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
	template <typename T>
	std::string to_int(T value)
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}


	float Convert::toFloat(int v)
	{
		return v;
	}

	float Convert::toFloat(const std::string& v)
	{
		return atof(v.c_str());
	}

	int Convert::toInt(float v)
	{
		return static_cast<int>(floor(v));
	}

	int Convert::toInt(const std::string &v)
	{
		return toInt(atof(v.c_str()));
	}

	std::string Convert::toString(int n)
	{
		return to_string(n);
	}

	std::string Convert::toString(float n)
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
