#ifndef CORE_CLASSES_H
#define CORE_CLASSES_H

#include "core_platform.h"

namespace core
{
	template<class T> 
	class ObjectsPool;
	
	template<class T> 
	class Lazy;
	
	template<class T, int size> 
	class StaticArray;
}

#endif /*CORE_CLASSES_H*/