#ifndef BASIC_TEST_H
#define BASIC_TEST_H

#include "core_platform.h"

namespace core
{
	class BasicTest
	{
	public:
		BasicTest();
		void Execute();
		int getTestTime();
	protected:
		virtual void OnExecute() = 0;
	private:
		int _testTime;
	};
}

#endif /*BASIC_TEST_H*/