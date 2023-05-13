#ifndef MATTER_H
#define MATTER_H

#include<QRect>
#include<QPixmap>

enum Position{     //事件类型
    fireball,longer,shorter,quicker,lower,biger,smaller
};

class Matter
{
public:
    Matter(qreal x,qreal y,qreal width,qreal height,Position b=Position::fireball);
    ~Matter();
    void setShape(const QRectF&);
    void setPixmap();
    void setPosition(const Position&);

    QRect getShape() const;
    QPixmap getPixmap() const;
    Position getPosition() const;

    void move();

private:
    QRectF *shape;
    QPixmap pixmap;
    Position position;


};

#endif // MATTER_H
