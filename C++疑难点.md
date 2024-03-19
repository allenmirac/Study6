# C++疑难点

## this指针

调用成员函数时，成员函数通过一个名为 this 的隐式参数来访问调用它的那个对象，用请求该函数的对象地址初始化 this ，this 的指向总是自己这个对象，所以 this 是一个常量指针

```cpp
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



## 深拷贝问题

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class String{
public:
	String(const char*str=nullptr);
	String(const String& other);
	~String();
	String& operator=(const String&rhs);
private:
	char* m_data;
};
String::String(const char*str){
	if(str==nullptr){
		m_data = new char[1];
		m_data[0]='\0';
	}else{
		m_data = new char[strlen(str)+1];
		strcpy(m_data, str);
	}
}
String::~String(){
	if(m_data!=nullptr){
		delete[] m_data;
	}
}
String& String::operator=(const String& other){
	if(this == &other)
		return *this;
	delete[] m_data;
	m_data = new char[strlen(other.m_data)+1];
	strcpy(m_data, other.m_data);
	return *this;
}

int main(){

	return 0;
}
```



## 友元函数和运算符重载

```cpp
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

其中后置++运算符返回值是局部变量，不能返回&，即对象的引用，局部变量在函数运行结束后就销毁了。

## 类型转换运算符重载

可以实现两个类之间使用=赋值

```cpp
#include <iostream>
using namespace std;

class A{
};

class B{
public:
	operator A(){
		return A();
	}
};

int main(){
	A a;
	B b;
	a=b;
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

3、基类中的被重写的方法是**虚函数**

这种技术让父类指针有**多种形态**，是一种泛型技术，直到运行时才决定执行哪个版本的函数。所谓**泛型技术**，就是使用不变的代码来实现可变的算法。多态中没有重写的函数是没有意义的。

水能载舟，亦能覆舟。多态也涉及了**安全性**的问题，

首先是无法访问子类中自己的虚函数，如`Base * base=new Derived(); base->f1();`编译时不会通过的（其中f1()是子类自己的虚函数，父类没有）；

然后是如果父类中虚函数是private或者protected，这些函数依旧会存在于虚函数表中，可以通过多态的方式来访问。

```cpp
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

继承关系画成图像一个菱形，所以就叫做菱形继承，采用**虚继承**来解决二义性问题。

```cpp
#include <iostream>
using namespace std;
class A
{
public:
	int n;
};
class B : virtual public A
{
};
class C : virtual public A
{
};
class D : public A
{
};
class E : public B, public C, public D
{
public:
	int getn() { return B::n; }
};
int main()
{
	E d;
	d.B::n = 10;
	d.C::n = 20;
	d.D::n = 30;
	cout << d.B::n << endl;
	cout << d.C::n << endl;
	cout << d.D::n << endl;
    cout << d.getn() << endl;
	return 0;
}
```

可以先在脑子里想一下结果，然后可以运行一下看看是否与其一致，有助于理解哦。

## 虚析构

总的来说是为了避免内存泄漏，利用基类指针释放派生类对象的内存。也就是说，虚析构函数使得删除指向子类的父类指针时，不仅可以调用父类的的析构函数，也会调用子类的析构函数，这样就可以释放子类指针成员变量在堆中的内存，达到防止内存泄漏的目的。

```cpp
#include <bits/stdc++.h>
using namespace std;
class CA
{
public:
    CA() { cout << "CA" << endl; }
    virtual void f1()
    {
        cout << "CA::f1( )" << endl;
        // f2();
    }
    void f2()
    {
        cout << "CA::f2( )" << endl;
    }
    virtual ~CA()
    {
        cout << "~CA" << endl;
    }
};
class CB : public CA
{
public:
    CB() { cout << "CB" << endl; }

    virtual void f1()
    {
        cout << "CB::f1( )" << endl;
    }
    void f2()
    {
        cout << "CB::f2( )" << endl;
    }
    virtual ~CB()
    {
        cout << "~CB" << endl;
    }
};
class CC : public CB
{
public:
    CC() { cout << "CC" << endl; }

    void f1()
    {
        cout<<"CC:f1()"<<endl;
    }
    void f2()
    {
        cout << "CC:f2()" << endl;
    }
    virtual ~CC()
    {
        cout << "~CC" << endl;
    }
};
int main()
{
    CA *pA = new CC();
    pA->f1();
    delete pA;
    CA *pA1 = new CB();
    pA1->f1();
    delete pA;
    return 0;
}//注意看构造和析构的顺序，正好是相反的
```

## 运用运算符delete删除一个动态对象时，是先调用析构函数还是先释放对象内存

当使用 `delete` 运算符删除动态分配的对象时，会**先调用对象的析构函数来执行清理操作，然后再释放对象所占用的内存。**

如果先释放内存而不调用析构函数，就无法确保对象所占用的资源被正确释放，可能导致内存泄漏的问题，即程序无法再访问这些内存区域，但却无法再对其进行释放，造成系统资源浪费。

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

# 一：结构化程序设计方法

## 位运算及其运用

判断奇数还是偶数

```cpp
int a=11;
int b=1;
if(b&a){
	cout << "is odd" <<endl;
}else{
	cout<< "is even" <<endl;
}
```

取出指定的位

```cpp
int a=0b11101101;
int b=0b1111;
cout << bitset<sizeof(char)*8>(a&b)<<endl;
```

判断是否为2的整数幂（0b10 0000 & 0b01 1111），结果是0，说明是2的整数幂。

```cpp
int x = 64;
int y = x - 1;
cout << ((x & y) ? "no" : "yes") << endl;
```

## 指针数组与指向数组的指针的概念与使用

a是一个指向含有三个int型数据数组的指针（如有int p[3]; a指向p是可以的 ）

a1是一个函数三个int*型数据的数组（如a1可以初始化为{&b, &c, &d}）;

```cpp
#include <iostream>
#include <typeinfo>
using namespace std;
#define sz(type) cout<<sizeof(type)<<endl;
int main(){
	int b=10, c=1, d=2;
	int (*a)[3];
	int* a1[3];
 	cout<<typeid(a).name()<<endl;
 	sz(a);
 	sz(a1);
 	sz(a1[0]);
 	sz(*a)
 	sz(a[0][0])
	return 0;
}
```

## 引用的分类与使用

1、初始化，定义引用时需要加&，

```cpp
int a=1;
int &b=a;//相当于给a取了一个别名
```

2、作为函数参数传递，c++在函数参数中传递数组时，直接变成了指针，因为如果将数组传递过去需要将值一个一个拷贝过去，增加了函数调用的开销，所以在 C++ 中，我们有了一种比指针更加便捷的传递聚合类型数据的方式，那就是**引用（Reference）**，通过这种方式传过去减少了生成副本的消耗。

```cpp
void swap(int &a, int &b){
	int temp=a;
	a=b;
	b=temp;
}
```

3、作为函数返回值来传递，例如重载左移运算符，需要输出多个内容就需要返回引用。

```cpp
#include <iostream>
#include <typeinfo>
using namespace std;
#define sz(type) cout << sizeof(type) << endl;
int function1(int &aa) // 以返回值的方法返回函数值
{
	return aa;
}
int &function2(int &aa) // 以引用方式返回函数值
{
	return aa;
}
int main()
{
	int a = 10;
	// 第一种情况，系统生成要返回值的副本（即临时变量）
	int b = function1(a); // function1()的返回值先储存在一个a的副本中，
						  // 然后再把副本赋值给b
	// 第二种情况，报错
	//  function1(a) = 20;// function1()的返回值为临时变量，不能赋值（即不能为左值）
	// 第三种情况，系统不会生成返回值的副本
	function2(a) = 20; // OK  此时a的值变成了20
	cout<<a<<endl;
}
```

重载左移运算符，返回引用才会连续输出两个值；


```cpp
#include <iostream>
using namespace std;

class Point
{
public:
    int x_, y_;
    Point(int x, int y) : x_(x), y_(y) {}

    friend ostream &operator<<(ostream &os, const Point &p)
    {
        os << p.x_ << " " << p.y_ << endl;
        return os;
    }
};

int main()
{
    Point p(1, 2);
    cout<<p<<p;

    return 0;
}
```

## 变量的作用域和生命周期

c++的内存主要分为一下几个部分：栈区、堆区、全局区（静态区）、文字常量区、程序代码区。

全局变量

> 存储在静态内存分配区，整个程序的生命周期都可以使用，其他文件使用关键字extern也可以使用

局部变量

> 存储在栈区，与函数共存亡

全局静态变量

> 与全局变量类似，也是存储在静态内存分配区，生命周期与整个程序同在，不过不能再其他文件使用。

局部静态变量

> 也是存储在静态内存分配区，调用函数后便一直存在，只不过只能在函数内可见。

## 类型别名与类型推断

1、别名：typedef、using

2、推断：auto、decltype

## STL顺序容器

vector：视作可变大小的数组，可随机访问，在非尾部的位置插入比较慢

deque：双端队列，支持随机访问，在头尾位置插入和删除比较快

list：双向链表，支持双向顺序访问（rbegin()、rend()），在 list 任意位置插入和删除都比较快

forward_list：单向链表，只能单向顺序访问，在 forward_list 的任意位置插入和删除都比较快

array：固定大小的数组，支持随机访问，不能添加或者删除元素

string：与 vector 类似，专门保存字符串，随机访问快，在尾部插入和删除快

## STL关联容器

map：键值对，一对一，基于红黑树，对关键字进行排序

set：只保存关键字，不重复

multimap：关键字可以重复的 map ，即一对多，如统计数学课的学生成绩

multiset：保存可以重复的关键字

unordered_map：键值对，一对一，基于哈希值，不对键值对进行排序

unordered_set：只保存关键字，不排序

unordered_multimap：键值对，一对多，基于哈希值

unordered_multiset：保存可以重复的关键字，不排序

## STL容器适配器

包括：stack、queue、priority_queue

 stack 和 queue 基于deque实现， priority_queue 基于 vector 实现

## 流对象

包括输入流对象（ostream，如std::cin，从键盘读取数据），输出流对象（istream，如std::cout，向屏幕写入数据）

还有文件输入流对象（ifstream，使用 std::ifstream 来创建对象，将数据从文件中读取出来），文件输出流对象（ofstream，使用 std::ofstream 来创建对象，将数据写入文件）

```cpp
#include <iostream>
#include <fstream>
using namespace std;

void test01()
{
    ofstream file;
    file.open("text.txt", ios::out);

    file << "xinm" << endl;
    file << "namji" << endl;
    file.close();
}
void test02()
{
    ifstream ifile;
    ifile.open("text.txt", ios::in);
    if(ifile.is_open()){
        char buf[1024];
        while(ifile >> buf)
        {
            cout<< buf<<endl;
        }
    }
    ifile.close();
}

int main()
{
    test02();
    return 0;
}
```

使用二进制流读写文件

```cpp
#include <iostream>
#include <fstream>
using namespace std;
class Person
{
public:
    // Person(char name[], int age):name_(name), age_(age){}
    char name_[20];
    int age_;
};

void test01()
{
    ofstream file;
    file.open("text.txt", ios::out | ios::binary);
    Person p = {"hhhhh", 18};
    file.write((const char *)&p, sizeof(p));
   
    file.close();
}
void test02()
{
    ifstream ifile;
    ifile.open("text.txt", ios::in | ios::binary);
    if (ifile.is_open())
    {
        // 1
        // char buf[1024];
        // while(ifile >> buf)
        // {
        //     cout<< buf<<endl;
        // }

        // 2
        // char buf[1024];
        // while (ifile.getline(buf, sizeof(buf)))
        // {
        //     cout << buf << endl;
        // }

        // 3
        // string buf;
        // while(getline(ifile, buf)){
        //     cout << buf <<endl;
        // }

        Person p;
        ifile.read((char *)&p, sizeof(p));
        cout << p.name_ << p.age_ << endl;
    }
    ifile.close();
}

int main()
{
    test02();
    return 0;
}
```

从txt文件中读出的以空格为间隔的数字，并排序

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <fstream>
using namespace std;
void readFromTxt(char* path, vector<int> &v){
	ifstream iFile;
	iFile.open(path, ios::in);
	if(iFile.is_open()){
		string s;
		while(iFile>>s){
//			v.push_back(s[0]-'0');
			cout << s << endl;
			int num=0;
			for(int i=0; i<s.size(); i++){
				num=num*10+s[i]-'0';
			}
			v.push_back(num);
		}
	}
	cout << "v" << endl;
	for(auto i:v){
		cout << i<< endl;
	}
}
void selectSort(vector<int> &v){
	int n=v.size();
	for(int i=0; i<n; i++){
		int min=i;
		for(int j=i+1; j<n; j++){
			if(v[j]<v[min]) min=j;
		}
		swap(v[i], v[min]);
	}
	
}
void testReadFromTxt(){
	char* path="a.txt";
	vector<int> v;
	readFromTxt(path, v);
	selectSort(v);
	cout << "v" << endl;
	for(auto i: v){
		cout << i << endl;
	}
}
int main(){
	testReadFromTxt();
	return 0;
}
```



## 异常处理的构造和析构函数

若在 try 中抛出异常，在转到 catch 前，会对有关对象进行析构

```cpp
#include <iostream>

using namespace std;

//异常处理的构造和析构函数 
class Student{
	private:
		string name;
		int sno;
	public:
		Student(string name1,int sno1)
		{
			name=name1;
			sno=sno1;
		} 
		~Student()
		{
			cout<<"Destruct Student:"<<sno<<endl;
		}
		void checkSno()
		{
			if(sno==0)
			{
				throw sno;
			}
			else
			{
				cout<<name<<":"<<sno<<endl; 
			}
		}
};

int main()
{
	try
	{
		Student a("pink",1);
		a.checkSno();
		Student b("floyd",0); //构造对象
		b.checkSno();  //抛出异常跳到catch语句块中
	}
	catch(int)
	{
		cout<<"error:sno=0!"<<endl;
	}
	return 0;
}
```

可以看到上面这段代码的输出结果是先对a、b析构然后再转到catch输出内容。

## 类模板与继承

要注意继承的时候要加上基类的类型

```cpp
#include <iostream>

using namespace std;

template<class T>
class Base
{
    public:
    T c;
};

template<class T>
class Deri: public Base<T>
{
    public:
    T a;
};

int main()
{
    Deri<int> d;
    d.a=4;
    d.c=6;
    cout <<d.Base::c<<endl;
    // cout << sizeof(Deri<int>)<<endl;
    return 0;
}
```

## 继承时不能被继承的函数

包括：构造函数、拷贝构造函数、析构函数，注意：operator=函数是可以被继承的

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class A{
public:
	A(){
	}
	A(int price, int weight):price_(price), weight_(weight){
	}
	A& operator=(const A& a){
		cout << "operator=1" << endl;
	}
private:
	int price_;
	int weight_;
};

class B: public A{
public:
	B(int price, int weight):price_(price), weight_(weight){
	}
private:
	int price_;
	int weight_;
};

int main(){
//	A a1(1,0);
//	A a2;
//	a2=a1;
	B b1(10,0),b2(2,0);
	b2=b1;
	return 0;
} 
```

上面说明operator=函数被继承下去了。

## 静态数据成员的只能在类外初始化

类内定义，类外初始化一次，然后在整个程序运行期间都可以存在

原因

1、静态成员变量存储在静态存储区域内，在编译阶段就为他们分配内存空间，

2、静态成员变量是类级别的，他在所有类实例中是公有的，如果允许在类内部初始化，就可能导致每个实例都有一个独立的副本

3、这样可以避免多次初始化

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>
using namespace std;
class Point
{
public:
	Point(int x, int y) : x_(x), y_(y)
	{
	}
	void setX(int val)
	{
		x_ = val;
	}
	void setY(int val)
	{
		y_ = val;
	}
	int getX() const
	{
		return x_;
	}
	int getY() const
	{
		return y_;
	}

private:
	int x_;
	int y_;
};
class Circle
{
public:
	Circle(int x, int y, int r) : center_(x, y), r_(r)
	{
		num_++;
		cout << "Circle number: " << getNum() << endl;
		;
	}
	static int getNum()
	{
		return num_;
	}
	friend double getDistance(const Circle &c1, const Circle &c2);

private:
	static int num_;
	Point center_;
	int r_;
};

int Circle::num_ = 0;
double getDistance(const Circle &c1, const Circle &c2)
{
		double x=double(c2.center_.getX()-c1.center_.getX());
		double y=double(c2.center_.getY()-c1.center_.getY());
		return sqrt(x*x + y*y);
}

void testCircle()
{
	Circle c1(1, 2, 3);
	Circle c2(4, 6, 2);
	cout << getDistance(c1, c2) << endl;
}

int main()
{
	testCircle();
	return 0;
}
```

## 处理字符和字符串的输入输出操作

输出函数：

1. putc()：将一个字符写入到指定的文件流中。
2. puts()：将一个字符串写入到标准输出流（屏幕）中，并自动添加换行符。
3. printf()：格式化输出函数，用于以指定格式将数据打印到标准输出流（屏幕）中。
4. fputc()：将一个字符写入到指定的文件流中。
5. fputs()：将一个字符串写入到指定的文件流中。
6. fprintf()：格式化输出函数，用于以指定格式将数据打印到指定的文件流中。

输入函数：

1. getc()：从指定的文件流中读取一个字符。
2. gets()：从标准输入流（键盘）中读取一行字符串，并存储到指定的字符数组中。
3. scanf()：格式化输入函数，用于从标准输入流（键盘）中读取指定格式的数据。
4. fgetc()：从指定的文件流中读取一个字符。
5. fgets()：从指定的文件流中读取一行字符串，并存储到指定的字符数组中。
6. fscanf()：格式化输入函数，用于从指定的文件流中读取指定格式的数据。

## 运算符的短路效果

在C语言中，逻辑与运算符 `&&` 和逻辑或运算符 `||` 属于短路逻辑运算符。这意味着当使用这两个运算符进行条件判断时，如果第一个操作数的值已经能够确定整个表达式的值，那么就不会再对第二个操作数进行求值，从而实现了“短路”效果。

具体来说：

- 对于逻辑与运算符 `&&`，当第一个操作数为假（0）时，整个表达式的值已经确定为假，此时不会再对第二个操作数进行求值，直接返回假。
- 对于逻辑或运算符 `||`，当第一个操作数为真（非0）时，整个表达式的值已经确定为真，此时不会再对第二个操作数进行求值，直接返回真。
