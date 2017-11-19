#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
const int MARGIN = 5;   //边距
const int BLOCK_SIZE = 25;//块大小
const int ROW = 20;         //行数
const int COLUMN = 12;      //列数



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void initButton();      //初始化按钮
    void initGame();        //初始化游戏数据
    void rotateBlock();     //方块旋转
    void copyBlock(int a[4][4], int b[4][4]);

    bool isCollision();     //碰撞检测
    void remove();          //消除行
    void updateColor();     //更新颜色

    void randBlock(int block[4][4]);       //生成随机块


protected:
    void paintEvent(QPaintEvent* event);    //绘制事件
    void keyPressEvent(QKeyEvent* event);   //按键事件

private:

    QPushButton m_startBtn;
    QPushButton m_pauseBtn;
    QPushButton m_quitBtn;
    QPushButton m_rankBtn;

    QTimer m_timer;

    int m_nextBlock[4][4];    //下一个方块
    int m_curBlock[4][4];     //当前方块
    int m_score;        //分数

    int m_offset_x;
    int m_offset_y;


    int xy[ROW][COLUMN];    //存放地图中已经占用的方块

    int m_mark;

    Qt::GlobalColor color1;
    Qt::GlobalColor color2;
private slots:
    void startGame();       //开始游戏
    void pauseGame();       //停止游戏
    void handleTimeout();   //定时器事件




};

#endif // WIDGET_H
