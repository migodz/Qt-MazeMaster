#ifndef MAPEDITWINDOW_H
#define MAPEDITWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "tile.h"

namespace Ui {
class MapEditWindow;
}

class MapEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapEditWindow(QWidget *parent = nullptr);
    ~MapEditWindow();

    //绘制背景图
    void paintEvent(QPaintEvent * e);

private:
    Ui::MapEditWindow *ui;
    int mazeWidth = 15; //迷宫宽度（大小），默认15
    Tile * tiles[31][31]; //图块

    void createTiles();//在窗口加载地图图块
    QVector< QVector<int> > createMaze(int width); //利用DFS随机生成迷宫地图

signals:
    void backBtnClicked();
};

#endif // MAPEDITWINDOW_H
