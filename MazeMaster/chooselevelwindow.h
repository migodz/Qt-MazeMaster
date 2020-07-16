#ifndef CHOOSELEVELWINDOW_H
#define CHOOSELEVELWINDOW_H

#include <QMainWindow>
#include "playwindow.h"

namespace Ui {
class ChooseLevelWindow;
}

class ChooseLevelWindow : public QMainWindow
{
    Q_OBJECT

public:
    //游玩窗口
    PlayWindow * playWindow = NULL;

    explicit ChooseLevelWindow(QWidget *parent = nullptr);
    ~ChooseLevelWindow();

    void paintEvent(QPaintEvent *);

private:
    Ui::ChooseLevelWindow *ui;

signals:
    void backBtnClicked();
};

#endif // CHOOSELEVELWINDOW_H
