#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chooselevelwindow.h"
#include "mapeditwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //选关窗口
    ChooseLevelWindow * chooseWindow = NULL;
    //编辑地图窗口
    MapEditWindow * editWindow = NULL;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //重写paintEvent，绘制背景图
    void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
