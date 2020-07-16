#ifndef ABSTRACTMAZE_H
#define ABSTRACTMAZE_H

#include <QString>

class MazeData
{
public:
    int mazeData [31][31];//状态：0为墙，1为普通通路，2为出发点，3为目的地
    int mazeSize;//迷宫的大小

    MazeData();
    MazeData(const int & mSize, const int mData[31][31]);
    void loadMaze(const QString & path);//根据文件路径读取迷宫
    void saveMaze(const QString & path);//保存迷宫数据文件到指定位置
};

#endif // ABSTRACTMAZE_H
