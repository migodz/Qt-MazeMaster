#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include "imgbutton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化选关窗口
    chooseWindow = new ChooseLevelWindow;

    //初始化编辑迷宫地图窗口
    editWindow = new MapEditWindow;

    //开始按钮
    ImgButton * startBtn = new ImgButton(":/res/startBtn.png");
    startBtn->setParent(this);
    startBtn->move(80+9, this->height()*0.8);
    startBtn->show();
    connect(startBtn, &ImgButton::clicked, [=](){
        //按钮弹跳
        startBtn->jumpDown();
        startBtn->jumpUp();

        //0.3秒后打开选择关卡页面
        QTimer::singleShot(300, this, [=](){
            this->hide();
            chooseWindow->setGeometry(this->geometry());
            chooseWindow->show();
        });
    });

    //制作地图按钮
    ImgButton * editBtn = new ImgButton(":/res/editBtn.png");
    editBtn->setParent(this);
    editBtn->move(80+160+9, this->height()*0.8);
    editBtn->show();
    connect(editBtn, &ImgButton::clicked, [=](){
        //按钮弹跳
        editBtn->jumpDown();
        editBtn->jumpUp();

        //0.3秒后打开地图制作页面
        QTimer::singleShot(300, this, [=](){
            this->hide();
            editWindow->setGeometry(this->geometry());
            editWindow->show();
        });
    });

    //关于按钮
    ImgButton * aboutBtn = new ImgButton(":/res/aboutBtn.png");
    aboutBtn->setParent(this);
    aboutBtn->move(80+160*2+9, this->height()*0.8);
    aboutBtn->show();
    connect(aboutBtn, &ImgButton::clicked, [=](){
        //按钮弹跳
        aboutBtn->jumpDown();
        aboutBtn->jumpUp();

        //0.3秒后打开关于信息对话框
        QTimer::singleShot(300, this, [=](){
             QMessageBox::about(this, "关于迷宫大师", "Code by 阿谜\nLanguage: C++ with Qt\ngithub repository:\n\t migodz / Qt-MazeMaster\n\n本程序为本人C++大作业项目，请勿商用！\n\n\t\t2020-07");
        });
    });

    //退出按钮
    ImgButton * exitBtn = new ImgButton(":/res/exitBtn.png");
    exitBtn->setParent(this);
    exitBtn->move(80+160*3+9, this->height()*0.8);
    exitBtn->show();
    connect(exitBtn, &ImgButton::clicked, [=](){
        //按钮弹跳
        exitBtn->jumpDown();
        exitBtn->jumpUp();

        //0.3秒后关闭程序
        QTimer::singleShot(300, this, [=](){
            this->close();
        });
    });

    //监听编辑地图窗口的返回按钮点击事件
    connect(editWindow, &MapEditWindow::backBtnClicked, [=](){
       editWindow->hide();
       setGeometry(editWindow->geometry());
       this->show();
    });

    //监听选关窗口的返回按钮点击事件
    connect(chooseWindow, &ChooseLevelWindow::backBtnClicked, [=](){
       chooseWindow->hide();
       setGeometry(chooseWindow->geometry());
       this->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent * e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/titile.jpg");
    painter.drawPixmap(0,0,pix);
}
