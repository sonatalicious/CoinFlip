#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include "playscene.h"

#include <QMainWindow>

class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);
    // 重写绘图事件
    void paintEvent(QPaintEvent*);
    // 游戏场景指针
    PlayScene* play = NULL;

signals:
    // 自定义信号写在这里，告诉主场景该返回了
    // 自定义的信号不需要实现
    void chooseSceneBack();

};

#endif // CHOOSELEVELSCENE_H
