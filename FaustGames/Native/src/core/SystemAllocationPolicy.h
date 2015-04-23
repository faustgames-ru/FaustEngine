#ifndef SYSTEM_ALLOCATION_POLICY_H
#define SYSTEM_ALLOCATION_POLICY_H

#include "core_classes.h"

namespace core
{
	class StandartAllocationPolicy
	{
	public:
		typedef std::size_t size_type;
		static int AllocatedSize;

		static inline void* allocate(size_type cnt)
		{
			return malloc(cnt);
		}

		static inline void deallocate(void* p)
		{
			free(p);
		}

		template<typename T>
		static inline T * alloc()
		{
			return (T *)allocate(sizeof(T));
		}

		template<typename T>
		static inline T * construct()
		{
			return new (alloc<T>()) T();
		}

		template<typename T>
		static inline void dispose(T *p)
		{
			p->~T();
			deallocate(p);
		}
	};
}

#endif /*SYSTEM_ALLOCATION_POLICY_H*/
