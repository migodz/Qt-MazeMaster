#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include <QSound>
#include <QTimer>
#include "mazedata.h"
#include "tile.h"

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QMainWindow
{
    Q_OBJECT

public:
    int lastTime; //剩余时间

    PlayWindow(QString dataPath);
    ~PlayWindow();

    void paintEvent(QPaintEvent *); //绘制背景图
    void keyPressEvent(QKeyEvent *);//重写keyPressEvent监听键盘事件

    //非递归深度优先搜索实现寻路，如果有通路，返回true
    bool findPossibleWay(bool drawPath); //参数设置true，会绘制可行路线，否则不绘制

private:
    Ui::PlayWindow *ui;
    MazeData * mazeData = NULL; //迷宫数据工具
    Tile * tiles[31][31]; //地图图块
    int posX, posY, startX, startY, endX, endY; //当前位置、起点和终点的坐标
    bool isStopped = false; //游戏是否结束
    QSound * winSound = NULL, * failSound = NULL; //胜利和失败音效
    QTimer *  timer1 = NULL; //关卡计时器

    void stopGame(bool isWon); //结束游戏函数，参数表示是否胜利

signals:
    void backBtnClicked();
};

#endif // PLAYWINDOW_H
