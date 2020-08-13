# Qt-MazeMaster

一个利用Qt/C++写的GUI迷宫游戏。 A GUI maze game made by using Qt/C++.

## 文件说明

工程文件（源代码和图像素材）在MazeMaster文件夹下；Windows下可执行程序在Executable文件夹下。

## 程序设计说明

迷宫大师是本人的C++程序设计的大作业，是一个可视化的迷宫小游戏。可视化界面基于Qt5，使用Qt Creator开发。

项目主要有如下特点：

- 对Qt自带的控件进行了二次封装，以实现更加美观的游戏效果；
- 为了更方便的设计关卡，配套开发了可视化的迷宫地图编辑器，并实现了复杂迷宫地图的深度优先生成；
- 迷宫游戏实现了文件的读写，实现了导出和读取玩家自制地图，并加入了通过DFS绘制迷宫出路的功能；

项目的设计树形图如下：

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872988362.png)


## 演示截图

### 主窗口

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872737075.png) 

### 地图编辑窗口

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872790176.png)

生成的效果如下图：

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872801315.png)

### 选关窗口ChooseLevelWindow

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872838783.png)

### 游玩窗口PlayWindow

![image](https://github.com/migodz/Qt-MazeMaster/blob/master/assets/1594872853885.png)


## 详细介绍

见本人的个人博客：[http://migod.top/176.html](http://migod.top/176.html)
