// -*- C++ -*-
#ifndef _MY_ALLOC_H
#define _MY_ALLOC_H 1

#include<bits/stdc++.h>

#define _MEMCPYABLE(T) constexpr(std::is_trivially_copyable_v<T>)
#define _MEMCPY_THRESHOLD 8
#define POOL_SIZE(x) pool<x> _Pool

namespace stl_with_memory_pool{
using __ull=unsigned long long;
constexpr auto operator ""_KiB(__ull x){return x<<10;}
constexpr auto operator ""_MiB(__ull x){return x<<20;}
constexpr auto operator ""_GiB(__ull x){return x<<30;}
constexpr auto operator ""_KB(__ull x){return x*1000;}
constexpr auto operator ""_MB(__ull x){return x*1000000;}
constexpr auto operator ""_GB(__ull x){return x*1000000000;}

void *__buf=0,*__orig_buf;

void assert_with_message(bool b,const char*m){
	if(!b)puts(m),abort();
}

template<uint32_t N> struct pool{
	pool(){
		assert_with_message(!__buf,"Memory pool has been created");
		__orig_buf=__buf=malloc(N);
	}
	~pool(){
		free(__orig_buf);
		__buf=0;
	}
};

template<class T> struct allocator{
	using value_type=T;
	using size_type=uint32_t;
	using difference_type=std::ptrdiff_t;
	using propagate_on_container_move_assignment=std::true_type;

	constexpr T* allocate(size_type n)const{
		// WARNING: this function does NOT check whether the allocation will fail!
		T *r=(T*)__buf;__buf=(void*)(r+n);
		return r;
	}

	void deallocate(T* p,size_type n)const{}
};

}

#undef _MEMCPYABLE
#undef _MEMCPY_THRESHOLD

#endif
