#include "mainscene.h"
#include "./ui_mainscene.h"
#include "mypushbutton.h"
#include "chooselevelscene.h"

#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QSoundEffect>
#include <QUrl>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 配置主场景

    // 设置固定大小
    setFixedSize(320,588);

    // 设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    // 设置标题
    setWindowTitle("翻金币");

    // 退出按钮实现
    connect(ui->actionQuit,&QAction::triggered,[=]()
            {
                this->close();
            });

    // 开始按钮
    MyPushButton* startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5, this->height()*0.7);

    // 实例化选择关卡场景
    this->chooseScene = new ChooseLevelScene;

    // 准备开始按钮音效
    QSoundEffect* startSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
    // 设置音量，范围为0.0到1.0
    startSound->setVolume(0.5f);

    // 点击开始，进入关卡场景
    connect(startBtn, &MyPushButton::clicked,[=](){
        // qDebug() << "点击了开始";\

        // 开启音效
        startSound->play();
        // 做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        // 延时进入关卡选择场景
        QTimer::singleShot(250,this,[=](){
            // 设置 chooseScene 的场景位置为主场景位置
            chooseScene->setGeometry(this->geometry());
            // 自身隐藏
            this->hide();
            // 进入选择场景
            chooseScene->show();
        });
    });

    // 监听返回关卡的返回按钮信号（ connect 就是将两个对象进行信号与槽的连接）
    connect(this->chooseScene, &ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry()); // 设置 mainScene 为 chooseScene 的位置
        chooseScene->hide(); // 将选择关卡场景隐藏
        this->show(); // 重新显示主场景
    });

}

void MainScene::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    //这是画 pix 的重载版本，将 pixmap 拉伸了
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    // 画背景上图标
    pix.load(":/res/Title.png");
    // 缩放一下 pixmap
    pix = pix.scaled(pix.width()*0.5, pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}

MainScene::~MainScene()
{
    delete ui;
}
