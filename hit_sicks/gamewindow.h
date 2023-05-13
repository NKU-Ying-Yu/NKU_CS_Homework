#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class GameWindow; }
QT_END_NAMESPACE

class QAction;
class QKeyEvent;
class QLabel;

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

public slots:
    void on_changeColor();           //游戏窗口上的一些选项按钮
    void on_newGame();
    void on_startGame();
    void on_stopGame();
    void on_about();
    void on_finished();
    void on_gameOne();
    void on_gameTwo();
    void on_gameThree();
    void on_changeScore( int ,int );
    void createNewGame( int );
    void changelevel(int);

protected:
    void keyPressEvent( QKeyEvent * );
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::GameWindow *ui;
    void createMenu();
        void createToolBar();
        void updateActions();


        QAction *newGame;
        QAction *aboutAction;
        QAction *startGame;
        QAction *stopGame;
        QAction *exitGame;
        QAction *changeColor;
        QAction *gameOne;
        QAction *gameTwo;
        QAction *gameThree;
        QLabel *gameLabel;
        QLabel *scoreLabel;
        QLabel *totalscore;

        Game *game;
        bool isRunning;
        int level=1;

        int tscore=0;
        int s[4]={0,0,0,0};

};
#endif // MAINWINDOW_H
