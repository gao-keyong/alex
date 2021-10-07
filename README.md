# C语言词法分析程序的设计与实现

## 实验内容及要求

1. 可以识别出用C语言编写的源程序中的每个单词符号，并以记号的形式输出每个单词符号。
2. 可以识别并跳过源程序中的注释。
3. 可以统计源程序中的语句行数、各类单词的个数、以及字符总数，并输出统计结果。
4. 检查源程序中存在的词法错误，并报告错误所在的位置。
5. 对源程序中出现的错误进行适当的恢复，使词法分析可以继续进行，对源程序进行一次扫描，即可检查并报告源程序中存在的所有词法错误。

## 实验环境

x86_64-pc-linux-gnu

## 程序设计说明

分别用以下两种方法实现：

1. 采用C/C++作为实现语言，手工编写词法分析程序。（必做）
2. 编写LEX源程序，利用LEX编译程序自动生成词法分析程序。

### 目录结构

```
.
// 手工编写的词法分析程序部分
├── alex                可执行程序(Linux下)
├── alex.cpp            主程序
├── lexer.cpp           语法分析类实现
├── token.cpp           标记类实现
├── lexer.h             语法分析类声明
├── token.h             标记类声明
├── Makefile            工程文件规则
├── README.md           Markdown文档
├── README.pdf          PDF文档
├── def
│   ├── KEYWORD.def     关键词集合
│   ├── PUNCTUATOR.def  标点符号集合
│   └── TOKEN_TYPE.def  标记类型集合
// LEX编译程序自动生成的词法分析程序
├── flex
│   ├── c11             可执行程序(Linux下)
│   ├── c11.lex         LEX源程序
│   ├── lex.yy.c        flex根据LEX源程序生成的.c源代码
│   ├── Makefile        工程文件规则
│   ├── README.md       Markdown文档
│   ├── test1.c         测试程序1
│   ├── test2.c         测试程序2
│   └── test2.png       测试程序2输出效果
// 测试程序
└── test
    ├── test1.c
    └── test2.c
```

### 程序设计说明

#### Token类型及对应的自动机

* Keyword: 关键词。C中的保留字。
* Identifier: 标识符。变量名或函数名。
![Graph_1.jpg](graph/Graph_1.jpg)
* Numerical_Constant: 数值常量。
![Graph_2.jpg](graph/Graph_2.jpg)
* Char_Constant: 字符常量。如`'a'`。
* String_Literal: 字符串常量。如`"bupt\n"`。
* Punctuator: 运算符。
* Error: 异常Token。

#### 程序划分

程序定义了`Lexer`类和`Token`类。

* `Lexer`类: 实现词法分析器。
* `Token`类: 产生标识对象。

对应的成员变量和成员函数的功能在`lexer.h`和`token.h`中的注释中有详细的解释。