
#include "ball.h"

Ball::Ball(qreal x,qreal y,qreal radius,QColor ballColor)
{
    dirX=1.0;
    dirY=-1.0;
    speed=1.0;

    shape=new QRectF(x,y,radius,radius);
    color = ballColor;
}

Ball::~Ball()
{
    delete shape;
}

void Ball::setShape(const QRectF &newShape)
{
    shape->setRect(newShape.left(),newShape.top()
                   ,newShape.width(),newShape.height());

}

void Ball::setColor( const QColor &newColor )
{
    color = newColor;
}

void Ball::setDirX( qreal newDirX )
{
    dirX = newDirX;
}

void Ball::setDirY( qreal newDirY )
{
    dirY = newDirY;
}

QRectF Ball::getShape() const
{
    return *shape;
}

QColor Ball::getColor() const
{
    return color;
}

qreal Ball::getDirX() const
{
    return dirX;
}

qreal Ball::getDirY() const
{
    return dirY;
}

void Ball::move()
{
    int dirx=1,diry=1;
    if(dirX<0) dirx=-dirx;
    if(dirY<0) diry=-diry;
    shape->setLeft( shape->left() + dirx );
    shape->setRight( shape->right() + dirx );
    shape->setTop( shape->top() + diry );
    shape->setBottom( shape->bottom() + diry );
}

void Ball::setSpeed( qreal newSpeed )
{
    speed = newSpeed;
    if((dirX>0&&speed>0)||(dirX<0&&speed<0))
        setDirX( speed );
    else setDirX(-speed);
    if((dirY>0&&speed>0)||(dirY<0&&speed<0))
        setDirY( speed );
    else setDirY(-speed);
}

qreal Ball::getSpeed() const
{
    return speed;
}

