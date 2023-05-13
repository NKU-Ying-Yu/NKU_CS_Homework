#include "game.h"
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

Game::Game(int gameNum,QWidget *parent) : QWidget(parent)
{
    isWin = false;
    isFailure = false;
    cnt=0;
    cocnt=0;
    guan=gameNum;

    frameWidth = 1200;
    frameHeight = 900;

    score = 0;
    rows = 15;
    columns = 12;


    srand( (int)time(NULL));
    color[0]=randomColor();       //随机设置颜色
    for(int i=1;i<5;i++)
    {
        QColor newcolor=randomColor();
        while(newcolor==color[i-1])
        {
            newcolor=randomColor();
        }
        color[i]=newcolor;      //确保相连的不重复
    }



    createBricks();
    createPaddle();
    createBall();               //生成三个组件
    resize(frameWidth,frameHeight);

    timer = new QTimer( this );
    timer->setInterval( 8 );
    connect( timer, SIGNAL( timeout() ), this, SLOT( moveBall() ) );


    this->setMouseTracking(true);
}

Game::~Game()
{
    delete ball;
    delete paddle;

    for(int i=0;i<15;i++)
            bricks[i].clear();

    while(!matters.isEmpty())
        delete matters.takeFirst();
}//清楚方块和事件

void Game::changeColor()
{
    srand( (int) time( NULL ) );

    cocnt++;
    for(int i=0;i<15;i++)
        for(int j=0;j<12;j++)
            bricks[i][j]->setColor(color[(cocnt+bricks[i][j]->yingdu)%5]);

    ball->setColor( randomColor() );
    paddle->setColor( randomColor() );
}//更换颜色

QColor Game::randomColor()
{
    return QColor( randomInt( 255 ), randomInt( 255 ), randomInt( 255 ) );
}//随机颜色

int Game::randomInt( int high )
{
    double d = (double)rand() / ( (double)RAND_MAX + 1 );
    int k = (int) (d * ( high  + 1 ));

    return k;
}

void Game::createBricks()
{
    qreal gap = 5.0;
    qreal brickWidth = frameWidth / ( columns + 1 ) - gap;
    qreal brickHeight = frameHeight  / ( rows + 1 ) - gap;

    for( int r = 0; r < rows; ++r )
    for( int c = 0; c < columns; ++c )
    {
        Brick *brick = new Brick(  brickWidth / 2 + c * ( brickWidth + gap ),
                       brickHeight / 2 + r * ( brickHeight + gap ),
                       brickWidth, brickHeight ,color[cocnt%5] );
        bricks[r].push_back(brick);
    }

    switch (guan)//对不同的关卡设置不同的砖块布置
    {
    case 1:
        for(int i=1;i<=6;i++) bricks[i][1]->vis=1;
        for(int j=2,i=0;j<=2;j++) {
             bricks[(i+=2)][j]->vis=1;
             bricks[(i+=1)][j]->vis=1;
        }
        bricks[4][3]->vis=1;
        for(int i=1;i<=6;i++) bricks[i][4]->vis=1;
        for(int i=1;i<=6;i++) bricks[i][8]->vis=1;
        for(int j=8,i=3;j<=11;j++) bricks[(i++)][j]->vis=1;
        for(int j=8,i=4;j<=11;j++) bricks[(i--)][j]->vis=1;
        cnt=27;
        MAXspeed=2;
        break;
    case 2:
        for(int i=2;i<=10;i++)
            for(int j=2;j<=9;j++)
            {
                Brick *temp=bricks[i][j];
                temp->vis=1;
                if(i>3&&i<=8&&j>=3&&j<=8)
                {
                    bricks[i][j]->setColor(color[(cocnt+2)%5]);
                    temp->yingdu=2;
                }
                else bricks[i][j]->setColor(color[(cocnt+1)%5]);
            }
        cnt=72;
        MAXspeed=3;
        break;
    case 3:
        color[(cocnt+2)%5]=Qt::red;
        color[(cocnt+1)%5]=Qt::yellow;
        color[(cocnt+3)%5]=QColor(255,192,213);
        cnt=73;
        for(int i=2;i<=13;i++)
        {
            int j=3;
            if(i==2) j=3;
            else if(i==3) j=2;
            else if(i>=4&&i<=6) j=1;
            else if(i==7||i==8) j=2;
            else if(i==9) j=3;
            else if(i==10||i==11) j=4;
            else if(i==12||i==13) j=5;

            for(;j<=10;j++)
            {
                if((i==2||i==3)&&j==5) j=7;
                else if((i==2||i==8||i==9)&&j==9) break;
                else if((i==3||i==7)&&j==10) break;
                else if(i==10&&j==8)break;
                else if(i==11&&j==8)break;
                else if((i==12||i==13)&&j==7) break;

                if( ( (i==5||i==7)&&(j==4||j==7) ) || ( (i==8||i==9)&&(j==5||j==6) ) || (i==6&&j>=3&&j<=8) )
                {
                    bricks[i][j]->setColor(Qt::yellow);
                    bricks[i][j]->yingdu=1;
                }
                else if(i==7&&(j==5||j==6))
                {
                    bricks[i][j]->setColor(QColor(255,192,213));
                    bricks[i][j]->yingdu=3;
                }
                else
                {
                    bricks[i][j]->setColor(Qt::red);
                    bricks[i][j]->yingdu=2;
                }
                bricks[i][j]->vis=1;
            }
        }
        MAXspeed=3;
        break;


    }
}

void Game::createPaddle()
{
    qreal paddleWidth = frameWidth / 10;
    qreal paddleHeight = paddleWidth / 5;

    paddle = new Paddle( frameWidth / 2 - paddleWidth / 2 ,
             frameHeight - paddleHeight * 2,
             paddleWidth, paddleHeight );
}

void Game::createBall()
{
    qreal ballSide = paddle->getShape().height();

    ball = new Ball( frameWidth / 2 - ballSide / 2, paddle->getShape().top() - ballSide, ballSide );
}

void Game::resizeEvent( QResizeEvent * )
{
    qreal scaleWidth = rect().width() / frameWidth;
    qreal scaleHeight = rect().height() / frameHeight;

    for(int i=0;i<rows;i++)
    for(int j=0;j<columns;j++)
    {
        QRectF shape = bricks[i][j]->getShape();
        bricks[i][j]->setShape( adjustShape( shape, scaleWidth, scaleHeight ) );
    }

    QRectF ballShape = ball->getShape();
    ball->setShape( adjustShape( ballShape, scaleWidth, scaleHeight ) );
    ball->setSpeed( ball->getSpeed() * scaleWidth );

    QRectF paddleShape = paddle->getShape();
    paddle->setShape( adjustShape( paddleShape, scaleWidth, scaleHeight ) );
    paddle->setStep( paddle->getStep() * scaleWidth );

    frameWidth = rect().width();
    frameHeight = rect().height();
}//调整大小

QRectF Game::adjustShape( QRectF shape, qreal scaleWidth, qreal scaleHeight )
{
    QRectF newShape( shape.left() * scaleWidth, shape.top() * scaleHeight,
            shape.width() * scaleWidth, shape.height() * scaleHeight );
    return newShape;
}

void Game::paintEvent( QPaintEvent * )
{
    QPainter painter;

    painter.begin(this);

    if(guan==1) painter.drawPixmap(rect(),QPixmap("../image/5.png"));
    else if(guan==2) painter.drawPixmap(rect(),QPixmap("../image/10.png"));
    else if(guan==3) painter.drawPixmap(rect(),QPixmap("../image/6.png"));//画背景图


    if( isWin  || isFailure ) //胜负事件已产生
    {
        QFont font( "宋体", 30, QFont::Bold );

        QFontMetrics fm( font );

        painter.setFont( font );
        painter.translate( QPoint( width() / 2, height() / 2 ) );

        if( isWin )
        {
            int textWidth = fm.horizontalAdvance("You are winner!" );
            painter.setPen( Qt::blue );
            painter.drawText( -textWidth / 2, 0, tr( "    你赢了!" ) );

        }
        else
        {
            int textWidth = fm.horizontalAdvance("You are loser!" );
            painter.setPen( Qt::red );
            painter.drawText( -textWidth / 2, 0, tr( "    你输了!" ) );

        }

        timer->stop();
        emit finished();
        return;
    }

    painter.setPen( Qt::NoPen );
    painter.setBrush( ball->getColor() );
    painter.drawEllipse( ball->getShape() );
    painter.fillRect( paddle->getShape(), paddle->getColor() );


    for(int i=0;i<rows;i++)
    for(int j=0;j<columns;j++)
    {
        if(bricks[i][j]->vis) painter.fillRect( bricks[i][j]->getShape(), bricks[i][j]->getColor() );

//        painter.setPen( Qt::red );
//        painter.drawText(bricks[i][j]->getShape(),0,tr("( %1,%2 )").arg(i).arg(j));//用于标记坐标的语句，可以用来规划关卡
    }

    Matter *temp;
    for(int i=0;i<matters.size();i++)
    {
        temp=matters.at(i);
        painter.drawPixmap(temp->getShape(),temp->getPixmap());
    }
}

void Game::movePaddleLeft()
{
    if( paddle->getLeft() > rect().left() )
    {
    paddle->moveLeft();
    update();
    }
}

void Game::movePaddleRight()
{
    if( paddle->getRight() < rect().right() )
    {
    paddle->moveRight();
    update();
    }
}

void Game::startGame()
{
    timer->start();
}

void Game::stopGame()
{
    timer->stop();
}

void Game::moveBall()
{
    static int sec = 0;
    QRectF ballShape= ball->getShape();
    QRectF paddleShape= paddle->getShape();

    qreal ballLeft = ballShape.left();
    qreal ballRight = ballShape.right();
    qreal ballBottom= ballShape.bottom();
    qreal ballTop=ballShape.top();
    qreal ballradiu=ballShape.height()/2;
    qreal ballCenterX= ( ballLeft + ballRight ) / 2;
    qreal ballCenterY= ( ballTop + ballBottom ) / 2;
    qreal paddlelen4=(paddleShape.right()-paddleShape.left())/4;
    qreal paddleCenterX= ( paddleShape.left() + paddleShape.right() ) / 2;
    qreal paddleHeight= paddleShape.bottom()-paddleShape.top();
for(int i=0;i<(ball->getSpeed()>0?1:-1)*ball->getSpeed();i++)
{
    ball->move();

    ballShape = ball->getShape();
    paddleShape = paddle->getShape();

    ballLeft = ballShape.left();
    ballRight = ballShape.right();
    ballBottom = ballShape.bottom();
    ballTop = ballShape.top();
    ballradiu=ballShape.height()/2;
    ballCenterX = ( ballLeft + ballRight ) / 2;
    ballCenterY = ( ballTop + ballBottom ) / 2;
    paddlelen4=(paddleShape.right()-paddleShape.left())/4;
    paddleCenterX = ( paddleShape.left() + paddleShape.right() ) / 2;
    paddleHeight = paddleShape.bottom()-paddleShape.top();

    if( ballLeft <= rect().left() )
    {
        ball->setDirX( 1 );
        if(ball->getSpeed()<MAXspeed) ball->setSpeed(ball->getSpeed()+0.01);
    }
    if( ballRight >= rect().right()  )
    {
        ball->setDirX( -1 );
        if(ball->getSpeed()<MAXspeed) ball->setSpeed(ball->getSpeed()+0.01);
    }


    if( ballBottom > paddleShape.top() )
    {
         if( paddleShape.intersects( ballShape ) )
         {
             if( (ballCenterX < paddleCenterX-paddlelen4 && ball->getDirX()>0)||(ballCenterX > paddleCenterX+paddlelen4 && ball->getDirX()<0 ))
                ball->setDirX( -ball->getDirX() );
             ball->setDirY( -1 );
             if(ball->getSpeed()<MAXspeed) ball->setSpeed(ball->getSpeed()+0.01);
         }
         if(ballTop>paddleShape.bottom())
         {
             isFailure = true;
             update();

             int judge;
             QMessageBox a;
             judge = a.question(this,"可惜","不好意思，你输了，是否重开本局？");
             if(judge==QMessageBox::Yes)
             {
                 emit opengame(guan);
             }
             else
             {
                 a.about(this,"提示",QString("你现在的等级是：%1，可以通过Game项打开游戏").arg(guan));
             }

             return;
         }
    }

    if( ballTop <= rect().top() )
    {
        ball->setDirY( 1 );
        if(ball->getSpeed()<MAXspeed) ball->setSpeed(ball->getSpeed()+0.01);
    }
    else
    {
        bool tu=0;
        for(int i=0;i<rows;i++)
        {
        for( int j=0;j<columns;j++ )
        {

            Brick *brick = bricks[i][j] ;
            if(brick->vis)
            {
            QRectF brickShape = brick->getShape();
            if( brickShape.intersects( ball->getShape() ) )
            {
                tu=1;
                if(!ball->isFire)
                {
                if( ( (ballCenterX<brickShape.left()&&ball->getDirX()>0) || (ballCenterX>brickShape.right()&&ball->getDirX()<0) ) && ( (ballCenterY<brickShape.top()&&ball->getDirY()>0) || (ballCenterY>brickShape.bottom()&&ball->getDirY()<0) ) )
                {
                    ball->setDirX(-ball->getDirX());
                    ball->setDirY(-ball->getDirY());
                }
                else if( ( ( ballCenterX<brickShape.left()&&ball->getDirX()>0 ) || (ballCenterX>brickShape.right()&&ball->getDirX()<0 ) )  && (ballCenterY>brickShape.top()-ballradiu&&ballCenterY<brickShape.bottom()+ballradiu ) )
                    ball->setDirX( -ball->getDirX() );
                else ball->setDirY( -ball->getDirY() );
                }
                else
                {
                    bricks[i][j]->yingdu=1;
                }

                Brick *temp = bricks[i][j];
                temp->yingdu--;

                if(temp->yingdu<=0)
                {
                    temp->vis=0;
                    cnt--;
                    if(ball->getSpeed()<MAXspeed) ball->setSpeed(ball->getSpeed()+0.01);

                    int judge = rand() % 30;
                    if(judge>=1&&judge<=7)
                    {
                        Matter *m=new Matter(temp->getShape().left(),temp->getShape().top(),paddleHeight*2,paddleHeight*2,Position::fireball);
                        switch (judge)
                        {
                        case 1:
                            if(ball->getColor()!=Qt::red) oldcolor=ball->getColor();
                            sec=0;
                            break;
                        case 2:
                            m->setPosition(Position::longer);
                            break;
                        case 3:
                            m->setPosition(Position::shorter);
                            break;
                        case 4:
                            m->setPosition(Position::quicker);
                            break;
                        case 5:
                            m->setPosition(Position::lower);
                            break;
                        case 6:
                            m->setPosition(Position::biger);
                            break;
                        case 7:
                            m->setPosition(Position::smaller);
                            break;
                        }
                        matters.append(m);

                   }

            }
            else temp->setColor(color[(cocnt+temp->yingdu)%5]);//设置根据硬度改变颜色的砖块特性
            score += 100;
            emit scoreChanged(guan, score );
            break;
            }
            }
        }
        if(tu) break;
        }

        //胜利条件
        if( cnt == 0 )
        {
            isWin = true;

            int judge;
            QMessageBox a;
            if(guan<3)
            {
                judge = a.question(this,"恭喜","你赢了，是否开始下一局？");
                if(judge==QMessageBox::Yes)
                {
                    emit opengame(guan+1);
                }
                else
                {
                    emit changeLevel(guan+1);
                    a.about(this,"提示",QString("你现在的等级为:%1,可以通过Game项来打开游戏").arg(guan+1));
                }
            }
            else
            {
                a.about(this,"恭喜通关","感谢玩耍，希望能给你带来一些乐趣！");
            }
            return;
        }
    }

    update();
}
for(int i=0;i<matters.size();i++)
{
    Matter *m =matters.at(i);
    m->move();
    int top=m->getShape().top();
    if(top>=paddleShape.bottom())
    {
        Matter *temp = matters.takeAt(i);
        delete temp;
        continue;
    }


    if(m->getShape().bottom()>=paddleShape.top()&&paddleShape.intersects(m->getShape()))
    {
        switch (m->getPosition())
        {
        case Position::fireball:
            if(!ball->isFire&&ball->getSpeed()<=3.0) ball->setSpeed(ball->getSpeed()+0.5);
            ball->isFire=1;
            break;
        case Position::biger:
            if(ballradiu<paddleHeight*4)
                ball->setShape(QRectF(ballLeft,ballTop,ballradiu*2+paddleHeight/2,ballradiu*2+paddleHeight/2));
            break;
        case Position::smaller:
            if(ballradiu>paddleHeight/4)
                ball->setShape(QRectF(ballLeft,ballTop,ballradiu*2-paddleHeight/2,ballradiu*2-paddleHeight/2));
            break;
        case Position::quicker:
            if(ball->getSpeed()<3.5)
                ball->setSpeed(ball->getSpeed()+0.3);
            break;
        case Position::lower:
            if(ball->getSpeed()>1.0)
                ball->setSpeed(ball->getSpeed()-1);
            break;
        case Position::longer:
        {
            int len=paddleShape.right()-paddleShape.left();
            if(len<500)
            {
                paddle->setLeft(paddle->getLeft()-30);
                paddle->setRight(paddle->getRight()+30);
            }
            break;
        }
        case Position::shorter:
        {
            int len=paddleShape.right()-paddleShape.left();
            if(len>60)
            {
                paddle->setLeft(paddle->getLeft()+30);
                paddle->setRight(paddle->getRight()-30);
            }
            break;
        }
        }//处理接到技能时的效果
        score+=500;
        emit scoreChanged(guan,score);
        Matter *temp = matters.takeAt(i);
        delete temp;

    }
}

    if(ball->isFire)
    {
        if(!sec)
        {
            ball->setColor(Qt::red);
        }
        if(sec<=1200)
        {
            sec++;
        }
        else
        {
            sec=0;
            ball->isFire=0;
            ball->setColor(oldcolor);
            if(ball->getSpeed()>0.5) ball->setSpeed(ball->getSpeed()-0.5);
        }
    }
}

void Game::setBallSpeed( const qreal &speed )
{
    ball->setSpeed( speed );
}

void Game::setPaddleStep( const qreal &step )
{
    paddle->setStep( step );
}

int Game::getlen2()
{
    return (paddle->getRight()-paddle->getLeft())/2;
}

void Game::setposition(int x, int len2)
{
    paddle->setLeft((qreal)x-len2);
    paddle->setRight((qreal)x+len2);
    if(ball->tu)
    {
        ball->setShape( QRectF(x , paddle->getShape().top()-paddle->getShape().height() , paddle->getShape().height() , paddle->getShape().height() ));
    }
    update();
}

void Game::setBallTu(bool a)
{
    ball->tu=a;
}

void Game::waigua()
{
    ball->isFire=1;
    ball->setColor(Qt::red);
    ball->setSpeed(2.0);
    paddle->setLeft(paddle->getLeft()-100);
    paddle->setRight(paddle->getRight()+100);
    update();
}
