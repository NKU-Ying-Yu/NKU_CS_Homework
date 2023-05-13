#ifndef BALL_H
#define BALL_H

#include<QRect>
#include<QColor>


class Ball
{
public:
    Ball(qreal,qreal,qreal,QColor=Qt::blue);
    ~Ball();

    void move();
    QRectF rect() const;

    void setShape(const QRectF &);   //设置形状
    void setColor(const QColor &);   //设置颜色
    void setDirX(qreal);
    void setDirY(qreal);             //设置坐标
    void setSpeed(qreal);            //设置速度

    QRectF getShape() const;         //对上述参数进行获取，以便进行更改
    QColor getColor() const;
    qreal getDirX() const;
    qreal getDirY() const;
    qreal getSpeed() const;

    bool tu=1;
    bool isFire=0;

private:
    QRectF *shape;
    QColor color;

    qreal dirX;
    qreal dirY;
    qreal speed;
};

#endif // BALL_H
