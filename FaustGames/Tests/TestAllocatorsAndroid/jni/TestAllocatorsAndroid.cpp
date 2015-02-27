/**********************************
 Java Native Interface library
**********************************/
#include <jni.h>

/** This is the C++ implementation of the Java native method.
@param env Pointer to JVM environment
@param thiz Reference to Java this object
*/

#include <cstdio>
#include <malloc.h>
#include <list>

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
		return 2147483648 / sizeof(T);
	}

	//    construction/destruction
	inline void construct(pointer p, const T& t) { new(p)T(t); }
	inline void destroy(pointer p) { p->~T(); }

	inline bool operator==(Allocator const&) { return true; }
	inline bool operator!=(Allocator const& a) { return !operator==(a); }
};    //    end of class Allocator 

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

std::list<int, Allocator<int, StandartAllocationPolicy>> _list;

extern "C"
JNIEXPORT void JNICALL
Java_test_allocators_android_TestAllocatorsAndroid_testallocatorsandroidNative( JNIEnv* env, jobject thiz )
{
	// Enter code here
}
