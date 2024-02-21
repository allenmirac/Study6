# C++疑难点

## this指针

调用成员函数时，成员函数通过一个名为 this 的隐式参数来访问调用它的那个对象，用请求该函数的对象地址初始化 this ，this 的指向总是自己这个对象，所以 this 是一个常量指针

```c++
Box* get_address()   //得到this的地址
{
	return this;
}
Box* get_address()   //得到this的地址
{
	Box box;
	this = &box; // error: lvalue required as left operand of assignment
    return this;
}
```



## 友元函数和运算符重载

```c++
#include <iostream>
#include <cmath>
using namespace std;

class MyInteger
{
public:
	MyInteger() { this->num_ = 0; }
	MyInteger(int num) : num_(num) {}
	MyInteger operator+(const MyInteger &other)
	{
		MyInteger temp;
		temp.num_ = this->num_ + other.num_;
		// this->num_ += m.num_; 		// 这违反了加法运算符的语义，加法运算通常是创建一个新的对象来保存结果，而不是改变原始对象。
		return temp;
	}

	MyInteger operator+(const int &other) // 运算符重载
	{
		MyInteger temp;
		temp.num_ = this->num_ + other;
		return temp;
	}

	MyInteger operator+=(const MyInteger &other)
	{
		MyInteger temp;
		temp.num_ = this->num_ + other.num_;
		return temp;
	}

	MyInteger operator+=(const int &other)
	{
		MyInteger temp;
		temp.num_ = this->num_ + other;
		return temp;
	}

	MyInteger &operator++()
	{
		++num_;
		return *this;
	}

	MyInteger operator++(int)
	{
		MyInteger temp(*this);
		num_++;
		return temp;
	}

	MyInteger &operator=(const MyInteger &myInteger) // 注意深浅拷贝问题
	{
		num_ = myInteger.num_;
	}

	friend ostream &operator<<(ostream &out, const MyInteger &myInteger); // 重载左移运算符 没加 const ，cout << m++;报错

private:
	int num_;
};

ostream &operator<<(ostream &out, const MyInteger &myInteger)
{
	out << myInteger.num_;
	return out;
}

void test01()
{
	MyInteger m;
	MyInteger m1(10);
	m1 = m + m1;
	cout << m << endl;
	cout << m1 << endl;
}

void test02()
{
	MyInteger m(1);
	MyInteger m1(100);
	m = m1 + m;
	m1 += 10;
	cout << m << " " << m1 << endl;
}

void test03()
{
	MyInteger m(2);
	MyInteger m1(8);
	// cout << ++(++m) << " " << ++m1 << endl;
	// cout << m << " ";
	cout << m++;
	cout << m;
}

int main()
{
	// test01();
	// test02();
	test03();
	// int a=1;
	// int b=2;
	// a=a+b;
	// cout<<a<<" "<<b<<endl;
	int c = 1;
	cout << endl;
	cout << c++ << endl;
	cout << c << endl;
	return 0;
}
```



## 不能重载的运算符

.，:: ，?:，sizeof，typeid，.* 这几个运算符不能被重载

## C++编译器至少给一个类提供四个函数

1、构造函数

2、析构函数

3、拷贝构造函数

4、operator= 函数

## 多态的实现

多态实现的前提三个条件

1、必须是共有继承

2、通过基类指针指向派生类，并且访问派生类重写的方法。

3、基类中的被重写的方法是虚函数

```c++
#include <iostream>
using namespace std;
class Father
{
public:
    virtual void f(){
        cout<<"hello Father!"<<endl;
    }
};

class Son : public Father
{
public:
    void f(){
        cout << "hello Son!"<<endl;
    }
};

int main() {
    Son s;
    Father *p = &s;
    p->f();

    Father f;
    p=&f;
    p->f();

    return 0;
}
```



## 菱形继承的问题

## 虚析构

## g++和gcc的区别

面试的时候，问到了gcc和g++的区别，没答上来： 

   首先说明：gcc 和 GCC 是两个不同的东西   

   GCC:GNU Compiler Collection(GNU 编译器集合)，它可以编译C、C++、JAV、Fortran、Pascal、Object-C、Ada等语言。   

   gcc是GCC中的GNU C Compiler（C 编译器）   

   g++是GCC中的GNU C++ Compiler（C++编译器）   

  

​    一个有趣的事实就是，就本质而言，gcc和g++并不是编译器，也不是编译器的集合，它们只是一种驱动器，根据参数中要编译的文件的类型，调用对应的GUN编译器而已，比如，用gcc编译一个c文件的话，会有以下几个步骤：    

​    Step1：Call a preprocessor, like cpp.    

​    Step2：Call an actual compiler, like cc or cc1.    

​    Step3：Call an assembler, like as.    

​    Step4：Call a linker, like ld    

​    由于编译器是可以更换的，所以gcc不仅仅可以编译C文件    

​    所以，更准确的说法是：gcc调用了C compiler，而g++调用了C++ compiler    

​    gcc和g++的**主要区别**：    

1. 对于 *.c和*.cpp文件，gcc分别当做c和cpp文件编译（c和cpp的语法强度是不一样的）    

2.  对于 *.c和*.cpp文件，g++则统一当做cpp文件编译    

3. 使用g++编译文件时，**g++会自动链接标准库STL，而gcc不会自动链接STL**    

4. gcc在编译C文件时，可使用的预定义宏是比较少的    

5. gcc在编译cpp文件时/g++在编译c文件和cpp文件时（这时候gcc和g++调用的都是cpp文件的编译器）

## 关于内联函数

一般是加快程序执行速度，可能减小可执行文件大小，可能增加可执行文件大小。

速度快：当函数体较短时，内敛函数会像宏一样展开，所以执行速度比一般函数要快。但是如果函数体过大，一般的编译器会放弃内联方式，意思就是你使用内联函数，只不过是向编译器提出了一个申请，编译器可以拒绝，这个函数又会像普通函数一样，执行效率也和普通函数一样。

减小可执行文件大小：内联函数适度

增加可执行文件大小：内联函数过多

## 仿函数

在c++中仿函数（Functor）是一个类或者结构体，重载了函数调用运算符`()`，它的主要作用是提供一种更加灵活的函数对象，它可以包含状态信息，并且可以被传递给算法或者函数，从而实现定制的行为。比如实现**排序准则、查找准则、谓词（就是一个bool返回值的函数）**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void Print(int val){cout<<val<<" ";}
class Print1
{
public:
    void operator()(int val){cout<<val<<" ";}
};

void test01()
{
    vector<int> v;
    v.push_back(6);
    v.push_back(66);
    for_each(v.begin(), v.end(), Print1());
    cout<<endl<<"--------------"<<endl;
    for_each(v.begin(), v.end(), Print);
}

int main()
{
    test01();
    return 0;
}
```

## pointer-like-class的作用

`pointer-like-class`是指类似指针的类，这种类一般用来模拟指针的行为，但是与裸指针相比具有更多的功能和安全性，如智能指针（Smart Pointers）、迭代器（Iterators）。Smart Pointers有`std::unique_ptr`和`std::shared_ptr`、`std::weak_ptr`。
