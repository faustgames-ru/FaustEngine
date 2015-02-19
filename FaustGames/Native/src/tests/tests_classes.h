#ifndef TESTS_CLASSES_H
#define TESTS_CLASSES_H

#include "tests_platform.h"
#include "tests_references.h"

namespace core
{
	class TestConstanants
	{
		static const int AllocationsTestsIterationsCount = 1024;
		static const int AllocationsTestsMinimumSize = 1;
		static const int AllocationsTestsMaximum = 1024*1024;
	};
}

#endif /*TESTS_CLASSES_H*/