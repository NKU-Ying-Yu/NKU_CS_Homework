#ifndef PADDLE_H
#define PADDLE_H

#include <QRect>
#include <QColor>

class Paddle
{
public:
    Paddle(qreal, qreal, qreal, qreal, QColor = Qt::green);
    ~Paddle();

    QRectF getShape() const;
    QColor getColor() const;
    qreal getLeft() const;
    qreal getRight() const;
    qreal getStep() const;

    void setShape(const QRectF &);
    void setColor(const QColor &);
    void setLeft(const qreal &);
    void setRight(const qreal &);
    void setStep(const qreal &);    //平台的参数设置和获取

    void moveLeft();
    void moveRight();

private:
    QRectF *shape;
    QColor color;
    qreal step;
};

#endif // PADDLE_H
