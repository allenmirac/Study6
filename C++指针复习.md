## const修饰指针

```c++
#include <iostream>

using namespace std;

class Rectangle{
  public:
  Rectangle(int length, int width){chang=length; kuan=width;}
    void SetLength(int length){ chang=length;}
    int GetLength() const{ return chang;}
  private:
    int chang;
    int kuan;
};

int main(){
  // int a=123;
  // const int *p=&a;//const 修饰*，p可以改
  // cout<<*p<<endl;
  // int b=567;
  // p=&b;
  // cout<<*p<<endl;
  // int const *p1=&a;
  // int * const p2=&b;const 修饰p2，*p2可以改
  // *p2=789;
  // // p2=&a;
  // cout<<*p2<<endl;

  Rectangle *rec=new Rectangle(123, 456);
  Rectangle *rec1=new Rectangle(789, 456);
  cout<<rec->GetLength()<<endl;

  const Rectangle * pRec=rec;
  cout<<pRec->GetLength()<<endl;
  pRec=rec1;//同p
  cout<<pRec->GetLength()<<endl;//只能调用const方法
  Rectangle * const pRec1=rec;//同p2
  cout<<pRec1->GetLength()<<endl;
  Rectangle *pPrec0=rec;
  pPrec0->SetLength(789);//非const方法也可以调用
  cout<<pRec->GetLength()<<endl;

}
```

## 一维数组指针的区别

a代表首地址，在sizeof和&a时当成了数组整体

```C++
#include <iostream>
using namespace std;
#define sz(type) cout<<sizeof(type)<<endl;
int main(){
  int a[3]={12,13,14};
  sz(a);
  cout<<a<<endl;
  cout<<&a<<endl;
  cout<<&a[0]<<endl;
  /*
  12
  0x9ffdb4
  0x9ffdb4
  0x9ffdb4
  */

  cout<<endl;
  cout<<a<<" "<<a+1<<" "<<a+2<<endl;            //a = &a[0] != &a
  cout<<&a<<" "<<&a+1<<" "<<&a+2<<endl;
  cout<<&a[0]<<" "<<&a[0]+1<<" "<<&a[0]+2<<endl;
  /*
  0x9ffdb4 0x9ffdb8 0x9ffdbc  //步长是4，一个int
  0x9ffdb4 0x9ffdc0 0x9ffdcc  //步长是12，三个int，也就是数组长度
  0x9ffdb4 0x9ffdb8 0x9ffdbc  //步长是4，一个int
  */
  return 0;
}
```

## 二维数组指针的区别

注意与一维指针类比区别开来。

指针有减法和比较运算，没有加法运算。

```c++
#include <iostream>
using namespace std;
#define sz(type) cout<<sizeof(type)<<endl;
int main(){
  int a[2][3]={
    {12,13,14},
    {112,113,114}
  };
  sz(a);
  sz(a[0]);
  sz(a[0][0]);
  cout<<a<<endl;
  cout<<&a[0]<<endl;
  cout<<a[0]<<endl;
  cout<<&a[0][0]<<endl;
  
  cout<<endl;//步长是三个元素的数组的长度
  cout<<"a: "<<a<<" "<<a+1<<endl;           //a = &a[0]
  cout<<"&a[0]:"<<&a[0]<<" "<<&a[0]+1<<endl;

  cout<<endl;//步长是一个int
  cout<<"a[0]: "<<a[0]<<" "<<a[0]+1<<endl;  //a[0] = &a[0][0]
  cout<<"&a[0][0]: "<<&a[0][0]<<" "<<&a[0][0]+1<<endl;

  cout<<endl;
  cout<<"&a: "<<&a<<" "<<&a+1<<endl;//步长是二维数组的长度
  return 0;
}
```

## 指针作为函数的参数

使用swap（交换两个变量的值）作为演示，实现Swap的四种方法。当数组名作为**函数的参数**来传递的时候，他的高贵的数组结构特性已经失去了，成了一个地地道道的只拥有4个字节的平民。

```c++
#include <iostream>
using namespace std;
#define SWAP(a, b, temp) (temp=a, a=b, b=temp);
void func(int *x, int *y){
  int z=*x;
  *x=*y;
  *y=z;
}
void func1(int &x, int &y){
  cout<<x<<" "<<y<<endl; 
  int z=x;
  x=y;
  y=z;
}
template<class T>
void Swap(T &a, T&b){
  T temp=a;
  a=b;
  b=temp;
}
int main(){
  float a=12.1, b=13.2;
  cout<<a<<" "<<b<<endl;

  // int c=a;
  // a=b;
  // b=c;
  float c;
  Swap(a, b);
  cout<<a<<" "<<b<<endl;
  return 0;
}
```

## 指针作为返回值

要作为返回值，必须保证调用后返回的指针不会销毁，否则就是野指针。

```c++
#include <iostream>
using namespace std;
int a[3];//全局变量默认初始化为0
int * func(){
  return a;
}
int main(){
  int a=1;
  cout<<func()[0]<<" "<<func()[1]<<endl;
  return 0;
}
```

## 字符数组指针

1、str和p是不相同的！！

```c++
#include <iostream>
#include <cstring>

int main() {
  char str[3]="ab";
  char *p=str;//此处可以修改str，因为str只是字符数组（使用了字符串字面量来初始化str），但是并不是常量
  size_t length = std::strlen(str);//遇到'\0'才会停止计数
  // 例如 char a[3]={'a', 'b', 'c'}, 使用strlen后的长度不是3!!!
  printf("%c-%s-%ld-%ld\n", *str, str, sizeof(str), strlen(str));
  // sizeof(str) 返回的是数组占用的内存大小，而 strlen(str) 返回的是字符串的长度。
  printf("%c-%s-%ld-%ld\n", *p, p, sizeof(p), strlen(p));
  // sizeof(p) 返回的是int型指针占用的内存大小，而 strlen(p) 返回的是字符串的长度。
  *p='A';
  *(p+1)='B';
  std::cout<<*str<<*(str+1)<<std::endl;
  
  return 0;
}
```

2、此处会有一个warning，因为“ab”是存储在常量区域的，不能修改，在使用时需要使用const来申明

```c++
#include <iostream>
#include <cstring>

int main() {
  char *p="ab";// 先在常量区保存好"ab"，然后在栈区建立一个char *
  //   const char *p="ab";
  printf("%c-%s-%ld-%ld\n", *p, p, sizeof(p), strlen(p));
  // sizeof(p) 返回的是int型指针占用的内存大小，而 strlen(p) 返回的是字符串的长度。
  *p='A';
  *(p+1)='B';
  std::cout<<*p<<*(p+1)<<std::endl;
  
  return 0;
}
```

3、此处会显示error: assignment of read-only location '* p'

```c++
#include <iostream>
#include <cstring>

int main() {
  const char *p="ab";
  printf("%c-%s-%ld-%ld\n", *p, p, sizeof(p), strlen(p));
  // sizeof(p) 返回的是int型指针占用的内存大小，而 strlen(p) 返回的是字符串的长度。
  *p='A';
  *(p+1)='B';
  std::cout<<*p<<*(p+1)<<std::endl;
  
  return 0;
}
```

4、字符数组指针的初始化

```c++
#include <iostream>
#include <cstring>
using namespace std;
int main() {
  char a='a';
  char b='b';
  char *p=&a;

  char c[]={a, b, 'c'};
  printf("%p--%p--%c--%c\n", &a, &b, a, b);
  printf("%p--%p--%c--%c\n", c, c+1, *c, *(c+1));
  char *p2[]={&a, p, c};
  for(int i=0; i<3; i++){
    printf("%p--%p--%c\n", &p2[i], p2[i], *p2[i]);
  }
  return 0;
}
```

## 函数指针

主要要把指针括起来

```c++
#include <iostream>
using namespace std;
void func(){
  cout<<"func"<<endl;
}

int add(int a, int b){
  return a+b;
}
int main(){
  int a=123;
  int *p=&a;
  printf("%p--%p\n", &a, p);

  //无参
  void (*pFunc)()=&func;//要不要&，都可
  printf("%p--%p\n", func, pFunc);

  //带参
  int (*pFunc1)(int, int)=&add;
  printf("%p--%p\n", add, pFunc1);
  return 0;
}
```

