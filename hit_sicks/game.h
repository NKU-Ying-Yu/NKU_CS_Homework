#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "matter.h"
#include <QWidget>


class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(int,QWidget *parent = nullptr);
    ~Game();

    void setBallSpeed(const qreal &);
    void setPaddleStep(const qreal&);
    int getlen2();
    void setBallTu(bool);
    void setposition(int x,int len2);
    int guan=1;
    int cnt=0;
    QColor color[5];
    int cocnt=0;
    QColor oldcolor=Qt::blue;
    qreal MAXspeed=3;

signals:
    void finished();
    void scoreChanged(int,const int&);
    void opengame(int);
    void changeLevel(int);

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent( QPaintEvent * );


public slots:
    void moveBall();           //移动小球
    void changeColor();        //改变颜色
    void movePaddleLeft();     //移动平台
    void movePaddleRight();
    void startGame();          //开始和结束游戏
    void stopGame();
    void waigua();             //后门

private:
    void createBricks();
    void createPaddle();
    void createBall();

    QRectF adjustShape(QRectF,qreal,qreal);
    QColor randomColor();
    int randomInt(int);


    Ball *ball;
    Paddle *paddle;
    QVector< Brick * >bricks[15];
    QList< Matter * >matters;
    QTimer *timer;

    int rows;
    int columns;
    int score;
    bool isWin;
    bool isFailure;

    double frameWidth;
    double frameHeight;
};

#endif // GAME_H
