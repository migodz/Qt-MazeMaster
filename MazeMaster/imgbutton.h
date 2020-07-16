#ifndef IMGBUTTON_H
#define IMGBUTTON_H

#include <QPushButton>
#include <QSound>

class ImgButton : public QPushButton
{
    Q_OBJECT
public:
    QString normalImgPath; //保存默认未按下时的图片路径
    QString pushedImgPath; //保存默认按下时的图片路径

    //构造方法，若按下和普通状态图像相同可以缺省按下时的图片路径
    ImgButton(QString normalImgPath, QString pushedImgPath = "");

    void jumpUp(); //按钮向上移动动画
    void jumpDown(); //按钮向下移动动画
    void mousePressEvent(QMouseEvent *); //重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *); //重写鼠标释放事件

private:
    QSound * sound = NULL; //按钮音效

signals:

};

#endif // IMGBUTTON_H
