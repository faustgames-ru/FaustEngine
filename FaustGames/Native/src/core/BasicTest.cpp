#include "BasicTest.h"

namespace core
{
	BasicTest::BasicTest() : _testTime(0)
	{
	}
	void BasicTest::Execute()
	{
		int start = clock();
		OnExecute();
		_testTime = clock() - start;
	}
	int BasicTest::getTestTime()
	{
		return _testTime;
	}
}