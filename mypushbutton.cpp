#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>
// MyPushButton::MyPushButton(QWidget *parent)
//     : QPushButton{parent}
// {}
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }

    // 设置图片固定大小
    this->setFixedSize(pix.width(),pix.height());
    // 设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px}");
    // 设置图标
    this->setIcon(pix);
    // 设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));

}

void MyPushButton::zoom1()
{
    // 创建动态对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    // 设置动画时间间隔
    animation->setDuration(200);

    // 起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    // 结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    // 执行动画
    animation->start();

}

void MyPushButton::zoom2()
{
    // 创建动态对象
    QPropertyAnimation* animation = new QPropertyAnimation(this,"geometry");
    // 设置动画时间间隔
    animation->setDuration(200);

    // 起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    // 结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    // 设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    // 执行动画
    animation->start();

}

void  MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "") // 传入的按下图片不为空，说明要有按下状态，切换成按下图片
    {
        // 这里是重新渲染一下，也可以把下面的一段代码封装成函数
        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        // 设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());
        // 设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px}");
        // 设置图标
        this->setIcon(pix);
        // 设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    // 实质上这里重写父类方法后是做了一层拦截
    // 让父类执行其他内容，因为这里是重写父类的方法，也许父类在函数内有其他动作，所以需要交还给父类执行
    return QPushButton::mousePressEvent(e);
}

void  MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "") // 传入的按下图片不为空，说明要有按下状态，切换成初始图片
    {
        // 这里是重新渲染一下，也可以把下面的一段代码封装成函数
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        // 设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());
        // 设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px}");
        // 设置图标
        this->setIcon(pix);
        // 设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }

    // 实质上这里重写父类方法后是做了一层拦截
    // 让父类执行其他内容，因为这里是重写父类的方法，也许父类在函数内有其他动作，所以需要交还给父类执行
    return QPushButton::mouseReleaseEvent(e);
}


