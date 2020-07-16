#include "chooselevelwindow.h"
#include "ui_chooselevelwindow.h"

#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QStyle>
#include <QFileDialog>
#include <QDebug>
#include "imgbutton.h"

ChooseLevelWindow::ChooseLevelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseLevelWindow)
{
    ui->setupUi(this);

    //返回按钮
    ImgButton * backBtn = new ImgButton(":/res/backBtn_1.png", ":/res/backBtn_2.png");
    backBtn->setParent(this);
    backBtn->move(600+34, this->height()*0.8+60);
    backBtn->show();
    connect(backBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            emit(backBtnClicked());
        });
    });

    //读取地图按钮
    ImgButton * loadBtn = new ImgButton(":/res/loadBtn_1.png", ":/res/loadBtn_2.png");
    loadBtn->setParent(this);
    loadBtn->move(450+34, this->height()*0.8+60);
    loadBtn->show();
    connect(loadBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            QString path = QFileDialog::getOpenFileName(this,"读取迷宫地图","./","(*.mazedata)");
            //返回值不为空说明成功选取迷宫文件
            if(path!=NULL)
            {
                this->hide();
                playWindow = new PlayWindow(path);
                playWindow->setGeometry(this->geometry());
                playWindow->show();

                //监听游戏窗口的返回按钮点击事件
                connect(playWindow, &PlayWindow::backBtnClicked, [=](){
                    playWindow->hide();
                    setGeometry(playWindow->geometry());
                    delete playWindow;
                    playWindow = NULL;
                    this->show();
                });
            }
        });
    });

    //关卡选择按钮生成
    for (int i = 0; i < 16; i++)
    {
        ImgButton * tempBtn = new ImgButton(":/res/chooseBtn_1.png", ":/res/chooseBtn_2.png");
        tempBtn->setParent(this);
        tempBtn->move(145+(i%4)*150, 90+(i/4)*100);
        tempBtn->show();

        //绑定按钮点击事件
        connect(tempBtn, &ImgButton::clicked, [=](){
            tempBtn->jumpDown();
            tempBtn->jumpUp();
            QTimer::singleShot(100, [=](){
                this->hide();
                playWindow = new PlayWindow(QString(":/mazedata/").append(QString::number(i+1)).append(".mazedata"));
                playWindow->setGeometry(this->geometry());
                playWindow->show();

                //监听游戏窗口的返回按钮点击事件
                connect(playWindow, &PlayWindow::backBtnClicked, [=](){
                   playWindow->hide();
                   setGeometry(playWindow->geometry());
                   delete playWindow;
                   playWindow = NULL;
                   this->show();
                });
            });
        });

        //设置按钮上的数字标签
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(tempBtn->width(),tempBtn->height());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //设置居中
        label->move(145+(i%4)*150, 90+(i/4)*100);
        label->setAttribute(Qt::WA_TransparentForMouseEvents,true);  //鼠标事件穿透
        label->setFont(QFont("黑体",13));
        //label->setStyleSheet("color:white;");
    }
}

ChooseLevelWindow::~ChooseLevelWindow()
{
    delete ui;
}

void ChooseLevelWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background2.jpg");
    painter.drawPixmap(0,0,pix);
}
