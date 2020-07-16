#include "mazedata.h"

#include <QFile>
#include <QTextStream>

MazeData::MazeData()
{
    this->mazeSize = 0;
    for(int i = 0; i < 31; i++)
        for(int j = 0; j < 31; j++)
            this->mazeData[i][j] = 0;
}

MazeData::MazeData(const int &mSize, const int mData[31][31])
{
    this->mazeSize = mSize;
    for(int i = 0; i < 31; i++)
        for(int j = 0; j < 31; j++)
            this->mazeData[i][j] = 0;
    for(int i = 0; i < mSize; i++)
        for(int j = 0; j < mSize; j++)
            this->mazeData[i][j] = mData[i][j];
}

void MazeData::loadMaze(const QString & path)
{
    QFile data(path);
    QString text;
    if (data.open(QFile::ReadOnly))
    {
        QTextStream in(&data);
        this->mazeSize = in.readLine().toInt();
        for(int i = 0; i < mazeSize; i++)
        {
            for(int j = 0; j < mazeSize; j++)
            {
                mazeData[i][j] = in.read(1).toInt();
            }
            in.read(1);
        }
    }
}

void MazeData::saveMaze(const QString & path)
{
    QFile data(path);
    if (data.open(QFile::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&data);
        out << mazeSize << "\n";
        for(int i = 0; i < mazeSize; i++)
        {
            for(int j = 0; j < mazeSize; j++)
            {
                out << mazeData[i][j];
            }
            out << "\n";
        }
    }

}


