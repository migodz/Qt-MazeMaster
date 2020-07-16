#include "tile.h"

#include <QDebug>

Tile::Tile(int status, bool isEditable)
{
    this->status = status;
    this->isEditable = isEditable;

    statusToPath[0] = QString(":/res/wall.jpg");
    statusToPath[1] = QString(":/res/ground.jpg");
    statusToPath[2] = QString(":/res/starting_place.jpg");
    statusToPath[3] = QString(":/res/destination.jpg");

    QPixmap pix;
    if(!pix.load(statusToPath[status]))
    {
        qDebug() << "图片加载失败";
        return;
    }

    //设置按钮大小为图片大小
    setFixedSize(40, 40);

    //设置按钮为不规则样式
    setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    setIcon(QIcon(pix));
    setIconSize(QSize(40, 40));
}

void Tile::mousePressEvent(QMouseEvent * e)
{
    if(isEditable)
        return;
    return QPushButton::mousePressEvent(e);
}

void Tile::setSize(int Size)
{
    setFixedSize(Size, Size);
    setIconSize(QSize(Size, Size));
}

void Tile::changeStatus()
{
    if(status != 3)
        status++;
    else
        status = 0;

    //设置图标
    QPixmap pix;
    pix.load(statusToPath[status]);
    setIcon(QIcon(pix));
}

void Tile::changeStatus(int status)
{
    this->status = status;
    //设置图标
    QPixmap pix;
    pix.load(statusToPath[status]);
    setIcon(QIcon(pix));
}

