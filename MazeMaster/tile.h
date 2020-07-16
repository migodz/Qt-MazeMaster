#ifndef TILE_H
#define TILE_H

#include <QPushButton>
#include <map>

class Tile : public QPushButton
{
    Q_OBJECT
public:
    const static int WALL = 0;
    const static int PATH = 1;
    const static int STARTING = 2;
    const static int ENDING = 3;

    int status; //状态：0为墙，1为普通通路，2为出发点，3为目的地
    int isEditable;
    std::map<int, QString> statusToPath;

    //构造方法，默认不可以编辑（即按钮无效）
    //如果可以编辑，点击按钮会切换图片,用于编辑地图
    Tile(int status, bool isEditable = false);

    void mousePressEvent(QMouseEvent * e);
    void setSize(int Size);
    void changeStatus();//无参改变状态，状态+1；如果比3大，会重置为0
    void changeStatus(int status);//指定状态

signals:

};

#endif // TILE_H
