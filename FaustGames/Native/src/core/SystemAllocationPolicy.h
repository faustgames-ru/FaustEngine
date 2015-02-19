#ifndef SYSTEM_ALLOCATION_POLICY_H
#define SYSTEM_ALLOCATION_POLICY_H

#include "core_classes.h"

namespace core
{
	class StandartAllocationPolicy
	{
	public:
		typedef std::size_t size_type;

		static inline void* allocate(size_type cnt)
		{
			return malloc(cnt);
		}

		static inline void deallocate(void* p)
		{
			free(p);
		}
	};
}

#endif /*SYSTEM_ALLOCATION_POLICY_H*/
