#include "matter.h"

Matter::Matter(qreal x,qreal y,qreal width,qreal height,Position b)
{
    shape = new QRectF(x,y,width,height);
    position = b;

    setPixmap();
}

Matter::~Matter()
{
    delete shape;
}

void Matter::setShape(const QRectF &a)
{
    shape ->setRect(a.left(),a.top(),a.width(),a.height());
}

void Matter::setPixmap()
{
    if(position==Position::fireball)
        pixmap.load("../image/fire.jpg");
    else if(position==Position::biger)
        pixmap.load("../image/biger.png");
    else if(position==Position::smaller)
        pixmap.load("../image/smaller.png");
    else if(position==Position::quicker)
        pixmap.load("../image/quicker.jpg");
    else if(position==Position::lower)
        pixmap.load("../image/lower.jpg");
    else if(position==Position::longer)
        pixmap.load("../image/longer.png");
    else if(position==Position::shorter)
        pixmap.load("../image/shorter.png");
}

void Matter::setPosition(const Position &b)
{
    position=b;
    setPixmap();
}

QRect Matter::getShape() const
{
    return QRect(shape->left(),shape->top(),shape->width(),shape->height());
}

QPixmap Matter::getPixmap() const
{
    return pixmap;
}

Position Matter::getPosition() const
{
    return position;
}

void Matter::move()
{
    shape->setTop(shape->top()+1);
    shape->setBottom(shape->bottom()+1);
}













