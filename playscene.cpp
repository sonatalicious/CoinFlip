#include "playscene.h"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"

#include <QDebug>
#include <QString>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSoundEffect>
#include <QUrl>

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}

PlayScene::PlayScene(int levelNum):levelIndex(levelNum)
{
    // QString str = QString("进入关卡 %1 ").arg(levelIndex);
    // qDebug()<< str;

    // 设置场景
    // 设置固定大小
    setFixedSize(320,588);
    // 设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    // 设置标题
    setWindowTitle("翻金币场景");

    // 创建菜单栏
    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);
    //创建开始菜单
    QMenu* startMenu = bar->addMenu("开始");
    // 创建按钮菜单项
    QAction* quitAction = startMenu->addAction("退出");

    // 准备翻金币音效
    QSoundEffect* flipSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    flipSound->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));
    // 设置音量，范围为0.0到1.0
    flipSound->setVolume(0.5f);
    // 准备胜利音效
    QSoundEffect* winSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    winSound->setSource(QUrl::fromLocalFile(":/res/LevelWinSound.wav"));
    // 设置音量，范围为0.0到1.0
    winSound->setVolume(0.5f);
    // 准备返回按钮音效
    QSoundEffect* backSound = new QSoundEffect(this);
    // 设置音频源文件的路径
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
    // 设置音量，范围为0.0到1.0
    backSound->setVolume(0.5f);

    // 点击退出，退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    // 返回按钮
    MyPushButton* backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(),this->height() - backBtn->height());
    // 点击返回
    connect(backBtn,&QPushButton::clicked,[=](){
        // qDebug() << "翻金币场景中，点击了返回按钮";
        backSound->play();
        // 延时发送信号，延时返回
        QTimer::singleShot(200,this,[=](){
            // 告诉选择场景，我返回了
            emit this->playSceneBack(); // 发送信号
        });
    });

    // 显示当前关卡数
    QLabel* label = new QLabel;
    label->setParent(this);
    // 设置字体
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString levStr = QString("Level: %1").arg(this->levelIndex);
    // 将字体设置到标签控件中
    label->setFont(font);
    label->setText(levStr);
    label->setGeometry(30 ,this->height() - 50, 120, 50);

    // 胜利图片加载到屏幕上方
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    bool pixLoad = tmpPix.load(":/res/LevelCompletedDialogBg.png");
    if(false == pixLoad)
        qDebug("胜利图片加载失败");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width() - tmpPix.width())*0.5, -tmpPix.height());

    // 初始化每个关卡的二维数组
    DataConfig config;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    // 显示金币背景图案
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            // 绘制背景图片
            QPixmap pix = QPixmap(":/res/BoardNode.png");
            QLabel* label = new QLabel;
            label->setGeometry(0, 0, pix.width(), pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57 + i*50, 200 + j*50);

            // 创建金币
            QString str;
            if(1 == this->gameArray[i][j])
                str = ":/res/Coin0001.png";
            else
                str = ":/res/Coin0008.png";
            MyCoin* coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50, 204 + j*50);

            // 给金币赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j]; // 1正面 0反面

            // 将金币放入到金币的二维数组里，以便于后期的维护
            this->coinBtn[i][j] = coin;

            connect(coin,&QPushButton::clicked,[=](){
                flipSound->play();
                // 点击过后，禁用所有按钮，防止快速点击其他按钮出现 bug，与下面的启用代码成对出现
                for(int i = 0; i < 4; ++i)
                {
                    for(int j = 0; j < 4; ++j)
                    {
                        coinBtn[i][j]->isWin = true; //其实应该新建一个相关属性的，不然容易混淆
                    }
                }

                coin->changeFlag();
                // 更新二维数组数据
                this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;

                // 延时翻转周围硬币
                QTimer::singleShot(300, this,[=](){
                    if(coin->posX+1 <=3)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY+1<=3)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY-1>=0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }

                    // 点击过后，启用所有按钮，与上面的禁用成对出现
                    for(int i = 0; i < 4; ++i)
                    {
                        for(int j = 0; j < 4; ++j)
                        {
                            coinBtn[i][j]->isWin = false;
                        }
                    }

                    // 判断是否胜利,这部分不能提到定时器 QTimer::singleShot 外面
                    // 因为定时器是异步的，不会阻塞在这里，后面的代码先执行，300ms后再回来执行定时器内代码
                    // 那样的话就永远 success 不了
                    this->isWin = true; // 注意这里是 playscene 的 iswin
                    for(int i = 0; i < 4; ++i)
                    {
                        for(int j = 0; j < 4; ++j)
                        {
                            if(false == coinBtn[i][j]->flag) // 只要有一个是反面，就算失败
                            {
                                this->isWin = false;
                                i = 5; // 把外面的循环也退出了
                                break;
                            }
                        }
                    }
                    // 关卡挑战成功
                    if(true == this->isWin)
                    {
                        winSound->play();
                        // 将所有按钮胜利标志改为 true
                        // 进而屏蔽鼠标对所有金币的 点击 动作
                        for(int i = 0; i < 4; ++i)
                        {
                            for(int j = 0; j < 4; ++j)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        }

                        // 让胜利图片移动下来
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel, "geometry");
                        // 设置时间间隔
                        animation->setDuration(1000);
                        // 设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        // 设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y() + 114,winLabel->width(),winLabel->height()));
                        // 设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        // 执行动画
                        animation->start();
                    }

                });
            });

        }
    }


}

void PlayScene::paintEvent(QPaintEvent* e)
{
    // 画背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    // 加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}





