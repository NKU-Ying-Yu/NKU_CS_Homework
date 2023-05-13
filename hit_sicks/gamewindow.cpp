#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLabel>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    isRunning = false;
        game = new Game( 1,this );
        this->setMouseTracking(true);

        setCentralWidget( game );

        createMenu();
        createToolBar();

        gameTwo->setEnabled(false);
        gameThree->setEnabled(false);

        connect( game, SIGNAL( finished() ), this, SLOT( on_finished() ) );
        connect( game,&Game::scoreChanged, this, &GameWindow::on_changeScore);

        connect(game,&Game::opengame,this,&GameWindow::createNewGame);
        connect(game,&Game::changeLevel,this,&GameWindow::changelevel );

        setWindowTitle( tr( "The Game" ) );


}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::createMenu()
{
    newGame = new QAction( QIcon( "../image/newgame.png" ), tr( "New Game" ), this );
    exitGame = new QAction( QIcon( "../image/exit.png" ), tr( "Exit" ), this );
    startGame = new QAction( QIcon( "../image/start.png" ), tr( "Start" ), this );
    stopGame = new QAction( QIcon( "../image/stop.png" ), tr( "Stop" ), this );
    changeColor = new QAction( QIcon( "../image/changecolor.png" ), tr( "Change Color" ), this );
    aboutAction = new QAction( QIcon( "../image/about.png" ), tr( "about" ), this );
    gameOne = new QAction( QIcon( "../image/game.png" ), tr( "Game One" ), this );
    gameTwo = new QAction( QIcon( "../image/game.png" ), tr( "Game Two" ), this );
    gameThree = new QAction( QIcon( "../image/game.png" ), tr( "Game Three" ), this );

    QMenu *optionMenu = menuBar()->addMenu( tr( "Option" ) );
    QMenu *controlMenu = menuBar()->addMenu( tr( "Control" ) );
    QMenu *setMenu = menuBar()->addMenu( tr( "Set" ) );
    QMenu *gameMenu = menuBar()->addMenu( tr( "Game" ) );
    QMenu *helpMenu = menuBar()->addMenu( tr( "Help" ) );

    optionMenu->addAction( newGame );
    optionMenu->addAction( exitGame );
    controlMenu->addAction( startGame );
    controlMenu->addAction( stopGame );
    setMenu->addAction( changeColor );
    gameMenu->addAction( gameOne );
    gameMenu->addAction( gameTwo );
    gameMenu->addAction( gameThree );
    helpMenu->addAction( aboutAction );

    connect( startGame, SIGNAL( triggered() ), this, SLOT( on_startGame() ) );
    connect( exitGame, SIGNAL( triggered() ), this, SLOT( close() ) );
    connect( changeColor, SIGNAL( triggered() ), this, SLOT( on_changeColor() ) );
    connect( stopGame, SIGNAL( triggered() ), this, SLOT( on_stopGame() ) );
    connect( newGame, SIGNAL( triggered() ), this, SLOT( on_newGame() ) );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( on_about() ) );
    connect( gameOne, SIGNAL( triggered() ), this, SLOT( on_gameOne() ) );
    connect( gameTwo, SIGNAL( triggered() ), this, SLOT( on_gameTwo() ) );
    connect( gameThree, SIGNAL( triggered() ), this, SLOT( on_gameThree() ) );

    updateActions();
}

void GameWindow::createToolBar()
{
    QToolBar *controlToolBar = addToolBar( tr( "Control" ) );
    controlToolBar->addAction( newGame );
    controlToolBar->addAction( startGame );
    controlToolBar->addAction( stopGame );
    controlToolBar->addAction( changeColor );
    controlToolBar->addSeparator();

    gameLabel = new QLabel( tr( "<h3><font color=blue>Game One</font></h3>" ), this );
    scoreLabel = new QLabel( tr( "<h2><font color=green>Score: 0</font></h2>" ), this );
    totalscore = new QLabel( tr( "<h2><font color=black>totalScore: 0</font></h2>" ), this );

    controlToolBar->addWidget( gameLabel );
    controlToolBar->addSeparator();
    controlToolBar->addWidget( scoreLabel );
    controlToolBar->addSeparator();
    controlToolBar->addWidget( totalscore );
}

void GameWindow::on_finished()
{
    stopGame->setEnabled( false );
    startGame->setEnabled( false );
    changeColor->setEnabled( false );
}
void GameWindow::on_changeScore( int guan,int score )
{
    scoreLabel->setText( tr( "<h2><font color=green>Score: %1</font></h2>" ).arg( score ) );
    if(score>s[guan]) s[guan]=score;
    tscore=s[1]+s[2]+s[3];
    totalscore->setText( tr( "<h2><font color=black>totalScore: %1</font></h2>" ).arg( tscore ) );
}

void GameWindow::on_newGame()
{
    createNewGame( 1 );
}

void GameWindow::createNewGame( int gameNum )
{
    if( gameNum == 1 )
    gameLabel->setText( tr( "<h3><font color=blue>Game One</font></h3>" ) );
    else if( gameNum == 2 )
    gameLabel->setText( tr( "<h3><font color=blue>Game Two</font></h3>" ) );
    else if( gameNum == 3 )
    gameLabel->setText( tr( "<h3><font color=blue>Game Three</font></h3>" ) );
    on_changeScore(gameNum, 0 );

    game->deleteLater();
    game = new Game(gameNum, this );
    setCentralWidget( game );
    if(gameNum==1) game->setBallSpeed( 1 );
    else if(gameNum==2) game->setBallSpeed( 1.2 );
    else game->setBallSpeed( 1.5 );
    game->setPaddleStep( gameNum * 10.0 );
    connect( game, SIGNAL( finished() ), this, SLOT( on_finished() ) );
    connect( game,&Game::scoreChanged, this, &GameWindow::on_changeScore);
    isRunning = false;

    if(level<gameNum) level=gameNum;
    gameTwo->setEnabled(level>=2);
    gameThree->setEnabled(level>=3);

    connect(game,&Game::opengame,this,&GameWindow::createNewGame);
    connect(game,&Game::changeLevel,this,&GameWindow::changelevel );

    changeColor->setEnabled( true );
    updateActions();
}

void GameWindow::changelevel(int a)
{
    if(a>level) level=a;
    if(level>=2) gameTwo->setEnabled(true);
    if(level>=3) gameThree->setEnabled(true);
}

void GameWindow::on_gameOne()
{
    createNewGame( 1 );
}

void GameWindow::on_gameTwo()
{
    createNewGame( 2 );
}


void GameWindow::on_gameThree()
{
    createNewGame( 3 );
}

void GameWindow::on_about()
{
    QMessageBox::about( this, tr( "About" ), tr( "This is a small game for NKU C++ Project!" ) );
}

void GameWindow::updateActions()
{
    startGame->setEnabled( !isRunning );
    stopGame->setEnabled( isRunning );
}

void GameWindow::on_startGame()
{
    game->startGame();
    isRunning = true;
    game->setBallTu(0);
    updateActions();
}

void GameWindow::on_stopGame()
{
    game->stopGame();
    isRunning = false;
    updateActions();
}

void GameWindow::keyPressEvent( QKeyEvent *event )
{
    QString s="";
    switch (event->modifiers()) {

    case Qt::ControlModifier:
        s="Ctrl+";
        break;
    case Qt::AltModifier:
        s="Alt+";
        break;

    }
    switch( event->key() )
    {
        case Qt::Key_Left:
        game->movePaddleLeft();
        break;

        case Qt::Key_Right:
        game->movePaddleRight();
        break;

        case Qt::Key_V:
            s+="V Press";
        break;
    }
    if(s=="Ctrl+V Press")
    {
        game->waigua();
    }
}

void GameWindow::on_changeColor()
{
    game->changeColor();
}

void GameWindow::mouseMoveEvent(QMouseEvent *e)
{
    int len2=game->getlen2();
    int x=e->x();
    if(x>=len2&&x<=width()-len2)
    {
        game->setposition(x,len2);
        update();
    }
}

void GameWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        game->setBallTu(false);
        if(!isRunning) on_startGame();
        else on_stopGame();
        update();
    }
}
