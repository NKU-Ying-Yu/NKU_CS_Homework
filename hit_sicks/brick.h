#ifndef BRICK_H
#define BRICK_H

#include <QRect>
#include <QColor>

class Brick
{
public:
    Brick(qreal, qreal, qreal, qreal, QColor = Qt::red);
    ~Brick();

    void setShape(const QRectF &);  //设置形状
    void setColor(const QColor &);  //设置颜色
    bool vis=0;
    int yingdu=1;

    QRectF getShape() const;
    QColor getColor() const;

private:
    QRectF *shape;
    QColor color;
};

#endif // BRICK_H
