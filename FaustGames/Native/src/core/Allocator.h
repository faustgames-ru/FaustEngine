#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "core_classes.h"
#include "HollowsAllocationPolicy.h"
#include <memory>
#include <limits.h>

namespace core
{
	template<typename T, typename TPolicy>
	class Allocator {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		template<typename U>
		struct rebind
		{
			typedef Allocator<U, TPolicy> other;
		};

		inline explicit Allocator() {}
		inline ~Allocator() {}
		inline explicit Allocator(Allocator const&) {}
		template<typename U>
		inline explicit Allocator(Allocator<U, TPolicy> const&) {}

		inline pointer address(reference r) { return &r; }
		inline const_pointer address(const_reference r) { return &r; }

		inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
		{
			return reinterpret_cast<pointer>(TPolicy::allocate(cnt * sizeof(T)));
		}

		inline void deallocate(pointer p, size_type)
		{
			TPolicy::deallocate(p);
		}

		//    size
		inline size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

		//    construction/destruction
		inline void construct(pointer p, const T& t) { new(p)T(t); }
		inline void destroy(pointer p) { p->~T(); }

		inline bool operator==(Allocator const&) { return true; }
		inline bool operator!=(Allocator const& a) { return !operator==(a); }
	};    //    end of class Allocator 

	typedef Allocator<void *, Mem> DAlloc;
}

#endif /*ALLOCATOR_H*/