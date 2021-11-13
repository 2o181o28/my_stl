// -*- C++ -*-
#ifndef _MY_STD_H
#define _MY_STD_H 1

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
	
template<class T> constexpr void swap(T& a,T& b){
	T c=std::move(a);a=std::move(b);b=std::move(c);
}

template<class T,std::size_t N>
constexpr void swap(T (&a)[N],T (&b)[N]){
	auto bf=[&]{for(std::size_t i=0;i<N;i++)swap(a[i],b[i]);};
	if _MEMCPYABLE(T){
		if(constexpr auto s=N*sizeof(T);s>_MEMCPY_THRESHOLD){
			alignas(T) char buf[s];T*c=(T*)buf;
			memcpy(c,a,s);memcpy(a,b,s);memcpy(b,c,s);
		}else bf();
	}else bf();
}
}

#undef _MEMCPYABLE
#undef _MEMCPY_THRESHOLD

#endif
