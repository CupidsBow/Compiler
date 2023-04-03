# compiler
一个小型的c语言编译器

### 目前实现的功能

1. 可以定义多个变量，并且能初始化。
2. 可以支持基本的加减乘除运算。
3. 可以支持带括号的多个变量的四则混合运算。
4. 可以支持单行注释和多行注释。
5. 可以输出%c格式的字符和%d格式的整数。
6. 可以定义int 类型和 char类型。
7. 可以输出字符串
8. 可以输出2位整数
9. 可以支持简单的if{}else{}语句。（大于和小于比较条件）
10. 以上支持的语法可以混合在同一个源程序中。 
11. 部分错误语法可以报错。

### 环境

- 编辑器：记事本、VC++6.0
- 汇编环境：dosbox、emu8086
- 汇编器、链接器：MASM、LINK
- 汇编语言：8086汇编
- 源代码：C++，用VC++6.0 / gcc编译

### 编译方法

> 目录中的a.out文件是在Linux下编译完成的可执行文件，可以直接运行。

该程序源代码中并未使用 C++11 标准，所以无论是在windows下还是在Linux平台下，只需要按照编译普通C++程序的方法即可。

### 运行方法

假设已经编译完成，生成可执行文件名：compiler

假设待编译的c程序为test1.c

#### 方法1：
直接双击可执行文件运行，根据提示输入待编译的c程序文件test1.c的路径，编译后在该路径下生成相同文件名的test1.asm文件。

#### 方法2：在终端（windows下为dos）运行下面命令

默认生成相同文件名的.asm汇编文件
- compiler  test1.c

可以指定编译生成的文件名和存放位置
- compiler  test1.c   T.asm

> 可以使用目录中的测试代码测试结果。

### 设计流程

扫源代码 --> 词法分析 --> 语法分析 -->目标代码 --> 汇编代码