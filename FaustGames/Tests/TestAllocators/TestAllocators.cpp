// TestAllocators.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <time.h>
#include <chrono>

#include "Allocator.h"
#include "StandartAllocationPolicy.h"

struct Data
{
	int id;
	int value;
	
	Data() : id(0), value(0)
	{
	}

	Data(int v) : id(0), value(v)
	{
	}
};

const int ListCount = 4 * 1024 * 1024;
template<typename Policy>
void testAlloc(char *testName)
{
	typedef std::list<Data, core::Allocator<Data, Policy>> TestContainer;

	TestContainer *list = new  TestContainer();
	auto time = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < ListCount; i++)
	{
		list->push_back(Data(i));
	}
	auto dt = std::chrono::high_resolution_clock::now() - time;
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count();
	std::cout << "	";

	time = std::chrono::high_resolution_clock::now();
	int index = 0;
	for (TestContainer::iterator i = list->begin(); i != list->end(); i++)
	{
		if (i->value != index)
		{
			std::cout << "!!!!!!";
			std::cout << "\n";
		}
		index++;
	}
	if (ListCount != index)
	{
		std::cout << "!!!!!!";
		std::cout << "\n";
	}
	dt = std::chrono::high_resolution_clock::now() -time;
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count();
	std::cout << "	";


	time = std::chrono::high_resolution_clock::now();
	delete list;
	dt = std::chrono::high_resolution_clock::now() -time;


	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(dt).count();
	std::cout << "	";
	std::cout << testName;
	std::cout << "\n";

}

typedef core::LinearAllocationPolicy<256 * 1024 * 1024> LinearAllocationPolicy;

int _tmain(int argc, _TCHAR* argv[])
{
	int i;
	std::cin >> i;
	for (int i = 0; i < 1; i++)
	{
		testAlloc<LinearAllocationPolicy>("LinearAllocationPolicy");
		testAlloc<core::HolowsAllocationPolicy>("HolowsAllocationPolicy");
		//testAlloc<core::StandartAllocationPolicy>("StandartAllocationPolicy")^;
	}

	std::cin >> i;

	core::HolowsAllocationPolicy::cleanup();
	//LinearAllocationPolicy::cleanup();
	std::cin >> i;

	return 0;
}

