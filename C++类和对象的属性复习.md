# C++类和对象的属性

## const修饰结构体指针

内部值不能修改，即：只能读，不能写。防止误操作

```c++
#include <iostream>
using namespace std;

struct Point
{
    int x;
    int y;
};

int main()
{
    // const 修饰指针，指向的对象不能通过该指针被修改
    const Point* p1 = new Point{3, 4};
    // p1->x = 5;  // 编译错误：指针指向的对象是 const 的，不能被修改

    // 指针本身是 const，不能指向其他对象
    Point* const p2 = new Point{5, 6};
    // p2 = new Point{7, 8};  // 编译错误：p2 是 const 指针，不能指向其他对象
    p2->y = 8;  // 合法：指针指向的对象可以通过此指针被修改

    // 同时 const 修饰指针和指向的对象
    const Point* const p3 = new Point{7, 8};
    // p3->x = 9;  // 编译错误：指针指向的对象是 const 的，不能被修改
    // p3 = new Point{10, 11};  // 编译错误：p3 是 const 指针，不能指向其他对象

    return 0;
}
```



## 左移运算符重载

无法用成员函数写这个函数，因为不能把左移cout写在左边

```c++
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



## 友元类技术

利用友元类技术，可以访问私有类型

```c++
#include <iostream>
using namespace std;

class MyClass{
private:
    int a_;
    friend class FriendClass;
public:
    MyClass(int a): a_(a){}
};

class FriendClass{
public:
    void accessPrivateMember(MyClass& my){
        cout<<"Myclass private menber: "<< my.a_<<endl;
    }

};

int main(){
    MyClass my(10);
    FriendClass friendClass;
    friendClass.accessPrivateMember(my);

    return 0;
}
```



## 构造函数

构造函数也可以重载

创建任何对象都要调用构造函数

## 析构顺序

当类内存在其他类成员时，构造函数顺序是先成员变量的构造函数，后本类的构造函数，析构顺序则相反

```c++
#include <iostream>
#include <string>
using namespace std;
class Phone;
class Person
{
public:
    Person()
    {
        cout << "moren" << endl;
    }
    Person(string name, int age, Phone* phone, int rea)
    {
        m_name = name;
        m_age = age;
        m_phone = phone;
        m_rea = new int(rea);
        cout << "gouzao---------------" << endl;
    }
    ~Person()
    {
        if (m_rea != nullptr)
        {
            delete m_rea;
        }
        cout << "xigou----------------==" << endl;
    }
    Person(const Person &p)
    {
        cout << "kaobei---------------===" << endl;
    }

private:
    string m_name;
    int m_age;
    Phone* m_phone;//为什么使用指针，有学问在里边
    int *m_rea;
};

class Phone
{
public:
    Phone(){
        m_Number = 1232;
    }
    Phone(int number) : m_Number(number)
    {
        cout << "Phone 构造" << endl;
    }
    ~Phone()
    {
        cout << "Phone 析构" << endl;
    }

private:
    int m_Number;
};
void test01()
{
    Person p1;
    cout << 1 << endl;
}

void test02()
{
    int t = 1;
    // Person p("123", 123, t);
    // Person p1(p);
    // cout << "02" << endl;
}

void test03(){
    Phone phone(123);
    Person p("123", 123, &phone, 123);
    cout<<"12333333333333333333333"<<endl;
}

int main()
{
    // test02();
    // cout << "03" << endl;
    test03();
    return 0;
}
```

上面`Person`的`Phone`成员变量使用指针是因为`Person`类中的`Phone`对象是一个不完全类型，因此编译器无法确定其**大小和布局**。这导致在`Person`类中使用`Phone`对象时出现错误。

## 拷贝构造函数

执行拷贝构造函数的三个时机

（1）、用一个已经存在的对象来创建一个新的对象时（**这是最本质的原因**）

Spot spot2=spot1；

（2）、对象作为函数的参数（值传递，不是引用传递）时，实际上就是在调用拷贝构造函数。

Spot spot3(spot2);

（3）、函数的返回值是一个对象，执行return语句时，系统会自动调用拷贝构造函数创建返回值。

```c++
#include <iostream>
using namespace std;

class MyClass {
private:
    int* data;
public:
    // 默认构造函数
    MyClass() {
        data = new int(0);
        cout << "Default constructor called." << endl;
    }

    // 拷贝构造函数
    MyClass(const MyClass& obj) {
        data = new int(*obj.data);  // 深拷贝
        cout << "Copy constructor called." << endl;
    }

    // 析构函数
    ~MyClass() {
        delete data;
        cout << "Destructor called." << endl;
    }

    // 打印数据成员
    void printData() {
        cout << "Data: " << *data << endl;
    }
};

// 函数参数为对象
void myFunction(MyClass obj) {
    obj.printData();
}

MyClass myFunction1(const MyClass& obj) {
    return obj;
}

int main() {
    // 用对象初始化另一个对象
    MyClass obj1;
    // MyClass obj2 = obj1;

    // 传递对象给函数参数
    // myFunction(obj1);

    // 通过值返回对象
    MyClass obj3 = myFunction1(obj1);

    return 0;
}
```

上面三个都是一次默认构造，一次拷贝构造，两次析构。`myFunction`和`myFunction1`的区别：按值传递会触发拷贝构造函数，按引用传递不会触发，但是传递返回值时会触发拷贝构造函数，所以输出结果相同

## C/C++数组名与指针区别

https://blog.csdn.net/ykm0722/article/details/7031387



## 静态数据成员

静态数据成员不能直接赋初值，一般在类外初始化

静态成员函数调用方式：通过对象或者通过:类名::静态成员函数

静态成员函数不可以访问非静态成员变量，因为静态成员函数是所有对象共有的，当一个对象调用这个函数时，如果修改非静态成员变量，他不知道是哪个对象的变量。无法区分是哪个对象的。

静态成员函数也有访问权限

```c++
#include <iostream>
#include <string>
using namespace std;
class Person
{
    string name;
    static int Num;

public:
    Person(string N) : name(N) { Num++; }
    Person(const Person& p){ Num++;}
    void Print() { cout << "共有" << Num << "个人" << endl; }
    ~Person() { Num--; }
};
int Person::Num = 0;
int main()
{
    Person p1("hehe");
    Person p2 = p1;
    Person p3(p2);
    Person p4("hh");
    p1.Print();
    return 0;
}
```

### 研究静态函数成员是否有this指针

结论：没有，因为静态成员函数**不属于某个对象**，被类的所有对象共享。

## 类的组合与继承

**当一个类将要被继承时，为了方便对他的继承，往往将他的private数据成员改为protected访问权限，因为基类的private成员不方便在派生类中访问**

**注意**:**只有构造函数初始化的时候能使用初始化列表**。成员函数不行，要不然会报错

## 友元类函数

定义友元类函数，再类外部定义时不能访问private成员，而在类内定义可以，程序如下

```c++
#include <iostream>
#include <cmath>
using namespace std;
class Point
{
    int x;
    int y;

public:
    friend float Distance(const Point &p1, const Point &p2)
    {
        return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    }
    Point(const int &X, const int &Y)
    {
        x = X;
        y = Y;
    }
    // void setX(int X) : x(X) {}
};
// float Distance(Point& p1, Point& p2)
//{
//	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
//}
int main()
{
    Point p1(10, 10), p2(11, 11);
    cout << Distance(p1, p2);
}                
```



## 多继承

优缺点：

[(11条消息) 多继承的概念和优缺点_HappyJandun's 学习笔录-CSDN博客_多继承的优缺点](https://blog.csdn.net/jandunlab/article/details/14110117)

```c++
#include<iostream>
using namespace std;

class Person
{
public:
    void sleep() { cout << "sleep" << endl; }
    void eat() { cout << "eat" << endl; }
};

class Author : public Person // Author继承自Person
{
public:
    void writeBook() { cout << "write Book" << endl; }
};

class Programmer : public Person // Programmer继承自Person
{
public:
    void writeCode() { cout << "write Code" << endl; }
};

class Programmer_Author : public Programmer, public Author // 多重继承
{
};

int main()
{
    Programmer_Author pa;
    pa.writeBook(); // 调用基类Author的方法
    pa.writeCode(); // 调用基类Programmer的方法
    // pa.eat();       // 编译错误，eat()定义不明确
    // pa.sleep();     // 编译错误，sleep()定义不明确
    return 0;
}
```

​                \#include <iostream> using namespace std; const double PI=3.14; class Point {    int x,y; public:    Point(int x1=0,int y1=0):x(x1),y(y1){}    //动态指定函数    virtual double area()    {        return 0.0;    }    // virtual double perimeter()    // {    //  return 0.0;    // }    double perimeter()    {        return 0.0;    }//这时会输出0 }; class Circle:public Point {    double radius; public:    Circle(int x1,int y1,int Radius):Point(x1,y1)    {        radius=Radius;    }    double area()    {        return PI*radius*radius;    }    double perimeter()    {        return 2*PI*radius;    } }; int main() {//虚函数！！！动态编译    Point p1(10,10);    Point *p=&p1;    cout<<"p1.area()="<<p1.area()<<endl;    cout<<"p1.pretimeter="<<p1.perimeter()<<endl;    Circle c1(1,1,1);    p=&c1;    //到程序执行才确定是指定哪一个函数    cout<<"c1.area()="<<p->area()<<endl;    cout<<"c1.pretimeter()="<<p->perimeter()<<endl;    return 0; }              

多重继承的优点很明显，就是**对象可以调用多个基类中的接口**

缺点是如果派生类所继承的多个基类有相同的基类，而派生类对象需要调用这个祖先类的接口方法，就会容易出现二义性。 

通常有两个解决方案：

（1）加上全局符确定调用哪一份拷贝。比如pa.Author::eat()调用属于Author的拷贝。

（2）使用虚拟继承，使得多重继承类Programmer_Author只拥有Person类的一份拷贝。比如在11行和17行的继承语句中加入virtual就可以了。

**C++为什么要用虚函数**

[(3条消息) C++为什么要用虚函数_Like a lunatic-CSDN博客](https://blog.csdn.net/noricky/article/details/80051219?ops_request_misc=%7B%22request%5Fid%22%3A%22163806102216780255288237%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=163806102216780255288237&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-2-80051219.first_rank_v2_pc_rank_v29&utm_term=什么时候要用虚函数&spm=1018.2226.3001.4187)

**总结起来：**这样做是为了当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用

要实现多态必须要满足的条件



定义了虚函数，如果有三辈的继承：爷爷，父亲，儿子

在主函数中，定义一个基类指针，指针指向谁，就调用谁的函数————实现的就是动态多态

## 纯虚函数

**一般语法**

```c++
virtual void print() =0;//纯虚函数，可以有函数体，可实例化的派生类必须重写它
```

**定义：**

像这样用virtual关键字修饰，并且末尾加上 =0 的格式声明的函数就是纯虚函数，还有一个重要特征，就是在当前类里这个方法不能有具体的实现，也就是不能有方法体。同时拥有纯虚函数的类会被默认定义为抽象类。

\#什么是抽象类

至少拥有一个或者一个以上的纯虚函数的类，这种类不能创建对象，但是可以被继承，通常通过继承抽象类，然后通过重写纯虚函数后，方可以通过抽象父类的子类的对象来访问函数和数据成员。

模板函数的重载

```c++
#include <iostream>
using namespace std;
template <typename T>
void Test1(T a, T b)
{
  cout<<"Test1()"<;
}
void Test1(int a, int b)//这里是重载
{
  cout<<"2"<;
}
void Test(int a, int b)
{
  cout<<"3"<;
}
int main()
{
  int a = 0, b = 1;
  char c = 'c', d= 'd';
  Test(a,b);
  Test1(a, c);
}
```

  

类模板的定义：

 

**为什么需要类模板：**有时，有两个或多个类，其功能是相同的，仅仅是数据类型不同，

  

**继承中的类模板语法：**



结论: 子类从模板类继承的时候,需要让编译器知道 父类的数据类型具体是什么(数据类型的本质:固定大小内存块的别名)A 

从模板类派生时，需要具体化模板类，C++编译器需要知道父类的数据类型是什么样子的 要知道父类所占的内存多少

## 单例模式

单例模式有许多种实现方法，

a. 懒汉式：使用的时候才创建，多线程访问的时候线程不安全(双检锁)

b. 饿汉式：类文件加载的时候已经创建好了对象，如果对象一直没有使用，则类对象浪费空间

特点与选择：

如果要进行线程同步，访问量比较大，或者可能访问的线程比较多时，采用饿汉实现，可以实现更好的性能。这是以空间换时间。

在访问量较小时，采用懒汉实现。这是以时间换空间。

CSDN博客：

[https://blog.csdn.net/yuexiaxiaoxi27172319/article/details/81410081?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163910357816780357234401%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=163910357816780357234401&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-81410081.first_rank_v2_pc_rank_v29&utm_term=c%2B%2B%E5%8D%95%E4%BE%8B%E6%A8%A1%E5%BC%8F&spm=1018.2226.3001.4187](https://blog.csdn.net/yuexiaxiaoxi27172319/article/details/81410081?ops_request_misc=%7B%22request%5Fid%22%3A%22163910357816780357234401%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=163910357816780357234401&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-81410081.first_rank_v2_pc_rank_v29&utm_term=c%2B%2B单例模式&spm=1018.2226.3001.4187)



设计原则之一：开放封闭原则：对扩展开放,对修改关闭