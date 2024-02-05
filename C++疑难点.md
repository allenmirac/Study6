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



## C++编译器至少给一个类提供四个函数

1、构造函数

2、析构函数

3、拷贝构造函数

4、operator= 函数

## 多态的实现



## 菱形继承的问题

## 虚析构
