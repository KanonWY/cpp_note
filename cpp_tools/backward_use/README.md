## Backward

用于程序崩溃时，打印堆栈信息，方便排查。  
也可以用来 debug 信息，例如吧指定代码位置的 st 打印出来，查看执行流。  
还可以用来快速学习三方库查看执行流。

```bash
# 安装 dw
sudo apt-get install libdw-dev
# 安装 Backward
sudo apt-get install libbackward-cpp-dev
```

### libdw?

`libdw` 是一个用于处理 ELF（Executable and Linkable Format）文件的库，它提供了对程序调试信息的访问和操作功能。ELF 文件是一种常见的可执行文件格式，用于在 Unix-like 系统中存储可执行程序、共享库和核心转储文件等。

主要功能包括：
1、调试信息访问：libdw 可以读取 ELF 文件中的调试信息，包括符号表、类型信息、源代码文件、行号、堆栈跟踪等。

2、符号解析：它可以解析 ELF 文件中的符号信息，包括函数名、全局变量名、局部变量名等。

3、源代码关联：libdw 可以将调试信息与源代码进行关联，使得可以通过调试信息来查找源代码文件和行号。

4、堆栈跟踪解析：它可以解析程序的堆栈跟踪信息，包括调用栈中的函数名、文件名、行号等。

5、类型信息解析：libdw 可以解析程序中使用的各种数据类型的信息，包括结构体、枚举、数组等。

### libdwarf 

`libdwarf` 是一个库，用于处理和解析 DWARF 调试信息，而 DWARF 则是一种调试信息的标准格式。

### DWARF 调试信息规范

DWARF（Debugging With Arbitrary Record Format）是一种用于描述程序调试信息的标准格式。它定义了一种通用的数据结构和布局，用于存储程序的调试信息，包括符号表、类型信息、源代码关联、堆栈跟踪等。


DWARF（Debugging With Arbitrary Record Format）是一种用于描述程序调试信息的标准格式。它定义了一种通用的数据结构和布局，用于存储程序的调试信息，包括符号表、类型信息、源代码关联、堆栈跟踪等。

DWARF 规范的主要目的是为了帮助开发者在调试和分析程序时能够更方便地获取相关的调试信息，并进行相应的操作。它提供了一种标准的方式来描述程序的调试信息，使得不同的编程语言和开发工具之间可以共享和交换调试信息。

DWARF 规范包括以下主要部分：

1. **标准格式**：DWARF 规范定义了调试信息的标准格式，包括如何组织和存储调试信息、如何描述源代码、符号、类型信息等。
2. **数据结构**：DWARF 规范定义了一系列数据结构，用于表示程序的调试信息，包括调试信息条目、符号表、类型信息、源代码关联、堆栈跟踪等。
3. **编码方式**：DWARF 规范定义了调试信息的编码方式，包括如何将调试信息编码成二进制格式，以便存储和传输。
4. **版本管理**：DWARF 规范还包括版本管理机制，用于管理不同版本的规范和兼容性。
