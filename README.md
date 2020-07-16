# Qt-MazeMaster

一个利用C++和Qt写的GUI迷宫游戏。 A GUI maze game made by using C++ and Qt

## 文件说明

工程文件（源代码和图像素材）在MazeMaster文件夹下；Windows下可执行程序在Executable文件夹下。

## 程序设计说明

### 程序模块结构示意图
![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872988362.png)



### 使用Qt GUI框架

Qt是一个成熟的跨平台的C++图形用户界面应用程序框架。选择Qt的作为GUI框架相对于选择OpenGL可以节省很多不必要的开发时间，提升开发效率的同时保证稳定性。Qt提供了一些工具类，比如QString，QDebug等。笔者选择使用Qt自带的这些类而不是C++本身提供的string，cin，cout这些工具类，以保证程序内部信息流交互更加方便。
 


### 封装图片按钮类ImgButton

手动封装一个自己的图片按钮类：ImgButton。这个类继承QPushButton。利用这个类和笔者自己用Photoshop制作的按钮图片素材，实现了比Qt自带的QPushButton更美观的按钮。笔者还为按钮添加了音效。



### 主窗口（标题界面）MainWindow

主窗口将选关窗口和地图编辑窗口作为数据成员，便于进行与这些界面间的切换。

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872737075.png) 



### 封装地图图块类Tile

通过继承QPushButton，我封装了一个地图图块类Tile，利用这个类和图片素材绘制迷宫地图。利用网上的图像素材，笔者制作了地图图块素材（墙、路、起点、终点）



### 迷宫数据工具类MazeData

设计迷宫数据文件：Unicode编码的文本文件，第一行n为迷宫大小（迷宫宽高相等），之后n行是迷宫地图信息。
如：

```
11
03000000000
01111111110
00000000010
01111111010
01000001010
01011111010
01010001010
01010211010
01010000010
01011111110
00000000000
```

为了方便我们在编辑窗口和游玩窗口快速读写迷宫文件，设计了一个迷宫数据工具类MazeData，利用二维数组保存迷宫数据。



### 地图编辑窗口MapEditWindow

加入地图编辑窗口，既方便笔者自己制作迷宫关卡，也为玩家提供了扩展游戏玩法和难度的可能性。通过笔者封装的Tile类，可以实现鼠标点击绘制。

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872790176.png)

地图编辑窗口类包含了本程序的核心难点之一：迷宫地图深度优先生成算法。

```
设置 迷宫内所有点 为 墙壁
设置 (1,1)点 为 路
把  (1,1)点 入栈
设置 迷宫连通标志 为 未连通
如果  迷宫连通标志 为 未连通，执行循环：
    设置 当前点 为 栈内最后一个点 //因为最后一个储存的是最深的
    寻找 当前点 潜在的邻居 //潜在邻居上下左右的墙都是未打通的，且和当前点只隔了一个墙
    如果 存在潜在的邻居：
		  设置 任意一个 潜在的邻居 为 真邻居
		  打通 当前点 与 真邻居 之间的墙
		  把  真邻居 入栈
	  否则：//该路径没有合适的邻居,去除最后一个不满足要求的,往上一个点寻找
		  出栈 
	  判断地图是否连通，把结果储存到 迷宫连通标志

```

算法流程图如下

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872824516.png)

生成的效果如下图：

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872801315.png)



### 选关窗口ChooseLevelWindow

选关窗口将游玩窗口为该类的数据成员，便于窗口间切换。除了内置关卡，笔者在选关窗口增加了读取地图的按钮，以期拓展游戏的可玩性。

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872838783.png)



### 游玩窗口PlayWindow

游玩窗口是游戏的核心窗口。游玩窗口实现玩家移动功能、判定是否存在通路功能、绘制通路功能、游戏倒计时功能，还增加了成功或者失败时的简单动画。

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872853885.png)

游玩窗口类包含了本程序的另一个难点：深度优先搜索迷宫通路（笔者采用了利用栈的非递归的实现）。这个算法使用在此类的findPosssibleWay成员函数里。算法的思路如下：

1. 首先起点入栈``pos_stack``，只要``pos_stack``中有值，说明还有待遍历的位置，继续遍历
2. 进入循环体，说明该点被遍历，该点加入``possible_path``栈
3. 判断该点是否为出口，如果是，已经发现一条可行路线，返回true`，函数结束
4. 如果循环向下执行，说明不是终点，将该点标记为已经走过
5. 从该点探索与该点连接的，其他可走的位置，入栈
6. 如果没有任何点入栈，说明是死路或者正在回退中，进行一步回溯，即出栈
7. 如果离开循环体，说明没有通路，返回false
