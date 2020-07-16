#include "playwindow.h"
#include "ui_playwindow.h"

#include <QTimer>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QStack>
#include <QPair>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QSound>
#include "imgbutton.h"

PlayWindow::PlayWindow(QString dataPath) :
    ui(new Ui::PlayWindow)
{
    ui->setupUi(this);

    //音效加载
    winSound = new QSound(":/res/LevelWinSound.wav",this);
    failSound = new QSound(":/res/LevelFailSound.wav",this);

    //读取迷宫地图文件到mazeData
    mazeData = new MazeData;
    mazeData->loadMaze(dataPath);

    //设置StrongFocus才能监听键盘事件
    setFocusPolicy(Qt::StrongFocus);

    //设置congraLabel初始位置
    ui->congraLabel->move(225, -150);

    //设置关卡最长闯关时间
    lastTime = mazeData->mazeSize * 3;
    ui->timeLabel->setText(QString::number(lastTime));

    //设置关卡计时器
    timer1 = new QTimer;
    timer1->start(1000);
    connect(timer1, &QTimer::timeout, [=](){
        if(lastTime>0)
        {
            lastTime--;
            ui->timeLabel->setText(QString::number(lastTime));
        }
        else
        {
            stopGame(false);
            timer1->stop();
        }
    });

    //返回按钮
    ImgButton * backBtn = new ImgButton(":/res/backBtn_1.png", ":/res/backBtn_2.png");
    backBtn->setParent(this);
    backBtn->move(600+34, this->height()*0.8+60);
    backBtn->show();
    connect(backBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            emit(backBtnClicked());
            timer1->stop();
        });
    });

    //迷宫无解按钮
    ImgButton * noWayBtn = new ImgButton(":/res/noWayBtn_1.png", ":/res/noWayBtn_2.png");
    noWayBtn->setParent(this);
    noWayBtn->move(600+34, this->height()*0.7);
    noWayBtn->show();
    connect(noWayBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            if(!findPossibleWay(false))
            {
                stopGame(true);
            }
            else
            {
                stopGame(false);
            }

        });
    });

    //自动寻路按钮
    ImgButton * solveBtn = new ImgButton(":/res/solveBtn_1.png", ":/res/solveBtn_2.png");
    solveBtn->setParent(this);
    solveBtn->move(600+34, this->height()*0.7+45);
    solveBtn->show();
    connect(solveBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            findPossibleWay(true);
        });
    });

    //根据mazeData绘制地图图块（tiles）
    for(int i = 0 ; i < 31;i++)
        for(int j = 0 ; j < 31; j++)
            tiles[i][j]=NULL;

    for(int i = 0 ; i < mazeData->mazeSize;i++)
    {
        for(int j = 0 ; j < mazeData->mazeSize; j++)
        {
            //寻找起点坐标
            if(mazeData->mazeData[i][j] == Tile::STARTING)
            {
                posX = i; startX = i;
                posY = j; startY = j;
            }

            //寻找终点坐标
            if(mazeData->mazeData[i][j] == Tile::ENDING)
            {
                endX = i;
                endY = j;
            }

            //创建金币对象
            Tile * tile = new Tile(mazeData->mazeData[i][j], false);//不可编辑的图块
            tile->setParent(this);
            tile->move(i*(600/mazeData->mazeSize), j*(600/mazeData->mazeSize));
            tile->setSize(600/mazeData->mazeSize);
            tiles[i][j] = tile;
            tile->lower();
            tile->show();
        }
    }
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background.jpg");
    painter.drawPixmap(0,0,pix);
}

//监听键盘事件，实现移动
void PlayWindow::keyPressEvent(QKeyEvent *e)
{
    if(!isStopped)
    {
        //按w，向上移动
        if(e->key() == Qt::Key_W && posY>0 && mazeData->mazeData[posX][posY-1]>0)
        {

            tiles[posX][posY]->changeStatus(Tile::PATH);
            posY--;
            tiles[posX][posY]->changeStatus(Tile::STARTING);
            if(mazeData->mazeData[posX][posY] == Tile::ENDING)
                stopGame(true);
        }

        //按s，向下移动
        if(e->key() == Qt::Key_S && posY < mazeData->mazeSize && mazeData->mazeData[posX][posY+1]>0)
        {

            tiles[posX][posY]->changeStatus(Tile::PATH);
            posY++;
            tiles[posX][posY]->changeStatus(Tile::STARTING);
            if(mazeData->mazeData[posX][posY] == Tile::ENDING)
                stopGame(true);
        }

        //按a，向左移动
        if(e->key() == Qt::Key_A && posX>0 && mazeData->mazeData[posX-1][posY]>0)
        {

            tiles[posX][posY]->changeStatus(Tile::PATH);
            posX--;
            tiles[posX][posY]->changeStatus(Tile::STARTING);
            if(mazeData->mazeData[posX][posY] == Tile::ENDING)
                stopGame(true);
        }

        //按d，向右移动
        if(e->key() == Qt::Key_D && posX < mazeData->mazeSize && mazeData->mazeData[posX+1][posY]>0)
        {

            tiles[posX][posY]->changeStatus(Tile::PATH);
            posX++;
            tiles[posX][posY]->changeStatus(Tile::STARTING);
            if(mazeData->mazeData[posX][posY] == Tile::ENDING)
                stopGame(true);
        }
    }
}

// 非递归深度优先搜索实现寻路
bool PlayWindow::findPossibleWay(bool drawPath)
{
    //探索路径策略
    int MOVE_STRATEGY[4][2] = {
        {1, 0},  // down
        {0, 1},  // right
        {-1, 0}, // up
        {0, -1}  // left
    };

    //是否走过的标记
    bool gone_mark[31][31];
    for (int i = 0; i < 31; i++)
    {
        for (int j = 0; j < 31; j++)
        {
            gone_mark[i][j] = false;
        }
    }

    QStack<QPair<int, int>> possible_path; // 储存路线的栈
    QStack<QPair<int, int>> pos_stack; // 储存待遍历位置的栈，位置的最后一个总是最先遍历
    pos_stack.push(QPair<int, int>(startX, startY)); // 加入起点

    while(!pos_stack.empty())
    {
            int pos_x = pos_stack.top().first;
            int pos_y = pos_stack.top().second;
            possible_path.push(pos_stack.top());

            // 终点出口
            if(pos_x == endX && pos_y == endY)
            {
                if(drawPath)
                {
                    for(QPair<int, int> p: possible_path)
                    {
                        int tx = p.first, ty = p.second;
                        if(tx!=posX || ty!=posY)
                            tiles[tx][ty]->changeStatus(Tile::ENDING);
                    }
                }
                return true;
                //return possible_path
            }

            //如果不是出口则标记走过
            gone_mark[pos_x][pos_y] = true;

            int wall_count=0 , gone_count=0;

            // 探索顺序与MOVE_STRATEGY相反，因为最后入栈的最先遍历
            for(int i=0; i<4; i++)
            {
                int next_pos_x = pos_x + MOVE_STRATEGY[i][0];
                int next_pos_y = pos_y + MOVE_STRATEGY[i][1];
                // 是否撞墙或已走过
                if(next_pos_x<0 || next_pos_x>=mazeData->mazeSize || next_pos_y<0 || next_pos_y>=mazeData->mazeSize || mazeData->mazeData[next_pos_x][next_pos_y] == Tile::WALL)
                {
                    wall_count++;
                    continue;
                }
                if(gone_mark[next_pos_x][next_pos_y])
                {
                    gone_count++;
                    continue;
                }
                pos_stack.push(QPair<int, int>(next_pos_x, next_pos_y));
            }
            if(wall_count + gone_count == 4)
            {
                pos_stack.pop();
                possible_path.pop();
                if(gone_count != 1)
                    possible_path.pop();
            }
    }
    if(drawPath)
        QMessageBox::information(this,"提示","该迷宫没有通路");
    return false;
}

void PlayWindow::stopGame(bool isWon)
{
    if(!isWon)
    {
        //失败切换congraLabel的pixmap
        QPixmap pix;
        pix.load(":/res/failed.png");
        ui->congraLabel->setPixmap(pix);
        failSound->play();
    }
    else
    {
        //胜利播放胜利音效
        winSound->play();
    }

    if(!isStopped)
    {
        this->isStopped = true;
        //标签落下动画
        ui->congraLabel->raise();
        QPropertyAnimation * anima =  new QPropertyAnimation(ui->congraLabel,"geometry");
        anima->setDuration(1000);
        anima->setStartValue(QRect(ui->congraLabel->x(),ui->congraLabel->y(),ui->congraLabel->width(),ui->congraLabel->height()));
        anima->setEndValue(QRect(ui->congraLabel->x(),ui->congraLabel->y()+150,ui->congraLabel->width(),ui->congraLabel->height()));
        anima->setEasingCurve(QEasingCurve::OutBounce);
        anima->start();

        //停止计时器
        timer1->stop();
    }
}
