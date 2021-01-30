# 图书管理系统作业（B班）

##### SJTU-ACM Data-Structure 2021

## 作业目的

体验工程向软件开发过程，为下学期数据结构课程大作业打基础。

## 基本要求

本次大作业需要使用C++实现一个网上书店管理系统，要求如下:

* 要求将数据存储在**文件**中
* 支持本文档中给出的命令操作
* 通过给定的数据测试，不要求完成GUI(当然允许并支持，但不推荐)，但需提供命令行界面来操作该系统(实现简单的CLI命令行交互)

## 交互模式

### 关于模式

通过命令行进行的直接操作

* 注：可以实现通过数字选择进行操作的用户界面，或实现更加友好的命令行界面。但不做强制要求。

注意:若程序启动时检测到当前为首次启动(之前未初始化)，则会进行一次初始化，其包括:

* 创建用户管理的相关文件结构，新建root用户(root的id[user-id]是root，用户名称[name]为root，默认密码[passwd]为sjtu)
* 创建书目、订单、日志管理的相关文件结构

## 关于数据测试

测试程序通过**stdin**接收输入数据，**stdout**输出。

测试中可能反复打开关闭提交的程序以验证是否正确完成文件存储。

也可能使用较多的命令以测试是否正确完成存储所用的数据结构。

**存在部分鲁棒性测试点，其中包含部分不合理操作（如命令不符合语法，买书时货不足，执行超过自己权限的命令等）。对于这些操作，输出``Invalid\n``并不要执行。**

**像这样**

```
Invalid

```

## 用户系统

网上书店管理系统服务于**老板(超级管理员)**，**员工(管理员)**以及**客户(常规权限)**，满足其不同需求，故应该存在对应的用户管理部分。

### 角色划分

* 老板：使用root账户（密码为sjtu），可以访问系统所有功能，可以进行用户管理。
* 员工：可以且仅可以访问与自己业务相关的功能、数据。
* 顾客：
  * 可以进行账户注册，查询书目以及书目购买，不能访问其他功能。
  * 在权限上满足子集关系，即老板7>员工3>顾客1>未登录0。(权限编号7，3，1，0仅为标识)
  * 除root账户默认进行创建外，账户在创建时会规定账户类型，账户类型不可更改。

下面提到的命令后会通过对应数字注明权限信息，即：

* 7为老板(root)专用
* 3为员工、老板均可使用
* 1为所有登录人员均可使用
* 0为未登录状态下也可使用

### 相关命令

用户系统使用如下命令:

**注：右边备注数字为执行该命令所需最低权限。即：当前用户登录态`privilege >= 该数字`可执行此命令**

```sh
su [user-id] [passwd] #0:登录到某用户，从高权限用户登录到低权限不需填写密码
logout #1:返回到未登录状态
useradd [user-id] [passwd] [7/3/1] [name] #3:增加一个指定权限的用户，只能创建小于自己权限的账户
register [user-id] [passwd] [name] #0:注册一个带有这些信息的权限1用户
delete [user-id] #7:删除某用户
passwd [user-id] [old-passwd(if not root)] [new-passwd] #1:root不需要填写旧密码，其余账户需要
```

`[user-id]`、`[passwd]`包含连续无空格的数字字母下划线，不超过30个字符。

` [name]` 包含连续无空格的字符串，不超过30个字符。

## 数据操作

### 不同角色的数据访问

* 老板: 了解财务情况，了解员工情况，了解库存，管理各类用户，查看日志
* 员工: 查看自己的操作记录，添加图书种类并进货，了解库存，修改书目信息
* 客户: 查询图书，购买图书

### 相关指令

为了简化指令模式，我们规定选定(select)机制(只能选择唯一图书)，修改和进货都在选中后进行。

注意:每次打开程序时默认select空对象，若此时进行import或modify视为Invalid。

**注：右边备注数字为执行该命令所需最低权限。即：`privilege >= 该数字`的账户可执行此命令**

```sh
select [ISBN] #3:选定ISBN为指定值的图书，若不存在则创建该ISBN的书并将其余信息留空等待modify进行填写
modify -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] -price=[price] #3:参数可省略，也不要求输入顺序，会更新(替换而非添加)上次选中的书至新的信息
import [quantity] [cost_price(in total)] #3:将上次选中的书以总共[cost_price]的价格进[quantity]本
show -ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] #1:参数可省略，也不要求输入顺序，将匹配的图书以ISBN号排序输出，需要注意该命令参数只支持单参数，如果参数是keyword，只支持单关键词查询
show finance [time] #7:time项省略时，输出总的收入与支出;否则输出近[time]次进货、卖出操作(分别算一次)的合计收入支出。
buy [ISBN] [quantity] #1:购买该ISBN号的图书[quantity]本
```

其中

* `[ISBN]` 为不超过20位的连续字符串
* `[name]` 和` [author]` 被双引号包起来(不含双引号)，保证
  * 本身内容没有双引号
  * 内容不超过60个英文字符
* `[keyword]` 整体被双引号包起来(不含双引号)，关键字之间| (竖线)隔开，保证
  * 每个关键字本身内容无引号以及空格
  * 内容总共不超过60个英文字符
* `[quantity]` 为整数(<1000000)

对于show操作，输出格式为:每行一本，所有信息项目间以`\t`隔开，价格保留两位小数，如:

```sh
$ show #允许无参数情况
9787532736553 The_Man_Who_Changed_China Robert_Lawrence_Kuhn China|Jiang 59.00 1926
9787111631101 Computer_Architecture_A_Quantitative_Approach John_L_Hennessy Computer_Architecture 238.70 1000
```

对于`show finance`操作，格式为`+ [收入] - [支出]\n`(不含大括号)，如:

```
+ 200.94 - 150.50

```

**注意：非管理员输入命令式的界面应该对无参数的show调用进行一定程度的限制**

## 性能要求

* 所有的数据应以文件形式存储并进行维护。
* 不允许把所有东西直接放进内存，启动的时候加载然后关闭的时候放回，要动态地对文件进行访问和修改。
* 要求至少使用块状链表，有能力者也可使用B+树。**注意：不推荐使用哈希+文件系统索引的做法。你的程序所创建的文件数量要求小于等于20**
* 对于不同参数的show命令要建立不同的索引(倒排文档)。

## 命令行界面

以下操作在测试中不会涉及，但需要实现，分数计算在用户界面(CLI)分数中

```sh
report finance #7:会生成一张赏心悦目的财务报表，格式自定
report employee #7:会生成一张赏心悦目的员工工作情况表，记录其操作，格式自定
log #7:会返回赏心悦目的日志记录，包括系统操作类的谁干了什么，以及财务上每一笔交易情况，格式自定
report myself #3:返回员工自己的操作记录，格式自定
```

## 截止日期

2.21 23:00

需提交文档和代码，具体提交方式在ddl前会说明。

## 代码审查

即Code Review，时间安排在开学后。

检查内容包括：

* 开发文档完善程度
* 正确通过测试**(可能包括额外的鲁棒性测试，未能通过会导致少量扣分)** 
* **OOP：过少或不当的封装将导致少量扣分**
* 与文档一致性：**代码实现与开发文档差异过大将大量扣分**
* 用户界面(CLI)的实现

## 补充内容

* 用户管理系统支持嵌套登录，可以用栈进行维护。不同层的select状态相互无关。即：可以在`su logout`前后`select`不同书目。（具体请参考样例）
* `buy`命令输出购买花费，保留两位小数。
* 本题`show finance`等命令无需考虑`double`型有效数字导致的误差问题。
* 添加命令`quit`，用于退出程序。即：退出条件为：读到`EOF`或读到`quit`。
* 高权限用户低权限用户，提供密码，但是密码错误，为`Invalid`。
* 对于`show`操作，按字典序输出，库存为0的书也要正常输出；如果没有符合条件的书，输出一个空行`\n`。

* 添加命令`exit`，用于退出程序。即：退出条件为：读到`EOF`或读到`exit`。（和`quit`并存）
* 图书信息中的`[name]`, `[author]`, `[keyword]`均不包含不可见字符。

## 一个样例

```sql
su root sjtu
useradd rbq rbqqqaq 3 rbq
select ISBN1
modify -name="name1" -author="author1" -keyword="keyword1" -price=10.00
show
// Output: ISBN1 name1 author1 keyword1 10.00 0
su rbq
select ISBN1
modify -ISBN=ISBN2
show
//Output: ISBN2 name1 author1 keyword1 10.00 0
su root sjtu
select ISBN2
modify -ISBN=ISBN3
show
//Output: ISBN3 name1 author1 keyword1 10.00 0
logout
modify -price=20.00
show
//Output: ISBN3 name1 author1 keyword1 20.00 0
logout
show
//Output: ISBN3 name1 author1 keyword1 20.00 0
modify -ISBN=ISBN4
show
//Output: ISBN4 name1 author1 keyword1 10.00 0
logout
```

## Q&A

+ 关于show操作
  + show操作有五种可能：
    + 用show -ISBN=1234搜索ISBN为1234的书
    + 用show -name=abab搜索名称为abab的书
    + 用show -author=cong258258搜索作者为cong258258的书
    + 用show -keyword=qwq来搜索包括qwq关键词的书
    + 用show（后面什么都不加，无参数）来展示所有书目
  + 每次搜索只能在上述五种中选一种，即show后加的参数只能有至多一个，例如，不能搜索作者为cong258258且名称为abab的书。
  + 虽然一本书可能有很多关键词，但在此操作中依然只能搜索一个关键词，比如不支持搜索关键词包含qwq以及qaq的书目这种操作。
  + 输出时按照ISBN从小到大输出。
+ 关于合法字符
  + 在命令中可能会出现非英文ASCII范围内的字符，但理论上不需要特殊处理。