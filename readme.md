我自己的STL实现，使用C++20语法。

# 行为

只列出和STL不一致的行为。

**以下的全部内容，如果不加额外声明，均不会抛出任何异常，但并没有全加上`noexcept`声明**

## allocator

内存只分配，从不回收。如果加起来使用了超过`POOL_SIZE`指定的内存，是未定义行为。

## vector

不支持`vector<bool>`的特化（估计没人喜欢这个特化吧）。

`vector::at`抛异常的行为与标准指定的相同。

`vector::size_type`是`uint32_t`，`difference_type`是`int`。

`vector<T>().max_size()==1e9`，如果存多于10^9个元素是未定义行为。

`vector::pop_back`会返回最后一个元素。（向Rust学习）

在`push_back`、`insert`等等可能导致内存重分配的函数中，间接或直接使用此`vector`内部的元素引用，均为未定义行为。

```cpp
vector<std::string> v;
v.push_back("123");
// v.push_back(v[0]);  // 非法，至于为啥，可以参考Rust
v.push_back(v[0]+"4"); // 合法
```

支持和`std::basic_string`相似的`+`、`+=`操作。对应的运算符重载声明如下：

```cpp
constexpr vector& operator+=(const T& value);
constexpr vector& operator+=(T&& value);
constexpr vector& operator+=(const vector& other);
constexpr vector& operator+=(vector&& other);
constexpr vector& operator+=(std::initializer_list<T> ilist);

template<class T,class Alloc>
	vector<T,Alloc> operator+(const vector<T,Alloc>& lhs,const vector<T,Alloc>& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(const vector<T,Alloc>& lhs,const T& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(const vector<T,Alloc>& lhs,T&& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(const T& lhs,const vector<T,Alloc>& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(T&& lhs,const vector<T,Alloc>& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(vector<T,Alloc>&& lhs,vector<T,Alloc>&& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(vector<T,Alloc>&& lhs,const vector<T,Alloc>& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(vector<T,Alloc>&& lhs,const T& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(vector<T,Alloc>&& lhs,T&& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(const vector<T,Alloc>& lhs,vector<T,Alloc>&& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(const T& lhs,vector<T,Alloc>&& rhs);
template<class T,class Alloc>
	vector<T,Alloc> operator+(T&& lhs,vector<T,Alloc>&& rhs);
```

## bitset

与STL完全相反，在任何情况下都会使用64位的压位。貌似C++标准没有要求长度可以为0，故不允许`bitset<0>`。

由于使用了`memset`，你不能创建一个`constexpr bitset<N>`。

`bitset::test`抛异常的行为与标准指定的相同。

新增`data()`函数，返回指向底层实现的指针。声明如下：

```cpp
constexpr const unsigned long long* data()const;
constexpr unsigned long long* data();
```

新增`find_first()`、`find_next(size_t pos)`函数，分别寻找第一个非零位，以及第`pos`位之后（不包含第`pos`位）的第一个非零位。如果不存在这样的位置，返回`size()`。（其实不算新增了，就是`std::bitset`的`_Find_first`和`_Find_next`）声明如下：

```cpp
size_t find_first()const;
size_t find_next(size_t pos)const;
```

暂不支持hash。

## map

**底层实现是B树**，故不支持`extract`等等C++17起的面向节点的内容，且不支持使用位置hint的各种函数（因为不会有任何的加速）。

`map::at`抛异常的行为与标准指定的相同。
