#include "mapeditwindow.h"
#include "ui_mapeditwindow.h"

#include <QPainter>
#include <QTimer>
#include <ctime>
#include <QRandomGenerator>
#include <QStack>
#include <QPair>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "imgbutton.h"
#include "mazedata.h"

MapEditWindow::MapEditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapEditWindow)
{
    ui->setupUi(this);

    //随机按钮
    ImgButton * randomBtn = new ImgButton(":/res/randomBtn_1.png", ":/res/randomBtn_2.png");
    randomBtn->setParent(this);
    randomBtn->move(600+34, this->height()*0.7);
    randomBtn->show();
    connect(randomBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            QVector<QVector<int> > mp = createMaze(mazeWidth);
//            for(QVector<int> v : mp)
//                qDebug() << v;
            for(int i = 0 ; i < mazeWidth;i++)
                for(int j = 0 ; j < mazeWidth; j++)
                    tiles[i][j]->changeStatus(mp[i][j]);
        });
    });

    //保存按钮
    ImgButton * saveBtn = new ImgButton(":/res/saveBtn_1.png", ":/res/saveBtn_2.png");
    saveBtn->setParent(this);
    saveBtn->move(600+34, this->height()*0.7+40);
    saveBtn->show();
    connect(saveBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            int data[31][31];
            int startNum=0;  //起点数目
            for(int i = 0 ; i < mazeWidth;i++)
                for(int j = 0 ; j < mazeWidth; j++)
                {
                    data[i][j] = tiles[i][j]->status;
                    if(data[i][j]==Tile::STARTING)
                        startNum++;
                }
            if(startNum == 1)
            {
                QString path = QFileDialog::getSaveFileName(this,"保存迷宫地图","./","(*.mazedata)");
                MazeData * maze = new MazeData(mazeWidth, data);
                maze->saveMaze(path);
                delete maze;
            }
            else
            {
                QMessageBox::warning(this,"起点数目错误","请设置 有且仅有 一个起点！");
            }
        });
    });

    //返回按钮
    ImgButton * backBtn = new ImgButton(":/res/backBtn_1.png", ":/res/backBtn_2.png");
    backBtn->setParent(this);
    backBtn->move(600+34, this->height()*0.7+80+15);
    backBtn->show();
    connect(backBtn, &ImgButton::clicked, [=](){
        QTimer::singleShot(100, [=](){
            emit(backBtnClicked());
        });
    });

    //+号按钮
    ImgButton * plusBtn = new ImgButton(":/res/plusBtn_1.png", ":/res/plusBtn_2.png");
    plusBtn->setParent(this);
    plusBtn->move(600+34+142-37-20, this->height()*0.7-45);
    plusBtn->show();
    connect(plusBtn, &ImgButton::clicked, [=](){
        if(mazeWidth != 29)
        {
            QTimer::singleShot(100, [=](){
                mazeWidth+=2;
                createTiles();
            });
        }
    });

    //-号按钮
    ImgButton * minusBtn = new ImgButton(":/res/minusBtn_1.png", ":/res/minusBtn_2.png");
    minusBtn->setParent(this);
    minusBtn->move(600+34+20, this->height()*0.7-45);
    minusBtn->show();
    connect(minusBtn, &ImgButton::clicked, [=](){
        if(mazeWidth != 9)
        {
            QTimer::singleShot(100, [=](){
                mazeWidth-=2;
                createTiles();
            });
        }
    });

    //初始化tiles数组
    for(int i = 0 ; i < 31;i++)
    {
        for(int j = 0 ; j < 31; j++)
        {
            tiles[i][j]=NULL;
        }
    }

    createTiles();

}

MapEditWindow::~MapEditWindow()
{
    delete ui;
}

void MapEditWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/background.jpg");
    painter.drawPixmap(0,0,pix);
}

void MapEditWindow::createTiles()
{
    for(int i = 0 ; i < 31;i++)
    {
        for(int j = 0 ; j < 31; j++)
        {
            if(tiles[i][j]!=NULL)
                delete tiles[i][j];
            tiles[i][j]=NULL;
        }
    }

    for(int i = 0 ; i < mazeWidth;i++)
    {
        for(int j = 0 ; j < mazeWidth; j++)
        {
            //创建金币对象
            Tile * tile = new Tile(Tile::WALL);
            tile->setParent(this);
            tile->move(i*(600/mazeWidth), j*(600/mazeWidth));
            tile->setSize(600/mazeWidth);
            tiles[i][j] = tile;
            tile->show();
            connect(tile, &Tile::clicked, [=](){
                tile->changeStatus();
            });
        }
    }
}

QVector<QVector<int> > MapEditWindow::createMaze(int width)
{
    QVector<QVector<int> > mp;
    qsrand(time(NULL));

    //初始化为全部是墙
    for(int i = 0 ; i < width;i++)
    {
        QVector<int> tempv;
        for(int j = 0 ; j < width;j++)
           tempv.push_back(0);
        mp.push_back(tempv);
    }

    //设置1,1为路并入栈
    mp[1][1] = 1;
    QStack<QPair<int, int>> st;
    st.push(QPair<int, int>(1,1));

    int x, y;
    bool isNotConnected = 1; //是否不连通
    while(isNotConnected && !st.empty()) //如果不连通继续生成
    {
        x = st.top().first;
        y = st.top().second;
        QVector<QPair<int, int>> nextcell;
        //一个潜在的邻居，如果他的上下左右的墙都是未打通的，那么他是符合要求的
        if(x-2>=1 && mp[x-2][y]==0 && mp[x-1][y]==0 && mp[x-3][y]==0 && mp[x-2][y+1]==0 && mp[x-2][y-1]==0)
            nextcell.append(QPair<int, int>(x-2,y));
        if(x+2<=width-2 && mp[x+2][y]==0 && mp[x+1][y]==0 && mp[x+3][y]==0 && mp[x+2][y+1]==0 && mp[x+2][y-1]==0)
            nextcell.append(QPair<int, int>(x+2,y));
        if(y-2>=1 && mp[x][y-2]==0 && mp[x][y-3]==0 && mp[x][y-1]==0 && mp[x-1][y-2]==0 && mp[x+1][y-2]==0)
            nextcell.append(QPair<int, int>(x,y-2));
        if(y+2<=width-2 && mp[x][y+2]==0 && mp[x][y+1]==0 && mp[x][y+3]==0 && mp[x-1][y+2]==0 and mp[x+1][y+2]==0)
            nextcell.append(QPair<int, int>(x,y+2));

        int cellNum = nextcell.size();
        if(cellNum == 0)
        {
            //该路径没有合适的邻居,先去除最后一个不满足要求的cell,把这个流程再来一次
            st.pop();
        }else
        {
            //随机选一个符合要求的邻居作为真正的邻居
            int num = qrand()%cellNum;
            int tempx = nextcell[num].first, tempy = nextcell[num].second;
            st.push(QPair<int, int>(tempx, tempy));

            //邻居和墙均需要被打通
            mp[tempx][tempy] = 1;
            if(tempx == x-2 && tempy == y)
                mp[x-1][y] = 1;
            else if(tempx == x+2 && tempy == y)
                mp[x+1][y] = 1;
            else if(tempx == x && tempy == y-2)
                mp[x][y-1] = 1;
            else if(tempx == x && tempy == y+2)
                mp[x][y+1] = 1;
        }

        //计算地图是否全部连通，考虑如果奇数行奇数列都为路则全部连通，否则没有连通
        int nx=0;
        isNotConnected=0;
        for(QVector<int> y : mp)
        {
            int ny;
            if(nx % 2 == 1) //奇数行再遍历
            {
                ny=0;
                for(int x : y)
                {
                    if(ny % 2 == 1 && x==0) //看奇数行奇数列是否为墙
                    {//如果为墙则不连通
                        isNotConnected=1;
                        break;
                    }
                    ny++;
                }
            }
            if(isNotConnected==1)
                break;
            nx++;
        }
    }
    return mp;
}
