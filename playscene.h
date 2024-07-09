#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "mycoin.h"

#include <QMainWindow>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    // explicit PlayScene(QWidget *parent = nullptr);

    PlayScene(int levelNum);
    // 重写 paintEvent 事件
    void paintEvent(QPaintEvent* );

    int levelIndex; // 內部成员属性 记录所选关卡
    int gameArray[4][4]; // 二维数组 维护每个关卡的二维数据
    MyCoin* coinBtn[4][4];
    bool isWin; // 是否胜利的标志

signals:
    // 返回信号
    void playSceneBack();
};

#endif // PLAYSCENE_H
