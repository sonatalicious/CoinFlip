#include "mycoin.h"

#include <QPixmap>
#include <QDebug>

// MyCoin::MyCoin(QWidget *parent)
//     : QPushButton{parent}
// {}


MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool ret = pix.load(btnImg);
    if(!ret){
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug() << str;
        return;
    }

    this->setFixedSize(pix.width(), pix.height());
    this->setStyleSheet("QPushButton{border:0px};");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));

    // 初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    // 监听正面翻反面信号，并翻转金币
    connect(timer1, &QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px};");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        // 判断如果翻完了，将 min 重置为 1
        if(this->min > this->max)
        {
            this->min = 1;
            this->isAnimation = false; // 结束动画
            this->timer1->stop();
        }
    });

    // 监听反面翻正面信号，并翻转金币
    connect(timer2, &QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px};");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        // 判断如果翻完了，将 min 重置为 1
        if(this->min > this->max)
        {
            this->max = 8;
            this->isAnimation = false; // 结束动画
            this->timer2->stop();
        }
    });
}

// 改变正反面标志的方法
void MyCoin::changeFlag()
{
// 如果是正面，翻到反面
    if(this->flag)
    {
        // 开始正面翻反面的定时器
        timer1->start(30);
        this->isAnimation = true; // 开始动画
        this->flag = false;

    }
    else // 反面翻正面
    {
        timer2->start(30);
        this->isAnimation = true; // 开始动画
        this->flag = true;
    }
}

// 正 在动画中时 或 关卡挑战成功时 禁用点击动作
void MyCoin::mousePressEvent(QMouseEvent* e)
{
    if(true == this->isAnimation || true == this->isWin)
        return;
    else
        QPushButton::mousePressEvent(e);
}
