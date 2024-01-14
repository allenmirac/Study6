# C语言的编译过程

## 预处理（Preprocessing）

-E是让编译器在**预处理之后**就退出，不进行后续编译过程；-o是指定输出文件名。

```shell
gcc -E hello.c -o hello.i
```

## 编译（Compilation）

![](https://s2.loli.net/2024/01/13/qWfwmgaBdeONbnZ.png)

这里的编译不是指程序从源文件到二进制程序的全部过程，而是指将经过预处理文件（test.i）之后的程序转换成特定汇编（test.s）代码的过程。

```shell
gcc -S hello.i -o hello.s
```

## 汇编

汇编过程将上一步的汇编代码转换成机器码，这一步产生的文件叫做目标文件，是二进制格式。

```shell
gcc -c hello.s -o hello.o
```

## 链接

链接过程使用链接器将该目标文件与其他目标文件、库文件、启动文件等链接起来生成可执行文件。附加的目标文件包括静态连接库和动态连接库。

```shell
gcc hello.o -o hello
```

## 总结

生成可执行程序过程为成四个步骤：

1、由.c文件到.i文件，这个过程叫预处理。
2、由.i文件到.s文件，这个过程叫编译。
3、由.s文件到.o文件，这个过程叫汇编。
4、由.o文件到可执行文件，这个过程叫链接。

## 静态库

在编译的过程中，会被直接载入到可执行文件中，将多个文件组成一个库，实现复用，以lib开头，以.a结尾，使用ar命令

## 动态库

在程序编译时并不会被连接到目标代码中，而是在程序运行是才被载入。这就带来了一个明显的好处：不同的应用程序如果调用相同的库，那么在内存里**只需要有一份该共享库的实例**，减小了各个模块之间的耦合程度，也减小了可执行文件的体积。

# MakeFile

## 基本格式

```makefile
target ... : prerequisites ...
    command
    ...
    ...
```

make debug执行下面的命令

```makefile
debug:
	echo hello
.PHONY: clean # 伪目标
clean:
	rm *.o
```

## 变量

```makefile
cpp := src/main.cpp
obj := obj/main.o

${obj}: ${cpp}
	g++ -c ${cpp} -o ${obj}
	# $@ target的名称
	# $< 第一个依赖文件
	# $^ 所有的依赖文件

compile: ${obj}

clean:
	rm -rf obj
	# Windows下使用del obj

.PHONY: clean debug
```

## 运算符号

:=：赋值后不可改变

= ：可以改变

?=：在变量尚未设置时才将其设置为值

+=：添加

\：续行符

## 函数

shell

subst、patsubst

foreach

dir

notdir、filter

