#include "chooselevelscene.h"
#include "mypushbutton.h"

#include <QMenuBar>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QSoundEffect>
#include <QUrl>

ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    // 设置场景
    setFixedSize(320,588);
    // 设置图标
    setWindowIcon(QPixmap(":/res/Coin0001.png"));
    // 设置标题
    setWindowTitle("选择关卡场景");

    // 创建菜单栏
    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu* startMenu = bar->addMenu("开始");
    // 创建按钮菜单项
    QAction* quitAction = startMenu->addAction("退出");
    // 点击退出，退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    // 准备选择按钮音效
    QSoundEffect* startSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));
    // 设置音量，范围为0.0到1.0
    startSound->setVolume(0.5f);


    // 准备返回按钮音效
    QSoundEffect* backSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
    // 设置音量，范围为0.0到1.0
    backSound->setVolume(0.5f);

    // 返回按钮
    MyPushButton* backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(),this->height() - backBtn->height());
    // 点击返回
    connect(backBtn,&QPushButton::clicked,[=](){
        // 返回音效
        backSound->play();

        // 延时返回
        QTimer::singleShot(200,this,[=](){
            // 告诉主场景，我返回了，主场景要监听 ChooseLevelScene 的返回按钮，所以要写一个自定义信号
            emit this->chooseSceneBack(); // 发送信号
        });
    });

    // 创建选择关卡按钮，并连接相应的信号槽
    for(int i = 0; i < 20; ++i)
    {
        MyPushButton* menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i%4*70, 130 + i/4*70);

        // 监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){
            // QString str = QString("您选择的是第 %1 关").arg(i+1);
            // qDebug() << str;

            startSound->play(); // 播放声音

            // 进入到游戏场景
            if(play == NULL)
            {
                this->hide(); // 将选关场景隐藏掉
                play = new PlayScene(i+1);
                play->setGeometry(this->geometry()); // 设置 play 为 chooseScene 的位置
                play->show(); // 显示选关场景
            }

            // 监听信号，从 playscene 中返回到本界面
            // QT 中 connect 的对象在调用 connect 之前必须要完成初始化。
            // 这是因为 connect 函数内部需要能够访问到这些对象的元对象系统（meta-object system）
            // 否则 Qt 不能正确地解析信号和槽的签名，并建立它们之间的连接
            // 所以这部分 connect代码不能像 mainsecne 里 72 行一样提到上一层 connect 外面
            // connect 提到外面时的 play 其实是 NULL
            connect(this->play,&PlayScene::playSceneBack,[=](){
                // 直接删掉 play 场景，每次重新建立 play 场景
                this->setGeometry(play->geometry()); // 设置 chooseScene 为 play 的位置
                delete play;
                play = NULL;
                this->show();
            });
        });

        QLabel* label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(1 + i));
        label->move(25 + i%4*70, 130 + i/4*70);
        // 设置 label 上的文字对齐方式为 水平居中和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        // 设置鼠标进行穿透，鼠标穿透属性，不然鼠标点击事件会被 label 接收，传不到下面的 menuBtn
        // Qt::WA_TranslucentBackground 为 51 号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

}

void ChooseLevelScene::paintEvent(QPaintEvent*)
{
    // 画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    // 加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);

}

