# C++类和对象的属性

千钧一发，让一根头发去承受三万斤的重量，但是它没有断。-----余华

## const修饰结构体指针

内部值不能修改，即：只能读，不能写。防止误操作

```cpp
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

## 运算符加号重载

```cpp
//1、成员函数重载+号
Person operator+(Person &p){
    Person temp;
    temp.m_A = this->m_A + p.m_A;
    temp.m_B = this->m_B + p.m_B;
    return temp;
}
//2、全局函数重载+号
Person operator+(Person &p1, Person &p2){
    Person temp;
    temp.m_A = p1.m_A + p2.m_A;
    temp.m_B = p1.m_B + p2.m_B;
    return temp;
}

// 成员函数调用本质
Person p3 = p1.operator+(p2);

// 全局函数调用本质
Person p3 = operator+(p1, p2);
```



## 友元技术

利用友元类技术，可以访问私有类型

当一个非成员函数需要直接访问类的私有成员时，可以将其声明为友元函数。而如果非成员函数不需要直接访问类的私有成员，那么就不必将其声明为友元函数，这种情况下，非成员函数可以通过类的公有接口进行访问，而无需成为友元函数。

```cpp
#include <iostream>
using namespace std;

class MyClass{
private:
    int a_;
    friend class FriendClass;
public:
    MyClass(int a): a_(a){}
    int getA()const {return a_;}
};

MyClass operator+(const MyClass& m1, const MyClass& m2){
    return MyClass(m1.getA()+m2.getA());
}

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

	MyClass my1(20);
	MyClass my2 = my+my1;
	cout << my2.getA() << endl;
    return 0;
}
```



## 构造函数

构造函数也可以重载

创建任何对象都要调用构造函数

## 析构顺序

当类内存在其他类成员时，构造函数顺序是先成员变量的构造函数，后本类的构造函数，析构顺序则相反

```cpp
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

```cpp
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

普通成员变量在类中只能声明可是不能定义，类在定义后并不分配存储空间，而是在定义类的对象的时候才分配存储空间，相反静态的数据成员和静态的成员函数是已经在内存中开辟了内存空间了，所以静态数据成员可以独立的访问在任何类对象没有建立起来都可以访问。

你们班里面有10个人（10个比如高一一班的对象），体育老师分给你们一个篮球（静态成员函数），你们每个人都带了一个篮球（非静态成员数），你们都很小气，自己的球只能自己拍，要是5对5打比赛，那就只能用那个静态的篮球了（每个人都可以拿来用，但是带来的影响是对全体的）。因此，我可以说那个篮球是高一一班的成员。所以也就是说：静态成员函数是类的成员函数（因为高一二班就不能拿来玩），但是这个篮球最后还是要还给老师的，任何私人不得占有。希望这样你能明白，其实在内存空间里面说白了静态的成员的内存是唯一的一份，就是当你在类内声明他时开辟的，但是非静态函数的空间分配是在你实例化对象时创建的。

静态成员变量是所有的类共有的，它有两种访问方式：

1、通过对象访问

2、通过类名访问（因为静态成员函数和静态变量是所有对象共有的一个性质）

静态成员函数调用方式：通过对象或者通过:类名::静态成员函数

静态成员函数不可以访问非静态成员变量，因为静态成员函数是所有对象共有的，当一个对象调用这个函数时，如果修改非静态成员变量，他不知道是哪个对象的变量。无法区分是哪个对象的。

静态成员函数也有访问权限

```cpp
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

~~定义友元类函数，再类外部定义时不能访问private成员，而在类内定义可以，~~

上面说法是错误的，原因是类外那个函数的定义的形参并没有+const，与类内的声明不相同，导致变成了一个普通函数。

程序如下

```cpp
#include <iostream>
#include <cmath>
using namespace std;
class Point
{
    int x;
    int y;

public:
    friend float Distance(const Point &p1, const Point &p2);
    // {
    //     return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    // }
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
float Distance(const Point& p1, const Point& p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}
int main()
{
    Point p1(10, 10), p2(11, 11);
    cout << Distance(p1, p2);
}
```

## 继承

如果子类中有与父类中同名的成员函数，直接调用是调用子类中的，子类中的同名的成员函数会覆盖父类中所有同名成员函数，如果想要访问的话，需要加上父类的作用域

```cpp
#include <iostream>
using namespace std;

class Parent {
public:
    void display() {
        cout << "Parent class display() function" << endl;
    }
};

class Child : public Parent {
public:
    void display() {
        cout << "Child class display() function" << endl;
        
        // 调用父类中的 display() 函数
        // Parent::display();
    }
};

int main() {
    Child obj;
    obj.Parent::display();
    
    return 0;
}
```



## 继承的时候构造析构的顺序

如果没有出现组合，构造顺序是：先按照继承的顺序构造，然后是自己的构造函数

若出现组合，构造顺序：先按照继承顺序构造，然后是成员对象的构造，最后是自己的构造函数

```cpp
#include<iostream>
using namespace std;
class B1
{
public:
    B1(){cout<<"B1"<<endl;}
    B1(int i){cout<<"B1:"<<i<<endl;}
	~B1(){
		cout << "Des B1" << endl;

	}
};
class B2
{
public:
    B2(){cout<<"B2"<<endl;}
    B2(int i){cout<<"B2:"<<i<<endl;}
	~B2(){
		cout << "Des B2" << endl;

	}
};
class C:public B2,public B1
{
public:
    C():B1(10),B2(20){cout<<"C"<<endl;};
	C(int a, int b):B1(a), B2(b){cout<<"C2"<<endl;}
	~C(){
		cout << "Des C" << endl;
	}
};
int main()
{
    C obj;
    return 0;
}
```



## 多继承

优缺点：

[(11条消息) 多继承的概念和优缺点_HappyJandun's 学习笔录-CSDN博客_多继承的优缺点](https://blog.csdn.net/jandunlab/article/details/14110117)

```cpp
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

多重继承的优点很明显，就是**对象可以调用多个基类中的接口**

缺点是如果派生类所继承的多个基类有相同的基类，而派生类对象需要调用这个祖先类的接口方法，就会容易出现二义性。 

通常有两个解决方案：

（1）加上全局符确定调用哪一份拷贝。比如pa.Author::eat()调用属于Author的拷贝。

（2）使用虚拟继承，使得多重继承类Programmer_Author只拥有Person类的一份拷贝。比如在类 Author 和 Programmer 的继承前加入virtual就可以了。

## 虚函数

```cpp
#include <iostream>
#include <string>
using namespace std;

class Animal
{
private:
    string m_sound;

public:
    Animal(string sound) : m_sound(sound) {}
    virtual void makeSomeSound() { cout << "Ainmal" << endl; }
};

class Cat : public Animal
{
private:
    string m_sound;

public:
    Cat(string sound) : m_sound(sound), Animal("animal") {} // 必须显式调用父类的有参构造函数
    virtual void makeSomeSound() override { cout << m_sound << endl; }
};

class Dog : public Animal
{
private:
    string m_sound;

public:
    Dog(string sound) : m_sound(sound), Animal("animal") {}
    virtual void makeSomeSound() override { cout << m_sound << endl; }
};

int main()
{
    // 视他人之疑目如盏盏鬼火，大胆地去走你的夜路吧。
    Animal *animal;
    Dog dog("Wang Wang");
    Cat cat("Miao Miao");
    animal = &dog;
    animal->makeSomeSound();

    animal = &cat;
    animal->makeSomeSound();
    return 0;
}
```

编译器为了实现虚函数，通常会生成虚函数表，在程序运行时通过虚函数动态的查找，然后据此调用正确的函数，这就是动态编译。vbtable是虚基表，虚基指针(vbptr)指向虚基表。

**C++为什么要用虚函数**

[(3条消息) C++为什么要用虚函数_Like a lunatic-CSDN博客](https://blog.csdn.net/noricky/article/details/80051219?ops_request_misc=%7B%22request%5Fid%22%3A%22163806102216780255288237%22%2C%22scm%22%3A%2220140713.130102334.pc%5Fall.%22%7D&request_id=163806102216780255288237&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-2-80051219.first_rank_v2_pc_rank_v29&utm_term=什么时候要用虚函数&spm=1018.2226.3001.4187)

**总结起来：**这样做是为了当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用

要实现多态，需要满足以下几个条件：

1. 子类重写（覆盖）父类的虚函数：在继承关系中，子类必须重写（覆盖）父类中的虚函数，即在子类中重新定义与父类中虚函数同名、同参数列表和同返回类型的函数，并使用关键字 "override" 明确说明这是对父类虚函数的重写。
2. 父类指针或引用指向子类对象：通过将父类的指针或引用指向子类的对象，可以实现多态。这样，在运行时，根据指针或引用指向的对象的实际类型来确定调用的是子类中的实现。
3. 虚函数机制：父类中被子类重写的函数必须声明为虚函数。通过虚函数表和动态绑定机制，确保在运行时根据对象的实际类型来调用正确的函数实现。

定义了虚函数，如果有三辈的继承：爷爷，父亲，儿子，在主函数中，定义一个基类指针，指针指向谁，就调用谁的函数————实现的就是动态多态

## 纯虚函数

**一般语法**

```cpp
virtual void print() =0;//纯虚函数，可以有函数体，可实例化的派生类必须重写它
```

**定义：**

像这样用virtual关键字修饰，并且末尾加上 =0 的格式声明的函数就是纯虚函数，还有一个重要特征，就是在当前类里这个方法不能有具体的实现，也就是不能有方法体。同时拥有纯虚函数的类会被默认定义为抽象类。

> 什么是抽象类：至少拥有一个或者一个以上的纯虚函数的类，这种类不能创建对象，但是可以被继承，通常通过继承抽象类，然后通过重写纯虚函数后，方可以通过抽象父类的子类的对象来访问函数和数据成员。

## 模板函数的重载

```cpp
#include <iostream>
using namespace std;
template <typename T>
void Test1(T a, T b)
{
  cout<<"Test1()"<<endl;
}
void Test1(int a, int b)//这里是重载
{
  cout<<"2"<<endl;
}
void Test(int a, int b)
{
  cout<<"3"<<endl;
}
int main()
{
  int a = 0, b = 1;
  char c = 'c', d= 'd';
  Test(a,b);
  Test1(a, c);
}
```

## 类模板

定义一个简单的Array的类模板：

```cpp
#include <iostream>
using namespace std;

template <class T>
class Array
{
public:
    Array(int size) : size_(size)
    {
        data_ = new T[size];
    }
    ~Array()
    {
        delete[] data_;
    }
    T &operator[](int i)
    {
        return data_[i];
    }
    friend ostream &operator<<(ostream &os, const Array<T> arr)
    {   
        for(int i=0; i<arr.size_; i++)
            cout<<arr.data_[i]<<" "; 
        return os;
    }

private:
    T *data_;
    int size_;
};

int main()
{
    Array<int> a(10);
    for (int i = 0; i < 10; i++)
    {
        a[i] = i + 1;
    }
    // for (int i = 0; i < 10; i++)
        cout << a << endl;

    return 0;
}
```

类模板的定义：类模板是一种通用的类的定义，通过模板可以存储不同数据类型的对象

**为什么需要类模板：**有时，有两个或多个类，其功能是相同的，仅仅是数据类型不同，如`vector`、`deque`、`map`等的实现。

**继承中的类模板：**

```cpp
#include <iostream>
using namespace std;

template <class T>
class Base{
private:
    T value_;
public:
    Base(T value):value_(value){
        cout<<"Base constructed"<<endl;
    }
    
};

template <class T>
class Derived: public Base<T>{
private:
    T value_;
public:
    Derived(T value1, T value2):Base<T>(value1), value_(value2){
        cout<<"Derived constructed"<<endl;
    }
};

int main(){
    Base<int> base(10);
    Derived<int> derived(1, 2);
    return 0;
}
```

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

[C++设计模式(一)单例模式](https://blog.csdn.net/yuexiaxiaoxi27172319/article/details/81410081?ops_request_misc=%7B%22request%5Fid%22%3A%22163910357816780357234401%22%2C%22scm%22%3A%2220140713.130102334..%22%7D&request_id=163910357816780357234401&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-81410081.first_rank_v2_pc_rank_v29&utm_term=c%2B%2B单例模式&spm=1018.2226.3001.4187)

设计原则之一：开放封闭原则：对扩展开放，对修改关闭

## 组合和继承的优缺点

（1）继承

继承是Is a 的关系，比如说Student继承Person,则说明Student is a Person。继承的优点是子类可以重写父类的方法来方便地实现对父类的扩展。

　　继承的缺点有以下几点：

　　①：父类的内部细节对子类是可见的。

　　②：子类从父类继承的方法在编译时就确定下来了，所以无法在运行期间改变从父类继承的方法的行为。

　　③：如果对父类的方法做了修改的话（比如增加了一个参数），则子类的方法必须做出相应的修改。所以说子类与父类是一种高耦合，违背了面向对象思想。

（2）组合

　　组合也就是设计类的时候把要组合的类的对象加入到该类中作为自己的**成员变量**。

　　组合的优点：

　　①：当前对象只能通过所包含的那个对象去调用其方法，所以所包含的对象的内部细节对当前对象时不可见的。

　　②：当前对象与包含的对象是一个低耦合关系，如果修改包含对象的类中代码不需要修改当前对象类的代码。

　　③：当前对象可以在运行时动态的绑定所包含的对象。可以通过set方法给所包含对象赋值。

　　组合的缺点：①：容易产生过多的对象。②：为了能组合多个对象，必须仔细对接口进行定义。



[const是定义为const函数的组成部分，那么就可以通过添加const实现函数重载]()





## 关于C++的常量的一点说明

我们说过，C++“真常量”只针对内置数据类型，自己定义的类，编译器没法帮你“内传折叠”，所以仍是“假常量”，是能通过指针“篡次的
另外常量是“建没性”的预防措施，并不能防蓄意破坏。

C++中的真常量只针对其内置数据类型，对于自己定义的类的，是假常量，可以通过指针修改

## 菱形的继承体系

```cpp
class A
{
public:
  int a;
};
class B : virtual public A
{
public:
  int b;
};
class C :virtual public A
{
public:
  int c; 
};
class D : public B, public C
{
public:
  int d;
};
```

内存布局

上面这种菱形的继承体系中，如果没有virtual继承，那么D中就有两个A的成员int a；继承下来，使用的时候，就会有很多二义性。而加了virtual继承，在D中就只有A的成员int a；的一份拷贝，该拷贝不是来自B，也不是来自C，而是一份单独的拷贝，那么，编译器是怎么实现的呢？？

在回答这个问题之前，先想一下，sizeof(A),sizeof(B),sizeof(C),sizeof(D)是多少？（在32位x86的linux2.6下面，或者在vc2005下面）在linux2.6下面，结果如下：sizeof(A) = 4; sizeof(B) = 12; sizeof(C) = 12; sizeof(D) = 24;sizeof（B）为什么是12呢，那是因为多了一个指针（这一点和虚函数的实现一样），那个指针是干嘛的呢？

那么sizeof(D)为什么是24呢？那是因为除了继承B中的b，C中的c，A中的a,和D自己的成员d之外，还继承了B，C多出来的2个指针（B和C分别有一个）。再强调一遍，D中的int a不是来自B也不是来自C，而是另外的一份从A直接靠过来的成员。

如果声明了D的对象d： D d；

那么d的内存布局如下：

vb_ptr: 继承自B的指针

int b： 继承自B公有成员

vc_ptr：继承自C的指针

int c： 继承自C的共有成员

int d： D自己的公有成员

int a： 继承自A的公有成员

## 关于多态

多态分为静态多态和动态多态：

* 静态多态是编译时发生了多态，多指函数重载和模板实现；

* 动态多态是运行时多态，一般通过虚函数实现。

实现动态多态三个条件

1：要有public继承；

2：要有虚函数重写；

3：用父类指针（父类引用）指向子类对象

**菱形继承虚基类**

虚继承的出现就是为了解决重复继承中多个间接父类的问题的，二义性。钻石型的结构是其最经典的结构。

C++中的虚函数的作用主要是实现了多态的机制。关于多态，简而言之就是用父类型别的指针指向其子类的实例，然后通过父类的指针调用实际子类的成员函数。这种技术可以让父类的指针有“多种形态”，这是一种泛型技术。所谓泛型技术，说白了就是试图使用不变的代码来实现可变的算法。比如：模板技术，RTTI技术，虚函数技术，要么是试图做到在**编译时决议**，要么试图做到**运行时决议**。

**类的缺省参数：（不能在两个地方同时声明）**

**既可以在类的声明中，也可以在函数定义中声明缺省参数，但不能既在类声明中又在函数定义中同时声明缺省参数。**

# 杂谈

## Union

```cpp
#include<iostream>
using namespace std;
union myun
{
    struct { int x; int y; int z; }u;
    short k;
}a;
int main()
{
    a.u.x = (1<<17)+2; // 2的17次方是： 131072, 低16位是0x00 02
    a.u.y = 5;
    a.u.z = 6;
    a.k = 0;
    printf("%d %d %d %d\n", a.u.x, a.u.y, a.u.z, a.k); // a.k的16位覆盖了x的低16位
    printf("sizeof(a):%d\n", sizeof(a));    // 联合体的 size 是最大的数据长度：三个 int 是12
    cout << ((1 << 17)+2);
    return 0;
}
// 这里short的16位覆盖了a.u.x的前16位
```

​             

我们都知道联合(也叫共用体)中的不同类型的变量都是放在同一个地址开始的内存单元里面，当然这个内存单元足够大，可以容下**所占空间最大的那个数据类型**。

## explicit关键字

在构造函数前面加上，为了防止其他类型的变量隐式转化为这个类的类型变量必须使用显式的转化

vector vs = {"hello", "world"};比如这里就将const char转为string，但是加上`explicit`之后就不可以了

## auto关键字的缺点

auto 不利：降低代码可读性、可能得不到你预想的类型、配合decltype有意想不到的结果

```cpp
2022-4-18
// (4)
for (auto &elem:vec) // pass by reference
    elem *= 3;
// (5) 编译器会把(4)转换为(5)
for (auto _pos = l.begin(), _end = l.end(); _pos != _end; ++_pos) {
    auto &elem = *_pos;
    elem *= 3;
}
auto ll1 = [](int x) -> int {
    return x + 10;
};
// lambda 匿名函数 的简单使用
function<int(int x)> ll = [](int x) -> int {
    return x + 10;
};
cout<<ll1(10)<<endl;
cout<<ll(11)<<endl;
return 0;
```

## lambda表达式

Lambda 表达式是 C++11 引入的一个重要特性，它允许我们定义匿名函数，从而使代码更加简洁和易于阅读。

Lambda 表达式的一般形式如下：

```cpp
Copy Code[capture list](parameter list) -> return type {function body}
```

其中：

- `capture list`：捕获列表，用于指定在函数体中使用的变量。
- `parameter list`：参数列表，指定函数的输入参数。
- `return type`：返回类型，指定函数的返回类型（可以自动推导）。
- `function body`：函数体，包含函数需要执行的代码。

```cpp
#include <iostream>

int main() {
    auto func = [] (int x, int y) -> int {
        return x + y;
    };

    std::cout << func(1, 2) << std::endl;  // 输出 3

    return 0;
}
```

与 `auto` 关键字

## 单例模式

### 简单单例模式

```cpp
//懒汉模式：只有在第一次用到的时候才会实例化
class singleton {
    private:
        singleton() {}
        static singleton *p;
    public:
        static singleton *instance();
};
singleton *singleton::p = nullptr;
singleton* singleton::instance() {
    if (p == nullptr)
        p = new singleton();
    return p;
}
//将构造函数写在private或者protected里面，
//同时有一个static指针维护这个单例
//实例的实现由一个public方法来实现，该方法返回该类的唯一实例。
//当然这个单例模式在单线程下面是安全的，
//如果是多线程下同时检测到p是nullptr，会创建两个实例

//饿汉模式
class singleton {
private:
    singleton() {}
    static singleton *p;
public:
    static singleton *instance();
};

singleton *singleton::p = new singleton();//实例化了
singleton* singleton::instance() {
    return p;
}
// 当然这个是线程安全的，对于我们通常阐述的线程不安全，为懒汉模式
```

`decltype` 的意思是获得表达式的 `type` ，有点像 `typeof`。

`final` 用在类后面 `class Base final {}` 表示这个类是继承的最后一个类，不能有其他类继承它

### 加了锁的单例模式

```cpp
#include <iostream>
#include <thread>         // std::thread 
#include <mutex>          // std::mutex
using namespace std;

/*用互斥量构建一个枷锁类*/
class Lock {
private:
    mutex mtx;//互斥量
public:
    void lock() { mtx.lock(); }//加锁
    void unlock() { mtx.unlock(); }//解锁
};
/*静态单例*/
class singleton {
private:
    int _value;//测试用值，静态单例在哪个线程中被第一次获取，_value的值就是哪个线程的id
    singleton(int value);//构造函数
    singleton(const singleton&);//复制构造函数
    singleton& operator=(const singleton&);//赋值构造函数
public:
    static Lock* Locker;//声明一个枷锁指针
    static singleton* Instance;//声明一个对象

    static singleton* getInstance(int thread_id);//声明获取单例的函数
    int getValue();//获取测试值
    ~singleton();//析构函数
};

singleton::singleton(int value) :_value(value) {}//构造函数
singleton::singleton(const singleton&) {}//复制构造函数
singleton& singleton::operator=(const singleton&) { return *this; }//赋值构造函数
singleton::~singleton() {}//析构函数

Lock* singleton::Locker = new Lock();//初始化静态单例中的枷锁
singleton* singleton::Instance = NULL;//初始化静态单例

singleton* singleton::getInstance(int thread_id) {//定义获取单例的函数
    if (Instance == NULL) {//由于加锁比较费事，所以先判断一次
        Locker->lock();
        if (Instance == NULL) {
            Instance = new singleton(thread_id);//初始化静态单例，参数为 线程的id
        }
        Locker->unlock();
    }
    return Instance;
}
int singleton::getValue() {//获取测试值
    return _value;
}

mutex myMtx;//为了防止多个线程争夺 标准输出资源而设置的互斥量
void print_thread_id(int thread_id) {//线程函数
    singleton* Instance = singleton::getInstance(thread_id);//获取静态单例
    myMtx.lock();
    cout << "thread_id: " << thread_id << endl;
    cout << "Instance is Created in thread: " << Instance->getValue() << endl;
    myMtx.unlock();
}

int main() {
    thread threads[2];//两个线程
    int i;
    for (i = 0; i < 2; ++i)
        threads[i] = thread(print_thread_id, i);//启动线程

    for (auto& th : threads) th.join();//等线程结束
    return 0;
}
```

```cpp
#include <iostream>
#include <thread>         // std::thread 
#include <mutex>          // std::mutex
using namespace std;
class lock12 {
private:
	mutex mtx;
public:
	void Lock() { mtx.lock(); }
	void unLock() { mtx.unlock(); }
};

class Singleton {
private:
	int value;
	Singleton(int _value) :value(_value) {};
	Singleton(const Singleton& single) {};
	Singleton& operator=(const Singleton&) { return *this; }
public:
	static lock12* lock1;
	static Singleton* instance;
	static Singleton* getInstance(int thread_id);
	int get_value() { return value; }

	~Singleton() {};
};
lock12* Singleton::lock1 = new lock12();
Singleton* Singleton::instance = NULL;
Singleton* Singleton::getInstance(int thread_id) {
	if (instance == NULL) {//双检锁 1
		lock1->Lock();
		if (instance == NULL) {//2
			instance = new Singleton(thread_id);
		}
		lock1->unLock();
	}
	return instance;
}
mutex mtLock;
void print_thread_id(int thread_id) {
	Singleton* instance = Singleton::getInstance(thread_id);
	
	mtLock.lock();
	cout << thread_id<<" ";
	cout << instance->get_value()<<endl;
	mtLock.unlock();
}
int main() {
	thread thread1[3];
	for (int i = 0; i < 3; i++) {
		thread1[i] = thread(print_thread_id, i);
	}
	for (auto& th : thread1) th.join();//等线程结束
	return 0;
}
```

### Scott Meyer/简洁的singleton写法

```cpp
Singleton *Singleton::getinstance(){    static Singleton p;    return &p; }               
```



## 解耦

**程序设计经常提到的解耦，到底是要解除什么之间的耦合**

解耦，就是将程序积木化

就像我们玩的积木一样，各个积木可以**组合**在一起而形成一个形状，又可以**拆分**，又可以**替换**，因为基本上各个积木块都是**独立**的，只要他们之间的接口（形状）匹配，就可以灵活地组合在一起。

当然，这是理想状态。**解耦是在逐达到这个理想状态。**

在实际开发中有几种方式实现解耦：

1、使用接口隔离

2、采用事件驱动架构

3、使用设计模式，提高灵活度